#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_keyboard.h"
#include "app.h"
#include "environment.h"
#include "controller.h"

#include <ostream>
#include <stdio.h>
#include <iostream>
#include <utility>


void loop (Application app, Environment env) 
{
    constexpr double dt = 0.016;
    uint64_t prevIterMs = SDL_GetTicks64();

    while (app.isRunning()) 
    {

        // Check quit event
        app.updateEvents();

        // Get Dt
        const uint64_t ms = SDL_GetTicks64(); 
        if (ms - prevIterMs < dt * 1000) continue;
        prevIterMs = ms;

        // Movement
        {
            for (Body& body : env.getBodiesMut()) 
            {
                // DEBUG: Set color to red on collision: Reset
                body.color = SDL_Color{0,0,0};
                
                // Controllers
                body.runControllers(app);

                // Gravity
                if (body.getGravity())
                    body.applyForce(Vec2(0, -9.8 * body.getMass()));

                // Integration
                body.update(dt);
            }
        }

        // Collision
        env.collide();

        // Render
        app.render(env);
    }
}

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
}

/*
 * A floor and two blocks, one with some downward initial velocity
 * This tests for gravity, basic collision, and sinking.
 */ 
Environment testEnv1() {
    Environment env; 

    auto r1 = Body::makeRect(300, 500, 50, 50, 10);
    auto r2 = Body::makeRect(300, 200, 50, 50, 10);
    r1->applyForce(Vec2{0, -4000});

    auto floor = Body::makeRect(340, 0, 680, 50, 1e10, false);

    env.addBody(*r1.release());
    env.addBody(*r2.release());
    env.addBody(*floor.release());

    return env;
}

/*
 * Two blocks, one with a force controller, and no gravity.
 * Tests for collision detection & resolution.
 */ 
Environment testEnv2() {
    Environment env;

    auto r1 = Body::makeRect(300, 300, 50, 50, 10, false);
    auto r2 = Body::makeRect(200, 200, 50, 50, 10, false);

    r1->useController(debugController);

    env.addBody(*r1.release());
    env.addBody(*r2.release());


    return env;
}

int main( int argc, char* args[] )
{
    Application* app;

    try {
        app = new Application();

    } catch (std::string err) {
        printf("App initialization failed with: '%s'\n", err.c_str());
        return 1;
    }

    auto env = testEnv2();
    loop(*app, env);

    return 0;
}
