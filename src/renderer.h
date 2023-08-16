#pragma once

#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "glad/gl.h"

#include "types.h"
#include "camera.h"
#include "gl_shader.h"

namespace sws {

    static const size_t MAX_LENGTH = 1024;

    struct PerFrameData
    {
        glm::mat4 mvp;
    };

    struct LightData
    {
        glm::vec4 omni_pos;
        glm::vec4 eye_pos;
    };

    struct Mesh {
        GLuint vao;
        GLuint position_vbo;
        GLuint normals_vbo;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
    };

    struct Material {
        char name[MAX_LENGTH];
        glm::vec4 diffuse_color;
        glm::vec4 specular_color;
        float specular_exponent;
    };

    struct Node {
        u32 mesh_idx;
        u32 material_idx;
        glm::mat4 transform;
    };

    struct RenderState {
        GLuint per_frame_bid;
        GLuint light_uniform;
        std::vector<Mesh> meshes;
        std::vector<Material> materials;
        std::vector<Node> nodes;
        f32 dt;
        f32 t;

        GLProgram shader_program;
    };

    void initialize(RenderState &state);
    void render(const sws::RenderState &state, f32 ratio, const Camera &camera);
}

