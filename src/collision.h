#ifndef __COLLISION_h__
#define __COLLISION_h__

#include "body.h"


struct Collision {
    Body* a, b;
    float depth;
};

#endif
