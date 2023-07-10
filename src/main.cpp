#include <stdio.h>
#include <iostream>
#include "SDL.h"
#include "SDL_timer.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


bool init() 
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
    {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
	else
	{
		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL ) 
        {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        // Setup renderer 
        renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    }
    return true;
}

void loop () 
{
    const float a = 1;
    bool running = true;
    while (running) {

        // Clear renderer 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

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
        SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

        // Render rect
        SDL_RenderFillRect( renderer, &r );

        // Render the rect to the screen
        SDL_RenderPresent(renderer);		
    }	
    return;
}

void quit () 
{
	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

    return;
}


int main( int argc, char* args[] )
{
    if ( !init() ) {
        printf("Err: Initialization failed\n");
        return 1;
    }

    loop();

    quit();
    return 0;
}
