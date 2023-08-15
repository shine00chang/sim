#include "app.h"
#include "environment.h"

#include <ostream>


/* Gravity & Floor. 
 * Creates random falling block when space bar is pressed
 */
void rainyController (Environment* env, const Application& app) {
    if (app.isPressed(SDLK_SPACE)) {
        double x = std::rand() % 400 + 100;
        double y = std::rand() % 200 + 200;
        double s = std::rand() % 50 + 20;
        double ang = std::asin(std::rand() % 1000 / 1000.0);

        auto b = Body::makeRect(x, y, s, s, 10);
        b->setOrient(ang);
        env->addBody(*b.release());
    }
}


Environment env () {
    Environment env;

    auto r1 = Body::makeRect(340, 300, 50, 50, 10);
    r1->setOrient(std::atan(0.8));
    auto floor = Body::makeRect(340, 0, 680, 50, 1e10);
    floor->setGravity(false);

    env.addBody(*floor.release());
    env.addController(rainyController);

    return env;
}


void setEnvs () {
    DefaultEnv = "angrybirds";
    EnvironmentLibrary = {
        {"angrybirds", env()}
    };
}
