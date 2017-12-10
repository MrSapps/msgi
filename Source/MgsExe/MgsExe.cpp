#include <windows.h>

void ConsoleInit();
void InitRealGameSections();
void ApplyDetours();
int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

bool IsMgsi()
{
    return true;
}

// Try to place in the very first section
#pragma code_seg(".text$_")
__declspec(allocate(".text$_")) char gRealGameSectionsBuffer[1024*1024*5] = { };

// Try to place in the second section
#pragma code_seg(".text$__")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    ConsoleInit();
    InitRealGameSections();
    ApplyDetours();
    return New_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
