//
// Created by andri on 08/02/2022.
//

#ifndef BROWNIANMOTION_SPRITE_H
#define BROWNIANMOTION_SPRITE_H

#include "sprite.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "SpriteRenderer.h"

class Sprite
{
public:
    // state
    glm::vec2 Position, Size;
    glm::vec2 Scale = glm::vec2(1);
    glm::vec2 Origin = glm::vec2(0);
    glm::vec4 Color;
    float Rotation, ColorMix;
    Texture2D Texture;

    // Constructors
    Sprite(Texture2D texture,
           glm::vec2 pos = glm::vec2(0.0f, 0.0f),
           glm::vec2 size = glm::vec2(1.0f, 1.0f),
           glm::vec4 color = glm::vec4(1.0f),
           float rotation = 0.f,
           float colormix = 0.f);

    void Move(glm::vec2 movement_vec);
    void SetScale(glm::vec2 scale);
    void SetPosition(glm::vec2 position);
    glm::vec2 GetPosition();
    void SetColour(glm::vec4 colour);
    void SetOrigin(glm::vec2 origin);

};

#endif //BROWNIANMOTION_SPRITE_H
