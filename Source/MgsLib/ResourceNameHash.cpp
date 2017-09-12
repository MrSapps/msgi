#include "stdafx.h"
#include "ResourceNameHash.hpp"
#include <gmock/gmock.h>

#define RESOURCENAMEHASH_IMPL true

void ResourceNameHashCpp_ForceLink() {} 

static void TestResourceNameHash()
{
    ASSERT_EQ(0xBCAA, ResourceNameHash("Blah"));
    ASSERT_EQ(0x72D5, ResourceNameHash("Ñ¦Ñ-í+Ñ»"));
}

static void TestHiTexHash_51D47A()
{
    ASSERT_EQ(0xBCAA, HiTexHash_51D47A("Blah"));
    ASSERT_EQ(0xBCAA, HiTexHash_51D47A("tga/Blah.tga"));
    ASSERT_EQ(0x72D5, HiTexHash_51D47A("LOLS/Ñ¦Ñ-í+Ñ».MEH"));
}

void DoResourceNameHashTest()
{
    TestResourceNameHash();
    TestHiTexHash_51D47A();
}

static WORD HashInternal(const BYTE* buffer, size_t len)
{
    WORD hash = 0;
    for (size_t i = 0; i < len; i++)
    {
        hash = ((hash << 0x05) | (hash >> 0x0B)) + buffer[i];
    }
    return hash;
}

WORD CC ResourceNameHash(const char* pName)
{
    if (pName)
    {
        return HashInternal(reinterpret_cast<const BYTE*>(pName), strlen(pName));
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0040B38E, ResourceNameHash, RESOURCENAMEHASH_IMPL);

WORD CC HiTexHash_51D47A(const char* pName)
{
    const char* fileNamePart = strchr(pName, '/');
    if (fileNamePart)
    {
        fileNamePart++;
    }
    else
    {
        fileNamePart = pName;
    }

    if (fileNamePart)
    {
        const size_t len = strlen(fileNamePart);
        for (size_t i = 0; i < len; i++)
        {
            if (fileNamePart[i] == '.')
            {
                return HashInternal(reinterpret_cast<const BYTE*>(fileNamePart), i);
            }
        }
        return HashInternal(reinterpret_cast<const BYTE*>(fileNamePart), len);
    }

    return 0;
}
MGS_FUNC_IMPLEX(0x0051D47A, HiTexHash_51D47A, RESOURCENAMEHASH_IMPL);
