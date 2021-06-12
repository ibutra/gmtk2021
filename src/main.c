#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "raylib.h"
#include "person.h"
#include "personarray.h"
#include "gui.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    const int screenWidth = 1024;
    const int screenHeight = 768;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "GMTK"); //TODO: change title
    SetTargetFPS(60);

    gui_initPersonIcons();

    PersonArray personArray;
    if (!personarray_create(&personArray)) {
        fprintf(stderr, "Failed to create personarray\n");
        return -1;
    }

    //TODO: Dummy for now:
    for (size_t i = 0; i < 201; i++) { // XD
        Person p = person_create();
        personarray_add(&personArray, p);
    }

    while(!WindowShouldClose()) {

        BeginDrawing();
        {
            ClearBackground(WHITE);
            gui_updateGameTime();
            gui_drawPersonlist(&personArray);
            gui_handleInput(&personArray);
            gui_drawInterface(&personArray);
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}
