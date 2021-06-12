#include "mainmenu.h"
#include <raygui.h>

State menu_show(void) {
    Rectangle r = {.x = GetScreenWidth() / 2 - 50, .y = GetScreenHeight() / 2, .width = 100, .height = 50};
    if (GuiButton(r, "PLAY")) {
        return STATE_GAME;
    }
    return STATE_MENU;
}
