#pragma once

#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "glad/gl.h"

#include "types.h"

namespace sws {

    struct PerFrameData
    {
        glm::mat4 mvp;
        int isWireframe;
    };

    struct Mesh {
        GLuint vao;
        GLuint position_vbo;
        GLuint normals_vbo;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
    };

    struct Node {
        u32 mesh_idx;
        glm::mat4 transform;
    };

    struct RenderState {
        GLuint per_frame_bid;
        GLuint light_uniform;
        std::vector<Mesh> meshes;
        std::vector<Node> nodes;

    };

    void initialize(RenderState &state);
    void render(const sws::RenderState &state, const f32 ratio);
}

