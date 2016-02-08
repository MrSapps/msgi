#include "stdafx.h"
#include "MgsFunction.hpp"
#include <set>

bool gbIsDll;

// Since the MgsFunctionBase instances are global we have to do this to avoid global static init ordering issues
/*static*/ std::map<DWORD, MgsFunctionBase*>& MgsFunctionBase::GetMgsFunctionTable()
{
    static std::map<DWORD, MgsFunctionBase*> table;
    return table;
}


// TODO: Check for multiply defined vars or overlapping vars
struct TVarInfo
{
    DWORD mAddr;
    DWORD mSize;
};

bool operator < (const TVarInfo& lhs, const TVarInfo& rhs)
{
    return lhs.mAddr < rhs.mAddr;
}

std::set<TVarInfo> gVars;

MgsVar::MgsVar(DWORD addr, DWORD sizeInBytes)
{
    for (const auto& var : gVars)
    {
        if (var.mAddr == addr)
        {
            // Var has been defined twice
            abort();
        }
        else if (var.mAddr >= addr && var.mAddr + var.mSize <= addr)
        {
            // Var overlaps
            abort();
        }
    }
    gVars.insert({ addr, sizeInBytes });
}
