#include "stdafx.h"
#include "Renderer.hpp"
#include "LibDG.hpp"
#include "Script.hpp"
#include "Timer.hpp"
#include "WinMain.hpp"
#include <time.h>

#define RENDERER_IMPL true

void RendererCpp_ForceLink() { }

signed int CC Render_ComputeTextureIdx_40CC50(__int16 tpage, __int16 u, __int16 v, uint32_t *textureIdx0, uint32_t *textureIdx1)
{
    *textureIdx0 = (tpage & 0xF) << 6;
    *textureIdx1 = 16 * (tpage & 0x10);
    unsigned __int16 new_u = 0 /*word_6C0EA4*/ + u;
    unsigned __int16 new_v = 0 /*word_6C0EA6*/ + v;

    int texturePage = (tpage & 0x180) >> 7;
    if (texturePage)
    {
        if (texturePage == 1)
        {
            *textureIdx0 += (signed int)new_u >> 1;
        }
        else if (texturePage == 2)
        {
            *textureIdx0 += new_u;
        }
        else
        {
            PrintDDError("Wrong Tpage format", 0);
        }
    }
    else
    {
        *textureIdx0 += (signed int)new_u >> 2;
    }

    *textureIdx1 += new_v;

    if (*textureIdx0 >= 640 || *textureIdx1 >= 240)
    {
        return 0;
    }

    if (*textureIdx0 >= 320)
    {
        *textureIdx0 -= 320;
    }

    return 1;
}
MGS_FUNC_IMPLEX(0x40CC50, Render_ComputeTextureIdx_40CC50, RENDERER_IMPL);

MGS_FUNC_NOT_IMPL(0x52078F, const char* __cdecl(), SoundGetName_52078F);

MGS_VAR(RENDERER_IMPL, 0x776858, int, gSurfaceStackIdx_dword_776858, 0);
MGS_ARY(RENDERER_IMPL, 0x774F48, IDirectDrawSurface7*, 1024, gSurfaceStack_dword_774F48, {});

void CC Render_FreeSurfaceStack_51DE0A()
{
    for (int i = 0; i < gSurfaceStackIdx_dword_776858; i++)
    {
        gSurfaceStack_dword_774F48[i]->Release();
    }
    gSurfaceStackIdx_dword_776858 = 0;
}
MGS_FUNC_IMPLEX(0x51DE0A, Render_FreeSurfaceStack_51DE0A, RENDERER_IMPL);

void CC Render_PushSurface_51DDF2(IDirectDrawSurface7* pSurface)
{
    gSurfaceStack_dword_774F48[gSurfaceStackIdx_dword_776858] = pSurface;
    gSurfaceStackIdx_dword_776858++;
}
MGS_FUNC_IMPLEX(0x51DDF2, Render_PushSurface_51DDF2, RENDERER_IMPL);

MGS_VAR(1, 0x6FC7B0, WORD, gDisp_w_word_6FC7B0, 0);
MGS_VAR(1, 0x6FC7B2, WORD, gDisp_y_word_6FC7B2, 0);
MGS_VAR(1, 0x6DF200, DISPENV, sDispEnv_6DF200, {});

void CC Render_Scene_DispEnv_40DD00(DISPENV* pRect)
{
    int bChanged = memcmp(pRect, &sDispEnv_6DF200.disp, sizeof(PSX_RECT));
    memcpy(&sDispEnv_6DF200, pRect, sizeof(sDispEnv_6DF200));
    
    gDisp_w_word_6FC7B0 = 0;
    gDisp_y_word_6FC7B2 = sDispEnv_6DF200.disp.y1;
    
    if (!sDispEnv_6DF200.disp.x1)
    {
        gDisp_w_word_6FC7B0 = 320;
    }

    if (bChanged)
    {
        Render_Loop_SetWinTitle_422210();
    }
}
MGS_FUNC_IMPLEX(0x40DD00, Render_Scene_DispEnv_40DD00, RENDERER_IMPL);

MGS_VAR(1, 0x6FC780, MGSVertex*, g_pMGSVertices_6FC780, 0);
MGS_VAR(1, 0x6FC784, DWORD, g_nVertexOffset_6FC784, 0);
MGS_VAR(1, 0x791C80, float, g_fXOffset, 0);
MGS_VAR(1, 0x791C84, float, g_fYOffset, 0);
MGS_VAR(1, 0x00650D30, DWORD, gModX2, 0);
MGS_VAR(1, 0x791C54, DWORD, otItemType_dword_791C54, 0);

MGS_VAR(1, 0x791C5C, float, g_fV3, 0);
MGS_VAR(1, 0x791C60, float, g_fV2, 0);
MGS_VAR(1, 0x791C64, float, g_fV1, 0);
MGS_VAR(1, 0x791C68, float, g_fV0, 0);
MGS_VAR(1, 0x791C6C, float, g_fU3, 0);
MGS_VAR(1, 0x791C70, float, g_fU2, 0);
MGS_VAR(1, 0x791C74, float, g_fU1, 0);
MGS_VAR(1, 0x791C78, float, g_fU0, 0);

MGS_VAR(1, 0x688CD0, DWORD, dword_688CD0, 0);
MGS_VAR(1, 0x688CD4, DWORD, dword_688CD4, 0);
MGS_VAR(1, 0x791C58, DWORD, size_dword_791C58, 0);

MGS_VAR(1, 0x6C0EA0, WORD, g_wXOffset, 0);
MGS_VAR(1, 0x6C0EA2, WORD, g_wYOffset, 0);

MGS_VAR(1, 0x6C0EAC, WORD, word_6C0EAC, 0);
MGS_VAR(1, 0x791C7C, DWORD, g_nTextureIndex, 0);
MGS_VAR(1, 0x6FC72C, WORD*, g_pwTextureIndices_6FC72C, 0);
MGS_VAR(1, 0x6FC78C, WORD, gNumTextures_word_6FC78C, 0);
MGS_VAR(1, 0x00650D28, float, gXRes, 0.0f);

MGS_VAR(1, 0x6C0EFC, prim_struct*, gPrimBuffer_dword_6C0EFC, nullptr); // Dynamically allocated array of 15000 items

MGS_VAR(1, 0x6FC788, DWORD, gPrimIdx_dword_6FC788, 0);
MGS_VAR(1, 0x6FC774, DWORD, dword_6FC774, 0);
MGS_VAR(1, 0x6FC744, DWORD, dword_6FC744, 0);
MGS_VAR(1, 0x650D10, DWORD, dword_650D10, 0);
MGS_VAR(1, 0x6FC778, DWORD, gStageIs_s11e_6FC778, 0);
MGS_VAR(1, 0x6FC760, DWORD, dword_6FC760, 0);



MGS_VAR(1, 0x6C0EAE, WORD, word_6C0EAE, 0);
MGS_VAR(1, 0x6C0E9A, WORD, word_6C0E9A, 0);
MGS_VAR(1, 0x6C0E9C, WORD, word_6C0E9C, 0);
MGS_VAR(1, 0x6C0E9E, WORD, word_6C0E9E, 0);
MGS_VAR(1, 0x6FC718, DWORD, dword_6FC718, 0);
MGS_VAR(1, 0x6FC720, DWORD, gSkipFrame_dword_6FC720, 0);
MGS_VAR(1, 0x6FC768, DWORD, dword_6FC768, 0);
MGS_VAR(1, 0x006FC794, DWORD, gSoftwareRendering, 0);
MGS_VAR(1, 0x6FC738, LPDIRECTDRAWSURFACE7, g_pBackBuffer_6FC738, nullptr);
MGS_VAR(1, 0x6FC868, void*, g_pBackBufferSurface, 0);
MGS_VAR(1, 0x6FC86C, DWORD, g_BackBufferPitch, 0);
MGS_VAR(1, 0x6FC734, LPDIRECTDRAWSURFACE7, gPrimarySurface_6FC734, nullptr);
MGS_ARY(1, 0x6C0F00, texture_struct, 1500, gTextures_6C0F00, {}); // Array of 1500 items

MGS_VAR(1, 0x6FC7DC, DWORD, gSkippedFrames_dword_6FC7DC, 0);
MGS_VAR(1, 0x6FC724, DWORD, sSceneStarted_dword_6FC724, 0);
MGS_VAR(1, 0x99562C, DWORD, gLastInputWasKeyBoard_dword_99562C, 0);
MGS_VAR(1, 0x6FC764, WORD, word_6FC764, 0);
MGS_VAR(1, 0x6FC790, short, gNumFreeTextures_6FC790, 0);
MGS_VAR(1, 0x9ADDA4, DWORD, dword_9ADDA4, 0);
MGS_VAR(1, 0x650D44, DWORD, dword_650D44, 0);
MGS_VAR(1, 0x733EA0, DWORD, dword_733EA0, 0);

double CC GetFPS_422A10()
{
    static DWORD sFrameCount_6FC7B4 = 0;
    static clock_t sClock_6FC7B8 = {};
    static double sFps_6FC7BC = 0.0;

    if (++sFrameCount_6FC7B4 == 64)
    {
        sClock_6FC7B8 = clock() - sClock_6FC7B8;

        if (sClock_6FC7B8)
        {
            sFps_6FC7BC = sFrameCount_6FC7B4 * 1000.0 / sClock_6FC7B8;
        }
        else
        {
            sFps_6FC7BC = 1000.0;
        }

        sClock_6FC7B8 = clock();
        sFrameCount_6FC7B4 = 0;
    }
    return sFps_6FC7BC;
}
MGS_FUNC_IMPLEX(0x422A10, GetFPS_422A10, RENDERER_IMPL);


void CC Render_Loop_SetWinTitle_422210()
{
    if (gSkipFrame_dword_6FC720)
    {
        ++gSkippedFrames_dword_6FC7DC;
    }
    else
    {
        gSkippedFrames_dword_6FC7DC = 0;
    }

    HRESULT hrErr = S_OK;
    if (sSceneStarted_dword_6FC724)
    {
        if (gD3dDevice_6FC74C)
        {
            if (!gSoftwareRendering)
            {
                hrErr = gD3dDevice_6FC74C->EndScene();
                if (hrErr == DDERR_SURFACELOST)
                {
                    Render_Restore_Lost_Surfaces_51E086();
                }

                if (FAILED(hrErr))
                {
                    PrintDDError("Can't end scene", hrErr);
                }
                sSceneStarted_dword_6FC724 = 0;
            }
        }
    }

   
    if (gFps)
    {
        HDC hdc = {};
        const char* pSkip = "";
        if (gSkipFrame_dword_6FC720)
        {
            pSkip = "skip";
        }

        const char* pKeyboardOrController = "";
        if (gLastInputWasKeyBoard_dword_99562C == 1)
        {
            pKeyboardOrController = "kb";
        }

        const auto hu2 = BYTE2(save_data_2048_unk_78D7C0[2]) + (BYTE3(save_data_2048_unk_78D7C0[2]) << 8);// something to do with game progression?
        const auto hu1 = BYTE2(save_data_2048_unk_78D7C0[2]) + (BYTE3(save_data_2048_unk_78D7C0[2]) << 8);
        const double fps = GetFPS_422A10();
        char stringBuffer[256] = {};
        sprintf(
            stringBuffer,
            "%05.2f %s w[%hu] s[%hu] N tex%i freetex%i N prim%i ViewTexN%i GMStat(%x) %s",
            static_cast<float>(fps),
            pKeyboardOrController,
            hu1,
            hu2,
            gNumTextures_word_6FC78C,
            gNumFreeTextures_6FC790,
            gPrimIdx_dword_6FC788,
            word_6FC764, // ViewTexN
            game_state_dword_72279C.flags,
            pSkip);

        SetWindowTextA(gHwnd, stringBuffer);
        dword_9ADDA4 = 0;
        g_pBackBuffer_6FC738->GetDC(&hdc);
        SetBkMode(hdc, 1);
        SetTextColor(hdc, 0xFFFF00u);
        TextOutA(hdc, 0, 0, stringBuffer, 5);
        g_pBackBuffer_6FC738->ReleaseDC(hdc);
    }

    dword_650D44 = 1;

    // Hard coded to off in the real game, we turn it on for the debug infos
    //if (false)
    {
        HDC hdc = {};
        const char* lpString = SoundGetName_52078F();
        if (lpString)
        {
            g_pBackBuffer_6FC738->GetDC(&hdc);
            SetBkMode(hdc, 1);
            SetTextColor(hdc, 0xC03050u);
            TextOutA(hdc, 20, 20, lpString, strlen(lpString));
            g_pBackBuffer_6FC738->ReleaseDC(hdc);
        }
    }

    if (!gSkipFrame_dword_6FC720)
    {
        RECT Rect = {};
        Rect.left = 0;
        Rect.top = 0;
        Rect.right = g_dwDisplayWidth_6DF214;
        Rect.bottom = g_dwDisplayHeight;

        if (gWindowedMode)
        {
            GetClientRect(gHwnd, &Rect);
            ClientToScreen(gHwnd, (LPPOINT)&Rect);
            ClientToScreen(gHwnd, (LPPOINT)&Rect.right);
        }

        RECT v22 = {};
        v22.left = 0;
        v22.top = 0;
        v22.right = g_dwDisplayWidth_6DF214;
        v22.bottom = g_dwDisplayHeight;

        HRESULT hr = S_OK;
        if (gWindowedMode || gSoftwareRendering)
        {
            do
            {
                hr = gPrimarySurface_6FC734->Blt(
                    &Rect,
                    g_pBackBuffer_6FC738,
                    &v22,
                    0x1000000,
                    0);
            } while (hr == DDERR_WASSTILLDRAWING);
        }
        else
        {
            hr = gPrimarySurface_6FC734->Flip(0, 1);
        }

        if (FAILED(hr))
        {
            PrintDDError("Can't blit", hr);
        }

        if (gSoftwareRendering || !gInfiniteAmmoCheat_650D4C)
        {
            DDBLTFX bltFx = {};
            bltFx.dwSize = 100;
            bltFx.dwFillColor = 0;
            do
            {
                hr = g_pBackBuffer_6FC738->Blt(0, 0, 0, 0x1000400, &bltFx);
            } while (hr == DDERR_WASSTILLDRAWING);
        }
        else
        {
            DDSURFACEDESC2 surfaceDesc = {};
            surfaceDesc.dwSize = 0x7C;
            memset(&surfaceDesc.dwFlags, 0, 120u);

            D3DVIEWPORT7 viewPort = {};
            gD3dDevice_6FC74C->GetViewport(&viewPort);
            g_pBackBuffer_6FC738->GetSurfaceDesc(&surfaceDesc);

            D3DVIEWPORT7 viewPortToSet = {};
            viewPortToSet.dwX = 0;
            viewPortToSet.dwY = 0;
            viewPortToSet.dwWidth = surfaceDesc.dwWidth;
            viewPortToSet.dwHeight = surfaceDesc.dwHeight;
            viewPortToSet.dvMinZ = 0;
            viewPortToSet.dvMaxZ = 0x3F800000;
            gD3dDevice_6FC74C->SetViewport(&viewPortToSet);
            gD3dDevice_6FC74C->Clear(
                0,
                0,
                1,
                0,
                0,
                0);
            gD3dDevice_6FC74C->SetViewport(&viewPort);
        }

        if (FAILED(hr))
        {
            PrintDDError("Can't color ren surf", hr);
        }
    }

    static LARGE_INTEGER sPerformanceCount = {};
    if (!dword_6FC7D8 && !counter_dword_6BED20)
    {
        QueryPerformanceCounter(&sPerformanceCount);
        while (sPerformanceCount.QuadPart - gTimer_PerformanceCount.QuadPart < qword_6DF248)
        {
            LARGE_INTEGER Frequency = {};
            DWORD dwMilliseconds = static_cast<DWORD>(qword_6DF248 - (sPerformanceCount.QuadPart - gTimer_PerformanceCount.QuadPart));
            QueryPerformanceFrequency(&Frequency);
            if (Frequency.QuadPart)
            {
                const DWORD dwMillisecondsa = 500 * dwMilliseconds;
                if (dwMillisecondsa >= Frequency.QuadPart)
                {
                    DWORD dwMillisecondsb = static_cast<DWORD>(dwMillisecondsa / Frequency.QuadPart);

                    if (dwMillisecondsb > 8)
                    {
                        dwMillisecondsb = 8;
                    }

                    if (dwMillisecondsb < 0)
                    {
                        dwMillisecondsb = 0;
                    }

                    Sleep(dwMillisecondsb);
                }
            }
            QueryPerformanceCounter(&sPerformanceCount);
        }
    }

    gSkipFrame_dword_6FC720 = 0;
    qword_6DF248 += gTimer_Frequency.QuadPart;
    if (!dword_6FC7D8 && !dword_733EA0)
    {
        QueryPerformanceCounter(&sPerformanceCount);
        if (counter_dword_6BED20 > 0 || sPerformanceCount.QuadPart - gTimer_PerformanceCount.QuadPart > qword_6DF248)
        {
            gSkipFrame_dword_6FC720 = 1;
        }
    }

    if (!sSceneStarted_dword_6FC724)
    {
        if (gD3dDevice_6FC74C)
        {
            if (!gSoftwareRendering)
            {
                hrErr = gD3dDevice_6FC74C->BeginScene();

                if (hrErr == DDERR_SURFACELOST)
                {
                    Render_Restore_Lost_Surfaces_51E086();
                    hrErr = gD3dDevice_6FC74C->BeginScene();
                }

                if (FAILED(hrErr))
                {
                    PrintDDError("Can't begin scene", hrErr);
                }
                else
                {
                    sSceneStarted_dword_6FC724 = 1;
                }
            }
        }
    }
    if (gSkipFrame_dword_6FC720)
    {
        if (gSkippedFrames_dword_6FC7DC == 11)
        {
            gSkipFrame_dword_6FC720 = 0;
            gSkippedFrames_dword_6FC7DC = 0;
        }
    }
}
MGS_FUNC_IMPLEX(0x422210, Render_Loop_SetWinTitle_422210, RENDERER_IMPL);

/* TODO: Implement me
uint32_t __cdecl Render_ComputeUVs(uint32_t textureIdx, uint32_t a1, uint16_t u, uint16_t v, float* outU, float* outV);
MSG_FUNC_IMPL(0x40CD80, Render_ComputeUVs)

uint32_t __cdecl Render_ComputeUVs(uint32_t textureIdx, uint32_t a1, uint16_t u, uint16_t v, float* outU, float* outV)
{
uint32_t ret = Render_ComputeUVs_.Ptr()(textureIdx, a1, u, v, outU, outV);
//LOG_INFO("t: " << textureIdx << " a: " << a1 << " u " << u << " v " << v << " ou " << *outU << " ov " << *outV);
return ret;
}
*/

MGS_FUNC_NOT_IMPL(0x40CD80, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t, float*, float*), Render_ComputeUVs);
MGS_FUNC_NOT_IMPL(0x40FF20, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t, float*, float*), sub_40FF20);
MGS_FUNC_NOT_IMPL(0x40D540, uint32_t __cdecl(int16_t*, int32_t, int32_t), sub_40D540);
MGS_FUNC_NOT_IMPL(0x418A70, int __cdecl(struct TaggedOrderingTablePointer* a_pStructVert, int a_nSize), Render_Software);
MGS_FUNC_NOT_IMPL(0x421280, void __cdecl(MGSVertex *pVert, int idx), Render_sub_421280);
MGS_FUNC_NOT_IMPL(0x424020, void __cdecl(IDirectDrawSurface7 *pSurface, MGSVertex* pVert), Render_DrawTextBeginScene_424020);
MGS_FUNC_NOT_IMPL(0x420840, void __cdecl (DWORD *a1, DWORD *arg4), Render_sub_420840);

HRESULT CC Render_SetTexture_41E9E0();

void CC Render_BlendMode_sub_421800(int mode, MGSVertex* pVerts, signed int vertexCount, int primIdx)
{
    MGSVertex pVertsToRender[10] = {};

    assert(vertexCount < 10);

    HRESULT hr = S_OK;
    if (gBlendMode)
    {
        switch (mode)
        {
        case 0:
            Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
            break;
        case 1:
            hr = Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 5);// D3DRS_SRCBLEND 
            hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 5);
            if (hr || !(gBlendMode & 1))
            {
                int texture = 0;
                while (texture < vertexCount)
                {
                    BYTE* vertDiffuse = (BYTE*)&pVerts[texture].diffuse;
                    vertDiffuse[2] >>= 1;
                    vertDiffuse[1] >>= 1;
                    vertDiffuse[0] >>= 1;
                    ++texture;
                }
                hr = Render_SetTexture_41E9E0();
                if (!hr)
                {
                    memcpy(pVertsToRender, pVerts, 4 * ((unsigned int)(sizeof(MGSVertex) * vertexCount) >> 2));
                    for (int i = 0; i < vertexCount; ++i)
                    {
                        pVertsToRender[i].diffuse = 0xFF7F7F7F;
                    }

                    hr = Render_SetRenderState_422A90(D3DRENDERSTATE_SHADEMODE, 1);
                    hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 1);
                    hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 3);
                    if (!hr)
                    {
                        hr = gD3dDevice_6FC74C->DrawPrimitive(
                            (D3DPRIMITIVETYPE)gPrimBuffer_dword_6C0EFC[primIdx].mPrimTypeQ,
                            0x1C4,
                            pVertsToRender,
                            vertexCount,
                            0);
                        gD3dDevice_6FC74C->SetTexture(0, 0);
                    }
                }
                Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 2);
                Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 2);
            }
            else
            {
                for (int texture = 0; texture < vertexCount; ++texture)
                {
                    pVerts[texture].diffuse = pVerts[texture].diffuse & 0xFFFFFF | 0x7F000000;
                }
            }
            break;
        case 2:
            Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 2);
            Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 2);
            break;
        case 3:
            hr = Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 1);
            hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 4);
            break;
        case 4:
            hr = Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHAREF, 63);
            hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 5);
            hr |= Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 2);
            if (hr || !(gBlendMode & 8))
            {
                int texture = 0;
                while (texture < vertexCount)
                {
                    BYTE* vertDiffuse = (BYTE*)&pVerts[texture].diffuse;
                    vertDiffuse[2] >>= 2;  // halve the RGB of the diffuse ?
                    vertDiffuse[1] >>= 2;
                    vertDiffuse[0] >>= 2;
                    ++texture;
                }
                Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
                Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 2);
                Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 2);
            }
            else
            {
                for (int texture = 0; texture < vertexCount; ++texture)
                {
                    pVerts[texture].diffuse = pVerts[texture].diffuse & 0xFFFFFF | 0x3F000000;
                }
            }
            break;
        default:
            return;
        }
    }
    else
    {
        Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
    }
}
MGS_FUNC_IMPLEX(0x421800, Render_BlendMode_sub_421800, RENDERER_IMPL);

// WinMain.cpp
MGS_VAR_EXTERN(LPDIRECTDRAWSURFACE7, g_pDDSurface_6FC740);
MGS_VAR_EXTERN(DWORD, gNoFilter);
MGS_VAR_EXTERN(DWORD, gNoEffects);

MGS_ARY(RENDERER_IMPL, 0x6DEF98, DWORD, 153, sRenderStates_6DEF98, {});

HRESULT CC Render_SetRenderState_422A90(D3DRENDERSTATETYPE renderState, DWORD value)
{
    static bool sbNotDone_650D50 = true;
    if (sbNotDone_650D50)
    {
        gD3dDevice_6FC74C->SetRenderState(D3DRENDERSTATE_LIGHTING, 0);
        gD3dDevice_6FC74C->SetRenderState(D3DRENDERSTATE_LOCALVIEWER, 0);
        gD3dDevice_6FC74C->SetRenderState(D3DRENDERSTATE_CLIPPING, 1);
        for (int i = 0; i < 153; ++i)
        {
            gD3dDevice_6FC74C->GetRenderState(static_cast<D3DRENDERSTATETYPE>(i), &sRenderStates_6DEF98[i]);
        }
        sbNotDone_650D50 = 0;
    }

    HRESULT ret = S_OK;
    if (renderState >= 153)
    {
        ret = gD3dDevice_6FC74C->SetRenderState(renderState, value);
    }
    else if (sRenderStates_6DEF98[renderState] != value)
    {
        ret = gD3dDevice_6FC74C->SetRenderState(renderState, value);
        gD3dDevice_6FC74C->GetRenderState(renderState, &sRenderStates_6DEF98[renderState]);
    }
    return ret;
}
MGS_FUNC_IMPLEX(0x422A90, Render_SetRenderState_422A90, RENDERER_IMPL);

HRESULT CC Render_InitTextureStages_422BC0(unsigned int stage, D3DTEXTURESTAGESTATETYPE type2, DWORD v)
{
    static bool sTexturesStagesNotInited_650D54 = true;
    static DWORD sTextureStageStates_dword_6C0B78[8][25] = {};

    if (sTexturesStagesNotInited_650D54)
    {
        for (DWORD stageNum = 0; stageNum < 8; ++stageNum)
        {
            gD3dDevice_6FC74C->SetTextureStageState(stageNum, D3DTSS_COLOROP, 1);
            gD3dDevice_6FC74C->SetTextureStageState(stageNum, D3DTSS_ALPHAOP, 1);
        }

        for (DWORD type1 = 0; type1 < 25; ++type1)
        {
            for (DWORD stageNum2 = 0; stageNum2 < 8; ++stageNum2)
            {
                gD3dDevice_6FC74C->GetTextureStageState(stageNum2, static_cast<D3DTEXTURESTAGESTATETYPE>(type1), &sTextureStageStates_dword_6C0B78[stageNum2][type1]);
            }
        }

        sTexturesStagesNotInited_650D54 = false;
    }

    HRESULT hr = S_OK;

    if (type2 >= 25 || stage >= 8)
    {
        hr = gD3dDevice_6FC74C->SetTextureStageState(stage, type2, v);
    }
    else if (sTextureStageStates_dword_6C0B78[stage][type2] != v)
    {
        hr = gD3dDevice_6FC74C->SetTextureStageState(stage, type2, v);
        gD3dDevice_6FC74C->GetTextureStageState(stage, type2, &sTextureStageStates_dword_6C0B78[stage][type2]);
    }
    return hr;
}
MGS_FUNC_IMPLEX(0x422BC0, Render_InitTextureStages_422BC0, RENDERER_IMPL);

bool CC ClearDDSurfaceWhite_41E990()
{
    DDBLTFX bltFX = {};
    bltFX.dwSize = sizeof(DDBLTFX);
    bltFX.dwFillColor = 0xFFFF;
    HRESULT hr;
    do
    {
        hr = g_pDDSurface_6FC740->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &bltFX);
    } while (hr == DDERR_WASSTILLDRAWING);
    return hr == S_OK;
}
MGS_FUNC_IMPLEX(0x41E990, ClearDDSurfaceWhite_41E990, RENDERER_IMPL);

HRESULT CC Render_SetTexture_41E9E0()
{
    if (g_pDDSurface_6FC740)
    {
        if (g_pDDSurface_6FC740->IsLost() == DDERR_SURFACELOST)
        {
            g_pDDSurface_6FC740->Restore();
            ClearDDSurfaceWhite_41E990();
        }
        return gD3dDevice_6FC74C->SetTexture(0, g_pDDSurface_6FC740);
    }
    return gD3dDevice_6FC74C->SetTexture(0, nullptr);
}

void CC Render_DrawHardware_helper_4233C0()
{
    if (dword_6FC760)
    {
        memset(&g_pMGSVertices_6FC780[g_nVertexOffset_6FC784], 0, sizeof(MGSVertex) * 6);
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 5].w = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 4].w = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 3].w = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 2].w = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 1].w = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 0].w = 1.0f;

        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 5].diffuse = 0xFFFFFF;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 4].diffuse = 0xFFFFFF;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 3].diffuse = 0xFFFFFF;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 2].diffuse = 0xFFFFFF;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 1].diffuse = 0xFFFFFF;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 0].diffuse = 0xFFFFFF;

        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 0].x = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 0].y = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 0].u = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 0].v = 0.0f;

        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 1].x = (float)gTextures_6C0F00[word_6FC764].field_10_x;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 1].y = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 1].u = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 1].v = 0.0f;

        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 2].x = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 2].y = (float)gTextures_6C0F00[word_6FC764].field_12_y;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 2].u = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 2].v = 0x3F800000;

        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 3].x = (float)gTextures_6C0F00[word_6FC764].field_10_x;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 3].y = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 3].u = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 3].v = 0.0f;

        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 4].x = (float)gTextures_6C0F00[word_6FC764].field_10_x;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 4].y = (float)gTextures_6C0F00[word_6FC764].field_12_y;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 4].u = 1.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 4].v = 1.0f;

        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 5].x = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 5].y = (float)gTextures_6C0F00[word_6FC764].field_12_y;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 5].u = 0.0f;
        g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 + 5].v = 1.0f;

        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 6;
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = 1;
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = 4;
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788++].nTextureIndex = word_6FC764;
        g_nVertexOffset_6FC784 += 6;
    }
}
MGS_FUNC_IMPLEX(0x4233C0, Render_DrawHardware_helper_4233C0, RENDERER_IMPL);

void CC Render_DrawHardware_421C00()
{
    for (DWORD i = 0u; i < g_nVertexOffset_6FC784; ++i)
    {
        g_pMGSVertices_6FC780[i].x = (g_pMGSVertices_6FC780[i].x - (float)gDisp_w_word_6FC7B0) * gXRes;
        g_pMGSVertices_6FC780[i].y = (g_pMGSVertices_6FC780[i].y - (float)gDisp_y_word_6FC7B2) * gXRes;
    }

    Render_DrawHardware_helper_4233C0();

    DWORD primSubIdx = 0;
    DWORD pPrimStartIdx = 0;

    while (primSubIdx < gPrimIdx_dword_6FC788)
    {
        if (gPrimBuffer_dword_6C0EFC[primSubIdx].mPrimTypeQ == 2000)
        {
           
            D3DVIEWPORT7 viewPort = {};

            if (g_pMGSVertices_6FC780[pPrimStartIdx].x < 0.0f)
            {
                g_pMGSVertices_6FC780[pPrimStartIdx].x = 0.0f;
            }

            if (g_pMGSVertices_6FC780[pPrimStartIdx].y < 0.0f)
            {
                g_pMGSVertices_6FC780[pPrimStartIdx].y = 0.0f;
            }

            if (gXRes * 320.0f - 1.0f <= g_pMGSVertices_6FC780[pPrimStartIdx].x)
            {
                g_pMGSVertices_6FC780[pPrimStartIdx].x = 0.0f;
            }
            if (gXRes * 224.0f - 1.0f <= g_pMGSVertices_6FC780[pPrimStartIdx].y)
            {
                g_pMGSVertices_6FC780[pPrimStartIdx].y = 0.0f;
            }

           
            viewPort.dwX = static_cast<DWORD>(g_pMGSVertices_6FC780[pPrimStartIdx].x);

            viewPort.dwY = static_cast<DWORD>(g_pMGSVertices_6FC780[pPrimStartIdx].y);
            viewPort.dwWidth = static_cast<DWORD>(gXRes * g_pMGSVertices_6FC780[pPrimStartIdx].u);
            viewPort.dwHeight = static_cast<DWORD>(gXRes * g_pMGSVertices_6FC780[pPrimStartIdx].v);

            viewPort.dvMinZ = 1.0f;
            viewPort.dvMaxZ = 1.0f;

            // TODO: Real func has dead branches here?
         
            const HRESULT hResult = gD3dDevice_6FC74C->SetViewport(&viewPort);
            if (FAILED(hResult))
            {
                PrintDDError("Can't set viewport", hResult);
            }
        }
        else
        {
            
            if (gPrimBuffer_dword_6C0EFC[primSubIdx].nBlendMode & 0x8000)
            {
                gPrimBuffer_dword_6C0EFC[primSubIdx].nBlendMode &= 0x7FFFu;
                Render_InitTextureStages_422BC0(0, D3DTSS_MAGFILTER, D3DTFG_POINT);
                Render_InitTextureStages_422BC0(0, D3DTSS_MINFILTER, D3DTFG_POINT);
            }
            else if (gNoFilter)
            {
                Render_InitTextureStages_422BC0(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
                Render_InitTextureStages_422BC0(0, D3DTSS_MINFILTER, D3DTFG_LINEAR);
            }

            Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHAREF, 127);

            int textureIdx = gPrimBuffer_dword_6C0EFC[primSubIdx].nTextureIndex;
            if (textureIdx >= gNumTextures_word_6FC78C || gTextures_6C0F00[textureIdx].mSurfaceType != 5)
            {
                if (textureIdx == 0xFFFE)
                {
                    if (gNoEffects)
                    {
                        textureIdx = 0;
                        Render_sub_421280(&g_pMGSVertices_6FC780[pPrimStartIdx], primSubIdx);
                    }
                    else
                    {
                        gPrimBuffer_dword_6C0EFC[primSubIdx].nBlendMode = 4;
                    }
                }
                else if (!gStageIs_s11e_6FC778)
                {
                    dword_650D44 = 1;
                }

                if (textureIdx < gNumTextures_word_6FC78C && gTextures_6C0F00[textureIdx].mSurface)
                {
                    Render_BlendMode_sub_421800(
                        gTextures_6C0F00[textureIdx].field_28 & gPrimBuffer_dword_6C0EFC[primSubIdx].nBlendMode,
                        &g_pMGSVertices_6FC780[pPrimStartIdx],
                        gPrimBuffer_dword_6C0EFC[primSubIdx].dwVertexCount,
                        primSubIdx);
                }
                else
                {
                    Render_BlendMode_sub_421800(
                        gPrimBuffer_dword_6C0EFC[primSubIdx].nBlendMode,
                        &g_pMGSVertices_6FC780[pPrimStartIdx],
                        gPrimBuffer_dword_6C0EFC[primSubIdx].dwVertexCount,
                        primSubIdx);
                }

                if (textureIdx >= gNumTextures_word_6FC78C)
                {
                    Render_InitTextureStages_422BC0(0, D3DTSS_COLOROP, 4);
                    Render_SetTexture_41E9E0();
                }
                else
                {
                    gD3dDevice_6FC74C->SetTexture(0, gTextures_6C0F00[textureIdx].mSurface);
                    if (gModX2)
                    {
                        Render_InitTextureStages_422BC0(0, D3DTSS_COLOROP, 5);
                    }
                    else
                    {
                        Render_InitTextureStages_422BC0(0, D3DTSS_COLOROP, 4);
                    }
                }
                

                Render_SetRenderState_422A90(D3DRENDERSTATE_SHADEMODE, gPrimBuffer_dword_6C0EFC[primSubIdx].mShadeMode);
                
                if (textureIdx == 0xFFFD)
                {
                    Render_sub_420840(&primSubIdx, &pPrimStartIdx);
                    gPrimBuffer_dword_6C0EFC[primSubIdx].nBlendMode = 5;
                }

                if (dword_650D10)
                {
                    const HRESULT hResult = gD3dDevice_6FC74C->DrawPrimitive(
                        (D3DPRIMITIVETYPE)gPrimBuffer_dword_6C0EFC[primSubIdx].mPrimTypeQ,
                        0x1C4,                              // vertex type desc
                        &g_pMGSVertices_6FC780[pPrimStartIdx],// lpvVertices
                        gPrimBuffer_dword_6C0EFC[primSubIdx].dwVertexCount,
                        0);                                 // flags

                    if (FAILED(hResult))
                    {
                        PrintDDError("Can't render primitives", hResult);
                    }

                    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
                }
            }
            else
            {
                Render_DrawTextBeginScene_424020(gTextures_6C0F00[textureIdx].mSurface, &g_pMGSVertices_6FC780[pPrimStartIdx]);
            }
        }
        pPrimStartIdx += gPrimBuffer_dword_6C0EFC[primSubIdx++].dwVertexCount;
    }

    gD3dDevice_6FC74C->SetTexture(0, 0);
    gPrimIdx_dword_6FC788 = 0;
    g_nVertexOffset_6FC784 = 0;
    dword_6FC744 = 0;
}
MGS_FUNC_IMPLEX(0x421C00, Render_DrawHardware_421C00, RENDERER_IMPL);

// TODO: Assert sizes
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

struct TaggedOrderingTablePointer
{
    uint8_t mPointerB0;
    uint8_t mPointerB1;
    uint8_t mPointerB2;
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

struct StructVertType1 // TODO: Poly_FT4 ??
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
    MGSLargeVertDif DifVtx[4];
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

struct VertsBlock
{
    TaggedOrderingTablePointer header;
    uint8_t padding[0x3C];
};
static_assert(sizeof(VertsBlock) == 0x40, "VertsBlock must be of size 0x40");

struct PrimitivesChain // TODO: same as struct_gv ??
{
    TaggedOrderingTablePointer* pStructVerts0[2];
    uint8_t nNumStructs;
    uint8_t padding0;
    uint8_t padding1;
    uint8_t padding2;
    uint16_t fieldC;
    uint8_t fieldE[0x4E];
    uint32_t field5C;
    uint32_t field60;
    uint32_t field64;
    uint32_t field68;
    VertsBlock vertBlock0[2];
    VertsBlock vertBlock1[2];
    VertsBlock vertBlock2[2];
};
static_assert(sizeof(PrimitivesChain) == 0x1EC, "PrimitivesChain must be of size 0x1EC");


static void convertVertexType0(StructVertType0* pStructVert, uint32_t nIndex)
{
    int32_t signedX, signedY;
    signedX = pStructVert->Vtxs[nIndex].x << 20;
    signedX >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->Vtxs[nIndex].y << 20;
    signedY >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = (float)signedY + g_fYOffset;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = diffuseColor;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = 1.0f;
    g_nVertexOffset_6FC784++;
}

static void convertColorWZType0(StructVertType0* pStructVert)
{
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = diffuseColor;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = 1.0f;
    g_nVertexOffset_6FC784++;
}

static void convertVertexType1(StructVertType1* pStructVert, uint32_t nIndex, float u, float v)
{
    int32_t signedX, signedY;
    signedX = pStructVert->TexVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->TexVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = (float)signedY + g_fYOffset;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = 0.0f;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = u;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = v;

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

    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = diffuseColor;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = 1.0f;
    g_nVertexOffset_6FC784++;
}

static void convertExceptPosType1(StructVertType1* pStructVert, float u, float v)
{
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = 0.0f;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = u;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = v;

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

    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = diffuseColor;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = 1.0f;
    g_nVertexOffset_6FC784++;
}

static void convertVertexType2(StructVertType2* pStructVert, uint32_t nIndex)
{
    int32_t signedX, signedY;
    signedX = pStructVert->DifVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->DifVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = (float)signedY + g_fYOffset;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->DifVtx[nIndex].diffuseR << 16) | (pStructVert->DifVtx[nIndex].diffuseG << 8) | (pStructVert->DifVtx[nIndex].diffuseB);
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = diffuseColor;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = 1.0f;
    g_nVertexOffset_6FC784++;
}

static void convertVertexType3(StructVertType3* pStructVert, uint32_t nIndex, float u, float v)
{
    int32_t signedX, signedY;
    signedX = pStructVert->DifVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->DifVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = (float)signedY + g_fYOffset;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = 0.0f;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = u;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = v;

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

    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = diffuseColor;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = 1.0f;
    g_nVertexOffset_6FC784++;
}

static float convertPositionFloat(WORD n)
{
    int32_t signedN;
    signedN = n << 20;
    signedN >>= 20;
    return (float)signedN;
}

static uint32_t calculateModX2Diffuse(uint32_t diffuseR, uint32_t diffuseG, uint32_t diffuseB)
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


static void handleBlendMode(uint16_t nBlend)
{
    if ((otItemType_dword_791C54 & 2) != 0)
    {
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nBlendMode = 1 + ((nBlend >> 5) & 3);
    }
    else
    {
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nBlendMode = 0;
    }
}

static void handleBlendMode(uint16_t nBlend, uint16_t offset)
{
    if ((otItemType_dword_791C54 & 2) != 0)
    {
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nBlendMode = 1 + offset + ((nBlend >> 5) & 3);
    }
    else
    {
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nBlendMode = offset;
    }
}

static void convertVertexType4(StructVertType4* pStructVert, uint32_t nIndex, float u, float v)
{
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = pStructVert->Vtx[nIndex].Vtx.x + g_fXOffset;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = pStructVert->Vtx[nIndex].Vtx.y + g_fYOffset;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = 0.0f;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = u;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = v;
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = calculateModX2Diffuse(pStructVert->diffuseR, pStructVert->diffuseG, pStructVert->diffuseB);
    g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = 1.0f;
    g_nVertexOffset_6FC784++;
}

int CC ConvertPolys_Hardware(TaggedOrderingTablePointer* otItem, int otItemSize)
{
    uint32_t var14 = dword_688CD4;
    uint32_t var1C = dword_688CD0;

    for (;;)
    {
        if (otItemSize <= 0)
        {
            return 1;
        }

        otItemType_dword_791C54 = otItem->structType;
        size_dword_791C58 = 0;
        g_fV3 = g_fV2 = g_fV1 = g_fV0 = 0;
        g_fU3 = g_fU2 = g_fU1 = g_fU0 = 0;

        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 0;
        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nBlendMode = 0;

        // 100-103 case has an issue, causes corrupted text
        //LOG_INFO("VTX type: " << dword_791C54);

        switch (otItemType_dword_791C54)
        {
        case 0:
        {
            return 1;
        }

        case 32: // monochrome 3 point polygon
        case 33:
        case 34:
        case 35:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 3;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLELIST;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 4;
            break;
        }

        case 40: // monchrome 4 point polygon
        case 41:
        case 42:
        case 43:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);
            convertVertexType0(pStructVert, 3);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 5;
            break;
        }

        case 44:
        case 45:
        case 46:
        case 47:
        {
            StructVertType1* pStructVert = (StructVertType1*)otItem;
            uint32_t TextureIdx0, TextureIdx1;
            Render_ComputeTextureIdx_40CC50(pStructVert->TexVtx[1].textureIdx, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1);
            TextureIdx0 &= 0xFFFF;
            TextureIdx1 &= 0xFFFF;

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = g_pwTextureIndices_6FC72C[TextureIdx1 * 0x400 + TextureIdx0];
            g_nTextureIndex = gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex;

            if (g_nTextureIndex >= gNumTextures_word_6FC78C)
            {
                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0;
            }
            else
            {
                const uint32_t texturePage = (pStructVert->TexVtx[1].textureIdx & 0x180) >> 7;
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &g_fU0, &g_fV0);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[1].u, pStructVert->TexVtx[1].v, &g_fU1, &g_fV1);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[2].u, pStructVert->TexVtx[2].v, &g_fU2, &g_fV2);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[3].u, pStructVert->TexVtx[3].v, &g_fU3, &g_fV3);
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->TexVtx[1].textureIdx);

            convertVertexType1(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType1(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType1(pStructVert, 2, g_fU2, g_fV2);
            convertVertexType1(pStructVert, 3, g_fU3, g_fV3);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 9;
            break;
        }

        case 48:
        case 49:
        case 50:
        case 51:
        {
            StructVertType2* pStructVert = (StructVertType2*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType2(pStructVert, 0);
            convertVertexType2(pStructVert, 1);
            convertVertexType2(pStructVert, 2);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 3;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_GOURAUD;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLELIST;

            size_dword_791C58 = 6;
            break;
        }

        case 52:
        case 53:
        case 54:
        case 55:
        {
            StructVertType3* pStructVert = (StructVertType3*)otItem;

            uint32_t TextureIdx0, TextureIdx1;
            Render_ComputeTextureIdx_40CC50(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &TextureIdx0, &TextureIdx1);
            TextureIdx0 &= 0xFFFF;
            TextureIdx1 &= 0xFFFF;

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = g_pwTextureIndices_6FC72C[TextureIdx1 * 0x400 + TextureIdx0];
            g_nTextureIndex = gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex;

            if (g_nTextureIndex >= gNumTextures_word_6FC78C)
            {
                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0;
            }
            else
            {
                const uint32_t texturePage = (pStructVert->DifVtx[1].textureIdx & 0x180) >> 7;
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &g_fU0, &g_fV0);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &g_fU1, &g_fV1);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &g_fU2, &g_fV2);
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->DifVtx[1].textureIdx);

            convertVertexType3(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType3(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType3(pStructVert, 2, g_fU2, g_fV2);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 3;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_GOURAUD;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLELIST;

            size_dword_791C58 = 9;
        }

        case 56:
        case 57:
        case 58:
        case 59:
        {
            StructVertType2* pStructVert = (StructVertType2*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            if (gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nBlendMode == 3)
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

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_GOURAUD;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            if (gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nBlendMode == 3 && dword_6FC774 != 0)
            {
                convertVertexType2(pStructVert, 0);
                convertVertexType2(pStructVert, 1);
                convertVertexType2(pStructVert, 2);
                convertVertexType2(pStructVert, 3);

                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount += 4;
            }

            size_dword_791C58 = 8;
            break;
        }

        case 60:
        case 61:
        case 62:
        case 63:
        {
            StructVertType3* pStructVert = (StructVertType3*)otItem;
            float position[16];
            position[11] = 0.5f;
            position[10] = 0.5f;
            position[9] = 0.5f;
            position[8] = 0.5f;
            position[15] = 1.0f;
            position[14] = 0.999999f;
            position[13] = 0.999999f;
            position[12] = 0.999999f;

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
            if (Render_ComputeTextureIdx_40CC50(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &TextureIdx0, &TextureIdx1) != 0)
            {
                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFE;
                g_fU0 = (float)(TextureIdx0 & 0xFFFF);
                g_fV0 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx_40CC50(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &TextureIdx0, &TextureIdx1);
                g_fU1 = (float)(TextureIdx0 & 0xFFFF);
                g_fV1 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx_40CC50(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &TextureIdx0, &TextureIdx1);
                g_fU2 = (float)(TextureIdx0 & 0xFFFF);
                g_fV2 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx_40CC50(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[3].u, pStructVert->DifVtx[3].v, &TextureIdx0, &TextureIdx1);
                g_fU3 = (float)(TextureIdx0 & 0xFFFF);
                g_fV3 = (float)(TextureIdx1 & 0xFFFF);
            }
            else
            {
                TextureIdx0 &= 0xFFFF;
                TextureIdx1 &= 0xFFFF;

                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = g_pwTextureIndices_6FC72C[TextureIdx1 * 0x400 + TextureIdx0];
                g_nTextureIndex = gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex;

                if (g_nTextureIndex >= gNumTextures_word_6FC78C)
                {
                    gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0;
                }
                else
                {
                    const uint32_t texturePage = (pStructVert->DifVtx[1].textureIdx & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[3].u, pStructVert->DifVtx[3].v, &g_fU3, &g_fV3);

                    uint16_t* pIndex = (uint16_t*)(0x6FC728 + ((pStructVert->DifVtx[0].textureIdx >> 6) << 11) + ((pStructVert->DifVtx[0].textureIdx & 0x3F) << 5));
                    if (*pIndex == 0xEDED)
                    {
                        gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFF0;
                        otItemType_dword_791C54 &= 0xFFFFFFFD;
                    }
                }
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->DifVtx[1].textureIdx);

            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = position[3] + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = position[7] + g_fYOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = position[11];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = position[15];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = g_fU0;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = g_fV0;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[0].diffuseR, pStructVert->DifVtx[0].diffuseG, pStructVert->DifVtx[0].diffuseB);
            g_nVertexOffset_6FC784++;

            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = position[2] + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = position[6] + g_fYOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = position[10];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = position[14];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = g_fU1;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = g_fV1;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[1].diffuseR, pStructVert->DifVtx[1].diffuseG, pStructVert->DifVtx[1].diffuseB);
            g_nVertexOffset_6FC784++;

            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = position[1] + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = position[5] + g_fYOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = position[9];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = position[13];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = g_fU2;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = g_fV2;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[2].diffuseR, pStructVert->DifVtx[2].diffuseG, pStructVert->DifVtx[2].diffuseB);
            g_nVertexOffset_6FC784++;

            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = position[0] + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = position[4] + g_fYOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].z = position[8];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].w = position[12];
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = g_fU3;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = g_fV3;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[3].diffuseR, pStructVert->DifVtx[3].diffuseG, pStructVert->DifVtx[3].diffuseB);
            g_nVertexOffset_6FC784++;

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_GOURAUD;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 0xC;
            break;
        }

        case 64:
        case 65:
        case 66:
        case 67:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            float fXSize, fYSize;
            sub_40FF20(pStructVert->Vtxs[0].x, pStructVert->Vtxs[0].y, pStructVert->Vtxs[1].x, pStructVert->Vtxs[1].y, &fXSize, &fYSize);
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 0);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 - 1].x += fXSize;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 - 1].y += fYSize;
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 1);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 - 1].x += fXSize;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784 - 1].y += fYSize;

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 3;
            break;
        }

        case 72:
        case 73:
        case 74:
        case 75:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 3;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_LINESTRIP;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 5;
            break;
        }

        case 76:
        case 77:
        case 78:
        case 79:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);
            convertVertexType0(pStructVert, 3);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_LINESTRIP;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 6;
            break;
        }

        case 80:
        case 81:
        case 82:
        case 83:
        {
            StructVertType2* pStructVert = (StructVertType2*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType2(pStructVert, 0);
            convertVertexType2(pStructVert, 1);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 2;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_GOURAUD;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_LINELIST;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 4;
            break;
        }

        case 96:
        case 97:
        case 98:
        case 99:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;

            float fSecondX = convertPositionFloat(pStructVert->Vtxs[0].x) + (float)((int16_t)pStructVert->Vtxs[1].x);
            float fSecondY = convertPositionFloat(pStructVert->Vtxs[0].y) + (float)((int16_t)pStructVert->Vtxs[1].y);

            handleBlendMode(word_6C0EAC);
            convertVertexType0(pStructVert, 0);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = fSecondX + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = convertPositionFloat(pStructVert->Vtxs[0].y) + g_fYOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = convertPositionFloat(pStructVert->Vtxs[0].x) + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = fSecondY + g_fYOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = fSecondX + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = fSecondY + g_fYOffset;
            convertColorWZType0(pStructVert);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 3;
            break;
        }

        case 100:
        case 101:
        case 102:
        case 103:
        {
            StructVertType1* pStructVert = (StructVertType1*)otItem;
            float fInverseRes = 1.0f / gXRes;

            int16_t diffX = (int16_t)pStructVert->TexVtx[1].Vtx.x;
            int16_t diffY = (int16_t)pStructVert->TexVtx[1].Vtx.y;
            float fSecondX = convertPositionFloat(pStructVert->TexVtx[0].Vtx.x) + (float)diffX;
            float fSecondY = convertPositionFloat(pStructVert->TexVtx[0].Vtx.y) + (float)diffY;

            uint32_t TextureIdx0, TextureIdx1;
            if (Render_ComputeTextureIdx_40CC50(word_6C0EAC, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1) != 0)
            {
                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFD;
                g_fU0 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV0 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx_40CC50(word_6C0EAC, pStructVert->TexVtx[0].u + diffX - 1, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1);
                g_fU1 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV1 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx_40CC50(word_6C0EAC, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v + diffY, &TextureIdx0, &TextureIdx1);
                g_fU2 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV2 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx_40CC50(word_6C0EAC, pStructVert->TexVtx[0].u + diffX - 1, pStructVert->TexVtx[0].v + diffY, &TextureIdx0, &TextureIdx1);
                g_fU3 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV3 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;
            }
            else
            {
                if (pStructVert->TexVtx[0].textureIdx & 0x8000)
                {
                    gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 1 + (pStructVert->TexVtx[0].textureIdx & 0xF);
                    g_nTextureIndex = gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex;
                    pStructVert->TexVtx[0].u = 0;
                    pStructVert->TexVtx[0].v = 0;
                }
                else
                {
                    TextureIdx0 &= 0xFFFF;
                    TextureIdx1 &= 0xFFFF;

                    gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = g_pwTextureIndices_6FC72C[TextureIdx1 * 0x400 + TextureIdx0];
                    g_nTextureIndex = gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex;
                }
                if (g_nTextureIndex >= gNumTextures_word_6FC78C)
                {
                    gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0;
                }
                else
                {
                    const uint32_t texturePage = (word_6C0EAC & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u + diffX, pStructVert->TexVtx[0].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v + diffY, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u + diffX, pStructVert->TexVtx[0].v + diffY, &g_fU3, &g_fV3);
                }
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC, 0x8000);

            convertVertexType1(pStructVert, 0, g_fU0, g_fV0);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = fSecondX + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = convertPositionFloat(pStructVert->TexVtx[0].Vtx.y) + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU1, g_fV1);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = convertPositionFloat(pStructVert->TexVtx[0].Vtx.x) + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = fSecondY + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU2, g_fV2);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = fSecondX + g_fXOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = fSecondY + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU3, g_fV3);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 4;
            break;
        }

        case 104:
        case 105:
        case 106:
        case 107:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            float fHalfOffset = (((gXRes - 1.0f) / 2.0f) + 1.0f) / gXRes;

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            float centerX = convertPositionFloat(pStructVert->Vtxs[0].x);
            float centerY = convertPositionFloat(pStructVert->Vtxs[0].y);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = centerX + g_fXOffset - fHalfOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = centerY + g_fYOffset - fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = centerX + g_fXOffset + fHalfOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = centerY + g_fYOffset - fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = centerX + g_fXOffset - fHalfOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = centerY + g_fYOffset + fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = centerX + g_fXOffset + fHalfOffset;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = centerY + g_fYOffset + fHalfOffset;
            convertColorWZType0(pStructVert);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 2;
            break;
        }

        case 128: // $80     move image in frame buffer
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            int16_t rawPos[4];

            // source coord
            rawPos[1] = pStructVert->Vtxs[0].y;
            rawPos[0] = pStructVert->Vtxs[0].x;

            // dest coord
            int16_t varDC = pStructVert->Vtxs[1].y;
            int16_t varD8 = pStructVert->Vtxs[1].x;

            // w/h to xfer
            rawPos[3] = pStructVert->Vtxs[2].y;
            rawPos[2] = pStructVert->Vtxs[2].x;

            // move image
            sub_40D540(rawPos, varD8, varDC);

            size_dword_791C58 = 4;
            break;
        }

        case 144:
        case 145:
        case 146:
        case 147:
        {
            StructVertType4* pStructVert = (StructVertType4*)otItem;
            if ((pStructVert->Vtx[0].textureIdx & 0x8000) != 0)
            {
                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFF0;
            }
            else
            {
                uint32_t TextureIdx0, TextureIdx1;
                Render_ComputeTextureIdx_40CC50(pStructVert->Vtx[1].textureIdx, pStructVert->Vtx[0].u, pStructVert->Vtx[0].v, &TextureIdx0, &TextureIdx1);
                TextureIdx0 &= 0xFFFF;
                TextureIdx1 &= 0xFFFF;
                gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = g_pwTextureIndices_6FC72C[TextureIdx1 * 0x400 + TextureIdx0];
                g_nTextureIndex = gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex;

                if (g_nTextureIndex >= gNumTextures_word_6FC78C)
                {
                    gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0;
                }
                else
                {
                    const uint32_t texturePage = (pStructVert->Vtx[1].textureIdx & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[0].u, pStructVert->Vtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[1].u, pStructVert->Vtx[1].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[2].u, pStructVert->Vtx[2].v, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[3].u, pStructVert->Vtx[3].v, &g_fU3, &g_fV3);
                }
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType4(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType4(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType4(pStructVert, 2, g_fU2, g_fV2);
            convertVertexType4(pStructVert, 3, g_fU3, g_fV3);

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 4;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 0xD;
            break;
        }

        case 225:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            word_6C0EAC = pStructVert->field0 & 0x1FF;
            word_6C0EAE = (((pStructVert->field0 >> 10) & 1) << 8) | ((pStructVert->field0 >> 9) & 1);

            size_dword_791C58 = 1;
            break;
        }

        case 226:
        {
            size_dword_791C58 = 1;
            break;
        }

        case 227:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            gDrawEnv_6C0E98.clip.x1 = pStructVert->field0 & 0x3FF;
            word_6C0E9A = (pStructVert->field0 >> 10) & 0x3FF;

            size_dword_791C58 = 1;
            break;
        }

        case 228:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            word_6C0E9C = (pStructVert->field0 & 0x3FF) - gDrawEnv_6C0E98.clip.x1 + 1;
            word_6C0E9E = ((pStructVert->field0 >> 10) & 0x3FF) - word_6C0E9A + 1;

            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].x = (float)gDrawEnv_6C0E98.clip.x1;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].y = (float)word_6C0E9A;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].u = (float)word_6C0E9C;
            g_pMGSVertices_6FC780[g_nVertexOffset_6FC784].v = (float)word_6C0E9E;
            g_nVertexOffset_6FC784++;

            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount = 1;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mShadeMode = D3DSHADE_FLAT;
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].mPrimTypeQ = 0x7D0; // ?
            gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 1;
            break;
        }

        case 229:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            g_wXOffset = pStructVert->field0 & 0x7FF;
            g_wYOffset = (pStructVert->field0 >> 11) & 0x3FF;

            size_dword_791C58 = 1;
            break;
        }

        case 255:
        {
            size_dword_791C58 = 1;
            break;
        }

        default:
            break;
        }

        if (gPrimBuffer_dword_6C0EFC[gPrimIdx_dword_6FC788].dwVertexCount != 0)
        {
            gPrimIdx_dword_6FC788++;
        }

        if (size_dword_791C58 == 0)
        {
            // = 0x650A5C byte[256] ?
            uint8_t* pValue = (uint8_t*)(0x650A5C + otItemType_dword_791C54);
            size_dword_791C58 = *pValue;
        }

        if (size_dword_791C58 == 0)
        {
            size_dword_791C58 = 1;
        }

        otItemSize -= size_dword_791C58;
        otItem = (TaggedOrderingTablePointer*)((intptr_t)otItem + size_dword_791C58 * 4);
    }
}
MGS_FUNC_IMPLEX(0x410560, ConvertPolys_Hardware, RENDERER_IMPL);

int CC Renderer_ClearOTag(DWORD* ot, int otSize)
{
    if (otSize - 1 <= 0)
    {
        // As we only have 1 item, set the start of the table to be the end marker
        *ot = OT_END_TAG;
    }
    else
    {
        // Get a pointer to the last item
        DWORD* pOTItem = &ot[otSize - 1];
        int count = otSize - 1;
        do
        {
            // Set the current item to point to the previous item
            *pOTItem = reinterpret_cast<DWORD>(pOTItem - 1);
            if (reinterpret_cast<DWORD>(pOTItem) & 0xFF000000)
            {
                printf(
                    "\n"
                    "\n"
                    "***** ERROR: ClearOTag() found a pointer which uses more than 24 bit *****\n"
                    " Invalid pointer value caused overflow [%x]\n"
                    "\n",
                    *pOTItem);
            }
            --pOTItem;
            --count;
        } while (count);

        // Set the first item to the end marker
        *ot = OT_END_TAG;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0044AB80, Renderer_ClearOTag, RENDERER_IMPL);

void CC Render_DrawGeneric_4103B0(TaggedOrderingTablePointer* a_pStructVert)
{
    if (dword_6FC718 == 1)
    {
        dword_6FC718 = 0;
         gSkipFrame_dword_6FC720 = 1;
        return;
    }

    dword_6FC718 = 0;
    if ( gSkipFrame_dword_6FC720 == 0)
    {
        if (gSoftwareRendering != 0)
        {
            DDSURFACEDESC2 desc;
            memset(&desc, 0, sizeof(DDSURFACEDESC2));
            desc.dwSize = sizeof(DDSURFACEDESC2);
            g_pBackBuffer_6FC738->Lock(NULL, &desc, 0, 0);
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

            // Get the pointer bytes of the OT, the remainder byte is the type.. TODO - how does the vertex info fit into the OT?
            uint32_t nextStructVert = ((uint32_t*)a_pStructVert)[0] & 0x00FFFFFF;
            a_pStructVert = (TaggedOrderingTablePointer*)nextStructVert;
        } while ((uint32_t)a_pStructVert != OT_END_TAG);

        if (gSoftwareRendering != 0)
        {
            g_pBackBuffer_6FC738->Unlock(0);
        }
        if (gSoftwareRendering == 0)
        {
            Render_DrawHardware_421C00();
        }
    }
    if (gSoftwareRendering == 0)
    {
        Render_FreeSurfaceStack_51DE0A();
    }
}
MGS_FUNC_IMPLEX(0x4103B0, Render_DrawGeneric_4103B0, RENDERER_IMPL);

void CC Render_DrawIndex_401619(uint32_t activeBuffer)
{
    TaggedOrderingTablePointer* pStructVert = (TaggedOrderingTablePointer*)&gLibGvStruct0_6BC180.dword_6BC3D8_dst[16 * activeBuffer];
    Render_DrawGeneric_4103B0(pStructVert);
}
MGS_FUNC_IMPLEX(0x401619, Render_DrawIndex_401619, RENDERER_IMPL);

__int16 CC Render_RestoreAll()
{
    if (gPrimarySurface_6FC734->IsLost() == DDERR_SURFACELOST)
    {
        const HRESULT hr = gPrimarySurface_6FC734->Restore();
        if (FAILED(hr))
        {
            PrintDDError("Prim restore caput", hr);
        }
    }

    if (g_pBackBuffer_6FC738->IsLost() == DDERR_SURFACELOST)
    {
        const HRESULT hr = g_pBackBuffer_6FC738->Restore();
        if (FAILED(hr))
        {
            PrintDDError("Ren restore caput", hr);
        }
    }

    for (int i = 0; i < gNumTextures_word_6FC78C; i++)
    {
        if (gTextures_6C0F00[i].mSurface)
        {
            if (gTextures_6C0F00[i].mSurface->IsLost() == DDERR_SURFACELOST)
            {
                const HRESULT hr = gTextures_6C0F00[i].mSurface->Restore();
                if (FAILED(hr))
                {
                    PrintDDError("tex #%i restore caput", i);
                }
            }
        }
    }

    return gNumTextures_word_6FC78C;
}
MGS_FUNC_IMPLEX(0x0041CC30, Render_RestoreAll, RENDERER_IMPL);

HRESULT CC SetDDSurfaceTexture_41E9E0()
{
    HRESULT hr;

    if (g_pDDSurface_6FC740 != 0)
    {
        if (g_pDDSurface_6FC740->IsLost() == DDERR_SURFACELOST)
        {
            g_pDDSurface_6FC740->Restore();
            ClearDDSurfaceWhite_41E990();
        }
        hr = gD3dDevice_6FC74C->SetTexture(0, g_pDDSurface_6FC740);
    }
    else
    {
        hr = gD3dDevice_6FC74C->SetTexture(0, NULL);
    }

    return hr;
}
MGS_FUNC_IMPLEX(0x0041E9E0, SetDDSurfaceTexture_41E9E0, RENDERER_IMPL);


#define MGSVERTEX_DEF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)

int CC Render_ClearBackBuffer_41E130(uint32_t a_ClearColor, uint32_t a_DiffuseColor, uint32_t* pFirstPixel, MGSVertex* a_pVertices)
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
        hr = g_pBackBuffer_6FC738->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &bltFX);
    } while (hr == DDERR_WASSTILLDRAWING);
    if (hr != 0)
        return 0;

    a_pVertices[2].diffuse = a_DiffuseColor;
    a_pVertices[1].diffuse = a_DiffuseColor;
    a_pVertices[0].diffuse = a_DiffuseColor;

    // result stored but not used
    // happens a few times in this function, I keep it
    hr = SetDDSurfaceTexture_41E9E0();

    hr = gD3dDevice_6FC74C->BeginScene();
    if (hr != 0)
        return 0;

    hr = gD3dDevice_6FC74C->DrawPrimitive(D3DPT_TRIANGLELIST, MGSVERTEX_DEF, a_pVertices, 3, 0);
    hr = gD3dDevice_6FC74C->SetTexture(0, NULL);
    if (hr != 0)
        return 0;

    hr = gD3dDevice_6FC74C->EndScene();
    if (hr != 0)
        return 0;

    DDSURFACEDESC2 ddDesc;
    memset(&ddDesc, 0, sizeof(DDSURFACEDESC2));
    ddDesc.dwSize = sizeof(DDSURFACEDESC2);

    do {
        hr = g_pBackBuffer_6FC738->Lock(NULL, &ddDesc, 0, 0);
    } while (hr == DDERR_WASSTILLDRAWING);
    if (hr != 0)
        return 0;

    WORD wFirstPixel = ((WORD*)ddDesc.lpSurface)[0];
    g_pBackBuffer_6FC738->Unlock(NULL);

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
MGS_FUNC_IMPLEX(0x41E130, Render_ClearBackBuffer_41E130, RENDERER_IMPL);


signed int CC Render_sub_41E3C0()
{
    signed int result;

    D3DDEVICEDESC7 caps = {};
    MGSVertex pPrim[3];
    uint32_t firstPixel;

    DWORD dwNumPasses = 1;
    pPrim[0].x = 1.0f;
    pPrim[1].x = static_cast<float>(g_dwDisplayWidth_6DF214 - 1);
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

    gD3dDevice_6FC74C->GetCaps(&caps);
    const DWORD srcBlendCaps = caps.dpcTriCaps.dwSrcBlendCaps;
    const DWORD dstBlendCaps = caps.dpcTriCaps.dwDestBlendCaps;
    Render_SetRenderState_422A90(D3DRENDERSTATE_SHADEMODE, 1);
    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 1);

    if (FAILED(gD3dDevice_6FC74C->ValidateDevice(&dwNumPasses)))
    {
        gAlphaModulate_dword_6FC798 = 0;
        Render_InitTextureStages_422BC0(0, D3DTSS_ALPHAOP, 2);
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
                Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 5);
                Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 5);
                if (FAILED(gD3dDevice_6FC74C->ValidateDevice(&dwNumPasses)))
                {
                    if (gAlphaModulate_dword_6FC798)
                    {
                        Render_ClearBackBuffer_41E130(0xFF707070, 0x7F404040u, &firstPixel, pPrim);
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
                Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 5);
                Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 2);
                if (FAILED(gD3dDevice_6FC74C->ValidateDevice(&dwNumPasses)))
                {
                    if (gAlphaModulate_dword_6FC798)
                    {
                        Render_ClearBackBuffer_41E130(0xFF101010, 0x3F404040u, &firstPixel, pPrim);
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
                    Render_SetRenderState_422A90(D3DRENDERSTATE_SRCBLEND, 1);
                    Render_SetRenderState_422A90(D3DRENDERSTATE_DESTBLEND, 4);
                    Render_ClearBackBuffer_41E130(0xFFA0FFA0, 0xFF400040, &firstPixel, pPrim);

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

MGS_FUNC_IMPLEX(0x41E3C0, Render_sub_41E3C0, RENDERER_IMPL);


void CC PrintDDError(const char* errMsg, HRESULT hrErr)
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

    if (hrErr != S_OK)
    {
        for (int i = 0; i < 5; ++i)
        {
            printf("!edq %s !  %s\n", errMsg, pStrErr);
        }
    }
}
MGS_FUNC_IMPLEX(0x00422D40, PrintDDError, RENDERER_IMPL);

// Only used by Res_movie_create_helper_4562AA
void CC Renderer_GetWH_51D50D(DWORD *pWidth, DWORD *pHeight)
{
    DWORD dwHeight = 0;
    DWORD dwWidth = 0;

    if (g_pBackBuffer_6FC738)
    {
        DDSURFACEDESC2 surfaceDesc = {};
        surfaceDesc.dwSize = 0x7C;
        if (!g_pBackBuffer_6FC738->GetSurfaceDesc(&surfaceDesc))
        {
            dwWidth = surfaceDesc.dwWidth;
            dwHeight = surfaceDesc.dwHeight;
        }
    }

    if (pWidth)
    {
        *pWidth = dwWidth;
    }

    if (pHeight)
    {
        *pHeight = dwHeight;
    }
}
MGS_FUNC_IMPLEX(0x0051D50D, Renderer_GetWH_51D50D, RENDERER_IMPL);

signed int CC Render_TextureScratchAlloc_41CA80()
{
    if (gSoftwareRendering)
    {
        MGS_FATAL("Software rendering not implemented");
    }
    else
    {
        DDSURFACEDESC2 surfaceDesc = {};
        surfaceDesc.dwSize = 124;
        surfaceDesc.dwFlags = 7;
        surfaceDesc.dwWidth = 256;
        surfaceDesc.dwHeight = 256;
        surfaceDesc.ddsCaps.dwCaps = 4096;
        surfaceDesc.ddsCaps.dwCaps2 = 16;
        const HRESULT hr = g_pDirectDraw->CreateSurface(&surfaceDesc, &gTextures_6C0F00[0].mSurface, 0);
        if (FAILED(hr))
        {
            return 0;
        }
    }
    gTextures_6C0F00[0].field_6_x = 0;
    gTextures_6C0F00[0].field_4_y = 0;
    gTextures_6C0F00[0].field_8_w = 320;
    gTextures_6C0F00[0].field_A_h = 240;
    gTextures_6C0F00[0].field_12_y = 256;
    gTextures_6C0F00[0].field_10_x = 256;
    gTextures_6C0F00[0].field_24_flagsQ = 0xFFFF;
    gTextures_6C0F00[0].float_field_14_uQ = 1.0f;
    gTextures_6C0F00[0].float_field_18_vQ = 1.0f;
    gTextures_6C0F00[0].mSurfaceType = 1;
    gTextures_6C0F00[0].field_28 = 2;
    gTextures_6C0F00[0].field_2C = 0;
    ++gNumTextures_word_6FC78C;
    return 1;
}
MGS_FUNC_IMPLEX(0x0041CA80, Render_TextureScratchAlloc_41CA80, RENDERER_IMPL);

signed int CC Render_sub_41D1D0()
{
    DDPIXELFORMAT pixelFormat = {};
    pixelFormat.dwSize = 32;
    gPrimarySurface_6FC734->GetPixelFormat(&pixelFormat);
    if (pixelFormat.dwGBitMask == 0x3E0)
    {
        return 0;
    }
    return 4;
}
MGS_FUNC_IMPLEX(0x0041D1D0, Render_sub_41D1D0, RENDERER_IMPL);

signed int CC Render_sub_41E130(int fillColour, int diffuseColour, DWORD* pOutMask, MGSVertex* p3TrisVerts)
{
    Sleep(500u);

    int convertedFillColour = 0;
    if (g_surface565Mode)
    {
        convertedFillColour = ((fillColour & 0xF80000u) >> 8) | ((fillColour & 0xFC00u) >> 5) | ((fillColour & 0xF8u) >> 3);
    }
    else
    {
        convertedFillColour = ((fillColour & 0x80000000) >> 16) | ((fillColour & 0xF80000u) >> 9) | ((fillColour & 0xF800u) >> 6) | ((fillColour & 0xF8u) >> 3);
    }

    DDBLTFX bltFx = {};
    bltFx.dwSize = 100;
    bltFx.dwFillColor = convertedFillColour;
    HRESULT hr = S_OK;
    do
    {
        hr = g_pBackBuffer_6FC738->Blt(0, 0, 0, 16778240, &bltFx);
    }
    while (hr == 0x8876021C);
    
    if (hr)
    {
        return 0;
    }

    p3TrisVerts[2].diffuse = diffuseColour;
    p3TrisVerts[1].diffuse = diffuseColour;
    p3TrisVerts->diffuse = diffuseColour;
    hr = Render_SetTexture_41E9E0();
    hr = gD3dDevice_6FC74C->BeginScene();
    if (hr)
    {
        return 0;
    }

    hr = gD3dDevice_6FC74C->DrawPrimitive(D3DPT_TRIANGLELIST, 0x1C4, p3TrisVerts, 3, 0);
    hr = gD3dDevice_6FC74C->SetTexture(0, 0);
    if (hr)
    {
        return 0;
    }

    hr = gD3dDevice_6FC74C->EndScene();
    if (hr)
    {
        return 0;
    }

    DDSURFACEDESC2 ddSurfaceDesc = {};
    ddSurfaceDesc.dwSize = 124;
    do
    {
        hr = g_pBackBuffer_6FC738->Lock(0, &ddSurfaceDesc, 0, 0);
    }
    while (hr == 0x8876021C);
    
    if (hr)
    {
        return 0;
    }

    const WORD firstPixel = *(WORD *)ddSurfaceDesc.lpSurface;
    g_pBackBuffer_6FC738->Unlock(0);
    
    *pOutMask = 0;

    if (g_surface565Mode)
    {
        *pOutMask = 8 * (firstPixel & 0x1F) | 32 * (firstPixel & 0x7E0) | ((unsigned __int16)(firstPixel & 0xF800) << 8);
    }
    else
    {
        *pOutMask = 8 * (firstPixel & 0x1F) | ((firstPixel & 0x3E0) << 6) | ((firstPixel & 0x7C00) << 9);
    }
    return 1;
}

MGS_VAR(1, 0x6FC770, DWORD, gTextureCapFlags_6FC770, 0);

bool CC Render_sub_41E730()
{
    gTextureCapFlags_6FC770 = 1;

    if (!gD3dDevice_6FC74C)
    {
        return true;
    }

    D3DDEVICEDESC7 deviceCaps = {};
    deviceCaps.dpcLineCaps.dwSize = 56;
    HRESULT hr = gD3dDevice_6FC74C->GetCaps(&deviceCaps);
    gTextureCapFlags_6FC770 = deviceCaps.dpcTriCaps.dwTextureCaps & 0x20;
    
    if (gColourKey)
    {
        return true;
    }

    if (FAILED(hr))
    {
        return true;
    }

    if (!(deviceCaps.dpcTriCaps.dwAlphaCmpCaps & 0x40))
    {
        return 1;
    }

    if (!gAlphaModulate_dword_6FC798)
    {
        return 1;
    }

    Render_SetRenderState_422A90(D3DRENDERSTATE_SHADEMODE, 1);
    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHATESTENABLE, 1);
    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHAREF, 127);
    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHAFUNC, 7);

    DWORD numPasses = 1;
    if (FAILED(gD3dDevice_6FC74C->ValidateDevice(&numPasses)))
    {
        return 1;
    }

    MGSVertex tris[3];
    tris[0].x = 0.0;
    tris[1].x = (float)(g_dwDisplayWidth_6DF214 - 1);
    tris[2].x = 0.0;
    tris[0].y = 0.0;
    tris[1].y = 0.0;
    tris[2].y = (float)(g_dwDisplayHeight - 1);
    tris[0].z = 0;
    tris[1].z = 0;
    tris[2].z = 0;
    tris[0].u = 0;
    tris[1].u = 1.0f;
    tris[2].u = 0;
    tris[0].v = 0;
    tris[1].v = 0;
    tris[2].v = 1.0f;
    tris[0].w = 0.9f;
    tris[1].w = 0.9f;
    tris[2].w = 0.9f;

    DWORD resultingMask = 0;
    Render_sub_41E130(0xFF707070, 0x40404040, &resultingMask, tris);

    Render_SetRenderState_422A90(D3DRENDERSTATE_ALPHATESTENABLE, 0);

    return (BYTE)resultingMask >= 0x75u || (BYTE)resultingMask <= 0x6Bu;
}
MGS_FUNC_IMPLEX(0x0041E730, Render_sub_41E730, RENDERER_IMPL);

bool CC Renderer_Is_Texture_In_Rect_40D150(const PSX_RECT* pRect, int textureIdx)
{
    const int width =  pRect->x1 + pRect->x2 - 1;
    const int height = pRect->y1 + pRect->y2 - 1;
    const int texturey = gTextures_6C0F00[textureIdx].field_4_y;
    const int texturex = gTextures_6C0F00[textureIdx].field_6_x;
    const int texturew = texturey + gTextures_6C0F00[textureIdx].field_8_w - 1;
    const int textureh = texturex + gTextures_6C0F00[textureIdx].field_A_h - 1;
    return (pRect->x1 <= texturey && texturey <= width && pRect->y1 <= texturex && texturex <= height)
        || (pRect->x1 <= texturew && texturew <= width && pRect->y1 <= texturex && texturex <= height)
        || (pRect->x1 <= texturey && texturey <= width && pRect->y1 <= textureh && textureh <= height)
        || (pRect->x1 <= texturew && texturew <= width && pRect->y1 <= textureh && textureh <= height);
}
MGS_FUNC_IMPLEX(0x0040D150, Renderer_Is_Texture_In_Rect_40D150, RENDERER_IMPL);

MGS_VAR(1, 0x6C076C, DWORD, dword_6C076C, 0);



MGS_FUNC_NOT_IMPL(0x40E840, IDirectDrawSurface7 *__cdecl (IDirectDraw7 *, const BYTE *, const WORD *, WORD *, WORD *, int *, int), Render_sub_40E840);

int CC Render_sub_40FA30(const PSX_RECT* pRect, const WORD* pallete, const BYTE* pixelData, int surfaceType, const BYTE* pTga, unsigned __int16 tgaW, unsigned __int16 tgaH)
{
    int idx = 0;
    if (gNumFreeTextures_6FC790 > 0)
    {
        // ?? actually just gNumFreeTextures_6FC790-- ?
//        idx = *((unsigned __int16 *)&gTextures_6C0F00[1499].field_4C + (unsigned __int16)gNumFreeTextures_6FC790 + 1);
        idx = gNumFreeTextures_6FC790--;
    }
    else
    {
        idx = (unsigned __int16)gNumTextures_word_6FC78C;
    }

    WORD x2 = pRect->x2;
    WORD y2 = pRect->y2;
    gTextures_6C0F00[idx].field_8_w = x2;
    gTextures_6C0F00[idx].field_A_h = y2;
    gTextures_6C0F00[idx].field_4_y = pRect->x1;
    gTextures_6C0F00[idx].field_6_x = pRect->y1;

    // NOTE: Pruned software rendering branch
    gTextures_6C0F00[idx].mSurfaceType = surfaceType;

    if (!pallete || !pixelData || !x2 || !y2)
    {
        return 0;
    }

    if (surfaceType)
    {
        if (surfaceType == 1)
        {
            x2 *= 2;
        }
    }
    else
    {
        x2 *= 4;
    }
    gTextures_6C0F00[idx].float_field_18_vQ = 1.0f;
    gTextures_6C0F00[idx].float_field_14_uQ = 1.0f;
    gTextures_6C0F00[idx].field_C = x2;
    gTextures_6C0F00[idx].field_E = y2;
    
    if (pTga)
    {
        gTextures_6C0F00[idx].float_field_14_uQ = (float)tgaW / (float)x2;
        gTextures_6C0F00[idx].float_field_18_vQ = (float)tgaH / (float)y2;
        x2 = tgaW;
        y2 = tgaH;
        surfaceType = 3;
        pixelData = pTga;
    }

    if (surfaceType == 5)
    {
        // Type 5 is malloc 280 byte buffer?
        //gTextures_6C0F00[idx].mSurface = Render_sub_4241C2(pixelData, pallete);
    }
    // NOTE: Pruned software rendering branch
    else
    {
        gTextures_6C0F00[idx].mSurface = Render_sub_40E840(g_pDirectDraw, pixelData, pallete, &x2, &y2, &surfaceType, idx);
        gTextures_6C0F00[idx].field_28 = surfaceType;
    }

    gTextures_6C0F00[idx].field_10_x = x2;
    gTextures_6C0F00[idx].field_12_y = y2;
    gTextures_6C0F00[idx].field_24_flagsQ = dword_6C076C;

    if (gNumFreeTextures_6FC790)
    {
        --gNumFreeTextures_6FC790;
    }
    else
    {
        ++gNumTextures_word_6FC78C;
    }

    return idx;
}
MGS_FUNC_IMPLEX(0x0040FA30, Render_sub_40FA30, false); // TODO: Implement

MGS_VAR(1, 0x6C0770, DWORD, gbKeepCopyingSurface_dword_6C0770, 0);

void CC Render_set_pixel_40C870(WORD* pData, int pitch, unsigned __int16 xpos, unsigned __int16 ypos, __int16 value)
{
    if (gSoftwareRendering)
    {
        value = value & 0x1F | 2 * (value & 0x7FE0);
    }

    const DWORD offset = (xpos) + ((pitch / sizeof(WORD)) * ypos); // Divide pitch by the pixel size as it is in bytes
    pData[offset] = value;
}
MGS_FUNC_IMPLEX(0x0040C870, Render_set_pixel_40C870, RENDERER_IMPL);

__int16 CC Render_get_pixel_vram_40C8E0(const WORD* pData, int pitch, int /*unknown*/, __int16 xpos, __int16 ypos)
{
    // Limited x/y to psx vram size
    const DWORD offset = (xpos & 1023) + ((pitch / sizeof(WORD)) * (ypos & 511));
    return pData[offset];
}
MGS_FUNC_IMPLEX(0x0040C8E0, Render_get_pixel_vram_40C8E0, RENDERER_IMPL);

void CC Render_copy_to_surface_40C930(WORD* pData, int pitch, unsigned __int16 width, unsigned __int16 height)
{
    // TODO: Need to understand what this algorithm is actually doing, its not a direct copy
    if (!gSoftwareRendering)
    {
        for (int ypos = 0; ypos < height; ++ypos)
        {
            for (int xpos = 0; xpos < width; ++xpos)
            {
                __int16 readPixel1 = Render_get_pixel_vram_40C8E0(pData, pitch, 0, xpos, ypos);
                __int16 readPixel2 = 0;

                if (readPixel1)
                {
                    continue;
                }

                if (xpos - 1 >= 0)
                {
                    readPixel2 = Render_get_pixel_vram_40C8E0(pData, pitch, 0, xpos - 1, ypos);
                    if (readPixel2 & 0x8000)
                    {
                        Render_set_pixel_40C870(pData, pitch, xpos, ypos, readPixel2 & 0x7FFF);
                        continue;
                    }
                }

                if (readPixel2)
                {
                    readPixel1 = readPixel2;
                }

                __int16 readPixel2a = 0;
                __int16 readPixel3 = 0;

                if (ypos - 1 >= 0)
                {
                    readPixel3 = Render_get_pixel_vram_40C8E0(pData, pitch, 0, xpos, ypos - 1);
                    readPixel2a = readPixel3;
                    if (readPixel3 & 0x8000)
                    {
                        Render_set_pixel_40C870(pData, pitch, xpos, ypos, readPixel3 & 0x7FFF);
                        continue;
                    }
                }

                if (readPixel2a)
                {
                    readPixel1 = readPixel2a;
                }

                __int16 readPixel4 = 0;
                if (xpos + 1 != width)
                {
                    readPixel4 = Render_get_pixel_vram_40C8E0(pData, pitch, 0, xpos + 1, ypos);
                    if (readPixel4 & 0x8000)
                    {
                        Render_set_pixel_40C870(pData, pitch, xpos, ypos, readPixel4 & 0x7FFF);
                        continue;
                    }
                }


                if (readPixel4)
                {
                    readPixel1 = readPixel4;
                }

                __int16 readPixel5 = 0;
                if (ypos + 1 != height)
                {
                    readPixel5 = Render_get_pixel_vram_40C8E0(pData, pitch, 0, xpos, ypos + 1);
                    if (readPixel5 & 0x8000)
                    {
                        Render_set_pixel_40C870(pData, pitch, xpos, ypos, readPixel5 & 0x7FFF);
                        continue;
                    }
                }

                if (readPixel5)
                {
                    readPixel1 = readPixel5;
                }
                Render_set_pixel_40C870(pData, pitch, xpos, ypos, readPixel1 & 0x7FFF);
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x0040C930, Render_copy_to_surface_40C930, RENDERER_IMPL);

unsigned __int16 CC Render_convert_colour_40D420(unsigned __int16 value)
{
    if (value == 0x8000)
    {
        ++value;
    }
    if (value)
    {
        value |= 0x8000u;
    }
    return ((value & 0x7C00) >> 10) | ((value & 0x1F) << 10) | value & 0x83E0;
}
MGS_FUNC_IMPLEX(0x0040D420, Render_convert_colour_40D420, RENDERER_IMPL);

int CC Render_sub_41C640(const PSX_RECT* pRect, const WORD* pallete, const BYTE* pixelData, int surfaceType, const BYTE* pTga, unsigned __int16 tga6, unsigned __int16 tga7)
{
    while (gbKeepCopyingSurface_dword_6C0770) {}
    ++gbKeepCopyingSurface_dword_6C0770;
    const auto result = Render_sub_40FA30(pRect, pallete, pixelData, surfaceType, pTga, tga6, tga7);
    --gbKeepCopyingSurface_dword_6C0770;
    return result;
}
MGS_FUNC_IMPLEX(0x0041C640, Render_sub_41C640, RENDERER_IMPL);

MGS_VAR(1, 0x774A40, DWORD, gTotalAllocated_dword_774A40, 0);

struct SurfaceBackup
{
    IDirectDrawSurface7* field_0_dd_surface;
    BYTE* field_4_surface_pixel_buffer;
    DWORD field_8_array_size;
};
MGS_ASSERT_SIZEOF(SurfaceBackup, 0xC);

MGS_VAR(1, 0x77644C, SurfaceBackup*, gSurfacesArray_77644C, nullptr);
MGS_VAR(1, 0x776854, DWORD, gSurfaceArraySize_776854, 0);

int CC Renderer_GrowSurfaceArray_51DF8E()
{
    gSurfacesArray_77644C = (SurfaceBackup *)realloc(gSurfacesArray_77644C, sizeof(SurfaceBackup) * (gSurfaceArraySize_776854 + 3));
    if (!gSurfacesArray_77644C)
    {
        return 0;
    }
    return ++gSurfaceArraySize_776854 - 1;
}
MGS_FUNC_IMPLEX(0x0051DF8E, Renderer_GrowSurfaceArray_51DF8E, RENDERER_IMPL);

void CC Render_Restore_Single_Surface_51E11A(int idx);

void CC Render_Restore_Lost_Surfaces_51E086()
{
    if (gPrimarySurface_6FC734)
    {
        if (gPrimarySurface_6FC734->IsLost())
        {
            gPrimarySurface_6FC734->Restore();
        }
    }
    if (g_pBackBuffer_6FC738)
    {
        if (g_pBackBuffer_6FC738->IsLost())
        {
            g_pBackBuffer_6FC738->Restore();
        }
    }

    if (!gSoftwareRendering)
    {
        for (DWORD i = 0; i < gSurfaceArraySize_776854; i++)
        {
            IDirectDrawSurface7* pSurface = gSurfacesArray_77644C[i].field_0_dd_surface;
            if (pSurface)
            {
                if (pSurface->IsLost() == DDERR_SURFACELOST)
                {
                    if (!gSurfacesArray_77644C[i].field_0_dd_surface->Restore())
                    {
                        Render_Restore_Single_Surface_51E11A(i);
                    }
                }
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x0051E086, Render_Restore_Lost_Surfaces_51E086, RENDERER_IMPL);

void CC Render_BackupSurface_51DE8F(IDirectDrawSurface7* pSurface)
{
    if (pSurface)
    {
        DDSURFACEDESC2 surfaceDesc = {};
        surfaceDesc.dwSize = sizeof(DDSURFACEDESC2);
        int retryCount = 100;
        do
        {
            HRESULT hr = pSurface->Lock(0, &surfaceDesc, 17, 0);
            if (hr != DDERR_WASSTILLDRAWING)
            {
                if (!FAILED(hr))
                {
                    int height = surfaceDesc.dwHeight;
                    int rowSize = surfaceDesc.dwWidth * surfaceDesc.ddpfPixelFormat.dwRGBBitCount >> 3;
                    const int pitch = surfaceDesc.lPitch;
                    if (!rowSize)
                    {
                        rowSize = 1;
                    }
                    const int totalSize = surfaceDesc.dwHeight * rowSize;
                    BYTE* surfacePixelsPtr = (BYTE *)surfaceDesc.lpSurface;
                    BYTE* pixelBuffer = (BYTE *)malloc(surfaceDesc.dwHeight * rowSize);
                    if (pixelBuffer)
                    {
                        const int idx = Renderer_GrowSurfaceArray_51DF8E();
                        gSurfacesArray_77644C[idx].field_0_dd_surface = pSurface;
                        gSurfacesArray_77644C[idx].field_4_surface_pixel_buffer = pixelBuffer;
                        gSurfacesArray_77644C[idx].field_8_array_size = totalSize;
                        gTotalAllocated_dword_774A40 += totalSize;
                        for (int i = 0; i < height; i++)
                        {
                            memcpy(pixelBuffer, surfacePixelsPtr, rowSize);
                            pixelBuffer += rowSize;
                            surfacePixelsPtr += surfaceDesc.lPitch;
                        }
                    }
                    pSurface->Unlock(0);
                    return;
                }
                --retryCount;
            }
        } while (retryCount);
    }
}
MGS_FUNC_IMPLEX(0x0051DE8F, Render_BackupSurface_51DE8F, RENDERER_IMPL);

void CC Render_Restore_Single_Surface_51E11A(int idx)
{
    DDSURFACEDESC2 surfaceDesc = {};
    surfaceDesc.dwSize = sizeof(DDSURFACEDESC2);
    int retryCount = 100;
    do
    {
        HRESULT hr = gSurfacesArray_77644C[idx].field_0_dd_surface->Lock(0, &surfaceDesc, 17, 0);
        if (hr != DDERR_WASSTILLDRAWING)
        {
            if (!FAILED(hr))
            {
                int rowSize = surfaceDesc.dwWidth * surfaceDesc.ddpfPixelFormat.dwRGBBitCount >> 3;
                const int pitch = surfaceDesc.lPitch;
                if (!rowSize)
                {
                    rowSize = 1;
                }

                BYTE* pSurfaceData = gSurfacesArray_77644C[idx].field_4_surface_pixel_buffer;
                char* pRow = (char*)surfaceDesc.lpSurface;

                for (DWORD i = 0; i < surfaceDesc.dwHeight; i++)
                {
                    memcpy(pRow, pSurfaceData, rowSize);
                    pRow += pitch;
                    pSurfaceData += rowSize;
                }

                gSurfacesArray_77644C[idx].field_0_dd_surface->Unlock(0);
                return;
            }
            --retryCount;
        }
    } while (retryCount);
}
MGS_FUNC_IMPLEX(0x0051E11A, Render_Restore_Single_Surface_51E11A, RENDERER_IMPL);

MGS_VAR(1, 0x650D1A, WORD, g_Render_sub_41C640_ret_650D1A, 0);
MGS_ARY(1, 0x6DE3C0, WORD, 1500, gFreeTextureIdxArray_6DE3C0, {});

void CC Renderer_SurfaceArray_PopIdx_51E020(unsigned int idx)
{
    if (gSurfacesArray_77644C)
    {
        if (idx < gSurfaceArraySize_776854)
        {
            SurfaceBackup* pLast = &gSurfacesArray_77644C[gSurfaceArraySize_776854 - 1];
            SurfaceBackup* pToRemove = &gSurfacesArray_77644C[idx];

            // Swap the very last item with the item to move
            pToRemove->field_0_dd_surface = pLast->field_0_dd_surface;
            pToRemove->field_8_array_size = pLast->field_8_array_size;
            pToRemove->field_4_surface_pixel_buffer = pLast->field_4_surface_pixel_buffer;

            // Now resize the memory to knock off the last item, or totally nuke the array if there are no items
            const int newSize = gSurfaceArraySize_776854-- - 1;
            if (gSurfaceArraySize_776854)
            {
                gSurfacesArray_77644C = (SurfaceBackup *)realloc(gSurfacesArray_77644C, sizeof(SurfaceBackup) * newSize);
            }
            else
            {
                free(gSurfacesArray_77644C);
                gSurfacesArray_77644C = 0;
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x0051E020, Renderer_SurfaceArray_PopIdx_51E020, RENDERER_IMPL);

void CC Renderer_SurfaceArray_RemoveSurface_51DFC1(IDirectDrawSurface7* pSurfaceToRemove)
{
    signed int idx = 0;
    if (gSurfaceArraySize_776854 > 0)
    {
        SurfaceBackup* pAryIter = gSurfacesArray_77644C;
        while (pAryIter->field_0_dd_surface != pSurfaceToRemove)
        {
            ++idx;
            ++pAryIter;
            if (idx >= gSurfaceArraySize_776854)
            {
                return;
            }
        }

        gTotalAllocated_dword_774A40 -= gSurfacesArray_77644C[idx].field_8_array_size;
        free(gSurfacesArray_77644C[idx].field_4_surface_pixel_buffer);

        gSurfacesArray_77644C[idx].field_4_surface_pixel_buffer = 0;
        gSurfacesArray_77644C[idx].field_8_array_size = 0;
        Renderer_SurfaceArray_PopIdx_51E020(idx);
    }
}
MGS_FUNC_IMPLEX(0x0051DFC1, Renderer_SurfaceArray_RemoveSurface_51DFC1, RENDERER_IMPL);

MGS_FUNC_NOT_IMPL(0x4241A4, void __cdecl(void *), Renderer_Free_Surface_Type_5_4241A4); // TODO

void CC Renderer_Free_Textures_At_Rect_40D2A0(const PSX_RECT* pRect)
{
    for (int idx = 1; idx < gNumTextures_word_6FC78C; ++idx)
    {
        if (idx != g_Render_sub_41C640_ret_650D1A
            && Renderer_Is_Texture_In_Rect_40D150(pRect, idx)
            && !gTextures_6C0F00[idx].field_24_flagsQ)
        {
            if (gTextures_6C0F00[idx].mSurfaceType == 5)
            {
                Renderer_Free_Surface_Type_5_4241A4(gTextures_6C0F00[idx].mSurface);
            }
            else if (gTextures_6C0F00[idx].mSurface)
            {
                if (gSoftwareRendering)
                {
                    free(gTextures_6C0F00[idx].mSurface);
                }
                else
                {
                    Renderer_SurfaceArray_RemoveSurface_51DFC1(gTextures_6C0F00[idx].mSurface);
                    Render_PushSurface_51DDF2(gTextures_6C0F00[idx].mSurface);
                }
            }
            gTextures_6C0F00[idx].mSurface = 0;
            gTextures_6C0F00[idx].field_24_flagsQ = 0;
            gTextures_6C0F00[idx].field_6_x = -1;
            gTextures_6C0F00[idx].field_4_y = -1;
            gTextures_6C0F00[idx].field_A_h = 0;
            gTextures_6C0F00[idx].field_8_w = 0;
            gFreeTextureIdxArray_6DE3C0[gNumFreeTextures_6FC790++] = idx;
        }
    }
}
MGS_FUNC_IMPLEX(0x0040D2A0, Renderer_Free_Textures_At_Rect_40D2A0, RENDERER_IMPL);

MGS_VAR(1, 0x6FC728, DWORD *, gImageBufer_dword_6FC728, 0);

void CC Render_BltToPixelBuffer_40CED0(const PSX_RECT* pRect, __int16 value)
{
    const int width = pRect->x1 + pRect->x2 - 1;
    const int result = pRect->y1 + pRect->y2 - 1;
    const int height = pRect->y1 + pRect->y2 - 1;
    for (int ypos = pRect->y1; ypos <= height; ++ypos)
    {
        for (int xpos = pRect->x1; xpos <= width; ++xpos)
        {
            g_pwTextureIndices_6FC72C[1024 * ypos + xpos] = value;
        }
    }
}
MGS_FUNC_IMPLEX(0x0040CED0, Render_BltToPixelBuffer_40CED0, RENDERER_IMPL);

int CC Render_BitBltToDxSurface_40FD20(const PSX_RECT* pRect, const BYTE* pPixels)
{
    if (pRect && pPixels)
    {
        const auto y = pRect->y1;
        const auto x = pRect->x1;
        const auto x2 = pRect->x2;
        const auto v2 = pRect->y2;

        if (pRect->x1 >= 0 && x < 640 && y >= 0 && y < 240)
        {
            puts(" $edq LOADING IN SCREEN AREA !!!!");
        }

        if (!pRect || !pPixels)
        {
            return 0;
        }

        if (pRect->x1 >= 960 && pRect->x1 < 965 && pRect->y1 >= 148 && pRect->y1 < 188)
        {
            puts("catch");
        }

        if (g_Render_sub_41C640_ret_650D1A == 0xFFFE)
        {
            // Copy into 1024x512 vram buffer
            auto y1 = pRect->y1;
            for (int yCount = 0;
                yCount < pRect->y2;
                memcpy((char *)gImageBufer_dword_6FC728 + 2048 * y1++ + 2 * x, &pPixels[2 * x2 * yCount++], 2 * x2))
            {
                ;
            }
        }
        else
        {
            Render_BltToPixelBuffer_40CED0(pRect, g_Render_sub_41C640_ret_650D1A);
            Renderer_Free_Textures_At_Rect_40D2A0(pRect);
        }
        g_Render_sub_41C640_ret_650D1A = -2;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0040FD20, Render_BitBltToDxSurface_40FD20, RENDERER_IMPL);

void CC Render_sub_41C6B0(const PSX_RECT* pRect, const BYTE* pPixelData)
{
    while (gbKeepCopyingSurface_dword_6C0770)
    {

    }
    ++gbKeepCopyingSurface_dword_6C0770;
    Render_BitBltToDxSurface_40FD20(pRect, pPixelData);
    --gbKeepCopyingSurface_dword_6C0770;
}
MGS_FUNC_IMPLEX(0x0041C6B0, Render_sub_41C6B0, RENDERER_IMPL);
