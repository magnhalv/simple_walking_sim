#pragma once

#include <glm/glm.hpp>

#include "types.h"

class Camera {
public:
    Camera(f32 yaw, f32 pitch, glm::vec3 position)
    : yaw(yaw), pitch(pitch), position(position), x_last(0), y_last(0) {}

    void update_keyboard(int key, int action) {
        glm::vec2 dir = glm::normalize(glm::vec2(direction.x, direction.z));
        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            position.x += dir.x * 0.1f;
            position.z += dir.y * 0.1f;
        }
        if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            position.x -= dir.x * 0.1f;
            position.z -= dir.y * 0.1f;
        }
        glm::vec3 cross = glm::cross(direction, up);
        glm::vec2 side_dir = glm::normalize(glm::vec2(cross.x, cross.z));
        if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            position.x -= side_dir.x * 0.1f;
            position.z -= side_dir.y * 0.1f;
        }
        if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            position.x += side_dir.x * 0.1f;
            position.z += side_dir.y * 0.1f;
        }
    }

    void update_cursor(f32 x_pos, f32 y_pos) {
        if (first_mouse) {
            x_last = x_pos;
            y_last = y_pos;
            first_mouse = false;
        }

        f32 x_offset = x_pos - x_last;
        f32 y_offset = y_pos - y_last;
        x_last = x_pos;
        y_last = y_pos;

        f32 sensitivity = 0.1f;
        yaw += x_offset*sensitivity;
        pitch -= y_offset*sensitivity;

        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        glm::vec3 new_direction;
        new_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_direction.y = sin(glm::radians(pitch));
        new_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction = glm::normalize(new_direction);

        printf("Yaw %f pitch %f\n", yaw, pitch);
    }

    glm::mat4 get_view() {
        return glm::lookAt(position, position + direction, up);
    }
private:
    f32 yaw;
    f32 pitch;
    f32 y_last;
    f32 x_last;
    bool first_mouse = true;
    glm::vec3 position{0.0f, -0.5f, -20.5f};
    glm::vec3 direction{0.0f, 0.0f, 0.0f};
    const glm::vec3 up{0, 1, 0};
};
