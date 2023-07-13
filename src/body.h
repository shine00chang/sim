#ifndef __BODY_h__
#define __BODY_h__

#include "collision.h"
#include "core.h"

#include <vector>
#include <memory>


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
    double getMass  () const { return mass; }
    bool getGravity () const { return gravity; }
    Vec2 getPos     () const { return pos; }
    Vec2 getVelo    () const { return velo; }
    Vec2 getSize    () const { return Vec2(50, 50); } // TODO: CHANGE
    const std::vector<Vec2>& getPoints () const { return points; }
    
    // Setter
    void setGravity (bool b)        { gravity = b; }
    void setPos     (const Vec2& v) { pos = v; }
    void setVelo    (const Vec2& v) { velo = v; } 

    // Collision Resolver 
    static void resolve(Body& b1, Body& b2, const Collision& collision); 

    // Convenience factories
    static std::unique_ptr<Body> makeRect(double x, double y, double w, double h, double m, bool gravity = true);
    static std::unique_ptr<Body> makeDiamond(double x, double y, double r, double m);
};

#endif
