#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "renderer.h"
#include "GLFW/glfw3.h"


void sws::initialize(sws::RenderState &state) {
    const GLsizeiptr kBufferSize = sizeof( sws::PerFrameData );
    glCreateBuffers( 1, &state.per_frame_bid );
    glNamedBufferStorage( state.per_frame_bid, kBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT );
    glBindBufferRange( GL_UNIFORM_BUFFER, 0, state.per_frame_bid, 0, kBufferSize );

    for (auto &mesh : state.meshes ) {
        glCreateVertexArrays(1, &mesh.vao);

        glCreateBuffers(1, &mesh.bid);
        auto data_size = static_cast<GLsizeiptr>(sizeof(glm::vec3) * mesh.positions.size()); // NOLINT

        // Populates the buffer
        glNamedBufferStorage(mesh.bid, data_size, mesh.positions.data(), 0);

        // Binds the buffer to binding point 0 in the vao
        glVertexArrayVertexBuffer(mesh.vao, 0, mesh.bid, 0, sizeof(glm::vec3));
        // Enables vertex attribute 0
        glEnableVertexArrayAttrib(mesh.vao, 0);
        glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        // Makes vertex attribute available in shader layout=0
        glVertexArrayAttribBinding(mesh.vao, 0, 0);
    }
    glBindVertexArray(0);
}
void sws::render(const sws::RenderState &state, const f32 ratio) {

    const GLsizeiptr kBufferSize = sizeof( sws::PerFrameData );
    for (const auto &node: state.nodes) {
        const glm::mat4 m = glm::rotate( glm::translate( node.transform, glm::vec3( 0.0f, -0.5f, -20.5f ) ), (f32)glfwGetTime(), glm::vec3( 0.0f, 1.0f, 0.0f ) );
        //const glm::mat4 m = glm::translate( node.transform, glm::vec3( 0.0f, -0.5f, -20.5f ) );
        const glm::mat4 p = glm::perspective( 45.0f, ratio, 0.1f, 1000.0f );

        sws::PerFrameData perFrameData = { .mvp = p * m, .isWireframe = false };

        const auto &mesh = state.meshes[node.mesh_idx];
        glNamedBufferSubData( state.per_frame_bid, 0, kBufferSize, &perFrameData );

        glBindVertexArray(mesh.vao);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDrawArrays( GL_TRIANGLES, 0, mesh.positions.size() );

        perFrameData.isWireframe = true;
        glNamedBufferSubData( state.per_frame_bid, 0, kBufferSize, &perFrameData );

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        //glDrawArrays( GL_TRIANGLES, 0, mesh.positions.size() );
    }
}

void free(std::vector<sws::Mesh> &meshes) {
    for (auto &mesh : meshes ) {
        glDeleteBuffers(1, &mesh.bid);
    }
}