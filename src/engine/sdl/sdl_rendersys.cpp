/*
 * Phosphor engine/sdl/sdl_rendersys.cpp
 * SDL rendering system implementation
 */

#ifdef PHOSPHOR_SDL
#include "phosphor/sdl/rendersys.h"

#include <algorithm>
#include <cmath>
#include <vector>

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
    draw_ellipse(renderer, x, y, r, r); //Reuse ellipse function with equal radii
}

void draw_ellipse(SDL_Renderer* renderer, int x, int y, int rx, int ry)
{
    //To avoid calculating with trig each time, we'll calculate the angle increment once and apply it to the
    //rest of the points
    double angle_increment = M_PI / 180;
    double cost = cos(angle_increment);
    double sint = sin(angle_increment);

    double x1 = rx;
    double y1 = 0;
    
    for(int i = 0; i < 360; i++) {
        //Draw a line from the last point to the new point
        int draw_x1 = x + static_cast<int>(x1);
        int draw_y1 = y + static_cast<int>(y1);

        double new_x = x1 * cost - y1 * sint;
        double new_y = x1 * sint + y1 * cost;

        x1 = new_x;
        y1 = new_y;

        int draw_x2 = x + static_cast<int>(x1);
        int draw_y2 = y + static_cast<int>(y1);

        SDL_RenderDrawLine(renderer, draw_x1, draw_y1, draw_x2, draw_y2);
    }
}

void draw_curve(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3)
{
    SDL_Point last_point = { x1, y1 };
    
    //Interpolate between points: p1 - p2, and p2 - p3 at the same time
    for(float t = 0; t <= 1; t += 0.01)
    {
        //Find the point on the line between p1 and p2
        float x12 = x1 + (x2 - x1) * t;
        float y12 = y1 + (y2 - y1) * t;

        //Find the point on the line between p2 and p3
        float x23 = x2 + (x3 - x2) * t;
        float y23 = y2 + (y3 - y2) * t;

        //Find the point on the line between the two points above
        float x = x12 + (x23 - x12) * t;
        float y = y12 + (y23 - y12) * t;

        SDL_RenderDrawLine(renderer, last_point.x, last_point.y, x, y);
        last_point.x = x;
        last_point.y = y;
    }
    

}

void draw_fill_rect(SDL_Renderer* renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);
}

void draw_fill_polyline(SDL_Renderer* renderer, SDL_Point* points, int num_points)
{
    // Scanline Fill Algorithm
    //Find the min and max y values
    int min_y = points[0].y;
    int max_y = points[0].y;

    for (int i = 1; i < num_points; i++)
    {
        if (points[i].y < min_y)
        {
            min_y = points[i].y;
        }
        if (points[i].y > max_y)
        {
            max_y = points[i].y;
        }
    } 

    //For each scanline
    for (int y = min_y; y <= max_y; y++)
    {
        //Find the intersections
        std::vector<int> intersections;
        for (int i = 0; i < num_points; i++) {
            int x1 = points[i].x;
            int y1 = points[i].y;
            int x2 = points[(i + 1) % num_points].x;
            int y2 = points[(i + 1) % num_points].y;

            if ((y1 <= y && y2 > y) || (y1 > y && y2 <= y))
            {
                int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
                intersections.push_back(x);
            }

        }

        //Sort the intersections
        std::sort(intersections.begin(), intersections.end());

        //Draw the scanline
        for (int i = 0; i < intersections.size(); i += 2)
        {
            SDL_RenderDrawLine(renderer, intersections[i], y, intersections[i + 1], y);
        }
    }

    draw_polyline(renderer, points, num_points, true);
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
