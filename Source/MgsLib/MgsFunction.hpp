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

bool IsMgsi();
void DoDetour(DWORD addr, DWORD func);

#define MGS_FATAL(x)  ::MessageBox(NULL, x, "ERROR", MB_ICONERROR | MB_OK); __debugbreak(); abort();

class MgsFunctionBase
{
public:
    MgsFunctionBase() = default;
    virtual ~MgsFunctionBase() = default;
    static void ApplyFunctions();
protected:
    virtual void Apply() = 0;
    static std::map<void*, MgsFunctionBase*>& GetMgsFunctionTable();
};

enum class CallingConvention
{
    eCDecl,
    eStdCall,
    eFastCall
};

enum class HookType
{
    eNotImpl,
    eNotImplWithStub,
    eImpl
};

template <void* kOldAddr, void* kNewAddr, HookType kHookType, CallingConvention convention, class Signature, class ReturnType, class... Args>
class MgsFunctionImpl : public MgsFunctionBase
{
public:
    using TFuncType = Signature*;

    explicit MgsFunctionImpl(const char* fnName)
        : mFnName(fnName)
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it != std::end(GetMgsFunctionTable()))
        {
            // duplicated function
            std::stringstream ss;
            ss << "Duplicated function 0x" << std::hex << kOldAddr << " name " << fnName;
            MGS_FATAL(ss.str().c_str());
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

    bool mStubCalled = false;

    ReturnType operator()(Args ... args)
    {
        if (!IsMgsi())
        {
            if (!mStubCalled)
            {
                if (!mRealFuncPtr)
                {
                    LOG_("WARNING: Unimpl call: " << mFnName);
                }
                mStubCalled = true;
            }

            if (mRealFuncPtr)
            {
                return mRealFuncPtr(args...);
            }

            // Cast handles "return void;" this case is a stub for when
            // calling a real game function outside of the game exe
            return (ReturnType)0;
        }

#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant

        if (kHookType == HookType::eImpl)
        {
            // This hook proc has been called because something called the original function, hence
            // we call the new function (the replacement reimplementation).
            return reinterpret_cast<TFuncType>(kNewAddr)(args...);
        }
        else if (kHookType == HookType::eNotImpl)
        {
            // Just pass through to the "original" function
            return mRealFuncPtr(args...);
        }
        else if (kHookType == HookType::eNotImplWithStub)
        {
            // Hook proc was called because something called the replacement function which is not
            // yet finished, so call the "real" function (i.e the original game function).
            return reinterpret_cast<TFuncType>(kOldAddr)(args...);
        }
        else
        {
            MGS_FATAL("Unknown HookType");
        }
#pragma warning(pop)
    }
    
    Signature* Ptr() const
    {
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
        if (!IsMgsi())
        {
            if (convention == CallingConvention::eCDecl)
            {
                return Cdecl_Static_Hook_Impl;
            }
            else if (convention == CallingConvention::eStdCall)
            {
                return reinterpret_cast<Signature*>(StdCall_Static_Hook_Impl);
            }
            else if (convention == CallingConvention::eFastCall)
            {
                return reinterpret_cast<Signature*>(FastCall_Static_Hook_Impl);
            }
            else
            {
                MGS_FATAL("Not __cdecl, __stdcall or __fastcall");
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
            MGS_FATAL("No function table??");
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
            MGS_FATAL("No function table??");
        }

        auto baseFunc = it->second;
        return static_cast<MgsFunctionImpl*>(baseFunc)->operator()(args...);
    }

    static ReturnType __fastcall FastCall_Static_Hook_Impl(Args ... args)
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it == std::end(GetMgsFunctionTable()))
        {
            // Impossible situation
            MGS_FATAL("No function table??");
        }

        auto baseFunc = it->second;
        return static_cast<MgsFunctionImpl*>(baseFunc)->operator()(args...);
    }
    virtual void Apply() override
    {
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
        switch (kHookType)
        {
        case HookType::eImpl:
            // Force calls from old to new
            std::cout << "Apply impl: old addr " << reinterpret_cast<void*>(kOldAddr) << " new addr " << kNewAddr << std::endl;
            ApplyImpl(reinterpret_cast<void*>(kOldAddr), kNewAddr);
            break;

        case HookType::eNotImpl:
            mRealFuncPtr = (TFuncType)kOldAddr;
            std::cout << "Apply not impl addr " << mRealFuncPtr << std::endl;
            break;

        case HookType::eNotImplWithStub:
            // Force calls from new to old
            std::cout << "Apply not impl: old addr " << kNewAddr  << " new addr " << reinterpret_cast<void*>(kOldAddr) << std::endl;
            ApplyImpl(kNewAddr, reinterpret_cast<void*>(kOldAddr));
            break;

        default:
            MGS_FATAL("Unknown Hook type");
        }
#pragma warning(pop)
    }

private:
    void ApplyImpl(void* funcToHook, void* replacement)
    {
        //TRACE_ENTRYEXIT;
        LONG err = 0;
        mRealFuncPtr = (TFuncType)funcToHook; // Must be set before detouring
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
        if (convention == CallingConvention::eCDecl)
        {
            err = DetourAttach(&(PVOID&)mRealFuncPtr, Cdecl_Static_Hook_Impl);
        }
        else if (convention == CallingConvention::eStdCall)
        {
            err = DetourAttach(&(PVOID&)mRealFuncPtr, StdCall_Static_Hook_Impl);
        }
        else if (convention == CallingConvention::eFastCall)
        {
            err = DetourAttach(&(PVOID&)mRealFuncPtr, FastCall_Static_Hook_Impl);
        }
        else
        {
            MGS_FATAL("Not __cdecl, __stdcall or __fastcall");
        }
#pragma warning(pop)
        if (err != NO_ERROR)
        {
            std::stringstream ss;
            ss << "DetourAttach failed with " << err << " for " << std::hex << " 0x" << funcToHook << " to 0x" << std::hex << replacement;
            MGS_FATAL(ss.str().c_str());
        }
    }

    TFuncType mRealFuncPtr = nullptr;
    const char* mFnName = nullptr;
};

template<void* kOldAddr, void* kNewAddr, HookType kHookType, class ReturnType>
class MgsFunction;

// __cdecl partial specialization
template<void* kOldAddr, void* kNewAddr, HookType kHookType, class ReturnType, class... Args>
class MgsFunction  <kOldAddr, kNewAddr, kHookType, ReturnType __cdecl(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, kHookType, CallingConvention::eCDecl, ReturnType __cdecl(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

// __stdcall partial specialization
template<void* kOldAddr, void* kNewAddr, HookType kHookType, class ReturnType, class ... Args>
class MgsFunction  <kOldAddr, kNewAddr, kHookType, ReturnType __stdcall(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, kHookType, CallingConvention::eStdCall, ReturnType __stdcall(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

// __fastcall partial specialization
template<void* kOldAddr, void* kNewAddr, HookType kHookType, class ReturnType, class ... Args>
class MgsFunction  <kOldAddr, kNewAddr, kHookType, ReturnType __fastcall(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, kHookType, CallingConvention::eFastCall, ReturnType __fastcall(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

class MgsVar
{
public:
    MgsVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isDynamicallyAllocated, bool isConstData);
};

#define MGS_ARY(Redirect, Addr, TypeName, Size, VarName, ...)\
TypeName LocalArray_##VarName[Size]=__VA_ARGS__;\
MgsVar Var_##VarName(#VarName, Addr, sizeof(LocalArray_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName* VarName = (Redirect && IsMgsi()) ? reinterpret_cast<TypeName*>(Addr) : reinterpret_cast<TypeName*>(&LocalArray_##VarName[0]);

// Only use this for pointers to arrays until it can be changed to MGS_ARY (so this is only used when the array size is not yet known)
#define MGS_PTR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalPtr_##VarName = Value;\
MgsVar Var_##VarName(#VarName, Addr, sizeof(LocalPtr_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
std::remove_pointer<TypeName>::type * VarName = (Redirect && IsMgsi()) ? reinterpret_cast<TypeName>(Addr) : LocalPtr_##VarName;

#define MGS_VAR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalVar_##VarName = Value;\
MgsVar Var_##VarName(#VarName, Addr, sizeof(LocalVar_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName& VarName = (Redirect && IsMgsi()) ? *reinterpret_cast<TypeName*>(Addr) : LocalVar_##VarName;

#define MGS_VAR_EXTERN(TypeName, VarName)\
extern TypeName LocalVar_##VarName;\
extern TypeName& VarName;

#define MGS_ARY_EXTERN(TypeName, Size, VarName)\
extern MgsVar Var_##VarName;\
extern TypeName* VarName ;


#define MGS_FUNC_NOT_IMPL(addr, signature, name) MgsFunction<(void*)addr, nullptr, HookType::eNotImpl, signature> name(#name);
#define EXTERN_MGS_FUNC_NOT_IMPL(addr, signature, name) extern MgsFunction<(void*)addr, nullptr, HookType::eNotImpl, signature> name;

#define MGS_FUNC_IMPL(addr, funcName) MgsFunction<(void*)addr, funcName, HookType::eImpl, decltype(funcName)> funcName##_(#funcName);

// isImplemented == false means redirect game func to our func. isImplemented == true means redirect our func to game func.
#define MGS_FUNC_IMPLEX(addr, funcName, isImplemented) MgsFunction<(void*)addr, funcName, isImplemented ? HookType::eImpl : HookType::eNotImplWithStub, decltype(funcName)> funcName##_(#funcName);

#define MGS_REDIRECT(addr, func) DoDetour((DWORD)addr, (DWORD)func)

#define MGS_ASSERT_SIZEOF(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)

#define CC __cdecl
#define MGS_COUNTOF(x) _countof(x)

#define BYTEn(x, n)   (*((BYTE*)&(x)+n))
#define BYTE0(x)   BYTEn(x,  0)
#define BYTE1(x)   BYTEn(x,  1)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR __asm { __asm nop __asm nop __asm nop __asm nop __asm nop }

void CheckVars();


class ScopedDetour
{
public:
    template<class T, class Y>
    ScopedDetour(T src, Y dst)
    {
        auto applyDetour = [this]()
        {
            return DetourAttach(&(PVOID&)mReal, (PVOID)mDst);
        };
        mReal = (PVOID)src;
        mDst = (PVOID)dst;
        DoDetour(applyDetour);
    }

    ~ScopedDetour()
    {
        auto removeDetour = [this]()
        {
            return  DetourDetach(&(PVOID&)mReal, (PVOID)mDst);
        };
        DoDetour(removeDetour);
    }

private:
    template<class T>
    void DoDetour(T fn)
    {
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

        err = fn();
        if (err != NO_ERROR)
        {
            abort();
        }

        err = DetourTransactionCommit();
        if (err != NO_ERROR)
        {
            abort();
        }
    }

    void* mReal;
    void* mDst;
};

#define SCOPED_REDIRECT(real, stub) ScopedDetour real##_scoped(real, stub)
