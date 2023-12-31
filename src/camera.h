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
        const f32 speed = 0.025f;
        if (input.move_up.ended_down) {
            position.x += dir.x * speed;
            position.z += dir.y * speed;
        }
        if (input.move_down.ended_down) {
            position.x -= dir.x * speed;
            position.z -= dir.y * speed;
        }
        glm::vec3 cross = glm::cross(direction, up);
        glm::vec2 side_dir = glm::normalize(glm::vec2(cross.x, cross.z));
        if (input.move_left.ended_down) {
            position.x -= side_dir.x * speed;
            position.z -= side_dir.y * speed;
        }
        if (input.move_right.ended_down) {
            position.x += side_dir.x * speed;
            position.z += side_dir.y * speed;
        }
    }

    void update_cursor(f32 dx, f32 dy) {
        f32 sensitivity = 0.2f;
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

    [[nodiscard]] auto get_view() const -> glm::mat4 {
        return glm::lookAt(position, position + direction, up);
    }

    [[nodiscard]] auto get_position() const -> glm::vec3 {
        return position;
    }
private:
    f32 yaw;
    f32 pitch;
    glm::vec3 position;
    glm::vec3 direction;
    const glm::vec3 up{0, 1, 0};
};
