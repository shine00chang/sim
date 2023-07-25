#ifndef __COLLISION_h__
#define __COLLISION_h__

#include "core.h"

struct Collision {
    Vec2 norm;
    double depth;
    Vec2 contactPoint;

    Collision (const Vec2& norm, double depth) : norm(norm), depth(depth), contactPoint(Vec2(0,0)) {};
};

#endif
