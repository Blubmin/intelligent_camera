#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "Globals.h"
#include "ModelLoader.h"
#include "World.h"

using namespace std;

char keys[1024] = { 0 };
char mouse_buttons[8] = { 0 };
char mods = 0;

double mouse_x = -1;
double mouse_x_diff = 0;
double mouse_y = -1;
double mouse_y_diff = 0;
double scroll = 0;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key < 1024)
        keys[key] = action;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    mods = mod;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mod)
{
    mouse_buttons[button] = action;
    mods = mod;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scroll = yoffset;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouse_x != -1 || mouse_y != -1)
    {
        mouse_x_diff = xpos - mouse_x;
        mouse_y_diff = ypos - mouse_y;
    }
    mouse_x = xpos;
    mouse_y = ypos;
}

void run() 
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        throw runtime_error("glfwInit failed");
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Intelligent Camera", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw runtime_error("glfwCreateWindow failed");
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw runtime_error("glewInit failed");

    // GLEW throws some errors, so discard all the errors so far
    while (glGetError() != GL_NO_ERROR) {}

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);
    glClearColor(100 / 255.f, 149 / 255.f, 237 / 255.f, 1.0f);

    shared_ptr<World> world = make_shared<World>();
    world->init();

    double oldTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double newTime = glfwGetTime();
        double timeElapsed = newTime - oldTime;

        world->step(timeElapsed);
        oldTime = newTime;
        scroll = 0;

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
        cin >> temp;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}