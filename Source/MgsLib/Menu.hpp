#pragma once

MGS_VAR_EXTERN(WORD, gSnakeCurrentHealth_78E7F6);
MGS_VAR_EXTERN(WORD, gSnakeMaxHealth_78E7F8);

struct MenuMan;

void CC Res_MenuMan_create_459A9A();
void CC MenuTextureLoad_44DEB3();
void CC Menu_create_459891();
void CC Menu_TextReset_459ACE();
void CC Menu_DrawText_459B63(const char* pFormatStr, int formatArg1 = 0, int formatArg2 = 0, int formatArg3 = 0, int formatArg4 = 0);
int CC TextSetRGB_459B27(int r, int g, int b);
void CC TextSetXYFlags_459B0B(int x, int y, int flags);
void CC Menu_Set_Text_BlendMode_459BE0();
int CC Menu_inventory_text_4689CB(MenuMan* pMenu, int* ot, int xpos, int ypos, const char* pText, int textFlags);
