#pragma once

MGS_VAR_EXTERN(WORD, gSnakeCurrentHealth_78E7F6);
MGS_VAR_EXTERN(WORD, gSnakeMaxHealth_78E7F8);

struct MenuMan;

struct MenuMan_Inventory_14h_Unk
{
    BYTE* field_0_pixels;
    WORD* field_4_word_ptr_pixels;
    BYTE field_8_index;
    BYTE field_9_x;
    BYTE field_A_y;
    BYTE field_B;
    DWORD field_C_colour;
    WORD field_10_w;
    WORD field_12_h;
};
MGS_ASSERT_SIZEOF(MenuMan_Inventory_14h_Unk, 0x14);


void DoMenuTests();

void CC Res_MenuMan_create_459A9A();
void CC MenuTextureLoad_44DEB3();
void CC Menu_create_459891();
void CC Menu_TextReset_459ACE();
void CC Menu_DrawText_459B63(const char* pFormatStr, int formatArg1 = 0, int formatArg2 = 0, int formatArg3 = 0, int formatArg4 = 0);
void CC TextSetRGB_459B27(int r, int g, int b);
void CC TextSetXYFlags_459B0B(int x, int y, int flags);
void CC Menu_Set_Text_BlendMode_459BE0();
int CC Menu_inventory_text_4689CB(MenuMan* pMenu, int* ot, int xpos, int ypos, const char* pText, int textFlags);
void Menu_render_unk_2_and_3_468C6B();
