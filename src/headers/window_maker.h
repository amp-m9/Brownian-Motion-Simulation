//
// Created by andri on 08/02/2022.
//

#ifndef BROWNIANMOTION_WINDOW_MAKER_H
#define BROWNIANMOTION_WINDOW_MAKER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "SpriteRenderer.h"
#include "sprite.h"
#include "RigidBody.h"



// GLFW function declarations

enum phase
{
    LOADING,
    READY,
    ACTIVE,
    PAUSED,
    ANIMATING
};
enum state
{
    BrownianBackground,
    BrownianSim,
    ProjectileBackground, 
    ProjectileSim
};
class Window
{
private:
    GLFWwindow *window;
    SpriteRenderer *Renderer;
    unsigned int Width;
    unsigned int Height;
    float vecX=0.0f, vecY=0.0f;
    bool success;
public:
    state _state = BrownianBackground;
    bool trackParticle = false;
    bool firedProjectile = false;
    Window(unsigned int screen_width, unsigned int screen_height, char *title, bool resizeable = false);

    void setKeyCallback(GLFWkeyfun callback);
    void setFrameBufferSizeCallback(GLFWframebuffersizefun callback);
    bool gladInit();
    void clear();
    void display();
    bool isOpen();
    void Draw(Sprite sprite);
    void Draw(RigidBody sprite);
    void DrawLineSegment(glm::vec2 start, glm::vec2 end, float lineWidth = 1, glm::vec4 colour = glm::vec4(1));
    void DrawLine(glm::vec2* path, int pointCount, int linewidth = 1, glm::vec4 colour = glm::vec4(1), int start = 0);
    void Draw(Shape2D *shape, glm::vec2 position, float rotation);
    void Draw(Texture2D texture, glm::vec2 size, glm::vec2 pos, float rotation = 0);

    void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);
    void TopBar();
    void ProjectileContext();
    void BrownianContext();
    void BrownianControls(float *speed);
    glm::vec2 SandBox(int (*fire_projectile)(glm::vec2), void (*reset_projectile)(), phase state, glm::vec2 distance);
};

#endif //BROWNIANMOTION_WINDOW_MAKER_H


