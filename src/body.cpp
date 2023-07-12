#include "body.h"

void Body::applyForce(const Vec2 &f) {
    accl = accl + f * invMass;
}

void Body::update(const double dt) {
    // Integrate Acceleration
    velo = velo + accl * dt;

    // Integrate Velocity
    pos = pos + velo * dt;
}
