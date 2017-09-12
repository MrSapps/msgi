#pragma once

#include "MgsFunction.hpp"

void ResourceNameHashCpp_ForceLink();
void DoResourceNameHashTest();

WORD CC ResourceNameHash(const char* string);
WORD CC HiTexHash_51D47A(const char* pName);
DWORD CC Hash_40A5C3(const char* pName);
DWORD CC Hash_40A58B(WORD resourceNameHashed, char extensionChar);
