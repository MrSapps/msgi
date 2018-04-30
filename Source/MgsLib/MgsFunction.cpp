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

static std::set<TVarInfo>& Vars()
{
    static std::set<TVarInfo> v;
    return v;
}

void CheckVars()
{
    TRACE_ENTRYEXIT;

    const auto& vars = Vars();

    for (const auto& varToCheck : vars)
    {
        for (const auto& var : vars)
        {
            if (&varToCheck != &var)
            {
                if (varToCheck.mAddr == var.mAddr)
                {
                    // Var has been defined twice
                    std::stringstream s;
                    s << "Var at addr 0x" << std::hex << varToCheck.mAddr << " (" << varToCheck.mName << ") is defined more than once (" << var.mName << ")";
                    MGS_FATAL(s.str().c_str());
                }

                // TODO: check size of varToCheck within range
                const DWORD varStart = var.mAddr;
                const DWORD varEnd = var.mAddr + var.mSize;

                const DWORD toCheckStart = varToCheck.mAddr;
                const DWORD toCheckEnd = varToCheck.mAddr + varToCheck.mSize;

                if (toCheckStart >= varStart && toCheckEnd < varEnd)
                {
                    // Var overlaps
                    std::stringstream s;
                    s << "Var at addr 0x" << std::hex << varToCheck.mAddr << " (" << varToCheck.mName << ") overlaps other data (" << var.mName << ") offset 0x" <<  std::hex << (std::abs((LONG)(varToCheck.mAddr - var.mAddr)));
                    MGS_FATAL(s.str().c_str());
                }
            }
        }
    }
}

void MgsFunctionBase::ApplyFunctions()
{
    TRACE_ENTRYEXIT;
    LONG err = DetourTransactionBegin();

    if (err != NO_ERROR)
    {
        MGS_FATAL("DetourTransactionBegin failed");
    }

    err = DetourUpdateThread(GetCurrentThread());

    if (err != NO_ERROR)
    {
        MGS_FATAL("DetourUpdateThread failed");
    }

    auto& funcs = GetMgsFunctionTable();
    for (auto func : funcs)
    {
        func.second->Apply();
    }
    err = DetourTransactionCommit();
    if (err != NO_ERROR)
    {
        MGS_FATAL("DetourTransactionCommit failed");
    }
}

// Since the MgsFunctionBase instances are global we have to do this to avoid global static init ordering issues
/*static*/ std::map<void*, MgsFunctionBase*>& MgsFunctionBase::GetMgsFunctionTable()
{
    static std::map<void*, MgsFunctionBase*> table;
    return table;
}

bool operator < (const TVarInfo& lhs, const TVarInfo& rhs)
{
    return lhs.mAddr < rhs.mAddr;
}

MgsVar::MgsVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isPointerType, bool isConstData)
{
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
