#include <conio.h>
#include <iostream>
#include <map>
#include <string.h>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <imgui\imgui.h>

#include <tinyfiledialogs\tinyfiledialogs.h>

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
#include <engine_base\StaticCamera.h>
#include <engine_base\TextureRenderer.h>
#include <engine_base\Utility.h>

#include "BlenderCamera.h"
#include "IndexRenderer.h"
#include "Selected.h"
#include "SelectionRenderer.h"
#include "SelectionSystem.h"

using namespace glm;
using namespace std;

bool hide = true;

int width = 1280;
int height = 720;
int dim = 100;

map<std::string, TextureRenderer*> models;

char const * filterPatterns[1] = { "*.obj" };

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void check_input() {
    if (Input::key_pressed(GLFW_KEY_H, GLFW_MOD_CONTROL)) {
        hide = !hide;
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, hide ? GLFW_CURSOR_DISABLED: GLFW_CURSOR_NORMAL);
    }
}

Entity* create_entity(const string& key, const vec3& pos, const vec3& rot, const vec3& scale) {
    Entity* entity = EntityFactory::create_entity(key, pos, rot, scale);
    entity->addComponent(new Selected());
    return entity;
}

void render_model(const string& key) {
    float color[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
    glClearColor(.8, .8, .8, 1);

    TextureRenderer* tex = new TextureRenderer(new FlatRenderer(), dim, dim);
    Scene scene = Scene();
    StaticCamera cam = StaticCamera(vec3(-1, 1, -1), vec3(0, .5, 0), 1.f);
    scene.add_entity(EntityFactory::create_entity(key));
    tex->draw(&scene, &cam);

    models[key] = tex;
    glClearColor(color[0], color[1], color[2], color[3]);
}

void run() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw_exception(runtime_error("glfwInit failed"));
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(width, height, "Level Builder", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw_exception(runtime_error("glfwCreateWindow failed"));
    }

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

    //glDisable(GL_BLEND);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui_ImplGlfwGL3_Init(window, false);
    Input::init(window);
    ModelLoader::init("assets");
    Scene* scene = new Scene();
    //BasicCamera* camera = new BasicCamera(vec3(0, 2, -5));
    BlenderCamera* camera = new BlenderCamera();
    BasicCamera* camera2 = new BasicCamera(vec3(0, 1, -5));
    //DebugRenderer* renderer = new DebugRenderer(new FlatRenderer(), vec2(1280 - (1280 / 3) - 1, 0), 0.3333f);
    TextureRenderer* texture = new TextureRenderer(new FlatRenderer(), width, height);
    
    SelectionSystem* selection = new SelectionSystem();
    scene->set_renderer(new SelectionRenderer());
    //scene->add_entity(create_entity("tree", vec3(), vec3(), vec3(5)));
    //scene->add_entity(create_entity("bush", vec3(2, 0, 3), vec3(), vec3(2)));
    //scene->add_entity(create_entity("rock", vec3(0, -1.0, 5), vec3(), vec3(3)));
    //scene->add_entity(create_entity("floor", vec3(), vec3(), vec3(100)));

    GLSL::check_gl_error("Render_Once");
    DebugRenderer* renderer = new DebugRenderer(texture->color(), vec2(width - (width / 3) - 1, 0), 0.3333f);

    glClearColor(100 / 255.f, 149 / 255.f, 237 / 255.f, 1.0f);

    glfwSwapInterval(.5);

    double oldTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        if (Input::key_pressed_down(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        check_input();

        double newTime = glfwGetTime();
        double timeElapsed = newTime - oldTime;

        ImGui_ImplGlfwGL3_NewFrame();

        if (selection->selected()) {        
            ImGui::Begin("Selected Object");
            ImGui::DragFloat3("Pos", value_ptr(*selection->pos()), 0.1);
            ImGui::DragFloat3("Rot", value_ptr(*selection->rot()), 0.1);
            ImGui::DragFloat3("Scale", value_ptr(*selection->scale()), 0.1);
            ImGui::End();
        }

        ImGui::Begin("Assets");
        ImGui::PushItemWidth(-1);
        ImGui::ListBoxHeader("##", ModelLoader::keys().size(), 30);
        for (auto k : ModelLoader::keys()) {
            if (ImGui::ImageButton((void*)(models[k]->color()), ImVec2(dim, dim), ImVec2(0, 1), ImVec2(1, 0), -1)) {
                Entity* e = create_entity(k.c_str(), vec3(), vec3(), vec3(1));
                scene->add_entity(e);
                selection->select(e);
            }
            ImGui::SameLine();
            ImGui::TextWrapped(k.c_str());
        }
        ImGui::ListBoxFooter();
        ImGui::PushItemWidth(-1);
        if (ImGui::Button("Import models")) {
            const char* file = tinyfd_openFileDialog("Select a file", "", array_len(filterPatterns), filterPatterns, NULL, true);
            if (file != NULL) {          
                vector<string> files = split(file, '|');
                for (auto f : files) {
                    vector<string> path = split(string(f), '\\');
                    ModelLoader::loadModelByName(f, path.back());
                    render_model(path.back());
                }            
            }
        }
        if (ImGui::Button("Save to file")) {
            const char* file = tinyfd_saveFileDialog("Save as", "untitled.scene", array_len(Scene::FilterPatterns), Scene::FilterPatterns, NULL);
            if (file != NULL) {
                scene->save(file);
            }
        }
        if (ImGui::Button("Import from file")) {
            const char* file = tinyfd_openFileDialog("Import scene", "", array_len(Scene::FilterPatterns), Scene::FilterPatterns, NULL, false);
            if (file != NULL) {
                scene->import(file);
                models.clear();
                for (const auto& k : ModelLoader::keys()) {
                    render_model(k);
                }
                for (auto e : scene->entities()) {
                    e->addComponent(new Selected());
                }
            }
        }
        ImGui::End();

        camera->update(timeElapsed);
        scene->update(timeElapsed);
        
        selection->update(timeElapsed, scene, camera);

        scene->draw(camera);
        texture->draw(scene, camera);
        renderer->draw(scene, camera);

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