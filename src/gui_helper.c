#include "gui_helper.h"

#include <stdio.h>

void gui_drawTextureScaledToSize(Texture2D texture, int x, int y, int width) {
    float factor = (float)width / (float)texture.width;
    DrawTextureEx(texture, (Vector2){.x = x, .y = y}, 0.0f, factor, WHITE); //TODO: figure out what exactly tint does
}
