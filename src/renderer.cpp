#include "app.h"
#include "body.h"
#include "environment.h"

void Application::render(const Environment& env) {
    // Clear renderer 
    SDL_SetRenderDrawColor( m_renderer, 255, 255, 255, 255);
    SDL_RenderClear( m_renderer );


    // Render each body as rectangle.
    for (const Body& body : env.getBodies()) {
        auto pos  = body.getPos ();
        auto size = body.getSize();

        // Creat a rect to represent the body
        SDL_Rect rect;
        rect.x = pos.x - size.x/2;
        rect.y = (SCREEN_HEIGHT - pos.y) - size.y/2;
        rect.w = size.x;
        rect.h = size.y;

        // Set render color to blue ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( m_renderer, 100, 100, 100, 255 );

        // Render rect
        SDL_RenderFillRect( m_renderer, &rect );
    }

    // Actually Render
    SDL_RenderPresent( m_renderer );
}
