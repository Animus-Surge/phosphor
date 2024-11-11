/*
 * Phosphor engine/sdl/sdl_rendersys.cpp
 * SDL rendering system implementation
 */

#ifdef PHOSPHOR_SDL
#include "phosphor/sdl/rendersys.h"

#include <cmath>

void draw_point(SDL_Renderer* renderer, int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

void draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void draw_rect(SDL_Renderer* renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderDrawRect(renderer, &rect);
}

void draw_polyline(SDL_Renderer* renderer, SDL_Point* points, int num_points, bool closed)
{
    SDL_RenderDrawLines(renderer, points, num_points);
    if (closed)
    {
        SDL_RenderDrawLine(renderer, points[num_points - 1].x, points[num_points - 1].y, points[0].x, points[0].y);
    }
}

void draw_circle(SDL_Renderer* renderer, int x, int y, int r)
{
    //Line segments
    for(int i = 0; i < 360; i++)
    {
        int x1 = x + r * cos(i * M_PI / 180);
        int y1 = y + r * sin(i * M_PI / 180);
        int x2 = x + r * cos(i * M_PI / 180);
        int y2 = y + r * sin(i * M_PI / 180);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void draw_ellipse(SDL_Renderer* renderer, int x, int y, int rx, int ry)
{
    //Line segments
    for(int i = 0; i < 360; i++)
    {
        int x1 = x + rx * cos(i * M_PI / 180);
        int y1 = y + ry * sin(i * M_PI / 180);
        int x2 = x + rx * cos(i * M_PI / 180);
        int y2 = y + ry * sin(i * M_PI / 180);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void draw_curve(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3)
{
    int i = 0;
    for (i = 0; i <= 1000; i++)
    {
        float t = i / 1000.0;
        float x = (1 - t) * (1 - t) * x1 + 2 * (1 - t) * t * x2 + t * t * x3;
        float y = (1 - t) * (1 - t) * y1 + 2 * (1 - t) * t * y2 + t * t * y3;
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void draw_fill_rect(SDL_Renderer* renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);
}

void draw_fill_polyline(SDL_Renderer* renderer, SDL_Point* points, int num_points)
{
    draw_polyline(renderer, points, num_points, true);

    //Scanline fill
    int i = 0;
    for (i = 0; i < num_points; i++)
    {
        int x1 = points[i].x;
        int y1 = points[i].y;
        int x2 = points[(i + 1) % num_points].x;
        int y2 = points[(i + 1) % num_points].y;
        int j = 0;
        for (j = y1; j <= y2; j++)
        {
            if(y1 != y2)
            {
                int x = x1 + (x2 - x1) * (j - y1) / (y2 - y1);
                SDL_RenderDrawPoint(renderer, x, j);
            }
        }
    }
}

void draw_fill_circle(SDL_Renderer* renderer, int x, int y, int r)
{
    int x1 = x - r;
    int y1 = y;
    int x2 = x + r;
    int y2 = y;
    int i = 0;
    for (i = x1; i <= x2; i++)
    {
        int j = y + sqrt(r * r - (i - x) * (i - x));
        SDL_RenderDrawLine(renderer, i, j, i, y);
        j = y - sqrt(r * r - (i - x) * (i - x));
        SDL_RenderDrawLine(renderer, i, j, i, y);
    }
}

void draw_fill_ellipse(SDL_Renderer* renderer, int x, int y, int rx, int ry)
{
    int x1 = x - rx;
    int y1 = y;
    int x2 = x + rx;
    int y2 = y;
    int i = 0;
    for (i = x1; i <= x2; i++)
    {
        int j = y + sqrt(ry * ry - (i - x) * (i - x) * ry * ry / rx / rx);
        SDL_RenderDrawLine(renderer, i, j, i, y);
        j = y - sqrt(ry * ry - (i - x) * (i - x) * ry * ry / rx / rx);
        SDL_RenderDrawLine(renderer, i, j, i, y);
    }
}

#endif // PHOSPHOR_SDL
