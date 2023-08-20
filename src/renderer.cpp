#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "renderer.h"
#include "GLFW/glfw3.h"

void allocate_uniform_buffer(u32 *id, u32 buffer_base, GLsizeiptr size) {
    glCreateBuffers(1, id);
    glNamedBufferStorage(*id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, buffer_base, *id);
}

void sws::initialize(sws::RenderState &state) {
    GLShader vertex_shader("./shaders/mesh.vert");
    GLShader frag_shader("./shaders/phong.frag");
    state.shader_program.initialize(vertex_shader, frag_shader);
    state.shader_program.useProgram();

    allocate_uniform_buffer(&state.per_frame_bid, 0, sizeof(sws::PerFrameData));
    allocate_uniform_buffer(&state.light_bid, 1, sizeof(sws::LightData));
    allocate_uniform_buffer(&state.material_bid, 2, sizeof(sws::Material));

    for (auto &mesh: state.assets.meshes) {
        glCreateVertexArrays(1, &mesh.vao);

        glCreateBuffers(1, &mesh.position_vbo);
        auto data_size = static_cast<GLsizeiptr>(sizeof(glm::vec3) * mesh.positions.size()); // NOLINT
        assert(mesh.positions.size() == mesh.normals.size());
        // Populates the buffer
        glNamedBufferStorage(mesh.position_vbo, data_size, mesh.positions.data(), 0);
        // Binds the buffer to binding point 0 in the vao
        glVertexArrayVertexBuffer(mesh.vao, 0, mesh.position_vbo, 0, sizeof(glm::vec3));
        // Enables vertex attribute 0
        glEnableVertexArrayAttrib(mesh.vao, 0);
        glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        // Makes vertex attribute available in shader layout=0
        glVertexArrayAttribBinding(mesh.vao, 0, 0);

        glCreateBuffers(1, &mesh.normals_vbo);
        // Populates the buffer
        glNamedBufferStorage(mesh.normals_vbo, data_size, mesh.normals.data(), 0);
        // Binds the buffer to binding point 1 in the vao
        glVertexArrayVertexBuffer(mesh.vao, 1, mesh.normals_vbo, 0, sizeof(glm::vec3));
        // Enables vertex attribute 1
        glEnableVertexArrayAttrib(mesh.vao, 1);
        glVertexArrayAttribFormat(mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
        // Makes vertex attribute available in shader layout=1
        glVertexArrayAttribBinding(mesh.vao, 1, 1);
    }
    glBindVertexArray(0);

    state.t = 0;
}

void sws::render(const sws::RenderState &state, const f32 ratio, const Camera &camera) {
    state.shader_program.useProgram();
    const GLsizeiptr kBufferSize = sizeof(sws::PerFrameData);
    const GLsizeiptr light_buffer_size = sizeof(sws::LightData);
    auto &assets = state.assets;

    auto change = state.t;
    auto change_y = state.t * 0;
    sws::LightData light_data = {
            .omni_pos = glm::vec4(sin(change) * 3.0, 0.5, 1.0, 1 / 3.0f),
            .omni_color = glm::vec4(1.0, 1.0, 1.0, 0),
            .eye_pos = glm::vec4(camera.get_position(), 0.0f)};
    glNamedBufferSubData(state.light_bid, 0, light_buffer_size, &light_data);

    for (const auto &node: assets.nodes) {
        for (auto i = 0; i < node.mesh_indices.size(); i++) {
            const auto &mesh = assets.meshes[node.mesh_indices[i]];
            const auto &material = assets.materials[node.material_indices[i]];
            glBindVertexArray(mesh.vao);

            const glm::mat4 m = camera.get_view() * node.transform;
            const glm::mat4 p = glm::perspective(45.0f, ratio, 0.1f, 1000.0f);
            sws::PerFrameData perFrameData = {.mvp = p * m};
            glNamedBufferSubData(state.per_frame_bid, 0, kBufferSize, &perFrameData);

            glNamedBufferSubData(state.material_bid, 0, sizeof(sws::Material), &material);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, mesh.positions.size());
        }


        //perFrameData.isWireframe = true;
        //glNamedBufferSubData( state.per_frame_bid, 0, kBufferSize, &perFrameData );

        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        //glDrawArrays( GL_TRIANGLES, 0, mesh.positions.size() );
    }
    glFinish();
}

void free(std::vector<sws::Mesh> &meshes) {
    for (auto &mesh: meshes) {
        glDeleteBuffers(1, &mesh.position_vbo);
    }
}