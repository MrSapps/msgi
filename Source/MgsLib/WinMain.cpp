#include "stdafx.h"

#include "WinMain.hpp"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <assert.h>
#include <iostream>
#include <memory>
#include <map>
#include <detours.h>

#define WINMAIN_IMPL true

// When changing this, delete the cfg files the game creates else it will get into a bad state and probably crash
#define HARDWARE_RENDERING_FORCE 1


#include "logger.hpp"
#include "MgsFunction.hpp"
#include "Sound.hpp"
#include "File.hpp"
#include "Input.hpp"
#include "Task.hpp"
#include "Script.hpp"
#include "Actor.hpp"
#include "System.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include "Renderer.hpp"
#include "ResourceNameHash.hpp"
#include "Psx.hpp"
#include "Actor_Rank.hpp"
#include "Actor_Delay.hpp"
#include "Actor_GameD.hpp"
#include "Actor_Loader.hpp"
#include "Actor_Movie.hpp"
#include "Timer.hpp"
#include "Fs.hpp"
#include "Imports.hpp"
#include "Menu.hpp"
#include "Kmd.hpp"
#include "Map.hpp"

#define _ELPP_THREAD_SAFE
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dsound.lib")



struct rend_struct
{
    DWORD field_0;
    DWORD field_4;
    DWORD field_8;
    DWORD float_field_C;
    DWORD field_10;
    DWORD field_14;
    DWORD field_18;
    DWORD field_1C;
};
static_assert(sizeof(rend_struct) == 0x20, "rend_struct should be 0x20");

struct Actor;
struct ActorList;

MGS_FUNC_NOT_IMPL(0x004397D7, bool __cdecl(), AskUserToContinueIfNoSoundCard);
MGS_FUNC_NOT_IMPL(0x0051D120, void __cdecl(int, int), CheckForMmf);
MGS_FUNC_NOT_IMPL(0x005202FE, DWORD __cdecl(float, float, float, float), FreeCameraMove_5202FE);

MGS_VAR(1, 0x71D1CC, HGDIOBJ, gScreenBitmap_71D1CC, nullptr);
MGS_VAR(1, 0x71D17C, DWORD, dword_71D17C, 0);

void CC jim_restore_screen_43AF41(HDC hdcDest)
{
    if (!gWindowedMode)
    {
        printf("$jim - jim_restore_screen {\n");
        if (gScreenBitmap_71D1CC)
        {
            HDC hdc = CreateCompatibleDC(hdcDest);
            if (hdc)
            {
                printf("$jim - storageDC is created\n");
                if (SelectObject(hdc, gScreenBitmap_71D1CC))
                {
                    printf("$jim - StretchBlt\n");

                    StretchBlt(hdcDest, 0, 0,
                        g_dwDisplayWidth_6DF214, g_dwDisplayHeight,
                        hdc, 0, 0,
                        g_dwDisplayWidth_6DF214, g_dwDisplayHeight,
                        SRCCOPY);

                    printf("$jim - StretchBlt( %d, %d, %d, %d ) <- ( 0, 0, %d, %d )\n",
                        0, 0, g_dwDisplayWidth_6DF214, g_dwDisplayHeight,
                        g_dwDisplayWidth_6DF214, g_dwDisplayHeight);

                    DeleteDC(hdc);
                }
                else
                {
                    printf("$jim - error selecting object storage in storageDC\n");
                }
            }
            else
            {
                printf("$jim - storageDC is not created right\n");
            }
        }
        else
        {
            printf("$jim - storage == 0 - no blt\n");
        }
        printf("$jim - jim_restore_screen }\n");
    }
}
MGS_FUNC_IMPLEX(0x43AF41, jim_restore_screen_43AF41, WINMAIN_IMPL);

void CC WmPaint_Handler_43ACC4(HDC hdcDest)
{
    bool bObtainedDC = false;
    if (!hdcDest)
    {
        printf("$jim - WM_PAINT wParam DC is NULL\n");
        if (gPrimarySurface_6FC734)
        {
            gPrimarySurface_6FC734->GetDC(&hdcDest);
        }
        bObtainedDC = true;
    }

    if (dword_71D17C)
    {
        printf("$jim - redraw\n");
        if (gPrimarySurface_6FC734)
        {
            jim_restore_screen_43AF41(hdcDest);
        }
    }

    if (bObtainedDC)
    {
        if (gPrimarySurface_6FC734)
        {
            gPrimarySurface_6FC734->ReleaseDC(hdcDest);
        }
    }

    printf("$jim end of WM_PAINT\n");
}
MGS_FUNC_IMPLEX(0x43ACC4, WmPaint_Handler_43ACC4, WINMAIN_IMPL);

MGS_FUNC_NOT_IMPL(0x0040815E, void __cdecl(), MemCardsInit);


MGS_FUNC_NOT_IMPL(0x0042B6A0, signed int __stdcall (GUID*, LPVOID*, const IID *const, IUnknown*), DirectDrawCreateExMGS);
MGS_FUNC_NOT_IMPL(0x004331D4, signed int __cdecl(), ParseMsgCfg_4331D4);
MGS_FUNC_NOT_IMPL(0x00433801, signed int __cdecl(), sub_433801);
MGS_FUNC_NOT_IMPL(0x0043C850, unsigned int __cdecl(), CheckJoyStickError_43C850);
MGS_FUNC_NOT_IMPL(0x00431C63, int __cdecl(), sub_431C63);
MGS_FUNC_NOT_IMPL(0x0042A630, void __cdecl(), _cfltcvt_init); // CRT func?
MGS_FUNC_NOT_IMPL(0x0041EA60, signed int __cdecl(), Render_Load_mlog_LCDN_10_Textures_41EA60);
MGS_FUNC_NOT_IMPL(0x0041C820, void __cdecl (float), Render_SetBrightness_sub_41C820);
MGS_FUNC_NOT_IMPL(0x0041CD70, int __cdecl(), Render_sub_41CD70);
MGS_FUNC_NOT_IMPL(0x0041CE20, bool __cdecl(), Render_sub_41CE20);
MGS_FUNC_NOT_IMPL(0x0041D420, signed int __cdecl(), Render_sub_41D420);
MGS_FUNC_NOT_IMPL(0x00431865, signed int __cdecl(), MakeFonts);
MGS_FUNC_NOT_IMPL(0x0051F5B8, signed int __stdcall(GUID*, const char*, char*, void*, HMONITOR), DeviceEnumCallBack);
MGS_FUNC_NOT_IMPL(0x52008A, int __cdecl(DWORD), DoSleep);
MGS_FUNC_NOT_IMPL(0x42BE0A, int __cdecl(), MainLoop_helper_42BE0A);
MGS_FUNC_NOT_IMPL(0x4317B3, BOOL __cdecl(), Fonts_Release_sub_4317B3);

MGS_VAR(1, 0x6FC748, IDirect3D7 *, g_pDirect3D, nullptr);

MGS_VAR(1, 0x6FC7E0, BYTE, byte_6FC7E0, 0);
MGS_VAR(1, 0x73491C, DWORD, dword_73491C, 0);
MGS_VAR(1, 0x71D164, DWORD, dword_71D164, 0);


void CC CentreWindow(HWND hWnd, int nWidth, int nHeight)
{
    RECT clientRect = {};
    MoveWindow(hWnd, 0, 0, nWidth, nHeight, FALSE);
    GetClientRect(hWnd, &clientRect);
    int height = nHeight - (clientRect.bottom - clientRect.top) + nHeight;
    int width = nWidth - (clientRect.right - clientRect.left) + nWidth;
    int y = (GetSystemMetrics(SM_CYMAXIMIZED) - height) / 2;
    int x = (GetSystemMetrics(SM_CXMAXIMIZED) - width) / 2;
    MoveWindow(hWnd, x, y, width, height, TRUE);
}
MGS_FUNC_IMPLEX(0x0051D09D, CentreWindow, WINMAIN_IMPL);

MGS_VAR(1, 0x717354, DWORD, gEscapePressed_NoMouseNoBgState_dword_717354, 0);
MGS_VAR(1, 0x717348, DWORD, gMouseMove_dword_717348, 0);
MGS_VAR(1, 0x7348FC, DWORD, gRestoreHealthCheat_7348FC, 0);
MGS_VAR(1, 0x732E64, DWORD, dword_732E64, 0);

//MGS_VAR(1, 0x64BDA8, const IID, IID_IDirectDraw7_MGS, {});
//MGS_VAR(1, 0x64BB98, const GUID, IID_IDirect3D7_MGS, {});
//MGS_VAR(1, 0x64BCA8, const GUID, IID_IDirectDrawGammaControl_MGS, {});
MGS_VAR(1, 0x6FC730, IDirectDraw7 *, g_pDirectDraw, nullptr);
MGS_VAR(1, 0x6C0EF8, IDirectDrawGammaControl *, g_pGammaControl, nullptr);
MGS_VAR(1, 0x6DF214, DWORD, g_dwDisplayWidth_6DF214, 0);
MGS_VAR(1, 0x6DF1FC, DWORD, g_dwDisplayHeight, 0);
MGS_VAR(1, 0x6FC750, LPDIRECTDRAWCLIPPER, g_pClipper, nullptr);

MGS_VAR(1, 0x6FC74C, LPDIRECT3DDEVICE7, gD3dDevice_6FC74C, nullptr);
MGS_VAR(1, 0x6FC740, LPDIRECTDRAWSURFACE7, g_pDDSurface_6FC740, nullptr);

MGS_VAR(1, 0x006DEF78, FILE *, gFile, nullptr);
MGS_VAR(1, 0x71D414, FILE *, gLogFile, nullptr);

MGS_VAR(1, 0x651CF8, DWORD, dword_651CF8, 0);
MGS_VAR(1, 0x716F5C, float, dword_716F5C, 0);
MGS_VAR(1, 0x716F78, DWORD, dword_716F78, 0);
MGS_VAR(1, 0x77C60C, DWORD, gDriverNum_dword_77C60C, 0);
MGS_VAR(1, 0x77C608, DWORD, gNumDrivers_dword_77C608, 0);

// Parts of 776B68
//MGS_PTR(1, 0x776B94, DWORD *, dword_776B94, nullptr); // TODO: Array?
//MGS_PTR(1, 0x776B90, DWORD *, dword_776B90, nullptr);

MGS_VAR(1, 0x716F70, DWORD, dword_716F70, 0);
MGS_VAR(1, 0x716F74, DWORD, dword_716F74, 0);
MGS_VAR(1, 0x650D2C, DWORD, dword_650D2C, 0);
MGS_VAR(1, 0x6DEF7C, void *, dword_6DEF7C, nullptr);
MGS_VAR(1, 0x6DEF90, void *, dword_6DEF90, nullptr);

MGS_VAR(1, 0x6FC798, DWORD, gAlphaModulate_dword_6FC798, 0);
MGS_VAR(1, 0x6FC7C0, DWORD, dword_6FC7C0, 0);
MGS_VAR(1, 0x716F6C, DWORD, dword_716F6C, 0);
MGS_VAR(1, 0x6FC7C4, DWORD, dword_6FC7C4, 0);
MGS_VAR(1, 0x651D94, DWORD, dword_651D94, 0);
MGS_VAR(1, 0x6FC79C, DWORD, g_surface565Mode, 0);
MGS_VAR(1, 0x716F60, DWORD, dword_716F60, 0);

struct jimDeviceDDId
{
    DDDEVICEIDENTIFIER2 identifier;
    DWORD field430;
    DWORD field434;
};
MGS_ASSERT_SIZEOF(jimDeviceDDId, 0x438);

struct jimDeviceIdentifier
{
    char pDriverDescription[0x28];      // 0x000
    GUID* pDeviceGUID;                  // 0x028
    GUID* pOtherGUID;                   // 0x02C
    GUID deviceGUID;                    // 0x030
    GUID otherGUID;                     // 0x040
    jimDeviceDDId ddIdentifier;         // 0x050
};
static_assert(sizeof(jimDeviceIdentifier) == 0x488, "jimDeviceIdentifier should be of size 0x488");

struct jimUnk0x204
{
    char    string[512];
    DWORD   field200;
};
static_assert(sizeof(jimUnk0x204) == 0x204, "jimUnk0x204 should be of size 0x204");


MGS_ARY(1, 0x776B68, struct jimDeviceIdentifier, 2, g_pDeviceIdentifiers_776B68, {}); // TODO: Check size, code seems to clamp it to 2
MGS_ARY(1, 0x689B68, struct jimUnk0x204, 2, array_689B68, {}); // TODO: Also 2?

MGS_ARY(1, 0x6C0778, char, 0x400, unk_6C0778, {}); // TODO: Struct?
MGS_VAR(1, 0x006FC7E8, HFONT, gFont_6FC7E8, nullptr);
MGS_VAR(1, 0x009ADDA0, HWND, gHwnd, nullptr);
MGS_VAR(1, 0x72279C, UGameStates, game_state_dword_72279C, {});

int CC sub_51F1E1(GUID** ppOtherGuid, GUID** ppDeviceGuid)
{
    if (gDriverNum_dword_77C60C >= gNumDrivers_dword_77C608)
    {
        *ppOtherGuid = 0;
        *ppDeviceGuid = 0;
    }
    else
    {
        *ppOtherGuid = g_pDeviceIdentifiers_776B68[gDriverNum_dword_77C60C].pOtherGUID;
        *ppDeviceGuid = g_pDeviceIdentifiers_776B68[gDriverNum_dword_77C60C].pDeviceGUID;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x51F1E1, sub_51F1E1, WINMAIN_IMPL);



//MGS_FUNC_NOT_IMPL(0x0041EC40, int __cdecl(), sub_41EC40);
int CC sub_41EC40()
{
    int nDriverIndex = gNumDrivers_dword_77C608 - 1;

    for (; nDriverIndex >= 0; nDriverIndex--)
    {
        int unkDword = array_689B68[nDriverIndex + 1].field200;

        if ((unkDword & 0x10) != 0)
            continue;

        gDriverNum_dword_77C60C = nDriverIndex;
        dword_716F70 = gNumDrivers_dword_77C608;
        dword_716F74 = gDriverNum_dword_77C60C;

        return 1;
    }

    return 0;
}
MGS_FUNC_IMPL(0x0041EC40, sub_41EC40);


//MSG_FUNC_NOT_IMPL(0x51E1D9, int __cdecl(), HandleExclusiveMode);
int CC HandleExclusiveMode()
{
    MSG oMsg = {};

    if (!g_pDirectDraw)
    {
        return 0;
    }

    if (g_pDirectDraw->TestCooperativeLevel() != DDERR_NOEXCLUSIVEMODE)
    {
        return 0;
    }

    Sound_StopSample();
    FS_StreamActorStop_4583BB();
    Task_Pause();

    do
    {
        if (PeekMessageA(&oMsg, 0, 0, 0, 1) != 0)
        {
            if (oMsg.message == WM_QUIT)
            {
                PostQuitMessage(0);
                return 1;
            }

            TranslateMessage(&oMsg);
            DispatchMessageA(&oMsg);
        }

        Sleep(1000);
    }
    while (g_pDirectDraw->TestCooperativeLevel() != 0);

    Render_Restore_Lost_Surfaces_51E086();
    Timer_30_1();
    Task_ResumeQ();
    Sound_PlaySample();

    if (dword_732E64 == 1)
    {
        PostMessageA(gHwnd, WM_KEYDOWN, VK_ESCAPE, 0);
    }

    return 0;
}
MGS_FUNC_IMPLEX(0x51E1D9, HandleExclusiveMode, WINMAIN_IMPL);

MGS_ARY(1, 0x9AD880, BYTE, 256, gKeys_9AD880, {});

int CC MainLoop_51C9A2()
{
    //char var11C[0xFF] = { 0xFF };
    //char var21B[0xFF] = { 0xFF };
    //BYTE var21C = byte_6FC7E0;
    MSG oMsg = {};

    Sound_PopulateBufferQ();

    if (dword_73491C == 1)
    {
        // This seems to skip frames when the game is running too "slow".
        // Disabling it when debug logging causes some frame lag actually improves performance a lot.
        dword_73491C = 2;
        while (dword_73491C == 2)
        {
            DoSleep(8 + (rand() % 4));
        }
        dword_73491C = 0;
    }
    if (dword_71D164 == 0 && gEscapePressed_NoMouseNoBgState_dword_717354 != 0 && gMouseMove_dword_717348 == 0)
    {
        gMouseMove_dword_717348 = 1;
        gKeys_9AD880[VK_ESCAPE] = 0;

        if (MainLoop_helper_42BE0A() == -17)
        {
            return 0;
        }

        dword_6FC718 = 1;
        gEscapePressed_NoMouseNoBgState_dword_717354 = 0;
        gKeys_9AD880[VK_ESCAPE] = 0;
        gMouseMove_dword_717348 = 0;
    }

    if (gRestoreHealthCheat_7348FC)
    {
        gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 = 1024;
        gGameStates_78E7E0.gSnakeMaxHealth_78E7F8 = 1024;
    }
    
    HandleExclusiveMode();

    if (PeekMessageA(&oMsg, 0, 0, 0, 1) == 0)
    {
        return 1;
    }

    if (oMsg.message == WM_QUIT)
    {
        PostQuitMessage(0);
        return 0;
    }

    TranslateMessage(&oMsg);
    DispatchMessageA(&oMsg);

    return 1;
}
MGS_FUNC_IMPLEX(0x51C9A2, MainLoop_51C9A2, WINMAIN_IMPL);


// FIX ME - need a way to handle non standard calling conventions
// 0x452E6E
int /*__usercall*/ sub_452E6E/*<eax>*/(/*<esi>*/)
{
    typedef decltype(&sub_452E6E) fn;
    return ((fn)(0x452E6E))();
}


MGS_VAR(1, 0x73492C, DWORD, gExitMainGameLoop, 0);
MGS_VAR(1, 0x0071D16C, char*, gCmdLine, nullptr);
MGS_VAR(1, 0x787774, DWORD, dword_787774, 0);
MGS_VAR(1, 0x787778, DWORD, dword_787778, 0);
MGS_VAR(1, 0x006DEF94, DWORD, gCrashCheck, 0);
MGS_VAR(1, 0x0071687C, DWORD, gCheatsEnabled_71687C, 0);
MGS_VAR(1, 0x006FD1F8, DWORD, gNoCdEnabled, 0);
MGS_VAR(1, 0x00650D14, DWORD, gWindowedMode, 0);
MGS_VAR(1, 0x6FC7A0, DWORD, dword_6FC7A0, 0);
MGS_VAR(1, 0x00650D24, DWORD, gNoEffects, 0);

MGS_VAR(1, 0x00650D34, DWORD, gNoFilter, 0);
MGS_VAR(1, 0x00650D40, DWORD, gUseTrueType_650D40, 0);
MGS_VAR(1, 0x006FC76C, DWORD, gFps, 0);
MGS_VAR(1, 0x006FC7A4, DWORD, gColourKey, 0);
MGS_VAR(1, 0x00650D38, int, gBlendMode, 0);
MGS_VAR(1, 0x00650D20, DWORD, gLowRes, 0);

MGS_VAR(1, 0x0071D1D0, HINSTANCE, gHInstance, 0);
MGS_VAR(1, 0x651D98, DWORD, gSoundFxVol_dword_651D98, 0);
MGS_VAR(1, 0x716F68, DWORD, gMusicVol_dword_716F68, 0);
MGS_VAR(1, 0x77C934, DWORD, gFreeCameraCheat_77C934, 0);

MGS_VAR(1, 0x73490C, DWORD, gInput_MouseY_dword_73490C, 0);
MGS_VAR(1, 0x734908, DWORD, gInput_MouseX_dword_734908, 0);
MGS_ARY(1, 0x009AD9A0, int, 256, gKeys_9AD9A0, {});

MGS_VAR(1, 0x009AD980, DWORD, gvirtualKeyRepeatCount, 0);
MGS_VAR(1, 0x009AD6B0, DWORD, gVirtualKeyCode, 0);
MGS_VAR(1, 0x71D194, DWORD, dword_71D194, 0);
MGS_VAR(1, 0x734900, DWORD, sLastMouseX_unk_734900, 0);
MGS_VAR(1, 0x734904, DWORD, sLastMouseY_unk_734904, 0);
MGS_VAR(1, 0x9AD988, BYTE, byte_9AD988,0);
MGS_VAR(1, 0x688CDC, DWORD, gActive_dword_688CDC, 0);

MGS_VAR(1, 0x688CD8, DWORD, dword_688CD8, 0);
MGS_VAR(1, 0x791DE4, DWORD, dword_791DE4, 0);
MGS_VAR(1, 0x733E34, DWORD, dword_733E34, 0);
MGS_VAR(1, 0x721E78, DWORD, dword_721E78, 0);
MGS_VAR(1, 0x650D4C, DWORD, gInfiniteAmmoCheat_650D4C, 0);
MGS_ARY(1, 0x0078E7C0, char, 32, gDest_78E7C0, {});

struct weapon_famas
{
    Actor mBase;
    DWORD field_20;
    DWORD field_24;
    DWORD field_28;
    DWORD field_2C;
    DWORD field_30;
    DWORD field_34;
    DWORD field_38;
    DWORD field_3C;
    DWORD field_40;
    DWORD field_44_a1;
    DWORD field_48_a2;
    DWORD field_4C_a3;
    DWORD field_50_a4;
    DWORD field_54;
    DWORD field_58;
    DWORD mbIsMp5;
};
MGS_ASSERT_SIZEOF(weapon_famas, 96);

MGS_VAR(1, 0x995368, WORD, word_995368, 0);
MGS_VAR(1, 0x995320, WORD, word_995320, 0);

MGS_FUNC_NOT_IMPL(0x00640CDC, int __cdecl(weapon_famas*), Res_famas_update_640CDC);
MGS_FUNC_NOT_IMPL(0x00640E9E, int* __cdecl(weapon_famas*), Res_famas_shutdown_640E9E);


//MSG_FUNC_NOT_IMPL(0x00640EAD, signed int __cdecl(weapon_famas*, int, int, int), Res_Weapon_famas_init_sub_640EAD);
signed int __cdecl Res_Weapon_famas_loader_640EAD(weapon_famas* pFamas, struc_kmd* a2, int a3, int bMp5)
{
    WORD resNameHashed = 0;
    if (bMp5)
    {
        resNameHashed = ResourceNameHash("mpfive");
    }
    else
    {
        resNameHashed = ResourceNameHash("famas");
    }

    struc_kmd* pField20 = (struc_kmd*)&pFamas->field_20;
    Kmd_Load_44FF7C(pField20, resNameHashed, 'm');

    if (pField20->field_0_pObj)
    {
        Kmd_Link_To_Parent_Mesh_45011B(pField20, a2, a3);
        return 0;
    }

    return -1;
}

weapon_famas* CC Res_Weapon_famas_96_sub_640C24(ActorList* a1, struc_kmd *a2, void(__cdecl *a3)(ActorList *), void(__cdecl *a4)(DWORD), int bMp5)
{
    weapon_famas* pFamas = Actor_ResourceAllocT<weapon_famas>(6);
    if (pFamas)
    {
        Actor_Init_40A347(&pFamas->mBase, (TActorFunction)Res_famas_update_640CDC.Ptr(), (TActorFunction)Res_famas_shutdown_640E9E.Ptr(), "C:\\mgs\\source\\Weapon\\famas.c");
        if (Res_Weapon_famas_loader_640EAD(pFamas, a2, (int)a3, bMp5) < 0)
        {
            Actor_DestroyOnNextUpdate_40A3ED(&pFamas->mBase);
            return 0;
        }
        pFamas->field_58 = 0;
        pFamas->field_44_a1 = (int)a1;
        pFamas->field_48_a2 = (int)a2;
        pFamas->field_4C_a3 = (int)a3;
        pFamas->field_50_a4 = (int)a4;
        pFamas->field_54 = 1;
        pFamas->mbIsMp5 = bMp5;
    }

    const WORD mp5ClipSize = (word_995368 != 0) + 25;                 // 25 is the ammo clip size
    if (bMp5)
    {
        word_995320 = mp5ClipSize;
        word_995368 = mp5ClipSize;
    }
    else
    {
        WORD famasClipSize = gGameStates_78E7E0.gWeapon_states_word_78E802[1];
        if (mp5ClipSize > 0 && famasClipSize > mp5ClipSize)
        {
            famasClipSize = mp5ClipSize;
        }
        word_995320 = mp5ClipSize;
        word_995368 = famasClipSize;
    }
    return pFamas;
}
MGS_FUNC_IMPLEX(0x640C24, Res_Weapon_famas_96_sub_640C24, WINMAIN_IMPL);

void __cdecl Input_AcquireOrUnAcquire();

LRESULT CALLBACK MainWindowProc_51C2D3(HWND hWnd, UINT msg, UINT wParam, LPARAM lParam)
{
    LRESULT result = 0;
    UINT vKey = 0;
    char keyName[256] = {};

    if (gFreeCameraCheat_77C934)
    {
        if (gKeys_9AD880[VK_LEFT])
        {
            FreeCameraMove_5202FE(0.0, -0.003, 0.0, 0.0);
        }
        if (gKeys_9AD880[VK_RIGHT])
        {
            FreeCameraMove_5202FE(0.0, 0.003, 0.0, 0.0);
        }
        if (gKeys_9AD880[VK_UP])
        {
            FreeCameraMove_5202FE(0.0, 0.0, -0.003, 0.0);
        }
        if (gKeys_9AD880[VK_DOWN])
        {
            FreeCameraMove_5202FE(0.0, 0.0, 0.003, 0.0);
        }
        if (gKeys_9AD880['A'])
        {
            FreeCameraMove_5202FE(0.029999999, 0.0, 0.0, 0.0);
        }
        if (gKeys_9AD880['Z'])
        {
            FreeCameraMove_5202FE(-0.029999999, 0.0, 0.0, 0.0);
        }
        const float mouseY = (double)gInput_MouseY_dword_73490C / 1024.0;
        const float mouseX = (double)gInput_MouseX_dword_734908 / 1024.0;
        FreeCameraMove_5202FE(0.0, mouseX, mouseY, 0.0);
        gInput_MouseX_dword_734908 = 9 * gInput_MouseX_dword_734908 / 10;
        gInput_MouseY_dword_73490C = 9 * gInput_MouseY_dword_73490C / 10;
    }

    switch (msg)
    {
    case WM_SYSKEYDOWN:
        gKeys_9AD9A0[wParam] = lParam;
        gvirtualKeyRepeatCount = lParam;
        gVirtualKeyCode = wParam;

        if (gVirtualKeyCode == VK_MENU)
        {
            gKeys_9AD880[VK_MENU] = 1;
            return 0;
        }

        if (gVirtualKeyCode == VK_F10)
        {
            dword_71D194 = 1;
            gKeys_9AD880[VK_F10] = 1;
            return 0;
        }

        if (wParam < 256)
        {
            gKeys_9AD880[wParam] = 1;
            return DefWindowProcA(hWnd, msg, wParam, lParam);
        }
        break;

    case WM_SYSKEYUP:
        gvirtualKeyRepeatCount = lParam;
        gVirtualKeyCode = wParam;

        if (gVirtualKeyCode == VK_MENU) // ALT
        {
            gKeys_9AD880[VK_MENU] = 0;
            return 0;
        }
        if (gVirtualKeyCode == VK_F10)
        {
            gKeys_9AD880[VK_F10] = 0;
            Actor_DumpActorSystem_40A0D4();
            return 0;
        }

        if (wParam < 256)
        {
            gKeys_9AD880[wParam] = 0;
            return DefWindowProcA(hWnd, msg, wParam, lParam);
        }
        break;

    case WM_MOUSEFIRST:
        if (gFreeCameraCheat_77C934)
        {
            gInput_MouseX_dword_734908 = LOWORD(lParam) - sLastMouseX_unk_734900;
            gInput_MouseY_dword_73490C = HIWORD(lParam) - sLastMouseY_unk_734904;
            sLastMouseX_unk_734900 = LOWORD(lParam);
            sLastMouseY_unk_734904 = HIWORD(lParam);
        }
        break;

    case WM_POWERBROADCAST:
        return BROADCAST_QUERY_DENY;

    case WM_CHAR:
        byte_9AD988 = wParam;
        return DefWindowProcA(hWnd, msg, wParam, lParam);

    case WM_ACTIVATE:
        if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
        {
            printf("$jim - WM_ACTIVATE (active)\n");
            gActive_dword_688CDC = 1;
        }
        else
        {
            printf("$jim - WM_ACTIVATE (inactive)\n");
            gActive_dword_688CDC = 0;
        }
        Input_AcquireOrUnAcquire();
        Timer_30_1();
        return 1;

    case WM_PAINT:
        printf("$jim - WM_PAINT\n");
        if (dword_71D17C)
        {
            WmPaint_Handler_43ACC4((HDC)wParam);
            return 0;
        }
        return DefWindowProcA(hWnd, msg, wParam, lParam);

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KEYUP:
        if (wParam < 256)
        {
            gKeys_9AD880[wParam] = 0;
        }
        return DefWindowProcA(hWnd, msg, wParam, lParam);

    case WM_KEYDOWN:
        vKey = MapVirtualKeyA(wParam, 0);
        if (vKey)
        {
            GetKeyNameTextA(vKey << 16, keyName, 256);
            gKeys_9AD9A0[wParam] = lParam;
        }
        gKeys_9AD9A0[wParam] = lParam;

        switch (wParam)
        {
        case VK_F2:
            if (gCheatsEnabled_71687C)
            {
                gRestoreHealthCheat_7348FC = gRestoreHealthCheat_7348FC == 0;
            }
            return 0;

        case VK_F4:
            if (gCheatsEnabled_71687C)
            {
                gInfiniteAmmoCheat_650D4C ^= 1u;
            }
            return 0;

        case VK_F5:
            if (gCheatsEnabled_71687C)
            {
                gFreeCameraCheat_77C934 = 0;
            }
            return 0;

        case VK_F6:
            if (gCheatsEnabled_71687C)
            {
                gFreeCameraCheat_77C934 = 1;
            }
            return 0;

        case VK_F7:
            if (gCheatsEnabled_71687C)
            {
                game_state_dword_72279C.flags = 0;
                FS_StreamEnd_521210();
                sub_452E6E();
            }
            return 0;

        case VK_F8:
            if (gCheatsEnabled_71687C)
            {
                dword_688CD0 = 0;
            }
            return 0;

        case VK_F9:
            if (gCheatsEnabled_71687C)
            {
                dword_688CD0 = 1;
            }
            return 0;

        case VK_F11:
            if (gCheatsEnabled_71687C)
            {
                dword_688CD4 ^= 1u;
            }
            return 0;

        case VK_F12:
            if (gCheatsEnabled_71687C)
            {
                dword_688CD8 ^= 1u;
            }
            return 0;

        case VK_ESCAPE:
            dword_791DE4 = 1;
            if ((game_state_dword_72279C.flags != 0x20000000 || !strstr(gDest_78E7C0, "s19a"))
                && !gEscapePressed_NoMouseNoBgState_dword_717354
                && !gMouseMove_dword_717348
                && !gKeys_9AD880[VK_BACK]
                && !dword_733E34
                && !dword_721E78)
            {
                gEscapePressed_NoMouseNoBgState_dword_717354 = 1;
            }
            // Fall through

        default:
            if (wParam < 256)
            {
                gKeys_9AD880[wParam] = 1;
            }
            return DefWindowProcA(hWnd, msg, wParam, lParam);
        }
        break;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
MGS_FUNC_IMPLEX(0x51C2D3, MainWindowProc_51C2D3, WINMAIN_IMPL);

struct MessageBoxStruct
{
    DWORD mShowErrCount;
    DWORD mDisplayThisErrCode;
};

MGS_ARY(1, 0x664EC0, MessageBoxStruct, 8, stru_664EC0, {});

struct ErrorStrings
{
    char mStrings[8][1024];
};

const ErrorStrings kArray_00662EC0 =
{
    {
        { "Hold down the Inventory and Weapon item buttons" },
        { "if you do not want to play Alternate Round" },
        { "RADAR OFF" },
        { "Another copy of Metal Gear Solid Integral or VR missions is running, please exit first." },
        { "Metal Gear Solid has detected that your graphics accelerator does not support the functions needed to run the game in Hardware mode.  The game will run correctly, but only in software mode." },
        { "Metal Gear Solid has detected that your graphics accelerator does not support all the functions needed to run the game correctly in Hardware mode.  The game will now default to start in software mode.  You can change this to run with your hardware acceleator at any time from the Video Options / Advanced menu.  This is not recomended as correct game functionality cannot be assured." },
        { "Metal Gear Solid requires at least 4 MB of video accelerator memory available.  Currently, there is not enough video memory on this system to run the game in Hardware mode.  The game will run correctly, but only in software mode.  If more video memory is made available, Metal Gear Solid will run in hardware mode.  For more information, contact your hardware manufacturer." },
        { "Metal Gear Solid has detected that you have a joystick in use on this system.  Please ensure that this joystick is centered prior to playing the game, otherwise the joystick input will override any keyboard direction input." }
    }
};


MGS_VAR(1, 0x00662EC0, const ErrorStrings, gStrErrStrings, kArray_00662EC0);

// 0x0043CBD9
int __cdecl MessageBox_Error(HWND hWnd, int errCode, LPCSTR lpCaption, UINT uType)
{
    int result;
    if (errCode <= 0 || (result = errCode, stru_664EC0[errCode].mDisplayThisErrCode))
    {
        if (errCode == -1)
        {
            signed int msgIdx = -1;

            for (int i = 0; i < 8; ++i)
            {
                if (stru_664EC0[i].mDisplayThisErrCode)
                {
                    if (stru_664EC0[i].mShowErrCount)
                    {
                        msgIdx = i;
                    }
                }
                result = i + 1;
            }

            for (int j = 0; j < 8; ++j)
            {
                if (stru_664EC0[j].mDisplayThisErrCode)
                {
                    if (stru_664EC0[j].mShowErrCount)
                    {
                        if (stru_664EC0[j].mDisplayThisErrCode < stru_664EC0[msgIdx].mDisplayThisErrCode)
                        {
                            msgIdx = j;
                            stru_664EC0[j].mShowErrCount = 0;
                        }
                    }
                }
                result = j + 1;
            }

            if (msgIdx >= 0)
            {
                result = MessageBoxA(hWnd, gStrErrStrings.mStrings[msgIdx], lpCaption, uType);
            }
        }
        else
        {
            ++stru_664EC0[errCode].mShowErrCount;
            result = 2;
        }
    }
    else
    {
        result = MessageBoxA(hWnd, gStrErrStrings.mStrings[errCode], lpCaption, uType);
    }
    return result;
}

void CC Create_Arial_Font_423F1B(int cWidth, int cHeight)
{
    if (gFont_6FC7E8)
    {
        DeleteObject(gFont_6FC7E8);
    }

    gFont_6FC7E8 = CreateFontA(cHeight, cWidth, 0, 0, 500, 0, 0, 0, 1u, 0, 0, 2u, 0, "Arial");
}
MGS_FUNC_IMPLEX(0x423F1B, Create_Arial_Font_423F1B, WINMAIN_IMPL);

MGS_FUNC_NOT_IMPL(0x00642382, int __stdcall(LPDDENUMCALLBACKEXA, LPVOID, DWORD), DirectDrawEnumerateExA_MGS);
MGS_FUNC_NOT_IMPL(0x51E382, int __cdecl(void*, int), File_msgvideocfg_Write_51E382);
MGS_FUNC_NOT_IMPL(0x51E586, int __cdecl(void*, int), file_msgvideocfg_Write2);

MGS_VAR(1, 0x68C3B8, DWORD, dword_68C3B8, 0);
MGS_ARY(1, 0x775F48, char, 1024, byte_775F48, {});
MGS_ARY(1, 0x774B48, char, 1024, gErrStr_774B48, {});
MGS_ARY(1, 0x776450, char, 1024, byte_776450, {});


signed int CC File_msgvideocfg_Has_Device_51E29B(LPDDDEVICEIDENTIFIER2 pDeviceToFind, jimDeviceDDId* pReadDevice, int devIdx)
{
    if (pReadDevice && pDeviceToFind)
    {
        FILE* hFile = fopen("mgsvideo.cfg", "rb");
        if (!hFile)
        {
            return 0;
        }

        if (devIdx >= 4)
        {
            fseek(hFile, sizeof(jimDeviceDDId) * 4, 0);
            if (ftell(hFile) == sizeof(jimDeviceDDId) * 4)
            {
                while (fread(pReadDevice, 1u, sizeof(jimDeviceDDId), hFile) == sizeof(jimDeviceDDId))
                {
                    if (!memcmp(pDeviceToFind, pReadDevice, 0x430u))
                    {
                        fclose(hFile);
                        return 1;
                    }
                }
            }
        }
        else
        {
            fseek(hFile, sizeof(jimDeviceDDId) * devIdx, 0);
            if (ftell(hFile) == static_cast<int>(sizeof(jimDeviceDDId) * devIdx)
                && fread(pReadDevice, 1u, sizeof(jimDeviceDDId), hFile) == sizeof(jimDeviceDDId)
                && !memcmp(pDeviceToFind, pReadDevice, 0x430u))
            {
                fclose(hFile);
                return 1;
            }
        }

        fclose(hFile);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x51E29B, File_msgvideocfg_Has_Device_51E29B, WINMAIN_IMPL);


//MSG_FUNC_NOT_IMPL(0x51E7FC, int __cdecl(LPD3DDEVICEDESC7, LPSTR, LPSTR, jimDeviceIdentifier*), validateDeviceCaps);
int __cdecl validateDeviceCaps(LPD3DDEVICEDESC7 pDesc, LPSTR /*lpDeviceDescription*/, LPSTR lpDeviceName, jimDeviceIdentifier* pIdentifier)
{
    byte_775F48 = 0;
    gErrStr_774B48 = 0;
    byte_776450 = 0;

    char* pStringError = byte_776450;
    char* pStringWarning = gErrStr_774B48;
    char localString[0x100];
    uint32_t status = 0;

    if (pIdentifier->ddIdentifier.identifier.dwVendorId == 0x8086)
    {
        strcat(pStringError, "Intel device found. Software only.\n");
        status = 1;
    }

    if (pDesc->dwMaxTextureWidth < 0x100 || pDesc->dwMaxTextureHeight < 0x100)
    {
        strcat(pStringError, "E2a:\tMaximum Texture Width/Height is below game requirement of 256\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHA))
    {
        strcat(pStringError, "E2b:\tNo Texture Alpha Channel support\n");
        status = 1;
    }

    // For a GTX770 either this check is wrong or somehow it supports less features than antique cards
    // disable this check so the card isn't marked as unsupported.
    //pDesc->dpcTriCaps.dwTextureCaps |= D3DPTEXTURECAPS_POW2;
#ifndef HARDWARE_RENDERING_FORCE
    if (!(pDesc->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2))
    {
        strcat(pStringError, "E2e:\tBilinear filtering not supported\n");
        status = 1;
    }
#endif
    if (!(pDesc->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_TRANSPARENCY) || !(pDesc->dpcTriCaps.dwAlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL))
    {
        strcat(pStringError, "E3a:\tNo Texture Transparency or Alpha Test (GREATEROREQUAL) support\n");
        status = 1;
    }
    if (pDesc->dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR)
    {
        strcat(pStringError, "E5a:\tDevice is PoverVR like, which is not supported\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwShadeCaps & (D3DPSHADECAPS_ALPHAFLATBLEND | D3DPSHADECAPS_ALPHAGOURAUDBLEND)))
    {
        strcat(pStringError, "E6a:\tFlat or Gourad Alpha Flat Blending required\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_COLORGOURAUDRGB))
    {
        strcat(pStringError, "E6b:\tRGB Color Gouraud Shading required\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_ONE))
    {
        strcat(pStringError, "E7a:\tSRCBLEND_ONE required\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_ZERO))
    {
        strcat(pStringError, "E7a:\tSRCBLEND_ZERO required\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_ONE))
    {
        strcat(pStringError, "E7a:\tSRCBLEND_ONE required\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_ZERO))
    {
        strcat(pStringError, "E7a:\tSRCBLEND_ZERO required\n");
        status = 1;
    }
    if (!(pDesc->dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_ONE))
    {
        strcat(pStringError, "E7a:\tSRCBLEND_ONE required\n");
        status = 1;
    }
    
    if (!(pDesc->dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) || !(pDesc->dpcTriCaps.dwDestBlendCaps & (D3DPBLENDCAPS_INVSRCCOLOR | D3DPBLENDCAPS_SRCALPHA)))
    {
        strcat(pStringWarning, "W1:\tDevice doesn't support minimum blending modes required\n");
        status |= 2;
    }
    if (pDesc->dwDevCaps & D3DDEVCAPS_SORTEXACT)
    {
        strcat(pStringWarning, "W2:\tDevice requires SORTEXACT feature, which may cause visual artifacts\n");
        status |= 2;
    }
    if (pDesc->dwDevCaps & D3DDEVCAPS_SORTDECREASINGZ)
    {
        strcat(pStringWarning, "W3:\tDevice requires SORTDECREASINGZ\n");
        status |= 2;
    }
    if (!(pDesc->dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_SUBPIXEL))
    {
        strcat(pStringWarning, "W4:\tDevice can't render with subpixel accurate\n");
        status |= 2;
    }
    if (!(pDesc->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_PERSPECTIVE))
    {
        strcat(pStringWarning, "W5:\tDevice can't render with perspective correct texture mapping\n");
        status |= 2;
    }
    
    uint32_t field480 = pIdentifier->ddIdentifier.field430;
    if (((field480 & 1) == 0) && ((field480 & 0x40) == 0) && (byte_776450 != 0))
    {
        sprintf(localString, "%s / (%s)", pIdentifier->ddIdentifier.identifier.szDescription, lpDeviceName);
        strcat(pStringError, "\n\tDevice doesn't meet minimum requirements, and will be ignored by the game\n");
        MessageBox_Error(0, 4, "Metal Gear Solid PC", MB_OK);
    }
    else if (((field480 & 2) == 0) && ((field480 & 1) == 0) && ((field480 & 0x40) == 0) && (gErrStr_774B48 != 0))
    {
        sprintf(localString, "%s / (%s)", pIdentifier->ddIdentifier.identifier.szDescription, lpDeviceName);
        strcat(pStringWarning, "\n\tDevice doesn't support everything the game needs\nBut it will be allowed for selection in Option/Advanced Menu\n");
        MessageBox_Error(0, 5, "Metal Gear Solid PC", MB_OK);
    }

    pIdentifier->ddIdentifier.field430 |= status;

#ifdef HARDWARE_RENDERING_FORCE
    //pIdentifier->ddIdentifier.field430 = 0;
#endif

    return pIdentifier->ddIdentifier.field430;
}

MGS_FUNC_IMPLEX(0x51E7FC, validateDeviceCaps, WINMAIN_IMPL);

HRESULT CALLBACK EnumModesCallback(LPDDSURFACEDESC2 pDesc, LPVOID pUser)
{
    jimDeviceIdentifier* pIdentifier = (jimDeviceIdentifier*)pUser;

    if (pDesc->ddpfPixelFormat.dwRGBBitCount != 0x10)
    {
        return 1;
    }

    if (pDesc->dwWidth == 320 && pDesc->dwHeight == 240)
    {
        pIdentifier->ddIdentifier.field434 |= 1;
    }
    if (pDesc->dwWidth == 640 && pDesc->dwHeight == 480)
    {
        pIdentifier->ddIdentifier.field434 |= 2;
    }
    if (pDesc->dwWidth == 800 && pDesc->dwHeight == 600)
    {
        pIdentifier->ddIdentifier.field434 |= 4;
    }
    if (pDesc->dwWidth == 1024 && pDesc->dwHeight == 768)
    {
        pIdentifier->ddIdentifier.field434 |= 8;
    }

    return 1;
}

// 0x51FA74
HRESULT CALLBACK Enum3DDevicesCallback(LPSTR lpDeviceDescription, LPSTR lpDeviceName, LPD3DDEVICEDESC7 pDesc, LPVOID pUser)
{
    jimDeviceIdentifier* pIdentifier = (jimDeviceIdentifier*)pUser;
    jimDeviceIdentifier* pGlobalIdentifier = g_pDeviceIdentifiers_776B68 +gNumDrivers_dword_77C608;

    memset(pGlobalIdentifier, 0, sizeof(jimDeviceIdentifier));

//#ifndef HARDWARE_RENDERING_FORCE
    if ((pDesc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION) && !(pDesc->dwDevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) && (pDesc->dwDeviceRenderBitDepth & DDBD_16))
    {
        if (pIdentifier->ddIdentifier.field434 == 0)
            return 1;
    }
    else
    {
        return 1;
    }
//#endif

    memcpy(&pGlobalIdentifier->deviceGUID, &pDesc->deviceGUID, sizeof(GUID));
    pGlobalIdentifier->pDeviceGUID = &pGlobalIdentifier->deviceGUID;

    strncpy(pGlobalIdentifier->pDriverDescription, lpDeviceName, 0x28);

    if (pIdentifier->pOtherGUID != 0)
    {
        memcpy(&pGlobalIdentifier->otherGUID, &pIdentifier->otherGUID, sizeof(GUID));
        pGlobalIdentifier->pOtherGUID = &pGlobalIdentifier->otherGUID;
        strncpy(pGlobalIdentifier->pDriverDescription, pIdentifier->pDriverDescription, 0x28);
    }
    memcpy(&pGlobalIdentifier->ddIdentifier, &pIdentifier->ddIdentifier, sizeof(pGlobalIdentifier->ddIdentifier));
    pGlobalIdentifier->ddIdentifier.field430 = pIdentifier->ddIdentifier.field430;
    pGlobalIdentifier->ddIdentifier.field434 = pIdentifier->ddIdentifier.field434;

    validateDeviceCaps(pDesc, lpDeviceDescription, lpDeviceName, pGlobalIdentifier);

    gNumDrivers_dword_77C608++;

    return 1;
}

// 0x51F5B8
BOOL WINAPI DDEnumCallbackEx(GUID *lpGUID, LPSTR lpDriverDescription, LPSTR /*lpDriverName*/, LPVOID /*lpContext*/, HMONITOR hm)
{
    HRESULT hr;
    IDirectDraw7* pDirectDraw;
    IDirect3D7* pDirect3D;
    DDDEVICEIDENTIFIER2 DDrawDeviceIdentifier;
    DDSCAPS2 ddCaps;
    DWORD dwMemTotal;
    DWORD dwMemFree;
    DWORD dwTexTotal;
    DWORD dwTexFree;
    jimDeviceIdentifier identifier;
    jimDeviceDDId Buf2;
    
    if (hm != 0)
        return TRUE;

    hr = DirectDrawCreateEx(lpGUID, (LPVOID*)&pDirectDraw, IID_IDirectDraw7, 0);
    if (hr < 0)
        return TRUE;

    memset(&DDrawDeviceIdentifier, 0, sizeof(DDDEVICEIDENTIFIER2));
    pDirectDraw->GetDeviceIdentifier(&DDrawDeviceIdentifier, 0);

    fprintf(gLogFile, "$DriverName   = %s\n", DDrawDeviceIdentifier.szDriver);
    fprintf(gLogFile, "$Description  = %s\n", DDrawDeviceIdentifier.szDescription);
    fprintf(gLogFile, "$DriverVersion= %i\n", DDrawDeviceIdentifier.liDriverVersion);
    fprintf(gLogFile, "$VendorId     = %i\n", DDrawDeviceIdentifier.dwVendorId);
    fprintf(gLogFile, "$DeviceId     = %i\n", DDrawDeviceIdentifier.dwDeviceId);
    fprintf(gLogFile, "$SubSysId     = %i\n", DDrawDeviceIdentifier.dwSubSysId);
    fprintf(gLogFile, "$Revision     = %i\n", DDrawDeviceIdentifier.dwRevision);
    fprintf(gLogFile, "$WHQLLevel    = %i\n", DDrawDeviceIdentifier.dwWHQLLevel);


    if (DDrawDeviceIdentifier.dwVendorId == 0x8086)
    {
        fprintf(gLogFile, "Intel device found. Do not enumerate it as a valid rendering device.\n");
    }

    hr = pDirectDraw->QueryInterface(IID_IDirect3D7, (LPVOID*)&pDirect3D);
    if (hr < 0)
    {
        pDirectDraw->Release();
        return TRUE;
    }

    // These methods report junk on modern hardware

    memset(&ddCaps, 0, sizeof(DDSCAPS2));
    ddCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
    pDirectDraw->GetAvailableVidMem(&ddCaps, &dwMemTotal, &dwMemFree);

    memset(&ddCaps, 0, sizeof(DDSCAPS2));
    ddCaps.dwCaps = DDSCAPS_TEXTURE;
    pDirectDraw->GetAvailableVidMem(&ddCaps, &dwTexTotal, &dwTexFree);

    if (!(dwMemTotal != 0 && dwTexTotal != 0))
    {
        pDirect3D->Release();
        pDirectDraw->Release();
        return TRUE;
    }

    memset(&identifier, 0, sizeof(identifier));
    strncpy(identifier.pDriverDescription, lpDriverDescription, 0x27);
    pDirectDraw->GetDeviceIdentifier(&identifier.ddIdentifier.identifier, 0);

    int result = File_msgvideocfg_Has_Device_51E29B(&identifier.ddIdentifier.identifier, &Buf2, -1);
    if (result != 0)
    {
        identifier.ddIdentifier.field430 = Buf2.field430;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            result = File_msgvideocfg_Has_Device_51E29B(&identifier.ddIdentifier.identifier, &Buf2, i);
            if (result != 0)
            {
                identifier.ddIdentifier.field430 = Buf2.field430;
                break;
            }
        }
    }

    if (dwTexTotal < 0x200000)
    {
        if ((identifier.ddIdentifier.field430 & 0x40) == 0 && (identifier.ddIdentifier.field430 & 1) == 0)
        {
            MessageBox_Error(0, 6, "Metal Gear Solid PC", MB_OK);
        }
        identifier.ddIdentifier.field430 |= 0x40; // Must mean "low vram" ?
    }
    else
    {
        identifier.ddIdentifier.field430 &= 0xBF;
    }

    pDirectDraw->EnumDisplayModes(0, 0, &identifier, EnumModesCallback);

    if (dwMemTotal < 0x300000)
        identifier.ddIdentifier.field434 &= 0xF7;
    if (dwMemTotal < 0x258000)
        identifier.ddIdentifier.field434 &= 0xFB;
    if (dwMemTotal < 0x1E0000)
        identifier.ddIdentifier.field434 &= 0xFD;

    pDirect3D->EnumDevices(Enum3DDevicesCallback, &identifier);

    pDirect3D->Release();
    pDirectDraw->Release();

    return TRUE;
}

int __cdecl jim_enumerate_devices()
{
    DWORD varC;
    int var8 = 0;
    jimDeviceDDId Dst;
    jimDeviceDDId Buf1;

    gNumDrivers_dword_77C608 = 0;
    gDriverNum_dword_77C60C = 0;
    DirectDrawEnumerateExA(DDEnumCallbackEx, 0, DDENUM_NONDISPLAYDEVICES);

    for (varC = 0; varC < gNumDrivers_dword_77C608; varC++)
    {
        (g_pDeviceIdentifiers_776B68+varC)->ddIdentifier.field430 |= 0x80;
    }

    int var4 = 0x41;
    varC = 0;
    for (;;)
    {
        if (varC >= gNumDrivers_dword_77C608)
            break;

        if (((g_pDeviceIdentifiers_776B68+varC)->ddIdentifier.field430 & var4) != 0)
        {
            memset(&Dst, 0, 0x438);
            memcpy(&Dst, &(g_pDeviceIdentifiers_776B68+varC)->ddIdentifier, 0x434);    // Copy of var18 is included by memcpying 4 bytes more
            if (File_msgvideocfg_Write_51E382(&Dst, -1) == 0)
                var8++;

            memset(g_pDeviceIdentifiers_776B68+varC, 0, 0x488);

            if (varC < gNumDrivers_dword_77C608)
            {
                int size = (gNumDrivers_dword_77C608 - (varC + 1)) * 0x488;
                memmove(g_pDeviceIdentifiers_776B68+varC, g_pDeviceIdentifiers_776B68 + varC + 1, size);
            }
            gNumDrivers_dword_77C608--;
            continue;
        }

        if ((g_pDeviceIdentifiers_776B68+varC)->pOtherGUID != 0)
        {
            (g_pDeviceIdentifiers_776B68 + varC)->pOtherGUID = &(g_pDeviceIdentifiers_776B68 + varC)->otherGUID;
        }

        if ((g_pDeviceIdentifiers_776B68 + varC)->pDeviceGUID != 0)
        {
            (g_pDeviceIdentifiers_776B68 + varC)->pDeviceGUID = &(g_pDeviceIdentifiers_776B68 + varC)->deviceGUID;
        }

        varC++;
    }

    for (varC = 0; varC < 4; varC++)
    {
        memset(&Buf1, 0, 0x438);

        if (varC < gNumDrivers_dword_77C608)
        {
            memcpy(&Buf1, &(g_pDeviceIdentifiers_776B68 + varC)->ddIdentifier, 0x434);   // Copy of var_450 included same way as earlier
            if (file_msgvideocfg_Write2(&Buf1, -1) == 1)
            {
                var8++;
            }
        }
        if (File_msgvideocfg_Write_51E382(&Buf1, varC) == 0)
        {
            var8++;
        }
    }

    if (gNumDrivers_dword_77C608 > 2)
        gNumDrivers_dword_77C608 = 2;

    dword_68C3B8 = 1;
    for (varC = 0; varC < gNumDrivers_dword_77C608; varC++)
    {
        memset(&array_689B68[dword_68C3B8], 0, 0x204);
        strncpy(array_689B68[dword_68C3B8].string, g_pDeviceIdentifiers_776B68[varC].ddIdentifier.identifier.szDescription, 0x200);
        array_689B68[dword_68C3B8].field200 = g_pDeviceIdentifiers_776B68[varC].ddIdentifier.field434;
        
        if (g_pDeviceIdentifiers_776B68[varC].ddIdentifier.field430 & 2)
        {
            array_689B68[dword_68C3B8].field200 |= 0x10;
        }

        dword_68C3B8++;
    }

    if (gNumDrivers_dword_77C608 != 0)
    {
        if (var8 != 0)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    return 0;
}


//MSG_FUNC_NOT_IMPL(0x0041ECB0, signed int __cdecl(), InitD3d_ProfileGfxHardware_41ECB0);
signed int Render_sub_41E3C0();

signed int CC InitD3d_ProfileGfxHardware_41ECB0()
{
    signed int result; // eax@41
    int v1; // edx@115
    float v2; // STB4_4@163
    DDPIXELFORMAT pixelFormat; // [sp+C8h] [bp-388h]@142
    DDSURFACEDESC2 dxSurfaceDesc3; // [sp+E8h] [bp-368h]@142
    DDSCAPS2 dxCaps1; // [sp+164h] [bp-2ECh]@104

    DDCAPS_DX7 dxCaps; // [sp+198h] [bp-2B8h]@114  sizeof = 0x17C
    GUID* v33; // [sp+314h] [bp-13Ch]@1
    int v34; // [sp+318h] [bp-138h]@3
    unsigned int i; // [sp+320h] [bp-130h]@34
    DDSURFACEDESC2 dxSurfaceDesc; // [sp+328h] [bp-128h]@86
    int v42; // [sp+3A4h] [bp-ACh]@30
    DDSURFACEDESC2 dxSurfaceDesc2; // [sp+3B0h] [bp-A0h]@70 = DDSURFACEDESC2 (sizeof = 0x7C)
    D3DVIEWPORT7 dxViewport; // [sp+42Ch] [bp-24h]@163
    HRESULT hr; // [sp+444h] [bp-Ch]@40
    GUID *lpGuid; // [sp+448h] [bp-8h]@1
    int v55; // [sp+44Ch] [bp-4h]@1

    lpGuid = 0;
    v33 = 0;

    pixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    pixelFormat.dwFlags = DDPF_ALPHAPIXELS | DDPF_RGB;
    pixelFormat.dwFourCC = 0;
    pixelFormat.dwRGBBitCount = 16;
    pixelFormat.dwRBitMask = 0x7C00;
    pixelFormat.dwGBitMask = 0x03E0;
    pixelFormat.dwBBitMask = 0x001F;
    pixelFormat.dwRGBAlphaBitMask = 0x8000;

    static_assert(sizeof(DDPIXELFORMAT) == 32, "wrong pixel format size");

    DDPIXELFORMAT pixelFormat2 = pixelFormat;
    DDPIXELFORMAT pixelFormat3 = pixelFormat;


    gFile = fopen("profile.log", "w");
    fputs("InitAll {\n", gFile);
    fflush(gFile);
    gLogFile = gFile;
    Input_Start_42D69E();
    fputs("jim_enumerate_devices()\n", gFile);
    fflush(gFile);
    v55 = jim_enumerate_devices();
    if (!v55)
    {
        gSoftwareRendering = 1;
        dword_716F5C = 1.0f;
        gXRes = 1.0f;
        fputs("No hardware rendering devices were enumerated\n", gFile);
        fflush(gFile);
        fputs(" #entering software mode\n", gFile);
        fflush(gFile);
    }
    v34 = 0;
    if (gWindowedMode)
        v34 = 1;
    fputs("jim_read_config_from_file\n", gFile);
    fflush(gFile);
    if (ParseMsgCfg_4331D4())
    {
        dword_651CF8 = 0;
        fputs(" . done\n", gFile);
        fflush(gFile);
        if (dword_716F78 == 1)
        {
            if (gCrashCheck)
            {
                MessageBoxA(
                    0,
                    "Game crashed during previous initialization, game starting software rendering mode...",
                    "Metal Gear Solid",
                    MB_OK);
                gSoftwareRendering = 1;
                dword_716F5C = 1.0f;
                gXRes = 1.0f;
            }
        }
    }
    else
    {
        dword_651CF8 = 1;
        dword_716F78 = 0;
        fputs(" . fail\n", gFile);
        fflush(gFile);
    }
    if (gCrashCheck)
    {
        dword_716F78 = 1;
        sub_433801();
    }
    if (!dword_651CF8 && (v55 == -1))
        {
            MessageBoxA(
                0,
                "Your video configuration has been updated - your system will be re-profiled\n",
                "Metal Gear Solid (PC)",
                MB_OK);
            v55 = 0;
            if (sub_41EC40())
            {
                gSoftwareRendering = 0;
                gXRes = 2.0f;
            }
            else
            {
                gSoftwareRendering = 1;
                lpGuid = 0;
                gXRes = 1.0f;
                dword_716F5C = 1.0f;
                MessageBox_Error(0, 5, "Metal Gear Solid PC", MB_OK);
            }
            gWindowedMode = 0;
            sub_433801();
            v34 = gWindowedMode;
        dword_651CF8 = ParseMsgCfg_4331D4() == 0;
    }
    CheckJoyStickError_43C850();
    if (dword_651CF8)
    {
        gSoundFxVol_dword_651D98 = 100;
        gMusicVol_dword_716F68 = 100;
        fputs("Executing system profiling sequence.\n", gFile);
        fflush(gFile);
        fputs("Choosing default 3D-accelerator\n", gFile);
        fflush(gFile);
        if (sub_41EC40())
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
            fprintf(gFile, " getting selected driver No %d from %d available\n", gDriverNum_dword_77C60C + 1, gNumDrivers_dword_77C608);
            gXRes = 2.0f;
            gWindowedMode = 0;
            gSoftwareRendering = 0;
        }
        else
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            fputs(" #entering software mode\n", gFile);
            fflush(gFile);
            gSoftwareRendering = 1;
            gWindowedMode = 0;
            dword_716F5C = 1.0f;
            gXRes = 1.0f;
            lpGuid = 0;
            MessageBox_Error(0, 5, "Metal Gear Solid PC", MB_OK);
        }
        sub_431C63();
    }
    else
    {
        gDriverNum_dword_77C60C = dword_716F74;
    }
    if (v34)
    {
        gDriverNum_dword_77C60C = 0;
        gXRes = 2.0f;
    }
    if (gSoftwareRendering)
    {
        v42 = gWindowedMode;
    }
    else
    {
        sub_51F1E1(&lpGuid, &v33);
        v42 = 0;
        gWindowedMode = 0;
    }
    if (v34)
    {
        v42 = 1;
        gWindowedMode = 1;
    }
    fputs("\n(i) List of enumerated devices:\n", gFile);
    fflush(gFile);
    for (i = 0; i < gNumDrivers_dword_77C608; ++i)
    {
        fprintf(gFile, "pDriverGUID %x, pDeviceGUID %x\n", (g_pDeviceIdentifiers_776B68 + i)->pDeviceGUID, (g_pDeviceIdentifiers_776B68 + i)->deviceGUID);
        fprintf(gFile, "D3DDevice description : %s", (g_pDeviceIdentifiers_776B68 + i)->ddIdentifier.identifier.szDescription);

        if (gDriverNum_dword_77C60C == i)
        {
            fputs("   /selected/\n", gFile);
            fflush(gFile);
        }
        else
        {
            fputs("\n", gFile);
            fflush(gFile);
        }
    }
    for (;;)
    {
        g_dwDisplayWidth_6DF214 = static_cast<DWORD>(320.0 * gXRes);
        g_dwDisplayHeight = static_cast<DWORD>(240.0 * gXRes);
        fputs("Creating DirectDraw7\n", gFile);
        fflush(gFile);

        if (IsMgsi())
        {
            // Call the games DD create
            hr = DirectDrawCreateExMGS(lpGuid, (LPVOID*)&g_pDirectDraw, &IID_IDirectDraw7, 0);
        }
        else
        {
            // Since we're not hosted in the game call the winapi to get a dd7 instance
            hr = CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_ALL, IID_IDirectDraw7, (void**)&g_pDirectDraw);
            if (!FAILED(hr))
            {
                hr = g_pDirectDraw->Initialize(NULL);
            }
        }
    
        if (hr < 0)
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            return 0;
        }
        fputs(" . done\n", gFile);
        fflush(gFile);
        if (!gSoftwareRendering)
        {
            fputs("Query interface...\n", gFile);
            fflush(gFile);
            g_pDirectDraw->QueryInterface(IID_IDirect3D7, (LPVOID*)&g_pDirect3D);
            if (hr < 0)
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                gSoftwareRendering = 1;
                gXRes = 1.0f;
                g_dwDisplayWidth_6DF214 = static_cast<DWORD>(320.0 * 1.0f);
                g_dwDisplayHeight = static_cast<DWORD>(240.0 * 1.0f);
                MessageBox_Error(0, 4, "Metal Gear Solid PC", MB_OK);
            }
            fputs(" . done\n", gFile);
            fflush(gFile);
        }
        if (gSoftwareRendering)
        {
            if (gXRes >= 1.1)
            {
                gXRes = 2.0f;
                dword_650D2C = 17;
            }
            else
            {
                gXRes = 1.0f;
                dword_650D2C = 16;
            }
        }
        if (gPrimarySurface_6FC734)
        {
            hr = gPrimarySurface_6FC734->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release primary surf", hr);
            }
            gPrimarySurface_6FC734 = 0;
        }
        if (g_pBackBuffer_6FC738)
        {
            hr = g_pBackBuffer_6FC738->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release render surf", hr);
            }
            g_pBackBuffer_6FC738 = 0;
        }
        if (g_pClipper)
        {
            hr = g_pClipper->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release clipper", hr);
            }
            g_pClipper = 0;
        }
        gPrimarySurface_6FC734 = 0;
        g_pBackBuffer_6FC738 = 0;
        g_pClipper = 0;
        fputs("Setting cooperative level...\n", gFile);
        fflush(gFile);
        if (v42)
        {
            fputs(" (windowed) \n", gFile);
            fflush(gFile);
            hr = g_pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_FPUPRESERVE | DDSCL_MULTITHREADED | DDSCL_NORMAL);
        }
        else
        {
            fputs(" (full-screen) \n", gFile);
            fflush(gFile);
            hr = g_pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_FPUPRESERVE | DDSCL_MULTITHREADED | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
        }
        if (FAILED(hr))
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            return 0;
        }
        fputs(" . done\n", gFile);
        fflush(gFile);
        if (!v42)
        {
            hr = g_pDirectDraw->SetDisplayMode(g_dwDisplayWidth_6DF214, g_dwDisplayHeight, 0x10, 0, 0);
            fprintf(gLogFile, "SetDisplayMode( %d, %d )\n", g_dwDisplayWidth_6DF214, g_dwDisplayHeight);
            if (hr < 0)
                return 0;
        }
        memset(&dxSurfaceDesc2, 0, 124);
        dxSurfaceDesc2.dwSize = 124;
        if (v42 || gSoftwareRendering)
        {
            dxSurfaceDesc2.dwFlags = DDSD_CAPS;
            dxSurfaceDesc2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        }
        else
        {
            dxSurfaceDesc2.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
            dxSurfaceDesc2.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
            dxSurfaceDesc2.dwBackBufferCount = 1;
        }

        memcpy(&dxSurfaceDesc2.ddpfPixelFormat, &pixelFormat2, sizeof(DDPIXELFORMAT));

        fputs("Creating primary surface...\n", gFile);
        fflush(gFile);
        hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc2, &gPrimarySurface_6FC734, 0);
        if (hr < 0)
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            return 0;
        }
        fputs(" . done\n", gFile);
        fflush(gFile);

        MakeFonts();
        if (dword_651CF8)
        {
            Render_sub_41CD70();
            Sleep(0x7D0u);
        }
        if (gWindowedMode)
        {
            fputs("Creating clipper...\n", gFile);
            fflush(gFile);
            hr = g_pDirectDraw->CreateClipper(0, &g_pClipper, 0);
            if (FAILED(hr))
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                PrintDDError("Can't create clipper", hr);
                return 0;
            }
            hr = g_pClipper->SetHWnd(0, gHwnd);
            if (FAILED(hr))
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                PrintDDError("Can't obtain clipper zone", hr);
                return 0;
            }
            hr = gPrimarySurface_6FC734->SetClipper(g_pClipper);
            if (FAILED(hr))
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                PrintDDError("Can't attach clipper", hr);
                return 0;
            }
            g_pClipper->Release();
            fputs(" . done\n", gFile);
            fflush(gFile);
        }
        memset(&dxSurfaceDesc, 0, 124);
        dxSurfaceDesc.dwSize = 124;
        if (gSoftwareRendering)
        {
            if (dword_651CF8 || (dword_716F6C && dword_716F6C != 1))
            {
                fputs("Testing software render speed to system and to video surface\n", gFile);
                fflush(gFile);
                if (Render_sub_41CE20())
                {
                    fputs(" . rendering to video surface is faster\n", gFile);
                    fflush(gFile);
                    dword_716F6C = 1;
                    dxSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_OFFSCREENPLAIN;
                }
                else
                {
                    fputs(" . rendering to system memory surface is faster\n", gFile);
                    fflush(gFile);
                    dword_716F6C = 0;
                    dxSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
                }
            }
            else
            {
                if (dword_716F6C)
                    dxSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_OFFSCREENPLAIN;
                else
                    dxSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
            }
            dxSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
            dxSurfaceDesc.dwWidth = g_dwDisplayWidth_6DF214;
            dxSurfaceDesc.dwHeight = g_dwDisplayHeight;

            fputs("Creating back buffer for software rendering...\n", gFile);
            fflush(gFile);


            memcpy(&dxSurfaceDesc.ddpfPixelFormat, &pixelFormat3, sizeof(DDPIXELFORMAT));

            hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc, &g_pBackBuffer_6FC738, 0);
            if (hr < 0)
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                return 0;
            }
            fputs(" . done\n", gFile);
            fflush(gFile);
        }
        else
        {
            if (v42)
            {
                dxSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                dxSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN;
                dxSurfaceDesc.dwWidth = g_dwDisplayWidth_6DF214;
                dxSurfaceDesc.dwHeight = g_dwDisplayHeight;
                fputs("Creating back buffer for windowed mode...\n", gFile);
                fflush(gFile);
                hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc, &g_pBackBuffer_6FC738, 0);
                if (hr < 0)
                {
                    fputs(" . fail\n", gFile);
                    fflush(gFile);
                    return 0;
                }
                fputs(" . done\n", gFile);
                fflush(gFile);
            }
            else
            {
                dxCaps1.dwCaps = DDSCAPS_BACKBUFFER;
                dxCaps1.dwCaps2 = 0;
                dxCaps1.dwCaps3 = 0;
                dxCaps1.dwCaps4 = 0;
                fputs("Getting back buffer from pPrim chain...\n", gFile);
                fflush(gFile);
                gPrimarySurface_6FC734->GetAttachedSurface(&dxCaps1, &g_pBackBuffer_6FC738);
                if (hr < 0)
                {
                    fputs(" . fail\n", gFile);
                    fflush(gFile);
                    return hr;
                }
                fputs(" . done\n", gFile);
                fflush(gFile);
            }
        }
        fputs("Restoring surfaces...\n", gFile);
        fflush(gFile);
        Render_RestoreAll();
        if (FAILED(hr))
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            PrintDDError("Restoring caput", hr);
        }
        else
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
        }
        fputs("Querying gamma interface...\n", gFile);
        fflush(gFile);
        gPrimarySurface_6FC734->QueryInterface(IID_IDirectDrawGammaControl, (LPVOID*)&g_pGammaControl);
        if (FAILED(hr))
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            PrintDDError("Can't get GammaControl interface", hr);
            dword_6FC7C4 = 0;
        }
        else
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
            dword_6FC7C4 = 1;
        }
        if (g_pDirectDraw)
        {
            memset(&dxCaps, 0, 380);
            dxCaps.dwSize = 380;
            hr = g_pDirectDraw->GetCaps(&dxCaps, 0);
            if (hr || (v1 = dxCaps.dwCaps2, !(v1 & 0x20000)))
                dword_6FC7C4 = 0;
        }
        g_surface565Mode = Render_sub_41D1D0();
       // fprintf(gFile, "565 mode = %i\n", g_surface565Mode);
        
        if (gSoftwareRendering)
            break;
        dxSurfaceDesc.dwSize = 124;
        g_pDirectDraw->GetDisplayMode(&dxSurfaceDesc);
        if (dxSurfaceDesc.ddpfPixelFormat.dwRGBBitCount <= 8)
        {
            fputs("Can't render to a palettized surface, exiting.\n", gFile);
            fflush(gFile);
            return 0;
        }
        fputs("Screen mode is ok\n", gFile);
        fflush(gFile);
        fputs("Creating device...\n", gFile);
        fflush(gFile);

        hr = g_pDirect3D->CreateDevice(*v33, g_pBackBuffer_6FC738, &gD3dDevice_6FC74C);

        if (SUCCEEDED(hr))
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
            Render_SetRenderState_422A90(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
            Render_SetRenderState_422A90(D3DRENDERSTATE_DITHERENABLE, 0);
            if (!gSoftwareRendering)
            {

                pixelFormat.dwSize = sizeof(DDPIXELFORMAT);
                pixelFormat.dwFlags = DDPF_ALPHAPIXELS | DDPF_RGB;
                pixelFormat.dwFourCC = 0;
                pixelFormat.dwRGBBitCount = 16;
                pixelFormat.dwRBitMask = 0x7C00;
                pixelFormat.dwGBitMask = 0x03E0;
                pixelFormat.dwBBitMask = 0x001F;
                pixelFormat.dwRGBAlphaBitMask = 0x8000;

                memset(&dxSurfaceDesc3, 0, 124);
                dxSurfaceDesc3.dwSize = 124;
                dxSurfaceDesc3.dwFlags = DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
                memcpy(&dxSurfaceDesc3.ddpfPixelFormat, &pixelFormat, sizeof(DDPIXELFORMAT));
                dxSurfaceDesc3.dwWidth = 16;
                dxSurfaceDesc3.dwHeight = 16;
                dxSurfaceDesc3.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
                dxSurfaceDesc3.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;

                hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc3, &g_pDDSurface_6FC740, 0);
                if (FAILED(hr))
                {
                    g_pDDSurface_6FC740 = 0;
                }
                else 
                {
                    if (!ClearDDSurfaceWhite_41E990())
                    {
                        g_pDDSurface_6FC740->Release();
                        g_pDDSurface_6FC740 = 0;
                    }
                }
            }

            Render_InitTextureStages_422BC0(0, D3DTSS_COLORARG1, 2);
            Render_InitTextureStages_422BC0(0, D3DTSS_COLOROP, 4);
            Render_InitTextureStages_422BC0(0, D3DTSS_COLORARG2, 0);
            Render_InitTextureStages_422BC0(0, D3DTSS_ALPHAARG1, 2);
            Render_InitTextureStages_422BC0(0, D3DTSS_ALPHAOP, 4);
            Render_InitTextureStages_422BC0(0, D3DTSS_ALPHAARG2, 0);

            if (Render_sub_41E3C0())
            {
                fprintf(gFile, "Blend modes = %i \n", gBlendMode);
                fprintf(gFile, "Alpha modulate = %i \n", gAlphaModulate_dword_6FC798);
                gColourKey = Render_sub_41E730();
                fprintf(gFile, "ColorKey = %i\n", gColourKey);
                Render_InitTextureStages_422BC0(0, D3DTSS_ADDRESS, 3);
                
                if (gModX2 == 2)
                {
                    gModX2 = Render_sub_41D420();
                }

                fprintf(gFile, "MODULATE2X = %i \n", gModX2);
                if (gColourKey)
                {
                    Render_SetRenderState_422A90(D3DRENDERSTATE_COLORKEYENABLE, 1);
                }
                else
                {
                    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHATESTENABLE, 1);
                    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHAREF, 127);
                    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
                }
                Render_SetRenderState_422A90(D3DRENDERSTATE_DITHERENABLE, 1);
                if (dword_651CF8)
                {
                    if (gLowRes != gLowRes) // FIX ME: This can't be right
                    {
                        if (!gLowRes)
                        {
                            fputs("Not enough video memory for high resolution textures, disabling.\n", gFile);
                            fflush(gFile);
                        }
                    }
                    if (gNoEffects)
                    {
                        fputs("Water and ninja effects are on\n", gFile);
                        fflush(gFile);
                    }
                    else
                    {
                        fputs("Water and ninja effects are unsupported by hardware, disabling.\n", gFile);
                        fflush(gFile);
                    }
                    if (gModX2)
                    {
                        fputs("Modulate by 2 works correctly, enabling.\n", gFile);
                        fflush(gFile);
                    }
                    else
                    {
                        fputs("Modulate by 2 doesn't work correctly or unsupported, disabling\n", gFile);
                        fflush(gFile);
                    }
                }
                dxViewport.dwX = 0;
                dxViewport.dwY = 0;
                dxViewport.dwWidth = g_dwDisplayWidth_6DF214;
                dxViewport.dwHeight = g_dwDisplayHeight;
                dxViewport.dvMinZ = 0;
                dxViewport.dvMaxZ = 1.0f;
                gD3dDevice_6FC74C->SetViewport(&dxViewport);
                v2 = ((float)dword_651D94 - 50.0f) / 100.0f;
                Render_SetBrightness_sub_41C820(v2);
            }
            else
            {
                gD3dDevice_6FC74C->Release();
                gD3dDevice_6FC74C = nullptr;
                MessageBox_Error(0, 5, "Metal Gear Solid PC", MB_OK);
                gSoftwareRendering = 1;
            }
            break;
        }
        fputs("D3D:CreateDevice() failed, switching to SOFTWARE MODE\n", gFile);
        fflush(gFile);
        g_dwDisplayWidth_6DF214 = 320;
        g_dwDisplayHeight = 240;
        fprintf(gLogFile, "Resetting DisplayMode to ( %d, %d )\n", g_dwDisplayWidth_6DF214, g_dwDisplayHeight);
        MessageBox_Error(0, 4, "Metal Gear Solid PC", MB_OK);
        gSoftwareRendering = 1;
        dword_716F5C = 1.0f;
        gXRes = dword_716F5C; // TODO: Float
        hr = g_pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_NORMAL);
        if (g_pGammaControl)
        { 
            g_pGammaControl->Release();
        }

        if (g_pBackBuffer_6FC738)
        {
            hr = g_pBackBuffer_6FC738->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release render surf", hr);
            }
            g_pBackBuffer_6FC738 = 0;
        }
        if (gPrimarySurface_6FC734)
        {
            hr = gPrimarySurface_6FC734->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't relaese primary surf", hr);
            }
            gPrimarySurface_6FC734 = 0;
        }
        if (g_pClipper)
        {
            hr = g_pClipper->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release clipper", hr);
            }
            g_pClipper = 0;
        }
        gPrimarySurface_6FC734 = 0;
        g_pBackBuffer_6FC738 = 0;
        g_pClipper = 0;
        if (g_pDirect3D)
        {
            g_pDirect3D->Release();
            g_pDirect3D = 0;
        }
        if (g_pDirectDraw)
        {
            g_pDirectDraw->Release();
            g_pDirectDraw = 0;
        }
    }
    fputs("other inits\n", gFile);


    fflush(gFile);


    for (i = 0; i < 1500; ++i)
    {
        gTextures_6C0F00[i].mSurface = 0;
        gTextures_6C0F00[i].field_20 = 0;
        gTextures_6C0F00[i].field_24_flagsQ = 0;
    }
    
    dword_6FC7C0 = Render_TextureScratchAlloc_41CA80() == 0;
    if (dword_6FC7C0)
    {
        gNoEffects = 0;
        dword_716F60 = 0;
    }
    Create_Arial_Font_423F1B(0, static_cast<int>(14.0 * gXRes));
    Render_Load_mlog_LCDN_10_Textures_41EA60();
    if (!gSoftwareRendering)
    {
        gPrimBuffer_dword_6C0EFC = (prim_struct*)malloc(150000 * sizeof(prim_struct));
        for (i = 0; i < 15000; ++i)
        {
            gPrimBuffer_dword_6C0EFC[i].field_0 = 0;
        }
        g_pMGSVertices_6FC780 = (MGSVertex*)malloc(sizeof(MGSVertex) * 150000);
    }
    gImageBufer_dword_6FC728 = (DWORD*)malloc(0x100000u);
    if (gImageBufer_dword_6FC728)
    {
        memset(gImageBufer_dword_6FC728, 0, 0x100000u);
        g_pwTextureIndices_6FC72C = (WORD*)malloc(1024*512*2); // Index per pixel of PSX VRAM
        if (gImageBufer_dword_6FC728)
        {
            memset(gImageBufer_dword_6FC728, -1, 0x100000u);
            _cfltcvt_init();
            memset(unk_6C0778, 0, 0x400u);
            dword_6DEF7C = malloc(0x200u);
            dword_6DEF90 = malloc(0x200u);
            memset(dword_6DEF7C, 0, 0x100u);
            fputs("jim_write_configuration_to_file()\n", gFile);
            fflush(gFile);
            sub_433801();
            fputs("InitAll }\n", gFile);
            fflush(gFile);
            fclose(gFile);
            result = 1;
        }
        else
        {
            PrintDDError("$edq Out of memory", 0);
            fclose(gFile);
            result = 0;
        }
    }
    else
    {
        PrintDDError("$edq Out of memory", 0);
        fclose(gFile);
        result = 0;
    }
    return result;
}
MGS_FUNC_IMPL(0x0041ECB0, InitD3d_ProfileGfxHardware_41ECB0);


// 0x00420810
signed int __cdecl DoInitAll()
{
    const auto ret = InitD3d_ProfileGfxHardware_41ECB0();
    MessageBox_Error(gHwnd, -1, "Metal Gear Solid PC", MB_OK);
    return ret;
}
MGS_FUNC_IMPL(0x00420810, DoInitAll);


// HACK: Prevents game init delay where DDRAW GetDeviceIdentifier checks if the driver is signed
// which then takes 8-12 seconds to complete.
LONG WINAPI Hook_WinVerifyTrust(HWND hwnd, GUID* pgActionID, LPVOID pWVTData)
{
    return 0;
}

// The varadic template hook class can't also mixing in varadic C functions, so we have too hook these manually
// good news is that these kind of functions are rare.
void InstallVaradicCFunctionHooks()
{
    LONG err = DetourTransactionBegin();

    if (err != NO_ERROR)
    {
        abort();
    }

    err = DetourUpdateThread(GetCurrentThread());

    if (err != NO_ERROR)
    {
        abort();
    }
    
    FARPROC winTrust = GetProcAddress(LoadLibraryA("WinTrust.dll"), "WinVerifyTrust");
    err = DetourAttach(&(PVOID&)winTrust, Hook_WinVerifyTrust);
    if (err != NO_ERROR)
    {
        abort();
    }

    err = DetourTransactionCommit();
    if (err != NO_ERROR)
    {
        abort();
    }
}


MGS_VAR(1, 0x791A08, int, gActiveBuffer_dword_791A08, 0);
MGS_VAR(1, 0x650110, int, gLastActiveBuffer_dword_650110, 0);
MGS_VAR(1, 0x6BED20, DWORD, counter_dword_6BED20, 0);

MGS_FUNC_NOT_IMPL(0x4021F2, int(), sub_4021F2);

//MGS_FUNC_NOT_IMPL(0x40162D, signed int __cdecl(int activeBuffer), OT_Related_40162D);
#define OT_POINTER_BITS OT_END_TAG

// TODO: This needs a lot of cleaning up + tests adding, seems to work OK for now
void CC OT_Related_40162D(int activeBuffer)
{
    int gv0OtPtr = (int)&gLibGvStruct0_6BC180.mOrderingTables[activeBuffer];// ot addr?
    for (int i=0; i<3; i++)
    {
        struct_gv* pCurGV = nullptr;
        if (i == 0)
        {
            pCurGV = &gLibGvStruct0_6BC180;
        }
        else if (i == 1)
        {
            pCurGV = &gLibGVStruct1_6BC36C;
        } 
        else if (i == 2)
        {
            pCurGV = &gLibGvStruct2_6BC558;
        }
        DWORD** curGVOtPtr = &pCurGV->mOrderingTables[activeBuffer];// pick ot1 or ot2 ?
        DWORD* pActiveBufArray = activeBuffer ? (DWORD*)pCurGV->dword_6BC498 : (DWORD*)pCurGV->dword_6BC458;

        char otSize = (pCurGV->word_6BC374_8 & 0xFF);
        int pOtEnd = (int)&(*curGVOtPtr)[1 << otSize];
        Renderer_ClearOTag(*curGVOtPtr, (1 << otSize) + 1);
        
        if (pCurGV->word_6BC37A_0_1EC_size > 0)
        {
            pCurGV->dword_6BC3C8_pStructure_rect = pCurGV->dword_6BC3D0_rect;

            // The -32's are picking 2 arrays of 16 before the pointed to array
            memcpy(pActiveBufArray - 32, activeBuffer ? &pCurGV->dword_6BC518_src_offsetted_dr_evn : &pCurGV->dword_6BC4D8_src_dr_env1 , sizeof(DR_ENV));
            pCurGV->word_6BC37A_0_1EC_size--;
        }

        *(pActiveBufArray - 32) ^= (pOtEnd ^ *(pActiveBufArray - 32)) & OT_POINTER_BITS;
        **curGVOtPtr ^= (**curGVOtPtr ^ (unsigned int)pActiveBufArray) & OT_POINTER_BITS;

        if (pCurGV->word_6BC376_16 < 0)
        {
            *pActiveBufArray |= OT_POINTER_BITS;
        }
        else
        {
            DWORD* tmp = (DWORD *)(*(DWORD *)gv0OtPtr + 4 * pCurGV->word_6BC376_16);
            *pActiveBufArray ^= (*tmp ^ *pActiveBufArray) & OT_POINTER_BITS;
            *tmp ^= (*tmp ^ (unsigned int)(pActiveBufArray - 32)) & OT_POINTER_BITS;
        }
    }
}
MGS_FUNC_IMPLEX(0x40162D, OT_Related_40162D, true); // TODO

void CC Main_sub_401C02()
{
    if (gSoftwareRendering)
    {
        // SW rendering path not implemented
        MGS_FATAL("Software rendering path not implemented");
    }
    else
    {
        const int activeBufferHW = gActiveBuffer_dword_791A08;
        if (counter_dword_6BED20 <= 0)
        {
            if (gLastActiveBuffer_dword_650110 < 0 || gActiveBuffer_dword_791A08 != gLastActiveBuffer_dword_650110)
            {
                gDispEnv_6BECF0.disp.x1 = static_cast<WORD>(dword_6BED18[gActiveBuffer_dword_791A08]);
                Render_Scene_DispEnv_40DD00(&gDispEnv_6BECF0);
                Render_DrawIndex_401619(1 - activeBufferHW);
                gLastActiveBuffer_dword_650110 = -1;
            }
        }
        else
        {
            if (gLastActiveBuffer_dword_650110 < 0)
            {
                gLastActiveBuffer_dword_650110 = gActiveBuffer_dword_791A08;
            }
            --counter_dword_6BED20;
        }
        System_HouseKeeping_40ACB2(activeBufferHW);
        System_HouseKeeping_40ACB2(2);
        Menu_TextReset_459ACE();
        OT_Related_40162D(activeBufferHW);                 // calls ClearOTag
        sub_4021F2();
    }
}
MGS_FUNC_IMPL(0x401C02, Main_sub_401C02);





signed int __cdecl Main()
{
    signed int result = 0;

    Resetgraph_AndPrintPsxStructureSizes(0);
    SetGraphDebug(0);
    //CdInit_44AC80();
    SetDispMask(0);
    PSX_RECT clearRect;
    clearRect.x1 = 0;
    clearRect.y1 = 0;
    clearRect.x2 = 1024;
    clearRect.y2 = 511;
    ClearImage(&clearRect, 0, 0, 0);
    //DrawSync_44AB50();
    SetDispMask(1);
    //InitGeom_14();
    //VSyncInit_13();
    //GetNewVblControlTable_8();
    MemCardsInit();
    // PadInit_9();
    GV_Init_40A4F6();
    FS_Init_40907E();
    DG_Init_40111A();
    GCL_Init_4090A7();
    HZD_Init_40B725();
    GM_Init_44E12B();

    for (;;)
    {
        result = MainLoop_51C9A2();
        if (!result)
        {
            break;
        }

        if (gExitMainGameLoop)
        {
            break;
        }

        // HACK: Sometimes the game crashes somewhere deep in here, not calling this seems to prevent the game
        // state from progressing.
        // In software rendering mode when game over it will crash, but this is an existing bug of the game.
        Actor_UpdateActors_40A1BF();

        //PsxGpuDebug_44A4D0();
    }
    return result;
}
MGS_FUNC_IMPLEX(0x00401005, Main, WINMAIN_IMPL);

// 0x00401000
//MSG_FUNC_NOT_IMPL(0x00401000, int __cdecl(), DoMain);
int __cdecl DoMain()
{
    return Main();
}

MGS_VAR(1, 0x6FC73C, IUnknown*, dword_6FC73C, nullptr); // TODO: Check what this is

// 00423020
void __cdecl ClearAll()
{
    puts(" *************************** CLEAR ALL START *************************");
    if (gNumTextures_word_6FC78C)
    {
        for (int i = 0; i < gNumTextures_word_6FC78C; ++i)
        {
            if (gTextures_6C0F00[i].mSurfaceType == 5)
            {
                Font_TrueTypeLinesFree_4241A4(reinterpret_cast<FontTextLine*>(gTextures_6C0F00[i].mSurface));
            }
            else if (!gSoftwareRendering)
            {
                if (gTextures_6C0F00[i].mSurface)
                {
                    gTextures_6C0F00[i].mSurface->Release();
                    gTextures_6C0F00[i].mSurface = 0;
                }
            }
        }
    }

    if (dword_6FC73C)
    {
        dword_6FC73C->Release();
        dword_6FC73C = nullptr;
    }

    if (gD3dDevice_6FC74C)
    {
        gD3dDevice_6FC74C->Release();
        gD3dDevice_6FC74C = nullptr;
    }

    if (g_pBackBuffer_6FC738)
    {
        g_pBackBuffer_6FC738->Release();
        g_pBackBuffer_6FC738 = nullptr;
    }

    if (g_pClipper)
    {
        g_pClipper->Release();
        g_pClipper = nullptr;
    }

    if (gPrimarySurface_6FC734)
    {
        gPrimarySurface_6FC734->Release();
        gPrimarySurface_6FC734 = nullptr;
    }

    if (g_pDirect3D)
    {
        g_pDirect3D->Release();
        g_pDirect3D = nullptr;
    }

    if (g_pDirectDraw)
    {
        g_pDirectDraw->Release();
        g_pDirectDraw = nullptr;
    }

    if (g_pDDSurface_6FC740)
    {
        g_pDDSurface_6FC740->Release();
        g_pDDSurface_6FC740 = nullptr;
    }

    if (g_pMGSVertices_6FC780)
    {
        free(g_pMGSVertices_6FC780);
        g_pMGSVertices_6FC780 = nullptr;
    }

    if (gPrimBuffer_dword_6C0EFC)
    {
        free(gPrimBuffer_dword_6C0EFC);
        gPrimBuffer_dword_6C0EFC = nullptr;
    }

    if (dword_6DEF7C)
    {
        free(dword_6DEF7C);
        dword_6DEF7C = nullptr;
    }

    if (dword_6DEF90)
    {
        free(dword_6DEF90);
        dword_6DEF90 = nullptr;
    }

    free(gImageBufer_dword_6FC728);
    free(g_pwTextureIndices_6FC72C);
}

void __cdecl DoClearAll()
{
    ClearAll();
}
MGS_FUNC_IMPL(0x004232B0, DoClearAll);

// 0x0051D180
void ShutdownEngine()
{
    if (gHwnd)
    {
        Fonts_Release_sub_4317B3();
        Input_Shutdown_sub_43C716();
        Sound_ShutDown();
        DoClearAll();
        Task_TerminateQ();
        DestroyWindow(gHwnd);
    }
}

#include <gmock/gmock.h>

static void RunTests()
{
    DoScriptTests();
    DoTestSystem();
    DoResourceNameHashTest();
    DoActor_RankTests();
    DoLibGv_Tests();
    DoMovie_Tests();
    DoFsTests();
    DoPsxTests();
    DoMenuTests();
    DoFontTests();
    DoDGTests();
    DoKmdTests();
    DoMapTests();
}

int Sink()
{
    return 0;
}

void ReplaceStdLib()
{
    if (!IsMgsi())
    {
        return;
    }

   // MGS_REDIRECT(0x005398F0, printf);
    MGS_REDIRECT(0x00539990, malloc);
    MGS_REDIRECT(0x00539DA0, calloc);
    MGS_REDIRECT(0x00539E20, realloc);
    MGS_REDIRECT(0x0053A400, free);

   // MGS_REDIRECT(0x0053DBE0, Sink);
    /*
    MGS_REDIRECT(0x0053C170, srand);
    MGS_REDIRECT(0x0053C180, rand);
    MGS_REDIRECT(0x0053C1C0, puts);
    
    MGS_REDIRECT(0x0053C4A0, fclose);
    MGS_REDIRECT(0x0053C5F0, fprintf);
    MGS_REDIRECT(0x0053C6C0, fflush);
    MGS_REDIRECT(0x0053C970, fputs);
    MGS_REDIRECT(0x0053CB40, fopen);
    MGS_REDIRECT(0x0053CD00, fseek);
    MGS_REDIRECT(0x0053CEA0, fread); // LockedFileRead
    MGS_REDIRECT(0x0053CEE0, fread);
    MGS_REDIRECT(0x0053D680, close);
    MGS_REDIRECT(0x0053DBE0, open);
    MGS_REDIRECT(0x0053E180, lseek);
    MGS_REDIRECT(0x0053F3D0, fgets);
    MGS_REDIRECT(0x0053F510, fwrite); // LockedFileWrite
    MGS_REDIRECT(0x0053F550, fwrite);
    MGS_REDIRECT(0x0053F820, ftell);
  
    MGS_REDIRECT(0x0053FB30, _findfirst);
    MGS_REDIRECT(0x0053FC70, _findnext);
    MGS_REDIRECT(0x0053FDA0, _findclose);
    */



    //MGS_REDIRECT(0x0053D680, close);
    //MGS_REDIRECT(0x0053D1A0, read);

    // Not std lib - just varadic
   // MGS_REDIRECT(0x00520157, DebugLog);

    /*
    MGS_REDIRECT(0x00540040, vsprintf);
    MGS_REDIRECT(0x00549920, _snprintf);
    MGS_REDIRECT(0x00549A20, _vsnprintf);
    */
}

int New_WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int /*nShowCmd*/)
{
    CheckVars();

    // DisableImports();
    ReplaceStdLib();

    ::testing::GTEST_FLAG(throw_on_failure) = true;
    int argCount = 0;
    ::testing::InitGoogleMock(&argCount, &lpCmdLine);

    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Configurations conf;
    conf.setToDefault();
    conf.parseFromText(R"(
-- default 
* GLOBAL:
   FORMAT               =  "%msg"
   FILENAME             =  "msgi_log.log"
   ENABLED              =  true
   TO_FILE              =  true
   TO_STANDARD_OUTPUT   =  true
* WARNING:
    TO_STANDARD_OUTPUT   =  true
* ERROR:
    TO_STANDARD_OUTPUT   =  true
* FATAL:
    TO_STANDARD_OUTPUT   =  true
)");

    el::Loggers::reconfigureAllLoggers(conf);

    int result; // eax@2
    void(__stdcall *pSetProcessAffinityMask)(HANDLE, signed int); // [sp+8h] [bp-464h]@13
    void(__stdcall *pSetThreadExecutionState)(unsigned int); // [sp+Ch] [bp-460h]@13
    HMODULE hKernel32; // [sp+10h] [bp-45Ch]@12
    char Dest[256]; // [sp+14h] [bp-458h]@11
    _MEMORYSTATUS Buffer = {};
    char *v11; // [sp+434h] [bp-38h]@52
    WNDCLASSA WndClass; // [sp+438h] [bp-34h]@27
    char *bRestart; // [sp+464h] [bp-8h]@8
    //int i; // [sp+468h] [bp-4h]@70

    SystemCpp_ForceLink();
    ScriptCpp_ForceLink();
    TaskCpp_ForceLink();
    SoundCpp_ForceLink();
    SoundCpp_Debug();
    LibDGCpp_ForceLink();
    LibGVCpp_ForceLink();
    RendererCpp_ForceLink();
    ResourceNameHashCpp_ForceLink();
    PsxCpp_ForceLink();
    Actor_RankCPP_ForceLink();
    Actor_DelayCpp_ForceLink();
    Actor_LoaderCpp_ForceLink();
    Fs_Cpp_ForceLink();
    Force_Actor_Movie_Cpp_Link();
    ForceLinkKmdCpp();

    InstallVaradicCFunctionHooks();


    RunTests();

    if (FindWindowA("Metal Gear Solid PC", "Metal Gear Solid PC") || strstr(lpCmdLine, "-restart"))
    {
        MessageBoxA(
            0,
            "Another copy of Metal Gear Solid Integral or VR missions is running, please exit first.",
            "Metal Gear Solid PC",
            MB_OK);
        return 0;
    }

    gCmdLine = lpCmdLine;
    bRestart = strstr(gCmdLine, "-restart");
    if (bRestart)
    {
        *bRestart = 0;
    }

    Buffer.dwLength = 32;
    GlobalMemoryStatus(&Buffer);
    if (Buffer.dwAvailPageFile < 0x4000000)// 50mb hard disk space check
    {
        sprintf(
            Dest,
            "Metal Gear Solid requires over 50mb of hard disk space as Virtual Memory before the game can function correctly. This system currently only has %dmb available.  Please close all open applications not in use,  and refer to the Metal Gear Solid readme for more information on this issue.",
            (Buffer.dwAvailPageFile - Buffer.dwAvailPhys) >> 20);
        MessageBoxA(0, Dest, "Metal Gear Solid PC", MB_OK);
        return 0;
    }

    hKernel32 = LoadLibraryA("KERNEL32.DLL");
    if (hKernel32)
    {
        pSetProcessAffinityMask = (decltype(pSetProcessAffinityMask))GetProcAddress(hKernel32, "SetProcessAffinityMask");
        pSetThreadExecutionState = (decltype(pSetThreadExecutionState))GetProcAddress(hKernel32, "SetThreadExecutionState");
        if (pSetProcessAffinityMask)
        {
            // Only execute on the first CPU
            pSetProcessAffinityMask(GetCurrentProcess(), 1);
        }
        if (pSetThreadExecutionState)
        {
            pSetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
        }
        FreeLibrary(hKernel32);
    }
    CheckForMmf(dword_787774, dword_787778);

    gGameStates_78E7E0.gFlags_dword_78E7E4 |= 0x4000u;
    gGameStates_78E7E0.gFlags_dword_78E7E4 |= 0x100u;

    _strlwr(lpCmdLine);
    _chdir(".");
    gCdId_78D7B0 = -1;

    gCrashCheck = strstr(lpCmdLine, "-nocrashcheck") != nullptr;
    gCheatsEnabled_71687C = strstr(lpCmdLine, "-cheatenable") != nullptr;
    gNoCdEnabled = strstr(lpCmdLine, "-nocd") != nullptr;


    WndClass.style = 3;
    WndClass.lpfnWndProc = MainWindowProc_51C2D3;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIconA(hInstance, (LPCSTR)0x65);
    WndClass.hCursor = 0;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(4);
    WndClass.lpszMenuName = 0;
    WndClass.lpszClassName = "Metal Gear Solid PC";
    if (!RegisterClassA(&WndClass))
    {
        return 1;
    }

    gWindowedMode = 0;
    if (strstr(lpCmdLine, "-st"))
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
        gUseTrueType_650D40 = 0;
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
    if (strstr(lpCmdLine, "-fs"))
        gWindowedMode = 0;
    if (strstr(lpCmdLine, "-w"))
        gWindowedMode = 1;
    if (strstr(lpCmdLine, "-soft"))
        gSoftwareRendering = 1;
    else
        gSoftwareRendering = 0;

    // HACK: Set some options that allow the game to actually start for now
    gCheatsEnabled_71687C = 1;
    gCrashCheck = 0;
    gSoftwareRendering = 0;
    gNoCdEnabled = 1;
    gFps = 1;

    gWindowedMode = 1;
    gModX2 = 1;
    gLowRes = 1;


    gHwnd = CreateWindowExA(
        0,
        WndClass.lpszClassName,
        "Metal Gear Solid PC",
        WS_POPUP | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, // x
        CW_USEDEFAULT, // y
        CW_USEDEFAULT, // w
        CW_USEDEFAULT, // h
        0,
        0,
        hInstance,
        0);

    if (!gHwnd)
    {
        return 1;
    }

    CentreWindow(gHwnd, 640, 480);
    ShowWindow(gHwnd, SW_SHOW);
    UpdateWindow(gHwnd);
    gHInstance = hInstance;
    if (DoInitAll())
    {
        if (Sound_Init(gHwnd) || AskUserToContinueIfNoSoundCard())
        {
            Sound_SetSoundVolume(gSoundFxVol_dword_651D98);
            Sound_SetSoundMusicVolume(gMusicVol_dword_716F68);
            Timer_30_1();

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

    // Real game uses atexit() to call this, we don't because we don't want it calling
    // during DllMain as this can cause a crash.
    ShutdownEngine();

    return result;
}

