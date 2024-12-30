/**
* Phosphor src/mesh.cpp
* Mesh implementation
*/

#include "phosphor/mesh/mesh.hpp"

#include <phosphor/shader.hpp>

Shader* Mesh::mesh_shader = nullptr;

// 2D shape primitives

// 3D shape primitives

Mesh *create_primitive_box(const float width, const float height, const float depth) {
    const std::vector<Vertex> vertices = {
        //Front face
        {{-width / 2, -height / 2, depth / 2}, {0, 0, 1}, {0, 0}},
        {{width / 2, -height / 2, depth / 2}, {0, 0, 1}, {1, 0}},
        {{width / 2, height / 2, depth / 2}, {0, 0, 1}, {1, 1}},
        {{-width / 2, height / 2, depth / 2}, {0, 0, 1}, {0, 1}},

        //Right face
        {{width / 2, -height / 2, depth / 2}, {1, 0, 0}, {0, 0}},
        {{width / 2, -height / 2, -depth / 2}, {1, 0, 0}, {1, 0}},
        {{width / 2, height / 2, -depth / 2}, {1, 0, 0}, {1, 1}},
        {{width / 2, height / 2, depth / 2}, {1, 0, 0}, {0, 1}},

        //Top face
        {{-width / 2, height / 2, depth / 2}, {0, 1, 0}, {0, 0}},
        {{width / 2, height / 2, depth / 2}, {0, 1, 0}, {1, 0}},
        {{width / 2, height / 2, -depth / 2}, {0, 1, 0}, {1, 1}},
        {{-width / 2, height / 2, -depth / 2}, {0, 1, 0}, {0, 1}},

        //Back face
        {{width / 2, -height / 2, -depth / 2}, {0, 0, -1}, {0, 0}},
        {{-width / 2, -height / 2, -depth / 2}, {0, 0, -1}, {1, 0}},
        {{-width / 2, height / 2, -depth / 2}, {0, 0, -1}, {1, 1}},
        {{width / 2, height / 2, -depth / 2}, {0, 0, -1}, {0, 1}},

        //Left face
        {{-width / 2, -height / 2, -depth / 2}, {-1, 0, 0}, {0, 0}},
        {{-width / 2, -height / 2, depth / 2}, {-1, 0, 0}, {1, 0}},
        {{-width / 2, height / 2, depth / 2}, {-1, 0, 0}, {1, 1}},
        {{-width / 2, height / 2, -depth / 2}, {-1, 0, 0}, {0, 1}},

        //Bottom face
        {{-width / 2, -height / 2, -depth / 2}, {0, -1, 0}, {0, 0}},
        {{width / 2, -height / 2, -depth / 2}, {0, -1, 0}, {1, 0}},
        {{width / 2, -height / 2, depth / 2}, {0, -1, 0}, {1, 1}},
        {{-width / 2, -height / 2, depth / 2}, {0, -1, 0}, {0, 1}}
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0, //Front
        4, 5, 6, 6, 7, 4, //Right
        8, 9, 10, 10, 11, 8, //Top
        12, 13, 14, 14, 15, 12, //Back
        16, 17, 18, 18, 19, 16, //Left
        20, 21, 22, 22, 23, 20 //Bottom
    };

    return new Mesh(vertices, indices);
}
