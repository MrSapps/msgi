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

static void TestHash_40A5C3()
{
    ASSERT_EQ(0xFF9F4555, Hash_40A5C3("tga/Blah"));
    ASSERT_EQ(0x00134555, Hash_40A5C3("tga/Blah.tga"));
    ASSERT_EQ(0xFFE4BCAA, Hash_40A5C3("Blah.EXE"));
    ASSERT_EQ(0xFF9FBCAA, Hash_40A5C3("Blah"));
    ASSERT_EQ(0xFF9F72D5, Hash_40A5C3("Ñ¦Ñ-í+Ñ»"));
}

static void TestHash_40A58B()
{
    ASSERT_EQ(0x13BCAA, HashFileName_40A58B(ResourceNameHash("Blah"), 't'));
}

static void TestHash_40A5A2()
{
    ASSERT_EQ(0x13BCAA, HashFileName_40A5A2("Blah", 't'));
}

void DoResourceNameHashTest()
{
    TestResourceNameHash();
    TestHiTexHash_51D47A();
    TestHash_40A5C3();
    TestHash_40A58B();
    TestHash_40A5A2();
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

DWORD CC Hash_40A5C3(const char* pName)
{
    size_t len = strlen(pName);
    int firstExtensionCharOrZero = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (pName[i] == '.')
        {
            len = i;
            firstExtensionCharOrZero = pName[len + 1];
            break;
        }
    }

    firstExtensionCharOrZero -= 0x61;
    if (firstExtensionCharOrZero >= 0x1A)
    {
        firstExtensionCharOrZero += 0x20;
    }
    return (firstExtensionCharOrZero << 16) + HashInternal(reinterpret_cast<const BYTE*>(pName), len);
}
MGS_FUNC_IMPLEX(0x0040A5C3, Hash_40A5C3, RESOURCENAMEHASH_IMPL);

DWORD CC HashFileName_40A58B(DWORD resourceNameHashed, char extensionChar)
{
    int extHash = extensionChar;
    extHash = extHash - 0x61;
    if (extHash >= 0x1A)
    {
        extHash += 0x20;
    }
    return resourceNameHashed + (extHash << 16);
}
MGS_FUNC_IMPLEX(0x0040A58B, HashFileName_40A58B, RESOURCENAMEHASH_IMPL);


DWORD CC HashFileName_40A5A2(const char* pName, char extensionChar)
{
    return HashFileName_40A58B(ResourceNameHash(pName), extensionChar);
}
MGS_FUNC_IMPLEX(0x0040A5A2, HashFileName_40A5A2, RESOURCENAMEHASH_IMPL);
