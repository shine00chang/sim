#include "SDL.h"
#include "SDL_timer.h"
#include "app.h"
#include "environment.h"

#include <stdio.h>
#include <iostream>

void loop (Application App, Environment Env) 
{
    bool running = true;
    while (running) {

        // Check quit event
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
        }

        // Movement
        {

        }

        // Collision
        Env.collide();

        // Render
        App.render(Env);
    }
}


int main( int argc, char* args[] )
{
    Application* App;
    Environment* Env;

    try {
        App = new Application();
        Env = new Environment();
        
        Body body(200, 0, 50, 50);
        Env->addBody(body);
    } catch (std::string err) {
        printf("App initialization failed with: '%s'\n", err.c_str());
        return 1;
    }

    loop(*App, *Env);

    return 0;
}
