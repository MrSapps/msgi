#include "stdafx.h"

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


#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"

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

MSG_FUNC_NOT_IMPL(0x004397D7, bool __cdecl(), AskUserToContinueIfNoSoundCard);
MSG_FUNC_NOT_IMPL(0x0051D120, void __cdecl(int, int), CheckForMmf);
MSG_FUNC_NOT_IMPL(0x00421680, signed __int64 __cdecl(), FpsTimerSetupQ);
MSG_FUNC_NOT_IMPL(0x005202FE, DWORD __cdecl(float, float, float, float), sub_5202FE);
MSG_FUNC_NOT_IMPL(0x00521210, void __cdecl(), sub_521210);
MSG_FUNC_NOT_IMPL(0x0043ACC4, int __cdecl(HDC), WmPaint_Handler);
MSG_FUNC_NOT_IMPL(0x0040815E, void __cdecl(), MemCardsInit);
MSG_FUNC_NOT_IMPL(0x00408086, int __cdecl(), sub_408086);
MSG_FUNC_NOT_IMPL(0x0040111A, int __cdecl(), sub_40111A);
MSG_FUNC_NOT_IMPL(0x004090A7, int __cdecl(), sub_4090A7);


MSG_FUNC_NOT_IMPL(0x0042B6A0, signed int __stdcall (GUID*, LPVOID*, const IID *const, IUnknown*), DirectDrawCreateExMGS);
MSG_FUNC_NOT_IMPL(0x0051D09D, BOOL __cdecl(HWND, int, int), SetWindowSize);
MSG_FUNC_NOT_IMPL(0x004331D4, signed int __cdecl(), ParseMsgCfg);
MSG_FUNC_NOT_IMPL(0x00433801, signed int __cdecl(), sub_433801);
MSG_FUNC_NOT_IMPL(0x0041EC40, signed int __cdecl(), sub_41EC40);
MSG_FUNC_NOT_IMPL(0x0043C850, unsigned int __cdecl(), sub_43C850);
MSG_FUNC_NOT_IMPL(0x00431C63, int __cdecl(), sub_431C63);
MSG_FUNC_NOT_IMPL(0x0051F1E1, int __cdecl(GUID**, GUID**), sub_51F1E1);
MSG_FUNC_NOT_IMPL(0x0042A630, void __cdecl(), _cfltcvt_init); // CRT func?
MSG_FUNC_NOT_IMPL(0x0041EA60, signed int __cdecl(), MissionLog_Related2);
MSG_FUNC_NOT_IMPL(0x0041C820, void __cdecl (float), Render_SetBrightness_sub_41C820);
MSG_FUNC_NOT_IMPL(0x0041CA80, signed int __cdecl(), Render_TextureScratchAlloc);
MSG_FUNC_NOT_IMPL(0x0041CD70, int __cdecl(), Render_sub_41CD70);
MSG_FUNC_NOT_IMPL(0x0041CE20, bool __cdecl(), Render_sub_41CE20);
MSG_FUNC_NOT_IMPL(0x0041D1D0, signed int __cdecl(), Render_sub_41D1D0);
MSG_FUNC_NOT_IMPL(0x0041D420, signed int __cdecl(), Render_sub_41D420);
MSG_FUNC_NOT_IMPL(0x0041E730, bool __cdecl(), Render_sub_41E730);
MSG_FUNC_NOT_IMPL(0x00422A90, int __cdecl(signed int, int), Render_SetRenderState);
MSG_FUNC_NOT_IMPL(0x00422BC0, int __cdecl (unsigned int, signed int, int), Render_InitTextureStages);
MSG_FUNC_NOT_IMPL(0x00431865, signed int __cdecl(), MakeFonts);
MSG_FUNC_NOT_IMPL(0x0051F5B8, signed int __stdcall(GUID*, const char*, char*, void*, HMONITOR), DeviceEnumCallBack);
MSG_FUNC_NOT_IMPL(0x0051ED67, int __cdecl(const char*), Stage_MGZ_RelatedLoad);
MSG_FUNC_NOT_IMPL(0x52008A, int __cdecl(DWORD), DoSleep);
MSG_FUNC_NOT_IMPL(0x42BE0A, int __cdecl(), sub_42BE0A);
MSG_FUNC_NOT_IMPL(0x4583BB, int __cdecl(), sub_4583BB);
MSG_FUNC_NOT_IMPL(0x51E086, int __cdecl(), sub_51E086);
MSG_FUNC_NOT_IMPL(0x4317B3, BOOL __cdecl(), Fonts_Release_sub_4317B3);

MGS_VAR(1, 0x6FC7E0, BYTE, byte_6FC7E0, 0);
MGS_VAR(1, 0x9AD89B, BYTE, byte_9AD89B, 0);
MGS_VAR(1, 0x73491C, DWORD, dword_73491C, 0);
MGS_VAR(1, 0x71D164, DWORD, dword_71D164, 0);



MGS_VAR(1, 0x78E7F8, WORD, word_78E7F8, 0);
MGS_VAR(1, 0x78E7F6, WORD, word_78E7F6, 0);
MGS_VAR(1, 0x717354, DWORD, dword_717354, 0);
MGS_VAR(1, 0x717348, DWORD, dword_717348, 0);
MGS_VAR(1, 0x7348FC, DWORD, dword_7348FC, 0);
MGS_VAR(1, 0x732E64, DWORD, dword_732E64, 0);

MGS_VAR(1, 0x64BDA8, const IID, IID_IDirectDraw7_MGS, {});
MGS_VAR(1, 0x64BB98, const GUID, IID_IDirect3D7_MGS, {});
MGS_VAR(1, 0x64BCA8, const GUID, IID_IDirectDrawGammaControl_MGS, {});
MGS_VAR(1, 0x6FC730, IDirectDraw7 *, g_pDirectDraw, nullptr);
MGS_VAR(1, 0x6FC748, IDirect3D7 *, g_pDirect3D, nullptr);
MGS_VAR(1, 0x6C0EF8, IDirectDrawGammaControl *, g_pGammaControl, nullptr);
MGS_VAR(1, 0x6DF214, DWORD, g_dwDisplayWidth, 0);
MGS_VAR(1, 0x6DF1FC, DWORD, g_dwDisplayHeight, 0);
MGS_VAR(1, 0x6FC750, LPDIRECTDRAWCLIPPER, g_pClipper, nullptr);

MGS_VAR(1, 0x6FC74C, LPDIRECT3DDEVICE7, g_pDirect3DDevice, nullptr);
MGS_VAR(1, 0x6FC740, LPDIRECTDRAWSURFACE7, g_pDDSurface, nullptr);

MGS_VAR(1, 0x006DEF78, FILE *, gFile, nullptr);
MGS_VAR(1, 0x71D414, FILE *, gLogFile, nullptr);

MGS_VAR(1, 0x651CF8, DWORD, dword_651CF8, 0);
MGS_VAR(1, 0x716F5C, float, dword_716F5C, 0);
MGS_VAR(1, 0x716F78, DWORD, dword_716F78, 0);
MGS_VAR(1, 0x77C60C, DWORD, gDriverNum_dword_77C60C, 0);
MGS_VAR(1, 0x77C608, DWORD, gNumDrivers_dword_77C608, 0);
MGS_PTR(1, 0x776B94, DWORD *, dword_776B94, nullptr); // TODO: Array?
MGS_PTR(1, 0x776B90, DWORD *, dword_776B90, nullptr);
MGS_VAR(1, 0x716F74, DWORD, dword_716F74, 0);
MGS_VAR(1, 0x650D2C, DWORD, dword_650D2C, 0);
MGS_VAR(1, 0x6FC728, DWORD *, gImageBufer_dword_6FC728, 0);
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
static_assert(sizeof(jimDeviceDDId) == 0x438, "jimUnk0x438 should be of size 0x438");

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



MGS_ARY(1, 0x776B68, struct jimDeviceIdentifier, 2, g_pDeviceIdentifiers, {}); // TODO: Check size, code seems to clamp it to 2
MGS_ARY(1, 0x689B68, struct jimUnk0x204, 2, array_689B68, {}); // TODO: Also 2?

MGS_ARY(1, 0x6C0778, char, 0x400, unk_6C0778, {}); // TODO: Struct?
MGS_VAR(1, 0x006FC7E8, HFONT, gFont, nullptr);
MGS_VAR(1, 0x009ADDA0, HWND, gHwnd, nullptr);
MGS_VAR(1, 0x72279C, DWORD, game_state_dword_72279C, 0);



//MSG_FUNC_NOT_IMPL(0x51E1D9, int __cdecl(), HandleExclusiveMode);
int __cdecl HandleExclusiveMode()
{
    MSG oMsg;

    if (!g_pDirectDraw)
        return 0;

    if (g_pDirectDraw->TestCooperativeLevel() != DDERR_NOEXCLUSIVEMODE)
        return 0;

    Sound_StopSample();
    sub_4583BB();
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

    sub_51E086();
    FpsTimerSetupQ();
    Task_ResumeQ();
    Sound_PlaySample();

    if (dword_732E64 == 1)
        PostMessageA(gHwnd, WM_KEYDOWN, VK_ESCAPE, 0);

    return 0;
}

//MSG_FUNC_NOT_IMPL_NOLOG(0x0051C9A2, int __cdecl(), MainLoop);
int __cdecl MainLoop()
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
    if (dword_71D164 == 0 && dword_717354 != 0 && dword_717348 == 0)
    {
        dword_717348 = 1;
        byte_9AD89B = 0;

        if (sub_42BE0A() == 0xFFFFFFEF)
            return 0;

        dword_6FC718 = 1;
        dword_717354 = 0;
        byte_9AD89B = 0;
        dword_717348 = 0;
    }
    if (dword_7348FC != 0)
    {
        word_78E7F6 = word_78E7F8 = 0x400;
    }
    
    HandleExclusiveMode();

    if (PeekMessageA(&oMsg, 0, 0, 0, 1) == 0)
        return 1;

    if (oMsg.message == WM_QUIT)
    {
        PostQuitMessage(0);
        return 0;
    }

    TranslateMessage(&oMsg);
    DispatchMessageA(&oMsg);

    return 1;
}


// FIX ME - need a way to handle non standard calling conventions
// 0x452E6E
int /*__usercall*/ sub_452E6E/*<eax>*/(/*<esi>*/)
{
    typedef decltype(&sub_452E6E) fn;
    return ((fn)(0x452E6E))();
}











MGS_VAR(1, 0x78E964, DWORD, dword_78E964, 0);
MGS_VAR(1, 0x73492C, DWORD, gExitMainGameLoop, 0);
MGS_VAR(1, 0x0071D16C, char*, gCmdLine, nullptr);
MGS_VAR(1, 0x787774, DWORD, dword_787774, 0);
MGS_VAR(1, 0x787778, DWORD, dword_787778, 0);
MGS_VAR(1, 0x78E7E4, WORD, dword_78E7E4, 0);
MGS_VAR(1, 0x006DEF94, DWORD, gCrashCheck, 0);
MGS_VAR(1, 0x0071687C, DWORD, gCheatsEnabled, 0);
MGS_VAR(1, 0x006FD1F8, DWORD, gNoCdEnabled, 0);
MGS_VAR(1, 0x00650D14, DWORD, gWindowedMode, 0);
MGS_VAR(1, 0x00688DB8, char*, off_688DB8, "");
MGS_VAR(1, 0x6FC7A0, DWORD, dword_6FC7A0, 0);
MGS_VAR(1, 0x00650D24, DWORD, gNoEffects, 0);

MGS_VAR(1, 0x00650D34, DWORD, gNoFilter, 0);
MGS_VAR(1, 0x00650D40, DWORD, gNoTrueType, 0);
MGS_VAR(1, 0x006FC76C, DWORD, gFps, 0);
MGS_VAR(1, 0x006FC7A4, DWORD, gColourKey, 0);
MGS_VAR(1, 0x00650D38, int, gBlendMode, 0);
MGS_VAR(1, 0x00650D20, DWORD, gLowRes, 0);
MGS_VAR(1, 0x688D40, char*, off_688D40, "");

MGS_VAR(1, 0x0071D1D0, HINSTANCE, gHInstance, 0);
MGS_VAR(1, 0x651D98, DWORD, gSoundFxVol_dword_651D98, 0);
MGS_VAR(1, 0x716F68, DWORD, gMusicVol_dword_716F68, 0);
MGS_VAR(1, 0x77C934, DWORD, dword_77C934, 0);
MGS_VAR(1, 0x9AD8A5, BYTE, byte_9AD8A5, 0);
MGS_VAR(1, 0x9AD8A7, BYTE, byte_9AD8A7, 0);
MGS_VAR(1, 0x9AD8A6, BYTE, byte_9AD8A6, 0);
MGS_VAR(1, 0x9AD8A8, BYTE, byte_9AD8A8, 0);
MGS_VAR(1, 0x9AD8DA, BYTE, byte_9AD8DA, 0);
MGS_VAR(1, 0x9AD8C1, BYTE, byte_9AD8C1, 0);
MGS_VAR(1, 0x73490C, DWORD, dword_73490C, 0);
MGS_VAR(1, 0x734908, DWORD, dword_734908, 0);
MGS_PTR(1, 0x009AD9A0, int*, gKeys, nullptr); // TODO: Array - defined as "plain" ptr so array access won't crash
MGS_ARY(1, 0x9AD880, BYTE, 256, byte_9AD880, {});
MGS_VAR(1, 0x009AD980, DWORD, gvirtualKeyRepeatCount, 0);
MGS_VAR(1, 0x009AD6B0, DWORD, gVirtualKeyCode, 0);
MGS_VAR(1, 0x009AD892, DWORD, gAltPressed, 0);
MGS_VAR(1, 0x71D194, DWORD, dword_71D194, 0);
MGS_VAR(1, 0x009AD8F9, DWORD, gF10Pressed, 0);
MGS_VAR(1, 0x734900, DWORD, dword_734900, 0);
MGS_VAR(1, 0x734904, DWORD, dword_734904, 0);
MGS_VAR(1, 0x9AD988, BYTE, byte_9AD988,0);
MGS_VAR(1, 0x688CDC, DWORD, gActive_dword_688CDC, 0);
MGS_VAR(1, 0x71D17C, DWORD, dword_71D17C, 0 );

MGS_VAR(1, 0x688CD8, DWORD, dword_688CD8, 0);
MGS_VAR(1, 0x791DE4, DWORD, dword_791DE4, 0);
MGS_VAR(1, 0x9AD888, BYTE, byte_9AD888, 0);
MGS_VAR(1, 0x733E34, DWORD, dword_733E34, 0);
MGS_VAR(1, 0x721E78, DWORD, dword_721E78, 0);
MGS_VAR(1, 0x650D4C, DWORD, dword_650D4C, 0);
MGS_VAR(1, 0x0078E7C0, char * , gDest, nullptr);

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
MSG_ASSERT_SIZEOF(weapon_famas, 96);

MGS_VAR(1, 0x995368, WORD, word_995368, 0);
MGS_VAR(1, 0x995320, WORD, word_995320, 0);
MGS_VAR(1, 0x78E804, WORD, word_78E804, 0);

MSG_FUNC_NOT_IMPL_NOLOG(0x00640CDC, int __cdecl(weapon_famas*), Res_famas_update_640CDC);
MSG_FUNC_NOT_IMPL(0x00640E9E, int* __cdecl(weapon_famas*), Res_famas_shutdown_640E9E);



MSG_FUNC_NOT_IMPL(0x0044FF7C, int __cdecl(int, int, int), sub_44FF7C);
MSG_FUNC_NOT_IMPL(0x0045011B, int __cdecl(int, int, int), sub_45011B);

//MSG_FUNC_NOT_IMPL(0x00640EAD, signed int __cdecl(weapon_famas*, int, int, int), Res_Weapon_famas_init_sub_640EAD);
signed int __cdecl Res_Weapon_famas_loader_640EAD(weapon_famas* pFamas, int a2, int a3, int bMp5)
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

    DWORD* pField20 = (DWORD*)&pFamas->field_20;
    sub_44FF7C((int)pField20, resNameHashed, 'm');

    if (*pField20)
    {
        sub_45011B((int)pField20, a2, a3);
        return 0;
    }

    return -1;
}

weapon_famas* CC Res_Weapon_famas_96_sub_640C24(ActorList* a1, ActorList *a2, void(__cdecl *a3)(ActorList *), void(__cdecl *a4)(DWORD), int bMp5)
{
    weapon_famas* pFamas = Actor_ResourceAllocT<weapon_famas>(6);
    if (pFamas)
    {
        Actor_Init(&pFamas->mBase, (TActorFunction)Res_famas_update_640CDC.Ptr(), (TActorFunction)Res_famas_shutdown_640E9E.Ptr(), "C:\\mgs\\source\\Weapon\\famas.c");
        if (Res_Weapon_famas_loader_640EAD(pFamas, (int)a2, (int)a3, bMp5) < 0)
        {
            Actor_DestroyOnNextUpdate(&pFamas->mBase);
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
        WORD famasClipSize = word_78E804;
        if (mp5ClipSize > 0 && word_78E804 > mp5ClipSize)
        {
            famasClipSize = mp5ClipSize;
        }
        word_995320 = mp5ClipSize;
        word_995368 = famasClipSize;
    }
    return pFamas;
}
MSG_FUNC_IMPLEX(0x640C24, Res_Weapon_famas_96_sub_640C24, WINMAIN_IMPL);

void __cdecl Input_AcquireOrUnAcquire();

//MSG_FUNC_NOT_IMPL(0x0051C2D3, signed int __stdcall(HWND, UINT, UINT, LPARAM), MainWindowProc);
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT Msg, UINT wParam, LPARAM lParam)
{
    float v4; // ST08_4@14
    float v5; // ST04_4@14
    signed int result; // eax@29
    UINT v7; // [sp+20h] [bp-104h]@54
    CHAR String[256]; // [sp+24h] [bp-100h]@55

    if (dword_77C934)
    {
        if (byte_9AD8A5)
            sub_5202FE(0.0, -0.003000000026077032, 0.0, 0.0);
        if (byte_9AD8A7)
            sub_5202FE(0.0, 0.003000000026077032, 0.0, 0.0);
        if (byte_9AD8A6)
            sub_5202FE(0.0, 0.0, -0.003000000026077032, 0.0);
        if (byte_9AD8A8)
            sub_5202FE(0.0, 0.0, 0.003000000026077032, 0.0);
        if (byte_9AD8C1)
            sub_5202FE(0.02999999932944775f, 0.0f, 0.0f, 0.0f);
        if (byte_9AD8DA)
            sub_5202FE(-0.02999999932944775f, 0.0f, 0.0f, 0.0f);
        v4 = (float)dword_73490C / 1024.0f;
        v5 = (float)dword_734908 / 1024.0f;
        sub_5202FE(0.0, v5, v4, 0.0);
        dword_734908 = 9 * dword_734908 / 10;
        dword_73490C = 9 * dword_73490C / 10;
    }
    if (Msg > WM_CHAR)
    {
        switch (Msg)
        {
        case WM_SYSKEYDOWN:
            gKeys[wParam] = lParam;
            gvirtualKeyRepeatCount = lParam;
            gVirtualKeyCode = wParam;
            if (wParam < 0x100)
                byte_9AD880[wParam] = 1;
            if ((unsigned __int16)gVirtualKeyCode == VK_MENU)
            {
                gAltPressed = 1;
                return 0;
            }
            if ((unsigned __int16)gVirtualKeyCode == VK_F10)
            {
                dword_71D194 = 1;
                gF10Pressed = 1;
                return 0;
            }
            break;

        case WM_SYSKEYUP:
            gvirtualKeyRepeatCount = lParam;
            gVirtualKeyCode = wParam;
            if (wParam < 0x100)
                byte_9AD880[wParam] = 0;
            if ((unsigned __int16)gVirtualKeyCode == VK_MENU) 
            {
                gAltPressed = 0;
                return 0;
            }
            if ((unsigned __int16)gVirtualKeyCode == VK_F10) 
            {
                gF10Pressed = 0;
                Actor_DumpActorSystem();
                return 0;
            }
            break;

        case WM_MOUSEMOVE:
            if (dword_77C934)
            {
                dword_734908 = (unsigned __int16)lParam - dword_734900;
                dword_73490C = (unsigned __int16)((unsigned int)lParam >> 16) - dword_734904;
                dword_734900 = (unsigned __int16)lParam;
                dword_734904 = (unsigned int)lParam >> 16;
            }
            break;

        case WM_POWERBROADCAST:
            return BROADCAST_QUERY_DENY;
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
    if (Msg == WM_CHAR)
    {
        byte_9AD988 = static_cast<BYTE>(wParam);
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
    if (Msg == WM_ACTIVATE)
    {
        if ((WORD)wParam)
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
        FpsTimerSetupQ();
        result = 1;
    }
    else
    {
        if (Msg == WM_PAINT)
        {
            printf("$jim - WM_PAINT\n");
            if (dword_71D17C)
            {
                WmPaint_Handler((HDC)wParam);
                return 0;
            }
            return DefWindowProcA(hWnd, Msg, wParam, lParam);
        }
        if (Msg == WM_CLOSE)
        {
            PostQuitMessage(0);
            return 0;
        }
        if (Msg != WM_KEYDOWN)
        {
            if (Msg != WM_KEYUP)
                return DefWindowProcA(hWnd, Msg, wParam, lParam);
        LABEL_108:
            if (wParam < 0x100)
                byte_9AD880[wParam] = Msg == WM_KEYDOWN;
            return DefWindowProcA(hWnd, Msg, wParam, lParam);
        }
        v7 = MapVirtualKeyA(wParam, 0);
        if (v7)
        {
            GetKeyNameTextA(v7 << 16, String, 256);
            gKeys[wParam] = lParam;
        }
        gKeys[wParam] = lParam;
        if (wParam > VK_F7)
        {
            switch (wParam)
            {
            case VK_F8:
                if (gCheatsEnabled)
                {
                    dword_688CD0 = 0;
                    result = 0;
                }
                else
                {
                    result = 0;
                }
                break;
            case VK_F9:
                if (gCheatsEnabled)
                {
                    dword_688CD0 = 1;
                    result = 0;
                }
                else
                {
                    result = 0;
                }
                break;
            case VK_F11:
                if (gCheatsEnabled)
                {
                    dword_688CD4 ^= 1u;
                    result = 0;
                }
                else
                {
                    result = 0;
                }
                break;
            default:
                if (wParam != VK_F12)
                    goto LABEL_108;
                if (gCheatsEnabled)
                {
                    dword_688CD8 ^= 1u;
                    result = 0;
                }
                else
                {
                    result = 0;
                }
                break;
            }
        }
        else
        {
            if (wParam == VK_F7)
            {
                if (gCheatsEnabled)
                {
                    game_state_dword_72279C = 0;
                    sub_521210();
                    sub_452E6E();
                    result = 0;
                }
                else
                {
                    result = 0;
                }
            }
            else
            {
                if (wParam == VK_ESCAPE)
                {
                    dword_791DE4 = 1;
                    if (game_state_dword_72279C != 0x20000000 || !strstr(gDest, "s19a"))
                    {
                        if (!dword_717354)
                        {
                            if (!dword_717348)
                            {
                                if (!byte_9AD888)
                                {
                                    if (!dword_733E34)
                                    {
                                        if (!dword_721E78)
                                        {
                                            dword_717354 = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    goto LABEL_108;
                }
                switch (wParam)
                {
                case VK_F2:
                    if (gCheatsEnabled)
                    {
                        if (dword_7348FC)
                            dword_7348FC = 0;
                        else
                            dword_7348FC = 1;
                        result = 0;
                    }
                    else
                    {
                        result = 0;
                    }
                    break;
                case VK_F4:
                    if (gCheatsEnabled)
                    {
                        dword_650D4C ^= 1u;
                        result = 0;
                    }
                    else
                    {
                        result = 0;
                    }
                    break;
                case VK_F5:
                    if (gCheatsEnabled)
                    {
                        dword_77C934 = 0;
                        result = 0;
                    }
                    else
                    {
                        result = 0;
                    }
                    break;
                default:
                    if (wParam != VK_F6)
                        goto LABEL_108;
                    if (gCheatsEnabled)
                    {
                        dword_77C934 = 1;
                        result = 0;
                    }
                    else
                    {
                        result = 0;
                    }
                    break;
                }
            }
        }
    }
    return result;
}

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
        "Hold down the Inventory and Weapon item buttons",
        "if you do not want to play Alternate Round",
        "RADAR OFF",
        "Another copy of Metal Gear Solid Integral or VR missions is running, please exit first.",
        "Metal Gear Solid has detected that your graphics accelerator does not support the functions needed to run the game in Hardware mode.  The game will run correctly, but only in software mode.",
        "Metal Gear Solid has detected that your graphics accelerator does not support all the functions needed to run the game correctly in Hardware mode.  The game will now default to start in software mode.  You can change this to run with your hardware acceleator at any time from the Video Options / Advanced menu.  This is not recomended as correct game functionality cannot be assured.",
        "Metal Gear Solid requires at least 4 MB of video accelerator memory available.  Currently, there is not enough video memory on this system to run the game in Hardware mode.  The game will run correctly, but only in software mode.  If more video memory is made available, Metal Gear Solid will run in hardware mode.  For more information, contact your hardware manufacturer.",
        "Metal Gear Solid has detected that you have a joystick in use on this system.  Please ensure that this joystick is centered prior to playing the game, otherwise the joystick input will override any keyboard direction input."
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

// 0x423F1B
HFONT __cdecl FontCreate_423F1B(int cWidth, int cHeight)
{
    HFONT result; // eax@3

    if (gFont)
    {
        DeleteObject(gFont);
    }

    result = CreateFontA(cHeight, cWidth, 0, 0, 500, 0, 0, 0, 1u, 0, 0, 2u, 0, "Arial");
    gFont = result;
    return result;
}

MSG_FUNC_NOT_IMPL(0x00642382, int __stdcall(LPDDENUMCALLBACKEXA, LPVOID, DWORD), DirectDrawEnumerateExA_MGS);
MSG_FUNC_NOT_IMPL(0x51E382, int __cdecl(void*, int), File_msgvideocfg_Write);
MSG_FUNC_NOT_IMPL(0x51E586, int __cdecl(void*, int), file_msgvideocfg_Write2);

MGS_VAR(1, 0x68C3B8, DWORD, dword_68C3B8, 0);
MGS_VAR(1, 0x775F48, uint8_t, byte_775F48, 0);
MGS_VAR(1, 0x774B48, uint8_t, byte_774B48, 0);
MGS_VAR(1, 0x776450, uint8_t, byte_776450, 0);


MSG_FUNC_NOT_IMPL(0x51E29B, int __cdecl(DDDEVICEIDENTIFIER2*, jimDeviceDDId*, int), File_msgvideocfg_Read);

//MSG_FUNC_NOT_IMPL(0x51E7FC, int __cdecl(LPD3DDEVICEDESC7, LPSTR, LPSTR, jimDeviceIdentifier*), validateDeviceCaps);
int __cdecl validateDeviceCaps(LPD3DDEVICEDESC7 pDesc, LPSTR /*lpDeviceDescription*/, LPSTR lpDeviceName, jimDeviceIdentifier* pIdentifier)
{
    byte_775F48 = 0;
    byte_774B48 = 0;
    byte_776450 = 0;

    char* pStringError = (char*)&byte_776450;
    char* pStringWarning = (char*)&byte_774B48;
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
    else if (((field480 & 2) == 0) && ((field480 & 1) == 0) && ((field480 & 0x40) == 0) && (byte_774B48 != 0))
    {
        sprintf(localString, "%s / (%s)", pIdentifier->ddIdentifier.identifier.szDescription, lpDeviceName);
        strcat(pStringWarning, "\n\tDevice doesn't support everything the game needs\nBut it will be allowed for selection in Option/Advanced Menu\n");
        MessageBox_Error(0, 5, "Metal Gear Solid PC", MB_OK);
    }

    pIdentifier->ddIdentifier.field430 |= status;

    return pIdentifier->ddIdentifier.field430;
}

MSG_FUNC_IMPLEX(0x51E7FC, validateDeviceCaps, WINMAIN_IMPL);

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
    jimDeviceIdentifier* pGlobalIdentifier = g_pDeviceIdentifiers +gNumDrivers_dword_77C608;

    memset(pGlobalIdentifier, 0, sizeof(jimDeviceIdentifier));
    
    if ((pDesc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION) && !(pDesc->dwDevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) && (pDesc->dwDeviceRenderBitDepth & DDBD_16))
    {
        if (pIdentifier->ddIdentifier.field434 == 0)
            return 1;
    }
    else
    {
        return 1;
    }

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

    hr = DirectDrawCreateExMGS(lpGUID, (LPVOID*)&pDirectDraw, &IID_IDirectDraw7_MGS, 0);
    if (hr < 0)
        return TRUE;

    memset(&DDrawDeviceIdentifier, 0, sizeof(DDDEVICEIDENTIFIER2));
    pDirectDraw->GetDeviceIdentifier(&DDrawDeviceIdentifier, 0);

    mgs_fprintf(gLogFile, "$DriverName   = %s\n", DDrawDeviceIdentifier.szDriver);
    mgs_fprintf(gLogFile, "$Description  = %s\n", DDrawDeviceIdentifier.szDescription);
    mgs_fprintf(gLogFile, "$DriverVersion= %i\n", DDrawDeviceIdentifier.liDriverVersion);
    mgs_fprintf(gLogFile, "$VendorId     = %i\n", DDrawDeviceIdentifier.dwVendorId);
    mgs_fprintf(gLogFile, "$DeviceId     = %i\n", DDrawDeviceIdentifier.dwDeviceId);
    mgs_fprintf(gLogFile, "$SubSysId     = %i\n", DDrawDeviceIdentifier.dwSubSysId);
    mgs_fprintf(gLogFile, "$Revision     = %i\n", DDrawDeviceIdentifier.dwRevision);
    mgs_fprintf(gLogFile, "$WHQLLevel    = %i\n", DDrawDeviceIdentifier.dwWHQLLevel);


    if (DDrawDeviceIdentifier.dwVendorId == 0x8086)
    {
        mgs_fprintf(gLogFile, "Intel device found. Do not enumerate it as a valid rendering device.\n");
    }

    hr = pDirectDraw->QueryInterface(IID_IDirect3D7_MGS, (LPVOID*)&pDirect3D);
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

    int result = File_msgvideocfg_Read(&identifier.ddIdentifier.identifier, &Buf2, -1);
    if (result != 0)
    {
        identifier.ddIdentifier.field430 = Buf2.field430;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            result = File_msgvideocfg_Read(&identifier.ddIdentifier.identifier, &Buf2, i);
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
    DirectDrawEnumerateExA_MGS(DDEnumCallbackEx, 0, DDENUM_NONDISPLAYDEVICES);

    for (varC = 0; varC < gNumDrivers_dword_77C608; varC++)
    {
        (g_pDeviceIdentifiers+varC)->ddIdentifier.field430 |= 0x80;
    }

    int var4 = 0x41;
    varC = 0;
    for (;;)
    {
        if (varC >= gNumDrivers_dword_77C608)
            break;

        if (((g_pDeviceIdentifiers+varC)->ddIdentifier.field430 & var4) != 0)
        {
            memset(&Dst, 0, 0x438);
            memcpy(&Dst, &(g_pDeviceIdentifiers+varC)->ddIdentifier, 0x434);    // Copy of var18 is included by memcpying 4 bytes more
            if (File_msgvideocfg_Write(&Dst, -1) == 0)
                var8++;

            memset(g_pDeviceIdentifiers+varC, 0, 0x488);

            if (varC < gNumDrivers_dword_77C608)
            {
                int size = (gNumDrivers_dword_77C608 - (varC + 1)) * 0x488;
                memmove(g_pDeviceIdentifiers+varC, g_pDeviceIdentifiers + varC + 1, size);
            }
            gNumDrivers_dword_77C608--;
            continue;
        }

        if ((g_pDeviceIdentifiers+varC)->pOtherGUID != 0)
        {
            (g_pDeviceIdentifiers + varC)->pOtherGUID = &(g_pDeviceIdentifiers + varC)->otherGUID;
        }

        if ((g_pDeviceIdentifiers + varC)->pDeviceGUID != 0)
        {
            (g_pDeviceIdentifiers + varC)->pDeviceGUID = &(g_pDeviceIdentifiers + varC)->deviceGUID;
        }

        varC++;
    }

    for (varC = 0; varC < 4; varC++)
    {
        memset(&Buf1, 0, 0x438);

        if (varC < gNumDrivers_dword_77C608)
        {
            memcpy(&Buf1, &(g_pDeviceIdentifiers + varC)->ddIdentifier, 0x434);   // Copy of var_450 included same way as earlier
            if (file_msgvideocfg_Write2(&Buf1, -1) == 1)
            {
                var8++;
            }
        }
        if (File_msgvideocfg_Write(&Buf1, varC) == 0)
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
        strncpy(array_689B68[dword_68C3B8].string, g_pDeviceIdentifiers[varC].ddIdentifier.identifier.szDescription, 0x200);
        array_689B68[dword_68C3B8].field200 = g_pDeviceIdentifiers[varC].ddIdentifier.field434;
        
        if (g_pDeviceIdentifiers[varC].ddIdentifier.field430 & 2)
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

//MSG_FUNC_NOT_IMPL(0x41E990, int __cdecl(), ClearDDSurfaceWhite);
bool __cdecl ClearDDSurfaceWhite()
{
    DDBLTFX bltFX = {};
    bltFX.dwSize = sizeof(DDBLTFX);
    bltFX.dwFillColor = 0xFFFF;
    HRESULT hr;
    do 
    {
        hr = g_pDDSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &bltFX);
    } while (hr == DDERR_WASSTILLDRAWING);
    return hr == S_OK;
}
MSG_FUNC_IMPLEX(0x41E990, ClearDDSurfaceWhite, WINMAIN_IMPL);

#define MGSVERTEX_DEF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)


//MSG_FUNC_NOT_IMPL(0x0041ECB0, signed int __cdecl(), InitD3d_ProfileGfxHardwareQ);
signed int Render_sub_41E3C0();

signed int __cdecl InitD3d_ProfileGfxHardwareQ()
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


    gFile = mgs_fopen("profile.log", "w");
    mgs_fputs("InitAll {\n", gFile);
    mgs_fflush(gFile);
    gLogFile = gFile;
    //Input_Start();
    mgs_fputs("jim_enumerate_devices()\n", gFile);
    mgs_fflush(gFile);
    v55 = jim_enumerate_devices();
    if (!v55)
    {
        gSoftwareRendering = 1;
        dword_716F5C = 1.0f;
        gXRes = 1.0f;
        mgs_fputs("No hardware rendering devices were enumerated\n", gFile);
        mgs_fflush(gFile);
        mgs_fputs(" #entering software mode\n", gFile);
        mgs_fflush(gFile);
    }
    v34 = 0;
    if (gWindowedMode)
        v34 = 1;
    mgs_fputs("jim_read_config_from_file\n", gFile);
    mgs_fflush(gFile);
    if (ParseMsgCfg())
    {
        dword_651CF8 = 0;
        mgs_fputs(" . done\n", gFile);
        mgs_fflush(gFile);
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
        mgs_fputs(" . fail\n", gFile);
        mgs_fflush(gFile);
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
        dword_651CF8 = ParseMsgCfg() == 0;
    }
    sub_43C850();
    if (dword_651CF8)
    {
        gSoundFxVol_dword_651D98 = 100;
        gMusicVol_dword_716F68 = 100;
        mgs_fputs("Executing system profiling sequence.\n", gFile);
        mgs_fflush(gFile);
        mgs_fputs("Choosing default 3D-accelerator\n", gFile);
        mgs_fflush(gFile);
        if (sub_41EC40())
        {
            mgs_fputs(" . done\n", gFile);
            mgs_fflush(gFile);
            mgs_fprintf(gFile, " getting selected driver No %d from %d available\n", gDriverNum_dword_77C60C + 1, gNumDrivers_dword_77C608);
            gXRes = 2.0f;
            gWindowedMode = 0;
            gSoftwareRendering = 0;
        }
        else
        {
            mgs_fputs(" . fail\n", gFile);
            mgs_fflush(gFile);
            mgs_fputs(" #entering software mode\n", gFile);
            mgs_fflush(gFile);
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
    mgs_fputs("\n(i) List of enumerated devices:\n", gFile);
    mgs_fflush(gFile);
    for (i = 0; i < gNumDrivers_dword_77C608; ++i)
    {
        mgs_fprintf(gFile, "pDriverGUID %x, pDeviceGUID %x\n", dword_776B94[290 * i], dword_776B90[290 * i]);
        mgs_fprintf(gFile, "D3DDevice description : %s", (g_pDeviceIdentifiers + i)->ddIdentifier.identifier.szDescription);

        if (gDriverNum_dword_77C60C == i)
        {
            mgs_fputs("   /selected/\n", gFile);
            mgs_fflush(gFile);
        }
        else
        {
            mgs_fputs("\n", gFile);
            mgs_fflush(gFile);
        }
    }
    for (;;)
    {
        g_dwDisplayWidth = static_cast<DWORD>(320.0 * gXRes);
        g_dwDisplayHeight = static_cast<DWORD>(240.0 * gXRes);
        mgs_fputs("Creating DirectDraw7\n", gFile);
        mgs_fflush(gFile);

        if (IsMgsi())
        {
            // Call the games DD create
            hr = DirectDrawCreateExMGS(lpGuid, (LPVOID*)&g_pDirectDraw, &IID_IDirectDraw7_MGS, 0);
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
            mgs_fputs(" . fail\n", gFile);
            mgs_fflush(gFile);
            return 0;
        }
        mgs_fputs(" . done\n", gFile);
        mgs_fflush(gFile);
        if (!gSoftwareRendering)
        {
            mgs_fputs("Query interface...\n", gFile);
            mgs_fflush(gFile);
            g_pDirectDraw->QueryInterface(IID_IDirect3D7_MGS, (LPVOID*)&g_pDirect3D);
            if (hr < 0)
            {
                mgs_fputs(" . fail\n", gFile);
                mgs_fflush(gFile);
                gSoftwareRendering = 1;
                gXRes = 1.0f;
                g_dwDisplayWidth = static_cast<DWORD>(320.0 * 1.0f);
                g_dwDisplayHeight = static_cast<DWORD>(240.0 * 1.0f);
                MessageBox_Error(0, 4, "Metal Gear Solid PC", MB_OK);
            }
            mgs_fputs(" . done\n", gFile);
            mgs_fflush(gFile);
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
        if (g_pPrimarySurface)
        {
            hr = g_pPrimarySurface->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release primary surf", hr);
            }
            g_pPrimarySurface = 0;
        }
        if (g_pBackBuffer)
        {
            hr = g_pBackBuffer->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release render surf", hr);
            }
            g_pBackBuffer = 0;
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
        g_pPrimarySurface = 0;
        g_pBackBuffer = 0;
        g_pClipper = 0;
        mgs_fputs("Setting cooperative level...\n", gFile);
        mgs_fflush(gFile);
        if (v42)
        {
            mgs_fputs(" (windowed) \n", gFile);
            mgs_fflush(gFile);
            hr = g_pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_FPUPRESERVE | DDSCL_MULTITHREADED | DDSCL_NORMAL);
        }
        else
        {
            mgs_fputs(" (full-screen) \n", gFile);
            mgs_fflush(gFile);
            hr = g_pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_FPUPRESERVE | DDSCL_MULTITHREADED | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
        }
        if (FAILED(hr))
        {
            mgs_fputs(" . fail\n", gFile);
            mgs_fflush(gFile);
            return 0;
        }
        mgs_fputs(" . done\n", gFile);
        mgs_fflush(gFile);
        if (!v42)
        {
            hr = g_pDirectDraw->SetDisplayMode(g_dwDisplayWidth, g_dwDisplayHeight, 0x10, 0, 0);
            mgs_fprintf(gLogFile, "SetDisplayMode( %d, %d )\n", g_dwDisplayWidth, g_dwDisplayHeight);
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

        mgs_fputs("Creating primary surface...\n", gFile);
        mgs_fflush(gFile);
        hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc2, &g_pPrimarySurface, 0);
        if (hr < 0)
        {
            mgs_fputs(" . fail\n", gFile);
            mgs_fflush(gFile);
            return 0;
        }
        mgs_fputs(" . done\n", gFile);
        mgs_fflush(gFile);

        MakeFonts();
        if (dword_651CF8)
        {
            Render_sub_41CD70();
            Sleep(0x7D0u);
        }
        if (gWindowedMode)
        {
            mgs_fputs("Creating clipper...\n", gFile);
            mgs_fflush(gFile);
            hr = g_pDirectDraw->CreateClipper(0, &g_pClipper, 0);
            if (FAILED(hr))
            {
                mgs_fputs(" . fail\n", gFile);
                mgs_fflush(gFile);
                PrintDDError("Can't create clipper", hr);
                return 0;
            }
            hr = g_pClipper->SetHWnd(0, gHwnd);
            if (FAILED(hr))
            {
                mgs_fputs(" . fail\n", gFile);
                mgs_fflush(gFile);
                PrintDDError("Can't obtain clipper zone", hr);
                return 0;
            }
            hr = g_pPrimarySurface->SetClipper(g_pClipper);
            if (FAILED(hr))
            {
                mgs_fputs(" . fail\n", gFile);
                mgs_fflush(gFile);
                PrintDDError("Can't attach clipper", hr);
                return 0;
            }
            g_pClipper->Release();
            mgs_fputs(" . done\n", gFile);
            mgs_fflush(gFile);
        }
        memset(&dxSurfaceDesc, 0, 124);
        dxSurfaceDesc.dwSize = 124;
        if (gSoftwareRendering)
        {
            if (dword_651CF8 || (dword_716F6C && dword_716F6C != 1))
            {
                mgs_fputs("Testing software render speed to system and to video surface\n", gFile);
                mgs_fflush(gFile);
                if (Render_sub_41CE20())
                {
                    mgs_fputs(" . rendering to video surface is faster\n", gFile);
                    mgs_fflush(gFile);
                    dword_716F6C = 1;
                    dxSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_OFFSCREENPLAIN;
                }
                else
                {
                    mgs_fputs(" . rendering to system memory surface is faster\n", gFile);
                    mgs_fflush(gFile);
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
            dxSurfaceDesc.dwWidth = g_dwDisplayWidth;
            dxSurfaceDesc.dwHeight = g_dwDisplayHeight;

            mgs_fputs("Creating back buffer for software rendering...\n", gFile);
            mgs_fflush(gFile);


            memcpy(&dxSurfaceDesc.ddpfPixelFormat, &pixelFormat3, sizeof(DDPIXELFORMAT));

            hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc, &g_pBackBuffer, 0);
            if (hr < 0)
            {
                mgs_fputs(" . fail\n", gFile);
                mgs_fflush(gFile);
                return 0;
            }
            mgs_fputs(" . done\n", gFile);
            mgs_fflush(gFile);
        }
        else
        {
            if (v42)
            {
                dxSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                dxSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN;
                dxSurfaceDesc.dwWidth = g_dwDisplayWidth;
                dxSurfaceDesc.dwHeight = g_dwDisplayHeight;
                mgs_fputs("Creating back buffer for windowed mode...\n", gFile);
                mgs_fflush(gFile);
                hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc, &g_pBackBuffer, 0);
                if (hr < 0)
                {
                    mgs_fputs(" . fail\n", gFile);
                    mgs_fflush(gFile);
                    return 0;
                }
                mgs_fputs(" . done\n", gFile);
                mgs_fflush(gFile);
            }
            else
            {
                dxCaps1.dwCaps = DDSCAPS_BACKBUFFER;
                dxCaps1.dwCaps2 = 0;
                dxCaps1.dwCaps3 = 0;
                dxCaps1.dwCaps4 = 0;
                mgs_fputs("Getting back buffer from pPrim chain...\n", gFile);
                mgs_fflush(gFile);
                g_pPrimarySurface->GetAttachedSurface(&dxCaps1, &g_pBackBuffer);
                if (hr < 0)
                {
                    mgs_fputs(" . fail\n", gFile);
                    mgs_fflush(gFile);
                    return hr;
                }
                mgs_fputs(" . done\n", gFile);
                mgs_fflush(gFile);
            }
        }
        mgs_fputs("Restoring surfaces...\n", gFile);
        mgs_fflush(gFile);
        Render_RestoreAll();
        if (FAILED(hr))
        {
            mgs_fputs(" . fail\n", gFile);
            mgs_fflush(gFile);
            PrintDDError("Restoring caput", hr);
        }
        else
        {
            mgs_fputs(" . done\n", gFile);
            mgs_fflush(gFile);
        }
        mgs_fputs("Querying gamma interface...\n", gFile);
        mgs_fflush(gFile);
        g_pPrimarySurface->QueryInterface(IID_IDirectDrawGammaControl_MGS, (LPVOID*)&g_pGammaControl);
        if (FAILED(hr))
        {
            mgs_fputs(" . fail\n", gFile);
            mgs_fflush(gFile);
            PrintDDError("Can't get GammaControl interface", hr);
            dword_6FC7C4 = 0;
        }
        else
        {
            mgs_fputs(" . done\n", gFile);
            mgs_fflush(gFile);
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
       // mgs_fprintf(gFile, "565 mode = %i\n", g_surface565Mode);
        
        if (gSoftwareRendering)
            break;
        dxSurfaceDesc.dwSize = 124;
        g_pDirectDraw->GetDisplayMode(&dxSurfaceDesc);
        if (dxSurfaceDesc.ddpfPixelFormat.dwRGBBitCount <= 8)
        {
            mgs_fputs("Can't render to a palettized surface, exiting.\n", gFile);
            mgs_fflush(gFile);
            return 0;
        }
        mgs_fputs("Screen mode is ok\n", gFile);
        mgs_fflush(gFile);
        mgs_fputs("Creating device...\n", gFile);
        mgs_fflush(gFile);

        hr = g_pDirect3D->CreateDevice(*v33, g_pBackBuffer, &g_pDirect3DDevice);

        if (SUCCEEDED(hr))
        {
            mgs_fputs(" . done\n", gFile);
            mgs_fflush(gFile);
            Render_SetRenderState(22, 1);
            Render_SetRenderState(26, 0);
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

                hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc3, &g_pDDSurface, 0);
                if (FAILED(hr))
                {
                    g_pDDSurface = 0;
                }
                else 
                {
                    if (!ClearDDSurfaceWhite())
                    {
                        g_pDDSurface->Release();
                        g_pDDSurface = 0;
                    }
                }
            }

            Render_InitTextureStages(0, 2, 2);
            Render_InitTextureStages(0, 1, 4);
            Render_InitTextureStages(0, 3, 0);
            Render_InitTextureStages(0, 5, 2);
            Render_InitTextureStages(0, 4, 4);
            Render_InitTextureStages(0, 6, 0);

            if (Render_sub_41E3C0())
            {
                mgs_fprintf(gFile, "Blend modes = %i \n", gBlendMode);
                mgs_fprintf(gFile, "Alpha modulate = %i \n", gAlphaModulate_dword_6FC798);
                gColourKey = Render_sub_41E730();
                mgs_fprintf(gFile, "ColorKey = %i\n", gColourKey);
                Render_InitTextureStages(0, 12, 3);
                if (gModX2 == 2)
                    gModX2 =  Render_sub_41D420();
                mgs_fprintf(gFile, "MODULATE2X = %i \n", gModX2);
                if (gColourKey)
                {
                    Render_SetRenderState(41, 1);
                }
                else
                {
                    Render_SetRenderState(15, 1);
                    Render_SetRenderState(24, 127);
                    Render_SetRenderState(25, 7);
                }
                Render_SetRenderState(26, 1);
                if (dword_651CF8)
                {
                    if (gLowRes != gLowRes) // FIX ME: This can't be right
                    {
                        if (!gLowRes)
                        {
                            mgs_fputs("Not enough video memory for high resolution textures, disabling.\n", gFile);
                            mgs_fflush(gFile);
                        }
                    }
                    if (gNoEffects)
                    {
                        mgs_fputs("Water and ninja effects are on\n", gFile);
                        mgs_fflush(gFile);
                    }
                    else
                    {
                        mgs_fputs("Water and ninja effects are unsupported by hardware, disabling.\n", gFile);
                        mgs_fflush(gFile);
                    }
                    if (gModX2)
                    {
                        mgs_fputs("Modulate by 2 works correctly, enabling.\n", gFile);
                        mgs_fflush(gFile);
                    }
                    else
                    {
                        mgs_fputs("Modulate by 2 doesn't work correctly or unsupported, disabling\n", gFile);
                        mgs_fflush(gFile);
                    }
                }
                dxViewport.dwX = 0;
                dxViewport.dwY = 0;
                dxViewport.dwWidth = g_dwDisplayWidth;
                dxViewport.dwHeight = g_dwDisplayHeight;
                dxViewport.dvMinZ = 0;
                dxViewport.dvMaxZ = 1.0f;
                g_pDirect3DDevice->SetViewport(&dxViewport);
                v2 = ((float)dword_651D94 - 50.0f) / 100.0f;
                Render_SetBrightness_sub_41C820(v2);
            }
            else
            {
                g_pDirect3DDevice->Release();
                g_pDirect3DDevice = 0;
                MessageBox_Error(0, 5, "Metal Gear Solid PC", MB_OK);
                gSoftwareRendering = 1;
            }
            break;
        }
        mgs_fputs("D3D:CreateDevice() failed, switching to SOFTWARE MODE\n", gFile);
        mgs_fflush(gFile);
        g_dwDisplayWidth = 320;
        g_dwDisplayHeight = 240;
        mgs_fprintf(gLogFile, "Resetting DisplayMode to ( %d, %d )\n", g_dwDisplayWidth, g_dwDisplayHeight);
        MessageBox_Error(0, 4, "Metal Gear Solid PC", MB_OK);
        gSoftwareRendering = 1;
        dword_716F5C = 1.0f;
        gXRes = dword_716F5C; // TODO: Float
        hr = g_pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_NORMAL);
        if (g_pGammaControl)
        { 
            g_pGammaControl->Release();
        }

        if (g_pBackBuffer)
        {
            hr = g_pBackBuffer->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't release render surf", hr);
            }
            g_pBackBuffer = 0;
        }
        if (g_pPrimarySurface)
        {
            hr = g_pPrimarySurface->Release();
            if (FAILED(hr))
            {
                PrintDDError("Can't relaese primary surf", hr);
            }
            g_pPrimarySurface = 0;
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
        g_pPrimarySurface = 0;
        g_pBackBuffer = 0;
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
    mgs_fputs("other inits\n", gFile);


    mgs_fflush(gFile);


    for (i = 0; i < 1500; ++i)
    {
        gTextures_dword_6C0F00[i].mSurface = 0;
        gTextures_dword_6C0F00[i].field_20 = 0;
        gTextures_dword_6C0F00[i].field_24 = 0;
    }
    
    dword_6FC7C0 = Render_TextureScratchAlloc() == 0;
    if (dword_6FC7C0)
    {
        gNoEffects = 0;
        dword_716F60 = 0;
    }
    FontCreate_423F1B(0, static_cast<int>(14.0 * gXRes));
    MissionLog_Related2();
    if (!gSoftwareRendering)
    {
        gPrimStructArray = (prim_struct*)mgs_malloc(150000 * sizeof(prim_struct));
        for (i = 0; i < 15000; ++i)
        {
            gPrimStructArray[i].field_0 = 0;
        }
        g_pMGSVertices = (MGSVertex*)mgs_malloc(sizeof(MGSVertex) * 150000);
    }
    gImageBufer_dword_6FC728 = (DWORD*)mgs_malloc(0x100000u);
    if (gImageBufer_dword_6FC728)
    {
        memset(gImageBufer_dword_6FC728, 0, 0x100000u);
        g_pwTextureIndices = (WORD*)mgs_malloc(0x100000u);
        if (gImageBufer_dword_6FC728)
        {
            memset(gImageBufer_dword_6FC728, -1, 0x100000u);
            _cfltcvt_init();
            memset(unk_6C0778, 0, 0x400u);
            dword_6DEF7C = mgs_malloc(0x200u);
            dword_6DEF90 = mgs_malloc(0x200u);
            memset(dword_6DEF7C, 0, 0x100u);
            mgs_fputs("jim_write_configuration_to_file()\n", gFile);
            mgs_fflush(gFile);
            sub_433801();
            mgs_fputs("InitAll }\n", gFile);
            mgs_fflush(gFile);
            mgs_fclose(gFile);
            result = 1;
        }
        else
        {
            PrintDDError("$edq Out of memory", 0);
            mgs_fclose(gFile);
            result = 0;
        }
    }
    else
    {
        PrintDDError("$edq Out of memory", 0);
        mgs_fclose(gFile);
        result = 0;
    }
    return result;
}
MSG_FUNC_IMPL(0x0041ECB0, InitD3d_ProfileGfxHardwareQ);


//MSG_FUNC_NOT_IMPL(0x41E9E0, HRESULT __cdecl(), SetDDSurfaceTexture);
HRESULT __cdecl SetDDSurfaceTexture()
{
    HRESULT hr;

    if (g_pDDSurface != 0)
    {
        if (g_pDDSurface->IsLost() == DDERR_SURFACELOST)
        {
            g_pDDSurface->Restore();
            ClearDDSurfaceWhite();
        }
        hr = g_pDirect3DDevice->SetTexture(0, g_pDDSurface);
    }
    else
    {
        hr = g_pDirect3DDevice->SetTexture(0, NULL);
    }

    return hr;
}

int __cdecl ClearBackBuffer(uint32_t a_ClearColor, uint32_t a_DiffuseColor, uint32_t* pFirstPixel, MGSVertex* a_pVertices);


signed int Render_sub_41E3C0()
{
    signed int result;
    
    D3DDEVICEDESC7 caps = {};
    MGSVertex pPrim[3];
    uint32_t firstPixel;

    DWORD dwNumPasses = 1;
    pPrim[0].x = 1.0f;
    pPrim[1].x = static_cast<float>(g_dwDisplayWidth - 1);
    pPrim[2].x = 1.0f;

    pPrim[0].y = 1.0f;
    pPrim[1].y = 1.0f;
    pPrim[2].y = static_cast<float>(g_dwDisplayHeight - 1);

    pPrim[0].z = 1.0f;
    pPrim[1].z = 1.0f;
    pPrim[2].z = 1.0f;

    pPrim[0].u = 1.0f;
    pPrim[1].u = 1.0f;
    pPrim[2].u = 1.0f;

    pPrim[0].v = 1.0f;
    pPrim[1].v = 1.0f;
    pPrim[2].v = 1.0f;

    pPrim[0].w = 0.99999899f;
    pPrim[1].w = 0.99999899f;
    pPrim[2].w = 0.99999899f;

    g_pDirect3DDevice->GetCaps(&caps);
    const DWORD srcBlendCaps = caps.dpcTriCaps.dwSrcBlendCaps;
    const DWORD dstBlendCaps = caps.dpcTriCaps.dwDestBlendCaps;
    Render_SetRenderState(9, 1);
    Render_SetRenderState(27, 1);

    if (SUCCEEDED(g_pDirect3DDevice->ValidateDevice(&dwNumPasses)))
    {
        gAlphaModulate_dword_6FC798 = 0;
        Render_InitTextureStages(0, 4, 2);
    }
    else
    {
        gAlphaModulate_dword_6FC798 = 1;
    }

    if (gBlendMode < 0)
    {
        gBlendMode = 0;
        if (srcBlendCaps & 0x10)
        {
            if (dstBlendCaps & 0x10)
            {
                Render_SetRenderState(19, 5);
                Render_SetRenderState(20, 5);
                if (FAILED(g_pDirect3DDevice->ValidateDevice(&dwNumPasses)))
                {
                    if (gAlphaModulate_dword_6FC798)
                    {
                        ClearBackBuffer(0xFF707070, 0x7F404040u, &firstPixel, pPrim);
                        if ((unsigned __int8)firstPixel < 0x5Bu && (unsigned __int8)firstPixel > 0x55u)
                        {
                            gBlendMode |= 1u;
                        }
                    }
                }
            }
        }
        if (srcBlendCaps & 0x10)
        {
            if (dstBlendCaps & 2)
            {
                Render_SetRenderState(19, 5);
                Render_SetRenderState(20, 2);
                if (FAILED(g_pDirect3DDevice->ValidateDevice(&dwNumPasses)))
                {
                    if (gAlphaModulate_dword_6FC798)
                    {
                        ClearBackBuffer(0xFF101010, 0x3F404040u, &firstPixel, pPrim);
                        if ((unsigned __int8)firstPixel < 0x25u && (unsigned __int8)firstPixel > 0x1Bu)
                        {
                            gBlendMode |= 8u;
                        }
                    }
                }
            }
        }
        if (srcBlendCaps & 2 && dstBlendCaps & 2)
        {
            gBlendMode |= 2u;
            if (srcBlendCaps & 1)
            {
                if (dstBlendCaps & 8)
                {
                    Render_SetRenderState(19, 1);
                    Render_SetRenderState(20, 4);
                    ClearBackBuffer(0xFFA0FFA0, 0xFF400040, &firstPixel, pPrim);

                    if ((unsigned __int8)firstPixel < 0x79u && (unsigned __int8)firstPixel > 0x6Fu)
                    {
                        gBlendMode |= 4u;
                    }

                    if ((firstPixel & 0xFF00) <= 0xFF00 && (firstPixel & 0xFF00) > 0xFB00)
                    {
                        dword_6FC774 = 1;
                    }
                }
            }
            result = 1;
        }
        else
        {
            result = 0;
        }
    }
    else
    {
        result = 1;
    }
    return result;
}

MSG_FUNC_IMPLEX(0x41E3C0, Render_sub_41E3C0, WINMAIN_IMPL);

//MSG_FUNC_NOT_IMPL(0x41E130, int __cdecl(uint32_t, uint32_t, uint32_t*, MGSVertex*), ClearBackBuffer);
int __cdecl ClearBackBuffer(uint32_t a_ClearColor, uint32_t a_DiffuseColor, uint32_t* pFirstPixel, MGSVertex* a_pVertices)
{
    HRESULT hr;
    Sleep(500);

    if (g_surface565Mode != 0)
    {
        a_ClearColor = ((a_ClearColor & 0xF8) >> 3) | ((a_ClearColor & 0xFC00) >> 5) | ((a_ClearColor & 0xF80000) >> 8);
    }
    else
    {
        a_ClearColor = ((a_ClearColor & 0xF8) >> 3) | ((a_ClearColor & 0xF800) >> 6) | ((a_ClearColor & 0xF80000) >> 9) | ((a_ClearColor & 0x80000000) >> 16);
    }

    DDBLTFX bltFX;
    bltFX.dwSize = sizeof(DDBLTFX);
    bltFX.dwFillColor = a_ClearColor;
    
    do {
        hr = g_pBackBuffer->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &bltFX);
    } while (hr == DDERR_WASSTILLDRAWING);
    if (hr != 0)
        return 0;

    a_pVertices[2].diffuse = a_DiffuseColor;
    a_pVertices[1].diffuse = a_DiffuseColor;
    a_pVertices[0].diffuse = a_DiffuseColor;

    // result stored but not used
    // happens a few times in this function, I keep it
    hr = SetDDSurfaceTexture();

    hr = g_pDirect3DDevice->BeginScene();
    if (hr != 0)
        return 0;

    hr = g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, MGSVERTEX_DEF, a_pVertices, 3, 0);
    hr = g_pDirect3DDevice->SetTexture(0, NULL);
    if (hr != 0)
        return 0;

    hr = g_pDirect3DDevice->EndScene();
    if (hr != 0)
        return 0;

    DDSURFACEDESC2 ddDesc;
    memset(&ddDesc, 0, sizeof(DDSURFACEDESC2));
    ddDesc.dwSize = sizeof(DDSURFACEDESC2);

    do {
        hr = g_pBackBuffer->Lock(NULL, &ddDesc, 0, 0);
    } while (hr == DDERR_WASSTILLDRAWING);
    if (hr != 0)
        return 0;

    WORD wFirstPixel = ((WORD*)ddDesc.lpSurface)[0];
    g_pBackBuffer->Unlock(NULL);

    *pFirstPixel = 0;
    if (g_surface565Mode != 0)
    {
        *pFirstPixel = ((wFirstPixel & 0xF800) << 8) | ((wFirstPixel & 0x07E0) << 5) | ((wFirstPixel & 0x001F) << 3);
    }
    else
    {
        *pFirstPixel = ((wFirstPixel & 0x7C00) << 9) | ((wFirstPixel & 0x03E0) << 6) | ((wFirstPixel & 0x001F) << 3);
    }
    return 1;
}
MSG_FUNC_IMPLEX(0x41E130, ClearBackBuffer, WINMAIN_IMPL);

MSG_FUNC_NOT_IMPL(0x44EAE5, uint32_t __cdecl(), sub_44EAE5);

// 0x00420810
signed int __cdecl DoInitAll()
{
    const auto ret = InitD3d_ProfileGfxHardwareQ();
    MessageBox_Error(gHwnd, -1, "Metal Gear Solid PC", MB_OK);
    return ret;
}
MSG_FUNC_IMPL(0x00420810, DoInitAll);


// 0x00520157
void DebugLog(const char *Format, ...)
{
    char Dest[512]; // [sp+0h] [bp-400h]@1
    va_list va; // [sp+40Ch] [bp+Ch]@1

    va_start(va, Format);
    vsprintf(Dest, Format, va);
    //OutputDebugStringA(Dest);
    printf("%s", Dest);
}

int mgs_printf(const char *fmt, ...)
{

    va_list myargs;
    va_start(myargs, fmt);


    int ret = vprintf(fmt, myargs);

    va_end(myargs);
    

    return ret;
}

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

    using DebugLog_Type = decltype(&DebugLog);
    DebugLog_Type oldPtr = (DebugLog_Type)0x00520157;
    err = DetourAttach(&(PVOID&)oldPtr, DebugLog);
    

    if (err != NO_ERROR)
    {
        abort();
    }
    
    using mgs_printf_Type = decltype(&mgs_printf);
    mgs_printf_Type old_mgs_printf = (mgs_printf_Type)0x005398F0;

    err = DetourAttach(&(PVOID&)old_mgs_printf, mgs_printf);
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
MGS_VAR(1, 0x6BED20, DWORD, dword_6BED20, 0);
MGS_ARY(1, 0x6BED18, DWORD, 2, dword_6BED18, {}); // TODO: Check 2 is correct

MSG_FUNC_NOT_IMPL(0x40DD00, struct DISPENV *__cdecl(PSX_RECT *pRect), sub_40DD00);
MSG_FUNC_NOT_IMPL(0x459ACE, int __cdecl(), sub_459ACE);
MSG_FUNC_NOT_IMPL(0x40162D, signed int __cdecl(int activeBuffer), sub_40162D);
MSG_FUNC_NOT_IMPL(0x4021F2, int(), sub_4021F2);


int __cdecl Main_sub_401C02()
{
    int result = 0;
    if (gSoftwareRendering)
    {
        // SW rendering path not implemented
        abort();
    }
    else
    {
        const int activeBufferHW = gActiveBuffer_dword_791A08;
        if (dword_6BED20 <= 0)
        {
            if (gLastActiveBuffer_dword_650110 < 0 || gActiveBuffer_dword_791A08 != gLastActiveBuffer_dword_650110)
            {
                gClipRect_6BECF0.x1 = static_cast<WORD>(dword_6BED18[gActiveBuffer_dword_791A08]);
                sub_40DD00(&gClipRect_6BECF0);
                Render_DrawIndex(1 - activeBufferHW);
                gLastActiveBuffer_dword_650110 = -1;
            }
        }
        else
        {
            if (gLastActiveBuffer_dword_650110 < 0)
            {
                gLastActiveBuffer_dword_650110 = gActiveBuffer_dword_791A08;
            }
            --dword_6BED20;
        }
        System_HouseKeeping_40ACB2(activeBufferHW);
        System_HouseKeeping_40ACB2(2);
        sub_459ACE();
        sub_40162D(activeBufferHW);                 // calls ClearOTag
        result = sub_4021F2();
    }
    return result;
}
MSG_FUNC_IMPL(0x401C02, Main_sub_401C02);





signed int __cdecl Main()
{
    signed int result = 0;

    Resetgraph_AndPrintPsxStructureSizes(0);
    SetGraphDebug(0);
    //null_44AC80();
    SetDispMask(0);
    PSX_RECT clearRect;
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
    LibGv_Init_sub_40A4F6();
    sub_408086();
    sub_40111A();
    sub_4090A7();
    LibGv_Set_Load_HZM_CallBack_40B725();
    Init_Gamed_Create_44E12B();

    for (;;)
    {
        result = MainLoop();
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
        Actor_UpdateActors();
    }
    return result;
}
MSG_FUNC_IMPLEX(0x00401005, Main, WINMAIN_IMPL);

// 0x00401000
//MSG_FUNC_NOT_IMPL(0x00401000, int __cdecl(), DoMain);
int __cdecl DoMain()
{
    return Main();
}

MGS_VAR(1, 0x6FC73C, IUnknown*, dword_6FC73C, nullptr); // TODO: Check what this is

MSG_FUNC_NOT_IMPL(0x4241A4, void* __cdecl(void *), sub_4241A4);

// 00423020
void __cdecl ClearAll()
{
    puts(" *************************** CLEAR ALL START *************************");
    if (g_NumTextures)
    {
        for (int i = 0; i < g_NumTextures; ++i)
        {
            if (gTextures_dword_6C0F00[i].mSurfaceType == 5)
            {
                sub_4241A4(gTextures_dword_6C0F00[i].mSurface);
            }
            else if (!gSoftwareRendering)
            {
                if (gTextures_dword_6C0F00[i].mSurface)
                {
                    gTextures_dword_6C0F00[i].mSurface->Release();
                    gTextures_dword_6C0F00[i].mSurface = 0;
                }
            }
        }
    }

    if (dword_6FC73C)
    {
        dword_6FC73C->Release();
        dword_6FC73C = 0;
    }

    if (g_pDirect3DDevice)
    {
        g_pDirect3DDevice->Release();
        g_pDirect3DDevice = 0;
    }

    if (g_pBackBuffer)
    {
        g_pBackBuffer->Release();
        g_pBackBuffer = 0;
    }

    if (g_pClipper)
    {
        g_pClipper->Release();
        g_pClipper = 0;
    }

    if (g_pPrimarySurface)
    {
        g_pPrimarySurface->Release();
        g_pPrimarySurface = 0;
    }

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

    if (g_pDDSurface)
    {
        g_pDDSurface->Release();
        g_pDDSurface = 0;
    }

    if (g_pMGSVertices)
    {
        mgs_free(g_pMGSVertices);
        g_pMGSVertices = 0;
    }

    if (gPrimStructArray)
    {
        mgs_free(gPrimStructArray);
        gPrimStructArray = 0;
    }

    if (dword_6DEF7C)
    {
        mgs_free(dword_6DEF7C);
        dword_6DEF7C = 0;
    }

    if (dword_6DEF90)
    {
        mgs_free(dword_6DEF90);
        dword_6DEF90 = 0;
    }

    mgs_free(gImageBufer_dword_6FC728);
    mgs_free(g_pwTextureIndices);
}

void __cdecl DoClearAll()
{
    ClearAll();
}
MSG_FUNC_IMPL(0x004232B0, DoClearAll);

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


int New_WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int /*nShowCmd*/)
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    int argCount = 0;
    ::testing::InitGoogleMock(&argCount, &lpCmdLine);
    DoScriptTests();
    DoTestSystem();
    DoResourceNameHashTest();
    DoActor_RankTests();

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

    if (IsMgsi())
    {
        InstallVaradicCFunctionHooks();
    }

    if (!FindWindowA("Metal Gear Solid PC", "Metal Gear Solid PC") || strstr(lpCmdLine, "-restart"))
    {
        gCmdLine = lpCmdLine;
        bRestart = strstr(gCmdLine, "-restart");
        if (bRestart)
            *bRestart = 0;
        Buffer.dwLength = 32;
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
            
            dword_78E7E4 |= 0x4000u;
            dword_78E7E4 |= 0x100u;

            _strlwr(lpCmdLine);
            _chdir(".");
            dword_78D7B0 = -1;
            if (strstr(lpCmdLine, "-nocrashcheck"))
                gCrashCheck = 0;
            else
                gCrashCheck = 1;
            if (strstr(lpCmdLine, "-cheatenable"))
                gCheatsEnabled = 1;
            else
                gCheatsEnabled = 0;
            if (strstr(lpCmdLine, "-nocd"))
                gNoCdEnabled = 1;
            else
                gNoCdEnabled = 0;
            WndClass.style = 3;
            WndClass.lpfnWndProc = MainWindowProc;
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
                    dword_6FC7A0 = 58; // "Normal" path, in real game setting this to zero seems to be impossible
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
                if (gHwnd)
                {
                    SetWindowSize(gHwnd, 640, 480);
                    ShowWindow(gHwnd, 5);
                    UpdateWindow(gHwnd);
                    gHInstance = hInstance;
                    if (DoInitAll())
                    {
                        if (Sound_Init(gHwnd) || AskUserToContinueIfNoSoundCard())
                        {
                            Sound_SetSoundFxVolume(gSoundFxVol_dword_651D98);
                            Sound_SetMusicVolume(gMusicVol_dword_716F68);
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
                Dest,
                "Metal Gear Solid requires over 50mb of hard disk space as Virtual Memory before the game can function correctly. This system currently only has %dmb available.  Please close all open applications not in use,  and refer to the Metal Gear Solid readme for more information on this issue.",
                (Buffer.dwAvailPageFile - Buffer.dwAvailPhys) >> 20);
            MessageBoxA(0, Dest, "Metal Gear Solid PC", MB_OK);
            result = 0;
        }
    }
    else
    {
        MessageBoxA(
            0,
            "Another copy of Metal Gear Solid Integral or VR missions is running, please exit first.",
            "Metal Gear Solid PC",
            MB_OK);
        result = 0;
    }

    // Real game uses atexit() to call this, we don't because we don't want it calling
    // during DllMain as this can cause a crash.
    ShutdownEngine();

    return result;
}
