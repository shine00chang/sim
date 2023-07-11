#include "SDL.h"
#include "SDL_timer.h"
#include "app.h"
#include "environment.h"

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
                if (!body.fixed)
                    body.applyForce(Vec2(0, -9.8 * body.getMass()));

                // Update (Integrate)
                body.update(dt);

                printf("(%lf, %lf)\n", body.getPos().x, body.getPos().y);
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
    Application* App;
    Environment* Env;

    try {
        App = new Application();
        Env = new Environment();
        
        Body body(300, 0, 50, 50, 10);
        Env->addBody(body);
    } catch (std::string err) {
        printf("App initialization failed with: '%s'\n", err.c_str());
        return 1;
    }

    loop(*App, *Env);

    return 0;
}
