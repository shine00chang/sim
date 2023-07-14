#include "SDL_render.h"
#include "app.h"
#include "body.h"
#include "environment.h"

void Application::render(const Environment& env) {
    // Clear renderer 
    SDL_SetRenderDrawColor( m_renderer, 255, 255, 255, 255);
    SDL_RenderClear( m_renderer );

    // Render each body as polygon 
    for (const Body& body : env.getBodies()) {
        auto pos  = body.getPos ();

        // Set render color to grey ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( m_renderer, body.color.r, body.color.g, body.color.b, body.color.a);

        auto points = body.getPoints();
        for (int i=0; i<points.size(); i++) {
            Vec2 a = pos + points[i];
            Vec2 b = pos + points[i == points.size()-1 ? 0 : i+1];

            SDL_RenderDrawLine( m_renderer, a.x, SCREEN_HEIGHT - a.y, b.x, SCREEN_HEIGHT - b.y);
        }
    }

    // Actually Render
    SDL_RenderPresent( m_renderer );
}
