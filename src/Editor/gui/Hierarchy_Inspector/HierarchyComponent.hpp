#pragma once

#include <string>
#include <json.hpp>
#include "../../../Engine/xplor.hpp"

namespace gui
{
    struct HierarchyComponent
    {
        std::string name;
        bool alive = true;

        unsigned int component_type = 0;

        HierarchyComponent** selectedID;

        std::vector<std::shared_ptr<HierarchyComponent>> children;

        std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>> entities;
        std::shared_ptr<std::vector<std::string>> model_names;
        std::shared_ptr<xplor::terrain::Terrain> terrain;

        HierarchyComponent() {}
        HierarchyComponent(std::string name, std::shared_ptr<std::vector<std::string>>& model_names,
                           std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
                           std::shared_ptr<xplor::terrain::Terrain> terrain, HierarchyComponent**& selectedID)
            : name(name), entities(entities), model_names(model_names), terrain(terrain), selectedID(selectedID)
        {}
        HierarchyComponent(std::string name, json::JSON& json, std::shared_ptr<std::vector<std::string>>& model_names,
                           std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
                           std::shared_ptr<xplor::terrain::Terrain> terrain, HierarchyComponent**& selectedID)
            : name(name), entities(entities), model_names(model_names), terrain(terrain), selectedID(selectedID)
        {
            createChildren(json);
        }

        void createChildren(json::JSON& json);
        
        // Set the ID as hierarchy ptr
        // If true, delete
        void renderInHierarchy();

        virtual void renderInInspector();

        virtual json::JSON buildJson();
        json::JSON createJsonObject(unsigned object = 0, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), float scale = 1, int model_ID = -1);

        ~HierarchyComponent()
        {
            alive = false;
            std::cout << name << "\n";
            if (children.size())
            {
                for (auto& c : children)
                {
                    children.erase(children.begin());
                }
            }

            if (*selectedID == this)
                *selectedID = nullptr;
        }
    };

    struct EntityComponent : public HierarchyComponent
    {
        std::shared_ptr<xplor::Entity> entity;

        EntityComponent() {}
        EntityComponent(std::string name, std::shared_ptr<xplor::Entity>& entity, std::shared_ptr<std::vector<std::string>>& model_names,
            std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
            std::shared_ptr<xplor::terrain::Terrain> terrain, HierarchyComponent**& selectedID)
            : entity(entity)
        {
            this->name = name; this->entities = entities;
            this->model_names = model_names; this->selectedID = selectedID;
        }
        EntityComponent(std::string name, std::shared_ptr<xplor::Entity>& entity, std::shared_ptr<std::vector<std::string>>& model_names,
                        json::JSON& json, std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
                        std::shared_ptr<xplor::terrain::Terrain> terrain, HierarchyComponent**& selectedID)
            : entity(entity)
        {
            this->name = name; this->component_type = component_type;
            this->entities = entities; this->model_names = model_names;
            this->selectedID = selectedID;
            createChildren(json);
        }

        void renderInInspector() override;

        json::JSON buildJson();
    };

    struct TerrainComponent : public HierarchyComponent
    {
        TerrainComponent() {}
        TerrainComponent(std::string name, std::shared_ptr<std::vector<std::string>>& model_names,
            std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
            std::shared_ptr<xplor::terrain::Terrain> terrain, HierarchyComponent**& selectedID)
        {
            this->name = name; this->terrain = terrain;
            this->model_names = model_names; this->entities = entities;
            this->selectedID = selectedID;
        }
        TerrainComponent(std::string name, std::shared_ptr<std::vector<std::string>>& model_names,
            json::JSON& json, std::shared_ptr<std::vector<std::shared_ptr<xplor::Entity>>>& entities,
            std::shared_ptr<xplor::terrain::Terrain> terrain, HierarchyComponent**& selectedID)
        {
            this->name = name; this->terrain = terrain;
            this->model_names = model_names; this->entities = entities;
            this->selectedID = selectedID;
            createChildren(json);
        }

        void renderInInspector() override;

        json::JSON buildJson();
    };

    namespace comp_type
    {
        enum component_type
        {
            blank,
            entity,
            light,
            terrain
        };
    }

    namespace objects
    {
        enum objects
        {
            structure,
            entity,
            terrain
        };
    }
}