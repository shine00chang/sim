#include <stdio.h>
#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_timer.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	
	// Setup renderer
    SDL_Renderer* renderer = NULL;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
    {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
	else
	{
		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		else
		{
            // Setup renderer 
            renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

            constexpr int a = 1;
            constexpr int t = 1;
            /*
            // Render moving block
            for (int t=0; t<500; t++) 
            {
            */
                // Set render color to red, then clear ( background will be rendered in this color )
                SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
                SDL_RenderClear( renderer );

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
                
                // Delay for next frame
                SDL_Delay(100);
            //}
            
            //Hack to get window to stay up
            SDL_Event e;
            bool quit = false; 
            while( quit == false ) 
               while( SDL_PollEvent( &e ) )
                    if( e.type == SDL_QUIT )
                        quit = true;
		}
	}

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

