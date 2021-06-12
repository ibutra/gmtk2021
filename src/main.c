#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "raylib.h"
#include "person.h"
#include "personarray.h"
#include "gui.h"
#include "state.h"
#include "mainmenu.h"

//TODO: Fix person generation
//TODO: add credits
//TODO: add help
//TODO: make somewhat pretty
//TODO: change font

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

static void style(void);

int main(int argc, char** argv) {
    srand(time(NULL));
    const int screenWidth = 1024;
    const int screenHeight = 768;
    State state = STATE_MENU;

    /* SetConfigFlags(FLAG_WINDOW_RESIZABLE); */
    InitWindow(screenWidth, screenHeight, "GMTK"); //TODO: change title
    SetTargetFPS(60);

    style();

    gui_init();

    while(!WindowShouldClose()) {

        BeginDrawing();
        {
            State newState;
            ClearBackground(WHITE);
            if (state == STATE_MENU) {
                newState = menu_show();
            } else if (state == STATE_GAME) {
                gui_updateGameTime();
                gui_drawPersonlist();
                gui_handleInput();
                newState = gui_drawInterface();
            }
            if (newState != state) {
                gui_reset();
                state = newState;
            }
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}

static void style(void) {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
}
