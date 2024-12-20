#pragma once

/**
 * Phosphor phosphor/widgets/wireframeWidget.hpp
 * Wireframe widget mesh class
 */

const std::string wireframe_vert = R"(
#version 430 core

layout(binding = 0) uniform Transform {
    mat4 transform;
};

layout(binding = 2) uniform Camera {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 pv_matrix;
    vec3 cam_position;
    vec3 cam_direction;
    vec3 cam_right;

    float cam_fov;
    float cam_aspect_ratio;
    float cam_near_clip;
    float cam_far_clip;
};

layout(location = 0) in vec3 vert_pos
layout(location = 1) in vec3 vert_color

out vec3 frag_color;

void main()
{
    gl_Position = pv_matrix * (transform * vec4(vert_pos, 1.0));
    frag_color = vert_color;
}
)";

const std::string wireframe_frag = R"(
#version 430 core

in vec3 frag_color;

out vec4 FragColor;

void main() {
    FragColor = vec4(frag_color, 1.0);
}
)";

struct WireframeVertex {
    glm::vec3 position;
    glm::vec3 color;
};

void createPrimitiveSphereWireframe(glm::vec3 color, float radius, int segments, std::vector<WireframeVertex>& vertices, std::vector<unsigned int>& indices) {
    float segment_angle = 2.0f * M_PI / segments;
    float ring_angle = M_PI / segments;

    for(int i = 0; i < segments; i++) {
        for(int j = 0; j < segments; j++) {
            float x = cos(segment_angle * j) * sin(ring_angle * i);
            float y = cos(ring_angle * i);
            float z = sin(segment_angle * j) * sin(ring_angle * i);

            vertices.push_back({glm::vec3(x, y, z) * radius, color});
        }
    }

    for(int i = 0; i < segments; i++) {
        for(int j = 0; j < segments; j++) {
            int index = i * segments + j;
            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + segments);

            indices.push_back(index + 1);
            indices.push_back(index + segments + 1);
            indices.push_back(index + segments);
        }
    }
}

void createPrimitiveCubeWireframe(glm::vec3 color, float size, std::vector<WireframeVertex>& vertices, std::vector<unsigned int>& indices) {
    float half_size = size / 2.0f;

    //Front face
    vertices.push_back({glm::vec3(-half_size, -half_size, -half_size), color});
    vertices.push_back({glm::vec3(half_size, -half_size, -half_size), color});
    vertices.push_back({glm::vec3(half_size, half_size, -half_size), color});
    vertices.push_back({glm::vec3(-half_size, half_size, -half_size), color});

    //Right face
    vertices.push_back({glm::vec3(half_size, -half_size, -half_size), color});
    vertices.push_back({glm::vec3(half_size, -half_size, half_size), color});
    vertices.push_back({glm::vec3(half_size, half_size, half_size), color});
    vertices.push_back({glm::vec3(half_size, half_size, -half_size), color});

    //Top face
    vertices.push_back({glm::vec3(-half_size, half_size, -half_size), color});
    vertices.push_back({glm::vec3(half_size, half_size, -half_size), color});
    vertices.push_back({glm::vec3(half_size, half_size, half_size), color});
    vertices.push_back({glm::vec3(-half_size, half_size, half_size), color});

    //Back face
    vertices.push_back({glm::vec3(half_size, -half_size, half_size), color});
    vertices.push_back({glm::vec3(-half_size, -half_size, half_size), color});
    vertices.push_back({glm::vec3(-half_size, half_size, half_size), color});
    vertices.push_back({glm::vec3(half_size, half_size, half_size), color});

    //Left face
    vertices.push_back({glm::vec3(-half_size, -half_size, half_size), color});
    vertices.push_back({glm::vec3(-half_size, -half_size, -half_size), color});
    vertices.push_back({glm::vec3(-half_size, half_size, -half_size), color});
    vertices.push_back({glm::vec3(-half_size, half_size, half_size), color});

    //Bottom face
    vertices.push_back({glm::vec3(-half_size, -half_size, half_size), color});
    vertices.push_back({glm::vec3(half_size, -half_size, half_size), color});
    vertices.push_back({glm::vec3(half_size, -half_size, -half_size), color});
    vertices.push_back({glm::vec3(-half_size, -half_size, -half_size), color});

    //Front face
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(0);

    //Right face
    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(6);
    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(4);

    //Top face
    indices.push_back(8);
    indices.push_back(11);
    indices.push_back(10);
    indices.push_back(10);
    indices.push_back(9);
    indices.push_back(8);

    //Back face
    indices.push_back(12);
    indices.push_back(15);
    indices.push_back(14);
    indices.push_back(14);
    indices.push_back(13);
    indices.push_back(12);

    //Left face
    indices.push_back(16);
    indices.push_back(19);
    indices.push_back(18);
    indices.push_back(18);
    indices.push_back(17);
    indices.push_back(16);

    //Bottom face
    indices.push_back(20);
    indices.push_back(23);
    indices.push_back(22);
    indices.push_back(22);
    indices.push_back(21);
    indices.push_back(20);
}

class WireframeWidget : public Mesh {
protected:
    Shader* wireframe_shader;
    std::vector<WireframeVertex> vertices;
    std::vector<unsigned int> indices;

public:
    /**
     * Constructor
     */
    WireframeWidget() {
        this->wireframe_shader = new Shader(wireframe_vert, wireframe_frag);
    }

    /**
     * Destructor
     */
    ~WireframeWidget() {
        delete this->wireframe_shader;
    }

    /**
     * Render the wireframe widget
     */
    void render() override {
        this->wireframe_shader->use();

        glBindVertexArray(this->vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(WireframeVertex), this->vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

        //Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WireframeVertex), (void*)offsetof(WireframeVertex, position));
        glEnableVertexAttribArray(0);

        //Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(WireframeVertex), (void*)offsetof(WireframeVertex, color));
        glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    }
};