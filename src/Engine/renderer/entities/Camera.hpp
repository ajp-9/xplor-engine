#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace xplor
{
    struct Camera
    {
        float fov;
        const float SENSITIVITY = 0.1f;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        // z up, left-handed
        glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

        enum Camera_Movement
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN
        };

        glm::mat4 getViewMatrix()
        {
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(position, position + front, up);
            return view;
        }
    };
}