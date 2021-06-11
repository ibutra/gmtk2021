#include "person.h"

#include <stdlib.h>
#include "names.h"

Person person_create(void) {
    unsigned int r = rand()/((RAND_MAX)/nameListNum);
    Person p = {
        .name = nameList[r],
        .wants = 0,
        .has = 0,
    };
    return p;
}
