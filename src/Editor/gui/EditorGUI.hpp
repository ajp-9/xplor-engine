#pragma once

#include "Hierarchy_Inspector/WorldHierarchy_InspectorGUI.hpp"

enum gui_events
{
	file_new,
	file_open,
	file_save,
	file_save_as,
    file_quit,
    
	add_3d_object
};

namespace gui
{
    struct Viewport : public xplor::gui::GUI
    {
        unsigned int fbo;
        unsigned int rbo;
        unsigned int texture;

        ImVec2 lastSize;

        void Init()
        {
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            // create a color attachment texture
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
            // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 2048, 2048); // use a single renderbuffer object for both a depth AND stencil buffer.
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
            // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void update(std::vector<int>& events) override
        {
            ImGui::Begin("Viewport", (bool*)0, ImGuiWindowFlags_NoCollapse);
            ImGui::BeginChild("GameRender");
            ImVec2 wsize = ImGui::GetWindowSize();

            //glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glBindTexture(GL_TEXTURE_2D, texture);

            if (lastSize.x != wsize.x || lastSize.y != wsize.y)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, int(wsize.x), int(wsize.y), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                glViewport(0, 0, int(wsize.x), int(wsize.y));
                lastSize = wsize;
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

            ImGui::Image((ImTextureID)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::EndChild();
            ImGui::End();
        }

        void setFramebufferActive()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        }
    };

    struct FileSystem : public xplor::gui::GUI
    {
        void update(std::vector<int>& events) override
        {
            ImGui::Begin("File System", (bool*)0, ImGuiWindowFlags_NoCollapse);

            ImGui::End();
        }

        void reload()
        {

        }
    };

    struct Console : public xplor::gui::GUI
    {
        ImVector<char*> logs;
        bool autoScroll = false;
        bool overrideAutoScroll = true;

        void update(std::vector<int>& events) override
        {
            ImGui::Begin("Console", (bool*)0, ImGuiWindowFlags_NoCollapse);

            ImGui::SameLine();
            if (ImGui::Button("Clear"))
                clearLog();
            ImGui::SameLine();

            if (ImGui::Button("Add log"))
                addLog("This is a log.");
            ImGui::SameLine();

            if (ImGui::Button("Add warning"))
                addLog("This is a warning.", 1);
            ImGui::SameLine();

            if (ImGui::Button("Add error"))
                addLog("This is a error.", 2);

            ImGui::SameLine();
            ImGui::Checkbox("Auto-scroll", &overrideAutoScroll);

            ImGui::Separator();

            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            for (auto& l : logs)
            {
                ImVec4 color;
                bool has_color = false;
                if (strstr(l, "[Error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                else if (strncmp(l, "[Warning]", 2) == 0) { color = ImVec4(0.981f, 1.000f, 0.358f, 1.000f); has_color = true; }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(l);
                if (has_color)
                    ImGui::PopStyleColor();
            }
            
            if (autoScroll && overrideAutoScroll)
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild();

            ImGui::End();
        }

        // Warning Levels:
        // None/0 = [Log]
        // 1 = [Warning]
        // 2 = [Error]
        void addLog(std::string msg, int level = 0)
        {
            if (level == 0)
                msg.insert(0, "[Log] ");
            if (level == 1)
                msg.insert(0, "[Warning] ");
            if (level == 2)
                msg.insert(0, "[Error] ");

            char* newMsg = _strdup(msg.c_str());
            logs.push_back(newMsg);
            autoScroll = true;
        }

        void clearLog()
        {
            for (auto& l : logs)
                free(l);
            logs.clear();
        }
    };

    struct Tools : public xplor::gui::GUI
    {
        void update(std::vector<int>& events) override
        {
            ImGui::Begin("Tools", (bool*)0, ImGuiWindowFlags_NoCollapse);

            ImGui::End();
        }
    };

    struct DockSpace : public xplor::gui::GUI
    {
        void update(std::vector<int>& events) override
        {
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", (bool*)0, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New"))
                    {
                        events.push_back(gui_events::file_new);
                    }

                    if (ImGui::MenuItem("Open"))
                    {
                        events.push_back(gui_events::file_open);
                    }

                    if (ImGui::MenuItem("Save"))
                    {
                        events.push_back(gui_events::file_save);
                    }

                    if (ImGui::MenuItem("Save As"))
                    {
                        events.push_back(gui_events::file_save_as);
                    }

                    ImGui::Separator();

                    if (ImGui::MenuItem("Options"))
                    {
                        events.push_back(gui_events::file_save_as);
                    }

                    ImGui::Separator();

                    if (ImGui::MenuItem("Quit"))
                    {
                        events.push_back(gui_events::file_quit);
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo"))
                    {
                        //events.push_back(gui_events::add_3d_object);
                    }

                    if (ImGui::MenuItem("Redo"))
                    {
                        //events.push_back(gui_events::add_3d_object);
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Add"))
                {
                    if (ImGui::MenuItem("3D Object"))
                    {
                        events.push_back(gui_events::add_3d_object);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }
    };
}