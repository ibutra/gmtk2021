#include "personarray.h"

#include <stdio.h>

#define INITIAL_CAPACITY (100)

bool personarray_create(PersonArray* out) {
    if (!out) {
        return false;
    }
    Person* storage = malloc(INITIAL_CAPACITY * sizeof(Person));
    if (!storage) {
        return false;
    }
    out->persons = storage;
    out->count = 0;
    out->capacity = INITIAL_CAPACITY;
    return true;
}

bool personarray_add(PersonArray* array, Person person) {
    if (!array) {
        return false;
    }
    if (array->count == array->capacity) {
        size_t newCapacity = array->capacity * 2;
        Person* newArray = realloc(array->persons, newCapacity * sizeof(Person));
        if (!newArray) {
            fprintf(stderr, "Failed to increase PersonArray\n");
            return false;
        }
        array->persons = newArray;
        array->capacity = newCapacity;
    }
    array->persons[array->count] = person;
    array->persons[array->count].index = array->count;
    array->count++;
    return true;
}

Person* personarray_get(PersonArray* array, size_t index) {
    if (!array) {
        return NULL;
    }
    if (index >= array->count) {
        return NULL;
    }
    return &array->persons[index];
}

void personarray_clear(PersonArray* array) {
    array->count = 0;
}
