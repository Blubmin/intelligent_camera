#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glog/logging.h>

#include <imgui\imgui.h>

#include <engine_base\imgui_impl_glfw_gl3.h>
#include <engine_base\Input.h>
#include <engine_base\ModelLoader.h>

#include "Globals.h"
#include "World.h"

using namespace std;

int cur_frame = 0;
int total_frames = 1000;

bool input_active = true;
bool view_hull = false;
bool calc_hull = true;
int hull_counter = 0;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


void run() 
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        LOG(ERROR) << "glfwInit failed" << std::endl;
    }

    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Intelligent Camera", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        LOG(ERROR) << "glfwCreateWindow failed" << std::endl;
    }

    Input::init(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        LOG(ERROR) << "glewInit failed" << std::endl;

    // GLEW throws some errors, so discard all the errors so far
    while (glGetError() != GL_NO_ERROR) {}

    ImGui_ImplGlfwGL3_Init(window, false);
    cout << ImGui::GetIO().MouseDrawCursor << endl;
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);
    //glEnable(GL_BLEND); 
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(100 / 255.f, 149 / 255.f, 237 / 255.f, 1.0f);
    
    glfwSwapInterval(.5);

    shared_ptr<World> world = make_shared<World>();
    world->init();

    double oldTime = glfwGetTime();

    while (!Input::key_pressed_down(GLFW_KEY_ESCAPE))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        double newTime = glfwGetTime();
        double timeElapsed = newTime - oldTime;

        world->step(timeElapsed);
        oldTime = newTime;

        Input::clear();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(int argc, char** argv)
{
    try
    {
        run();
    }
    catch (const exception& e)
    {
        char temp;
        cerr << e.what() << endl;
        cerr << "Press anything to quit..." << endl;
        getchar();
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}