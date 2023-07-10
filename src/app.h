#ifndef __APP_h__
#define __APP_h__

#include "SDL.h"

#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Application {
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

public: 
     Application();
    ~Application();

    SDL_Window* window();
    SDL_Renderer* renderer();
};

#endif
