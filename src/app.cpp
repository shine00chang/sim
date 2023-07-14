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

// Check & Update state according to events.
void Application::updateEvents() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev) != 0) {
        switch( ev.type ){
        
        // If app quit
        case SDL_QUIT:
            m_running = false;
            break;

        // Update keys
        case SDL_KEYDOWN:
            m_keys.insert(ev.key.keysym.sym);
            break;

        case SDL_KEYUP:
            m_keys.erase(ev.key.keysym.sym);
            break;
        }
    }
}
