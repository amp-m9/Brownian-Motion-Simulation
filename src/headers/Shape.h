//
// Created by andri on 03/03/2022.
//

#ifndef BROWNIANMOTION_SHAPE_H
#define BROWNIANMOTION_SHAPE_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <tuple>


enum type
{
    CIRCLE, TRIANGLE, RECT, QUAD, PENTAGON, HEXAGON
};
// Represents the four possible (collision) directions
enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

struct Line
{
public:
    std::vector<glm::vec2> Points;
    glm::vec4 Colour = glm::vec4(1);
    glm::vec4 Outline = Colour;

};

struct Shape2D
{
public:
    type Shape;
    glm::vec2 Position = glm::vec2(0), Scale = glm::vec2(1);
    glm::vec4 Colour = glm::vec4(1);
    glm::vec3 Outline = glm::vec3(-1);
    glm::vec2 Size;
    float Width;
    float Height;
    float Radius = 50.f;


    Shape2D(type shape, glm::vec2 size, glm::vec2 position)
            : Shape(shape), Position(position),  Size(size), Width(size.x), Height(size.y)
    {
        if (shape != RECT)
            throw std::invalid_argument("MUST BE RECT TO DEFINE WITH POSITION VEC2 AND SIZE VEC2");


    }

    Shape2D(type shape, float radius)
            : Shape(shape), Size(radius * 2),  Width(radius * 2), Height(radius * 2), Radius(radius)
    {
        if (shape != CIRCLE)
            throw std::invalid_argument("ONLY CIRCLE CAN BE DEFINED WITH FLOAT");
    }

    Shape2D(type shape, glm::vec2 size) : Shape(shape), Position(glm::vec2(0))
    {
        Width = size.x;
        Height = size.y;
        Size = size;
    }


    Collision overLapping(Shape2D *shapeB)
    {
        // Both circles
        if (this->Shape == CIRCLE && CIRCLE == shapeB->Shape) {
            return circleVsCircleOverlap(this->Position, this->Radius, shapeB->Position, shapeB->Radius);
        }
            // shapeB is not circle
        else if (this->Shape == CIRCLE && RECT == shapeB->Shape) {
            return circleVsPolyOverlap(this, shapeB);
        }
            // Current shape is not a circle
        else if (this->Shape == RECT && CIRCLE == shapeB->Shape) {
            return circleVsPolyOverlap(shapeB, this);
        }
        // Final Case: Both not circles

    }

    Collision circleVsCircleOverlap(glm::vec2 posA, float radiusA, glm::vec2 posB, float radiusB)
    {
        posA+=glm::vec2(radiusA);
        posB+=glm::vec2(radiusB);
        glm::vec2 dV = posA - posB;
        float d = sqrt(dV.x * dV.x + dV.y * dV.y);
        return std::make_tuple(d < radiusA + radiusB, NONE, glm::vec2(0));
    }

    Collision circleVsPolyOverlap(Shape2D* circle, Shape2D* rect)
    {
        float radius = (circle->Radius);
        radius *=circle->Scale.x;
        glm::vec2 center(circle->Position.x+radius, circle->Position.y+radius);
        glm::vec2 aabb_half_extents(rect->Size.x / 2.0f, rect->Size.y / 2.0f);
        glm::vec2 aabb_center(rect->Position.x + aabb_half_extents.x, rect->Position.y + aabb_half_extents.y);

        glm::vec2 difference = center - aabb_center; //vec from aabb to circle
        glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
        glm::vec2 closest = aabb_center + clamped;
        difference = closest - center;
//        if (glm::length(difference)<=radiusA)
//            std::cout<<"COLLIDED BABY\n";
        return std::make_tuple(glm::length(difference) <= circle->Radius, VectorDirection(difference), difference);
    }

    bool
    polyVsPolyOverlap(glm::vec2 position1, glm::vec2 size1, glm::vec2 position2, glm::vec2 size2)
    {
        return false;
    }

    void move(glm::vec2 vec)
    {
        Position += vec;
    }

    void setPosition(glm::vec2 pos)
    {
        this->Position = pos;
    }

    void setColour(glm::vec3 colour)
    { Colour = glm::vec4(colour, 1.0f); }

    void setColour(glm::vec4 colour)
    { Colour = colour; }


    void setOutline(glm::vec3 outline)
    { Outline = outline; }

    void ScaleShape(glm::vec2 scale)
    {
        Size.x *= scale.x;
        Size.y *= scale.y;
        if (Shape == CIRCLE) Radius*=scale.x;
    }

    void checkShapeValid(type shape, std::vector<glm::vec2> points)
    {
        if (shape == CIRCLE) {
            if (points.size() != 1)
                throw std::invalid_argument("Shape definition error: circle is defined by float for second argument!");
        }
        if (shape == TRIANGLE && points.size() != 3)
            throw std::invalid_argument("Shape definition error: triangle is defined by exactly 3 points!");

        if (shape == RECT && (points.size() != 2))
            throw std::invalid_argument("Shape definition error: quads are defined by exactly 2 points!");

        if (shape == PENTAGON && points.size() != 5)
            throw std::invalid_argument("Shape definition error: pentagon is defined by exactly 5 points!");
    }

    Direction VectorDirection(glm::vec2 target)
    {
        glm::vec2 compass[] = {
                glm::vec2(0.0f, 1.0f),    // up
                glm::vec2(1.0f, 0.0f),    // right
                glm::vec2(0.0f, -1.0f),    // down
                glm::vec2(-1.0f, 0.0f)    // left
        };
        float max = 0.0f;
        unsigned int best_match = -1;
        for (unsigned int i = 0; i < 4; i++) {
            float dot_product = glm::dot(glm::normalize(target), compass[i]);
            if (dot_product > max) {
                max = dot_product;
                best_match = i;
            }
        }
        return (Direction) best_match;
    }

};


#endif //BROWNIANMOTION_SHAPE_H
