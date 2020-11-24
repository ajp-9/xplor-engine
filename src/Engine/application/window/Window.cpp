#include "Window.hpp"

#include <glad/glad.h>

#include <iostream>

namespace xplor
{
    void Window::Init()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            fprintf(stderr, "%s: %s\n", "Could not initalize SDL", SDL_GetError());

        window = SDL_CreateWindow(title, position.x, position.y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        // Set attributes
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); // how many colors there can be
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // 8 * 4
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // allocate 2 buffers

        //SDL_SetWindowResizable(window, SDL_TRUE);

        if (window == NULL)
            fprintf(stderr, "%s: %s\n", "Couldn't create window: ", SDL_GetError());

        // Create OpenGL context
        context = SDL_GL_CreateContext(window);
        if (context == NULL)
            fprintf(stderr, "%s: %s\n", "Could not create the OpenGL context: ", SDL_GetError());

        // Load OpenGL functions glad SDL
        gladLoadGLLoader(SDL_GL_GetProcAddress);

        // V-Sync
        SDL_GL_SetSwapInterval(1);

        if (isMouseCatched)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE); isMouseCatched = true;
        }
        else
        {
            SDL_SetRelativeMouseMode(SDL_FALSE); isMouseCatched = false;
        }

        setUpOpenGL();
    }

    void Window::update() // might need to fix order of swapping later
    {
        SDL_GL_SwapWindow(window); // swap buffers

        glClearColor(0, 0, 0, 1.0f); // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //SDL_WarpMouseInWindow(window, width / 2, height / 2);
    }

    void Window::catchMouse(bool catched)
    {
        if (catched)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE); isMouseCatched = true;
        }
        else
        {
            SDL_SetRelativeMouseMode(SDL_FALSE); isMouseCatched = false;
        }
    }

    void Window::resizeWindow(int width, int height)
    {
        glViewport(0, 0, width, height);
        //SDL_SetWindowSize(window, width, height); makes a loop loll
        this->width = width;
        this->height = height;
    }

    void Window::setUpOpenGL()
    {
        GLint vpWidth, vpHeight;
        SDL_GL_GetDrawableSize(window, &vpWidth, &vpHeight);
        glViewport(0, 0, vpWidth, vpHeight);
        glEnable(GL_DEPTH_TEST);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    Window::~Window()
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}