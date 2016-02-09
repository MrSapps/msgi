#include <windows.h>

bool IsMgsi()
{
    return false;
}

int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return New_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
