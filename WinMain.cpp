#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

void __cdecl CheckForMmf(int a1, int a2)
{
    typedef decltype(&CheckForMmf) fn;
    ((fn)(0x0051D120))(a1, a2);
}

signed int __stdcall MainWindowProc(HWND hWnd, UINT Msg, UINT wParam, LPARAM lParam)
{
    typedef decltype(&MainWindowProc) fn;
    return ((fn)(0x0051C2D3))(hWnd, Msg, wParam, lParam);
}

void __cdecl ShutdownEngine()
{
    typedef decltype(&ShutdownEngine) fn;
    ((fn)(0x0051D180))();
}

BOOL __cdecl SetWindowSize(HWND hWnd, int nWidth, int nHeight)
{
    typedef decltype(&SetWindowSize) fn;
    return ((fn)(0x0051D09D))(hWnd, nWidth, nHeight);
}

// 0x00420810
signed int __cdecl DoInitAll()
{
    typedef decltype(&DoInitAll) fn;
    return ((fn)(0x00420810))();
}

// 0x0052269C
signed int __cdecl SoundInit(HWND hwnd)
{
    typedef decltype(&SoundInit) fn;
    return ((fn)(0x0052269C))(hwnd);
}

// 0x004397D7
bool __cdecl AskUserToContinueIfNoSoundCard()
{
    typedef decltype(&AskUserToContinueIfNoSoundCard) fn;
    return ((fn)(0x004397D7))();
}

// 0x5224C8
int __cdecl sub_5224C8(int a1)
{
    typedef decltype(&sub_5224C8) fn;
    return ((fn)(0x5224C8))(a1);
}

// 0x52255B
int __cdecl sub_52255B(int a1)
{
    typedef decltype(&sub_52255B) fn;
    return ((fn)(0x52255B))(a1);
}

// 0x00421680
signed __int64 __cdecl FpsTimerSetupQ()
{
    typedef decltype(&FpsTimerSetupQ) fn;
    return ((fn)(0x00421680))();
}

// 0x00401000
int __cdecl DoMain()
{
    typedef decltype(&DoMain) fn;
    return ((fn)(0x00401000))();
}

// 0x004232B0
void __cdecl DoClearAll()
{
    typedef decltype(&DoClearAll) fn;
    ((fn)(0x004232B0))();
}

char* gCmdLinePtr = (char*)0x0071D16C;
char*& gCmdLine = gCmdLinePtr;

DWORD dword_787774;
DWORD dword_787778; // 0x787778;
WORD* dword_78E7E4; // 2 elements
DWORD dword_78D7B0;
DWORD gNoCrashCheck;
DWORD gCheatsEnabled;
DWORD gNoCdEnabled;
DWORD gWindowedMode;
char* off_688DB8;
DWORD dword_6FC7A0;
DWORD gNoEffects;
float gXRes;
DWORD gNoFilter;
DWORD gModX2;
DWORD gNoTrueType;
DWORD gFps;
DWORD gColourKey;
DWORD gBlendMode;
DWORD gLowRes;
char* off_688D40;
DWORD gSoftwareRendering;
HWND gHwnd;
HINSTANCE gHInstance;
DWORD dword_651D98;
DWORD dword_716F68;

int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    int result; // eax@2
    HANDLE currentProcess; // eax@14
    void(__stdcall *pSetProcessAffinityMask)(HANDLE, signed int); // [sp+8h] [bp-464h]@13
    void(__stdcall *pSetThreadExecutionState)(unsigned int); // [sp+Ch] [bp-460h]@13
    HMODULE hKernel32; // [sp+10h] [bp-45Ch]@12
    char Dest; // [sp+14h] [bp-458h]@11
    struct _MEMORYSTATUS Buffer; // [sp+414h] [bp-58h]@10
    char *v11; // [sp+434h] [bp-38h]@52
    WNDCLASSA WndClass; // [sp+438h] [bp-34h]@27
    char *bRestart; // [sp+464h] [bp-8h]@8
    int i; // [sp+468h] [bp-4h]@70


    if (!FindWindowA("Metal Gear Solid PC", "Metal Gear Solid PC") || strstr(lpCmdLine, "-restart"))
    {
        gCmdLine = lpCmdLine;
        bRestart = strstr(gCmdLine, "-restart");
        if (bRestart)
            *bRestart = 0;
        Buffer.dwLength = 32;
        memset(&Buffer.dwMemoryLoad, 0, 0x1Cu);
        GlobalMemoryStatus(&Buffer);
        if (Buffer.dwAvailPageFile >= 0x4000000)// 50mb hard disk space check
        {
            hKernel32 = LoadLibraryA("KERNEL32.DLL");
            if (hKernel32)
            {
                pSetProcessAffinityMask = (void(__stdcall *)(HANDLE, signed int))GetProcAddress(
                    hKernel32,
                    "SetProcessAffinityMask");
                pSetThreadExecutionState = (void(__stdcall *)(unsigned int))GetProcAddress(
                    hKernel32,
                    "SetThreadExecutionState");
                if (pSetProcessAffinityMask)
                {
                    currentProcess = GetCurrentProcess();
                    pSetProcessAffinityMask(currentProcess, 1);
                }
                if (pSetThreadExecutionState)
                    pSetThreadExecutionState(0x80000003u);
                FreeLibrary(hKernel32);
            }
            CheckForMmf(dword_787774, dword_787778);
            dword_78E7E4[0] |= 0x4000u;
            dword_78E7E4[0] |= 0x100u;
            _strlwr(lpCmdLine);
            _chdir(".");
            dword_78D7B0 = -1;
            if (strstr(lpCmdLine, "-nocrashcheck"))
                gNoCrashCheck = 0;
            else
                gNoCrashCheck = 1;
            if (strstr(lpCmdLine, "-cheatenable"))
                gCheatsEnabled = 1;
            else
                gCheatsEnabled = 0;
            if (strstr(lpCmdLine, "-nocd"))
                gNoCdEnabled = 1;
            else
                gNoCdEnabled = 0;
            WndClass.style = 3;
            WndClass.lpfnWndProc = (WNDPROC)MainWindowProc;
            WndClass.cbClsExtra = 0;
            WndClass.cbWndExtra = 0;
            WndClass.hInstance = hInstance;
            WndClass.hIcon = LoadIconA(hInstance, (LPCSTR)0x65);
            WndClass.hCursor = 0;
            WndClass.hbrBackground = (HBRUSH)GetStockObject(4);
            WndClass.lpszMenuName = 0;
            WndClass.lpszClassName = "Metal Gear Solid PC";
            if (RegisterClassA(&WndClass))
            {
                gWindowedMode = 0;
                if (strstr(lpCmdLine, off_688DB8))
                    dword_6FC7A0 = 58;
                else
                    dword_6FC7A0 = 0;
                if (strstr(lpCmdLine, "-noeffects"))
                    gNoEffects = 0;
                if (strstr(lpCmdLine, "-320"))
                    gXRes = 1.0;
                if (strstr(lpCmdLine, "-800"))
                    gXRes = 2.5;
                if (strstr(lpCmdLine, "-nofilter"))
                    gNoFilter = 0;
                if (strstr(lpCmdLine, "-nomod2x"))
                    gModX2 = 0;
                if (strstr(lpCmdLine, "-mod2x"))
                    gModX2 = 1;
                if (strstr(lpCmdLine, "-notruetype"))
                    gNoTrueType = 0;
                if (strstr(lpCmdLine, "-1024"))
                    gXRes = 3.200000047683716;
                if (strstr(lpCmdLine, "-fps"))
                    gFps = 1;
                if (strstr(lpCmdLine, "-colorkey"))
                    gColourKey = 1;
                v11 = strstr(lpCmdLine, "-blend");
                if (v11)
                {
                    v11 += 6;
                    gBlendMode = atoi(v11);
                }
                if (strstr(lpCmdLine, "-lowres"))
                    gLowRes = 1;
                if (strstr(lpCmdLine, off_688D40))
                    gWindowedMode = 0;
                if (strstr(lpCmdLine, "-w"))
                    gWindowedMode = 1;
                if (strstr(lpCmdLine, "-soft"))
                    gSoftwareRendering = 1;
                else
                    gSoftwareRendering = 0;
                gHwnd = CreateWindowExA(
                    0,
                    WndClass.lpszClassName,
                    "Metal Gear Solid PC",
                    0x80C80000u,
                    0x80000000u,
                    0x80000000u,
                    0x80000000u,
                    0x80000000u,
                    0,
                    0,
                    hInstance,
                    0);
                if (gHwnd)
                {
                    atexit(ShutdownEngine);
                    SetWindowSize(gHwnd, 640, 480);
                    ShowWindow(gHwnd, 5);
                    UpdateWindow(gHwnd);
                    gHInstance = hInstance;
                    if (DoInitAll())
                    {
                        if (SoundInit(gHwnd) || AskUserToContinueIfNoSoundCard())
                        {
                            sub_5224C8(dword_651D98);
                            sub_52255B(dword_716F68);
                            FpsTimerSetupQ();

                            for (i = 1024; i && ShowCursor(0) >= 0; --i)// some hack to hide the cursor
                            {

                            }

                            DoMain();
                            result = 0;
                        }
                        else
                        {
                            DoClearAll();
                            result = 0;
                        }
                    }
                    else
                    {
                        DoClearAll();
                        result = 0;
                    }
                }
                else
                {
                    result = 1;
                }
            }
            else
            {
                result = 1;
            }
        }
        else
        {
            sprintf(
                (char *)&Dest,
                "Metal Gear Solid requires over 50mb of hard disk space as Virtual Memory before the game can function correctly. This system currently only has %dmb available.  Please close all open applications not in use,  and refer to the Metal Gear Solid readme for more information on this issue.",
                (Buffer.dwAvailPageFile - Buffer.dwAvailPhys) >> 20);
            MessageBoxA(0, &Dest, "Metal Gear Solid PC", 0);
            result = 0;
        }
    }
    else
    {
        MessageBoxA(
            0,
            "Another copy of Metal Gear Solid Integral or VR missions is running, please exit first.",
            "Metal Gear Solid PC",
            0);
        result = 0;
    }

    return result;
}
