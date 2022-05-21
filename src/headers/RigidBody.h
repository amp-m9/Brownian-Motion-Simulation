//
// Created by andri on 06/12/2021.
//

#ifndef BROWNIANMOTION_RIGIDBODY_H
#define BROWNIANMOTION_RIGIDBODY_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "SpriteRenderer.h"
#include "Shape.h"



class RigidBody
{
private:
    glm::vec2 Position;
public:

    explicit RigidBody(Shape2D shape, glm::vec2 pos = glm::vec2(0.0f, 0.0f),
              glm::vec2 velocity = glm::vec2(0.0f, 0.0f),
              glm::vec4 color = glm::vec4(1.0f),
              float mass = 1.0f);

// state
    glm::vec2  Size, Velocity, Origin = glm::vec2(0), Scale = glm::vec2(1);
    glm::vec4 Color;
    float Rotation = 0.0f, ColorMix = -1, Mass;
    Texture2D Texture;
    Shape2D Collider;
    bool hasTexture = false;
    int index = -1;

    // Constructors
    explicit RigidBody(Texture2D texture,
                       glm::vec2 pos = glm::vec2(0.0f, 0.0f),
                       glm::vec2 size = glm::vec2(1.0f, 1.0f),
                       glm::vec2 velocity = glm::vec2(0.0f, 0.0f),
                       glm::vec4 color = glm::vec4(1.0f),
                       float mass = 1.0f
    );


    void Move(float dt, unsigned int window_width, unsigned int window_height);
    void Move(float dt);
    void Move(glm::vec2 vector);
    glm::vec2 GetPosition();
    void SetPosition(glm::vec2 position);
    void SetCollider(Shape2D collider);
    void ScaleBody(float scale_X, float scale_y);
    void ScaleBody(float scale_X_and_Y);
    void ScaleBody(glm::vec2 scale);
    void SetColour(glm::vec3 colour);
    void SetColour(glm::vec4 colour);
    void SetOrigin(glm::vec2 origin);
    Shape2D GetCollider();

    void SetColMix(float mix);
};


#endif //BROWNIANMOTION_RIGIDBODY_H
