#include <conio.h>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <imgui\imgui.h>

#include <engine_base\ExceptionWrapper.h>
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

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

<<<<<<< Updated upstream
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_H && mod & GLFW_MOD_CONTROL && action == GLFW_PRESS) {
        input_active = !input_active;
        glfwSetInputMode(window, GLFW_CURSOR, input_active ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
    }

    if (key == GLFW_KEY_C && mod & GLFW_MOD_CONTROL && action == GLFW_PRESS) {
        view_hull = !view_hull;
    }

    if (key < 1024)
        keys[key] = input_active ? action : -1;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    mods = mod;

    ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mod);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mod)
{
    mouse_buttons[button] = action;
    mods = mod;

    ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mod);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scroll = yoffset;

    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!input_active) return;

    if (mouse_x != -1 || mouse_y != -1)
    {
        mouse_x_diff = xpos - mouse_x;
        mouse_y_diff = ypos - mouse_y;
    }
    mouse_x = xpos;
    mouse_y = ypos;
}

=======
>>>>>>> Stashed changes
void run() 
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        throw_exception(runtime_error("glfwInit failed"));
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Intelligent Camera", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw_exception(runtime_error("glfwCreateWindow failed"));
    }

    Input::init(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw_exception(runtime_error("glewInit failed"));

    // GLEW throws some errors, so discard all the errors so far
    while (glGetError() != GL_NO_ERROR) {}

    ImGui_ImplGlfwGL3_Init(window, false);
    cout << ImGui::GetIO().MouseDrawCursor << endl;
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);
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
        _getch();
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}