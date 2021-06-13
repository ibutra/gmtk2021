#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "raylib.h"
#include "person.h"
#include "personarray.h"
#include "game.h"
#include "state.h"
#include "mainmenu.h"
#include "font.h"
#include "creditscreen.h"

//TODO: make somewhat pretty

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

    //Load Font
    font_load();
    menu_init();

    style();

    gui_init();

    while(!WindowShouldClose()) {

        BeginDrawing();
        {
            State newState;
            ClearBackground(RAYWHITE);
            if (state == STATE_MENU) {
                newState = menu_show();
            } else if (state == STATE_GAME) {
                gui_updateGameTime();
                gui_drawPersonlist();
                gui_handleInput();
                newState = gui_drawInterface();
            } else if (state == STATE_CREDITS) {
                newState = credits_show();
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
