

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "types.h"
#include "gl_shader.h"
#include "renderer.h"
#include "assimp_util.h"
#include "camera.h"

Camera camera;


void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
    camera.update(static_cast<float>(x_pos), static_cast<float>(y_pos));
}

int main()
{
	glfwSetErrorCallback(
		[]( int error, const char* description )
		{
			fprintf( stderr, "Error: %s\n", description );
		}
	);

	if ( !glfwInit() )
		exit( EXIT_FAILURE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	GLFWwindow* window = glfwCreateWindow( 1024, 768, "Simple example", nullptr, nullptr );
	if ( !window )
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetKeyCallback(
		window,
		[]( GLFWwindow* window, int key, int scancode, int action, int mods )
		{
			if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
				glfwSetWindowShouldClose( window, GLFW_TRUE );
		}
	);

	glfwMakeContextCurrent( window );
	gladLoadGL( glfwGetProcAddress );
	glfwSwapInterval( 1 );

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);


    GLShader vertex_shader("./shaders/mesh.vert");
    GLShader frag_shader("./shaders/basic_light.frag");
    GLProgram shader_program(vertex_shader, frag_shader);
	shader_program.useProgram();

	GLuint vao;
	glCreateVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

    const aiScene* scene = aiImportFile( "vehicles.glb", aiProcess_Triangulate );

    if ( scene == nullptr || !scene->HasMeshes()) {
        printf("Unable to load basic_scene.glb\n");
        exit(255);
    }
    else {
        printf("Scene loaded.\n");
    }


    sws::RenderState state;
    for (i32 m = 0; m < scene->mNumMeshes; m++) {
        sws::Mesh &mesh = state.meshes.emplace_back();
        const auto &ai_mesh = scene->mMeshes[m];

        for (u32 f = 0; f < ai_mesh->mNumFaces; f++) {
            const auto &ai_face = ai_mesh->mFaces[f];
            assert(ai_face.mNumIndices == 3);
            const u32 indices[3] = { ai_face.mIndices[0], ai_face.mIndices[1], ai_face.mIndices[2]};
            for (const auto idx: indices) {
                assert(idx < ai_mesh->mNumVertices);
                const auto &ai_vec = ai_mesh->mVertices[idx];
                mesh.positions.emplace_back(ai_vec.x, ai_vec.y, ai_vec.z);

                const auto &ai_norm = ai_mesh->mNormals[idx];
                mesh.normals.emplace_back(ai_norm.x, ai_norm.y, ai_norm.z);
            }
        }
    }

    printf("  Meshed loaded.\n");

    const auto root = scene->mRootNode;
    for (i32 c = 0; c < root->mNumChildren; c++) {
        const auto &child = root->mChildren[c];

        if (child->mNumMeshes == 0) {
            continue;
        }

        if (child->mNumChildren > 0) {
            printf("Child with children: %s", child->mName.data);
        }

        auto &node = state.nodes.emplace_back();
        node.transform = ConvertMatrixToGLMFormat(child->mTransformation);
        node.mesh_idx = child->mMeshes[0];
    }

    //assert(state.nodes.size() == state.meshes.size());
    //assert(state.nodes.size() == 3);

    printf("Converted from assimp to local representation.\n");
    aiReleaseImport(scene);

    sws::initialize(state);

	while ( !glfwWindowShouldClose( window ) )
	{
        int width, height;
        glfwGetFramebufferSize( window, &width, &height );
        const f32 ratio = static_cast<f32>(width) / static_cast<f32>(height);

        glViewport( 0, 0, width, height );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glEnable(GL_DEPTH_TEST);

        shader_program.useProgram();
        sws::render(state, camera.get_view(), ratio);

        glfwSwapBuffers( window );
        glfwPollEvents();
	}

	glDeleteVertexArrays( 1, &vao );

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}
