#pragma once

#include <glm/glm.hpp>
#include <glfw/glfw3.h>

#include "types.h"
#include "input.h"

class Camera {
public:
    Camera(f32 yaw, f32 pitch, glm::vec3 position)
    : yaw(yaw), pitch(pitch), position(position) {}

    void update_keyboard(Input &input) {
        glm::vec2 dir = glm::normalize(glm::vec2(direction.x, direction.z));
        if (input.move_up.ended_down) {
            position.x += dir.x * 0.1f;
            position.z += dir.y * 0.1f;
        }
        if (input.move_down.ended_down) {
            position.x -= dir.x * 0.1f;
            position.z -= dir.y * 0.1f;
        }
        glm::vec3 cross = glm::cross(direction, up);
        glm::vec2 side_dir = glm::normalize(glm::vec2(cross.x, cross.z));
        if (input.move_left.ended_down) {
            position.x -= side_dir.x * 0.1f;
            position.z -= side_dir.y * 0.1f;
        }
        if (input.move_right.ended_down) {
            position.x += side_dir.x * 0.1f;
            position.z += side_dir.y * 0.1f;
        }
    }

    void update_cursor(f32 dx, f32 dy) {
        f32 sensitivity = 0.1f;
        yaw += dx*sensitivity;
        pitch -= dy*sensitivity;

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

        //printf("Yaw %f pitch %f\n", yaw, pitch);
    }

    glm::mat4 get_view() {
        return glm::lookAt(position, position + direction, up);
    }

    [[nodiscard]] auto get_direction() const -> glm::vec3 {
        return position;
    }
private:
    f32 yaw;
    f32 pitch;
    glm::vec3 position{0.0f, -0.5f, -20.5f};
    glm::vec3 direction{0.0f, 0.0f, 0.0f};
    const glm::vec3 up{0, 1, 0};
};
