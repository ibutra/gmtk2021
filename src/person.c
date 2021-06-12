#include "person.h"

#include <stdio.h>
#include <stdlib.h>
#include "names.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define NUM_WANTS (3)
#define NUM_HAS   (5)

#define CHANCE_HAPPY_0 (0)
#define CHANCE_HAPPY_1 (20)
#define CHANCE_HAPPY_2 (30)
#define CHANCE_HAPPY_3 (75)
#define CHANCE_HAPPY_4 (95)
#define CHANCE_HAPPY_5 (100)
#define CHANCE_HAPPY_6 (100)
static int person_probabilities[] = {CHANCE_HAPPY_0, CHANCE_HAPPY_1, CHANCE_HAPPY_2, CHANCE_HAPPY_3, CHANCE_HAPPY_4, CHANCE_HAPPY_5, CHANCE_HAPPY_6};

static Interests person_generateInterest(int num);
static uint64_t person_getCount(Interests interests);

Person person_create(void) {
    unsigned int r = rand()/((RAND_MAX)/nameListNum);//Not equally distributed but eh
    Person p = {
        .name = nameList[r],
        .has = person_generateInterest(NUM_HAS),
        .expired = false,
        .position = {.x = 0, .y = 0},
        .partner = NULL,
        .index = 0,
    };
    return p;
}

bool person_match(Person* a, Person* b)  {
    a->partner = b;
    b->partner = a;
    uint64_t score = person_getScoreBetween(a, b);
    score = MIN(MAX(0, score), sizeof(person_probabilities) / sizeof(int));
    int probability = person_probabilities[score];
    int v = rand() % 100;
    printf("Score: %lli P: %i Target: %i\n", score, v, probability);
    if (v < probability) {
        a->happy = true;
        b->happy = true;
        return true;
    }
    return false;
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
    /* Interests combined = a->wants & b->has; */
    /* score += person_getCount(combined); */
    /* combined = a->has & b->wants; */
    /* score += person_getCount(combined); */
    Interests combined = a->has & b->has;
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
