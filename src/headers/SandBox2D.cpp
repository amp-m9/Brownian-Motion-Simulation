//
// Created by andri on 03/03/2022.
//

#include "SandBox2D.h"
#include "resource_manager.h"
#include <thread>
std::random_device rand_device;
std::mt19937 gen(rand_device());

SandBox2D::SandBox2D(unsigned int width, unsigned int height)
    : WIDTH(width), HEIGHT(height)
{
    arrow_rand_x = std::uniform_int_distribution<int>(100, 500);
    target_rand_y = std::uniform_int_distribution<int>(0, height - target_size- 400);
    target_rand_x = std::uniform_int_distribution<int>(400, width - target_size);
}
void SandBox2D::Init()
{
    background = new RigidBody(Shape2D(RECT, glm::vec2(WIDTH, HEIGHT)),
                    glm::vec2(0,0));
    background->SetColour(glm::vec3(0.82, 0.83, 0.90));
    background->SetPosition(glm::vec2(0,0));


    ResourceManager::LoadTexture("./assets/bow.png", true, "bow");

    ResourceManager::LoadTexture("./assets/cannon.png", true, "Canon");
    RigidBody Canon(ResourceManager::GetTexture("Canon"),
                    glm::vec2(-5000),
                    glm::vec2(-96, -57));

    RigidBody Bow2(ResourceManager::GetTexture("bow"),
                   glm::vec2(0),
                   glm::vec2(208 * .2, 960 * .2));
    Bow2.SetOrigin(glm::vec2(84.6, 192));
    Bow2.SetPosition(glm::vec2(430, 490));
    Bow2.Mass = -2;

    ResourceManager::LoadTexture("./assets/arrow.png", true, "arrow");
    RigidBody Arrow(ResourceManager::GetTexture("arrow"),
                    glm::vec2(0),
                    glm::vec2(1428 * .1, 353 * .1));

    Arrow.Mass = -1;
    Arrow.SetOrigin(glm::vec2(255, 30));
    Arrow.SetPosition(glm::vec2(430, 490));

    //    Arrow.Position = glm::vec2(0);

    RigidBody Floor(Shape2D(RECT, glm::vec2(WIDTH, 100)),
                    glm::vec2(0, HEIGHT - 100));
    Floor.Mass = -1;
    Floor.SetColour(glm::vec3(0.439, 0.714, 0.467));

    ResourceManager::LoadTexture("./assets/target.png", true, "target");
    Target = new Sprite(ResourceManager::GetTexture("target"),
                        glm::vec2(WIDTH - target_size, -230),
                        glm::vec2(target_size));
    // ResourceManager::LoadTexture("./assets/pixelApple.png", true, "aplle");
    // Target = new Sprite(ResourceManager::GetTexture("aplle"),
    //                     glm::vec2(WIDTH - target_size, -230),
    //                     glm::vec2(target_size));

    Floor.SetOrigin(glm::vec2(0));
    Arrow.SetCollider(Shape2D(CIRCLE, 253 * .1 * .35));
    Canon.SetOrigin(glm::vec2(0));
    Floor.Mass = -1;

    AddObject(Floor);
    AddObject(Arrow);
    AddObject(Bow2);
    this->Bow = &Objects[2];
    this->Arrow = &Objects[1];
    _phase = READY;
    reset();
}
void SandBox2D::editOrigin(float x, float y)
{
    Bow->SetOrigin(Bow->Origin + glm::vec2(x, y));
    std::cout << Bow->Origin.x << ",  " << Bow->Origin.y << "\n";
}
void SandBox2D::Update(float dt, glm::vec2 inputDir)
{
    tickTime += dt;
    if (tickTime > 1.5)
    {
        //         std::cout << Objects[1].GetPosition().x << ", " << Objects[1].GetPosition().y << " at " << glfwGetTime()<< current_animations <<" Animations\n";
        tickTime = 0;
    }
    glm::vec2 vel = (_phase == ACTIVE) ? Arrow->Velocity : inputDir;
    float theta = vel.x / sqrt(vel.x * vel.x +
                               vel.y * vel.y);
    theta = atan(vel.y / vel.x);
    theta *= 180 / glm::pi<float>();
    if (vel.x < 0)
        theta += 180;
    Arrow->Rotation = theta;

    if (_phase != ACTIVE)
    {
        Bow->Rotation = theta;
        Bow->SetPosition(Arrow->GetPosition());
        return;
    }

    for (unsigned int i = 0; i < Objects.size(); ++i)
    {
        if (Objects[i].Mass > 0)
        {
            Objects[i].Move(dt);
        }
    }

    // detect and resolve
    std::thread t1(&SandBox2D::CheckTargetHit, this, Arrow);
    CollisionUpdate();

    // Decrease velocity
    for (unsigned int i = 0; i < Objects.size(); i++)
    {
        if (Objects[i].Mass > 0)
        {
            Objects[i].Velocity.y += Gravity * dt*100;
        }
    }
    t1.join();
    //    Objects[1].Velocity.y += Gravity * dt;
}

void SandBox2D::CollisionUpdate()
{
    Collision col;
    for (unsigned int i = 0; i < Objects.size() - 1; i++)
    {
        for (unsigned int j = i + 1; j < Objects.size(); j++)
        {
            col = Objects[i].Collider.overLapping(&Objects[j].Collider);
            if (std::get<0>(col) && Objects[i].Mass != -2 && Objects[j].Mass != -2)
                CollisionResolution(&Objects[i], &Objects[j], std::get<1>(col), std::get<2>(col));
        }
    }
}

void SandBox2D::Render(Window *window)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    window->Draw(*background);
    for (auto obj : Objects)
    {
        window->Draw(obj);
    }
    window->Draw(*Target);

    window->Draw(*Bow);

    glm::vec2 arrow_center = Arrow->GetPosition();
    // glm::vec2 velocity = glm::normalize(Objects[1].Velocity);
    // velocity *= 253 * .1 * .5;
    // window.DrawLineSegment(arrow_center, arrow_center + velocity, 253 * .1 * .5, glm::vec4(0, 0, 1, 1));

    if (_phase != ACTIVE)
    {
        // Draw x distance
        window->DrawLineSegment(arrow_center, glm::vec2(Target->Position.x + Target->Size.x / 2, arrow_center.y), 2, glm::vec4(.8f, 0, .8f, 1));
        // Draw y distance
        window->DrawLineSegment(glm::vec2(Target->Position.x + Target->Size.x / 2, arrow_center.y), Target->Position + glm::vec2(Target->Size.x / 2), 2, glm::vec4(0, 0, 1, 1));
        // Draw difference in origins(debugging)
//         window->DrawLineSegment(Bow->GetPosition() - Bow->Origin, Bow->GetPosition(), 1.9f, glm::vec4(0, 0, 1, 1));
    }
// // Uncomment to view hitboxes for debugging
//     Shape2D hitBox(RECT, Objects[1].GetCollider().Size);
//     hitBox.setPosition(Objects[1].GetCollider().Position);
//     RigidBody boxy(hitBox, Objects[1].GetPosition() - glm::vec2(Objects[1].GetCollider().Radius));
//     boxy.SetColour(glm::vec4(1, 0, 0, .5));
//     boxy.SetOrigin(glm::vec2(0));
//     window->Draw(boxy);
//     boxy.SetColour(glm::vec4(0, 0, 1, .5));
//     boxy.SetOrigin(glm::vec2(boxy.Size.x / 2));
//     boxy.SetPosition(Bow->GetPosition() - Bow->Origin);
//     window->Draw(boxy);
//     // window.RenderText("TEST", glm::vec2(WIDTH/2, HEIGHT/2), 1, glm::vec3(0));
}

bool SandBox2D::AreColliding(Shape2D *objA, Shape2D *objB)
{
    return std::get<0>(objA->overLapping(objB));
}

void SandBox2D::AddObject(RigidBody obj)
{
    obj.index = Objects.size();
    Objects.emplace_back(obj);
}

void SandBox2D::RemoveObject(RigidBody obj)
{
    Objects.erase(Objects.begin() + obj.index);
}

void SandBox2D::CollisionResolution(RigidBody *objA, RigidBody *objB, Direction dir, glm::vec2 diff)
{
    if (objA->Collider.Shape == CIRCLE && objB->Collider.Shape == CIRCLE)
    {
        CircleVsCircleResolution(objA, objB);
        std::cout << "CIRCLE V CIRCLE\n";
    }
    else if (objA->Collider.Shape == CIRCLE && objB->Collider.Shape == RECT)
    {
        CircleVsRectResolution(objA, objB, dir, diff);
        std::cout << "CIRCLE V RECT\n";
    }
    else if (objA->Collider.Shape == RECT && objB->Collider.Shape == CIRCLE)
    {
        CircleVsRectResolution(objB, objA, dir, diff);
    }
}

bool SandBox2D::MovingTowards(RigidBody *objA, RigidBody *objB)
{
    return glm::dot(objA->GetPosition() - objB->GetPosition(), objB->Velocity) > 0 or
           glm::dot(objB->GetPosition() - objA->GetPosition(), objA->Velocity) > 0;
}

void SandBox2D::CircleVsCircleResolution(RigidBody *objA, RigidBody *objB)
{
    if (!MovingTowards(objA, objB))
        return;
    glm::vec2 tangentVector;
    tangentVector.y = -(objA->GetPosition().x - objB->GetPosition().x);
    tangentVector.x = (objA->GetPosition().y - objB->GetPosition().y);
    tangentVector = glm::normalize(tangentVector);

    glm::vec2 relativeVelocity;
    relativeVelocity = glm::vec2(objA->Velocity.x - objB->Velocity.x,
                                 objA->Velocity.y - objB->Velocity.y);
    float length = glm::dot(relativeVelocity, tangentVector);

    glm::vec2 velocityComponentOnTangent;
    velocityComponentOnTangent = tangentVector * length;

    glm::vec2 velocityComponentPerpToTan;
    velocityComponentPerpToTan = relativeVelocity - velocityComponentOnTangent;
    if (objA->Mass >= 0 && objB->Mass >= 0)
    {
        objA->Velocity -= velocityComponentPerpToTan;
        objB->Velocity += velocityComponentPerpToTan;
    }
    else
    {
        if (objA->Mass < 0)
        {
            glm::vec2 objBVel = objB->Velocity;
            objB->Velocity += velocityComponentPerpToTan;
            objB->Velocity = glm::normalize(objB->Velocity);
            objB->Velocity = objB->Velocity * glm::length(objBVel);
        }
        if (objB->Mass < 0)
        {
            glm::vec2 objAVel = objA->Velocity;
            objA->Velocity += velocityComponentPerpToTan;
            objA->Velocity = glm::normalize(objA->Velocity);
            objA->Velocity = objA->Velocity * glm::length(objAVel);
        }
    }
}

void SandBox2D::CircleVsRectResolution(RigidBody *circleBody, RigidBody *rectBody, Direction dir, glm::vec2 diff)
{
    if (dir == LEFT || dir == RIGHT)
    {
        rectBody->Velocity.x *= -1;

        float penetration = circleBody->Collider.Radius * circleBody->Scale.x - std::abs(diff.x);
        if (dir == LEFT)
        {
            if (circleBody->Velocity.x > 0)
                circleBody->Velocity.x *= -1;
            circleBody->SetPosition(circleBody->GetPosition() + glm::vec2(penetration, 0));
        }
        else
        {
            if (circleBody->Velocity.x < 0)
                circleBody->Velocity.x *= -1;
            glm::vec2 newPosition = circleBody->GetPosition() - glm::vec2(penetration, 0);
            circleBody->SetPosition(newPosition);
        }
    }
    else
    {
        //        circleBody->Velocity.y*=-1;

        //         float penetration = circleBody->Collider.Radius * circleBody->Collider.Scale.y - std::abs(diff.y);
        if (dir == UP)
        {
            if (circleBody->Velocity.y > 0)
                circleBody->Velocity.y *= -1.0f;
            //            circleBody->GetPosition().y -= penetration;
        }
        else
        {
            if (circleBody->Velocity.y < 0)
                circleBody->Velocity.y *= -1.0f;
            //            circleBody->GetPosition().y += penetration;
        }
    }
}

int SandBox2D::fire(glm::vec2 vec)
{
    if (_phase != READY)
        return -1;

    Objects[1].Velocity = glm::vec2(vec.x*100, vec.y*100);
    Objects[1].Mass = 30;
    _phase = ACTIVE;
    return 1;
}

void SandBox2D::reset()
{
    static int counter = 0;
    _phase = ANIMATING;
    counter++;
    anim_start = glfwGetTime();
    glm::vec2 target_end = glm::vec2(target_rand_x(gen), target_rand_y(gen));
    glm::vec2 arrow_end = glm::vec2(230, arrow_rand_x(gen));
    // TODO:: START TWEEN FUNCTION THREAD FOR ALL OBJECTS POSITIONS.
    std::thread ballTween(&SandBox2D::TweenPos, this,
                          &Objects[1], Objects[1].GetPosition(), arrow_end,
                          Target->Position, target_end);
    ballTween.detach();
    Target->SetColour(glm::vec4(1.0f));
    displacement = target_end - arrow_end;
    Objects[1].Velocity = glm::vec2(0);
    Objects[1].Mass = -1;

    std::cout << counter << ":" << target_end.x << ", " << target_end.y << "\n";
}

float easeInOutQuart(float x) // interpolate x based on progress
{
    return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

void SandBox2D::TweenPos(RigidBody *body, glm::vec2 start, glm::vec2 end, glm::vec2 start2, glm::vec2 end2)
{
    float time_since_start = 0.0f;
    float t = 0.0f;
    glm::vec2 arrow_displacement(end.x - start.x, end.y - start.y);
    glm::vec2 target_displacement(end2.x - start2.x, end2.y - start2.y);
    float frac = 0.0f; // Total completion
    current_animations++;
    while (t <= 1)
    {
        time_since_start = glfwGetTime() - anim_start;
        t = time_since_start / animation_duration; // <------progress
        frac = easeInOutQuart(t); // get fraction of displacement that should be covered.
        // displace
        body->SetPosition(glm::vec2(start.x + frac * arrow_displacement.x,
                                    start.y + frac * arrow_displacement.y));
        Bow->SetPosition(body->GetPosition());
        Target->Position = glm::vec2(start2.x + frac * target_displacement.x,
                                     start2.y + frac * target_displacement.y);
    }
    // aniamtion now finished
    current_animations--;
    if (current_animations <= 0)
    {
        // set new value for displacement to be used in UI
        displacement = (Target->Position + glm::vec2(Target->Size.x / 2)) - body->GetPosition();
        _phase = READY;
    }
}
void SandBox2D::TweenAlpha(Sprite *sprite, float start, float end)
{
    float time_since_start = 0.0f;
    float t = 0.0f;
    float frac = 0.0f;
    current_animations++;
    float difference = end - start;
    while (t <= 1)
    {
        time_since_start = glfwGetTime() - anim_start;
        t = time_since_start / animation_duration + .5;
        frac = easeInOutQuart(t);
        glm::vec4 prevColour = sprite->Color;
        prevColour.w = start + t * difference;
        sprite->Color = (prevColour);
    }
    current_animations--;
    if (current_animations <= 0)
    {
        _phase = READY;
    }
}
void SandBox2D::CheckTargetHit(RigidBody *body)
{
    Shape2D SpritePosition(CIRCLE, Target->Size.x * Target->Scale.x*.5f);
    SpritePosition.setPosition(Target->Position);
    Shape2D arrow_head = body->GetCollider();
    arrow_head.setPosition(arrow_head.Position - glm::vec2(arrow_head.Radius));
    if (std::get<0>(SpritePosition.overLapping(&arrow_head)))
    {
        //          TweenAlpha(Target, 1.0f, 0.0f);
        Target->SetColour(glm::vec4(0, 1, 0, 1));
    }
}
