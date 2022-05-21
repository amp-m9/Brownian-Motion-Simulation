//
// Created by andri on 03/03/2022.
//

#ifndef BROWNIANMOTION_SANDBOX2D_H
#define BROWNIANMOTION_SANDBOX2D_H
#include "RigidBody.h"
#include "vector"
#include "window_maker.h"
#include "Shape.h"
#include <random>


class SandBox2D
{
public:
    float Gravity = 9.81f;
    float tickTime = .0f;
    bool fired = false;
    bool hitTarget = false;
    phase _phase = LOADING;
    float time_after_bounce = 0.0f;
    float animation_duration = .75f;
    float anim_start = 0.0f;
    int Bounces = 0;
    int current_animations = 0;
    glm::vec2 cannonOldPos, cannonNewPos;
    glm::vec2 projectileOldPos, ProjectileNewPos;
    glm::vec2 displacement = glm::vec2(0);

    float target_size = 250 * .2f;
    std::random_device rand_device;
    std::mt19937 gen;
    std::uniform_int_distribution<int> arrow_rand_x, target_rand_x, target_rand_y;


    std::vector<RigidBody> Objects;
    unsigned int WIDTH, HEIGHT;
    RigidBody *background;
    Sprite *Target;
    RigidBody *Bow;
    RigidBody *Arrow;
    SandBox2D(unsigned int width, unsigned int height);
    void Init();

    void Update(float dt, glm::vec2 movement);
    void Render(Window *window);
    void CollisionUpdate();

    void CollisionResolution(RigidBody *objA, RigidBody *objB, Direction dir, glm::vec2 diff);
    bool MovingTowards(RigidBody *objA, RigidBody *objB);

    void RemoveObject(RigidBody obj);
    void AddObject(RigidBody obj);
    bool AreColliding(Shape2D *objA, Shape2D *objB);
    void CircleVsCircleResolution(RigidBody *objA, RigidBody *objB);
    int fire(glm::vec2 vec);
    void reset();
    void TweenPos(RigidBody *body, glm::vec2 start, glm::vec2 end, glm::vec2 start2, glm::vec2 end2);
    void TweenAlpha(Sprite *sprite, float start, float end);
    void CircleVsRectResolution(RigidBody *circleBody, RigidBody *rectBody, Direction dir, glm::vec2 diff);
    void CheckTargetHit(RigidBody *body);
    void editOrigin(float x, float y);
};

#endif // BROWNIANMOTION_SANDBOX2D_H
