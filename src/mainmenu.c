#include "mainmenu.h"
#include <raygui.h>

#include "font.h"
#include "help.h"
#include "gui_helper.h"
#include "logo.h"

Texture2D logo;

void menu_init(void) {
    Image logoImage = LoadImageFromMemory("png", logo_png, logo_png_len);
    SetWindowIcon(logoImage);
    logo = LoadTextureFromImage(logoImage);
}

State menu_show(void) {
    const int logoHeight = 350;
    float f = (float)logoHeight / logo.height;
    const int width = logo.width * f;
    const int x = (GetScreenWidth() - width) / 2;
    const int y = 10;
    DrawTextureEx(logo, (Vector2){.x = x, .y = y}, 0.0f, f, RAYWHITE);

    const int margin = 50;
    const int buttonHeight = GetScreenHeight() * 7 / 8;
    const int offset = 0;
    Rectangle helpRect = {.x = margin, .y = logoHeight - offset, .width = GetScreenWidth() - 2 * margin, .height = buttonHeight - logoHeight + offset};
    DrawTextRec(gameFont_20, (const char*)help_txt, helpRect, 20.0f, 1.0f, true, BLACK);
    Rectangle r = {.x = GetScreenWidth() / 2 - 50 - 100, .y = buttonHeight, .width = 100, .height = 50};
    if (GuiButton(r, "Credits")) {
        return STATE_CREDITS;
    }
    r.x = GetScreenWidth() / 2 - 50 + 100;
    if (GuiButton(r, "PLAY")) {
        return STATE_GAME;
    }
    return STATE_MENU;
}
