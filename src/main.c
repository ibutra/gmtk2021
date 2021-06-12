#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "raylib.h"
#include "person.h"
#include "personarray.h"
#include "gui_personlist.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    const int screenWidth = 1024;
    const int screenHeight = 768;

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
            ClearBackground(RAYWHITE);
            /* DrawText("Hello World", 100, 100, 20, BLACK); */
            gui_drawPersonlist(&personArray);
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}
