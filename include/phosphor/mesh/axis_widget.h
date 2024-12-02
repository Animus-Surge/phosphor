#pragma once

/**
 * Phosphor phosphor/mesh/axis_widget.h
 * Axis widget mesh class
 */

#include "phosphor/mesh/mesh.h"
#include "phosphor/shader.h"

class AxisWidget : public Mesh {
protected:
    Shader* axis_shader;
public:
    /**
     * Constructor
     */
    AxisWidget();

    /**
     * Destructor
     */
    ~AxisWidget();

    /**
     * Render the axis widget
     */
    void render() override;

    void set_transform(glm::mat4 transform);
}; // class AxisWidget
