#ifndef GUI_H
#define GUI_H

#include "personarray.h"
#include "state.h"

void gui_init(void);

void gui_updateGameTime(void);
void gui_reset(void);
void gui_drawPersonlist(void);
State gui_drawInterface(void);
void gui_handleInput(void);

#endif //GUI_H
