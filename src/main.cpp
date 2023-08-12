#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_keyboard.h"
#include "app.h"
#include "environment.h"
#include "controller.h"
#include "testEnv.h"

#include <ostream>
#include <stdio.h>
#include <iostream>
#include <utility>

constexpr double GRAVTIY = -20;

void loop (Application app, View view, Environment env) 
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
        for (Body& body : env.getBodiesMut()) 
        {
            // Controllers
            body.runControllers(app);

            // Gravity
            if (body.getGravity())
                body.applyForce(Vec2(0, GRAVTIY * body.getMass()));

            // Integration
            body.accumulateForces(dt);
        }
        

        // Collision
        env.collide(dt);

        // Update
        for (Body& body : env.getBodiesMut()) 
        {
            body.update(dt);
        }


        // Render
        app.render(view, env);
    }
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

    View view = View();
    Environment env = TestEnvironments::testEnv1();
    loop(*app, view, env);

    return 0;
}
