//
// Created by andri on 08/02/2022.
//

#include "sprite.h"

Sprite::Sprite(Texture2D texture, glm::vec2 pos, glm::vec2 size, glm::vec4 color, float rotation, float colormix) :
        Position(pos), Size(size),  Scale(glm::vec2(1)), Color(color), Rotation(rotation),ColorMix(colormix),  Texture(texture)
{}

void Sprite::Move(glm::vec2 movement_vec)
{
    Position+=movement_vec;
}

void Sprite::SetScale(glm::vec2 scale){
    Scale = scale;
}

void Sprite::SetColour(glm::vec4 colour)
{
    this->Color=colour;
}
void Sprite::SetOrigin(glm::vec2 origin)
{
    this->Origin=origin;
}
glm::vec2 Sprite::GetPosition(){return this->Position+this->Origin;}
void Sprite::SetPosition(glm::vec2 position){this->Position = position-this->Origin;}
