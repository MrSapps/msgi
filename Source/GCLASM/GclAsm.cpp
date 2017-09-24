#include <iostream>
#include <vector>
#include <string>
#include "Script.hpp"
#include "types.hpp"
#include "ResourceNameHash.hpp"

using vec_u8 = std::vector<u8>;

template<class TOnPassword>
static bool DoBruteForce(char* str, int index, int maxDepth, TOnPassword cb)
{
    // Actually there are more JP chars to deal with here - but this will probably get common ones..
    static const char kAlphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    static const int kAlphabetSize = MGS_COUNTOF(kAlphabet) - 1;
    for (int i = 0; i < kAlphabetSize; ++i)
    {
        str[index] = kAlphabet[i];
        if (index == maxDepth - 1)
        {
            if (cb(str))
            {
                return true;
            }
        }
        else
        {
            if (DoBruteForce(str, index + 1, maxDepth, cb))
            {
                return true;
            }
        }
    }
    return false;
}

template<int maxLen, class TOnPassword>
static bool BruteForce(TOnPassword cb)
{
    char buf[maxLen + 1];
    for (int i = 1; i <= maxLen; ++i)
    {
        memset(buf, 0, maxLen + 1);
        if (DoBruteForce(buf, 0, i, cb))
        {
            return false;
        }
    }
    return false;
}

bool IsMgsi()
{
    return false;
}

namespace Utils
{
    inline bool StringStartsWith(const std::string& s1, const std::string& s2)
    {
        return (s1.compare(0, s2.length(), s2) == 0);
    }

    inline vec_u8 FileToByteVector(const char* filename)
    {
        // open the file:
        std::basic_ifstream<u8> file(filename, std::ios::binary);

        // read the data:
        return std::vector<BYTE>((std::istreambuf_iterator<BYTE>(file)),
            std::istreambuf_iterator<u8>());
    }
}

class Arguments
{
public:
    Arguments(int argc, char** argv)
    {
        std::vector<std::string> args;
        for (int i = 0; i < argc; i++)
        {
            args.push_back(argv[i]);
        }
        Parse(args);
    }

    std::string mInputFile;
    std::string mOutputFile;

private:

    void Parse(std::vector<std::string>& args)
    {
        for (const std::string& arg : args)
        {
            if (Utils::StringStartsWith(arg, "-input"))
            {
                mInputFile = arg;
            }
        }
    }
};

void DisAsmProc(u8* pScript);

int main(int argc, char** argv)
{
    std::cout << "GCL ASM" << std::endl;

    /*
    BruteForce<12>([](const char* text) 
    {
        if (ResourceNameHash(text) == 0xE253)
        {
            std::cout << "MATCH for : " << text << std::endl;
            return false;
        }
        return false;
    });
    */


    Arguments arguments(argc, argv);

    // TODO: Hard coded for now - will make everything work nice once tool is useful

   // arguments.mInputFile = "C:\\Program Files (x86)\\Metal Gear Solid\\stage\\init\\scenerio.gcx";
    arguments.mInputFile = "C:\\Program Files (x86)\\Metal Gear Solid\\stage\\s04a\\scenerio.gcx";

    
    std::cout << "Input: " << arguments.mInputFile << std::endl;

    vec_u8 buffer = Utils::FileToByteVector(arguments.mInputFile.c_str());

    // Byte swap proc table
    u8* afterProcTable = Script_InitProcTables_sub_409C87(buffer.data());

    GCL_ProcInfo* procInfos = reinterpret_cast<GCL_ProcInfo*>(buffer.data());
    int procCount = 0;
    for (;;)
    {
        if (procInfos->mId == 0 && procInfos->mOffset == 0)
        {
            break;
        }
        procInfos++;
        procCount++;
    }
    procInfos = reinterpret_cast<GCL_ProcInfo*>(buffer.data());

    std::cout << "GCX has " << procCount << " procs" << std::endl;

    for (int i = 0; i < procCount; i++)
    {
        std::cout << "Proc ID: 0x" << std::hex << procInfos[i].mId << " offset: 0x" << std::hex << procInfos[i].mOffset << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < procCount; i++)
    {
        std::cout << "PROC(0x" << std::hex << procInfos[i].mId << ")" << std::endl;
        DisAsmProc(afterProcTable + procInfos[i].mOffset);
        std::cout << "END_PROC()" << std::endl << std::endl;
    }

    return 0;
}

u8* ReadGCL(u8* pScript, int len);

/*
[60] - built in command
[00 12] - length (18)
[64 C0] - command id - EVAL
[0D] - length (13)
[30] - the unknown block
[0B] - length (11)
[14] - read vars
[02 00 2D 02] 
[00] - end
[31] - exit read vars
[14] - read vars
[31] - exit read vars
[00] - end
[00] - alignment


60 00 13 64 C0 0E 30 0C 16 00 
00 40 06 AD BF 31 14 31 00 00 60 00 12 64 C0 0D 30 0B 14 03 00 2D 02 00
31 14 31 00 00 60 00 12 64 C0 0D 30 0B 14 04 00 2D 02 00 31 14 31 00 00 
60 00 12 64 C0 0D 30 0B 12 00 00 19 02 07 31 14 31 00 00 00 00 00 00 23 
40 00 22 60 00 0F 9A 1F 01 50 73 01 50 66 01 50 6D 01 00 70 04 65 D5 00 
70 04 D0 BB 00 70 04 DF 2A 00 00 00 00 00 00

*/

u8* HandleBuiltInCommand(u8* pScript, WORD cmdId, WORD cmdLen)
{
    switch (cmdId)
    {
    case 0x0d86:
        std::cout << "IF(";
        break;

    case 0xC8BB:
        std::cout << "LOAD(";
        break;

    case 0x9906:
        std::cout << "CHARA(";
        break;

    case 0x64C0:
        std::cout << "EVAL(";
        break;

    default:
        std::cout << "CMD_UNKNOWN(0x" << std::hex << cmdId << ")" << std::endl;
        return nullptr;
    }

    u8* pEnd = pScript + cmdLen;
    pScript = pScript + 1;
    do
    {
        pScript = ReadGCL(pScript, cmdLen);
    } while (pScript != pEnd);
    std::cout << ")" << std::endl;

    return pScript;
}

void DisAsmProc(u8* pScript)
{
    for (;;)
    {
        switch (*pScript)
        {
        case 0:
            std::cout << "END" << std::endl;
            return;

        // TODO: Only 30, 60 and 70 should be at the top level statements?

        case 0x70: // proc call
        {
            pScript++;
            const int length = *pScript;
            const WORD id = ToWORD(pScript+1);

            // TODO: Pull args
            std::cout << "CALL(" << id << ")" << std::endl;

            pScript = pScript + length;
        }
        break;

        case 0x60:
        {
            const int length = ToWORD(pScript + 1);
            const WORD cmdId = ToWORD(pScript + 3);
            u8* pRet = HandleBuiltInCommand(&pScript[5], cmdId, length - 4);
            if (pRet)
            {
                pScript = pRet;
            }
            else
            {
                pScript = pScript + length + 1;
            }
        }
        break;

        case 0x40:
        {
            WORD jumpLocation = ToWORD(pScript + 1);
            pScript += 3;

            std::cout << "JUMP_BY(0x" << std::hex << jumpLocation << ")" << std::endl;
        }
        break;

        default:
            std::cout << "Unknown code 0x" << std::hex << static_cast<int>(*pScript) << std::endl;
            return;
        }
    }
}

struct ScriptCtx
{
    DWORD m2;
    DWORD m3;
    DWORD m4;
};

void HandleScript_Unknown6(u8* pScript, int len)
{
    ScriptCtx stack[99] = {};
    ScriptCtx* pCtx = &stack[0];

    if (*pScript == 0)
    {
        std::cout << "UNKNOWN6_END_1()" << std::endl;
        return;
    }

    for (;;)
    {
        for (;;)
        {
            if (*pScript == 0x31)
            {
                break;
            }
            pScript = ReadGCL(pScript, len);
        }

        pScript++;

        if (*pScript == 0)
        {
            std::cout << "UNKNOWN6_END_2()" << std::endl;
            return;
        }

        if (*pScript == 0x14)
        {
            std::cout << "VAR_WRITE()" << std::endl;
            //throw std::runtime_error("Impl me");
            //Script_VarWrite_40957B(pCtx->m3, pCtx->m2);
        }
        else
        {
            std::cout << "LOGIC_OP()" << std::endl;
            //throw std::runtime_error("Impl me");
            //pCtx->m4 = Script_Operator_Evaluate(pScript[1], pCtx->m4, pCtx->m2);
            //pCtx->m3 = 0;
        }
        pScript++;
    }
}

static u8* ReadVar(BYTE* pScript)
{
    return pScript = pScript + 4;
}

u8* ReadGCL(u8* pScript, int len)
{
    u8* pEnd = pScript + len;
    //while (pScript != pEnd)
    {
        if ((*pScript & 0xF0) == 16)
        {
            pScript = ReadVar(pScript);
        }
        else
        {
            switch (*pScript)
            {
            case 0x70: // proc call TODO: Copy paste from above - not sure if calls should be allowed at this point ??
            {
                pScript++;
                const int length = *pScript;
                const WORD id = ToWORD(pScript + 1);

                // TODO: Pull args
                std::cout << "CALL(" << id << ")" << std::endl;

                pScript = pScript + length;
            }
            break;
            // TODO: Copy paste from above - not sure if commands should be allowed at this point ??
            case 0x60:
            {
                const int length = ToWORD(pScript + 1);
                const WORD cmdId = ToWORD(pScript + 3);
                u8* pRet = HandleBuiltInCommand(&pScript[5], cmdId, length - 4);
                if (pRet)
                {
                    pScript = pRet;
                }
                else
                {
                    pScript = pScript + length + 1;
                }
            }
            break;

            // Read stack argument
            case 0x20:
            {
                std::cout << "READ_STACK(" << static_cast<WORD>(pScript[1]) << ")" << std::endl;
                pScript = pScript + 2;
            }
            break;

            // ??
            case 0x30:
            {
                HandleScript_Unknown6(pScript + 2, pScript[1]);
                pScript += *(pScript + 1);
            }
            break;

            // Jump
            case 0x40:
            {
                WORD jumpLocation = ToWORD(pScript + 1);
                pScript += 3;
                std::cout << "JUMP_BY(0x" << std::hex << jumpLocation << ")" << std::endl;
            }
            break;

            // Parameter
            case 0x50:
            {
                char id = pScript[1];
                std::cout << "PARAM(" << std::string(1, id) << ")" << std::endl;
                //DWORD paramTypeAndId = *pScriptByte1 << 16;
                //paramTypeAndId |= 0x50;
                //*ppScript = paramTypeAndId;
               // *pRet = (DWORD)(pScript + 3);
                //pScriptByte1 += pScript[2] + 1;

                pScript = pScript + 3;
            }
            break;

            // End
            case 0:
            {
                //pScriptByte1 = 0;

                return pScript + 1;
            }
            break;

            // Read s16
            case 1:
            {
                WORD w = ToWORD(&pScript[1]);
                std::cout << "READ_S16(0x" << std::hex << (u16)w << ")" << std::endl;
                pScript = pScript + 3;
            }
            break;

            // Read u8
            case 2:
            case 3:
            case 4:
            {
                std::cout << "READ_U8(0x" << std::hex << static_cast<WORD>(pScript[1]) << ")" << std::endl;
                pScript = pScript + 2;
            }
            break;

            // Read string
            case 7:
            {
                char* pStr = (char*)&pScript[2];
                std::cout << "READ_STRING(" << pStr << ")" << std::endl;
                pScript += *(pScript + 1) + 2;
            }
            break;

            // Read u16
            case 6:
            case 8:
            {
                WORD w = ToWORD(&pScript[1]);
                std::cout << "READ_U16(0x" << std::hex << w << ")" << std::endl;
                pScript = pScript + 3;
            }
            break;

            // Read u32
            case 9:
            case 10:
            {
                DWORD d = ToDWORD(&pScript[1]);
                std::cout << "READ_U32(0x" << std::hex << d << ")" << std::endl;
                pScript = pScript + 5;
            }
            break;

            default:
                std::cout << "Unknown sub code 0x" << std::hex << static_cast<int>(*pScript) << std::endl;
                return pScript;
            }
        }
    }
    return pScript;
}