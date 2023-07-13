#include "body.h"
#include <memory>

void Body::applyForce(const Vec2 &f) {
    accl = accl + f * invMass;
}

void Body::update(const double dt) {
    // Integrate Acceleration
    velo = velo + accl * dt;

    // Integrate Velocity
    pos = pos + velo * dt;

    accl = Vec2{0, 0};
}

std::unique_ptr<Body> Body::makeRect(double x, double y, double w, double h, double m, bool gravity) {
    std::vector<Vec2> v{
        Vec2(-w/2, -h/2),
        Vec2( w/2, -h/2),
        Vec2( w/2,  h/2),
        Vec2(-w/2,  h/2),
    };
    std::unique_ptr<Body> ptr( new Body(x, y, v, m, gravity) );

    return ptr;
}

std::unique_ptr<Body> Body::makeDiamond(double x, double y, double r, double m) {
    std::vector<Vec2> v{
        Vec2( 0, -r),
        Vec2( r,  0),
        Vec2( 0,  r),
        Vec2(-r,  0),
    };
    std::unique_ptr<Body> ptr( new Body(x, y, v, m) );

    return ptr;
}
