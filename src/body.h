#ifndef __BODY_h__
#define __BODY_h__

#include "collision.h"
#include "core.h"

#include <vector>


class Body 
{
    bool gravity;

    double mass;
    double invMass;

    std::vector<Vec2> points;

    Vec2 pos;
    Vec2 accl{0, 0};
    Vec2 velo{0, 0};

public: 
     Body(double x, double y, std::vector<Vec2> points, double mass, bool gravity = true) : 
         gravity(gravity),
         mass(mass), 
         invMass(1 / mass),
         points(points), 
         pos(x, y)
    {    
        // Must be a polygon
        if (points.size() == 3) throw "Not a polygon";
        invMass = 1 / mass;
    };

    ~Body() {};

    void applyForce (const Vec2& f);
    void update(const double dt);

    // Getters
    double getMass () const { return mass; }
    bool getGravity() const { return gravity; }
    Vec2 getPos  () const { return pos; }
    Vec2 getVelo () const { return velo; }
    Vec2 getSize () const { return Vec2(50, 50); }
    const std::vector<Vec2>& getPoints () const { return points; }

    static void resolve(Body& b1, Body& b2, const Collision& collision); 
    static Body makeRect(double x, double y, double w, double h, double m, bool gravity = true);
    static Body makeDiamond(double x, double y, double r, double m);
};

#endif
