#include "Game.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Game::Game()
{
    core.window.title = "Xplor Engine v0.1.0";
    core.window.width = 1000;
    core.window.height = 1000;
    core.window.position = glm::ivec2(900, 30);

    player.fov = 60;
    player.position = glm::vec3(-50, -50, 3);

    core.initApplicatiion();
    core.renderer.initRenderer();
}

void Game::ready()
{
    shader_manager.Init(glm::perspective(glm::radians(player.fov), (float)core.window.width / (float)core.window.height, 0.25f, 500.0f));

    model_manager.loadModels();
    entities.push_back(Entity(model::crate, glm::vec3(0, 0, 0), glm::vec3(90, 0, 0), 3));

    world.terrain->generateTerrain("grassy.png", "mud.png", "grassFlowers.png", "path.png");

    shader_manager.setSpotLightActivated(true);
}

void Game::update()
{
	core.window.update();

    shader_manager.setCameraPosVec(player.position);
    shader_manager.setDirectionalLight(world.sun.direction);
    pointLight.position = position;
    shader_manager.setPointLight(pointLight);

    shader_manager.terrain.bind();
    world.terrain->render(shader_manager.terrain);

    shader_manager.setSpotLightDirection(player.front);
    {
        shader_manager.lighting.bind();
        for (auto& e : entities)
            e.render(shader_manager.lighting, model_manager);
        
        glm::mat4 view = player.getViewMatrix();
        shader_manager.setViewMatrix(view);
    }

    tryEvents();
    player.tryKeyPress();

    //core.window.gui_manager.update(core.window.window);
}

void Game::tryKeyPress() {}

void Game::mouseMove() {}

void Game::mouseClick() {}

void Game::tryEvents()
{
    while (SDL_PollEvent(&e))
    {
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

Game::~Game()
{
    core.~Core();
}