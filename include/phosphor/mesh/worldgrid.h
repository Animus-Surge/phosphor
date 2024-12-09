#pragma once

/**
 * Phosphor phosphor/mesh/worldgrid.h
 * World grid mesh class
 */

#include <glm/glm.hpp>

#include "phosphor/mesh/mesh.h"

class WorldGrid : public Mesh {
protected:
    float grid_line_spacing = 1.0f;

    int grid_size = 10;

    bool weighted_axis_lines = false;

public:
    /**
     * Constructor
     */
    WorldGrid(float grid_line_spacing, int grid_size);

    /**
     * Destructor
     */
    ~WorldGrid();

    /**
     * Render the world grid
     */
    void render() override;

    /**
     * Regenerate the uniform buffer object
     */
    void regen_ubo() override {} //Shouldn't need to regen the ubo, but it's here just in case

    void set_weighted_axis_lines(bool weighted_axis_lines) {
        this->weighted_axis_lines = weighted_axis_lines;
    }
    void set_grid_line_spacing(float grid_line_spacing) {
        this->grid_line_spacing = grid_line_spacing;
    }
    void set_grid_size(int grid_size) {
        this->grid_size = grid_size;
    }

    bool get_weighted_axis_lines() {
        return weighted_axis_lines;
    }
    float get_grid_line_spacing() {
        return grid_line_spacing;
    }
    int get_grid_size() {
        return grid_size;
    }
}; // class WorldGrid
