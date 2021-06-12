#include "gui.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <raylib.h>

#include "gui_helper.h"
#include "icons.h"
#include "person.h"

/*
 * Defines
 */
#define TIME_TO_START (1000000.0f) //Seconds //TODO: set reasonable
#define SCROLL_FACTOR (40.0f) //TODO: decide whether this gets progressively faster

#define INTERFACE_HEIGHT (50)

#define CHARACTER_RECT (100)
#define INTEREST_RECT (40)
#define CHARACTER_FILE_WIDTH (CHARACTER_RECT + 5 * INTEREST_RECT)

#define MARGIN (100)
#define INTER_CHARACTER_MARGIN (10)


/*
 * Globals
 */

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

static int64_t expired_index = 0; //We don't need to consider characters before this index, they are already unhappy ;)
static size_t index_visible = 0; //This is the index of the first visible character
static size_t num_expired = 0; //How many characters didn't get a match
static uint64_t score = 0; //The score duh. If anybody overflows this let me know
static Person* startDragPerson = NULL; //Person we are currently dragging from
static bool waitForRelease = false; //We joined two persons so waiting for mouse release
static float elapsedTime = -TIME_TO_START;


/*
 * Static functions
 */
static void gui_handleMatch(Person* person);
static void gui_handleExpired(Person* person);
static Texture2D gui_getIconForInterest(Interests interest);
static Vector2 gui_getLineAnchor(Person* person);
static Rectangle gui_getCharacterFileRect(Person* person);

/*
 * Implementation
 */

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

void gui_updateGameTime(void) {
    elapsedTime += GetFrameTime();
}

// Draw two scrolling lists of persons
void gui_drawPersonlist(PersonArray* array) {
    for (size_t i = index_visible; i < array->count; i++) {
        int width = GetScreenWidth();
        int margin = MARGIN;
        while (width - 2 * CHARACTER_FILE_WIDTH - 2 * margin < 0) {
            margin -= 10;
        }
        margin = MAX(0, margin);

        int x = i & 1? width - CHARACTER_FILE_WIDTH - margin : margin;
        int y = (i >> 1) * (CHARACTER_RECT + INTER_CHARACTER_MARGIN); //Space characters nicely
        y += MARGIN + INTERFACE_HEIGHT - SCROLL_FACTOR * MAX(0, elapsedTime); // Add offset to expired line and scroll based on time

        Person* person = personarray_get(array, i);
        person->position.x = x;
        person->position.y = y;
        //Handle expired characters
        if (y == INTERFACE_HEIGHT && !person->partner && !person->expired) {
            expired_index = person->index;
            gui_handleExpired(person);
        }
        if (y < -CHARACTER_RECT + INTERFACE_HEIGHT) {
            index_visible = person->index + 1; 
        }
        Color tint = WHITE;
        if (person->expired) {
            tint = GRAY;
        } else if (person->partner) {
            tint = PINK;
        }
        //Draw character
        DrawRectangleRec(gui_getCharacterFileRect(person), tint);
        gui_drawTextureScaledToSize(character, x, y, CHARACTER_RECT, tint);
        if (!person) {
            fprintf(stderr, "Invalid person requested, something is seriously wrong!!!\n");
            exit(-1);
        }
        x += CHARACTER_RECT;
        //Draw interests the character wants
        int num = 0;
        for (int i = 0; i < NUM_INTERESTS; i++) {
            if (person->wants & (1 << i)) {
                Texture2D texture = gui_getIconForInterest(1 << i);
                gui_drawTextureScaledToSize(texture, x + num * INTEREST_RECT, y, INTEREST_RECT, tint);
                num++;
            }
        }
        //Draw interests the character has
        y += CHARACTER_RECT - INTEREST_RECT;
        num = 0;
        for (int i = 0; i < NUM_INTERESTS; i++) {
            if (person->has & (1 << i)) {
                Texture2D texture = gui_getIconForInterest(1 << i);
                gui_drawTextureScaledToSize(texture, x + num * INTEREST_RECT, y, INTEREST_RECT, tint);
                num++;
            }
        }
        //Outline
        DrawRectangleLinesEx(gui_getCharacterFileRect(person), 2.0f, BLACK);
        //Connection to partner
        if (person->partner) {
            Person* partner = person->partner;
            if (partner->index < person->index) { //We draw line from the later character so it remains visible
                Vector2 personAnchor = gui_getLineAnchor(person);
                Vector2 partnerAnchor = gui_getLineAnchor(partner);
                int controlY = (personAnchor.y + partnerAnchor.y) / 2;
                int controlX = GetScreenWidth() / 2;
                DrawLineBezierQuad(personAnchor, partnerAnchor, (Vector2){.x = controlX, .y = controlY}, 2.0f, BLACK);
                char buffer[40];
                snprintf(buffer, 40, "%lli", person_getScore(person));
                const int size = 20;
                DrawRectangle(controlX, controlY - size / 2, MeasureText(buffer, size), size, WHITE);
                DrawText(buffer, controlX, controlY - size / 2, size, BLACK);
            }
        }
    }
}

void gui_drawInterface(PersonArray* array) {
    //Start timer
    if (elapsedTime < 0.0f) {
        char buffer[20];
        snprintf(buffer, 20, "%i", (int)fabsf(elapsedTime));
        DrawText(buffer, GetScreenWidth() / 2, GetScreenHeight() / 2, 20, BLACK);
    }

    //Score and expired
    int y = INTERFACE_HEIGHT / 2;
    DrawRectangle(0, 0, GetScreenWidth(), INTERFACE_HEIGHT, WHITE);
    DrawRectangleLines(0, 0, GetScreenWidth(), INTERFACE_HEIGHT, BLACK);
    char buffer[1024];
    snprintf(buffer, 1024, "Sad Characters: %li", num_expired);
    DrawText(buffer, MARGIN, y, 20, BLACK);
    snprintf(buffer, 1024, "Score: %lli", score);
    DrawText(buffer, GetScreenWidth() - 200, y, 20, BLACK);

    //Expiry line
    y = INTERFACE_HEIGHT + CHARACTER_RECT;
    DrawLine(0, y, GetScreenWidth(), y, RED);
    DrawRectangleGradientV(0, INTERFACE_HEIGHT, GetScreenWidth(), CHARACTER_RECT, RED, (Color){.r = 255, .a = 10});
}

void gui_handleInput(PersonArray* array) {
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonDown(0)) {
        if (waitForRelease) {
            return;
        }
        if (startDragPerson && startDragPerson->expired) {
            waitForRelease = true;
            return;
        }
        if (startDragPerson) {
            //Draw a cool line or soemthing
            Vector2 anchor = gui_getLineAnchor(startDragPerson);
            int controlY = (anchor.y + mouse.y) / 2;
            int controlX = GetScreenWidth() / 2;
            DrawLineBezierQuad(anchor, mouse, (Vector2){.x = controlX, .y = controlY}, 2.0f, RED);
        }
        //Check for collision with a character
        for (size_t i = expired_index; i < array->count; i++) {
            Person* p = personarray_get(array, i);
            Rectangle r = gui_getCharacterFileRect(p);
            if (CheckCollisionPointRec(mouse, r)) {
                if (p->partner || p->expired) {
                    continue;
                }
                if (!startDragPerson) {
                    startDragPerson = p;
                } else if (p != startDragPerson) {
                    startDragPerson->partner = p;
                    p->partner = startDragPerson;
                    gui_handleMatch(p);
                    startDragPerson = NULL;
                    waitForRelease = true;
                }
            }
        }
    } else {
        startDragPerson = NULL;
        waitForRelease = false;
    }
}

static void gui_handleMatch(Person* person) {
    score += person_getScore(person);
}

static void gui_handleExpired(Person* person) {
    person->expired = true;
    num_expired += 1;
}

static Texture2D gui_getIconForInterest(Interests interest) {
    switch (interest) {
        case COOKING:
            return cooking_pot;
            break;
        case HANDYWORK:
            return hammer;
            break;
        case PAINT:
            return paint_brush;
            break;
        case GAME:
            return gamepad;
            break;
        case LAZY:
            return sloth;
            break;
        case PARTY:
            return party_popper;
            break;
        case SPORT:
            return basketball_jersey;
            break;
        case WATER_SPORT:
            return wave_surfer;
            break;
        case READ:
            return white_book;
            break;
        case GARDEN:
            return gardening_shears;
            break;
        case TRAVEL:
            return journey;
            break;
        case SHOPPING:
            return shopping_bag;
            break;
        default: 
            fprintf(stderr, "Requested invalid interest\n");
            exit(-1);
    }
}

static Vector2 gui_getLineAnchor(Person* person) {
    bool right = person->index & 1;
    if (right) {
        return (Vector2){.x = person->position.x, .y = person->position.y + CHARACTER_RECT / 2};
    } else {
        return (Vector2){.x = person->position.x + CHARACTER_FILE_WIDTH, .y = person->position.y + CHARACTER_RECT / 2};
    }
}

static Rectangle gui_getCharacterFileRect(Person* person) {
    return (Rectangle){.x = person->position.x, .y = person->position.y, .width = CHARACTER_FILE_WIDTH, .height = CHARACTER_RECT};
}
