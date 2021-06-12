#include "person.h"

#include <stdio.h>
#include <stdlib.h>
#include "names.h"

static Interests person_generateInterest(void);
static uint64_t person_getCount(Interests interests);

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

uint64_t person_getScore(Person* person) {
    return person_getScoreBetween(person, person->partner);
}

uint64_t person_getScoreBetween(Person* a, Person* b) {
    if (!a || ! b) {
        return 0;
    }
    uint64_t score = 0;
    //Check my direction
    Interests combined = person->wants & partner->has;
    score += person_getCount(combined);
    combined = person->has & partner->wants;
    score += person_getCount(combined);
    return score;
}

static Interests person_generateInterest(int num) {
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
        if (i == num) {
            break;
        }
    }
    return result;
}

static uint64_t person_getCount(Interests interests) {
    uint64_t count = 0;
    for (int i = 0; i < NUM_INTERESTS; i++) {
        if (interests & (1 << i)) {
            count += 1;
        }
    }
    return count;
}
