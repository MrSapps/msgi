#include <windows.h>

int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

extern "C"
{
    __declspec(dllexport) signed int __cdecl EBUEula(LPCSTR phkResult, LPARAM a2, int a3, int a4)
    {
        // TODO: Hook any functions that we don't reimplement but want to log etc

        // TODO: Call into the re-implemented WinMain
        


        New_WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);

        // Don't allow control to go back to the real WinMain
        ExitProcess(0);
        
        // Not exiting and returning 1 makes the game take its "normal" path through winmain
    }
}

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {

    }

    return TRUE;
}
