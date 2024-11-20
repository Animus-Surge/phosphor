#pragma once

/**
 * Phosphor phosphor/components/PrimitiveShape.h
 * PrimitiveShape component
 */

//TODO: namespace

#include <vector>
#include <string>

#include "phosphor/core.h"

#include <SDL2/SDL.h>

namespace phosphor {

    class PrimitiveShape : public Component {
        public:
            SDL_Color fill_color = {255, 255, 255, 255}; //TODO: replace with a ColorValue object which can handle gradients
            SDL_Color border_color = {255, 255, 255, 255};
            int border_width = 1;

            bool fill = false;

            /*
             * 0: polygon
             * 1: curve
             * 2: line
             * 3: ellipse/circle
             * 4: rect
             */
            int type = 0;

            //Fields specific to Polygons/Polylines
            std::vector<SDL_Point> points;
            bool close = true;

            //Fields specific to lines
            SDL_Point start, end;

            //Fields specific to Rectangles
            int width, height;
            SDL_Point position;

            //Fields specific to Ellipses/Circles
            int rx, ry;

            //Overrideables
            void input() override;
            void update() override;
            void fixed_update(float delta) override;
            void render(SDL_Renderer* renderer) override;

    }; // class PrimitiveShape

} // namespace phosphor
