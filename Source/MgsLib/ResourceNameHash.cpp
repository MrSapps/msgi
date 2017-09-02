#include "stdafx.h"
#include "ResourceNameHash.hpp"
#include <gmock/gmock.h>

#define RESOURCENAMEHASH_IMPL true

void ResourceNameHashCpp_ForceLink() {} 

void DoResourceNameHashTest()
{
    ASSERT_EQ(0xBCAA, ResourceNameHash("Blah"));
    ASSERT_EQ(0x72D5, ResourceNameHash("Ñ¦Ñ-í+Ñ»"));
}

WORD CC ResourceNameHash(const char* pName)
{
    WORD hash = 0;
    if (*pName)
    {
        do
        {
            hash = ((hash << 0x05) | (hash >> 0x0B)) + *reinterpret_cast<const BYTE*>(pName);
            pName++;
        } while (*pName);
    }
    return hash;
}
MSG_FUNC_IMPLEX(0x0040B38E, ResourceNameHash, RESOURCENAMEHASH_IMPL);
