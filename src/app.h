#ifndef __APP_h__
#define __APP_h__

#include "SDL.h"
#include "SDL_keycode.h"
#include "environment.h"

#include <set>

enum Colors {
    Red, 
    Blue, 
    Green,
    Black
};
extern std::vector<std::pair<Vec2, Colors>> debugPoints;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Application {
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    std::set<SDL_Keycode> m_keys;
    bool m_running = true;

public: 
     Application();
    ~Application();

    void render(const Environment& env);
    void updateEvents ();

    bool isPressed (SDL_Keycode k) const { return m_keys.count(k); }
    bool isRunning () const { return m_running; }

    SDL_Window* window () { return m_window; }
};

#endif
