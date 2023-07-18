#pragma once

#include <glm/glm.hpp>

#include "types.h"

class Camera {
public:
    void update(f32 x_pos, f32 y_pos) {
        if (first_mouse) {
            x_last = x_pos;
            y_last = y_pos;
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
    f32 yaw = -251.0f;
    f32 pitch = 0.3f;
    f32 y_last;
    f32 x_last;

    bool first_mouse = false;

    glm::vec3 position{0.0f, -0.5f, -20.5f};
    glm::vec3 direction;
    glm::vec3 up{0, 1, 0};
};
