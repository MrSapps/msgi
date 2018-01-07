#pragma once

MGS_VAR_EXTERN(WORD, gSnakeCurrentHealth_78E7F6);
MGS_VAR_EXTERN(WORD, gSnakeMaxHealth_78E7F8);

void CC Res_MenuMan_create_459A9A();
void CC MenuTextureLoad_44DEB3();
void CC Menu_create_459891();
void CC Menu_TextReset_459ACE();
int CC Menu_DrawText_459B63(const char* Format, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0);
int CC TextSetRGB_459B27(int r, int g, int b);
void CC TextSetXYFlags_459B0B(int x, int y, int flags);
