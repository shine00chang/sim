#ifndef __TESTENV_h__
#define __TESTENV_h__

#include "environment.h"
#include "app.h"
#include <cmath>
#include <string>
#include <map>



/*
 * Debug / Test purposes. 
 * WASD to increment position. Arrow keys to apply force.
 */
void debugController (Body* body, const Application& app) {
    if (app.isPressed(SDLK_w)) {
        body->applyForce(Vec2(0, 100));
    }
    if (app.isPressed(SDLK_a)) {
        body->applyForce(Vec2(-100, 0));
    }
    if (app.isPressed(SDLK_s)) {
        body->applyForce(Vec2(0, -100));
    }
    if (app.isPressed(SDLK_d)) {
        body->applyForce(Vec2(100, 0));
    }
    if (app.isPressed(SDLK_q)) {
        body->setAngAccl( body->getAngAccl() + 0.1 );
    }
    if (app.isPressed(SDLK_e)) {
        body->setAngAccl( body->getAngAccl() - 0.1 );
    }
}

/*
 * A floor and two blocks, one with some downward initial velocity
 * This tests for gravity, basic collision, and sinking.
 */ 
Environment test1() {
    Environment env; 

    auto r1 = Body::makeRect(360, 400, 50, 50, 10);
    auto r2 = Body::makeRect(300, 330, 50, 50, 10);
    r1->applyForce(Vec2{0, -10000});
    r1->setOrient(std::atan(0.6));
    r2->setOrient(std::atan(1.5));

    auto floor = Body::makeRect(340, 0, 680, 50, 1e10);
    floor->setGravity(false);

    env.addBody(*r1.release());
    env.addBody(*r2.release());
    env.addBody(*floor.release());

    return env;
}

/*
 * Two blocks, one with a force controller, and no gravity.
 * Tests for collision detection & resolution.
 */ 
Environment test2() {
    Environment env;

    auto r1 = Body::makeRect(340, 300, 50, 50, 10);
    auto r2 = Body::makeRect(300, 200, 50, 50, 10);

    r1->setGravity(false);
    r2->setGravity(false);

    r1->useController(debugController);

    env.addBody(*r1.release());
    env.addBody(*r2.release());


    return env;
}

/*
 * A block and a diamond, one with a force controller, and no gravity.
 * Tests for collision detection on a non-AABB shape.
 */ 
Environment test3() {
    Environment env;

    auto r1 = Body::makeDiamond(300, 300, 50, 10);
    auto r2 = Body::makeRect(200, 200, 50, 50, 10); 

    r1->setGravity(false);
    r2->setGravity(false);

    r1->useController(debugController);

    env.addBody(*r1.release());
    env.addBody(*r2.release());


    return env;
}

/* Falling Diamond
 */ 
Environment fallingDiamond() {
    Environment env;

    auto r1 = Body::makeRect(340, 300, 50, 50, 10);
    r1->setOrient(std::atan(0.8));
    auto floor = Body::makeRect(340, 0, 680, 50, 1e10);
    floor->setGravity(false);

    env.addBody(*r1.release());
    env.addBody(*floor.release());

    return env;
}

/* Gravity & Floor. 
 * Creates random falling block when space bar is pressed
 */
Environment rainyDay () {
    Environment env;

    auto r1 = Body::makeRect(340, 300, 50, 50, 10);
    r1->setOrient(std::atan(0.8));
    r1->setGravity(false);

    env.addBody(*r1.release());

    return env;
}

void makeTestEnvs () {
    EnvironmentLibrary = {
        {"test1", test1()},
        {"test2", test2()},
        {"test3", test3()},
        {"fallingDiamond", fallingDiamond()},
        {"rainyDay", rainyDay()},
    };
}
#endif
