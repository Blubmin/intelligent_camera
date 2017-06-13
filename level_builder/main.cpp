#include <conio.h>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <imgui\imgui.h>

#include <engine_base\BasicCamera.h>
#include <engine_base\DebugRenderer.h>
#include <engine_base\Entity.h>
#include <engine_base\EntityFactory.h>
#include <engine_base\ExceptionWrapper.h>
#include <engine_base\FlatRenderer.h>
#include <engine_base\GLSL.h>
#include <engine_base\Input.h>
#include <engine_base\imgui_impl_glfw_gl3.h>
#include <engine_base\ModelLoader.h>
#include <engine_base\Scene.h>

#include "SelectionSystem.h"

using namespace glm;
using namespace std;

bool hide = true;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void check_input() {
    if (Input::key_pressed_down(GLFW_KEY_H, GLFW_MOD_CONTROL)) {
        hide = !hide;
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, hide ? GLFW_CURSOR_DISABLED: GLFW_CURSOR_NORMAL);
    }
}

void run() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw_exception(runtime_error("glfwInit failed"));
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Intelligent Camera", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw_exception(runtime_error("glfwCreateWindow failed"));
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw_exception(runtime_error("glewInit failed"));

    // GLEW throws some errors, so discard all the errors so far
    while (glGetError() != GL_NO_ERROR) {}

    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);
    //glEnable(GL_BLEND); 
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui_ImplGlfwGL3_Init(window, false);
    Input::init(window);
    ModelLoader::init("assets");
    Scene* scene = new Scene();
    BasicCamera* camera = new BasicCamera(vec3(0, 2, -5));
    BasicCamera* camera2 = new BasicCamera(vec3(0, 1, -5));
    DebugRenderer* renderer = new DebugRenderer(new FlatRenderer(), vec2(1280 - (1280 / 3) - 1, 0), 0.3333f);
    SelectionSystem* selection = new SelectionSystem();
    scene->add_entity(EntityFactory::create_entity("tree", vec3(), vec3(), vec3(5)));
    scene->add_entity(EntityFactory::create_entity("bush", vec3(2, 0, 3), vec3(), vec3(2)));
    scene->add_entity(EntityFactory::create_entity("rock", vec3(0, -1.0, 5), vec3(), vec3(3)));
    scene->add_entity(EntityFactory::create_entity("floor", vec3(), vec3(), vec3(100)));

    glClearColor(100 / 255.f, 149 / 255.f, 237 / 255.f, 1.0f);

    glfwSwapInterval(.5);

    double oldTime = glfwGetTime();

    while (!Input::key_pressed_down(GLFW_KEY_ESCAPE)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        check_input();

        double newTime = glfwGetTime();
        double timeElapsed = newTime - oldTime;

        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Begin("Test");
        ImGui::ImageButton((void*)renderer->texture(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        camera->update(timeElapsed);
        scene->update(timeElapsed);
        
        selection->update(timeElapsed, scene, camera);

        scene->draw(camera);
        renderer->draw(scene, camera2);

#ifdef _DEBUG
        GLSL::check_gl_error("End of loop");
#endif
        oldTime = newTime;

        ImGui::Render();
        glfwSwapBuffers(window);
        Input::clear();
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
}

int main(int argc, char** argv) {
    try {
        run();
    }
    catch (const exception& e) {
        char temp;
        cerr << e.what() << endl;
        cerr << "Press anything to quit..." << endl;
        _getch();
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}