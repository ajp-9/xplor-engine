#include "HierarchyComponent.hpp"

#include "../../../Engine/application/GUI/imgui/imgui_stdlib.h"

namespace gui
{
    void HierarchyComponent::createChildren(json::JSON& json)
    {
        for (auto& j : json["children"].ObjectRange())
        {
            int type = j.second[".type"].ToInt();

            if (type == objects::structure)
            {
                children.push_back(std::make_shared<HierarchyComponent>(j.first, j.second, model_names, entities, terrain, selectedID));
            }
            else if (type == objects::entity)
            {
                glm::vec3 position = glm::vec3(0);

                int current = 0;
                for (auto& p : j.second["data"]["pos"].ArrayRange())
                {
                    if (current == 0)
                    { 
                        position.x = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 1)
                    {
                        position.y = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 2)
                    {
                        position.z = float(p.ToFloat());
                        current = 0;
                    }
                }

                current = 0;
                glm::vec3 rotation = glm::vec3(0);
                for (auto& p : j.second["data"]["rot"].ArrayRange())
                {
                    if (current == 0)
                    {
                        rotation.x = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 1)
                    {
                        rotation.y = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 2)
                    {
                        rotation.z = float(p.ToFloat());
                        current = 0;
                    }
                }
                std::shared_ptr<xplor::Entity> entity = std::make_shared<xplor::Entity>(int(j.second["data"][".model"].ToInt()), position, rotation, float(j.second["data"]["scale"].ToFloat()));
                entities->push_back(entity);
                children.push_back(std::make_shared<EntityComponent>(j.first, entity, model_names, j.second, entities, terrain, selectedID));
            }
            else if (type == objects::terrain)
            {
                glm::vec3 position = glm::vec3(0);

                int current = 0;
                for (auto& p : j.second["data"]["pos"].ArrayRange())
                {
                    if (current == 0)
                    {
                        position.x = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 1)
                    {
                        position.y = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 2)
                    {
                        position.z = float(p.ToFloat());
                        current = 0;
                    }
                }

                current = 0;
                glm::vec3 rotation = glm::vec3(0);
                for (auto& p : j.second["data"]["rot"].ArrayRange())
                {
                    if (current == 0)
                    {
                        rotation.x = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 1)
                    {
                        rotation.y = float(p.ToFloat());
                        current++;
                    }
                    else if (current == 2)
                    {
                        rotation.z = float(p.ToFloat());
                        current = 0;
                    }
                }
                children.push_back(std::make_shared<TerrainComponent>(j.first, model_names, j.second, entities, terrain, selectedID));
            }
        }
    }

    void HierarchyComponent::renderInHierarchy()
    {
        ImGui::PushID(&name);

        if (children.size())
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            if (*selectedID == this)
                flags |= ImGuiTreeNodeFlags_Selected;

            bool node_open = ImGui::TreeNodeEx(name.c_str(), flags);

            if (ImGui::IsItemClicked())
                *selectedID = this;

            ImGui::PushID(&name + 1);
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::BeginMenu("Add"))
                {
                    if (ImGui::MenuItem("Structure"))
                    {
                        children.push_back(std::make_shared<HierarchyComponent>("Structure", model_names, entities, terrain, selectedID));
                    }
                    if (ImGui::MenuItem("Entity"))
                    {
                        std::shared_ptr<xplor::Entity> entity = std::make_shared<xplor::Entity>(0, glm::vec3(0), glm::vec3(0), 1.0f);
                        entities->push_back(entity);
                        children.push_back(std::make_shared<EntityComponent>("Entity", entity, model_names, entities, terrain, selectedID));
                    }
                    if (ImGui::MenuItem("Terrain"))
                    {
                        children.push_back(std::make_shared<TerrainComponent>("Terrain", model_names, entities, terrain, selectedID));
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::MenuItem("Delete"))
                {
                    alive = false;
                    if (*selectedID == this)
                        *selectedID = nullptr;
                }

                ImGui::EndPopup();
            }
            ImGui::PopID();

            if (node_open)
            {
                unsigned indx = 0;
                for (auto& child : children)
                {
                    child->renderInHierarchy();

                    if(!child->alive)
                    {
                        children.erase(children.begin() + indx);
                        //ImGui::TreePop();
                        break;
                    }

                    indx++;
                }

                ImGui::TreePop();
            }
        }
        else
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Bullet;

            if (*selectedID == this)
                flags |= ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            bool node_open = ImGui::TreeNodeEx(name.c_str(), flags);

            if (ImGui::IsItemClicked())
                *selectedID = this;

            ImGui::PushID(&name + 1);
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::BeginMenu("Add"))
                {
                    if (ImGui::MenuItem("Structure"))
                    {
                        children.push_back(std::make_shared<HierarchyComponent>("Structure", model_names, entities, terrain, selectedID));
                    }
                    if (ImGui::MenuItem("Entity"))
                    {
                        std::shared_ptr<xplor::Entity> entity = std::make_shared<xplor::Entity>(0, glm::vec3(0), glm::vec3(0), 1.0f);
                        entities->push_back(entity);
                        children.push_back(std::make_shared<EntityComponent>("Entity", entity, model_names, entities, terrain, selectedID));
                    }
                    if (ImGui::MenuItem("Terrain"))
                    {
                        children.push_back(std::make_shared<TerrainComponent>("Terrain", model_names, entities, terrain, selectedID));
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::MenuItem("Delete"))
                {
                    alive = false;
                    if (*selectedID == this)
                        *selectedID = nullptr;
                }

                ImGui::EndPopup();
            }
            ImGui::PopID();

            if (node_open)
            {
                // do something when double clicked
                ImGui::TreePop();
            }
        }

        ImGui::PopID();
    }

    void HierarchyComponent::renderInInspector()
    {
        ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue);
    }

    json::JSON HierarchyComponent::buildJson()
    {
        json::JSON json;

        for (auto& c : children)
        {
            json["children"][c->name] = c->buildJson();
        }

        json[".type"] = int(objects::structure);
        json["data"] = createJsonObject(objects::structure);

        return json;
    }

    json::JSON HierarchyComponent::createJsonObject(unsigned object, glm::vec3 position, glm::vec3 rotation, float scale, int model_ID)
    {
        if (object == objects::structure)
        {
            json::JSON obj;

            return obj;
        }
        else if (object == objects::entity)
        {
            json::JSON obj;

            obj[".model"] = model_ID;
            obj["pos"] = json::Array(position.x, position.y, position.z);
            obj["rot"] = json::Array(rotation.x, rotation.y, rotation.z);
            obj["scale"] = scale;

            return obj;
        }
        else if (object == objects::terrain)
        {
            json::JSON obj;

            obj["pos"] = json::Array(position.x, position.y, position.z);
            obj["rot"] = json::Array(rotation.x, rotation.y, rotation.z);
            obj["scale"] = scale;

            return obj;
        }
        else
        {
            json::JSON obj;

            return obj;
        }
    }

    /*
     *   Entity Component
     */

    void EntityComponent::renderInInspector()
    {
        ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue);

        ImGui::Separator();

        std::string combo_label;

        if (entity->modelID != -1)
            combo_label = model_names->operator[](entity->modelID);  // Label to preview before opening the combo (technically could be anything)
        else
            combo_label = "";
        if (ImGui::BeginCombo("Model", combo_label.c_str()))
        {
            for (unsigned n = 0; n < model_names->size(); n++)
            {
                const bool is_selected = (entity->modelID == n);
                if (ImGui::Selectable(model_names->operator[](n).c_str(), is_selected))
                    entity->modelID = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            if (ImGui::Selectable("none"))
                entity->modelID = -1;

            ImGui::EndCombo();
        }

        if (ImGui::TreeNodeEx("Translation:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue;

            if (ImGui::TreeNodeEx("Position:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::InputFloat("X", &entity->position.x, .25, 1, 6, flags))
                    entity->updateTransformationMatrix();
                if (ImGui::InputFloat("Y", &entity->position.y, .25, 1, 6, flags))
                    entity->updateTransformationMatrix();
                if (ImGui::InputFloat("Z", &entity->position.z, .25, 1, 6, flags))
                    entity->updateTransformationMatrix();

                ImGui::TreePop();
            }

            ImGui::Separator();

            if (ImGui::TreeNodeEx("Rotation:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::DragFloat("X", &entity->rotation.x, 1, -180, 180))
                    entity->updateTransformationMatrix();
                if (ImGui::DragFloat("Y", &entity->rotation.y, 1, -180, 180))
                    entity->updateTransformationMatrix();
                if (ImGui::DragFloat("Z", &entity->rotation.z, 1, -180, 180))
                    entity->updateTransformationMatrix();

                ImGui::TreePop();
            }

            ImGui::Separator();

            if (ImGui::TreeNodeEx("Scale:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::InputFloat("S", &entity->scale, .25, 1, 6, flags))
                    entity->updateTransformationMatrix();

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }

    json::JSON EntityComponent::buildJson()
    {
        json::JSON json;

        for (auto& c : children)
        {
            json["children"][c->name] = c->buildJson();
        }

        json[".type"] = int(objects::entity);
        json["data"] = createJsonObject(objects::entity, entity->position, entity->rotation, entity->scale, entity->modelID);

        return json;
    }

    /*
     *   Terrain Component
     */

    void TerrainComponent::renderInInspector()
    {
        ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue);

        ImGui::Separator();

        if (ImGui::TreeNodeEx("Translation:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue;

            if (ImGui::TreeNodeEx("Position:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::InputFloat("X", &terrain->position.x, .25, 1, 6, flags))
                    terrain->updateTransformationMatrix();
                if (ImGui::InputFloat("Y", &terrain->position.y, .25, 1, 6, flags))
                    terrain->updateTransformationMatrix();
                if (ImGui::InputFloat("Z", &terrain->position.z, .25, 1, 6, flags))
                    terrain->updateTransformationMatrix();

                ImGui::TreePop();
            }

            ImGui::Separator();

            if (ImGui::TreeNodeEx("Scale:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::InputFloat("S", &terrain->scale, .25, 1, 6, flags))
                    terrain->updateTransformationMatrix();

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        if (ImGui::TreeNodeEx("Terrain:", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Text("Stuff.");
            ImGui::TreePop();
        }
    }
    json::JSON TerrainComponent::buildJson()
    {
        json::JSON json;

        for (auto& c : children)
        {
            json["children"][c->name] = c->buildJson();
        }

        json[".type"] = int(objects::terrain);
        json["data"] = createJsonObject(objects::terrain, terrain->position, glm::vec3(0), terrain->scale);

        return json;
    }
}