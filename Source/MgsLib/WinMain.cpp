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

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dsound.lib")

struct texture_struct
{
    IDirectDrawSurface7* mSurface;
    WORD field_4;
    WORD field_6;
    WORD field_8;
    WORD field_A;
    WORD field_C;
    WORD field_E;
    WORD field_10;
    WORD field_12;
    DWORD float_field_14;
    DWORD float_field_18;
    DWORD mSurfaceType;
    DWORD field_20;
    DWORD field_24;
    DWORD field_28;
    DWORD field_2C;
    DWORD field_30;
    DWORD field_34;
    DWORD field_38;
    DWORD field_3C;
    DWORD field_40;
    DWORD field_44;
    DWORD field_48;
    DWORD field_4C;
};
static_assert(sizeof(texture_struct) == 0x50, "texture_struct should be 0x50");

MGS_ARY(1, 0x6C0F00, texture_struct, 1500, gTextures_dword_6C0F00, {}); // Array of 1500 items

struct prim_struct
{
    DWORD field_0;
    WORD nTextureIndex;
    WORD nBlendMode;
    DWORD mShadeMode;
    DWORD mPrimTypeQ;
    DWORD dwVertexCount;
};
static_assert(sizeof(prim_struct) == 0x14, "prim_struct should be 0x14");
MGS_VAR(1, 0x6C0EFC, prim_struct*, gPrimStructArray, nullptr); // Dynamically allocated array of 15000 items

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
MSG_FUNC_NOT_IMPL(0x0040A4F6, void __cdecl(), sub_40A4F6);
MSG_FUNC_NOT_IMPL(0x00408086, int __cdecl(), sub_408086);
MSG_FUNC_NOT_IMPL(0x0040111A, int __cdecl(), sub_40111A);
MSG_FUNC_NOT_IMPL(0x004090A7, int __cdecl(), sub_4090A7);
MSG_FUNC_NOT_IMPL(0x0040B725, int __cdecl(), sub_40B725);
MSG_FUNC_NOT_IMPL(0x00452610, int __cdecl(), sub_452610);
MSG_FUNC_NOT_IMPL(0x0044E9D2, int __cdecl(DWORD*), sub_44E9D2);
MSG_FUNC_NOT_IMPL(0x0044E381, void* __cdecl(int), sub_44E381);
MSG_FUNC_NOT_IMPL(0x0044E1F9, int __cdecl(), unknown_libname_3); // Note: Not a CRT func!!
MSG_FUNC_NOT_IMPL(0x0044E287, void __cdecl(), sub_44E287);
MSG_FUNC_NOT_IMPL(0x0044E212, void* __cdecl(), sub_44E212);
MSG_FUNC_NOT_IMPL(0x0044E226, Actor* __cdecl(), sub_44E226);
MSG_FUNC_NOT_IMPL(0x004232B0, void __cdecl(), DoClearAll);
MSG_FUNC_NOT_IMPL(0x00459A9A, int __cdecl(), Menu_Related1);
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
MSG_FUNC_NOT_IMPL(0x0041E3C0, int __cdecl(), Render_sub_41E3C0);
MSG_FUNC_NOT_IMPL(0x0041E730, bool __cdecl(), Render_sub_41E730);
MSG_FUNC_NOT_IMPL(0x00422A90, int __cdecl(signed int, int), Render_SetRenderState);
MSG_FUNC_NOT_IMPL(0x00422BC0, int __cdecl (unsigned int, signed int, int), Render_InitTextureStages);
MSG_FUNC_NOT_IMPL(0x00431865, signed int __cdecl(), MakeFonts);
MSG_FUNC_NOT_IMPL(0x0051F5B8, signed int __stdcall(GUID*, const char*, char*, void*, HMONITOR), DeviceEnumCallBack);
MSG_FUNC_NOT_IMPL(0x0051ED67, int __cdecl(const char*), Stage_MGZ_RelatedLoad);
//MSG_FUNC_NOT_IMPL(0x0040A3FC, int __cdecl (actor_related_struct*), Actor_Unknown3);
MSG_FUNC_NOT_IMPL(0x0040A3ED, Actor* __cdecl(Actor*), Actor_SetRemoveFnPtr);
MSG_FUNC_NOT_IMPL(0x0040A30C, void* __cdecl(int, int), ResourceCtorQ);
MSG_FUNC_NOT_IMPL(0x52008A, int __cdecl(DWORD), DoSleep);
MSG_FUNC_NOT_IMPL(0x42BE0A, int __cdecl(), sub_42BE0A);
MSG_FUNC_NOT_IMPL(0x4583BB, int __cdecl(), sub_4583BB);
MSG_FUNC_NOT_IMPL(0x51E086, int __cdecl(), sub_51E086);
MSG_FUNC_NOT_IMPL(0x4317B3, BOOL __cdecl(), Fonts_Release_sub_4317B3);

MGS_VAR(1, 0x6FC7E0, BYTE, byte_6FC7E0, 0);
MGS_VAR(1, 0x9AD89B, BYTE, byte_9AD89B, 0);
MGS_VAR(1, 0x73491C, DWORD, dword_73491C, 0);
MGS_VAR(1, 0x71D164, DWORD, dword_71D164, 0);
MGS_VAR(1, 0x6FC718, DWORD, dword_6FC718, 0);
MGS_VAR(1, 0x6FC720, DWORD, dword_6FC720, 0);
MGS_VAR(1, 0x6FC768, DWORD, dword_6FC768, 0);
MGS_VAR(1, 0x78E7F8, WORD, word_78E7F8, 0);
MGS_VAR(1, 0x78E7F6, WORD, word_78E7F6, 0);
MGS_VAR(1, 0x717354, DWORD, dword_717354, 0);
MGS_VAR(1, 0x717348, DWORD, dword_717348, 0);
MGS_VAR(1, 0x7348FC, DWORD, dword_7348FC, 0);
MGS_VAR(1, 0x732E64, DWORD, dword_732E64, 0);

MGS_VAR(1, 0x64BDA8, IID, IID_IDirectDraw7_MGS, {});
MGS_VAR(1, 0x64BB98, GUID, IID_IDirect3D7_MGS, {});
MGS_VAR(1, 0x64BCA8, GUID, IID_IDirectDrawGammaControl_MGS, {});
MGS_VAR(1, 0x6FC730, IDirectDraw7 *, g_pDirectDraw, nullptr);
MGS_VAR(1, 0x6FC748, IDirect3D7 *, g_pDirect3D, nullptr);
MGS_VAR(1, 0x6C0EF8, IDirectDrawGammaControl *, g_pGammaControl, nullptr);
MGS_VAR(1, 0x6DF214, DWORD, g_dwDisplayWidth, 0);
MGS_VAR(1, 0x6DF1FC, DWORD, g_dwDisplayHeight, 0);
MGS_VAR(1, 0x6FC734, LPDIRECTDRAWSURFACE7, g_pPrimarySurface, nullptr);
MGS_VAR(1, 0x6FC750, LPDIRECTDRAWCLIPPER, g_pClipper, nullptr);
MGS_VAR(1, 0x6FC738, LPDIRECTDRAWSURFACE7, g_pBackBuffer, nullptr);
MGS_VAR(1, 0x6FC74C, LPDIRECT3DDEVICE7, g_pDirect3DDevice, nullptr);
MGS_VAR(1, 0x6FC740, LPDIRECTDRAWSURFACE7, g_pDDSurface, nullptr);

MGS_VAR(1, 0x006DEF78, FILE *, gFile, nullptr);
MGS_VAR(1, 0x71D414, FILE *, gLogFile, nullptr);

MGS_VAR(1, 0x651CF8, DWORD, dword_651CF8, 0);
MGS_VAR(1, 0x716F5C, float, dword_716F5C, 0);
MGS_VAR(1, 0x716F78, DWORD, dword_716F78, 0);
MGS_VAR(1, 0x77C60C, DWORD, gDriverNum_dword_77C60C, 0);
MGS_VAR(1, 0x77C608, DWORD, gNumDrivers_dword_77C608, 0);
MGS_PTR(1, 0x776B94, DWORD *, dword_776B94, nullptr);
MGS_PTR(1, 0x776B90, DWORD *, dword_776B90, nullptr);
MGS_VAR(1, 0x716F74, DWORD, dword_716F74, 0);
MGS_VAR(1, 0x650D2C, DWORD, dword_650D2C, 0);
MGS_VAR(1, 0x6FC728, DWORD *, gImageBufer_dword_6FC728, 0);
MGS_VAR(1, 0x6DEF7C, void *, dword_6DEF7C, nullptr);
MGS_VAR(1, 0x6DEF90, void *, dword_6DEF90, nullptr);
MGS_VAR(1, 0x6FC72C, WORD*, g_pwTextureIndices, 0);
MGS_VAR(1, 0x6FC798, DWORD, dword_6FC798, 0);
MGS_VAR(1, 0x6FC7C0, DWORD, dword_6FC7C0, 0);
MGS_VAR(1, 0x716F6C, DWORD, dword_716F6C, 0);
MGS_VAR(1, 0x6FC7C4, DWORD, dword_6FC7C4, 0);
MGS_VAR(1, 0x651D94, DWORD, dword_651D94, 0);
MGS_VAR(1, 0x6FC79C, DWORD, g_surface565Mode, 0);
MGS_VAR(1, 0x716F60, DWORD, dword_716F60, 0);

MGS_PTR(1, 0x776B68, struct jimDeviceIdentifier *, g_pDeviceIdentifiers, nullptr); // TODO: Array?
MGS_PTR(1, 0x689B68, struct jimUnk0x204 *, array_689B68, nullptr); // TODO: Array?

MGS_VAR(1, 0x6C0778, char *, unk_6C0778, nullptr);
MGS_VAR(1, 0x006FC7E8, HFONT, gFont, nullptr);
MGS_VAR(1, 0x009ADDA0, HWND, gHwnd, nullptr);
MGS_VAR(1, 0x72279C, DWORD, dword_72279C, 0);
MGS_VAR(1, 0x6FC78C, WORD, g_NumTextures, 0);


//MSG_FUNC_NOT_IMPL(0x00422D40, char *__cdecl(char*, HRESULT), PrintDDError);
void __cdecl PrintDDError(const char* errMsg, HRESULT hrErr)
{
    char* pStrErr = nullptr;
    switch (hrErr)
    {
    case DDERR_NOBLTHW:                 pStrErr = "DD - NOBLTHW";                   break;
    case DDERR_WRONGMODE:               pStrErr = "DD - WRONGMODE";                 break;
    case DDERR_IMPLICITLYCREATED:       pStrErr = "DD - IMPLICITLYCREATED";         break;
    case DDERR_CLIPPERISUSINGHWND:      pStrErr = "DD - CLIPPERISUSINGHWND";        break;
    case DDERR_WASSTILLDRAWING:         pStrErr = "DD - WASSTILLDRAWING";           break;
    case DDERR_SURFACELOST:             pStrErr = "DD - SURFACELOST";               break;
    case DDERR_NOEXCLUSIVEMODE:         pStrErr = "DD - NOEXCLUSIVEMODE";           break;
    case DDERR_SURFACEBUSY:             pStrErr = "DD - DSURFACEBUSY";              break;
    case DDERR_INVALIDRECT:             pStrErr = "DD - INVALIDRECT";               break;
    case DDERR_INCOMPATIBLEPRIMARY:     pStrErr = "DD - DDERR_INCOMPATIBLEPRIMARY"; break;
    case DDERR_INVALIDCLIPLIST:         pStrErr = "DD - INVALIDCLIPLIST";           break;
    case DDERR_INVALIDOBJECT:           pStrErr = "DD - INVALIDOBJECT";             break;
    case DDERR_EXCEPTION:               pStrErr = "DD - EXCEPTION";                 break;
    case DDERR_INVALIDPARAMS:           pStrErr = "DD - INVALIDPARAMS";             break;
    case DDERR_OUTOFMEMORY:             pStrErr = "DD - DDERR_OUTOFMEMORY";         break;
    case DDERR_UNSUPPORTED:             pStrErr = "DD - UNSUPPORTED";               break;
    case DDERR_GENERIC:                 pStrErr = "DD - GENERIC";                   break;
    }

    char Dest[224] = {};
    if (!pStrErr)
    {
        sprintf(Dest, "Code Err=%i", hrErr);
        pStrErr = Dest;
    }

    if (hrErr)
    {
        for (int i = 0; i < 5; ++i)
        {
            printf("!edq %s !  %s\n", errMsg, pStrErr);
        }
    }
}
MSG_FUNC_IMPL(0x00422D40, PrintDDError);

//MSG_FUNC_NOT_IMPL(0x0041CC30, __int16 __cdecl(), Render_RestoreAll);
__int16 __cdecl Render_RestoreAll()
{
    if (g_pPrimarySurface->IsLost() == DDERR_SURFACELOST)
    {
        const HRESULT hr = g_pPrimarySurface->Restore();
        if (FAILED(hr))
        {
            PrintDDError("Prim restore caput", hr);
        }
    }

    if (g_pBackBuffer->IsLost() == DDERR_SURFACELOST)
    {
        const HRESULT hr = g_pBackBuffer->Restore();
        if (FAILED(hr))
        {
            PrintDDError("Ren restore caput", hr);
        }
    }

    for (int i = 0; i < g_NumTextures; i++)
    {
        if (gTextures_dword_6C0F00[i].mSurface)
        {
            if (gTextures_dword_6C0F00[i].mSurface->IsLost() == DDERR_SURFACELOST)
            {
                const HRESULT hr = gTextures_dword_6C0F00[i].mSurface->Restore();
                if (hr)
                {
                    PrintDDError("tex #%i restore caput", i);
                }
            }
        }
    }

    return g_NumTextures;
}
MSG_FUNC_IMPL(0x0041CC30, Render_RestoreAll);

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
    char var11C[0xFF];
    char var21B[0xFF];
    BYTE var21C = byte_6FC7E0;
    MSG oMsg;
    memset(var21B, 0, 0xFF);
    memset(var11C, 0, 0xFF);

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

struct Actor
{
    Actor* pPrevious;
    Actor* pNext;
    void(__cdecl *update)(Actor*);
    void(__cdecl *fnUnknown3)(Actor*);
    void(__cdecl *fnUnknown2)(Actor*);
    char* mNamePtr;
    DWORD field_18;
    DWORD field_1C;
};
static_assert(sizeof(Actor) == 0x20, "Actor should be 0x20");

// TODO: Could be linked list header?
struct struct_8
{
    DWORD mId;
    DWORD* field_4;
};
static_assert(sizeof(struct_8) == 0x8, "struct_8 should be 0x8");

struct struct_lib_gvd
{
    Actor mActor;
    DWORD field_6BFF00;
    DWORD field_6BFF04;
    struct_8* struct_8_ptr_6BFF08; // Seems to point to one of mStruct8_128Array_06BFF80
    int(__cdecl* field_6BFF0C[26])(DWORD*);
    struct_8* struct_8_ptr_6BFF74;
    struct_8* struct_8_ptr_6BFF78;
    DWORD pad_field_6BFF7C;
    struct_8 mStruct8_128Array_06BFF80[128];
    DWORD field_6C0380;
    DWORD field_6C0384;
    DWORD field_6C0388[6];
    DWORD field_6C03A0;
    DWORD field_6C03A4;
    DWORD field_6C03B0[81];
};
// TODO: This is actually probably bigger!
static_assert(sizeof(struct_lib_gvd) == 0x60C, "struct_lib_gvd should be 0x60C");

MGS_VAR(1, 0x6BFEE0, struct_lib_gvd, g_lib_gvd_stru_6BFEE0, {});

void __cdecl LibGvd_sub_40A69D()
{
    memset(g_lib_gvd_stru_6BFEE0.field_6BFF0C, 0, sizeof(g_lib_gvd_stru_6BFEE0.field_6BFF0C));
}
MSG_FUNC_IMPL(0x40A69D, LibGvd_sub_40A69D);

// Other likely LibGvd funcs
MSG_FUNC_NOT_IMPL(0x40A72A, struct_8 *__cdecl(), LibGvd_sub_40A72A);
MSG_FUNC_NOT_IMPL(0x40A6CD, char* __cdecl(), LibGvd_sub_40A6CD);
MSG_FUNC_NOT_IMPL(0x40A6AC, struct_8 *__cdecl(), LibGvd_128_inits_sub_40A6AC);
MSG_FUNC_NOT_IMPL(0x40A603, int __cdecl(int), LibGvd_sub_40A603);


struct ActorList
{
    Actor first;
    Actor last;
    WORD mPause;
    WORD mKill;
};
static_assert(sizeof(ActorList) == 0x44, "ActorList should be 0x44");

struct Rect16
{
    WORD x1, y1, x2, y2;
};
static_assert(sizeof(Rect16) == 8, "Rect16 should be 8");

MGS_VAR(1, 0x995344, DWORD, dword_995344, 0);
MGS_VAR(1, 0x722780, DWORD, dword_722780, 0);
MGS_VAR(1, 0x722784, DWORD, dword_722784, 0);
MGS_VAR(1, 0x7227A0, DWORD, dword_7227A0, 0);
MGS_VAR(1, 0x7227A4, DWORD, dword_7227A4, 0);
MGS_VAR(1, 0x9942B8, DWORD, dword_9942B8, 0);
MGS_VAR(1, 0x78D7B0, DWORD, dword_78D7B0, 0);
MGS_VAR(1, 0x78E7E8, WORD, word_78E7E8, 0);
MGS_VAR(1, 0x995324, DWORD, dword_995324, 0);
MGS_VAR(1, 0x7919C0, DWORD, dword_7919C0, 0);

MGS_VAR(1, 0x722760, Actor, g_gamed_722760, {}); // TODO: Will actually big an Actor + other data

//actor_related_struct* gActors = (actor_related_struct*)0x006BFC78; // Array of 9 items, TODO: Check correct
MGS_ARY(1, 0x006BFC78, ActorList, 9, gActors, {});

MGS_VAR(1, 0x78E7FC, WORD, word_78E7FC, 0);
MGS_VAR(1, 0x78E7FE, WORD, word_78E7FE, 0);
MGS_VAR(1, 0x78E960, DWORD, gResidentTop_dword_78E960, 0);
MGS_VAR(1, 0x78E964, DWORD, dword_78E964, 0);
MGS_VAR(1, 0x791A0C, DWORD, dword_791A0C, 0);
MGS_VAR(1, 0x9942A0, DWORD, dword_9942A0, 0);
MGS_VAR(1, 0x73492C, DWORD, gExitMainGameLoop, 0);
MGS_VAR(1, 0x994320, WORD, word_994320, 0);
MGS_VAR(1, 0x669AE0, WORD, word_669AE0, 0);
MGS_VAR(1, 0x993F44, DWORD, dword_993F44, 0);
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
MGS_VAR(1, 0x00650D28, float, gXRes, 0.0f);
MGS_VAR(1, 0x00650D34, DWORD, gNoFilter, 0);
MGS_VAR(1, 0x00650D30, DWORD, gModX2, 0);
MGS_VAR(1, 0x00650D40, DWORD, gNoTrueType, 0);
MGS_VAR(1, 0x006FC76C, DWORD, gFps, 0);
MGS_VAR(1, 0x006FC7A4, DWORD, gColourKey, 0);
MGS_VAR(1, 0x00650D38, DWORD, gBlendMode, 0);
MGS_VAR(1, 0x00650D20, DWORD, gLowRes, 0);
MGS_VAR(1, 0x688D40, char*, off_688D40, "");
MGS_VAR(1, 0x006FC794, DWORD, gSoftwareRendering, 0);
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
MGS_VAR(1, 0x688CD0, DWORD, dword_688CD0, 0);
MGS_VAR(1, 0x688CD4, DWORD, dword_688CD4, 0);
MGS_VAR(1, 0x688CD8, DWORD, dword_688CD8, 0);
MGS_VAR(1, 0x791DE4, DWORD, dword_791DE4, 0);
MGS_VAR(1, 0x9AD888, BYTE, byte_9AD888, 0);
MGS_VAR(1, 0x733E34, DWORD, dword_733E34, 0);
MGS_VAR(1, 0x721E78, DWORD, dword_721E78, 0);
MGS_VAR(1, 0x650D4C, DWORD, dword_650D4C, 0);
MGS_VAR(1, 0x0078E7C0, char * , gDest, nullptr);

struct weapon_famas
{
    ActorList mActor;
    DWORD field_44_a1;
    DWORD field_48_a2;
    DWORD field_4C_a3;
    DWORD field_50_a4;
    DWORD field_54;
    DWORD field_58;
    DWORD mbIsMp5;
};
static_assert(sizeof(weapon_famas) == 96, "weapon_famas should be 96");

MGS_VAR(1, 0x995368, WORD, word_995368, 0);
MGS_VAR(1, 0x995320, WORD, word_995320, 0);
MGS_VAR(1, 0x78E804, WORD, word_78E804, 0);

MSG_FUNC_NOT_IMPL_NOLOG(0x00640CDC, int __cdecl(weapon_famas*), Res_famas_sub_640CDC);
MSG_FUNC_NOT_IMPL(0x00640E9E, int* __cdecl(weapon_famas*), sub_640E9E);

Actor* __cdecl Actor_Init(Actor* a1, void(__cdecl *fn1)(Actor*), void(__cdecl *fn2)(Actor*), char *srcFileName);

MSG_FUNC_NOT_IMPL(0x0040B38E, int __cdecl(char*), ResourceRequestQ);
MSG_FUNC_NOT_IMPL(0x0044FF7C, int __cdecl(int, int, int), sub_44FF7C);
MSG_FUNC_NOT_IMPL(0x0045011B, int __cdecl(int, int, int), sub_45011B);

//MSG_FUNC_NOT_IMPL(0x00640EAD, signed int __cdecl(weapon_famas*, int, int, int), Res_Weapon_famas_init_sub_640EAD);
signed int __cdecl Res_Weapon_famas_init_sub_640EAD(weapon_famas *a1, int a2, int a3, int bMp5)
{
    int v4; // esi@1
    int res; // eax@2
    signed int result; // eax@5

    v4 = (int)&a1->mActor.last;
    if (bMp5)
        res = ResourceRequestQ("mpfive");
    else
        res = ResourceRequestQ("famas");
    sub_44FF7C(v4, res, 109);
    if (*(DWORD *)v4)
    {
        sub_45011B(v4, a2, a3);
        result = 0;
    }
    else
    {
        result = -1;
    }
    return result;
}

// TODO : check if ActorList or Actor
weapon_famas *__cdecl Res_Weapon_famas_96_sub_640C24(ActorList *a1, ActorList *a2, void(__cdecl *a3)(ActorList *), void(__cdecl *a4)(DWORD), int bMp5)
{
    weapon_famas *pFamas; // eax@1 MAPDST
    int v8; // eax@5
    __int16 v9; // cx@6

    pFamas = (weapon_famas *)ResourceCtorQ(6, 96);
    if (pFamas)
    {
        Actor_Init(&pFamas->mActor.first, (void(__cdecl *)(Actor*))Res_famas_sub_640CDC.Ptr(), (void(__cdecl *)(Actor*))sub_640E9E.Ptr(), "C:\\mgs\\source\\Weapon\\famas.c");
        if (Res_Weapon_famas_init_sub_640EAD(pFamas, (int)a2, (int)a3, bMp5) < 0)
        {
            Actor_SetRemoveFnPtr(&pFamas->mActor.first);
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
    v8 = (word_995368 != 0) + 25;                 // 25 is the ammo clip size
    if (bMp5)
    {
        word_995368 = (word_995368 != 0) + 25; // Remainder in clip
        word_995320 = v8; // clip size?
    }
    else
    {
        v9 = word_78E804;
        if (v8 > 0 && word_78E804 > v8)
            v9 = (word_995368 != 0) + 25;
        word_995320 = (word_995368 != 0) + 25;
        word_995368 = v9;
    }
    return pFamas;
}
MSG_FUNC_IMPL(0x640C24, Res_Weapon_famas_96_sub_640C24);

//MSG_FUNC_NOT_IMPL(0x0040A0D4, int __cdecl(), Actor_DumpActorSystem);
int __cdecl Actor_DumpActorSystem()
{
    int result; // eax@1
    int v1; // [sp+0h] [bp-18h]@6
    Actor* pNextActor; // [sp+4h] [bp-14h]@4
    Actor* pActorCopy; // [sp+8h] [bp-10h]@3
    signed int i; // [sp+10h] [bp-8h]@1
    ActorList* pActor; // [sp+14h] [bp-4h]@1

    pActor = gActors;
    result = printf("--DumpActorSystem--\n");
    for (i = 0; i < 9; ++i)
    {
        printf("Lv %d Pause %d Kill %d\n", i, pActor->mPause, pActor->mKill);
        pActorCopy = &pActor->first;
        do
        {
            pNextActor = pActorCopy->pNext;
            if (pActorCopy->update)
            {
                if (pActorCopy->field_1C <= 0)
                    v1 = 0;
                else
                    v1 = 100 * pActorCopy->field_18 / pActorCopy->field_1C;
                printf("Lv%d %04d.%02d %08X %s\n", i, v1 / 100, v1 % 100, pActorCopy->update, pActorCopy->mNamePtr);
                pActorCopy->field_1C = 0;
                pActorCopy->field_18 = 0;
            }
            pActorCopy = pNextActor;
        } while (pNextActor);
        ++pActor;
        result = i + 1;
    }
    return result;
}

//MSG_FUNC_NOT_IMPL(0x0040A37C, void __cdecl (Actor*), Actor_Remove);
void __cdecl Actor_Remove(Actor* pActor)
{
    if (!pActor)
        return;

    pActor->pNext->pPrevious = pActor->pPrevious;
    pActor->pPrevious->pNext = pActor->pNext;
    pActor->pPrevious = 0;
    pActor->pNext = 0;

    if (pActor->fnUnknown3)
        pActor->fnUnknown3(pActor);

    if (pActor->fnUnknown2)
        pActor->fnUnknown2(pActor);
}

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
        byte_9AD988 = wParam;
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
                    dword_72279C = 0;
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
                    if (dword_72279C != 0x20000000 || !strstr(gDest, "s19a"))
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
MGS_ARY(1, 0x00662EC0, char, 8*1024, gStrErrStrings, {}); // 2d array of [8][1024]

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
                result = MessageBoxA(hWnd, &gStrErrStrings[msgIdx * 1024], lpCaption, uType);
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
        result = MessageBoxA(hWnd, &gStrErrStrings[errCode * 1024], lpCaption, uType);
    }
    return result;
}

// 0x423F1B
HFONT __cdecl sub_423F1B(int cWidth, int cHeight)
{
    HFONT result; // eax@3

    if (gFont)
        DeleteObject(gFont);
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



MSG_FUNC_NOT_IMPL(0x51E29B, int __cdecl(DDDEVICEIDENTIFIER2*, jimDeviceDDId*, int), File_msgvideocfg_Read);

//MSG_FUNC_NOT_IMPL(0x51E7FC, int __cdecl(LPD3DDEVICEDESC7, LPSTR, LPSTR, jimDeviceIdentifier*), validateDeviceCaps);
int __cdecl validateDeviceCaps(LPD3DDEVICEDESC7 pDesc, LPSTR lpDeviceDescription, LPSTR lpDeviceName, jimDeviceIdentifier* pIdentifier)
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
    if (!(pDesc->dpcTriCaps.dwShadeCaps & (D3DPSHADECAPS_ALPHAFLATBLEND || D3DPSHADECAPS_ALPHAGOURAUDBLEND)))
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

MSG_FUNC_IMPL(0x51E7FC, validateDeviceCaps);

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
BOOL WINAPI DDEnumCallbackEx(GUID *lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm)
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

MSG_FUNC_NOT_IMPL(0x0051F22F, int __cdecl(), jim_enumerate_devices_);
int __cdecl jim_enumerate_devices_z()
{
    abort();

    int varC;
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
    while (true)
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
                var8++;
        }
        if (File_msgvideocfg_Write(&Buf1, varC) == 0)
            var8++;
    }

    if (gNumDrivers_dword_77C608 > 2)
        gNumDrivers_dword_77C608 = 2;

    dword_68C3B8 = 1;
    for (varC = 0; varC < gNumDrivers_dword_77C608; varC++)
    {
        memset(&array_689B68[dword_68C3B8], 0, 0x204);
        strncpy(array_689B68[dword_68C3B8].string, (g_pDeviceIdentifiers + varC)->ddIdentifier.identifier.szDescription, 0x200);
        array_689B68[dword_68C3B8].field200 = (g_pDeviceIdentifiers + varC)->ddIdentifier.field434;
        
        if ((g_pDeviceIdentifiers + varC)->ddIdentifier.field430 & 2)
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
int __cdecl ClearDDSurfaceWhite()
{
    HRESULT hr;

    DDBLTFX bltFX;
    bltFX.dwSize = sizeof(DDBLTFX);
    bltFX.dwFillColor = 0xFFFF;

    do {
        hr = g_pBackBuffer->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &bltFX);
    } while (hr == DDERR_WASSTILLDRAWING);

    if (hr != 0)
        return 0;

    return 1;
}

#define MGSVERTEX_DEF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
struct MGSVertex
{
    float x;
    float y;
    float z;
    float w;
    DWORD diffuse;
    DWORD specular;
    float u;
    float v;
};
static_assert(sizeof(MGSVertex) == 0x20, "MGSVertex must be of size 0x20");

MGS_VAR(1, 0x6FC780, MGSVertex*, g_pMGSVertices, 0);


//MSG_FUNC_NOT_IMPL(0x0041ECB0, signed int __cdecl(), InitD3d_ProfileGfxHardwareQ);

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
    Input_Start();
    mgs_fputs("jim_enumerate_devices()\n", gFile);
    mgs_fflush(gFile);
    v55 = jim_enumerate_devices_();
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
    while (1)
    {
        g_dwDisplayWidth = (signed __int64)(320.0 * gXRes);
        g_dwDisplayHeight = (signed __int64)(240.0 * gXRes);
        mgs_fputs("Creating DirectDraw7\n", gFile);
        mgs_fflush(gFile);
        hr = CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_ALL, IID_IDirectDraw7, (void**)&g_pDirectDraw);
        if (!FAILED(hr))
        {
            hr = g_pDirectDraw->Initialize(NULL);
        }
        //hr = DirectDrawCreateExMGS(lpGuid, (LPVOID*)&g_pDirectDraw, &IID_IDirectDraw7_MGS, 0);
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
                g_dwDisplayWidth = (signed __int64)(320.0 * 1.0f);
                g_dwDisplayHeight = (signed __int64)(240.0 * 1.0f);
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
            if (hr)
                PrintDDError("Can't release primary surf", hr);
            g_pPrimarySurface = 0;
        }
        if (g_pBackBuffer)
        {
            hr = g_pBackBuffer->Release();
            if (hr)
                PrintDDError("Can't release render surf", hr);
            g_pBackBuffer = 0;
        }
        if (g_pClipper)
        {
            hr = g_pClipper->Release();
            if (hr)
                PrintDDError("Can't release clipper", hr);
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
        if (hr < 0)
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
            if (hr)
            {
                mgs_fputs(" . fail\n", gFile);
                mgs_fflush(gFile);
                PrintDDError("Can't create clipper", hr);
                return 0;
            }
            hr = g_pClipper->SetHWnd(0, gHwnd);
            if (hr)
            {
                mgs_fputs(" . fail\n", gFile);
                mgs_fflush(gFile);
                PrintDDError("Can't obtain clipper zone", hr);
                return 0;
            }
            hr = g_pPrimarySurface->SetClipper(g_pClipper);
            if (hr)
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
            if (dword_651CF8 || dword_716F6C && dword_716F6C != 1)
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
        if (hr)
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
        if (hr)
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
        //mgs_fprintf(gFile, "565 mode = %i\n", g_surface565Mode);
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

        if (hr >= 0)
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
                dxSurfaceDesc3.dwFlags = 4103;// DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
                memcpy(&dxSurfaceDesc3.ddpfPixelFormat, &pixelFormat, sizeof(DDPIXELFORMAT));
                dxSurfaceDesc3.dwWidth = 16;
                dxSurfaceDesc3.dwHeight = 16;
                dxSurfaceDesc3.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
                dxSurfaceDesc3.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;

                hr = g_pDirectDraw->CreateSurface(&dxSurfaceDesc3, &g_pDDSurface, 0);
                if (hr)
                {
                    g_pDDSurface = 0;
                }
                else if (!ClearDDSurfaceWhite())
                    {

                        g_pDDSurface->Release();
                        g_pDDSurface = 0;
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
                mgs_fprintf(gFile, "Alpha modulate = %i \n", dword_6FC798);
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
                    if (gLowRes != gLowRes)
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
            if (hr)
                PrintDDError("Can't release render surf", hr);
            g_pBackBuffer = 0;
        }
        if (g_pPrimarySurface)
        {
            hr = g_pPrimarySurface->Release();
            if (hr)
                PrintDDError("Can't relaese primary surf", hr);
            g_pPrimarySurface = 0;
        }
        if (g_pClipper)
        {
            hr = g_pClipper->Release();
            if (hr)
                PrintDDError("Can't release clipper", hr);
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
    sub_423F1B(0, (signed __int64)(14.0 * gXRes));
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
            memset(&unk_6C0778, 0, 0x400u);
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

//MSG_FUNC_IMPL(0x0041ECB0, InitD3d_ProfileGfxHardwareQ);


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

struct MGSSmallVert
{
    WORD x; // 11.1 bits
    WORD y; // 11.1 bits
};

struct MGSFloatVert
{
    float x;
    float y;
};

struct StructVert
{
    uint8_t pad0;
    uint8_t pad1;
    uint8_t pad2;
    uint8_t structType;
};

struct StructVertType0
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t structType;
    MGSSmallVert Vtxs[4];
};

struct MGSLargeVert
{
    MGSSmallVert Vtx;
    uint8_t u;
    uint8_t v;
    uint16_t textureIdx;
};

struct StructVertType1
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t structType;
    MGSLargeVert TexVtx[4];
};

struct MGSDiffuseVert
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t padding;
    MGSSmallVert Vtx;
};

struct StructVertType2
{
    MGSDiffuseVert DifVtx[4];
};

struct MGSLargeVertDif
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t padding;
    MGSSmallVert Vtx;
    uint8_t u;
    uint8_t v;
    uint16_t textureIdx;
};

struct StructVertType3
{
    MGSLargeVertDif DifVtx[3];
};

struct MGSVertType4
{
    MGSFloatVert Vtx;
    uint8_t u;
    uint8_t v;
    uint16_t textureIdx;
};

struct StructVertType4
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t padding;
    MGSVertType4 Vtx[4];
};

struct StructVertType5
{
    uint32_t field0;
};

MGS_VAR(1, 0x791C54, DWORD, dword_791C54, 0);
MGS_VAR(1, 0x791C58, DWORD, dword_791C58, 0);
MGS_VAR(1, 0x6FC774, DWORD, dword_6FC774, 0);
MGS_VAR(1, 0x791C5C, float, g_fV3, 0);
MGS_VAR(1, 0x791C60, float, g_fV2, 0);
MGS_VAR(1, 0x791C64, float, g_fV1, 0);
MGS_VAR(1, 0x791C68, float, g_fV0, 0);
MGS_VAR(1, 0x791C6C, float, g_fU3, 0);
MGS_VAR(1, 0x791C70, float, g_fU2, 0);
MGS_VAR(1, 0x791C74, float, g_fU1, 0);
MGS_VAR(1, 0x791C78, float, g_fU0, 0);
MGS_VAR(1, 0x6FC788, DWORD, g_nPrimitiveIndex, 0);
MGS_VAR(1, 0x6FC784, DWORD, g_nVertexOffset, 0);
MGS_VAR(1, 0x6C0EA0, WORD, g_wXOffset, 0);
MGS_VAR(1, 0x6C0EA2, WORD, g_wYOffset, 0);
MGS_VAR(1, 0x791C80, float, g_fXOffset, 0);
MGS_VAR(1, 0x791C84, float, g_fYOffset, 0);
MGS_VAR(1, 0x791C7C, DWORD, g_nTextureIndex, 0);
MGS_VAR(1, 0x6C0EAC, WORD, word_6C0EAC, 0);
MGS_VAR(1, 0x6C0EAE, WORD, word_6C0EAE, 0);
MGS_VAR(1, 0x6C0E98, WORD, word_6C0E98, 0);
MGS_VAR(1, 0x6C0E9A, WORD, word_6C0E9A, 0);
MGS_VAR(1, 0x6C0E9C, WORD, word_6C0E9C, 0);
MGS_VAR(1, 0x6C0E9E, WORD, word_6C0E9E, 0);


MSG_FUNC_NOT_IMPL(0x44EAE5, uint32_t __cdecl(), sub_44EAE5);
MSG_FUNC_NOT_IMPL(0x40CC50, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t*, uint32_t*), Render_ComputeTextureIdx);
MSG_FUNC_NOT_IMPL(0x40CD80, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t, float*, float*), Render_ComputeUVs);
MSG_FUNC_NOT_IMPL(0x40FF20, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t, float*, float*), sub_40FF20);
MSG_FUNC_NOT_IMPL(0x40D540, uint32_t __cdecl(int16_t*, int32_t, int32_t), sub_40D540);


void convertVertexType0(StructVertType0* pStructVert, uint32_t nIndex)
{
    int32_t signedX, signedY;
    signedX = pStructVert->Vtxs[nIndex].x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->Vtxs[nIndex].y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

void convertColorWZType0(StructVertType0* pStructVert)
{
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

void convertVertexType1(StructVertType1* pStructVert, uint32_t nIndex, float u, float v)
{
    int32_t signedX, signedY;
    signedX = pStructVert->TexVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->TexVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (pStructVert->diffuseR * 0x19A) >> 8;
        uint32_t G = (pStructVert->diffuseG * 0x19A) >> 8;
        uint32_t B = (pStructVert->diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }
    
    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

void convertExceptPosType1(StructVertType1* pStructVert, float u, float v)
{
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (pStructVert->diffuseR * 0x19A) >> 8;
        uint32_t G = (pStructVert->diffuseG * 0x19A) >> 8;
        uint32_t B = (pStructVert->diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }

    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

void convertVertexType2(StructVertType2* pStructVert, uint32_t nIndex)
{
    int32_t signedX, signedY;
    signedX = pStructVert->DifVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->DifVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->DifVtx[nIndex].diffuseR << 16) | (pStructVert->DifVtx[nIndex].diffuseG << 8) | (pStructVert->DifVtx[nIndex].diffuseB);
    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

void convertVertexType3(StructVertType3* pStructVert, uint32_t nIndex, float u, float v)
{
    int32_t signedX, signedY;
    signedX = pStructVert->DifVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->DifVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->DifVtx[nIndex].diffuseR << 16) | (pStructVert->DifVtx[nIndex].diffuseG << 8) | (pStructVert->DifVtx[nIndex].diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (pStructVert->DifVtx[nIndex].diffuseR * 0x19A) >> 8;
        uint32_t G = (pStructVert->DifVtx[nIndex].diffuseG * 0x19A) >> 8;
        uint32_t B = (pStructVert->DifVtx[nIndex].diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }

    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

float convertPositionFloat(WORD n)
{
    int32_t signedN;
    signedN = n << 20;
    signedN >>= 20;
    return (float)signedN;
}

uint32_t calculateModX2Diffuse(uint32_t diffuseR, uint32_t diffuseG, uint32_t diffuseB)
{
    uint32_t diffuseColor = 0xFF000000 | (diffuseR << 16) | (diffuseG << 8) | (diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (diffuseR * 0x19A) >> 8;
        uint32_t G = (diffuseG * 0x19A) >> 8;
        uint32_t B = (diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }

    return diffuseColor;
}

void handleBlendMode(uint16_t nBlend)
{
    if ((dword_791C54 & 2) != 0)
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 1 + ((nBlend >> 5) & 3);
    else
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 0;
}

void handleBlendMode(uint16_t nBlend, uint32_t offset)
{
    if ((dword_791C54 & 2) != 0)
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 1 + offset + ((nBlend >> 5) & 3);
    else
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = offset;
}

void convertVertexType4(StructVertType4* pStructVert, uint32_t nIndex, float u, float v)
{
    g_pMGSVertices[g_nVertexOffset].x = pStructVert->Vtx[nIndex].Vtx.x + g_fXOffset;
    g_pMGSVertices[g_nVertexOffset].y = pStructVert->Vtx[nIndex].Vtx.y + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;
    g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->diffuseR, pStructVert->diffuseG, pStructVert->diffuseB);
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

MSG_FUNC_NOT_IMPL(0x418A70, int __cdecl(StructVert* a_pStructVert, int a_nSize), Render_Software);

// Untested for the moment
//MSG_FUNC_NOT_IMPL(0x410560, int __cdecl(StructVert* a_pStructVert, int a_nSize), ConvertPolys_Hardware);
int __cdecl ConvertPolys_Hardware(StructVert* a_pStructVert, int a_nSize)
{
    uint32_t var14 = dword_688CD4;
    uint32_t var1C = dword_688CD0;

    while (true)
    {
        if (a_nSize <= 0)
            return 1;

        dword_791C54 = a_pStructVert->structType;
        dword_791C58 = 0;
        g_fV3 = g_fV2 = g_fV1 = g_fV0 = 0;
        g_fU3 = g_fU2 = g_fU1 = g_fU0 = 0;

        gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 0;
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 0;

        switch (dword_791C54)
        {
        case 0:
            return 1;

        case 32:
        case 33:
        case 34:
        case 35:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLELIST;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            dword_791C58 = 4;
            break;
        }

        case 40:
        case 41:
        case 42:
        case 43:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);
            convertVertexType0(pStructVert, 3);

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;
            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            dword_791C58 = 5;
            break;
        }

        case 44:
        case 45:
        case 46:
        case 47:
        {
            StructVertType1* pStructVert = (StructVertType1*)a_pStructVert;
            uint32_t TextureIdx0, TextureIdx1;
            Render_ComputeTextureIdx(pStructVert->TexVtx[1].textureIdx, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1);
            TextureIdx0 &= 0xFFFF;
            TextureIdx1 &= 0xFFFF;

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
            g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

            if (g_nTextureIndex >= g_NumTextures)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
            }
            else
            {
                uint32_t var7C = (pStructVert->TexVtx[1].textureIdx & 0x180) >> 7;
                Render_ComputeUVs(g_nTextureIndex, var7C, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &g_fU0, &g_fV0);
                Render_ComputeUVs(g_nTextureIndex, var7C, pStructVert->TexVtx[1].u, pStructVert->TexVtx[1].v, &g_fU1, &g_fV1);
                Render_ComputeUVs(g_nTextureIndex, var7C, pStructVert->TexVtx[2].u, pStructVert->TexVtx[2].v, &g_fU2, &g_fV2);
                Render_ComputeUVs(g_nTextureIndex, var7C, pStructVert->TexVtx[3].u, pStructVert->TexVtx[3].v, &g_fU3, &g_fV3);
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->TexVtx[1].textureIdx);

            convertVertexType1(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType1(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType1(pStructVert, 2, g_fU2, g_fV2);
            convertVertexType1(pStructVert, 3, g_fU3, g_fV3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            dword_791C58 = 9;
            break;
        }

        case 48:
        case 49:
        case 50:
        case 51:
        {
            StructVertType2* pStructVert = (StructVertType2*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType2(pStructVert, 0);
            convertVertexType2(pStructVert, 1);
            convertVertexType2(pStructVert, 2);

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;
            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLELIST;

            dword_791C58 = 6;
            break;
        }

        case 52:
        case 53:
        case 54:
        case 55:
        {
            StructVertType3* pStructVert = (StructVertType3*)a_pStructVert;

            uint32_t TextureIdx0, TextureIdx1;
            Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &TextureIdx0, &TextureIdx1);
            TextureIdx0 &= 0xFFFF;
            TextureIdx1 &= 0xFFFF;

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
            g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

            if (g_nTextureIndex >= g_NumTextures)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
            }
            else
            {
                uint32_t var110 = (pStructVert->DifVtx[1].textureIdx & 0x180) >> 7;
                Render_ComputeUVs(g_nTextureIndex, var110, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &g_fU0, &g_fV0);
                Render_ComputeUVs(g_nTextureIndex, var110, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &g_fU1, &g_fV1);
                Render_ComputeUVs(g_nTextureIndex, var110, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &g_fU2, &g_fV2);
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->DifVtx[1].textureIdx);

            convertVertexType3(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType3(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType3(pStructVert, 2, g_fU2, g_fV2);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLELIST;

            dword_791C58 = 9;
        }

        case 56:
        case 57:
        case 58:
        case 59:
        {
            StructVertType2* pStructVert = (StructVertType2*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            if (gPrimStructArray[g_nPrimitiveIndex].nBlendMode == 3)
            {
                for (int i = 0; i < 4; i++)
                {
                    pStructVert->DifVtx[i].diffuseB = (uint8_t)min(pStructVert->DifVtx[i].diffuseB << 2, 0xFF);
                    pStructVert->DifVtx[i].diffuseG = (uint8_t)min(pStructVert->DifVtx[i].diffuseG << 2, 0xFF);
                    pStructVert->DifVtx[i].diffuseR = (uint8_t)min(pStructVert->DifVtx[i].diffuseR << 2, 0xFF);
                }
            }
            
            convertVertexType2(pStructVert, 0);
            convertVertexType2(pStructVert, 1);
            convertVertexType2(pStructVert, 2);
            convertVertexType2(pStructVert, 3);

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;
            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            if (gPrimStructArray[g_nPrimitiveIndex].nBlendMode == 3 && dword_6FC774 != 0)
            {
                convertVertexType2(pStructVert, 0);
                convertVertexType2(pStructVert, 1);
                convertVertexType2(pStructVert, 2);
                convertVertexType2(pStructVert, 3);

                gPrimStructArray[g_nPrimitiveIndex].dwVertexCount += 4;
            }

            dword_791C58 = 8;
            break;
        }

        case 60:
        case 61:
        case 62:
        case 63:
        {
            StructVertType3* pStructVert = (StructVertType3*)a_pStructVert;
            float position[16];
            position[11] = 0.5f;
            position[10] = 0.5f;
            position[ 9] = 0.5f;
            position[ 8] = 0.5f;
            position[15] = 1.0f;
            position[14] = 0.999999f;
            position[13] = 0.999999f;
            position[12] = 0.999999;

            position[3] = convertPositionFloat(pStructVert->DifVtx[0].Vtx.x);
            position[2] = convertPositionFloat(pStructVert->DifVtx[1].Vtx.x);
            position[1] = convertPositionFloat(pStructVert->DifVtx[2].Vtx.x);
            position[0] = convertPositionFloat(pStructVert->DifVtx[3].Vtx.x);

            position[7] = convertPositionFloat(pStructVert->DifVtx[0].Vtx.y);
            position[6] = convertPositionFloat(pStructVert->DifVtx[1].Vtx.y);
            position[5] = convertPositionFloat(pStructVert->DifVtx[2].Vtx.y);
            position[4] = convertPositionFloat(pStructVert->DifVtx[3].Vtx.y);

            if ((pStructVert->DifVtx[3].textureIdx & 0x8000) != 0 && var1C != 0)
            {
                float* var68 = (float*)(0x734A40 + ((pStructVert->DifVtx[3].textureIdx & 0xFFF) << 6)); // TODO : Var this struct (sizeof = 0x40)
                if (var68[8] > 0.0005f)
                    position[15] = 1.0f / var68[8];
                if (var68[9] > 0.0005f)
                    position[14] = 1.0f / var68[9];
                if (var68[10] > 0.0005f)
                    position[13] = 1.0f / var68[10];
                if (var68[11] > 0.0005f)
                    position[12] = 1.0f / var68[11];

                if (var14 != 0)
                {
                    position[3] = var68[0];
                    position[2] = var68[1];
                    position[1] = var68[2];
                    position[0] = var68[3];

                    position[7] = var68[4];
                    position[6] = var68[5];
                    position[5] = var68[6];
                    position[4] = var68[7];
                }
            }

            uint32_t TextureIdx0, TextureIdx1;
            if (Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &TextureIdx0, &TextureIdx1) != 0)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFE;
                g_fU0 = (float)(TextureIdx0 & 0xFFFF);
                g_fV0 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &TextureIdx0, &TextureIdx1);
                g_fU1 = (float)(TextureIdx0 & 0xFFFF);
                g_fV1 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &TextureIdx0, &TextureIdx1);
                g_fU2 = (float)(TextureIdx0 & 0xFFFF);
                g_fV2 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[3].u, pStructVert->DifVtx[3].v, &TextureIdx0, &TextureIdx1);
                g_fU3 = (float)(TextureIdx0 & 0xFFFF);
                g_fV3 = (float)(TextureIdx1 & 0xFFFF);
            }
            else
            {
                TextureIdx0 &= 0xFFFF;
                TextureIdx1 &= 0xFFFF;

                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
                g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

                if (g_nTextureIndex >= g_NumTextures)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
                }
                else
                {
                    uint32_t var6C = (pStructVert->DifVtx[1].textureIdx & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, var6C, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, var6C, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, var6C, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, var6C, pStructVert->DifVtx[3].u, pStructVert->DifVtx[3].v, &g_fU3, &g_fV3);

                    uint16_t* pIndex = (uint16_t*)(0x6FC728 + ((pStructVert->DifVtx[0].textureIdx >> 6) << 11) + ((pStructVert->DifVtx[0].textureIdx & 0x3F) << 5));
                    if (*pIndex == 0xEDED)
                    {
                        gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFF0;
                        dword_791C54 &= 0xFFFFFFFD;
                    }
                }
            }
            
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->DifVtx[1].textureIdx);

            g_pMGSVertices[g_nVertexOffset].x = position[ 3] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[ 7] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[11];
            g_pMGSVertices[g_nVertexOffset].w = position[15];
            g_pMGSVertices[g_nVertexOffset].u = g_fU0;
            g_pMGSVertices[g_nVertexOffset].v = g_fV0;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[0].diffuseR, pStructVert->DifVtx[0].diffuseG, pStructVert->DifVtx[0].diffuseB);
            g_nVertexOffset++;

            g_pMGSVertices[g_nVertexOffset].x = position[ 2] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[ 6] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[10];
            g_pMGSVertices[g_nVertexOffset].w = position[14];
            g_pMGSVertices[g_nVertexOffset].u = g_fU1;
            g_pMGSVertices[g_nVertexOffset].v = g_fV1;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[1].diffuseR, pStructVert->DifVtx[1].diffuseG, pStructVert->DifVtx[1].diffuseB);
            g_nVertexOffset++;

            g_pMGSVertices[g_nVertexOffset].x = position[ 1] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[ 5] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[ 9];
            g_pMGSVertices[g_nVertexOffset].w = position[13];
            g_pMGSVertices[g_nVertexOffset].u = g_fU2;
            g_pMGSVertices[g_nVertexOffset].v = g_fV2;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[2].diffuseR, pStructVert->DifVtx[2].diffuseG, pStructVert->DifVtx[2].diffuseB);
            g_nVertexOffset++;

            g_pMGSVertices[g_nVertexOffset].x = position[ 0] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[ 4] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[ 8];
            g_pMGSVertices[g_nVertexOffset].w = position[12];
            g_pMGSVertices[g_nVertexOffset].u = g_fU3;
            g_pMGSVertices[g_nVertexOffset].v = g_fV3;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[3].diffuseR, pStructVert->DifVtx[3].diffuseG, pStructVert->DifVtx[3].diffuseB);
            g_nVertexOffset++;

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            dword_791C58 = 0xC;
            break;
        }

        case 64:
        case 65:
        case 66:
        case 67:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            float fXSize, fYSize;
            sub_40FF20(pStructVert->Vtxs[0].x, pStructVert->Vtxs[0].y, pStructVert->Vtxs[1].x, pStructVert->Vtxs[1].y, &fXSize, &fYSize);
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 0);
            g_pMGSVertices[g_nVertexOffset - 1].x += fXSize;
            g_pMGSVertices[g_nVertexOffset - 1].y += fYSize;
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 1);
            g_pMGSVertices[g_nVertexOffset - 1].x += fXSize;
            g_pMGSVertices[g_nVertexOffset - 1].y += fYSize;

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            dword_791C58 = 3;
            break;
        }

        case 72:
        case 73:
        case 74:
        case 75:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_LINESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            dword_791C58 = 5;
            break;
        }

        case 76:
        case 77:
        case 78:
        case 79:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);
            convertVertexType0(pStructVert, 3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_LINESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            dword_791C58 = 6;
            break;
        }

        case 96:
        case 97:
        case 98:
        case 99:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;

            float fSecondX = convertPositionFloat(pStructVert->Vtxs[0].x) + (float)((int16_t)pStructVert->Vtxs[1].x);
            float fSecondY = convertPositionFloat(pStructVert->Vtxs[0].y) + (float)((int16_t)pStructVert->Vtxs[1].y);

            handleBlendMode(word_6C0EAC);
            convertVertexType0(pStructVert, 0);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = convertPositionFloat(pStructVert->Vtxs[0].y) + g_fYOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = convertPositionFloat(pStructVert->Vtxs[0].x) + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertColorWZType0(pStructVert);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            dword_791C58 = 3;
            break;
        }

        case 100:
        case 101:
        case 102:
        case 103:
        {
            StructVertType1* pStructVert = (StructVertType1*)a_pStructVert;
            float fInverseRes = 1.0f / gXRes;

            int16_t diffX = (int16_t)pStructVert->TexVtx[1].Vtx.x;
            int16_t diffY = (int16_t)pStructVert->TexVtx[1].Vtx.y;
            float fSecondX = convertPositionFloat(pStructVert->TexVtx[0].Vtx.x) + (float)diffX;
            float fSecondY = convertPositionFloat(pStructVert->TexVtx[0].Vtx.y) + (float)diffY;

            uint32_t TextureIdx0, TextureIdx1;
            if (Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1) != 0)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFD;
                g_fU0 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV0 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u + diffX - 1, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1);
                g_fU1 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV1 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v + diffY, &TextureIdx0, &TextureIdx1);
                g_fU2 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV2 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u + diffX - 1, pStructVert->TexVtx[0].v + diffY, &TextureIdx0, &TextureIdx1);
                g_fU3 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV3 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;
            }
            else
            {
                if (pStructVert->TexVtx[0].textureIdx & 0x8000)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 1 + (pStructVert->TexVtx[0].textureIdx & 0xF);
                    g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;
                    pStructVert->TexVtx[0].u = 0;
                    pStructVert->TexVtx[0].v = 0;
                }
                else
                {
                    TextureIdx0 &= 0xFFFF;
                    TextureIdx1 &= 0xFFFF;
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
                    g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;
                }
                if (g_nTextureIndex >= g_NumTextures)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
                }
                else
                {
                    uint32_t var9C = (word_6C0EAC & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, var9C, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, var9C, pStructVert->TexVtx[1].u + diffX, pStructVert->TexVtx[1].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, var9C, pStructVert->TexVtx[2].u, pStructVert->TexVtx[2].v + diffY, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, var9C, pStructVert->TexVtx[3].u + diffX, pStructVert->TexVtx[3].v + diffY, &g_fU3, &g_fV3);
                }
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC, 0x8000);

            convertVertexType1(pStructVert, 0, g_fU0, g_fV0);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = convertPositionFloat(pStructVert->TexVtx[0].Vtx.y) + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU1, g_fV1);
            g_pMGSVertices[g_nVertexOffset].x = convertPositionFloat(pStructVert->TexVtx[0].Vtx.x) + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU2, g_fV2);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU3, g_fV3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            dword_791C58 = 4;
            break;
        }

        case 104:
        case 105:
        case 106:
        case 107:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            float fHalfOffset = (((gXRes - 1.0f) / 2.0f) + 1.0f) / gXRes;

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            float centerX = convertPositionFloat(pStructVert->Vtxs[0].x);
            float centerY = convertPositionFloat(pStructVert->Vtxs[0].y);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset - fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset - fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset + fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset - fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset - fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset + fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset + fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset + fHalfOffset;
            convertColorWZType0(pStructVert);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            dword_791C58 = 2;
            break;
        }

        case 128:
        {
            StructVertType0* pStructVert = (StructVertType0*)a_pStructVert;
            int16_t rawPos[4];
            rawPos[1] = pStructVert->Vtxs[0].y;
            rawPos[0] = pStructVert->Vtxs[0].x;

            int16_t varDC = pStructVert->Vtxs[1].y;
            int16_t varD8 = pStructVert->Vtxs[1].x;

            rawPos[3] = pStructVert->Vtxs[2].y;
            rawPos[2] = pStructVert->Vtxs[2].x;

            sub_40D540(rawPos, varD8, varDC);

            dword_791C58 = 4;
            break;
        }

        case 144:
        case 145:
        case 146:
        case 147:
        {
            StructVertType4* pStructVert = (StructVertType4*)a_pStructVert;
            if ((pStructVert->Vtx[0].textureIdx & 0x8000) != 0)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFF0;
            }
            else
            {
                uint32_t TextureIdx0, TextureIdx1;
                Render_ComputeTextureIdx(pStructVert->Vtx[1].textureIdx, pStructVert->Vtx[0].u, pStructVert->Vtx[0].v, &TextureIdx0, &TextureIdx1);
                TextureIdx0 &= 0xFFFF;
                TextureIdx1 &= 0xFFFF;
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
                g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

                if (g_nTextureIndex >= g_NumTextures)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
                }
                else
                {
                    uint32_t var124 = (pStructVert->Vtx[1].textureIdx & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, var124, pStructVert->Vtx[0].u, pStructVert->Vtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, var124, pStructVert->Vtx[1].u, pStructVert->Vtx[1].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, var124, pStructVert->Vtx[2].u, pStructVert->Vtx[2].v, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, var124, pStructVert->Vtx[3].u, pStructVert->Vtx[3].v, &g_fU3, &g_fV3);
                }
            }
            
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType4(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType4(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType4(pStructVert, 2, g_fU2, g_fV2);
            convertVertexType4(pStructVert, 3, g_fU3, g_fV3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            dword_791C58 = 0xD;
            break;
        }

        case 225:
        {
            StructVertType5* pStructVert = (StructVertType5*)a_pStructVert;

            word_6C0EAC = pStructVert->field0 & 0x1FF;
            word_6C0EAE = (((pStructVert->field0 >> 10) & 1) << 8) | ((pStructVert->field0 >> 9) & 1);

            dword_791C58 = 1;
            break;
        }

        case 226:
        {
            dword_791C58 = 1;
            break;
        }

        case 227:
        {
            StructVertType5* pStructVert = (StructVertType5*)a_pStructVert;

            word_6C0E98 = pStructVert->field0 & 0x3FF;
            word_6C0E9A = (pStructVert->field0 >> 10) & 0x3FF;

            dword_791C58 = 1;
            break;
        }

        case 228:
        {
            StructVertType5* pStructVert = (StructVertType5*)a_pStructVert;

            word_6C0E9C = (pStructVert->field0 & 0x3FF) - word_6C0E98 + 1;
            word_6C0E9E = ((pStructVert->field0 >> 10) & 0x3FF) - word_6C0E9A + 1;

            g_pMGSVertices[g_nVertexOffset].x = (float)word_6C0E98;
            g_pMGSVertices[g_nVertexOffset].y = (float)word_6C0E9A;
            g_pMGSVertices[g_nVertexOffset].u = (float)word_6C0E9C;
            g_pMGSVertices[g_nVertexOffset].v = (float)word_6C0E9E;
            g_nVertexOffset++;

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 1;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = 0x7D0; // ?
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            dword_791C58 = 1;
            break;
        }

        case 229:
        {
            StructVertType5* pStructVert = (StructVertType5*)a_pStructVert;

            g_wXOffset = pStructVert->field0 & 0x7FF;
            g_wYOffset = (pStructVert->field0 >> 11) & 0x3FF;

            dword_791C58 = 1;
            break;
        }

        case 255:
        {
            dword_791C58 = 1;
            break;
        }

        default:
            break;
        }

        if (gPrimStructArray[g_nPrimitiveIndex].dwVertexCount != 0)
            g_nPrimitiveIndex++;

        if (dword_791C58 == 0)
        {
            uint8_t* pValue = (uint8_t*)(0x650A5C + dword_791C54);
            dword_791C58 = *pValue;
        }
        if (dword_791C58 == 0)
            dword_791C58 = 1;

        a_nSize -= dword_791C58;
        a_pStructVert = (StructVert*)((intptr_t)a_pStructVert + dword_791C58 * 4);
    }
}

//MSG_FUNC_IMPL(0x410560, ConvertPolys_Hardware);

MGS_VAR(1, 0x6FC868, void*, g_pBackBufferSurface, 0);
MGS_VAR(1, 0x6FC86C, DWORD, g_BackBufferPitch, 0);

MSG_FUNC_NOT_IMPL(0x421C00, void __cdecl(), Render_DrawHardware);
MSG_FUNC_NOT_IMPL(0x51DE0A, void __cdecl(), sub_51DE0A);

//MSG_FUNC_NOT_IMPL(0x4103B0, int __cdecl(), Render_DrawGeneric);
void __cdecl Render_DrawGeneric(StructVert* a_pStructVert)
{
    if (dword_6FC718 == 1)
    {
        dword_6FC718 = 0;
        dword_6FC720 = 1;
        return;
    }

    dword_6FC718 = 0;
    if (dword_6FC720 == 0)
    {
        if (gSoftwareRendering != 0)
        {
            DDSURFACEDESC2 desc;
            memset(&desc, 0, sizeof(DDSURFACEDESC2));
            g_pBackBuffer->Lock(NULL, &desc, 0, 0);
            g_pBackBufferSurface = desc.lpSurface;
            g_BackBufferPitch = desc.lPitch;
        }
        do
        {
            if (a_pStructVert->structType != 0 && dword_6FC768 == 0)
            {
                if (gSoftwareRendering != 0)
                {
                    Render_Software(&a_pStructVert[1], a_pStructVert->structType);
                }
                else
                {
                    ConvertPolys_Hardware(&a_pStructVert[1], a_pStructVert->structType);
                }
            }
            uint32_t nextStructVert = ((uint32_t*)a_pStructVert)[0] & 0x00FFFFFF;
            a_pStructVert = (StructVert*)nextStructVert;
        }
        while ((uint32_t)a_pStructVert != 0xFFFFFF);

        if (gSoftwareRendering != 0)
        {
            g_pBackBuffer->Unlock(0);
        }
        if (gSoftwareRendering == 0)
        {
            Render_DrawHardware();
        }
    }
    if (gSoftwareRendering == 0)
    {
        sub_51DE0A();
    }
}
signed int __cdecl argh();

// 0x00420810
signed int __cdecl DoInitAll()
{
    signed int v1; // ST10_4@1

    //v1 = InitD3d_ProfileGfxHardwareQ_Test();
    v1 = InitD3d_ProfileGfxHardwareQ();
    MessageBox_Error(gHwnd, -1, "Metal Gear Solid PC", MB_OK);
    return v1;
}
MSG_FUNC_IMPL(0x00420810, DoInitAll);

// 0x457B5B
void *__cdecl sub_457B5B()
{
    void *result; // eax@1

    result = memcpy(&word_994320, &word_669AE0, 0x1000u);
    dword_993F44 = (int)&word_994320;
    return result;
}

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
MSG_FUNC_IMPL(0x0044A7B0, Resetgraph);


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
    //OutputDebugStringA(Dest);
    printf(Dest);
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

    err = DetourTransactionCommit();
    if (err != NO_ERROR)
    {
        abort();
    }
}

int __cdecl ClearImage(Rect16 *rect, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b)
{
    DebugLog(".ClearImage((%d,%d,%d,%d),r=%d,g=%d,b=%d)\n", rect->x1, rect->y1, rect->x2, rect->y2, r, g, b);
    return 0;
}

MGS_VAR(1, 0x7227C8, WORD, word_7227C8, 0);

// 0x44EAED
void *__cdecl sub_44EAED()
{
    return memset(&word_7227C8, 0, 0x10u);
}

// 0x40A68D
//MSG_FUNC_NOT_IMPL(0x40A68D, int __cdecl(int, int), sub_40A68D);
void __cdecl LibGvd_SetFnPtr_sub_40A68D(int number, int(__cdecl* fn)(DWORD*))
{
    const int idx = number - 0x61; // Some compiler optimization, just adds this onto g_lib_gvd_stru_6BFEE0 to index the array
    assert(idx < _countof(g_lib_gvd_stru_6BFEE0.field_6BFF0C));
    g_lib_gvd_stru_6BFEE0.field_6BFF0C[idx] = fn;
}
MSG_FUNC_IMPL(0x40A68D, LibGvd_SetFnPtr_sub_40A68D);

struct_8 *__cdecl LibGvd_sub_40A618(int id)
{
    signed int cnt;
    int v4;

    cnt = 128;
    v4 = 128 - id % 128;
    struct_8* result = &g_lib_gvd_stru_6BFEE0.mStruct8_128Array_06BFF80[id % 128];

    while (result->mId & 0xFFFFFF)
    {
        if ((result->mId & 0xFFFFFF) == id) // 3 bytes are id
        {
            return result;
        }
        
        ++result;
        
        if (!--v4)
        {
            result = &g_lib_gvd_stru_6BFEE0.mStruct8_128Array_06BFF80[0];
        }

        if (--cnt <= 0)
        {
            g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08 = 0;
            return 0;
        }
    }
    g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08 = result;
    return 0;
}
MSG_FUNC_IMPL(0x40A618, LibGvd_sub_40A618);

signed int __cdecl LibGvd_sub_40A662(int a1, DWORD* a2)
{
    if (!LibGvd_sub_40A618(a1) && g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08 != 0)
    {
        g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08->mId = a1;
        g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08->field_4 = a2;
        return 0;
    }
    return -1;
}
MSG_FUNC_IMPL(0x40A662, LibGvd_sub_40A662);

/* TODO FIX ME - this seems to halt the game states, can't see why..
int __cdecl LibGvd_sub_40A77F(DWORD* a1, signed int a2, int a3);
MSG_FUNC_IMPL(0x40A77F, LibGvd_sub_40A77F);
int __cdecl LibGvd_sub_40A77F(DWORD* a1, signed int a2, int a3)
{
    int result = 0;
    if (a3)
    {
        if (LibGvd_sub_40A618(a2) || g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08 == 0)
        {
            printf("id conflict\n");
            return -1;
        }

        signed int v6 = a2;
        if (a3 != 1)
        {
            v6 = a2 | 0x1000000;
        }

        g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08->mId = v6;
        g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08->field_4 = a1;
        const auto v7 = g_lib_gvd_stru_6BFEE0.field_6BFF0C[a2 / 0x10000];
        if (v7)
        {
            result = v7(a1);
            if (result <= 0)
            {
                g_lib_gvd_stru_6BFEE0.struct_8_ptr_6BFF08->mId = 0;
                return result;
            }
        }
        return 1;
    }

    auto v3 = g_lib_gvd_stru_6BFEE0.field_6BFF0C[a2 / 0x10000];

    if (!v3)
    {
        return 1;
    }

    result = v3(a1);

    if (result > 0)
    {
        return 1;
    }

    return result;
}
*/

//MSG_FUNC_NOT_IMPL(0x44E1E0, __int16 __cdecl(), sub_44E1E0);
__int16 __cdecl sub_44E1E0()
{
    word_78E7FE = word_78E7FC = -1;
    return -1;
}

//MSG_FUNC_NOT_IMPL(0x0040A347, Actor* __cdecl (Actor*, (void(__cdecl *)(Actor*)), (void(__cdecl *)(Actor*)), char*), Actor_Init);
Actor* __cdecl Actor_Init(Actor* a1, void(__cdecl *update)(Actor*), void(__cdecl *fn2)(Actor*), char *srcFileName)
{
    a1->update = update;
    a1->fnUnknown3 = fn2;
    a1->mNamePtr = srcFileName;
    a1->field_1C = 0;
    a1->field_18 = 0;
    return a1;
}
MSG_FUNC_IMPL(0x0040A347, Actor_Init);

//MSG_FUNC_NOT_IMPL(0x0040B36E, int __cdecl(), GetResidentTop);
int __cdecl GetResidentTop()
{
    int result; // eax@1

    result = gResidentTop_dword_78E960;
    dword_78E964 = gResidentTop_dword_78E960;
    return result;
}

Actor* __cdecl Actor_PushBack(int a_nLvl, Actor* a_pActor, void(__cdecl *fn)(Actor*));

//MSG_FUNC_NOT_IMPL(0x44E12B, void *__cdecl(), sub_44E12B);
void *__cdecl sub_44E12B()
{
    dword_995344 = 0;
    dword_7227A4 = 0;
    dword_7227A0 = 0;
    dword_9942B8 = 0;
    Menu_Related1();
    sub_44EAED();
    sub_457B5B();
    sub_452610();
    LibGvd_SetFnPtr_sub_40A68D(98, sub_44E9D2.Ptr());
    sub_44E1E0();
    Actor_PushBack(1, &g_gamed_722760, 0);
    Actor_Init(&g_gamed_722760, (void(__cdecl*)(Actor*))sub_44E381.Ptr(), 0, "C:\\mgs\\source\\Game\\gamed.c");

    unknown_libname_3();
    sub_44E287();
    sub_44E212();
    word_78E7E8 = (WORD)(dword_78D7B0 + 1);
    dword_995324 = (int)&dword_7919C0;
    GetResidentTop();
    dword_722780 = 0;
    dword_722784 = 0;
    return sub_44E226();
}

//MSG_FUNC_NOT_IMPL(0x0040A1BF, int __cdecl(), Actor_UpdateActors);
int __cdecl Actor_UpdateActors()
{
    int result; // eax@8
    Actor *v1; // [sp+0h] [bp-18h]@5
    Actor *v2; // [sp+4h] [bp-14h]@4
    signed int i; // [sp+10h] [bp-8h]@1
    ActorList *pActor; // [sp+14h] [bp-4h]@1

    pActor = gActors;


    for (i = 9; i > 0; --i)
    {
        if (!(dword_791A0C & pActor->mPause))
        {
            v2 = &pActor->first;
            do
            {
                v1 = v2->pNext;
                auto fn = v2->update;

                // bool isFamasFunc = fn == (void*)0x640CDC;

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

struct PauseKill
{
    WORD mPause;
    WORD mKill;
};

MGS_ARY(1, 0x6507EC, PauseKill, 9, gPauseKills, { { 0, 7 }, { 0, 7 }, { 9, 4 }, { 9, 4 }, { 0xF, 4 }, { 0xF, 4 }, { 0xF, 4 }, { 9, 4 }, { 0, 7 } });

//MSG_FUNC_NOT_IMPL(0x0040A006, void __cdecl(), ActorList_Init);
void __cdecl ActorList_Init()
{
    ActorList* pActor = gActors;

    for (int i = 0; i < 9; i++)
    {
        pActor->first.pPrevious = 0;
        pActor->first.pNext = &pActor->last;
        pActor->last.pPrevious = &pActor->first;
        pActor->last.pNext = 0;

        pActor->first.fnUnknown2 = 0;
        pActor->first.fnUnknown3 = 0;
        pActor->last.fnUnknown2 = 0;
        pActor->last.fnUnknown3 = 0;

        pActor->mPause = gPauseKills[i].mPause;
        pActor->mKill = gPauseKills[i].mKill;

        pActor++;
    }

    dword_791A0C = 0;
}

//MSG_FUNC_NOT_IMPL(0x0040A2AF, Actor* __cdecl(int, Actor*, void(__cdecl *)(Actor*)), Actor_PushBack);
Actor* __cdecl Actor_PushBack(int a_nLvl, Actor* a_pActor, void(__cdecl *fn)(Actor*))
{
    Actor* pLast = &gActors[a_nLvl].last;
    Actor* pLastPrevious = pLast->pPrevious;
    pLast->pPrevious = a_pActor;
    pLastPrevious->pNext = a_pActor;
    a_pActor->pNext = pLast;
    a_pActor->pPrevious = pLastPrevious;
    a_pActor->fnUnknown3 = 0;
    a_pActor->update = 0;
    a_pActor->fnUnknown2 = fn;

    return a_pActor;
}

// 0x00401005
//MSG_FUNC_NOT_IMPL(0x00401005, signed int __cdecl(), Main);
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

        // HACK: Somtimes the game crashes somewhere deep in here, not calling this seems to prevent the game
        // state from progressing.
        // In software rendering mode when gameover it will crash, but this is an existing bug of the game.
        Actor_UpdateActors();
    }
    return result;
}

// 0x00401000
//MSG_FUNC_NOT_IMPL(0x00401000, int __cdecl(), DoMain);
int __cdecl DoMain()
{
    return Main();
}


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


int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    int result; // eax@2
    void(__stdcall *pSetProcessAffinityMask)(HANDLE, signed int); // [sp+8h] [bp-464h]@13
    void(__stdcall *pSetThreadExecutionState)(unsigned int); // [sp+Ch] [bp-460h]@13
    HMODULE hKernel32; // [sp+10h] [bp-45Ch]@12
    char Dest[256]; // [sp+14h] [bp-458h]@11
    struct _MEMORYSTATUS Buffer; // [sp+414h] [bp-58h]@10
    char *v11; // [sp+434h] [bp-38h]@52
    WNDCLASSA WndClass; // [sp+438h] [bp-34h]@27
    char *bRestart; // [sp+464h] [bp-8h]@8
    //int i; // [sp+468h] [bp-4h]@70

    TaskCpp_ForceLink();
    SoundCpp_ForceLink();
    SoundCpp_Debug();
 
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
