#include <stdio.h>

#include "raylib.h"

int main(int argc, char** argv) {
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "GMTK"); //TODO: change title
    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Hello World", 100, 100, 20, BLACK);
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}
