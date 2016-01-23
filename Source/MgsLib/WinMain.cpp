#include "stdafx.h"

#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <assert.h>
#include <iostream>
#include <memory>
#include <map>
#include <detours.h>

#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"

#include "logger.hpp"

// No arguments case
void doPrint(std::ostream& out)
{

}

template <typename T>
void doPrint(std::ostream& out, T t)
{
    out << t;
}

template <typename T, typename U, typename... Args>
void doPrint(std::ostream& out, T t, U u, Args... args)
{
    out << t << ',';
    doPrint(out, u, args...);
}

class MgsFunctionBase
{
public:
    MgsFunctionBase() = default;
};

const extern bool gbIsDll;

// TODO: Probably need the function name too
template<DWORD kOldAddr, void* kNewAddr, class ReturnType>
class MgsFunction;

template <DWORD kOldAddr, void* kNewAddr, class ReturnType, class... Args>
class MgsFunction<kOldAddr, kNewAddr, ReturnType(Args...)> : public MgsFunctionBase
{
public:
    using TFuncType = ReturnType(*)(Args...);

    MgsFunction(const char* fnName)
        : mFnName(fnName)
    {
        mRealFuncPtr = (TFuncType)kOldAddr;

        auto it = gFuncMap.find(kOldAddr);
        if (it != std::end(gFuncMap))
        {
            // duplicated function
            abort();
        }
        else
        {
            gFuncMap.insert(std::make_pair(kOldAddr, this));
        }

        std::cout << "old addr " << kOldAddr << " new addr " << kNewAddr << std::endl;

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        if (kNewAddr)
        {
            // Hook oldAddr to point to newAddr
            DetourAttach(&(PVOID&)mRealFuncPtr, Static_Hook_Impl);
        }
        else
        {
            // point oldAddr to Static_Hook_Impl
            DetourAttach(&(PVOID&)mRealFuncPtr, (TFuncType)kNewAddr);
        }
        const auto error = DetourTransactionCommit();
        if (error != NO_ERROR)
        {
            abort();
        }
    }

    static ReturnType Static_Hook_Impl(Args ... args)
    {
        auto it = gFuncMap.find(kOldAddr);
        if (it == std::end(gFuncMap))
        {
            // Impossible situation
        }

        
        MgsFunctionBase* baseFunc = it->second;
        // dont know if this will work or just blow up..
        return static_cast<MgsFunction*>(baseFunc)->operator()(args...);
    }

    ~MgsFunction()
    {
        auto it = gFuncMap.find(kOldAddr);
        if (it != std::end(gFuncMap))
        {
            gFuncMap.erase(it);
        }
    }

    ReturnType operator()(Args ... args)
    {
        doPrint(std::cout, args...);

        if (kNewAddr)
        {
            // Call "newAddr" since we've replaced the function completely
            return reinterpret_cast<TFuncType>(kNewAddr)(args...);
        }
        else
        {
            // Call "mRealFuncPtr" here so that we are calling the "real" function

            // If not running within the game then we can't call real so just return
            // a default R and log params
            return mRealFuncPtr(args...);
        }
    }

private:
    TFuncType mRealFuncPtr = nullptr;
    const char* mFnName = nullptr;
};

std::map<DWORD, MgsFunctionBase*> gFuncMap;

struct actor_related_struct
{
    actor_related_struct* field_0;
    actor_related_struct* actor_struct_ptr1;
    void(__cdecl *fn_unknown)(actor_related_struct *);
    void(__cdecl *fnUnknown3)(actor_related_struct *);
    void(__cdecl *fnUnknown2)(actor_related_struct *);
    char* mNamePtr;
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

struct Rect16
{
    WORD x1, y1, x2, y2;
};
static_assert(sizeof(Rect16) == 8, "Rect16 should be 8");

DWORD& dword_995344 = *(DWORD*)0x995344;
DWORD& dword_7227A4 = *(DWORD*)0x7227A4;
DWORD& dword_9942B8 = *(DWORD*)0x9942B8;
DWORD& dword_78D7B0 = *(DWORD*)0x78D7B0;
WORD& word_78E7E8 = *(WORD*)0x78E7E8;
DWORD& dword_995324 = *(DWORD*)0x995324;
DWORD& dword_7919C0 = *(DWORD*)0x7919C0;

actor_related_struct& stru_722760 = *(actor_related_struct*)0x722760;
actor_related_struct* gActors = (actor_related_struct*)0x006BFC78; // Array of 9 items, TODO: Check correct

WORD& word_78E7FC = *(WORD*)0x78E7FC;
WORD& word_78E7FE = *(WORD*)0x78E7FE;
DWORD& gResidentTop_dword_78E960 = *(DWORD*)0x78E960;
DWORD& dword_78E964 = *(DWORD*)0x78E964;
DWORD& dword_791A0C = *(DWORD*)0x791A0C;
DWORD& dword_9942A0 = *(DWORD*)0x9942A0;
DWORD& gExitMainGameLoop = *(DWORD*)0x0073492C;
WORD& word_994320 = *(WORD*)0x994320;
WORD& word_669AE0 = *(WORD*)0x669AE0;
DWORD& dword_993F44 = *(DWORD*)0x993F44;

char*& gCmdLine = *((char**)0x0071D16C);
DWORD& dword_787774 = *((DWORD*)0x787774);
DWORD& dword_787778 = *((DWORD*)0x787778);
WORD& dword_78E7E4 = *(WORD*)0x78E7E4;
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


#define VAR(type,name,addr) type& name = *(type*)addr;
VAR(DWORD, dword_77C934, 0x77C934);
VAR(BYTE, byte_9AD8A5, 0x9AD8A5);
VAR(BYTE, byte_9AD8A7, 0x9AD8A7);
VAR(BYTE, byte_9AD8A6, 0x9AD8A6);
VAR(BYTE, byte_9AD8A8, 0x9AD8A8);
VAR(BYTE, byte_9AD8DA, 0x9AD8DA);
VAR(BYTE, byte_9AD8C1, 0x9AD8C1);
VAR(DWORD, dword_73490C, 0x73490C);
VAR(DWORD, dword_734908, 0x734908);

int* gKeys = (int*)0x009AD9A0;
BYTE* byte_9AD880 = (BYTE*)0x9AD880;

VAR(DWORD, gvirtualKeyRepeatCount, 0x009AD980);
VAR(DWORD, gVirtualKeyCode, 0x009AD6B0);

VAR(DWORD, gAltPressed, 0x009AD892);
VAR(DWORD, dword_71D194, 0x71D194);
VAR(DWORD, gF10Pressed, 0x009AD8F9);
VAR(DWORD, dword_734900, 0x734900);
VAR(DWORD, dword_734904, 0x734904);
VAR(BYTE, byte_9AD988, 0x9AD988);
VAR(DWORD, dword_688CDC, 0x688CDC);
VAR(DWORD, dword_71D17C, 0x71D17C);
VAR(DWORD, dword_688CD0, 0x688CD0);
VAR(DWORD, dword_688CD4, 0x688CD4);
VAR(DWORD, dword_688CD8, 0x688CD8);
VAR(DWORD, dword_791DE4, 0x791DE4);
VAR(DWORD, dword_717354, 0x717354);
VAR(DWORD, dword_717348, 0x717348);
VAR(BYTE, byte_9AD888, 0x9AD888);
VAR(DWORD, dword_733E34, 0x733E34);
VAR(DWORD, dword_721E78, 0x721E78);
VAR(DWORD, dword_7348FC, 0x7348FC);
VAR(DWORD, dword_650D4C, 0x650D4C);
VAR(char*, gDest, 0x0078E7C0);



// 0x00421680
signed __int64 __cdecl FpsTimerSetupQ()
{
    typedef decltype(&FpsTimerSetupQ) fn;
    return ((fn)(0x00421680))();
}

// 0x0040A0D4
int __cdecl Actor_DumpActorSystem()
{
    int result; // eax@1
    int v1; // [sp+0h] [bp-18h]@6
    actor_related_struct *pNextActor; // [sp+4h] [bp-14h]@4
    actor_related_struct *pActorCopy; // [sp+8h] [bp-10h]@3
    signed int i; // [sp+10h] [bp-8h]@1
    actor_related_struct *pActor; // [sp+14h] [bp-4h]@1

    pActor = gActors;
    result = printf("--DumpActorSystem--\n");
    for (i = 0; i < 9; ++i)
    {
        printf("Lv %d Pause %d Kill %d\n", i, pActor->mPause, pActor->mKill);
        pActorCopy = pActor;
        do
        {
            pNextActor = pActorCopy->actor_struct_ptr1;
            if (pActorCopy->fn_unknown)
            {
                if (pActorCopy->field_1C <= 0)
                    v1 = 0;
                else
                    v1 = 100 * pActorCopy->field_18 / pActorCopy->field_1C;
                printf("Lv%d %04d.%02d %08X %s\n", i, v1 / 100, v1 % 100, pActorCopy->fn_unknown, pActorCopy->mNamePtr);
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

// 0x5202FE
DWORD __cdecl sub_5202FE(float a1, float a2, float a3, float a4)
{
    typedef decltype(&sub_5202FE) fn;
    return ((fn)(0x5202FE))(a1, a2, a3, a4);
}

// 0x43BCF0 joystick related
int __cdecl sub_43BCF0()
{
    typedef decltype(&sub_43BCF0) fn;
    return ((fn)(0x43BCF0))();
}

// 0x521210
void __cdecl sub_521210()
{
    typedef decltype(&sub_521210) fn;
    return ((fn)(0x521210))();
}

// 0x452E6E
int /*__usercall*/ sub_452E6E/*<eax>*/(/*<esi>*/)
{
    typedef decltype(&sub_452E6E) fn;
    return ((fn)(0x452E6E))();
}

// 0x0043ACC4 
int __cdecl WmPaint_Handler(HDC hdcDest)
{
    typedef decltype(&WmPaint_Handler) fn;
    return ((fn)(0x0043ACC4))(hdcDest);
}

// 0x0051C2D3
signed int __stdcall MainWindowProc(HWND hWnd, UINT Msg, UINT wParam, LPARAM lParam)
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
            sub_5202FE(0.02999999932944775, 0.0, 0.0, 0.0);
        if (byte_9AD8DA)
            sub_5202FE(-0.02999999932944775, 0.0, 0.0, 0.0);
        v4 = (double)dword_73490C / 1024.0;
        v5 = (double)dword_734908 / 1024.0;
        sub_5202FE(0.0, v5, v4, 0.0);
        dword_734908 = 9 * dword_734908 / 10;
        dword_73490C = 9 * dword_73490C / 10;
    }
    if (Msg > 0x102)
    {
        switch (Msg)
        {
        case 0x104u:
            gKeys[wParam] = lParam;
            gvirtualKeyRepeatCount = lParam;
            gVirtualKeyCode = wParam;
            if (wParam < 0x100)
                byte_9AD880[wParam] = 1;
            if ((unsigned __int16)gVirtualKeyCode == 18)
            {
                gAltPressed = 1;
                return 0;
            }
            if ((unsigned __int16)gVirtualKeyCode == 121)
            {
                dword_71D194 = 1;
                gF10Pressed = 1;
                return 0;
            }
            break;
        case 0x105u:                              // WM_SYSKEYUP
            gvirtualKeyRepeatCount = lParam;
            gVirtualKeyCode = wParam;
            if (wParam < 0x100)
                byte_9AD880[wParam] = 0;
            if ((unsigned __int16)gVirtualKeyCode == 18)// VK_MENU
            {
                gAltPressed = 0;
                return 0;
            }
            if ((unsigned __int16)gVirtualKeyCode == 121)// VK_F10
            {
                gF10Pressed = 0;
                Actor_DumpActorSystem();
                return 0;
            }
            break;
        case 0x200u:
            if (dword_77C934)
            {
                dword_734908 = (unsigned __int16)lParam - dword_734900;
                dword_73490C = (unsigned __int16)((unsigned int)lParam >> 16) - dword_734904;
                dword_734900 = (unsigned __int16)lParam;
                dword_734904 = (unsigned int)lParam >> 16;
            }
            break;
        case 0x218u:
            return 1112363332;
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
    if (Msg == 258)
    {
        byte_9AD988 = wParam;
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
    if (Msg == 6)
    {
        if ((WORD)wParam)
        {
            printf("$jim - WM_ACTIVATE (active)\n");
            dword_688CDC = 1;
        }
        else
        {
            printf("$jim - WM_ACTIVATE (inactive)\n");
            dword_688CDC = 0;
        }
        sub_43BCF0();
        FpsTimerSetupQ();
        result = 1;
    }
    else
    {
        if (Msg == 15)
        {
            printf("$jim - WM_PAINT\n");
            if (dword_71D17C)
            {
                WmPaint_Handler((HDC)wParam);
                return 0;
            }
            return DefWindowProcA(hWnd, Msg, wParam, lParam);
        }
        if (Msg == 16)
        {
            PostQuitMessage(0);
            return 0;
        }
        if (Msg != 256)
        {
            if (Msg != 257)
                return DefWindowProcA(hWnd, Msg, wParam, lParam);
        LABEL_108:
            if (wParam < 0x100)
                byte_9AD880[wParam] = Msg == 256;
            return DefWindowProcA(hWnd, Msg, wParam, lParam);
        }
        v7 = MapVirtualKeyA(wParam, 0);
        if (v7)
        {
            GetKeyNameTextA(v7 << 16, String, 256);
            gKeys[wParam] = lParam;
        }
        gKeys[wParam] = lParam;
        if (wParam > 0x76)
        {
            switch (wParam)
            {
            case 0x77u:
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
            case 0x78u:
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
            case 0x7Au:
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
                if (wParam != 123)
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
            if (wParam == 118)
            {
                if (gCheatsEnabled)
                {
                    stru_722760.field_3C = 0;
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
                if (wParam == 27)
                {
                    dword_791DE4 = 1;
                    if (stru_722760.field_3C != 0x20000000 || !strstr(gDest, "s19a"))
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
                                            dword_717354 = 1;
                                    }
                                }
                            }
                        }
                    }
                    goto LABEL_108;
                }
                switch (wParam)
                {
                case 0x71u:
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
                case 0x73u:
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
                case 0x74u:
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
                    if (wParam != 117)
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

DWORD* dword_664EC4 = (DWORD*)0x664EC4;
DWORD* dword_664EC0 = (DWORD*)0x664EC0;
char* aHoldDownTheInv = (char*)0x00662EC0;

// 0x0043CBD9
int __cdecl MessageBox_Sometimes(HWND hWnd, int a2, LPCSTR lpCaption, UINT uType)
{
    int result; // eax@2
    signed int i; // [sp+0h] [bp-8h]@5
    signed int j; // [sp+0h] [bp-8h]@11
    signed int v7; // [sp+4h] [bp-4h]@5

    if (a2 <= 0 || (result = a2, dword_664EC4[2 * a2]))
    {
        if (a2 == -1)
        {
            v7 = -1;
            for (i = 0; i < 8; ++i)
            {
                if (dword_664EC4[2 * i])
                {
                    if (dword_664EC0[2 * i])
                        v7 = i;
                }
                result = i + 1;
            }
            for (j = 0; j < 8; ++j)
            {
                if (dword_664EC4[2 * j])
                {
                    if (dword_664EC0[2 * j])
                    {
                        if (dword_664EC4[2 * j] < (unsigned int)dword_664EC4[2 * v7])
                        {
                            v7 = j;
                            dword_664EC0[2 * j] = 0;
                        }
                    }
                }
                result = j + 1;
            }
            if (v7 >= 0)
                result = MessageBoxA(hWnd, &aHoldDownTheInv[1024 * v7], lpCaption, uType); // Hold down the Inventory and Weapon item buttons
        }
        else
        {
            ++dword_664EC0[2 * a2];
            result = 2;
        }
    }
    else
    {
        result = MessageBoxA(hWnd, &aHoldDownTheInv[1024 * a2], lpCaption, uType);
    }
    return result;
}

// 0x42B6A0
signed int __stdcall DirectDrawCreateExMGS(GUID* lpGuid, LPVOID* lplpDD, const IID *const iid, IUnknown* punkOuter)
{
    typedef decltype(&DirectDrawCreateExMGS) fn;
    return ((fn)(0x42B6A0))(lpGuid, lplpDD, iid, punkOuter);
}

VAR(IID, IID_IDirectDraw7_MGS, 0x64BDA8);
VAR(GUID, IID_IDirect3D7_MGS, 0x64BB98);
VAR(GUID, IID_IDirectDrawGammaControl_MGS, 0x64BCA8);
VAR(IDirectDraw7*, pDirectDraw, 0x6FC730);
VAR(IDirect3D7*, pDirect3D, 0x6FC748);
VAR(IDirectDrawGammaControl*, pGammaControl, 0x6C0EF8);
VAR(DWORD, dwDisplayWidth, 0x6DF214);
VAR(DWORD, dwDisplayHeight, 0x6DF1FC);
VAR(LPDIRECTDRAWSURFACE7, pPrimarySurface, 0x6FC734);
VAR(LPDIRECTDRAWCLIPPER, pClipper, 0x6FC750);
VAR(LPDIRECTDRAWSURFACE7, pBackBuffer, 0x6FC738);
VAR(LPDIRECT3DDEVICE7, pDirect3DDevice, 0x6FC74C);
VAR(LPDIRECTDRAWSURFACE7, pDDSurface, 0x6FC740);

VAR(FILE*, gFile, 0x006DEF78);
VAR(FILE*, gLogFile, 0x71D414);

VAR(DWORD, dword_651CF8, 0x651CF8);
VAR(DWORD, dword_716F5C, 0x716F5C);
VAR(DWORD, dword_716F78, 0x716F78);
VAR(DWORD, dword_77C60C, 0x77C60C);
VAR(DWORD, dword_77C608, 0x77C608);
VAR(DWORD*, dword_776B94, 0x776B94);
VAR(DWORD*, dword_776B90, 0x776B90);
VAR(DWORD, dword_716F74, 0x716F74);
VAR(DWORD, gXSize_dword_6DF214, 0x6DF214);
VAR(DWORD, dword_650D2C, 0x650D2C);
VAR(DWORD*, dword_6C0EFC, 0x6C0EFC);
VAR(void*, dword_6FC780, 0x6FC780);
VAR(DWORD*, dword_6FC728, 0x6FC728);
VAR(void*, dword_6DEF7C, 0x6DEF7C);
VAR(void*, dword_6DEF90, 0x6DEF90);
VAR(void*, dword_6FC72C, 0x6FC72C);
VAR(DWORD*, dword_6C0F00, 0x6C0F00);
VAR(DWORD, dword_6FC798, 0x6FC798);
VAR(DWORD, dword_6FC7C0, 0x6FC7C0);
VAR(DWORD*, dword_6C0F20, 0x6C0F20);
VAR(DWORD*, dword_6C0F24, 0x6C0F24);
VAR(DWORD, dword_716F6C, 0x716F6C);
VAR(DWORD, dword_6FC7C4, 0x6FC7C4);
VAR(DWORD, dword_651D94, 0x651D94);
VAR(DWORD, dword_6FC79C, 0x6FC79C);
VAR(DWORD, dword_716F60, 0x716F60);
VAR(char*, unk_776B68, 0x776B68);
VAR(DWORD, gYSize, 0x006DF1FC);
VAR(char*, unk_6C0778, 0x6C0778);


int __cdecl InitDirectInput(HWND hWnd);

// 0x0042D69E
int __cdecl DoDirectInputInit()
{
    int result; // eax@1

    dword_717348 = 0;
    result = InitDirectInput(gHwnd);
    if (result < 0)
        result = printf("$jim failed to init direct input");
    return result;
}

// 0x0051F22F
int __cdecl jim_enumerate_devices()
{
    typedef decltype(&jim_enumerate_devices) fn;
    return ((fn)(0x0051F22F))();
}

// 0x004331D4
signed int __cdecl ParseMsgCfg()
{
    typedef decltype(&ParseMsgCfg) fn;
    return ((fn)(0x004331D4))();
}

// 0x433801
signed int __cdecl sub_433801()
{
    typedef decltype(&sub_433801) fn;
    return ((fn)(0x433801))();
}

// 0x41EC40
signed int __cdecl sub_41EC40()
{
    typedef decltype(&sub_41EC40) fn;
    return ((fn)(0x41EC40))();
}

// 0x43C850
unsigned int __cdecl sub_43C850()
{
    typedef decltype(&sub_43C850) fn;
    return ((fn)(0x43C850))();
}

// 0x431C63
int __cdecl sub_431C63()
{
    typedef decltype(&sub_431C63) fn;
    return ((fn)(0x431C63))();
}

// 0x51F1E1
int __cdecl sub_51F1E1(GUID** a1, int* a2)
{
    typedef decltype(&sub_51F1E1) fn;
    return ((fn)(0x51F1E1))(a1, a2);
}

// 0x0042A630
void __cdecl _cfltcvt_init()
{
    typedef decltype(&_cfltcvt_init) fn;
    ((fn)(0x0042A630))();
}

// 0x0041EA60
signed int __cdecl MissionLog_Related2()
{
    typedef decltype(&MissionLog_Related2) fn;
    return ((fn)(0x0041EA60))();
}

// 0x00422D40
char *__cdecl PrintDDError(char *a1, signed int a2)
{
    typedef decltype(&PrintDDError) fn;
    return ((fn)(0x00422D40))(a1, a2);
}

// 0x41C820
void __cdecl sub_41C820(float a1)
{
    typedef decltype(&sub_41C820) fn;
    return ((fn)(0x41C820))(a1);
}

// 0x41CA80
signed int __cdecl sub_41CA80()
{
    typedef decltype(&sub_41CA80) fn;
    return ((fn)(0x41CA80))();
}

// 0x41CC30
__int16 __cdecl sub_41CC30()
{
    typedef decltype(&sub_41CC30) fn;
    return ((fn)(0x41CC30))();
}

// 0x41CD70
int __cdecl sub_41CD70()
{
    typedef decltype(&sub_41CD70) fn;
    return ((fn)(0x41CD70))();
}

// 0x41CE20
bool __cdecl sub_41CE20()
{
    typedef decltype(&sub_41CE20) fn;
    return ((fn)(0x41CE20))();
}

// 0x41D1D0
signed int __cdecl sub_41D1D0()
{
    typedef decltype(&sub_41D1D0) fn;
    return ((fn)(0x41D1D0))();
}

// 0x41D420
signed int __cdecl sub_41D420()
{
    typedef decltype(&sub_41D420) fn;
    return ((fn)(0x41D420))();
}

// 0x41E3C0
signed int __cdecl sub_41E3C0()
{
    typedef decltype(&sub_41E3C0) fn;
    return ((fn)(0x41E3C0))();
}

// 0x41E730
bool __cdecl sub_41E730()
{
    typedef decltype(&sub_41E730) fn;
    return ((fn)(0x41E730))();
}

// 0x41E990
bool __cdecl sub_41E990()
{
    typedef decltype(&sub_41E990) fn;
    return ((fn)(0x41E990))();
}

// 0x00422A90
int __cdecl Render_Unknown1 (signed int a1, int a2)
{
    typedef decltype(&Render_Unknown1) fn;
    return ((fn)(0x00422A90))(a1, a2);
}

// 0x422BC0
int __cdecl sub_422BC0(unsigned int a1, signed int a2, int a3)
{
    typedef decltype(&sub_422BC0) fn;
    return ((fn)(0x422BC0))(a1, a2, a3);
}

HFONT& gFont = *(HFONT*)0x006FC7E8;

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

// 0x431865
signed int __cdecl MakeFonts()
{
    typedef decltype(&MakeFonts) fn;
    return ((fn)(0x431865))();
}


// 0x0041ECB0
signed int __cdecl InitD3d_ProfileGfxHardwareQ()
{
    signed int result; // eax@41
    int v1; // edx@115
    float v2; // STB4_4@163
    DDPIXELFORMAT pixelFormat; // [sp+C8h] [bp-388h]@142
    DDSURFACEDESC2 dxSurfaceDesc3; // [sp+E8h] [bp-368h]@142
    DDSCAPS2 dxCaps1; // [sp+164h] [bp-2ECh]@104
    int v19; // [sp+168h] [bp-2E8h]@104
    int v20; // [sp+16Ch] [bp-2E4h]@104
    int v21; // [sp+170h] [bp-2E0h]@104
    int v22; // [sp+174h] [bp-2DCh]@76
    int v23; // [sp+178h] [bp-2D8h]@1
    int v24; // [sp+17Ch] [bp-2D4h]@1
    int v25; // [sp+180h] [bp-2D0h]@1
    int v26; // [sp+184h] [bp-2CCh]@1
    int v27; // [sp+188h] [bp-2C8h]@1
    int v28; // [sp+18Ch] [bp-2C4h]@1
    int v29; // [sp+190h] [bp-2C0h]@1
    int v30; // [sp+194h] [bp-2BCh]@1
    DDCAPS_DX7 dxCaps; // [sp+198h] [bp-2B8h]@114
    int v32; // [sp+1A0h] [bp-2B0h]@115
    int v33; // [sp+314h] [bp-13Ch]@1
    int v34; // [sp+318h] [bp-138h]@3
    unsigned int i; // [sp+320h] [bp-130h]@34
    DDSURFACEDESC2 dxSurfaceDesc; // [sp+328h] [bp-128h]@86
    int v37; // [sp+32Ch] [bp-124h]@97
    int v38; // [sp+330h] [bp-120h]@97
    int v39; // [sp+334h] [bp-11Ch]@97
    unsigned int v40; // [sp+37Ch] [bp-D4h]@119
    int v41; // [sp+390h] [bp-C0h]@91
    int v42; // [sp+3A4h] [bp-ACh]@30
    DDSURFACEDESC2 dxSurfaceDesc2; // [sp+3B0h] [bp-A0h]@70 = DDSURFACEDESC2 (sizeof = 0x7C)
    int v44; // [sp+3B4h] [bp-9Ch]@72
    int v45; // [sp+3C4h] [bp-8Ch]@73
    int v46; // [sp+418h] [bp-38h]@72
    int v47; // [sp+42Ch] [bp-24h]@163
    int v48; // [sp+430h] [bp-20h]@163
    int v49; // [sp+434h] [bp-1Ch]@163
    int v50; // [sp+438h] [bp-18h]@163
    int v51; // [sp+43Ch] [bp-14h]@163
    int v52; // [sp+440h] [bp-10h]@163
    int v53; // [sp+444h] [bp-Ch]@40
    GUID *lpGuid; // [sp+448h] [bp-8h]@1
    int v55; // [sp+44Ch] [bp-4h]@1

    lpGuid = 0;
    v33 = 0;
    v23 = 32;
    v24 = 65;
    v25 = 0;
    v26 = 16;
    v27 = 31744;
    v28 = 992;
    v29 = 31;
    v30 = 32768;
    gFile = fopen("profile.log", "w");
    fputs("InitAll {\n", gFile);
    fflush(gFile);
    gLogFile = gFile;
    DoDirectInputInit();
    fputs("jim_enumerate_devices()\n", gFile);
    fflush(gFile);
    v55 = jim_enumerate_devices();
    if (!v55)
    {
        gSoftwareRendering = 1;
        dword_716F5C = 1065353216; // TODO: float
        gXRes = 1065353216; // TODO: float
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
    if (ParseMsgCfg())
    {
        dword_651CF8 = 0;
        fputs(" . done\n", gFile);
        fflush(gFile);
        if (dword_716F78 == 1)
        {
            if (gNoCrashCheck)
            {
                MessageBoxA(
                    0,
                    "Game crashed during previous initialization, game starting software rendering mode...",
                    "Metal Gear Solid",
                    0);
                gSoftwareRendering = 1;
                dword_716F5C = 1065353216; // TODO: This is actually a float type being set to 1.0!
                gXRes = 1065353216; // TODO: float
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
    if (gNoCrashCheck)
    {
        dword_716F78 = 1;
        sub_433801();
    }
    if (!dword_651CF8)
    {
        if (v55 == -1)
        {
            MessageBoxA(
                0,
                "Your video configuration has been updated - your system will be re-profiled\n",
                "Metal Gear Solid (PC)",
                0);
            v55 = 0;
            if (sub_41EC40())
            {
                gSoftwareRendering = 0;
                gXRes = 1073741824; // TODO: float
            }
            else
            {
                gSoftwareRendering = 1;
                lpGuid = 0;
                gXRes = 1065353216; // TODO: float
                dword_716F5C = 1065353216;
                MessageBox_Sometimes(0, 5, "Metal Gear Solid PC", 0);
            }
            gWindowedMode = 0;
            sub_433801();
            v34 = gWindowedMode;
            if (ParseMsgCfg())
                dword_651CF8 = 0;
            else
                dword_651CF8 = 1;
        }
    }
    sub_43C850();
    if (dword_651CF8)
    {
        dword_651D98 = 100;
        dword_716F68 = 100;
        fputs("Executing system profiling sequence.\n", gFile);
        fflush(gFile);
        fputs("Choosing default 3D-accelerator\n", gFile);
        fflush(gFile);
        if (sub_41EC40())
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
            fprintf(gFile, " getting selected driver No %d from %d available\n", dword_77C60C + 1, dword_77C608);
            gXRes = 1073741824; // TODO: float
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
            dword_716F5C = 1065353216; // TODO: float
            gXRes = 1065353216; // TODO: float
            lpGuid = 0;
            MessageBox_Sometimes(0, 5, "Metal Gear Solid PC", 0);
        }
        sub_431C63();
    }
    else
    {
        dword_77C60C = dword_716F74;
    }
    if (v34)
    {
        dword_77C60C = 0;
        gXRes = 1073741824; // TODO: Float
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
    for (i = 0; i < dword_77C608; ++i)
    {
        fprintf(gFile, "pDriverGUID %x, pDeviceGUID %x\n", dword_776B94[290 * i], dword_776B90[290 * i]);
        fprintf(gFile, "D3DDevice description : %s", (char *)&unk_776B68 + 1160 * i);
        if (dword_77C60C == i)
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
    while (1)
    {
        gXSize_dword_6DF214 = (signed __int64)(320.0 * gXRes);
        gYSize = (signed __int64)(240.0 * gXRes);
        fputs("Creating DirectDraw7\n", gFile);
        fflush(gFile);
        //v53 = DirectDrawCreateEx(lpGuid, &lpDD, &iid, 0);
        v53 = DirectDrawCreateExMGS(lpGuid, (LPVOID*)&pDirectDraw, &IID_IDirectDraw7_MGS, 0);
        if (v53 < 0)
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
            pDirectDraw->QueryInterface(IID_IDirect3D7_MGS, (LPVOID*)&pDirect3D);
            if (v53 < 0)
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                gSoftwareRendering = 1;
                gXRes = 1065353216; // TODO: Float
                gXSize_dword_6DF214 = (signed __int64)(320.0 * gXRes);
                gYSize = (signed __int64)(240.0 * gXRes);
                MessageBox_Sometimes(0, 4, "Metal Gear Solid PC", 0);
            }
            fputs(" . done\n", gFile);
            fflush(gFile);
        }
        if (gSoftwareRendering)
        {
            if (gXRes >= 1.1)
            {
                gXRes = 1073741824; // TODO: Float
                dword_650D2C = 17;
            }
            else
            {
                gXRes = 1065353216; // TODO: Float
                dword_650D2C = 16;
            }
        }
        if (pPrimarySurface)
        {
            pPrimarySurface->Release();
            if (v53)
                PrintDDError("Can't relaese primary surf", v53);
            pPrimarySurface = 0;
        }
        if (pBackBuffer)
        {
            pBackBuffer->Release();
            if (v53)
                PrintDDError("Can't release render surf", v53);
            pBackBuffer = 0;
        }
        if (pClipper)
        {
            pClipper->Release();
            if (v53)
                PrintDDError("Can't release clipper", v53);
            pClipper = 0;
        }
        pPrimarySurface = 0;
        pBackBuffer = 0;
        pBackBuffer = 0;
        fputs("Setting cooperative level...\n", gFile);
        fflush(gFile);
        if (v42)
        {
            fputs(" (windowed) \n", gFile);
            fflush(gFile);
            v53 = pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_FPUPRESERVE | DDSCL_MULTITHREADED | DDSCL_NORMAL);
        }
        else
        {
            fputs(" (full-screen) \n", gFile);
            fflush(gFile);
            v53 = pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_FPUPRESERVE | DDSCL_MULTITHREADED | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
        }
        if (v53 < 0)
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            return 0;
        }
        fputs(" . done\n", gFile);
        fflush(gFile);
        if (!v42)
        {
            v53 = pDirectDraw->SetDisplayMode(dwDisplayWidth, dwDisplayHeight, 0x10, 0, 0);
            fprintf(gLogFile, "SetDisplayMode( %d, %d )\n", gXSize_dword_6DF214, gYSize);
            if (v53 < 0)
                return 0;
        }
        memset(&dxSurfaceDesc2, 0, 124);
        dxSurfaceDesc2.dwSize = 124;
        if (v42 || gSoftwareRendering)
        {
            v44 = 1;
            v46 = 512;
        }
        else
        {
            v44 = 33;
            v46 = 8728;
            v45 = 1;
        }
        fputs("Creating primary surface...\n", gFile);
        fflush(gFile);
        v53 = pDirectDraw->CreateSurface(&dxSurfaceDesc2, &pPrimarySurface, 0);
        if (v53 < 0)
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            return 0;
        }
        fputs(" . done\n", gFile);
        fflush(gFile);
        v22 = 0;
        MakeFonts();
        if (dword_651CF8)
        {
            sub_41CD70();
            Sleep(0x7D0u);
        }
        if (gWindowedMode)
        {
            fputs("Creating clipper...\n", gFile);
            fflush(gFile);
            pDirectDraw->CreateClipper(0, &pClipper, 0);
            if (v53)
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                PrintDDError("Can't create clipper", v53);
                return 0;
            }
            v53 = pClipper->SetHWnd(0, gHwnd);
            if (v53)
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                PrintDDError("Can't obtain clipper zone", v53);
                return 0;
            }
            pPrimarySurface->SetClipper(pClipper);
            if (v53)
            {
                fputs(" . fail\n", gFile);
                fflush(gFile);
                PrintDDError("Can't attach clipper", v53);
                return 0;
            }
            pClipper->Release();
            fputs(" . done\n", gFile);
            fflush(gFile);
        }
        memset(&dxSurfaceDesc, 0, 124);
        dxSurfaceDesc.dwSize = 124;
        if (gSoftwareRendering)
        {
            if (dword_651CF8 || dword_716F6C && dword_716F6C != 1)
            {
                fputs("Testing software render speed to system and to video surface\n", gFile);
                fflush(gFile);
                if (sub_41CE20())
                {
                    fputs(" . rendering to video surface is faster\n", gFile);
                    fflush(gFile);
                    dword_716F6C = 1;
                    v41 = 16448;
                }
                else
                {
                    fputs(" . rendering to system memory surface is faster\n", gFile);
                    fflush(gFile);
                    dword_716F6C = 0;
                    v41 = 2112;
                }
            }
            else
            {
                if (dword_716F6C)
                    v41 = 16448;
                else
                    v41 = 2112;
            }
            v37 = 7;
            v39 = gXSize_dword_6DF214;
            v38 = gYSize;
            fputs("Creating back buffer for software rendering...\n", gFile);
            fflush(gFile);
            v53 = pDirectDraw->CreateSurface(&dxSurfaceDesc, &pBackBuffer, 0);
            if (v53 < 0)
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
                v37 = 7;
                v41 = 8256;
                v39 = gXSize_dword_6DF214;
                v38 = gYSize;
                fputs("Creating back buffer for windowed mode...\n", gFile);
                fflush(gFile);
                v53 = pDirectDraw->CreateSurface(&dxSurfaceDesc, &pBackBuffer, 0);
                if (v53 < 0)
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
                dxCaps1.dwCaps = 4;
                v19 = 0;
                v20 = 0;
                v21 = 0;
                fputs("Getting back buffer from pPrim chain...\n", gFile);
                fflush(gFile);
                pPrimarySurface->GetAttachedSurface(&dxCaps1, &pBackBuffer);
                if (v53 < 0)
                {
                    fputs(" . fail\n", gFile);
                    fflush(gFile);
                    return v53;
                }
                fputs(" . done\n", gFile);
                fflush(gFile);
            }
        }
        fputs("Restoring surfaces...\n", gFile);
        fflush(gFile);
        sub_41CC30();
        if (v53)
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            PrintDDError("Restoring caput", v53);
        }
        else
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
        }
        fputs("Querying gamma interface...\n", gFile);
        fflush(gFile);
        pPrimarySurface->QueryInterface(IID_IDirectDrawGammaControl_MGS, (LPVOID*)&pGammaControl);
        if (v53)
        {
            fputs(" . fail\n", gFile);
            fflush(gFile);
            PrintDDError("Can't get GammaControl interface", v53);
            dword_6FC7C4 = 0;
        }
        else
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
            dword_6FC7C4 = 1;
        }
        if (pDirectDraw)
        {
            memset(&dxCaps, 0, 380);
            dxCaps.dwSize = 380;
            pDirectDraw->GetCaps(&dxCaps, 0);
            if (v53 || (v1 = v32, !(v1 & 0x20000)))
                dword_6FC7C4 = 0;
        }
        dword_6FC79C = sub_41D1D0();
        fprintf(gFile, "565 mode = %i\n", dword_6FC79C);
        if (gSoftwareRendering)
            break;
        dxSurfaceDesc.dwSize = 124;
        pDirectDraw->GetDisplayMode(&dxSurfaceDesc);
        if (v40 <= 8)
        {
            fputs("Can't render to a palettized surface, exiting.\n", gFile);
            fflush(gFile);
            return 0;
        }
        fputs("Screen mode is ok\n", gFile);
        fflush(gFile);
        fputs("Creating device...\n", gFile);
        fflush(gFile);

        v53 = pDirect3D->CreateDevice(*((GUID*)(&v33)), pBackBuffer, &pDirect3DDevice);
        if (v53 >= 0)
        {
            fputs(" . done\n", gFile);
            fflush(gFile);
            Render_Unknown1(22, 1);
            Render_Unknown1(26, 0);
            if (!gSoftwareRendering)
            {
                assert(sizeof(DDPIXELFORMAT) == 32);
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

                v53 = pDirectDraw->CreateSurface(&dxSurfaceDesc3, &pDDSurface, 0);
                if (v53)
                {
                    pDDSurface = 0;
                }
                else
                {
                    if (!sub_41E990())
                    {
                        pDDSurface->Release();
                        pDDSurface = 0;
                    }
                }
            }
            sub_422BC0(0, 2, 2);
            sub_422BC0(0, 1, 4);
            sub_422BC0(0, 3, 0);
            sub_422BC0(0, 5, 2);
            sub_422BC0(0, 4, 4);
            sub_422BC0(0, 6, 0);
            if (sub_41E3C0())
            {
                fprintf(gFile, "Blend modes = %i \n", gBlendMode);
                fprintf(gFile, "Alpha modulate = %i \n", dword_6FC798);
                gColourKey = sub_41E730();
                fprintf(gFile, "ColorKey = %i\n", gColourKey);
                sub_422BC0(0, 12, 3);
                if (gModX2 == 2)
                    gModX2 = sub_41D420();
                fprintf(gFile, "MODULATE2X = %i \n", gModX2);
                if (gColourKey)
                {
                    Render_Unknown1(41, 1);
                }
                else
                {
                    Render_Unknown1(15, 1);
                    Render_Unknown1(24, 127);
                    Render_Unknown1(25, 7);
                }
                Render_Unknown1(26, 1);
                if (dword_651CF8)
                {
                    if (gLowRes != gLowRes)
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
                v47 = 0;
                v48 = 0;
                v49 = gXSize_dword_6DF214;
                v50 = gYSize;
                v51 = 0;
                v52 = 1065353216;
                pDirect3DDevice->SetViewport((D3DVIEWPORT7*)&v47);
                v2 = ((double)dword_651D94 - 50.0) / 100.0;
                sub_41C820(v2);
            }
            else
            {
                pDirect3DDevice->Release();
                pDirect3DDevice = 0;
                MessageBox_Sometimes(0, 5, "Metal Gear Solid PC", 0);
                gSoftwareRendering = 1;
            }
            break;
        }
        fputs("D3D:CreateDevice() failed, switching to SOFTWARE MODE\n", gFile);
        fflush(gFile);
        gXSize_dword_6DF214 = 320;
        gYSize = 240;
        fprintf(gLogFile, "Resetting DisplayMode to ( %d, %d )\n", gXSize_dword_6DF214, gYSize);
        MessageBox_Sometimes(0, 4, "Metal Gear Solid PC", 0);
        gSoftwareRendering = 1;
        dword_716F5C = 1065353216;
        gXRes = dword_716F5C; // TODO: Float
        v53 = pDirectDraw->SetCooperativeLevel(gHwnd, DDSCL_NORMAL);
        if (pGammaControl)
        { 
            pGammaControl->Release();
        }

        if (pBackBuffer)
        {
            pBackBuffer->Release();
            if (v53)
                PrintDDError("Can't release render surf", v53);
            pBackBuffer = 0;
        }
        if (pPrimarySurface)
        {
            pPrimarySurface->Release();
            if (v53)
                PrintDDError("Can't relaese primary surf", v53);
            pPrimarySurface = 0;
        }
        if (pClipper)
        {
            pClipper->Release();
            if (v53)
                PrintDDError("Can't release clipper", v53);
            pClipper = 0;
        }
        pPrimarySurface = 0;
        pBackBuffer = 0;
        pClipper = 0;
        if (pDirect3D)
        {
            pDirect3D->Release();
            pDirect3D = 0;
        }
        if (pDirectDraw)
        {
            pDirectDraw->Release();
            pDirectDraw = 0;
        }
    }
    fputs("other inits\n", gFile);
    fflush(gFile);
    for (i = 0; (signed int)i < 1500; ++i)
    {
        dword_6C0F00[20 * i] = 0;
        dword_6C0F20[20 * i] = 0;
        dword_6C0F24[20 * i] = 0;
    }
    dword_6FC7C0 = sub_41CA80() == 0;
    if (dword_6FC7C0)
    {
        gNoEffects = 0;
        dword_716F60 = 0;
    }
    sub_423F1B(0, (signed __int64)(14.0 * gXRes));
    MissionLog_Related2();
    if (!gSoftwareRendering)
    {
        dword_6C0EFC = (DWORD*)malloc(0x493E0u);
        for (i = 0; i < 15000; ++i)
        {
            dword_6C0EFC[5 * i] = 0;
        }
        dword_6FC780 = malloc(0x75300u);
    }
    dword_6FC728 = (DWORD*)malloc(0x100000u);
    if (dword_6FC728)
    {
        memset(dword_6FC728, 0, 0x100000u);
        dword_6FC72C = malloc(0x100000u);
        if (dword_6FC728)
        {
            memset(dword_6FC728, -1, 0x100000u);
            _cfltcvt_init();
            memset(&unk_6C0778, 0, 0x400u);
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

#define MSG_FUNC_NOT_IMPL(addr, signature, name) MgsFunction<addr, nullptr, signature> name(#name);
#define MSG_FUNC_IMPL(addr, funcName) MgsFunction<addr, funcName, decltype(funcName)> funcName##_(#funcName);

MSG_FUNC_NOT_IMPL(0x0051D120, void __cdecl(int, int), CheckForMmf);
MSG_FUNC_IMPL(0x0041ECB0, InitD3d_ProfileGfxHardwareQ);


// 0x00420810
signed int __cdecl DoInitAll()
{
    signed int v1; // ST10_4@1

    v1 = InitD3d_ProfileGfxHardwareQ();
    MessageBox_Sometimes(gHwnd, -1, "Metal Gear Solid PC", 0);
    return v1;
}

MSG_FUNC_IMPL(0x00420810, DoInitAll);
MSG_FUNC_NOT_IMPL(0x0052269C, signed int __cdecl(HWND), SoundInit);
MSG_FUNC_NOT_IMPL(0x004397D7, bool __cdecl(), AskUserToContinueIfNoSoundCard);
MSG_FUNC_NOT_IMPL(0x005224C8, int __cdecl (int), sub_5224C8);
MSG_FUNC_NOT_IMPL(0x0052255B, int __cdecl (int), sub_52255B);

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

// 0x00553090
signed int __stdcall DirectInputCreateExMGS(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{
    typedef decltype(&DirectInputCreateExMGS) fn;
    return ((fn)(0x00553090))(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

LPDIENUMDEVICESCALLBACKA EnumDevicesCallback = (LPDIENUMDEVICESCALLBACKA)0x0043B078;
LPDIENUMDEVICEOBJECTSCALLBACKA EnumDeviceObjectsCallback = (LPDIENUMDEVICEOBJECTSCALLBACKA)0x0043B0C8;
LPDIENUMDEVICEOBJECTSCALLBACKA CountDeviceObjectsCallback = (LPDIENUMDEVICEOBJECTSCALLBACKA)0x0043B0B3;

VAR(DWORD, dword_71D670, 0x71D670);
VAR(DWORD, dword_71D790, 0x71D790);
VAR(DWORD, dword_71D798, 0x71D798);
VAR(LPDIRECTINPUT7, pDirectInput, 0x71D664);
VAR(LPDIRECTINPUTDEVICE7, pJoystickDevice, 0x71D66C);
VAR(LPDIRECTINPUTDEVICEA, pMouseDevice, 0x71D668);
VAR(DWORD, dword_71D41C, 0x71D41C);
VAR(DIDEVICEINSTANCEA, JoystickDeviceInfos, 0x71D420);
VAR(DIDATAFORMAT, JoystickDataFormat, 0x64DA88);
VAR(DIDATAFORMAT, MouseDataFormat, 0x64DA70);
VAR(DIDEVCAPS, JoystickDeviceCaps, 0x71D1D8);
DWORD* dword_65714C = (DWORD*)0x65714C;
DWORD* dword_657184 = (DWORD*)0x657184;
DWORD* dword_6571BC = (DWORD*)0x6571BC;
DWORD* dword_6571F4 = (DWORD*)0x6571F4;
char* sidewinderEtc = (char*)0x657298;
GUID& IID_IDirectInput7A_MGS = *((GUID*)0x64B028);
GUID& GUID_SysMouse_MGS = *((GUID*)0x64AEE8);
DWORD* dword_65726C = (DWORD*)0x65726C;
char* buttonNames = (char*)0x65510C;
char* buttonList = (char*)0x654A98;
VAR(DWORD, nJoystickDeviceObjects, 0x71D68C);
VAR(DWORD, dword_6FD1DC, 0x6FD1DC);


// Implementation untested for the moment
// 0x0043B1D1
int __cdecl InitDirectInput(HWND hWnd)
{
    char productName[0x80];
    char instanceName[0x20];
    dword_71D670 = 0;
    //fputs("InitDirectInput {\n", gLogFile);
    // I'll do log prints later
    HRESULT hr = DirectInputCreateExMGS(gHInstance, DIRECTINPUT_VERSION, IID_IDirectInput7A_MGS, (LPVOID*)&pDirectInput, 0);
    if (hr < 0)
        return hr;

    hr = pDirectInput->EnumDevices(DIDEVTYPE_JOYSTICK, EnumDevicesCallback, 0, DIEDFL_ATTACHEDONLY);
    if (hr >= 0)
    {
        if (pJoystickDevice != 0)
        {
            memset(&JoystickDeviceInfos, 0, sizeof(DIDEVICEINSTANCEA));
            assert(sizeof(DIDEVICEINSTANCEA) == 0x244);
            JoystickDeviceInfos.dwSize = sizeof(DIDEVICEINSTANCEA);
            HRESULT hGetInfosRes = pJoystickDevice->GetDeviceInfo(&JoystickDeviceInfos);
            hr = pJoystickDevice->SetDataFormat(&JoystickDataFormat);
            if (hr >= 0)
            {
                hr = pJoystickDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
                if (hr >= 0)
                {
                    hr = pJoystickDevice->GetCapabilities(&JoystickDeviceCaps);
                    if (hr >= 0)
                    {
                        pJoystickDevice->EnumObjects(EnumDeviceObjectsCallback, hWnd, DIDFT_AXIS);
                        pJoystickDevice->EnumObjects(CountDeviceObjectsCallback, hWnd, DIDFT_BUTTON);
                        hr = pJoystickDevice->Acquire();
                        if (hr >= 0)
                        {
                            if (hGetInfosRes >= 0)
                            {
                                strcpy((char*)0x71D690, JoystickDeviceInfos.tszInstanceName);

                                for (int i = 0; i < 6; i++)
                                {
                                    int var14 = 1;
                                    strcpy(productName, JoystickDeviceInfos.tszProductName);
                                    _strlwr(productName);
                                    strcpy(instanceName, JoystickDeviceInfos.tszInstanceName);
                                    _strlwr(instanceName);

                                    for (int j = 0; j < 5; j++)
                                    {
                                        size_t offset = i * 0x140 + j * 0x40;
                                        if (strstr(productName, &sidewinderEtc[offset]) == 0 && strstr(instanceName, &sidewinderEtc[offset]) == 0)
                                        {
                                            var14 = 0;
                                        }
                                    }

                                    if (var14 != 0)
                                    {
                                        if (i == 5)
                                            i = 4;

                                        dword_71D790 = 1;
                                        dword_71D41C = dword_65726C[i * 2];
                                        dword_71D798 = i + 1;

                                        for (int nButton = 0; nButton < 0x38; nButton++)
                                        {
                                            size_t offset = i * 0x672 + nButton * 0x19;
                                            strcpy(&buttonList[nButton * 0x19], &buttonNames[offset]);
                                        }
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                dword_71D790 = 0;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = 0;
                                    dword_657184[i] = 0;
                                }
                            }
                            if (dword_71D798 == 5)
                            {
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_6571BC[i] = 0xFF;
                                    dword_6571F4[i] = 0xFF;
                                }
                                dword_6571BC[ 0] = dword_6571F4[ 0] = 0;
                                dword_6571BC[ 1] = dword_6571F4[ 1] = 1;
                                dword_6571BC[ 2] = dword_6571F4[ 2] = 2;
                                dword_6571BC[ 3] = dword_6571F4[ 3] = 3;
                                dword_6571BC[ 4] = dword_6571F4[ 4] = 6;
                                dword_6571BC[ 5] = dword_6571F4[ 5] = 6;
                                dword_6571BC[ 6] = dword_6571F4[ 6] = 7;
                                dword_6571BC[ 7] = dword_6571F4[ 7] = 7;
                                dword_6571BC[ 8] = dword_6571F4[ 8] = 4;
                                dword_6571BC[ 9] = dword_6571F4[ 9] = 0x21;
                                dword_6571BC[10] = dword_6571F4[10] = 0x20;
                                dword_6571BC[11] = dword_6571F4[11] = 0x23;
                                dword_6571BC[12] = dword_6571F4[12] = 0x22;
                                dword_6571BC[13] = dword_6571F4[13] = 5;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = dword_657184[i];
                                }
                            }
                            else if (dword_71D798 != 1 && dword_71D798 != 4)
                            {
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_6571BC[i] = 0xFF;
                                    dword_6571F4[i] = 0xFF;
                                }
                                int var124 = 0;
                                for (int i = 0; i < 14; i++)
                                {
                                    if (dword_71D790 != 0 && var124 == dword_71D41C)
                                    {
                                        var124++;
                                    }
                                    if (var124 == nJoystickDeviceObjects)
                                        break;

                                    if (i == 9)
                                        i = 13;

                                    dword_6571BC[i] = var124;
                                    dword_6571F4[i] = var124;

                                    var124++;
                                }
                                dword_6571BC[ 9] = dword_6571F4[ 9] = 0x21;
                                dword_6571BC[10] = dword_6571F4[10] = 0x20;
                                dword_6571BC[11] = dword_6571F4[11] = 0x23;
                                dword_6571BC[12] = dword_6571F4[12] = 0x22;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = 0;
                                    dword_657184[i] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (hr < 0)
    {
        for (int i = 0; i < dword_6FD1DC; i++)
        {
            dword_6571F4[i] = 0xFF;
        }
    }

    // 0x43BBEC
    hr = pDirectInput->CreateDevice(GUID_SysMouse_MGS, &pMouseDevice, 0);
    if (hr < 0)
        return hr;

    hr = pMouseDevice->SetDataFormat(&MouseDataFormat);
    if (hr < 0)
        return hr;

    if (gWindowedMode != 0)
    {
        hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    }
    else
    {
        hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    }
    if (hr < 0)
        return hr;

    hr = pMouseDevice->Acquire();

    return 0;
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


// 0x00459A9A
int __cdecl Menu_Related1()
{
    typedef decltype(&Menu_Related1) fn;
    return ((fn)(0x00459A9A))();
}

WORD& word_7227C8 = *(WORD*)0x7227C8;

// 0x44EAED
void *__cdecl sub_44EAED()
{
    return memset(&word_7227C8, 0, 0x10u);
}

// 0x457B5B
void *__cdecl sub_457B5B()
{
    void *result; // eax@1

    result = memcpy(&word_994320, &word_669AE0, 0x1000u);
    dword_993F44 = (int)&word_994320;
    return result;
}

// 0x452610
int __cdecl sub_452610()
{
    typedef decltype(&sub_452610) fn;
    return ((fn)(0x452610))();
}

// 0x44E9D2
signed int __cdecl sub_44E9D2(int a1)
{
    typedef decltype(&sub_44E9D2) fn;
    return ((fn)(0x44E9D2))(a1);
}

// 0x40A68D
int __cdecl sub_40A68D(int number, int fn)
{
    int result; // eax@1

    result = fn;
    *((DWORD *)&gActors[4].field_0 + number) = fn;
    return result;
}

// 0x44E1E0
__int16 __cdecl sub_44E1E0()
{
    __int16 result; // ax@1

    word_78E7FC = -1;
    result = word_78E7FC;
    word_78E7FE = word_78E7FC;
    return result;
}

// 0x0040A2AF
actor_related_struct *__cdecl Actor_Unknown4(int actorIdx, actor_related_struct *a2, void(__cdecl *actorFn)(actor_related_struct*))
{
    typedef decltype(&Actor_Unknown4) fn;
    return ((fn)(0x0040A2AF))(actorIdx, a2, actorFn);
    /*
    actor_related_struct *result; // eax@1
    actor_related_struct *v4; // ST04_4@1

    v4 = gActors[actorIdx].actor_struct_ptr2;
    gActors[actorIdx].actor_struct_ptr2 = a2;
    v4->actor_struct_ptr1 = a2;
    a2->actor_struct_ptr1 = &gActors[actorIdx] + 8;
    a2->field_0 = v4;
    a2->fnUnknown3 = 0;
    a2->fn_unknown = 0;
    result = a2;
    a2->fnUnknown2 = actorFn;
    return result;
    */
}

// 0x44E381
void *__cdecl sub_44E381(int a1)
{
    typedef decltype(&sub_44E381) fn;
    return ((fn)(0x44E381))(a1);
}

actor_related_struct *__cdecl Actor_Unknown6(actor_related_struct *a1, int fn1, int fn2, char *srcFileName)
{
    actor_related_struct *result; // eax@1

    a1->fn_unknown = (void(__cdecl *)(actor_related_struct*))fn1;
    a1->fnUnknown3 = (void(__cdecl *)(actor_related_struct*))fn2;
    a1->mNamePtr = srcFileName;
    a1->field_1C = 0;
    result = a1;
    a1->field_18 = 0;
    return result;
}

// 0x0044E1F9
int __cdecl unknown_libname_3() // Note: Not a CRT func!!
{
    typedef decltype(&unknown_libname_3) fn;
    return ((fn)(0x0044E1F9))();
}

// 0x44E287
void __cdecl sub_44E287()
{
    typedef decltype(&sub_44E287) fn;
    ((fn)(0x44E287))();
}

// 0x44E212
void *__cdecl sub_44E212()
{
    typedef decltype(&sub_44E212) fn;
    return ((fn)(0x44E212))();
}


// 0x0040B36E
int __cdecl GetResidentTop()
{
    int result; // eax@1

    result = gResidentTop_dword_78E960;
    dword_78E964 = gResidentTop_dword_78E960;
    return result;
}

// 0x44E226
actor_related_struct *__cdecl sub_44E226()
{
    typedef decltype(&sub_44E226) fn;
    return ((fn)(0x44E226))();
}

// 0x44E12B
void *__cdecl sub_44E12B()
{
    dword_995344 = 0;
    dword_7227A4 = 0;
    *(DWORD *)&stru_722760.mPause = 0;
    dword_9942B8 = 0;
    Menu_Related1();
    sub_44EAED();
    sub_457B5B();
    sub_452610();
    sub_40A68D(98, (int)sub_44E9D2);
    sub_44E1E0();
    Actor_Unknown4(1, &stru_722760, 0);
    Actor_Unknown6(&stru_722760, (int)sub_44E381, 0, "C:\\mgs\\source\\Game\\gamed.c");

    unknown_libname_3();
    sub_44E287();
    sub_44E212();
    word_78E7E8 = (WORD)(dword_78D7B0 + 1);
    dword_995324 = (int)&dword_7919C0;
    GetResidentTop();
    stru_722760.actor_struct_ptr2 = 0;
    stru_722760.field_24 = 0;
    return sub_44E226();
}

// 0x0051C9A2
signed int __cdecl MainLoop()
{
    typedef decltype(&MainLoop) fn;
    return ((fn)(0x0051C9A2))();
}

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


int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    int result; // eax@2
    HANDLE currentProcess; // eax@14
    void(__stdcall *pSetProcessAffinityMask)(HANDLE, signed int); // [sp+8h] [bp-464h]@13
    void(__stdcall *pSetThreadExecutionState)(unsigned int); // [sp+Ch] [bp-460h]@13
    HMODULE hKernel32; // [sp+10h] [bp-45Ch]@12
    char Dest[256]; // [sp+14h] [bp-458h]@11
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
                gFps = 1;

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
                Dest,
                "Metal Gear Solid requires over 50mb of hard disk space as Virtual Memory before the game can function correctly. This system currently only has %dmb available.  Please close all open applications not in use,  and refer to the Metal Gear Solid readme for more information on this issue.",
                (Buffer.dwAvailPageFile - Buffer.dwAvailPhys) >> 20);
            MessageBoxA(0, Dest, "Metal Gear Solid PC", 0);
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
