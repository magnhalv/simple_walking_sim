

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <glad/gl.h>
#include <glad/wgl.h>
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
#include "input.h"

#include <windows.h>
#include <errhandlingapi.h>
#include <windowsx.h>
#include <iostream>

typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int *);
typedef const char *(WINAPI *PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int);
typedef int(WINAPI *PFNWGLGETSWAPINTERVALEXTPROC)(void);


Camera camera{-251.0f, 0.3f, glm::vec3(0.0f, 1.0f, -20.0f)};

bool is_running = true;

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void win32_process_pending_messages(Input &new_input, Input &old_input);

#if _DEBUG
#pragma comment(linker, "/subsystem:console")

int main(int argc, const char **argv) {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}

#else
#pragma comment(linker, "/subsystem:windows")
#endif

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    wndclass.lpszMenuName = 0;
    wndclass.lpszClassName = "Win32 Game Window";
    RegisterClassEx(&wndclass);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int client_width = 1280;
    int client_height = 720;
    RECT windowRect;
    SetRect(&windowRect, (screenWidth / 2) - (client_width / 2), (screenHeight / 2) - (client_height / 2),
            (screenWidth / 2) + (client_width / 2), (screenHeight / 2) + (client_height / 2));

    DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX |
                   WS_MAXIMIZEBOX); // WS_THICKFRAME to resize
    AdjustWindowRectEx(&windowRect, style, FALSE, 0);
    HWND hwnd = CreateWindowEx(0, wndclass.lpszClassName, "Game Window", style, windowRect.left, windowRect.top,
                               windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL,
                               hInstance, szCmdLine);
    HDC hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    // Create OpenGL context
    HGLRC tempRC = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempRC);
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");

    const int attribList[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            3,
            WGL_CONTEXT_MINOR_VERSION_ARB,
            3,
            WGL_CONTEXT_FLAGS_ARB,
            0,
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0,
    };
    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribList);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(hdc, hglrc);

    if (!gladLoaderLoadGL()) {
        std::cout << "Could not initialize GLAD\n";
    } else {
        //std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " loaded\n";
    }

    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress(
            "wglGetExtensionsStringEXT");
    bool swapControlSupported = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;

    int vsynch = 0;
    if (swapControlSupported) {
        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress(
                "wglSwapIntervalEXT");
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress(
                "wglGetSwapIntervalEXT");

        if (wglSwapIntervalEXT(1)) {
            std::cout << "Enabled vsynch\n";
            vsynch = wglGetSwapIntervalEXT();
        } else {
            std::cout << "Could not enable vsynch\n";
        }
    } else { // !swapControlSupported
        std::cout << "WGL_EXT_swap_control not supported\n";
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);


    GLShader vertex_shader("./shaders/mesh.vert");
    GLShader frag_shader("./shaders/basic_light.frag");
    GLProgram shader_program(vertex_shader, frag_shader);
    shader_program.useProgram();

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    const aiScene *scene = aiImportFile("vehicles.glb", aiProcess_Triangulate);

    if (scene == nullptr || !scene->HasMeshes()) {
        printf("Unable to load basic_scene.glb\n");
        exit(255);
    } else {
        printf("Scene loaded.\n");
    }

    // TODO: Move this to importer class
    sws::RenderState state;
    for (i32 m = 0; m < scene->mNumMeshes; m++) {
        sws::Mesh &mesh = state.meshes.emplace_back();
        const auto &ai_mesh = scene->mMeshes[m];

        for (u32 f = 0; f < ai_mesh->mNumFaces; f++) {
            const auto &ai_face = ai_mesh->mFaces[f];
            assert(ai_face.mNumIndices == 3);
            const u32 indices[3] = {ai_face.mIndices[0], ai_face.mIndices[1], ai_face.mIndices[2]};
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
    // END TODO

    //assert(state.nodes.size() == state.meshes.size());
    //assert(state.nodes.size() == 3);

    printf("Converted from assimp to local representation.\n");
    aiReleaseImport(scene);

    sws::initialize(state);

    Input inputs[2];
    u32 curr_input = 0;
    auto &current_input = inputs[curr_input];
    auto &previous_input = inputs[curr_input + 1];

    while (is_running) {
        int width, height;
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        height = clientRect.bottom - clientRect.top;
        width = clientRect.right - clientRect.left;
        const f32 ratio = static_cast<f32>(width) / static_cast<f32>(height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shader_program.useProgram();


        win32_process_pending_messages(current_input, previous_input);

        glViewport(0, 0, width, height);
        glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        sws::render(state, camera.get_view(), ratio);

        SwapBuffers(hdc);
        if (vsynch != 0) {
            glFinish();
        }

        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            printf("OpenGL Error %d\n", err);
        }
    }

    glDeleteVertexArrays(1, &vao);
    // Clean up?

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch (iMsg) {
        case WM_CLOSE:
            is_running = false;
            break;
        case WM_DESTROY: {
            is_running = false;
            /*HDC hdc = GetDC(hwnd);
            HGLRC hglrc = wglGetCurrentContext();

            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hglrc);
            ReleaseDC(hwnd, hdc);

            PostQuitMessage(0);*/
        }

            break;
            // TODO: Need to look into what this does.
        case WM_PAINT:
        case WM_ERASEBKGND:

            return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void win32_process_keyboard_message(ButtonState &new_state, bool is_down) {
    if (new_state.ended_down != is_down) {
        new_state.ended_down = is_down;
        new_state.half_transition_count++;
    }
}

void win32_process_pending_messages(Input &new_input, Input &old_input) {
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
        switch (message.message) {
            case WM_QUIT: {
                is_running = false;
            }
                break;
            case WM_LBUTTONDOWN: {
                // new_input.left_mouse = true;
                //input_add_event(new_input, InputEvent::LeftClick);
                //new_input.left_mouse = true;
            }
                break;
            case WM_LBUTTONUP: {
                //new_input.left_mouse = false;
            }
                break;
            case WM_MOUSEMOVE: {
                i32 xPos = GET_X_LPARAM(message.lParam);
                i32 yPos = GET_Y_LPARAM(message.lParam);

                camera.update_cursor(xPos, yPos);
                //new_input.mouse_x = xPos;
                //new_input.mouse_y = yPos;

                //if (!old_input.mouse_has_entered) {
//                    new_input.mouse_has_entered = true;
//                    old_input.mouse_x = xPos;
//                    old_input.mouse_y = yPos;
//                }

                //new_input.mouse_dx = new_input.mouse_x - old_input.mouse_x;
                //new_input.mouse_dy = new_input.mouse_y - old_input.mouse_y;
            }
                break;
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP: {
                u32 vk_code = (u32) message.wParam;
                bool was_down = ((message.lParam & (1 << 30)) != 0);
                bool is_down = ((message.lParam & (1 << 31)) == 0);
                if (was_down != is_down) {
                    if (vk_code == 'W') {
                        win32_process_keyboard_message(new_input.move_up, is_down);
                    }
                    if (vk_code == 'A') {
                        win32_process_keyboard_message(new_input.move_left, is_down);
                    }
                    if (vk_code == 'S') {
                        win32_process_keyboard_message(new_input.move_down, is_down);
                    }
                    if (vk_code == 'D') {
                        win32_process_keyboard_message(new_input.move_right, is_down);
                    }
                    if (vk_code == VK_UP) {
                    }
                    if (vk_code == VK_DOWN) {
                    }
                    if (vk_code == VK_RIGHT) {
                    }
                    if (vk_code == VK_LEFT) {
                    }
                    if (vk_code == VK_ESCAPE) {
                        is_running = false;
                    }
                    if (vk_code == VK_SPACE) {
                        win32_process_keyboard_message(new_input.space, is_down);
                    }
                }
            }
                break;
            default:
                TranslateMessage(&message);
                DispatchMessageA(&message);
        }
    }
}
