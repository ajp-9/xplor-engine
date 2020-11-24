#include "Editor.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Editor::Editor()
{
    core.window.title = "Xplor Engine Editor v0.1.0";
    core.window.width = 1200;
    core.window.height = 800;
    core.window.position = glm::ivec2((1920 / 2) - (core.window.width / 2), 100);

    player.fov = 60;
    player.position = glm::vec3(-50, -50, 3);

    core.initApplicatiion();
    core.renderer.initRenderer();
}

void Editor::ready()
{
    world.terrain->generateTerrain("grassy.png", "mud.png", "grassFlowers.png", "path.png"); // first so more blocks of memory

    shader_manager.Init(glm::perspective(glm::radians(player.fov), float(core.window.width) / float(core.window.height), 0.25f, 500.0f));

    model_manager.loadModels();

    shader_manager.setSpotLightActivated(true);

    gui_manager.Init(core.window.window, core.window.context, model_manager.model_names, world.entities, world.terrain);

    shader_manager.setDirectionalLight(world.sun.direction);

    world.loadWorld();
}

void Editor::update()
{
    gui_manager.viewPort.setFramebufferActive(); // everything after draws to the viewport framebuffer 

	core.window.update();

    shader_manager.setCameraPosVec(player.position);
    pointLight.position = position;
    shader_manager.setPointLight(pointLight);

    world.renderWorld(shader_manager, model_manager, player);

    tryEvents();
    player.tryKeyPress();

    if (!gui_manager.update(core.window.window))
        running = false;

    if(!running)
        gui_manager.worldHierarchy.saveJSON();
}

void Editor::tryKeyPress() {}

void Editor::mouseMove() {}

void Editor::mouseClick() {}

void Editor::tryEvents()
{
    while (SDL_PollEvent(&e))
    {
        gui_manager.processEvent(e);
        switch (e.type)
        {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                core.window.catchMouse(false);
                return;
            case SDL_SCANCODE_C:
                core.window.catchMouse(true);
                return;
            case SDL_SCANCODE_0:
                position = player.position;
                return;
            }    
            break;
        case SDL_MOUSEMOTION:
            if (core.window.isMouseCatched)
            {
                mouseMove();
                int x, y;
                SDL_GetRelativeMouseState(&x, &y);
                player.moveMouse(float(x), float(y));
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseClick();
            break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                core.window.resizeWindow(e.window.data1, e.window.data2);
            break;
        case SDL_QUIT:
            running = false;
            return;
        }
    }
}

Editor::~Editor()
{
    core.~Core();
}