#ifndef PERSON_H
#define PERSON_H

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

typedef struct {
    const char* name;
    Interests wants;
    Interests has;
}Person;

Person person_create(void);

#endif //PERSON_H
