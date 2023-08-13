#ifndef __APP_h__
#define __APP_h__

#include "SDL.h"
#include "SDL_keycode.h"
#include "environment.h"
#include "view.h"

#include <memory>
#include <set>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Application {
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    std::set<SDL_Keycode> m_keys;
    bool m_running = true;

    void loop (View view, Environment env);
    void updateEvents ();
public: 


     Application();
    ~Application();

    void start(Environment env);

    bool isPressed (SDL_Keycode k) const { return m_keys.count(k); }
    bool isRunning () const { return m_running; }
};

#endif
