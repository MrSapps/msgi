#pragma once

#include <windows.h>
#include <ostream>
#include <map>
#include <memory>
#include <fstream>
#include <set>
#include <vector>
#include <type_traits>
#include "logger.hpp"
#include "detours.h"

// Turn off logging unless required, while using a Console Window its much too
// slow. Need to use something like ImGui and log the output in to the game 
// rendering itself to be fast enough.
#define ENABLE_LOGGING 0

bool IsMgsi();

inline std::ostream& operator<<(std::ostream& out, IID id)
{
    // TODO: Print GUIDS properly
    out << static_cast<unsigned int>(id.Data1);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const char* ptr)
{
    if (ptr)
    {
        out.write(ptr, strlen(ptr));
    }
    else
    {
        out << "(null char*)";
    }
    return out;
}

// No arguments case
inline void doPrint(std::ostream& /*out*/)
{

}

template <typename T>
void doPrint(std::ostream& out, T t)
{
    out << t;
}

template <typename T, typename U, typename... Args>
void doPrint(std::ostream& out, T t, U u, Args... args)
{
    out << t << ',';
    doPrint(out, u, args...);
}

class MgsFunctionBase
{
public:
    MgsFunctionBase() = default;
    virtual ~MgsFunctionBase() = default;
    static void ApplyFunctions()
    {
        TRACE_ENTRYEXIT;
        LONG err = DetourTransactionBegin();
        
        if (err != NO_ERROR)
        {
            abort();
        }
        
        err = DetourUpdateThread(GetCurrentThread());
        
        if (err != NO_ERROR)
        {
            abort();
        }

        auto& funcs = GetMgsFunctionTable();
        for (auto func : funcs)
        {
            func.second->Apply();
        }
        err = DetourTransactionCommit();
        if (err != NO_ERROR)
        {
            abort();
        }
    }
protected:
    virtual void Apply() = 0;
    static std::map<DWORD, MgsFunctionBase*>& GetMgsFunctionTable();
};

enum CallingConvention
{
    eCDecl,
    eStdCall
};

template <DWORD kOldAddr, void* kNewAddr, bool kReverseHook, bool kLogArgs, CallingConvention convention, class Signature, class ReturnType, class... Args>
class MgsFunctionImpl : public MgsFunctionBase
{
public:
    using TFuncType = Signature*;

    MgsFunctionImpl(const char* fnName)
        : mFnName(fnName)
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it != std::end(GetMgsFunctionTable()))
        {
            // duplicated function
            abort();
        }
        else
        {
            GetMgsFunctionTable().insert(std::make_pair(kOldAddr, this));
        }
    }

    virtual ~MgsFunctionImpl()
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it != std::end(GetMgsFunctionTable()))
        {
            GetMgsFunctionTable().erase(it);
        }
    }


    ReturnType operator()(Args ... args)
    {
#if ENABLE_LOGGING
        if (kLogArgs)
        {
            if (!IsMgsi() && !mRealFuncPtr)
            {
                std::cout << "WARNING: Unimpl call: " << mFnName << std::endl;
            }
            std::cout << mFnName << " (";
            doPrint(std::cout, args...);
            std::cout << ")" << std::endl;
        }
#endif

#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
        if (kNewAddr && !kReverseHook)
        {
            // Call "newAddr" since we've replaced the function completely
            return reinterpret_cast<TFuncType>(kNewAddr)(args...);
        }
        else if (kOldAddr && kReverseHook)
        {
            return reinterpret_cast<TFuncType>(kOldAddr)(args...);
        }
        else
        {
            if (!IsMgsi())
            {
                // Cast handles "return void;" this case is a stub for when
                // calling a real game function outside of the game exe
                return (ReturnType)0;
            }
            // Call "mRealFuncPtr" here so that we are calling the "real" function

            // If not running within the game then we can't call real so just return
            // a default R and log params
            return mRealFuncPtr(args...);
        }
#pragma warning(pop)


    }
    
    Signature* Ptr() const
    {
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
        if (!IsMgsi())
        {
            if (convention == eCDecl)
            {
                return Cdecl_Static_Hook_Impl;
            }
            else if (convention == eStdCall)
            {
                return reinterpret_cast<Signature*>(StdCall_Static_Hook_Impl);
            }
            else
            {
                abort();
            }
        }
#pragma warning(pop)
        return mRealFuncPtr;
    }

protected:
    static ReturnType __cdecl Cdecl_Static_Hook_Impl(Args ... args)
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it == std::end(GetMgsFunctionTable()))
        {
            // Impossible situation
            abort();
        }

        auto baseFunc = it->second;
        return static_cast<MgsFunctionImpl*>(baseFunc)->operator()(args...);
    }

    static ReturnType __stdcall StdCall_Static_Hook_Impl(Args ... args)
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it == std::end(GetMgsFunctionTable()))
        {
            // Impossible situation
            abort();
        }

        auto baseFunc = it->second;
        return static_cast<MgsFunctionImpl*>(baseFunc)->operator()(args...);
    }

    virtual void Apply() override
    {
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
        if (kReverseHook)
        {
            // Redirect calls to our reimpl to the game function
            ApplyImpl(kNewAddr, reinterpret_cast<void*>(kOldAddr));
        }
        else
        {
            // Redirect internal game function to our reimpl
            ApplyImpl(reinterpret_cast<void*>(kOldAddr), kNewAddr);
        }
#pragma warning(pop)
    }

private:
    void ApplyImpl(void* funcToHook, void* replacement)
    {
        TRACE_ENTRYEXIT;

        std::cout << "old addr " << funcToHook << " new addr " << replacement << std::endl;

        mRealFuncPtr = (TFuncType)funcToHook;

        LONG err = 0;
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
        if (convention == eCDecl)
        {
            err = DetourAttach(&(PVOID&)mRealFuncPtr, Cdecl_Static_Hook_Impl);
        }
        else if (convention == eStdCall)
        {
            err = DetourAttach(&(PVOID&)mRealFuncPtr, StdCall_Static_Hook_Impl);
        }
        else
        {
            abort();
        }
#pragma warning(pop)
        if (err != NO_ERROR)
        {
            abort();
        }
    }

    TFuncType mRealFuncPtr = nullptr;
    const char* mFnName = nullptr;
    bool mPassThrough = false;
};

template<DWORD kOldAddr, void* kNewAddr, bool kReverseHook, bool kLogArgs, class ReturnType>
class MgsFunction;

// __cdecl partial specialization
template<DWORD kOldAddr, bool kLogArgs, bool kReverseHook, void* kNewAddr, class ReturnType, class... Args>
class MgsFunction    <kOldAddr, kNewAddr, kReverseHook, kLogArgs, ReturnType __cdecl(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, kReverseHook, kLogArgs, eCDecl, ReturnType __cdecl(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

// __stdcall partial specialization
template<DWORD kOldAddr, void* kNewAddr, bool kReverseHook, bool kLogArgs, class ReturnType, class ... Args>
class MgsFunction    <kOldAddr, kNewAddr, kReverseHook, kLogArgs, ReturnType __stdcall(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, kReverseHook, kLogArgs, eStdCall, ReturnType __stdcall(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

class AutoCs
{
public:
    AutoCs(const AutoCs&) = delete;
    AutoCs& operator = (const AutoCs&) = delete;
    AutoCs()
    {
        ::InitializeCriticalSection(&mCs);
    }

    ~AutoCs()
    {
        ::DeleteCriticalSection(&mCs);
    }

    void Lock()
    {
        ::EnterCriticalSection(&mCs);
    }

    void UnLock()
    {
        ::LeaveCriticalSection(&mCs);
    }
private:
    CRITICAL_SECTION mCs;
};

class AutoCsLock
{
public:
    AutoCsLock(const AutoCsLock&) = delete;
    AutoCsLock& operator = (const AutoCsLock&) = delete;
    explicit AutoCsLock(AutoCs& cs)
        : mCs(cs)
    {
        mCs.Lock();
    }

    ~AutoCsLock()
    {
        mCs.UnLock();
    }
private:
    AutoCs& mCs;
};

class BaseVarSnapShot
{
public:
    BaseVarSnapShot(const char* name, DWORD addr) : mName(name), mAddr(addr) { }
    virtual ~BaseVarSnapShot() = default;
    virtual void Restore() = 0;
    DWORD Address() const { return mAddr; }
protected:
    static void WriteVec(const char* fileName, const std::vector<BYTE>& vec)
    {
        std::ofstream fout(fileName, std::ios::out | std::ios::binary);
        fout.write((char*)vec.data(), vec.size());
        fout.close();
    }

    const char* mName = nullptr;
    DWORD mAddr = 0;
};

class StaticVarSnapShot : public BaseVarSnapShot
{
public:
    StaticVarSnapShot(const char* name, DWORD addr, DWORD size) : BaseVarSnapShot(name, addr), mSize(size) { SnapShot(); }
    virtual void Restore() override;
    bool operator == (const StaticVarSnapShot& other);
    bool operator != (const StaticVarSnapShot& other)
    {
        return !(*this == other);
    }
private:
    void SnapShot();
    DWORD mSize = 0;
    std::vector<BYTE> mValue;
};

// TODO: Could be improved to "delay" free()'s while a snapshot exists
// this way the old pointer could be safely restored if it was free()'ed since the
// snapshot was taken. This can be added if/when its needed.
class DynamicVarSnapShot : public BaseVarSnapShot
{
public:
    DynamicVarSnapShot(const char* name, DWORD addr) : BaseVarSnapShot(name, addr) { SnapShot(); }
    virtual void Restore() override;
    bool operator == (const DynamicVarSnapShot& other);
    bool operator != (const DynamicVarSnapShot& other)
    {
        return !(*this == other);
    }
private:
    void SnapShot();
    DWORD mPtrValue = 0;
    std::vector<BYTE> mBufferCopy;
};

class MgsVar
{
public:
    // Debugging aid to check real game function VS re-implementation does the same thing
    // won't always work since if the function deleted a file then this can't restore it.
    // Or if the global state of a direct3D device was changed then again it won't be restored.
    // Memory allocation can be a problem too since things will take copies of pointers in locals or currently
    // untracked vars.
    // But in a lot of cases this is can be useful.
    class SnapShot
    {
    public:
        SnapShot();
        SnapShot(const SnapShot&) = delete;
        void AssertEqual(const SnapShot& other);
        void Restore();
    private:
        std::map<DWORD, std::unique_ptr<BaseVarSnapShot>> mVars;
    };
    MgsVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isDynamicallyAllocated, bool isConstData);
    static std::unique_ptr<SnapShot> MakeSnapShot();
    static void TrackAlloc(void* ptr, size_t size);
    static void TrackFree(void* ptr);
private:
    static AutoCs mCs;
    static std::map<void*, size_t> mAllocs;
    friend class DynamicVarSnapShot;
    friend class StaticVarSnapShot;
};

#define MGS_ARY(Redirect, Addr, TypeName, Size, VarName, ...)\
TypeName LocalArray_##VarName[Size]=__VA_ARGS__;\
MgsVar Var_##VarName(#VarName, Addr, sizeof(LocalArray_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName* VarName = (Redirect && IsMgsi()) ? reinterpret_cast<TypeName*>(Addr) : reinterpret_cast<TypeName*>(&LocalArray_##VarName[0]);

// Only use this for pointers to arrays until it can be changed to MGS_ARY (so this is only used when the array size is not yet known)
#define MGS_PTR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalPtr_##VarName = Value;\
MgsVar Var_##VarName(#VarName, Addr, sizeof(LocalPtr_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
std::remove_pointer<TypeName>::type * const VarName = (Redirect && IsMgsi()) ? reinterpret_cast<TypeName>(Addr) : LocalPtr_##VarName;

#define MGS_VAR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalVar_##VarName = Value;\
MgsVar Var_##VarName(#VarName, Addr, sizeof(LocalVar_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName& VarName = (Redirect && IsMgsi()) ? *reinterpret_cast<TypeName*>(Addr) : LocalVar_##VarName;

#define MGS_VAR_EXTERN(TypeName, VarName)\
extern TypeName LocalVar_##VarName;\
extern TypeName& VarName;

#define MSG_FUNC_NOT_IMPL(addr, signature, name) MgsFunction<addr, nullptr, false, true, signature> name(#name);
#define EXTERN_MSG_FUNC_NOT_IMPL(addr, signature, name) extern MgsFunction<addr, nullptr, false, true, signature> name;
#define MSG_FUNC_NOT_IMPL_NOLOG(addr, signature, name) MgsFunction<addr, nullptr, false, false, signature> name(#name);
#define MSG_FUNC_IMPL(addr, funcName) MgsFunction<addr, funcName, false, true, decltype(funcName)> funcName##_(#funcName);

// isImplemented == false means redirect game func to our func. isImplemented == true means redirect our func to game func.
#define MSG_FUNC_IMPLEX(addr, funcName, isImplemented) MgsFunction<addr, funcName, !isImplemented, true, decltype(funcName)> funcName##_(#funcName);
#define MSG_FUNC_IMPL_NOLOG(addr, funcName) MgsFunction<addr, funcName, false, false, decltype(funcName)> funcName##_(#funcName);

#define MSG_ASSERT_SIZEOF(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)

#define CC __cdecl
#define MSG_COUNTOF(x) _countof(x)

#define BYTEn(x, n)   (*((BYTE*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
