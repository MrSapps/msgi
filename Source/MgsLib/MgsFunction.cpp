#include "stdafx.h"
#include "MgsFunction.hpp"
#include <set>
#include <fstream>

struct TVarInfo
{
    DWORD mAddr;
    DWORD mSize;
    bool mIsPointerType;
    bool mIsConstData;
    const char* mName;
};

// Since the MgsFunctionBase instances are global we have to do this to avoid global static init ordering issues
/*static*/ std::map<DWORD, MgsFunctionBase*>& MgsFunctionBase::GetMgsFunctionTable()
{
    static std::map<DWORD, MgsFunctionBase*> table;
    return table;
}

bool operator < (const TVarInfo& lhs, const TVarInfo& rhs)
{
    return lhs.mAddr < rhs.mAddr;
}

std::set<TVarInfo>& Vars()
{
    static std::set<TVarInfo> v;
    return v;
}


MgsVar::MgsVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isPointerType, bool isConstData)
{

    for (const auto& var : Vars())
    {
        if (var.mAddr == addr)
        {
            // Var has been defined twice
            LOG_ERROR("Var at addr " << var.mAddr << " is defined more than once");
            abort();
        }
        else if (var.mAddr >= addr && var.mAddr + var.mSize <= addr)
        {
            // Var overlaps
            LOG_ERROR("Var at addr " << var.mAddr << " overlaps another var");
            abort();
        }
    }
    Vars().insert({ addr, sizeInBytes, isPointerType, isConstData, name });

}

void DoDetour(DWORD addr, DWORD func)
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

    err = DetourAttach(&(PVOID&)addr, (PVOID)func);
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
