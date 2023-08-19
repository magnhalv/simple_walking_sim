#pragma once

#include <map>

#include <glm/glm.hpp>
#include "types.h"

namespace sws {
    struct Material {
        glm::vec4 diffuse_color;
        glm::vec4 specular_color;
        glm::vec4 emission_color;
        glm::vec4 ambient_color;
        float specular_exponent;
    };

    inline auto get_material(const char *material_name) -> Material {
        if (std::strcmp(material_name, "metal") == 0) {
            return {
                    .diffuse_color = glm::vec4(0.2, 0.2, 0.2, 0),
                    .specular_color = glm::vec4(0.7,  0.7, 0.7, 0),
                    .emission_color = glm::vec4(0, 0, 0, 0),
                    .ambient_color = glm::vec4(0, 0, 0, 0),
                    .specular_exponent = 70.0
            };
        }
        if (std::strcmp(material_name, "wood") == 0) {
            return {
                    .diffuse_color = glm::vec4(0.21, 0.15, 0.12, 0),
                    .specular_color = glm::vec4(0.1, 0.1, 0.1, 0),
                    .emission_color = glm::vec4(0, 0, 0, 0),
                    .ambient_color = glm::vec4(0.38, 0.25, 0.1, 0),
                    .specular_exponent = 20
            };
        }
        printf("WARNING: Unknown material: %s\n", material_name);
        return {
                .diffuse_color = glm::vec4(205 / 255.0, 117 / 255.0, 132 / 255.0, 0),
                .emission_color = glm::vec4(0, 0, 0, 0),
                .ambient_color = glm::vec4(205 / 255.0, 117 / 255.0, 132 / 255.0, 0),
                .specular_exponent = 1000
        };
    }
}