#pragma once

#include <windows.h>
#include <ostream>
#include <map>
#include <memory>
#include "logger.hpp"
#include "detours.h"

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
inline void doPrint(std::ostream& out)
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

extern bool gbIsDll;


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

template <DWORD kOldAddr, void* kNewAddr, bool kLogArgs, CallingConvention convention, class Signature, class ReturnType, class... Args>
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
        if (kLogArgs)
        {
            std::cout << mFnName << " (";
            doPrint(std::cout, args...);
            std::cout << ")" << std::endl;
        }

        if (kNewAddr)
        {
            // Call "newAddr" since we've replaced the function completely
            return reinterpret_cast<TFuncType>(kNewAddr)(args...);
        }
        else
        {
            // Call "mRealFuncPtr" here so that we are calling the "real" function

            // If not running within the game then we can't call real so just return
            // a default R and log params
            return mRealFuncPtr(args...);
        }
    }

    Signature* Ptr() const
    {
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
        TRACE_ENTRYEXIT;

        std::cout << "old addr " << kOldAddr << " new addr " << kNewAddr << std::endl;

        mRealFuncPtr = (TFuncType)kOldAddr;

        LONG err = 0;

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

        if (err != NO_ERROR)
        {
            abort();
        }
    }

private:
    TFuncType mRealFuncPtr = nullptr;
    const char* mFnName = nullptr;
};

template<DWORD kOldAddr, void* kNewAddr, bool kLogArgs, class ReturnType>
class MgsFunction;

// __cdecl partial specialization
template<DWORD kOldAddr, bool kLogArgs, void* kNewAddr, class ReturnType, class... Args>
class MgsFunction    <kOldAddr, kNewAddr, kLogArgs, ReturnType __cdecl(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, kLogArgs, eCDecl, ReturnType __cdecl(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

// __stdcall partial specialization
template<DWORD kOldAddr, void* kNewAddr, bool kLogArgs, class ReturnType, class ... Args>
class MgsFunction    <kOldAddr, kNewAddr, kLogArgs, ReturnType __stdcall(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, kLogArgs, eStdCall, ReturnType __stdcall(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

#define MSG_FUNC_NOT_IMPL(addr, signature, name) MgsFunction<addr, nullptr, true, signature> name(#name);
#define MSG_FUNC_NOT_IMPL_NOLOG(addr, signature, name) MgsFunction<addr, nullptr, false, signature> name(#name);
#define MSG_FUNC_IMPL(addr, funcName) MgsFunction<addr, funcName, true, decltype(funcName)> funcName##_(#funcName);
