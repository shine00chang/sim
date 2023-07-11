#include "app.h"
#include "body.h"
#include "environment.h"

void Application::render(const Environment& env) {
    // Clear renderer 
    SDL_SetRenderDrawColor( m_renderer, 255, 255, 255, 255);
    SDL_RenderClear( m_renderer );


    // Render each body as rectangle.
    for (const Body& body : env.bodies()) {
        // Creat a rect to represent the body
        SDL_Rect rect;
        rect.x = body.min.x;
        rect.y = body.min.y;
        rect.w = body.max.x - body.min.x;
        rect.h = body.max.y - body.min.y;

        // Set render color to blue ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( m_renderer, 100, 100, 100, 255 );

        // Render rect
        SDL_RenderFillRect( m_renderer, &rect );
    }

    // Actually Render
    SDL_RenderPresent( m_renderer );
}
