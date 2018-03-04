#pragma once

MGS_VAR_EXTERN(WORD, gSnakeCurrentHealth_78E7F6);
MGS_VAR_EXTERN(WORD, gSnakeMaxHealth_78E7F8);

struct MenuMan;
struct TextConfig;
struct MenuPrimBuffer;
struct Menu_Item_Unknown;

struct MenuMan_Inventory_14h_Unk
{
    BYTE* field_0_pixels;
    WORD* field_4_word_ptr_pixels;
    char field_8_index;
    char field_9_x;
    char field_A_y;
    BYTE field_B; // Padding?
    DWORD field_C_uv_clut;
    WORD field_10_w;
    WORD field_12_h;
};
MGS_ASSERT_SIZEOF(MenuMan_Inventory_14h_Unk, 0x14);

MGS_VAR_EXTERN(short, gMenu_Selected_item_idx_word_78E7FE);

void DoMenuTests();

void CC Res_MenuMan_create_459A9A();
void CC MenuTextureLoad_44DEB3();
void CC Menu_create_459891();
void CC Menu_TextReset_459ACE();
void CC Menu_DrawText_459B63(const char* pFormatStr, int formatArg1 = 0, int formatArg2 = 0, int formatArg3 = 0, int formatArg4 = 0);
void CC TextSetRGB_459B27(int r, int g, int b);
void CC TextSetXYFlags_459B0B(int x, int y, int flags);
void CC Menu_Set_Text_BlendMode_459BE0();
int CC Menu_inventory_text_4689CB(MenuMan* pMenu, DWORD* ot, int xpos, int ypos, const char* pText, int textFlags);
void Menu_render_unk_2_and_3_468C6B();
int CC Script_tbl_menu_sub_4521A7(BYTE* pScript);
void CC Menu_render_number_as_string_468529(MenuPrimBuffer* pPrimBuffer, TextConfig* pTextConfig, signed int numberRemainder);
void CC Menu_render_number_as_string_with_flags_4688DC(MenuMan* pMenu, DWORD* ot, int textX, int textY, signed int number, int textFlags);
void CC Render_Text_Small_font_468642(MenuPrimBuffer* pPrimBuffer, TextConfig* pTextSettings, const char* pString);
void CC Menu_render_text_fractional_468915(MenuMan* pMenu, int x, int y, signed int currentValue, signed int maxValue);
void CC Menu_inventory_common_icon_helper_46AFE1(MenuMan_Inventory_14h_Unk* pMenuUnk);
void CC Menu_j_inventory_right_init_11_items_459A95();
signed int CC Menu_inventory_common_update_helper_46B979(int idx);
void CC Menu_render_unk_46B081(MenuMan_Inventory_14h_Unk *pUnk, int idx);
signed int CC Menu_46B540(Menu_Item_Unknown* pItem);
void CC Menu_inits_459A48();
