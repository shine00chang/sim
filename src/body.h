#ifndef __BODY_h__
#define __BODY_h__

#include "core.h"

#include <vector>


class Body 
{
    double mass;
    double invMass;

    Vec2 pos;
    Vec2 size;
    Vec2 accl;
    Vec2 velo;

public: 
    bool fixed;

     Body(double x, double y, double w, double h, double mass) : 
         mass(mass), 
         pos(x,y), 
         size(w, h),
         accl(0, 0),
         velo(0, 0),
         fixed(false)
    {    
         invMass = 1 / mass;
    };
    ~Body() {};

    void applyForce (const Vec2& f);
    void update(const double dt);

    // Getters
    double getMass () const { return mass; }
    Vec2 getPos  () const { return pos; }
    Vec2 getSize () const { return size; }
};

#endif
