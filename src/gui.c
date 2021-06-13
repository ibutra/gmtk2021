#include "gui.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <raylib.h>
#include <raygui.h>

#include "gui_helper.h"
#include "icons.h"
#include "person.h"

/*
 * Defines
 */
#define TIME_TO_START (10.0f) //Seconds //TODO: set reasonable
#define SCROLL_FACTOR (30.0f) //TODO: decide whether this gets progressively faster

#define SHOW_SCORE (0)

#define NUM_EXPIRED_LOST (20)

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
static PersonArray personArray;
static PersonArray* array = &personArray;


/*
 * Static functions
 */
static void gui_handleMatch(Person* person);
static void gui_handleExpired(Person* person);
static Texture2D gui_getIconForInterest(Interests interest, bool bw);
static Vector2 gui_getLineAnchor(Person* person);
static Rectangle gui_getCharacterFileRect(Person* person);
static bool gui_lost(void);

/*
 * Implementation
 */

void gui_init(void) {
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
    if (!personarray_create(array)) {
        fprintf(stderr, "Failed to create person array\n");
        exit(-1);
    }
}

void gui_updateGameTime(void) {
    if (!gui_lost()) {
        elapsedTime += GetFrameTime();
    }
}

void gui_reset(void) {
    score = 0;
    expired_index = 0;
    index_visible = 0;
    num_expired = 0;
    startDragPerson = NULL;
    waitForRelease = false;
    elapsedTime = -TIME_TO_START;
    //Initial filling of array
    personarray_clear(array);
    for (size_t i = 0; i < 200; i++) {
        personarray_add(array, person_create());
    }
}

// Draw two scrolling lists of persons
void gui_drawPersonlist(void) {
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
        Color bg = WHITE;
        if (gui_lost()) {
            tint = LIGHTGRAY;
            bg = LIGHTGRAY;
        }
        if (person->expired) {
            bg = GRAY;
        } else if (person->partner) {
            if (person->happy) {
                bg = GREEN;
            } else {
                bg = RED;
            }
        }
        //Draw character
        DrawRectangleRec(gui_getCharacterFileRect(person), bg);
        gui_drawTextureScaledToSize(character, x, y, CHARACTER_RECT, tint);
        if (!person) {
            fprintf(stderr, "Invalid person requested, something is seriously wrong!!!\n");
            exit(-1);
        }
        x += CHARACTER_RECT;
        //Draw name
        DrawText(person->name, x, y + 20, 25, BLACK);
        //Draw interests the character has
        y += CHARACTER_RECT - INTEREST_RECT;
        int num = 0;
        for (int i = 0; i < NUM_INTERESTS; i++) {
            bool bright = true;
            if (person->has & (1 << i)) {
                Texture2D texture = gui_getIconForInterest(1 << i, !bright);
                gui_drawTextureScaledToSize(texture, x + (num % (NUM_INTERESTS / 2))* INTEREST_RECT, y, INTEREST_RECT, tint);
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
            }
        }
    }
}

State gui_drawInterface(void) {
    //Score and expired
    DrawRectangle(0, 0, GetScreenWidth(), INTERFACE_HEIGHT, WHITE);
    DrawRectangleLines(0, 0, GetScreenWidth(), INTERFACE_HEIGHT, BLACK);
    int y = INTERFACE_HEIGHT / 2;
    if (gui_lost()) {
        const char* msg = "GAME OVER!";
        const int fontSize = 50;
        const int padding = 3;
        DrawRectangle((GetScreenWidth() - MeasureText(msg, fontSize)) / 2 - padding, (GetScreenHeight()) / 2 - padding, MeasureText(msg, fontSize) + 2 * padding, fontSize + 2 * padding, WHITE);
        DrawText(msg, (GetScreenWidth() - MeasureText(msg, fontSize)) / 2 , GetScreenHeight() / 2, fontSize, BLACK);
        if(GuiButton((Rectangle){.x = GetScreenWidth() / 2 - 50, .y = GetScreenHeight() / 2 + 50, .width = 100, .height = 30}, "Menu")) {
            return STATE_MENU;
        }
    }
    //Start timer
    if (elapsedTime < 0.0f) {
        char buffer[20];
        snprintf(buffer, 20, "%i", (int)fabsf(elapsedTime));
        DrawText(buffer, GetScreenWidth() / 2, y, 20, BLACK);
    }

    char buffer[1024];
    snprintf(buffer, 1024, "Sad Characters: %li/%i", num_expired, NUM_EXPIRED_LOST);
    DrawText(buffer, MARGIN, y, 20, BLACK);
    snprintf(buffer, 1024, "Score: %lli", score);
    DrawText(buffer, GetScreenWidth() - 200, y, 20, BLACK);

    //Expiry line
    y = INTERFACE_HEIGHT + CHARACTER_RECT;
    DrawLine(0, y, GetScreenWidth(), y, RED);
    DrawRectangleGradientV(0, INTERFACE_HEIGHT, GetScreenWidth(), CHARACTER_RECT, RED, (Color){.r = 255, .a = 10});
    return STATE_GAME;
}

void gui_handleInput(void) {
    if (gui_lost()) {
        return;
    }
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonDown(0)) {
        if (waitForRelease) {
            return;
        }
        if (startDragPerson && startDragPerson->expired) {
            waitForRelease = true;
            return;
        }
        //Draw a cool line or soemthing
        if (startDragPerson) {
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
#if SHOW_SCORE == 1
                } else if (startDragPerson && p != startDragPerson) { //If we hover over a person show potential score
                    char buffer[40];
                    snprintf(buffer, 40, "%lli", person_getScoreBetween(startDragPerson, p));
                    const int mouseOffset = 10;
                    const int size = 20;
                    const int offset = 2;
                    DrawRectangle(mouse.x - offset, mouse.y - size / 2 - offset - mouseOffset, MeasureText(buffer, size) + 2 * offset, size + 2 * offset, WHITE);
                    DrawText(buffer, mouse.x, mouse.y - size / 2 - mouseOffset, size, BLACK);
#endif
                }
            }
        }
    } else if (IsMouseButtonReleased(0)) {
        if (startDragPerson && !startDragPerson->partner && !startDragPerson->expired) {
            for (size_t i = expired_index; i < array->count; i++) {
                Person* p = personarray_get(array, i);
                Rectangle r = gui_getCharacterFileRect(p);
                if (CheckCollisionPointRec(mouse, r)) {
                    if (p->partner || p->expired) {
                        continue;
                    }
                    if (p != startDragPerson) {
                        person_match(startDragPerson, p);
                        gui_handleMatch(p);
                        startDragPerson = NULL;
                        waitForRelease = true;
                    }
                }
            }
        }
    } else {
        startDragPerson = NULL;
        waitForRelease = false;
    }
}

static void gui_handleMatch(Person* person) {
    if (!person->happy) {
        num_expired += 2;
    } else {
        score += person_getScore(person);
    }
}

static void gui_handleExpired(Person* person) {
    person->expired = true;
    num_expired += 1;
}

static Texture2D gui_getIconForInterest(Interests interest, bool bw) {
    if (!bw) {
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
    } else {
        switch (interest) {
            case COOKING:
                return cooking_potBW;
                break;
            case HANDYWORK:
                return hammerBW;
                break;
            case PAINT:
                return paint_brushBW;
                break;
            case GAME:
                return gamepadBW;
                break;
            case LAZY:
                return slothBW;
                break;
            case PARTY:
                return party_popperBW;
                break;
            case SPORT:
                return basketball_jerseyBW;
                break;
            case WATER_SPORT:
                return wave_surferBW;
                break;
            case READ:
                return white_bookBW;
                break;
            case GARDEN:
                return gardening_shearsBW;
                break;
            case TRAVEL:
                return journeyBW;
                break;
            case SHOPPING:
                return shopping_bagBW;
                break;
            default: 
                fprintf(stderr, "Requested invalid interest\n");
                exit(-1);
        }
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

static bool gui_lost(void) {
    return num_expired >= NUM_EXPIRED_LOST;
}
