#ifndef __BODY_h__
#define __BODY_h__

#include "SDL.h"
#include "collision.h"
#include "core.h"
#include "controller.h"

#include <vector>
#include <memory>

class Application;
class Body 
{
    bool gravity = true;

    double mass;
    double invMass;
    double intertia;
    double invInertia;

    std::vector<Vec2> points;

    Vec2 pos;
    Vec2 velo{0, 0};
    Vec2 accl{0, 0};

    double orient = 0;
    double angVelo = 0;
    double angAccl = 0;

    std::vector<Controller> m_controllers;

public: 
    SDL_Color color {0,0,0}; 

     Body(double x, double y, std::vector<Vec2> points, double mass, double intertia) : 
         mass(mass), 
         invMass(1 / mass),
         intertia(intertia),
         invInertia(1 / intertia),
         points(points), 
         pos(x, y)
    {    
        // Must be a polygon
        if (points.size() == 3) throw "Not a polygon";
        invMass = 1 / mass;
    };

    ~Body() {};

    void useController  (Controller controller) { m_controllers.push_back(controller); }
    void runControllers (const Application& app); 

    void applyForce (const Vec2& f);
    void update(const double dt);

    // Getters
    double getMass  () const { return mass; }
    bool getGravity () const { return gravity; }
    Vec2 getPos     () const { return pos; }
    Vec2 getVelo    () const { return velo; }
    double getAngAccl () const { return angAccl; }

    // Point getters, has transformation logic 
    const std::vector<Vec2>& getPointsRaw  () const { return points; }
    const std::vector<Vec2> getPointsLocal () const { 
        auto v = points;
        for (Vec2& p : v) 
            p = p.rotate(orient);
        return v;
    }
    const std::vector<Vec2> getPointsGlobal () const { 
        auto v = points;
        for (Vec2& p : v) 
            p = p.rotate(orient) + pos;
        return v;
    }
    
    // Setter
    void setGravity (bool b)        { gravity = b; }
    void setPos     (const Vec2& v) { pos = v; }
    void setVelo    (const Vec2& v) { velo = v; } 
    void setAngAccl (double a)      { angAccl = a; }

    // Collision Resolver 
    static void resolve(Body& b1, Body& b2, const Collision& collision); 

    // Convenience factories
    static std::unique_ptr<Body> makeRect(double x, double y, double w, double h, double m, double i);
    static std::unique_ptr<Body> makeDiamond(double x, double y, double r, double m, double i);
};

#endif
