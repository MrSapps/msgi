#include <iostream>
#include <vector>
#include <string>
#include "Script.hpp"
#include "types.hpp"

using vec_u8 = std::vector<u8>;

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

    Arguments arguments(argc, argv);

    // TODO: Hard coded for now - will make everything work nice once tool is useful

    arguments.mInputFile = "C:\\Program Files (x86)\\Metal Gear Solid\\stage\\init\\scenerio.gcx";

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

void ReadGCL(u8* pScript, int len);

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
            switch (cmdId)
            {
            case 0x0d86:
                std::cout << "IF()" << std::endl;
                break;

            case 0xC8BB:
                std::cout << "LOAD()" << std::endl;
                break;

            case 0x64C0:
                // TODO: Contents
                std::cout << "EVAL(";
                ReadGCL(pScript + 3, length - 3);
                std::cout << ")" << std::endl;
                break;

            default:
                std::cout << "CMD_UNKNOWN(0x" << std::hex << cmdId << ")" << std::endl;
                break;
            }

            pScript = pScript + length + 1;
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

void ReadGCL(u8* pScript, int len)
{
    pScript++;

    if ((*pScript & 0xF0) == 16)
    {
        std::cout << "Var read not impl" << std::endl;
        return;
    }

    switch (*pScript)
    {
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
        //*pRet = Script_Unknown6(pScript + 2, pRet);
        //pScriptByte1 += *pScriptByte1;
    }
    break;

    // Jump
    case 0x40:
    {
        //pScriptByte1 = &pScript[ToWORD(pScriptByte1) + 1];
        //*pRet = (DWORD)(pScript + 3);
    }
    break;

    // Parameter
    case 0x50:
    {
        //DWORD paramTypeAndId = *pScriptByte1 << 16;
        //paramTypeAndId |= 0x50;
        //*ppScript = paramTypeAndId;
       // *pRet = (DWORD)(pScript + 3);
        //pScriptByte1 += pScript[2] + 1;
    }
    break;

    // End
    case 0:
    {
        //pScriptByte1 = 0;
    }
    break;

    // Read s16
    case 1:
    {
        //int value = static_cast<s16>(ToWORD(pScriptByte1));
        //*pRet = value;
        //pScriptByte1 = pScript + 3;
    }
    break;

    // Read u8
    case 2:
    case 3:
    case 4:
    {
        //*pRet = *pScriptByte1;
        //pScriptByte1 = pScript + 2;
    }
    break;

    // Read string
    case 7:
    {
        //*pRet = reinterpret_cast<DWORD>(pScript + 2);
        //pScriptByte1 += *pScriptByte1 + 1;
    }
    break;

    // Read u16
    case 6:
    case 8:
    {
        //*pRet = ToWORD(pScriptByte1);
        //pScriptByte1 = pScript + 3;
    }
    break;

    // Read u32
    case 9:
    case 10:
    {
        //*pRet = ToDWORD(pScriptByte1);
        //pScriptByte1 = pScript + 5;
    }
    break;

    default:
        std::cout << "Unknown sub code 0x" << std::hex << static_cast<int>(*pScript) << std::endl;
        return;
    }
}