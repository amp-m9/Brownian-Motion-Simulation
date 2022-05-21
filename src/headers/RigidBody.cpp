//
// Created by andri on 06/12/2021.
//

#include "RigidBody.h"

#include <utility>
#include "glm/glm.hpp"

RigidBody::RigidBody(Texture2D texture,
                     glm::vec2 pos,
                     glm::vec2 size,
                     glm::vec2 velocity,
                     glm::vec4 color,
                     float mass
)
        :
        Position(pos), Size(size), Velocity(velocity), Color(color), Mass(mass), Texture(texture),
        Collider(Shape2D(CIRCLE, size.x/2))
{
    Collider.setPosition(pos);
    hasTexture = true;
    SetOrigin(glm::vec2(size.x/2, size.y/2));
}

RigidBody::RigidBody(Shape2D shape,
                     glm::vec2 pos,
                     glm::vec2 velocity,
                     glm::vec4 color,
                     float mass
)
        :
        Position(pos), Size(shape.Size), Velocity(velocity), Color(color), Mass(mass), Collider(shape)
{
    Collider.setPosition(pos);
    hasTexture = false;
    SetOrigin(glm::vec2(shape.Size.x/2, shape.Size.y/2));

}


void RigidBody::Move(float dt, unsigned int window_width, unsigned int window_height)
{
        this->Position += this->Velocity * dt;
    // checks if it is outside of window width and height. If it is then 
    // change sign of the respective velocity
    if (this->Position.x <= 0.0f) {
        this->Velocity.x = -this->Velocity.x;
        this->Position.x = 0;
    } else if (this->Position.x + this->GetCollider().Size.x>= window_width) {
        this->Velocity.x = -this->Velocity.x;
        this->Position.x = window_width - (this->GetCollider().Size.x);
    }
    if (this->Position.y<= 0.0f) {
        this->Velocity.y = -this->Velocity.y;
        this->Position.y = 0;
    }
    else if (this->Position.y + this->GetCollider().Size.y>= window_height) {
        this->Velocity.y = -this->Velocity.y;
        this->Position.y = window_height - (this->GetCollider().Size.y);
    }
    this->Collider.setPosition(this->GetPosition());
}

void RigidBody::Move(float dt)
{
    this->Position += this->Velocity * dt;
    this->Collider.setPosition(this->GetPosition());

}

void RigidBody::Move(glm::vec2 vector)
{
    this->Position += vector;
    this->Collider.setPosition(this->Position);
}

void RigidBody::SetCollider(Shape2D collider)
{
    this->Collider = collider;
    this->Collider.setPosition(this->Position+this->Origin);
}

Shape2D RigidBody::GetCollider()
{
    return Collider;
}

void RigidBody::ScaleBody(float scale)
{
    ScaleBody(scale,scale);
}
void RigidBody::ScaleBody(glm::vec2 scale)
{
    ScaleBody(scale.x, scale.y);
}
void RigidBody::ScaleBody(float scale_x, float scale_y)
{
    this->Scale = glm::vec2(this->Scale.x * scale_x, this->Scale.y * scale_y);
    this->Collider.ScaleShape(this->Scale);
}

void RigidBody::SetColour(glm::vec3 colour)
{
    if(hasTexture){
        if (ColorMix==-1)
            ColorMix = .5;
    }
    Color = glm::vec4(colour, 1.0f);
}
void RigidBody::SetColour(glm::vec4 colour)
{
    if(hasTexture){
        if (ColorMix==-1)
            ColorMix = .5;
    }
    Color = colour;
}

void RigidBody::SetColMix(float mix){ this->ColorMix = mix;}
void RigidBody::SetOrigin(glm::vec2 origin){this->Origin = origin;}
glm::vec2 RigidBody::GetPosition(){return this->Position+this->Origin;}
void RigidBody::SetPosition(glm::vec2 position){this->Position = position-this->Origin;this->Collider.setPosition(this->GetPosition());}
