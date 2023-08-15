#include "app.h"
#include "environment.h"

#include <ostream>


/* Gravity & Floor. 
 * Creates random falling block when space bar is pressed
 */
void envController (Environment* env, const Application& app) 
{
    // Spawn in new block
    if (app.mouseClicked()) {
        double x = app.mouse().x;
        double y = SCREEN_HEIGHT - app.mouse().y;

        auto b = Body::makeRect(x, y, 50, 50, 10);
        env->addBody(*b.release());
    }

    // Launch Bird
    if (app.isPressed(SDLK_SPACE)) {
        auto b = Body::makeRect(0, 100, 30, 30, 100);
        b->applyForce(Vec2(1000000, 400000));

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
    env.addController(envController);

    return env;
}


void setEnvs () {
    DefaultEnv = "angrybirds";
    EnvironmentLibrary = {
        {"angrybirds", env()}
    };
}
