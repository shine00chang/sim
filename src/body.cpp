#include "body.h"

void Body::applyForce(const Vec2 &f) {
    accl = accl + f * invMass;
}

void Body::update(const double dt) {
    //printf("(%lf, %lf) -> ", pos.x, pos.y);
    // Integrate Acceleration
    velo = velo + accl * dt;

    // Integrate Velocity
    auto delta = velo * dt;
    //printf("delta: (%lf, %lf) delta + pos: (%lf, %lf)\n", delta.x, delta.y, (pos+delta).x, (pos+delta).y);
    pos = pos + velo * dt;

    //printf("(%lf, %lf)\n", pos.x, pos.y);
}
