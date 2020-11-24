#include "WorldHierarchy_InspectorGUI.hpp" 

#include <iostream>
#include <json.hpp>
#include <fstream>

namespace gui
{
    void WorldHierarchy::readJSON(std::string path, std::shared_ptr<std::vector<std::string>>& model_names,
                                  std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
                                  std::shared_ptr<xplor::terrain::Terrain> terrain)
    {
        std::ifstream file("res/world.json");

        std::string content((std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));

        json::JSON json = json::JSON::Load(content);

        for (auto& j : json.ObjectRange())
        {
            rootComponents.push_back(HierarchyComponent(j.first, j.second, model_names, entities, terrain, currentSelection));
        }
    }

    void WorldHierarchy::parseJsonObjs(json::JSON& json) {}

    void WorldHierarchy::saveJSON()
    {
        json::JSON json;

        for (auto& c : rootComponents)
        {
            json[c.name] = c.buildJson();
        }

        std::ofstream ofile("res/world.json");
        ofile << json;

        ofile.flush();
    }

    void WorldHierarchy::update(std::vector<int>& events)
    {
        ImGui::Begin("World Hierarchy", (bool*)0, ImGuiWindowFlags_NoCollapse);

        unsigned indx = 0;
        for (auto& c : rootComponents)
        {
            c.renderInHierarchy();
            if(!c.alive)
            {
                rootComponents.erase(rootComponents.begin() + indx);
            }

            indx++;
        }

        if(ImGui::IsKeyPressed(SDL_SCANCODE_ESCAPE) && ImGui::IsWindowFocused())
            currentSelection = nullptr;

        ImGui::End();
    }

    // Inspector -------------------------

    void Inspector::setCurrentSelection(HierarchyComponent** currentSelection)
    {
        this->currentSelection = currentSelection;
    }

    void Inspector::update(std::vector<int>& events)
    {
        ImGui::Begin("Inspector", (bool*)0, ImGuiWindowFlags_NoCollapse);

        if (*currentSelection != nullptr)
        {
            (*currentSelection)->renderInInspector();
        }

        ImGui::End();
    }
}