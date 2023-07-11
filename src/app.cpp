#include "app.h"
#include "SDL.h"
#include "SDL_render.h"

#include <string>
#include <format>


Application::Application () {
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
		throw "SDL could not initialize!\nSDL_Error: {}\n"; // SDL_GetError();
                                                            //
    //Create window
    m_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( m_window == NULL ) 
        throw "Window could not be created! SDL_Error: %s\n"; // SDL_GetError();
                                                              //
    // Setup renderer 
    m_renderer =  SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED);
}

Application::~Application () {
    //Destroy window
	SDL_DestroyWindow( m_window );

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Window*   Application::window()   { return m_window; }
