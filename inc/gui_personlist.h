#ifndef GUI_PERSONLIST_H
#define GUI_PERSONLIST_H

#include "personarray.h"

void gui_initPersonIcons(void);
void gui_drawPersonlist(PersonArray* array);
void gui_handleInput(PersonArray* array);

#endif //GUI_PERSONLIST_H
