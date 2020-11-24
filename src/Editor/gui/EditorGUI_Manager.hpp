#pragma once

#include "gui/EditorGUI.hpp"

struct EditorGUI_Manager : xplor::gui::GUI_Manager
{
	EditorGUI_Manager()
	{
	}

    void Init(SDL_Window* window, SDL_GLContext& context, std::shared_ptr<std::vector<std::string>>& model_names,
              std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
               std::shared_ptr<xplor::terrain::Terrain> terrain)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 330");

        setImGUIColors();
        setImGUITheme();

        // Init guis w opengl functions

        viewPort.Init();
        
        worldHierarchy.readJSON("", model_names, entities, terrain);
    }

    // Returns false if quit
    bool update(SDL_Window* window)
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        if (worldHierarchy.currentSelection != nullptr)
        {
            if (worldHierarchy.currentSelection != inspector.currentSelection)
                inspector.setCurrentSelection(worldHierarchy.currentSelection);
        } else { inspector.currentSelection = nullptr; }

        dockSpace.update(latest_gui_events);
        tools.update(latest_gui_events);
        console.update(latest_gui_events);
        inspector.update(latest_gui_events);
        fileSystem.update(latest_gui_events);
        worldHierarchy.update(latest_gui_events);
        viewPort.update(latest_gui_events);

        // Rendering
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        return processGuiEvents();
    }

    bool processGuiEvents()
    {
        for (auto& e : latest_gui_events)
        {
            if (gui_events::file_quit)
            {
                return 0;
            }
        }

        return 1;
    }

    void destroyImGui()
    {
        printf("REE");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

//private:

    gui::DockSpace dockSpace;
    gui::Tools tools;
    gui::Console console;
    gui::Inspector inspector;
    gui::FileSystem fileSystem;
    gui::WorldHierarchy worldHierarchy;
    gui::Viewport viewPort;
};