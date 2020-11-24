#pragma once

#include "../../../Engine/xplor.hpp"


struct PlayerController : virtual public xplor::Camera
{
    bool firstMouse = true;

    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = .1f;
    const float FAST_SPEED = 30.0f;
    const float SENSITIVITY = .1f; // change this value to your liking

    float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch = 0.0f;
    float lastX = 800.0f / 2.0;
    float lastY = 600.0 / 2.0;

    void move(int direction)
    {
        if (direction == FORWARD)
            position += front * SPEED;
        if (direction == BACKWARD)
            position -= front * SPEED;
        if (direction == LEFT)
            position -= right * SPEED;
        if (direction == RIGHT)
            position += right * SPEED;
        if (direction == DOWN)
            position += glm::normalize(glm::cross(front, right)) * SPEED;
        if (direction == UP)
            position -= glm::normalize(glm::cross(front, right)) * SPEED;
    }

    void tryKeyPress()
    {
        const Uint8* k = SDL_GetKeyboardState(NULL);

        if (k[SDL_SCANCODE_W])
            move(FORWARD);
        if (k[SDL_SCANCODE_S])
            move(BACKWARD);
        if (k[SDL_SCANCODE_A])
            move(LEFT);
        if (k[SDL_SCANCODE_D])
            move(RIGHT);
        if (k[SDL_SCANCODE_E])
            move(UP);
        if (k[SDL_SCANCODE_Q])
            move(DOWN);
    }

    void moveMouse(float xpos, float ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos;
        float yoffset = -ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        xoffset *= SENSITIVITY;
        yoffset *= SENSITIVITY;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.z = sin(glm::radians(pitch));
        front.y = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, up));
    }
};