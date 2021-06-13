#include "creditscreen.h"

#include <stdio.h>
#include <raygui.h>
#include "font.h"
#include "credits.h"

static Vector2 scroll = {0, 0};

State credits_show(void) {
    Rectangle bounds = {.x = 10, .y = 10, .width = GetScreenWidth() - 20, .height = GetScreenHeight() - 20 - 50};
    Rectangle content = {0, 0, GetScreenWidth() - 50, 2500};
    Rectangle view = GuiScrollPanel(bounds, content, &scroll);

    BeginScissorMode(view.x, view.y, view.width, view.height);
    {
        Rectangle r = {bounds.x + scroll.x, bounds.y + scroll.y, content.width, content.height};
        DrawTextRec(gameFont_15, (const char*)credits_txt, r, 15, 1.0f, true, BLACK);
    }
    EndScissorMode();
    Rectangle b = {.x = (GetScreenWidth() - 100) / 2, .y = GetScreenHeight() - 50, .width = 100, .height = 30};
    if (GuiButton(b, "Back")) {
        return STATE_MENU;
    }
    return STATE_CREDITS;
}
