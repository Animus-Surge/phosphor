#pragma once

/**
* Phosphor include/phosphor/util.hpp
* Utility functions for Phosphor.
*/

#include <cstdio>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

void print_vec2(const glm::vec2& vec) {
    printf("glm::vec2(%f, %f)\n", vec.x, vec.y);
}
void print_vec3(const glm::vec3& vec) {
    printf("glm::vec3(%f, %f, %f)\n", vec.x, vec.y, vec.z);
}
void print_vec4(const glm::vec4& vec) {
    printf("glm::vec4(%f, %f, %f, %f)\n", vec.x, vec.y, vec.z, vec.w);
}

void print_mat2(const glm::mat2& mat) {
    printf("glm::mat2(\n");
    printf("  %f, %f,\n", mat[0][0], mat[1][0]);
    printf("  %f, %f\n", mat[0][1], mat[1][1]);
    printf(")\n");
}
void print_mat3(const glm::mat3& mat) {
    printf("glm::mat3(\n");
    printf("  %f, %f, %f,\n", mat[0][0], mat[1][0], mat[2][0]);
    printf("  %f, %f, %f,\n", mat[0][1], mat[1][1], mat[2][1]);
    printf("  %f, %f, %f\n", mat[0][2], mat[1][2], mat[2][2]);
    printf(")\n");
}
void print_mat4(const glm::mat4& mat) {
    printf("glm::mat4(\n");
    printf("  %f, %f, %f, %f,\n", mat[0][0], mat[1][0], mat[2][0], mat[3][0]);
    printf("  %f, %f, %f, %f,\n", mat[0][1], mat[1][1], mat[2][1], mat[3][1]);
    printf("  %f, %f, %f, %f,\n", mat[0][2], mat[1][2], mat[2][2], mat[3][2]);
    printf("  %f, %f, %f, %f\n", mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
    printf(")\n");
}
