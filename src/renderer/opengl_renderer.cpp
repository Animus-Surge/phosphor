/**
 * Phosphor renderer/opengl_renderer.cpp
 * OpenGL renderer implementation
 */

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "phosphor/renderer.h"
#include "phosphor/backends/opengl_renderer.h"
#include "phosphor/shader.h"

Shader* shader;

void OpenGLRenderer::init() {
    //SDL initialization

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        exit(1); //TODO: return with error code instead
    }
    
    //OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //SDL window creation and OpenGL context setup
    //TODO: all error checking
    SDL_Window* win = SDL_CreateWindow("Phosphor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(win == nullptr) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1); //TODO: return with error code instead
    }
    SDL_GLContext context = SDL_GL_CreateContext(win);
    if(context == nullptr) {
        printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(1);
    }

    int current = SDL_GL_MakeCurrent(win, context);
    if(current != 0) {
        printf("SDL_GL_MakeCurrent Error: %s\n", SDL_GetError());
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(1);
    }

    //GLEW initialization
    glewExperimental = GL_TRUE;
    GLenum glew_error = glewInit();

    if(glew_error != GLEW_OK) {
        printf("GLEW Error: %s\n", glewGetErrorString(glew_error));
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(1);
    }

    //Shader program
    shader = new Shader("resources/vert.glsl", "resources/frag.glsl");

}

void OpenGLRenderer::shutdown() {
    SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
    SDL_DestroyWindow(SDL_GL_GetCurrentWindow());
    SDL_Quit();
}

void OpenGLRenderer::render() {
    SDL_Window* window = SDL_GL_GetCurrentWindow();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    
    float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
        
    glUseProgram(0);
    
    
    SDL_GL_SwapWindow(window);

}


