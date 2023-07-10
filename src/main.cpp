#include <stdio.h>
#include <iostream>
#include "SDL.h"
#include "SDL_timer.h"
#include "app.h"

void loop (Application App) 
{
    const float a = 1;
    bool running = true;
    while (running) {

        // Clear renderer 
        SDL_SetRenderDrawColor( App.renderer(), 255, 255, 255, 255);
        SDL_RenderClear( App.renderer());

        // Check quit event
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
        }

        // Get ticks
        Uint32 t = SDL_GetTicks() / 100;
        printf("ticks: %d\n", t);

        // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
        SDL_Rect r;
        r.x = 2 * a * t;
        r.y = 2 * a * t * t;
        r.w = 50;
        r.h = 50;

        // Set render color to blue ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( App.renderer(), 0, 0, 255, 255 );

        // Render rect
        SDL_RenderFillRect( App.renderer(), &r );

        // Render the rect to the screen
        SDL_RenderPresent( App.renderer());
    }	
    return;
}

int main( int argc, char* args[] )
{
    Application* App;
    try {
        App = new Application();
    } catch (std::string err) {
        printf("App initialization failed with: '%s'\n", err.c_str());
        return 1;
    }

    loop(*App);

    return 0;
}
