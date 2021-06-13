#include "font.h"

#include <stdlib.h>
#include <raygui.h>

#include "Gidole-Regular.h"

Font gameFont_50;
Font gameFont_30;
Font gameFont_20;
Font gameFont_15;

void font_load(void) {
    gameFont_50 = LoadFontFromMemory("ttf", Gidole_Regular_ttf, Gidole_Regular_ttf_len, 50, NULL, 200);
    gameFont_30 = LoadFontFromMemory("ttf", Gidole_Regular_ttf, Gidole_Regular_ttf_len, 30, NULL, 200);
    gameFont_20 = LoadFontFromMemory("ttf", Gidole_Regular_ttf, Gidole_Regular_ttf_len, 20, NULL, 200);
    gameFont_15 = LoadFontFromMemory("ttf", Gidole_Regular_ttf, Gidole_Regular_ttf_len, 15, NULL, 200);
    GuiSetFont(gameFont_30);
}
