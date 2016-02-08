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
MgsVar::MgsVar(DWORD addr)
{

}

MgsVar::MgsVar(DWORD addr, DWORD sizeInBytes)
{

}

