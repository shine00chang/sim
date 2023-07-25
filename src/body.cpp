#include "body.h"
#include "app.h"
#include <memory>


// Apply force
void Body::applyForce(const Vec2 &f) {
    accl = accl + f * invMass;
}

// Integration
void Body::update(const double dt) {
    // Integrate Acceleration
    velo = velo + accl * dt;

    // Integrate Velocity
    pos = pos + velo * dt;

    // Reset Accl
    accl = Vec2{0, 0};

    // Integrate Ang Accl
    angVelo += angAccl * dt;

    // Integrate Ang Velocity
    orient += angVelo * dt;

    angAccl = 0;
}

// Have controllers act on object. 
void Body::runControllers(const Application &app) {
    for (auto c : m_controllers) 
        c(this, app);
}

// Convenience Factory function
std::unique_ptr<Body> Body::makeRect(double x, double y, double w, double h, double m, double i) {
    std::vector<Vec2> v{
        Vec2(-w/2, -h/2),
        Vec2( w/2, -h/2),
        Vec2( w/2,  h/2),
        Vec2(-w/2,  h/2),
    };
    std::unique_ptr<Body> ptr( new Body(x, y, v, m, i) );

    return ptr;
}

// Convenience Factory function
std::unique_ptr<Body> Body::makeDiamond(double x, double y, double r, double m, double i) {
    std::vector<Vec2> v{
        Vec2( 0, -r),
        Vec2( r,  0),
        Vec2( 0,  r),
        Vec2(-r,  0),
    };
    std::unique_ptr<Body> ptr( new Body(x, y, v, m, i) );

    return ptr;
}


