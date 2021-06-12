#ifndef GUI_HELPER_H
#define GUI_HELPER_H

#include <raylib.h>

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

void gui_drawTextureScaledToSize(Texture2D texture, int x, int y, int width, Color tint);

#endif //GUI_HELPER_H
