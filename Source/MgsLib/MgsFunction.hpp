#pragma once

#include <windows.h>
#include <ostream>
#include <map>
#include <memory>

inline std::ostream& operator<<(std::ostream& out, IID id)
{
    // TODO: Print GUIDS properly
    out << static_cast<unsigned int>(id.Data1);
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
protected:
    static std::map<DWORD, MgsFunctionBase*>& GetMgsFunctionTable();

};

template <DWORD kOldAddr, void* kNewAddr, class Signature, class ReturnType, class... Args>
class MgsFunctionImpl : public MgsFunctionBase
{
public:
    using TFuncType = Signature*;

    MgsFunctionImpl(const char* fnName)
        : mFnName(fnName)
    {
        mRealFuncPtr = (TFuncType)kOldAddr;

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

        std::cout << "old addr " << kOldAddr << " new addr " << kNewAddr << std::endl;

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        if (kNewAddr)
        {
            // Hook oldAddr to point to newAddr
            DetourAttach(&(PVOID&)mRealFuncPtr, Static_Hook_Impl);
        }
        else
        {
            // point oldAddr to Static_Hook_Impl
            DetourAttach(&(PVOID&)mRealFuncPtr, (TFuncType)kNewAddr);
        }
        const auto error = DetourTransactionCommit();
        if (error != NO_ERROR)
        {
            abort();
        }
    }

    static ReturnType Static_Hook_Impl(Args ... args)
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it == std::end(GetMgsFunctionTable()))
        {
            // Impossible situation
        }


        auto baseFunc = it->second;
        // dont know if this will work or just blow up..
        return static_cast<MgsFunctionImpl*>(baseFunc)->operator()(args...);
    }

    ~MgsFunctionImpl()
    {
        auto it = GetMgsFunctionTable().find(kOldAddr);
        if (it != std::end(GetMgsFunctionTable()))
        {
            GetMgsFunctionTable().erase(it);
        }
    }

    ReturnType operator()(Args ... args)
    {
        doPrint(std::cout, args...);

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

private:
    TFuncType mRealFuncPtr = nullptr;
    const char* mFnName = nullptr;
};

template<DWORD kOldAddr, void* kNewAddr, class ReturnType>
class MgsFunction;

// __cdecl partial specialization
template<DWORD kOldAddr, void* kNewAddr, class ReturnType, class... Args>
class MgsFunction    <kOldAddr, kNewAddr, ReturnType __cdecl(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, ReturnType __cdecl(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

// __stdcall partial specialization
template<DWORD kOldAddr, void* kNewAddr, class ReturnType, class ... Args>
class MgsFunction    <kOldAddr, kNewAddr, ReturnType __stdcall(Args...) > : public
    MgsFunctionImpl<kOldAddr, kNewAddr, ReturnType __stdcall(Args...), ReturnType, Args...>
{
public:
    MgsFunction(const char* name) : MgsFunctionImpl(name) { }
};

#define MSG_FUNC_NOT_IMPL(addr, signature, name) MgsFunction<addr, nullptr, signature> name(#name);
#define MSG_FUNC_IMPL(addr, funcName) MgsFunction<addr, funcName, decltype(funcName)> funcName##_(#funcName);
