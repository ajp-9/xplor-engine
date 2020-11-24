#pragma once

#include <vector>
#include <string>

#include "../../../Engine/xplor.hpp"
#include "HierarchyComponent.hpp"

namespace gui
{
    struct WorldHierarchy : public xplor::gui::GUI
    {
        std::vector<HierarchyComponent> rootComponents;
        HierarchyComponent** currentSelection = new HierarchyComponent*(nullptr);

        WorldHierarchy() {}

        void readJSON(std::string path, std::shared_ptr<std::vector<std::string>>& models,
                      std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
                      std::shared_ptr<xplor::terrain::Terrain> terrain);
        void parseJsonObjs(json::JSON& json);

         
        void saveJSON();

        void update(std::vector<int>& events) override;
    };

    struct Inspector : public xplor::gui::GUI
    {
        HierarchyComponent** currentSelection;

        void setCurrentSelection(HierarchyComponent** currentSelection);
        void update(std::vector<int>& events) override;
    };
}