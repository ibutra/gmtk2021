#ifndef PERSONARRAY_H
#define PERSONARRAY_H

#include <stdlib.h>
#include <stdbool.h>
#include "person.h"

typedef struct {
    Person* persons;
    size_t count;
    size_t capacity;
} PersonArray;

bool personarray_create(PersonArray* out);
bool personarray_add(PersonArray* array, Person person);
const Person* personarray_get(PersonArray* array, size_t index);

#endif //PERSONARRAY_H
