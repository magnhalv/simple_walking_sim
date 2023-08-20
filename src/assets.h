#pragma once

#include <vector>

#include <glm/glm.hpp>
#include "glad/gl.h"

#include "types.h"

namespace sws {
    struct Material {
        glm::vec4 diffuse_color;
        glm::vec4 specular_color;
        glm::vec4 emission_color;
        glm::vec4 ambient_color;
        float specular_exponent;
    };

    struct Mesh {
        GLuint vao;
        GLuint position_vbo;
        GLuint normals_vbo;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
    };

    struct Node {
        std::vector<u32> mesh_indices;
        std::vector<u32> material_indices;
        glm::mat4 transform;
    };

    struct MeshGroup {
        std::vector<Mesh> meshes;
        std::vector<Material> materials;
        std::vector<Node> nodes;
    };

    inline auto get_material(const char *material_name) -> Material {
        if (std::strcmp(material_name, "metal") == 0) {
            return {
                    .diffuse_color = glm::vec4(0.3, 0.3, 0.3, 0),
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
                    .ambient_color = glm::vec4(0.21/10.0, 0.15/10.0, 0.12/10.0, 0),
                    .specular_exponent = 20
            };
        }
        if (std::strcmp(material_name, "wall_dark") == 0) {
            return {
                    .diffuse_color = glm::vec4(0.4, 0.4, 0.4, 0),
                    .specular_color = glm::vec4(0.0, 0.0, 0.0, 0),
                    .emission_color = glm::vec4(0, 0, 0, 0),
                    .ambient_color = glm::vec4(0.4/10.0, 0.4/10.0, 0.4/10.0, 0),
                    .specular_exponent = 0
            };
        }
        if (std::strcmp(material_name, "wall_light") == 0) {
            return {
                    .diffuse_color = glm::vec4(0.8, 0.8, 0.8, 0),
                    .specular_color = glm::vec4(0.0, 0.0, 0.0, 0),
                    .emission_color = glm::vec4(0, 0, 0, 0),
                    .ambient_color = glm::vec4(0.7/10.0, 0.7/10.0, 0.7/10.0, 0),
                    .specular_exponent = 0
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
