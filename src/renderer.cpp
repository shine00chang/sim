#include "SDL_render.h"
#include "app.h"
#include "body.h"
#include "environment.h"
#include <vector>

std::vector<std::pair<Vec2, Colors>> debugPoints;

void setColor ( SDL_Renderer* ren, Colors c) {
    switch (c) {
    case Red: 
        SDL_SetRenderDrawColor( ren, 230, 0, 0, 255 );
        break;
    case Green: 
        SDL_SetRenderDrawColor( ren, 0, 230, 0, 255 );
        break;
    case Blue: 
        SDL_SetRenderDrawColor( ren, 0, 0, 230, 255 );
        break;
    case Black: 
        SDL_SetRenderDrawColor( ren, 0, 0, 0, 255 );
        break;
    }
}

void Application::render(const Environment& env) {
    // Clear renderer 
    SDL_SetRenderDrawColor( m_renderer, 255, 255, 255, 255 );
    SDL_RenderClear( m_renderer );

    // Render each body as polygon 
    for (const Body& body : env.getBodies()) 
    {
        // Set render color to grey ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( m_renderer, body.color.r, body.color.g, body.color.b, body.color.a);

        auto points = body.getPointsGlobal();
        for (int i=0; i<points.size(); i++) {
            const Vec2& a = points[i];
            const Vec2& b = points[i == points.size()-1 ? 0 : i+1];

            SDL_RenderDrawLine( m_renderer, a.x, SCREEN_HEIGHT - a.y, b.x, SCREEN_HEIGHT - b.y);
        }

    }

    for (auto& [p, c] : debugPoints)
    {
        setColor(m_renderer, c);
        SDL_Rect r;
        r.x = p.x-2;
        r.y = SCREEN_HEIGHT - (p.y-2);
        r.w = 4;
        r.h = 4; 

        SDL_RenderFillRect( m_renderer, &r );
    }
    debugPoints.clear();

    // Actually Render
    SDL_RenderPresent( m_renderer );
}
