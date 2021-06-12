#ifndef GUI_H
#define GUI_H

#include "personarray.h"

void gui_initPersonIcons(void);

void gui_updateGameTime(void);
void gui_drawPersonlist(PersonArray* array);
void gui_drawInterface(PersonArray* array);
void gui_handleInput(PersonArray* array);

#endif //GUI_H
