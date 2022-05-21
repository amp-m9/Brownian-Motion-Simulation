#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "headers/Simulation.h"
#include "headers/resource_manager.h"
#include "headers/window_maker.h"
#include "headers/SandBox2D.h"

#include <random>
#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int fireProjectile(glm::vec2 vec);
void resetProjectile();
// The Width of the screen
const unsigned int SCREEN_WIDTH = 1080;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 720;
int points = 50;
char name[] = "particle";
bool pressedFlag = false;
Simulation BrownianMotion(SCREEN_WIDTH, SCREEN_HEIGHT);
SandBox2D ProjMotionSB(SCREEN_WIDTH, SCREEN_HEIGHT);
Window *_window;
enum active
{
    BROWNIAN_ACTIVE,
    SANDBOX_ACTIVE
};
active currentlyPlaying;
int main(int argc, char *argv[])
{
    _window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, name);
    _window->setKeyCallback(key_callback);
    _window->setFrameBufferSizeCallback(framebuffer_size_callback);

    // initialize games
    // ---------------
    BrownianMotion.Init();
    ProjMotionSB.Init();
    //    ProjectileSandbox.Init();
    currentlyPlaying = BROWNIAN_ACTIVE;
    BrownianMotion.State = SIM_ACTIVE;
    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> widthGen(0, SCREEN_WIDTH), heightGen(0, SCREEN_HEIGHT);

    glm::vec2 line[50];
    for (int i = 0; i < 50; i++)
    {
        line[i] = (glm::vec2(widthGen(generator), heightGen(generator)));
    }
    glm::vec2 start = glm::vec2(0, 0), end = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
    while (_window->isOpen())
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

//         ImGui::ShowDemoWindow();

        _window->clear();
        _window->TopBar();
        switch (_window->_state){
            case BrownianBackground:
                _window->BrownianContext();
                break;

            case BrownianSim:
                BrownianMotion.Update(deltaTime);
                _window->BrownianControls(BrownianMotion.getSpeed());
                BrownianMotion.Render(_window);
                break;

            case ProjectileBackground:
                _window->ProjectileContext();
                break;

            case ProjectileSim:
                glm::vec2 motion = _window->SandBox(&fireProjectile, &resetProjectile, ProjMotionSB._phase, ProjMotionSB.displacement);
                ProjMotionSB.Update(deltaTime, motion);
                ProjMotionSB.Render(_window);
                break;
        }
        // update game state
        // ------------------
        // render
        // ------------------
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        _window->display();
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        //        if (action == GLFW_PRESS)
        //            Breakout.Keys[key] = true;
        //        else if (action == GLFW_RELEASE)
        //            Breakout.Keys[key] = false;
    }
    if ((key == GLFW_KEY_BACKSPACE || key == GLFW_KEY_SPACE))
    {
        if (action == GLFW_PRESS and !pressedFlag)
        {
            pressedFlag = true;
            if (_window->_state==ProjectileSim)
            {
                _window->_state = BrownianSim;
            }
            else
            {
                _window->_state = ProjectileSim;
            }
        }
        else if (action == GLFW_RELEASE)
            pressedFlag = false;
    }
    // if (key == GLFW_KEY_F)
    // {
    //     BrownianMotion.logPerf();
    // }
    if (!pressedFlag && action == GLFW_PRESS)
    {
        pressedFlag = true;
        // if (key == GLFW_KEY_DOWN)
        //     ProjMotionSB.editOrigin(0, 1);
        // if (key == GLFW_KEY_UP)
        //     ProjMotionSB.editOrigin(0, -1);
    }
    else if(action == GLFW_RELEASE)
        pressedFlag = false;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int fireProjectile(glm::vec2 vec)
{
    return ProjMotionSB.fire(vec);
}
void resetProjectile()
{
    ProjMotionSB.reset();
}
