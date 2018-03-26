#include "stdafx.h"
#include "Actor_Debug.hpp"
#include "ResourceNameHash.hpp"
#include "LibDG.hpp"
#include "Menu.hpp"
#include "WinMain.hpp"
#include "Actor_GameD.hpp"
#include "Actor_Movie.hpp"

union PolyTag2
{
    struct
    {
        WORD LowPart;
        WORD HighPart;
    };
    DWORD WordPart;
};

void SetDepth(POLY_F4* p, u16 depth)
{
    PolyTag2* pTag = (PolyTag2*)&p->tag;
    //pTag->LowPart = depth;
    pTag->HighPart = 0x0900;
}

static std::string ButtonFlagsToString(DWORD flags)
{
    std::string buttons;

    if (flags & PsxButtonBits::eCircle)
    {
        buttons += "O ";
    }

    if (flags & PsxButtonBits::eCross)
    {
        buttons += "X ";
    }

    if (flags & PsxButtonBits::eDPadDown)
    {
        buttons += "D ";
    }

    if (flags & PsxButtonBits::eDPadLeft)
    {
        buttons += "L ";
    }

    if (flags & PsxButtonBits::eDPadRight)
    {
        buttons += "R ";
    }

    if (flags & PsxButtonBits::eDPadUp)
    {
        buttons += "U ";
    }

    if (flags & PsxButtonBits::eL1)
    {
        buttons += "L1 ";
    }

    if (flags & PsxButtonBits::eL2)
    {
        buttons += "L2 ";
    }

    if (flags & PsxButtonBits::eR1)
    {
        buttons += "R1 ";
    }

    if (flags & PsxButtonBits::eR2)
    {
        buttons += "R2 ";
    }

    if (flags & PsxButtonBits::eSelect)
    {
        buttons += "Select ";
    }

    if (flags & PsxButtonBits::eSquare)
    {
        buttons += "Square ";
    }

    if (flags & PsxButtonBits::eStart)
    {
        buttons += "Start ";
    }

    if (flags & PsxButtonBits::eTriangle)
    {
        buttons += "Triangle ";
    }

    return buttons;
}

static std::string LimitTo64Chars(const std::string& s)
{
    if (s.size() > 64)
    {
        return s.substr(0, 60) + "...";
    }
    return s;
}

Actor *__cdecl Res_jimctrl_create_45942B(__int16 a2);
MGS_FUNC_IMPLEX(0x45942B, Res_jimctrl_create_45942B, false);

Actor *__cdecl Res_jimctrl_create_45942B(__int16 a2)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
    //return nullptr;
   // return Res_jimctrl_create_45942B_.Ptr()(a2 );
    return nullptr;
}

void __cdecl SetSubTitleTextAndOtherVars_462D96(char *pSubs, signed int a2)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x462D96, SetSubTitleTextAndOtherVars_462D96, false);


Actor *__cdecl Res_strctrl_create_45803B(int scriptVar_s_vox_code, unsigned int scriptVar_p, int b24Or30fps);
MGS_FUNC_IMPLEX(0x45803B, Res_strctrl_create_45803B, false);

Actor *__cdecl Res_strctrl_create_45803B(int scriptVar_s_vox_code, unsigned int scriptVar_p, int b24Or30fps)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
    //return Res_strctrl_create_45803B_.Ptr()(scriptVar_s_vox_code, scriptVar_p, b24Or30fps);
    return nullptr;
}

static void RenderInputState()
{
    TextSetXYFlags_459B0B(60, 40, 0x0);
    TextSetRGB_459B27(255, 255, 0);
    const std::string buttonsStatus = "Button status = " + ButtonFlagsToString(gButtonsArray4_7919C0[0].field_0_button_status);
    Menu_DrawText_459B63(LimitTo64Chars(buttonsStatus).c_str());

    TextSetXYFlags_459B0B(60, 50, 0x0);
    const std::string buttonsPressed = "Button pressed = " + ButtonFlagsToString(gButtonsArray4_7919C0[0].field_2_button_pressed);
    Menu_DrawText_459B63(LimitTo64Chars(buttonsPressed).c_str());

    TextSetXYFlags_459B0B(60, 60, 0x0);
    const std::string buttonsReleased = "Button released = " + ButtonFlagsToString(gButtonsArray4_7919C0[0].field_4_button_release);
    Menu_DrawText_459B63(LimitTo64Chars(buttonsReleased).c_str());

    TextSetXYFlags_459B0B(60, 70, 0x0);
    const std::string buttonsQuick = "Button quick = " + ButtonFlagsToString(gButtonsArray4_7919C0[0].field_6_button_quick);
    Menu_DrawText_459B63(LimitTo64Chars(buttonsQuick).c_str());
}

static void UpdateSnakeHpTest()
{
    if (gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 < gGameStates_78E7E0.gSnakeMaxHealth_78E7F8)
    {
        gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6++;
        if (gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 >= gGameStates_78E7E0.gSnakeMaxHealth_78E7F8)
        {
            gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 = 0;
        }
    }
}

static void AddTestPOLYFT4s(Actor_Debug* pDebug)
{
    POLY_F4 * pDst = (POLY_F4  *)pDebug->mPrimData->field_40_pDataStart[gActiveBuffer_dword_791A08];
    POLY_F4 * pSrc = &pDebug->mPolyF4;

    memcpy(pDst, pSrc, sizeof(POLY_F4));
    SetDepth(pDst, 0x0900);
}

void CC Debug_UpdateTextTexture(Actor_Debug* pDebug, char* pText)
{
    gUseTrueType_dword_6FC7AC = gUseTrueType_650D40;
    Font_set_text_45C80A(&pDebug->mTestFont, pText);
    Font_render_45C76C(&pDebug->mTestFont);
    gUseTrueType_dword_6FC7AC = 0;
}

void Debug_DrawTextTexture(Actor_Debug* pDebug, short x, short y, short w, short h)
{
    
    pDebug->mTestFontTexture.w = w;
    pDebug->mTestFontTexture.h = h;
    pDebug->mTestFontTexture.x0 = x;
    pDebug->mTestFontTexture.y0 = y;
    

   // addPrim(pMenu->field_20_prim_buffer->mOt, itemTextSprite);

    addPrim(gLibGvStruct2_6BC558.mOrderingTables[gActiveBuffer_dword_791A08], &pDebug->mTestFontTexture);

    //    gMenuPrimBuffer_7265E0.mOt = gLibGvStruct2_6BC558.mOrderingTables[gActiveBuffer_dword_791A08];
}

const int kFontWidth = 240;
const int kFontHeight = 100;

MGS_VAR(1, 0x733858, char*, gSubsText_dword_733858, 0);
MGS_VAR(1, 0x73382C, Font, gFont_73382C, {});



static void CC Debug_Update(Actor_Debug* pDebug)
{
   // gUseTrueType_650D40 = 0;
    /*
    static BYTE toDraw[20] = { 200, 0, 0 ,0 ,0 ,0 ,0 ,0 };
    static int delay = 0;
    delay++;
    if (delay > 1)
    {
        delay = 0;
        

        if (toDraw[0] == 255)
        {
            toDraw[0] = 0;
            toDraw[1]++;
        }
        else
        {
            toDraw[0]++;
        }

        if (toDraw[0] == '#' && toDraw[1] == 0)
        {
            toDraw[0]++;
        }
        if (toDraw[0] == 151 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 152 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 153 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 157 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 183 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 184 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 185 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 189 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 215 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 216 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 217 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 221 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 247 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 248 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 249 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        if (toDraw[0] == 253 && toDraw[1] == 0)
        {
            toDraw[0]++;
        }

        printf("Value is [%d %c][%d %c]]\n", toDraw[0], toDraw[0], toDraw[1], toDraw[1]);
    }
    Debug_UpdateTextTexture(pDebug, (char*)toDraw);
    */



/*
static BYTE test[] = 
{ 
    0x82, 0x2f, // first word
    0x82, 0x04, 
    0x82, 0x26, 
    0x82, 0x0f, 
    0x90, 0xfe, 
    0x90, 0xff, 
    0x9c, 0x01, 
    0x9c, 0x02, 
    0x20, 0x80, // [f_start]FOX FOUND
    0x23, 0x80, 
    0x7b, 0x46, 
    0x4f, 0x58, 
    0x20, 0x48, 
    0x4f, 0x55, 
    0x4e, 0x44, 
    0xd0, 0x02, // [f_mid]
    0x82, 0x35, // [text here appears above FOX HOUND]
    0xc2, 0x09, 
    'P', 'A',
    'U', 'L', 
    0x82, 0x0f, 
    0x82, 0x19, 
    0x82, 0x2f, 
    0x82, 0x06, 
    0x82, 0x53, 
    0x82, 0x29, 
    0xc0, 0x7d, 
    0x80, 0x23, // [f_end]
    0x00, 0x00 
};
*/
/*
static BYTE test[] =
{
    'H', 'e', 'l', 'l', 'o',
    0x23, 0x80,
    0x7b,
    'W', 'o', 'r', 'l', 'd',
    0xd0, 0x02,
    'P', 'A', 'U', 'L',
    0xc0, 0x7d,
    0x80, 0x23,
    'L', 'O', 'L',
    0x00, 0x00
};
*/
static BYTE test[] =
{
    'H', 'e', 'l', 'l', 'o',
    0x23, 0x80,
    0x4e, 0x80,
    0x7b,
    'W', 'o', 'r', 'l', 'd',
    0xd0, 0x02,
    'P', 'A', 'U', 'L',
    0xc0, 0x7d,
    'L', 'O', 'L',

    0x23, 0x80,
    0x7b,
    'W', 'o', 'r', 'l', 'd',
    0xd0, 0x02,
    'P', 'A', 'U', 'L',
    0xc0, 0x7d,
    'L', 'O', 'L',


    0x00, 0x00
};

    //gGameStates_78E7E0.gFlags_dword_78E7E4 |= 0x800;  // Set Japanese
    //gGameStates_78E7E0.gFlags_dword_78E7E4 &= ~0x100; // Unset English ??

    //gGameStates_78E7E0.gFlags_dword_78E7E4 &= ~0x400; // bit 11
/*
    bool draw = true;
  //  if (gButtonsArray4_7919C0[0].field_2_button_pressed & PsxButtonBits::eDPadDown)
    {
//        Font_Set_global_alloc_ptr_45C7F2(&pDebug->mTestFont);
       
        gUseTrueType_dword_6FC7AC = gUseTrueType_650D40;
        Font_set_text_45C80A(&gFont_73382C, (char*)test);
        Font_Set_global_alloc_ptr_45C7F2(&gFont_73382C);
        Font_render_45C76C(&gFont_73382C);
        gUseTrueType_dword_6FC7AC = 0;

       // Debug_UpdateTextTexture(pDebug, (char*)test);
      //  draw = false;
    }
    if (draw)
    {
        Debug_DrawTextTexture(pDebug, 20, 80, kFontWidth, kFontHeight);
    }
    */
    //gSubsText_dword_733858 = (char*)test;

    TextSetXYFlags_459B0B(30, 100, 0);
    TextSetRGB_459B27(0x80, 0x80, 0x80);
    Menu_DrawText_459B63 ("A textual test");

    TextSetXYFlags_459B0B(20, 120, 0x20);
    TextSetRGB_459B27(255, 0, 255);
    Menu_DrawText_459B63("A textual test !()_");

    TextSetXYFlags_459B0B(50, 140, 0x20);
    TextSetRGB_459B27(55, 255, 100);
    TextSetXYFlags_459B0B(50, 140, 0x10);
    Menu_DrawText_459B63("A textual test !$%^&*()_");

    AddTestPOLYFT4s(pDebug);

    RenderInputState();
   
    if (gButtonsArray4_7919C0[0].field_2_button_pressed & PsxButtonBits::eDPadUp)
    {
        // vc001501.vox
       // Res_strctrl_create_45803B(0xfc061006, 0, 0);
       Actor_Movie* pMovie = Res_movie_create_4561DF(ResourceNameHash("e399"), 0);

      // pMovie->word_7248F2_11_param_i = 12;
     //  pMovie->dword_7248F4_param_o = -12;

       gSubsText_dword_733858 = "MOVIE SUB TEST";
    }
    // 900074

    UpdateSnakeHpTest();

    //Sleep(50);
}

static void CC Debug_Shutdown(Actor_Debug* pDebug)
{
    auto pPal = Font_Get_Palette_45AB0B(&pDebug->mTestFont);
    if (pPal)
    {
        System_2_free_40B2A7(pPal);
    }
}

MGS_VAR_EXTERN(u32, dword_9942A0);

static void Debug_InitTestPOLYFT4s(Actor_Debug* pDebug)
{
    pDebug->mPolyF4 = {};

    int numPrims = 1;
    Prim_unknown_0x54* pPrim16Data = PrimAlloc_405050(2066, numPrims, 0, 0, 0);
    if (pPrim16Data)
    {
        PrimAdd_401805(pPrim16Data);
        pPrim16Data->field_28_flags_or_type = (signed __int16)dword_9942A0;
    }
    pDebug->mPrimData = pPrim16Data;

    //const WORD resHash = ResourceNameHash("back_l");
    //Res_rank_prim_related_4767CE(nullptr, resHash, &pDebug->mPolyF4, -160, -112, 0, 112, 0, 0);

    pDebug->mPolyF4.tag = 0x9000000;
    pDebug->mPolyF4.code = 2;

    pDebug->mPolyF4.r0 = 255;
    pDebug->mPolyF4.g0 = 255;
    pDebug->mPolyF4.b0 = 255;

    pDebug->mPolyF4.x0 = 0;
    pDebug->mPolyF4.y0 = 0;

    pDebug->mPolyF4.x1 = 0;
    pDebug->mPolyF4.y1 = 1000;

    pDebug->mPolyF4.x2 = 1000;
    pDebug->mPolyF4.y2 = 1000;

    pDebug->mPolyF4.x3 = 0;
    pDebug->mPolyF4.y3 = 1000;

}

static void Debug_InitRenderToTextureFont(Actor_Debug* pDebug)
{
    static PSX_RECT textVramArea { 960, 256, kFontWidth/4, kFontHeight };
  
    setSprt(&pDebug->mTestFontTexture);
    setRGB0(&pDebug->mTestFontTexture, 128, 128, 128);

    pDebug->mTestFontTexture.v0 = static_cast<BYTE>(textVramArea.y1);
    pDebug->mTestFontTexture.u0 = 0;

    pDebug->mTestFontTexture.w = kFontWidth;
    pDebug->mTestFontTexture.h = kFontHeight;

    pDebug->mTestFontTexture.x0 = 0;
    pDebug->mTestFontTexture.y0 = 0;

    pDebug->mTestFontTexture.clut = 32700;
    ClearImage(&textVramArea, 0, 0, 0);
    Font_45A70D(&pDebug->mTestFont, &textVramArea, 960, 510);
    Font_45A796(&pDebug->mTestFont, -1 /*rect max x lines*/, -1 /*rect max y lines*/, 0 /*char spacing*/, 6 /*extra line spacing*/, 2, 0);
    const int sizeToAlloc = Font_CalcSize_45AA45(&pDebug->mTestFont);
    void* pAllocated = System_2_zerod_allocate_memory_40B296(sizeToAlloc);
    if (!pAllocated)
    {
        return;
    }
    Font_Set_Buffer_45AAE9(&pDebug->mTestFont, (WORD*)pAllocated);
    Font_ColourRelated_45A89F(&pDebug->mTestFont, 0, 0x6739, 0);
    //Font_ColourRelated_45A89F(&pDebug->mTestFont, 0, 0x0, 0xff); // pal idx, fg/bg colours

    Font_Set_global_alloc_ptr_45C7F2(&pDebug->mTestFont);
}

static int CC Debug_Loader(Actor_Debug* pDebug)
{
    Debug_InitTestPOLYFT4s(pDebug);

    Debug_InitRenderToTextureFont(pDebug);

    // TODO: Hacks because these don't get set correctly in standalone exe yet
    game_state_dword_72279C.flags = 0x00400000;
    gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 = 1024-256;
    gGameStates_78E7E0.gSnakeMaxHealth_78E7F8 = 1024;
    gSnakeCurrentO2_995348 = 512;
    GiveAllItemsCheat();

    return 0;
}

Actor_Debug* AddDebugActor()
{
    Actor_Debug* pDebug = Actor_ResourceAllocT<Actor_Debug>(1);
    if (pDebug)
    {
        Actor_Init_40A347(&pDebug->mBase,
            reinterpret_cast<TActorFunction>(Debug_Update),
            reinterpret_cast<TActorFunction>(Debug_Shutdown),
            __FILE__);

        if (Debug_Loader(pDebug) < 0)
        {
            Actor_DestroyOnNextUpdate_40A3ED(&pDebug->mBase);
        }
    }
    return pDebug;
}
