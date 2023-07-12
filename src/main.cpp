#include "SDL.h"
#include "SDL_timer.h"
#include "app.h"
#include "environment.h"

#include <ostream>
#include <stdio.h>
#include <iostream>


void loop (Application app, Environment env) 
{
    uint64_t prevIterMs = SDL_GetTicks64();
    bool running = true;
    while (running) 
    {
        // Check quit event
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
        }
        // Get Dt
        const uint64_t ms = SDL_GetTicks64(); 
        const double   dt = (ms - prevIterMs) / 1000.0; 
        prevIterMs = ms;

        // Movement
        {
            for (Body& body : env.getBodiesMut()) {

                // Gravity
                if (body.getGravity())
                    body.applyForce(Vec2(0, -9.8 * body.getMass()));

                body.update(dt);
            }
        }

        // Collision
        env.collide();

        /*
        for (auto& body : env.getBodiesMut())
            body.update(dt);
         */

        // Render
        app.render(env);
    }
}


int main( int argc, char* args[] )
{
    Application* App;
    Environment* Env;

    try {
        App = new Application();
        Env = new Environment();
        
        Body r1 = Body::makeRect(300, 500, 50, 50, 10);
        Body r2 = Body::makeRect(300, 200, 50, 50, 10);
        r1.applyForce(Vec2{0, -4000});

        Body floor = Body::makeRect(340, 0, 680, 50, 1e10, false);

        Env->addBody(r1);
        Env->addBody(r2);
        Env->addBody(floor);

    } catch (std::string err) {
        printf("App initialization failed with: '%s'\n", err.c_str());
        return 1;
    }

    loop(*App, *Env);

    return 0;
}
