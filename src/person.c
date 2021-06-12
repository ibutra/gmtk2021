#include "person.h"

#include <stdio.h>
#include <stdlib.h>
#include "names.h"

static Interests person_generateInterest(void);

Person person_create(void) {
    unsigned int r = rand()/((RAND_MAX)/nameListNum);//Not equally distributed but eh
    Person p = {
        .name = nameList[r],
        .wants = person_generateInterest(),
        .has = person_generateInterest(),
        .expired = false,
        .position = {.x = 0, .y = 0},
        .partner = NULL,
        .index = 0,
    };
    return p;
}

static Interests person_generateInterest(void) {
    //I want 5 of the possible interests
    Interests result = 0;
    int i = 0;
    while(true) {
        int r = rand() % NUM_INTERESTS; //Not equally distributed but eh
        if (result & (1 << r)) {
            continue;
        }
        result |= (1 <<r);
        i++;
        if (i == 5) {
            break;
        }
    }
    return result;
}

uint64_t person_getScore(Person* person) {
    return person->partner? 1 : 0; //TODO: come up with some fancy ranking based on matched interests
}
