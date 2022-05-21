//
// Created by andri on 03/12/2021.
//

#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader sprites)
{
    this->spriteShader = sprites;
    this->initRenderData();
}



SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}


void SpriteRenderer::DrawSprite(
        Texture2D texture,
        glm::vec2 position,
        glm::vec2 size, glm::vec2 scale, glm::vec2 origin,
        float rotate,
        glm::vec4 color,
        float colorMix
)
{
    // glEnableVertexAttribArray(0);
    // prepare transformations
    this->spriteShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position,
                                            0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    // model = glm::translate(model,
    //                        glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad

    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate

    // model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
    model = glm::translate(model, glm::vec3(-0.5f * origin.x, -0.5f * origin.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));// last scale

    this->spriteShader.SetMat4("model", model);
    this->spriteShader.SetVec4f("spriteColor", color);
    this->spriteShader.SetBool("rect", false);
    this->spriteShader.SetFloat("colorMix", colorMix);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // glDisableVertexAttribArray(0);
}

void SpriteRenderer::DrawShape(Shape2D *shape, glm::vec2 position,glm::vec4 colour, float rotate)
{
    if (shape->Shape == RECT) {
        // glEnableVertexAttribArray(quadVAO-1);
        this->spriteShader.Use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale
        // happens first, then rotation, and then final
        // translation happens; reversed order)

        model = glm::scale(model, glm::vec3(shape->Width, shape->Height, 1.0f));

        this->spriteShader.SetMat4("model", model);
        this->spriteShader.SetVec4f("colour", colour);
        this->spriteShader.SetBool("rect", true);
        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // glDisableVertexAttribArray(0);
    }
}


void SpriteRenderer::DrawLine(glm::vec2 start, glm::vec2 end, int width, glm::vec4 colour)
{
    if(end.x<start.x){
        glm::vec2 swap = start;
        start = end;
        end = swap;
    }
    float length = 0.0f;
    glm::vec2 lineSegment;
    // glEnableVertexAttribArray(quadVAO-1);
    lineSegment = end-start;
    length = glm::length(lineSegment);
    float theta = lineSegment.x / sqrt(lineSegment.x * lineSegment.x +
                                       lineSegment.y * lineSegment.y);

    theta = atan(lineSegment.y/lineSegment.x);
    theta *= 180/glm::pi<float>();
    // Draw the line
    this->spriteShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(start.x, start.y - width/2, 0.0f));  // first translate (transformations are: scale
    // happens first, then rotation, and then final
    // translation happens; reversed order)
    model = glm::translate(model,
                           glm::vec3(0, 0.5f * width, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(theta), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(0, -0.5f * width, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(length, width, 1.0f));


    this->spriteShader.SetMat4("model", model);
    this->spriteShader.SetVec4f("colour", colour);
    this->spriteShader.SetBool("rect", true);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // glDisableVertexAttribArray(0);


}


void SpriteRenderer::initRenderData()
{

    float vertices[] = {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    // configure VAO/VBO
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);

    // For Sprites
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO - 1);
    glBindVertexArray(quadVAO - 1);
    // glDisableVertexAttribArray(0);

}
