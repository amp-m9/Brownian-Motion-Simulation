//
// Created by andri on 03/12/2021.
//

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "shader.h"
#include "texture.h"
#include "Shape.h"

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)

    SpriteRenderer(Shader sprites);

// Destructor
    ~SpriteRenderer();

    // Renders a defined quad textured with given sprite
    void DrawSprite(
            Texture2D texture,
            glm::vec2 position,
             glm::vec2 size , glm::vec2 scale, glm::vec2 origin,
            float rotate = 0.0f,
            glm::vec4 color = glm::vec4(1.0f),
            float colorMix = 0);

    void DrawShape(Shape2D *shape, glm::vec2 position, glm::vec4 colour, float rotate);

    void DrawLine(glm::vec2 start, glm::vec2 end, int lindWidth = 8, glm::vec4 colour = glm::vec4(1));

private:
    // Render state
    Shader spriteShader;
    GLuint quadVAO, quadVBO;
    GLuint bgVAO, bgVBO;


    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();

};

#endif //SPRITERENDERER_H
