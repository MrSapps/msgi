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

struct Rect16
{
    WORD x1, y1, x2, y2;
};
static_assert(sizeof(Rect16) == 8, "Rect16 should be 8");

// 0x0044A7B0
signed int __cdecl Resetgraph(int a1)
{
    printf(".Resetgraph(%d)\n", a1);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "RECT", 8, 8, 2, 2);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "RECT32", 16, 16, 4, 4);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_ENV", 64, 64, 16, 16);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DRAWENV", 92, 92, 23, 23);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DISPENV", 20, 20, 5, 5);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "P_TAG", 8, 8, 2, 2);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "P_CODE", 4, 4, 1, 1);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_F3", 20, 20, 5, 5);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_F4", 24, 24, 6, 6);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_FT3", 32, 32, 8, 8);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_FT4", 40, 40, 10, 10);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_G3", 28, 28, 7, 7);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_G4", 36, 36, 9, 9);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_GT3", 40, 40, 10, 10);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "POLY_GT4", 52, 52, 13, 13);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "LINE_F2", 16, 16, 4, 4);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "LINE_G2", 20, 20, 5, 5);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "LINE_F3", 24, 24, 6, 6);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "LINE_G3", 32, 32, 8, 8);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "LINE_F4", 28, 28, 7, 7);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "LINE_G4", 40, 40, 10, 10);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "SPRT", 20, 20, 5, 5);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "SPRT_16", 16, 16, 4, 4);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "SPRT_8", 16, 16, 4, 4);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "TILE", 16, 16, 4, 4);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "TILE_16", 12, 12, 3, 3);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "TILE_8", 12, 12, 3, 3);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "TILE_1", 12, 12, 3, 3);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_MODE", 12, 12, 3, 3);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_TWIN", 12, 12, 3, 3);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_AREA", 12, 12, 3, 3);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_OFFSET", 12, 12, 3, 3);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_MOVE", 24, 24, 6, 6);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_LOAD", 68, 68, 17, 17);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_TPAGE", 8, 8, 2, 2);
    printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", "DR_STP", 12, 12, 3, 3);
    return 1;
}

// 0x0044AB30
int __cdecl SetGraphDebug(int a1)
{
    printf(".SetGraphDebug(%d)\n", a1);
    return 0;
}

// 0x0044AC40
int __cdecl SetDispMask(int a1)
{
    return printf(".SetDispMask(%d)\n", a1);
}

// 0x00520157
void DebugLog(const char *Format, ...)
{
    char Dest[512]; // [sp+0h] [bp-400h]@1
    va_list va; // [sp+40Ch] [bp+Ch]@1

    va_start(va, Format);
    vsprintf(Dest, Format, va);
    OutputDebugStringA(Dest);
}

int __cdecl ClearImage(Rect16 *rect, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b)
{
    DebugLog(".ClearImage((%d,%d,%d,%d),r=%d,g=%d,b=%d)\n", rect->x1, rect->y1, rect->x2, rect->y2, r, g, b);
    return 0;
}

// 0x0040815E
void __cdecl MemCardsInit()
{
    typedef decltype(&MemCardsInit) fn;
    ((fn)(0x0040815E))();
}

void __cdecl sub_40A4F6()
{
    typedef decltype(&sub_40A4F6) fn;
    ((fn)(0x40A4F6))();
}

int __cdecl sub_408086()
{
    typedef decltype(&sub_408086) fn;
    return ((fn)(0x408086))();
}

int __cdecl sub_40111A()
{
    typedef decltype(&sub_40111A) fn;
    return ((fn)(0x40111A))();
}

int __cdecl sub_4090A7()
{
    typedef decltype(&sub_4090A7) fn;
    return ((fn)(0x4090A7))();
}

int __cdecl sub_40B725()
{
    typedef decltype(&sub_40B725) fn;
    return ((fn)(0x40B725))();
}

void *__cdecl sub_44E12B()
{
    typedef decltype(&sub_44E12B) fn;
    return ((fn)(0x44E12B))();
}

// 0x0051C9A2
signed int __cdecl MainLoop()
{
    typedef decltype(&MainLoop) fn;
    return ((fn)(0x0051C9A2))();
}

struct actor_related_struct
{
    DWORD field_0;
    actor_related_struct* actor_struct_ptr1;
    void(__cdecl *fn_unknown)(actor_related_struct *);
    void(__cdecl *fnUnknown3)(actor_related_struct *);
    void(__cdecl *fnUnknown2)(actor_related_struct *);
    DWORD mNamePtr;
    DWORD field_18;
    DWORD field_1C;
    actor_related_struct* actor_struct_ptr2;
    DWORD field_24;
    DWORD field_28;
    DWORD field_2C;
    DWORD field_30;
    DWORD field_34;
    DWORD field_38;
    DWORD field_3C;
    WORD mPause;
    WORD mKill;
};

static_assert(sizeof(actor_related_struct) == 0x44, "actor_related_struct should be 0x44");

actor_related_struct* gActors = (actor_related_struct*)0x006BFC78; // Array of 9 items
DWORD& dword_791A0C = *(DWORD*)0x791A0C;
DWORD& dword_9942A0 = *(DWORD*)0x9942A0;

// 0x0040A1BF
int __cdecl Actor_Unknown()
{
    int result; // eax@8
    actor_related_struct *v1; // [sp+0h] [bp-18h]@5
    actor_related_struct *v2; // [sp+4h] [bp-14h]@4
    void(__cdecl *fn)(actor_related_struct *); // [sp+8h] [bp-10h]@5
    signed int i; // [sp+10h] [bp-8h]@1
    actor_related_struct *pActor; // [sp+14h] [bp-4h]@1

    pActor = gActors;
    for (i = 9; i > 0; --i)
    {
        if (!(dword_791A0C & pActor->mPause))
        {
            v2 = pActor;
            do
            {
                v1 = v2->actor_struct_ptr1;
                fn = v2->fn_unknown;
                if (fn)
                {
                    fn(v2);
                }
                dword_9942A0 = 0;
                v2 = v1;
            } while (v1);
        }
        ++pActor;
        result = i - 1;
    }
    return result;
}

DWORD& gExitMainGameLoop = *(DWORD*)0x0073492C;

// 0x00401005
signed int __cdecl Main()
{
    signed int result; // eax@2
    Rect16 clearRect; // [sp+4h] [bp-8h]@1

    Resetgraph(0);
    SetGraphDebug(0);
    //null_44AC80();
    SetDispMask(0);
    clearRect.x1 = 0;
    clearRect.y1 = 0;
    clearRect.x2 = 1024;
    clearRect.y2 = 511;
    ClearImage(&clearRect, 0, 0, 0);
    //null_44AB50();
    SetDispMask(1);
    //nullsub_14();
    //nullsub_13();
    //nullsub_8();
    MemCardsInit();
    //nullsub_9();
    sub_40A4F6();
    sub_408086();
    sub_40111A();
    sub_4090A7();
    sub_40B725();
    sub_44E12B();
    while (1)
    {
        result = MainLoop();
        if (!result)
            break;
        if (gExitMainGameLoop)
            break;

        // HACK: The game crashes somewhere deep in here, not calling this seems to prevent the game
        // state from progressing
        Actor_Unknown();
    }
    return result;
}

// 0x00401000
int __cdecl DoMain()
{
    return Main();
}

// 0x004232B0
void __cdecl DoClearAll()
{
    typedef decltype(&DoClearAll) fn;
    ((fn)(0x004232B0))();
}

char*& gCmdLine = *((char**)0x0071D16C);
DWORD& dword_787774 = *((DWORD*)0x787774);
DWORD& dword_787778 = *((DWORD*)0x787778);
WORD& dword_78E7E4 = *(WORD*)0x78E7E4;
DWORD& dword_78D7B0 = *((DWORD*)0x78D7B0);
DWORD& gNoCrashCheck = *((DWORD*)0x006DEF94);
DWORD& gCheatsEnabled = *(DWORD*)0x0071687C;
DWORD& gNoCdEnabled = *(DWORD*)0x006FD1F8;
DWORD& gWindowedMode = *(DWORD*)0x00650D14;
char*& off_688DB8 = *(char**)0x688DB8;
DWORD& dword_6FC7A0 = *((DWORD*)0x6FC7A0);
DWORD& gNoEffects = *(DWORD*)0x00650D24;
float& gXRes = *(float*)0x00650D28;
DWORD& gNoFilter = *(DWORD*)0x00650D34;
DWORD& gModX2 = *(DWORD*)0x00650D30;
DWORD& gNoTrueType = *(DWORD*)0x00650D40;
DWORD& gFps = *(DWORD*)0x006FC76C;
DWORD& gColourKey = *(DWORD*)0x006FC7A4;
DWORD& gBlendMode = *(DWORD*)0x00650D38;
DWORD& gLowRes = *(DWORD*)0x00650D20;
char*& off_688D40 = *(char**)0x688D40;
DWORD& gSoftwareRendering = *(DWORD*)0x006FC794;
HWND& gHwnd = *(HWND*)0x009ADDA0;
HINSTANCE& gHInstance = *(HINSTANCE*)0x0071D1D0;
DWORD& dword_651D98 = *((DWORD*)0x651D98);
DWORD& dword_716F68 = *((DWORD*)0x716F68);

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
    //int i; // [sp+468h] [bp-4h]@70


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
            
            dword_78E7E4 |= 0x4000u;
            dword_78E7E4 |= 0x100u;

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

                // HACK: Set some options that allow the game to actually start for now
                gCheatsEnabled = 1;
                gNoCrashCheck = 1;
                gWindowedMode = 1;
                gSoftwareRendering = 1;
                gNoCdEnabled = 1;

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

                            /* HACK: Leave cursor showing while developing
                            for (i = 1024; i && ShowCursor(0) >= 0; --i)// some hack to hide the cursor
                            {

                            }
                            */

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
