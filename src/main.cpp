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

int main( int argc, char* args[] )
{
    Application* app;

    try {
        app = new Application();

    } catch (std::string err) {
        printf("App initialization failed with: '%s'\n", err.c_str());
        return 1;
    }

    auto env = TestEnvironments::testEnv3();
    loop(*app, env);

    return 0;
}
