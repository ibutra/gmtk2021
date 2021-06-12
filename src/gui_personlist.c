#include "gui_personlist.h"

#include <raylib.h>

#include "gui_helper.h"

#include "character.h"
#include "3d-hammer.h"
#include "basketball-jersey.h"
#include "cooking-pot.h"
#include "gamepad.h"
#include "gardening-shears.h"
#include "journey.h"
#include "paint-brush.h"
#include "party-popper.h"
#include "shopping-bag.h"
#include "sloth.h"
#include "wave-surfer.h"
#include "white-book.h"

#define CHARACTER_RECT (50)

Texture2D character;
Texture2D hammer;
Texture2D basketball_jersey;
Texture2D cooking_pot;
Texture2D gamepad;
Texture2D gardening_shears;
Texture2D journey;
Texture2D paint_brush;
Texture2D party_popper;
Texture2D shopping_bag;
Texture2D sloth;
Texture2D wave_surfer;
Texture2D white_book;

void gui_initPersonIcons(void) {
    character = LoadTextureFromImage(LoadImageFromMemory("png", character_png, character_png_len));
    hammer = LoadTextureFromImage(LoadImageFromMemory("png", __3d_hammer_png, __3d_hammer_png_len));
    basketball_jersey = LoadTextureFromImage(LoadImageFromMemory("png", basketball_jersey_png, basketball_jersey_png_len));
    cooking_pot = LoadTextureFromImage(LoadImageFromMemory("png", cooking_pot_png, cooking_pot_png_len));
    gamepad = LoadTextureFromImage(LoadImageFromMemory("png", gamepad_png, gamepad_png_len));
    gardening_shears = LoadTextureFromImage(LoadImageFromMemory("png", gardening_shears_png, gardening_shears_png_len));
    journey = LoadTextureFromImage(LoadImageFromMemory("png", journey_png, journey_png_len));
    paint_brush = LoadTextureFromImage(LoadImageFromMemory("png", paint_brush_png, paint_brush_png_len));
    party_popper = LoadTextureFromImage(LoadImageFromMemory("png", party_popper_png, party_popper_png_len));
    shopping_bag = LoadTextureFromImage(LoadImageFromMemory("png", shopping_bag_png, shopping_bag_png_len));
    sloth = LoadTextureFromImage(LoadImageFromMemory("png", sloth_png, sloth_png_len));
    wave_surfer = LoadTextureFromImage(LoadImageFromMemory("png", wave_surfer_png, wave_surfer_png_len));
    white_book = LoadTextureFromImage(LoadImageFromMemory("png", white_book_png, white_book_png_len));
}

// Draw two scrolling lists of persons
void gui_drawPersonlist(PersonArray* array) {
    for (size_t i = 0; i < array->count; i++) {
        //TODO: calculate positions based on window size
        int x = i & 1? 10 : 220;
        int y = (i >> 1) * 100;
        gui_drawTextureScaledToSize(character, x, y, CHARACTER_RECT);
    }
}
