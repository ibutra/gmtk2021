#ifndef PERSON_H
#define PERSON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <raylib.h>

#define NUM_INTERESTS (12)
typedef enum {
    COOKING     = (1 << 0),
    HANDYWORK   = (1 << 1),
    PAINT       = (1 << 2),
    GAME        = (1 << 3),
    LAZY        = (1 << 4),
    PARTY       = (1 << 5),
    SPORT       = (1 << 6),
    WATER_SPORT = (1 << 7),
    READ        = (1 << 8),
    GARDEN      = (1 << 9),
    TRAVEL      = (1 << 10),
    SHOPPING    = (1 << 11),
} Interests;

typedef struct Person{
    const char* name;
    Interests has;
    bool expired;
    Vector2 position;
    struct Person* partner;
    bool happy;
    size_t index; //Meh
} Person;

Person person_create(void);
bool person_match(Person* a, Person* b); //Returns true if they stay happily ever after
uint64_t person_getScore(Person* person);
uint64_t person_getScoreBetween(Person* a, Person* b);

#endif //PERSON_H
