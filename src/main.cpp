#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "classes/staticObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include <utility>

#include "stb_image.h"
#include "classes/game.h"
#include "classes/player.h"


#define GLFW_INITIALIZE glfwInit();  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#define GLFW_WINDOW_INITIALIZE  GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "EngineTwo", nullptr, nullptr); if (window == nullptr){ std::cout << "Failed to create GLFW window" << std::endl; glfwTerminate(); return -1;} glfwMakeContextCurrent(window); glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))){ std::cout << "Failed to initialize GLAD" << std::endl; return -1;}


#ifdef __APPLE__
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void checkGLError(const char* location);


int screenWidth = 800;
int screenHeight = 600;
constexpr int tile  = 24;



int main()
{

    GLFW_INITIALIZE;
    stbi_set_flip_vertically_on_load(1);
    GLFW_WINDOW_INITIALIZE;
    glfwSetKeyCallback(window, key_callback);

    game Game(tile,screenWidth,screenHeight);
    glfwSetWindowUserPointer(window,&Game);
    player Character(Game.gameSpace,"../src/shaders/static.vs","../src/shaders/static.fs","../src/textures/player.png",10,10);
    Game.Player = &Character;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Game.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }


    Game.close();
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    screenWidth = width;
    screenHeight = height;

    glViewport(0, 0, width, height);
    game* Game = static_cast<game*>(glfwGetWindowUserPointer(window));
    if (Game) {
        Game->gameSpace.resize(width,height);
        Game->Player->world.resize(width, height);
        for (auto& c: Game->staticGameObjects) {
            c.world.resize(width,height);
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    game* Game = static_cast<game*>(glfwGetWindowUserPointer(window));
    if (Game) {
        Game->update(key,action);
    }
}

void checkGLError(const char* location) {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error at " << location << ": " << err << std::endl;
    }
}