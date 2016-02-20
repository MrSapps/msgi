#include <windows.h>
#include "logger.hpp"
#include "MgsFunction.hpp"

int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

bool IsMgsi()
{
    return true;
}

// real addr of new func is 0x4103B0

extern "C"
{
    __declspec(dllexport) signed int __cdecl EBUEula(LPCSTR phkResult, LPARAM a2, int a3, int a4)
    {
        // TODO: Hook any functions that we don't reimplement but want to log etc

        // TODO: Call into the re-implemented WinMain
        

        LOG_INFO("Entering WinMain..");
        New_WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);

        // Don't allow control to go back to the real WinMain
        ExitProcess(0);
        
        // Not exiting and returning 1 makes the game take its "normal" path through winmain
    }
}

class outbuf : public std::streambuf
{
public:
    outbuf()
    {
        setp(0, 0);
    }

    virtual int_type overflow(int_type c = traits_type::eof()) override
    {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

outbuf ob;
std::streambuf *sb = nullptr;

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        SetConsoleTitleA("Debug Console");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

        // set std::cout to use my custom streambuf
        sb = std::cout.rdbuf(&ob);

        LOG_INFO("DLL_PROCESS_ATTACH");

        LOG_INFO("Applying detours...");
        MgsFunctionBase::ApplyFunctions();
        LOG_INFO("Detours done");
    }
    else if(fdwReason == DLL_PROCESS_DETACH)
    {
        LOG_INFO("DLL_PROCESS_DETACH");
        
        // make sure to restore the original so we don't get a crash on close!
        if (sb)
        {
            std::cout.rdbuf(sb);
        }
    }

    return TRUE;
}
