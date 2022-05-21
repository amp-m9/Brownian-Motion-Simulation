
// Created by andri on 07/12/2021.
//

#include <random>
#include "Simulation.h"
#include "resource_manager.h"
#include <math.h>       /* sqrt */
#include <thread>
#include <future>
std::random_device rand_dev, rand2;
std::mt19937 generator(rand_dev()), gen2(rand2());
std::uniform_int_distribution<int> distr(10, 300), randColor(0, 20);
const glm::vec2 INITIAL_VELOCITY(0.0f, -90.0f);

const float PARTICLE_RADIUS = 10.0f;
const float SPACING = 5.0f;

float timeElapsed = 0.0f;
glm::vec2 *pathOfParticle;
RigidBody *trackedParticle;
int pathStart = 0, nextFree = 0, pointCount = 20;
RigidBody *Particle;

Simulation::Simulation(unsigned int width, unsigned int height)
        : State(SIM_ACTIVE), Width(width), Height(height)
{}

float * Simulation::getSpeed(){
    return &speed;
}
void Simulation::Init()
{
    // create the particles!
    int max_cols = floor(Width / (PARTICLE_RADIUS * SPACING));
    int max_rows = floor(Height / (PARTICLE_RADIUS * SPACING));
    glm::vec2 radVec(0, PARTICLE_RADIUS);
    Shape2D particleCircle(CIRCLE, PARTICLE_RADIUS);
    ResourceManager::LoadTexture("./assets/250sphere.png", true, "particle");
    for (int i = 0; i < max_rows; ++i) {
        for (int j = 0; j < max_cols; ++j) {

            Particles.emplace_back(
                    new RigidBody(ResourceManager::GetTexture("particle"),
                                  glm::vec2(PARTICLE_RADIUS * 3 / 2 + (PARTICLE_RADIUS * 3) * i,
                                            PARTICLE_RADIUS * 3 / 2 + (PARTICLE_RADIUS * 3) * i),
                                  glm::vec2(PARTICLE_RADIUS * 2),
                                  glm::vec2(distr(generator), distr(generator)),
                                  //glm::vec3((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,
                                  //          (float) rand() / RAND_MAX)
                                  glm::vec4(0.6, 0.8 + j/(max_cols*1.8), 1, 1)

                    )
            );
        }
    }
    // set all colliders and origins
    for (int i = 0; i < max_rows * max_cols; i++) {
        Particles[i]->SetCollider( Shape2D(CIRCLE, PARTICLE_RADIUS));
        Particles[i]->Origin = (glm::vec2(PARTICLE_RADIUS));
    }

    // initialise tracking path
    pathOfParticle = (glm::vec2 *) malloc(sizeof(glm::vec2) * pointCount);
    for (int i = 0; i < pointCount; i++) {
        pathOfParticle[i] = glm::vec2(-1);
    }
    trackedParticle = Particles[max_rows * max_cols -1];
    pathOfParticle[nextFree] = trackedParticle->GetPosition();
    nextFree++;
}

void Simulation::Update(float dt)
{
    timeElapsed += dt;
    dt = dt*speed;
    std::thread t1(&Simulation::TrackParticle, this, timeElapsed);
    CollisionUpdate(dt);
    for (RigidBody *part: Particles) {
        part->Move(dt, this->Width, this->Height);
    }
    t1.join();
    if(timeElapsed>0.1f)
        timeElapsed=0;
}

void Simulation::Render(Window *window)
{
    std::vector<glm::vec2> destination;
    std::promise<std::vector<glm::vec2>> p;
    auto f = p.get_future();
//     std::thread t1(&Simulation::ConvertArrToVec, this, &destination);
    std::thread t(&Simulation::ConvertArrToVec, this, std::move(p));
    glClearColor(.5, .5, .5, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    for (RigidBody *particle: Particles) {
        window->Draw(*particle);
    }
//     t1.join();
    t.join();
    destination = f.get();
//      window->DrawLine(destination, 2, glm::vec3(1,0,0));
    if (window->trackParticle){
        trackedParticle->SetColour(glm::vec3(1,0,0));
        window->DrawLine(pathOfParticle, pointCount, 2, glm::vec4(1, 0, 0, 1), pathStart);
    }else{    trackedParticle->SetColour(glm::vec4(0.6, 0.988, 1, 1));
}
}

bool AreColliding(RigidBody *objA, RigidBody *objB)
{
    return std::get<0>(objA->Collider.overLapping(&objB->Collider));
}

void Simulation::TrackParticle(float time)
{
//     std::cout<< pathOfParticle[nextFree].x<<", "<< pathOfParticle[nextFree].y <<"\n" ;
    if(time>=.1f) {
        pathOfParticle[nextFree] = trackedParticle->GetPosition() + glm::vec2( trackedParticle->Collider.Radius);
        if (nextFree == pathStart) {
            pathStart++;
            pathStart %= pointCount;
        }
        nextFree++;
        nextFree %= pointCount;
        // Print Circular Queue.
        //-----------------------------------------------------
//          std::cout << '[';
//          for (int i = 0; i < pointCount; i++) {
//              if (pathOfParticle[(i) % pointCount] != glm::vec2(-1))
//                  std::cout << '(' << pathOfParticle[i].x << ", "
//                            << pathOfParticle[i].y << "), ";
//              else break;
//          }
// std::cout << "]\n";
    }
}
void Simulation::ConvertArrToVec(std::promise<std::vector<glm::vec2>> && p){
    std::vector<glm::vec2> destination;
    for(int i = pathStart; i< pathStart+pointCount; i++){
        if(pathOfParticle[i % pointCount] != glm::vec2(-1))
            destination.push_back(pathOfParticle[i % pointCount]);
        else break;
    }
    p.set_value(destination);
}
void Simulation::CollisionUpdate(float dt) // BruteForce!!
{
    std::vector<std::pair<RigidBody *, RigidBody *>> collisionsConfirmed;
    for (unsigned int i = 0; i < Particles.size(); i++) {
        for (unsigned int j = i + 1; j < Particles.size(); j++) {
            if (std::get<0>(Particles[i]->Collider.overLapping(&Particles[j]->Collider))
                && movingTowardsEachother(Particles[i], Particles[j]))
            {
                collisionsConfirmed.emplace_back(Particles[i], Particles[j]);
            }
        }
    }
    for (auto &i: collisionsConfirmed) {
        CollisionResolution(i.first, i.second);
    }

}

Simulation::~Simulation()
= default;

void Simulation::CollisionResolution(RigidBody *pCircle, RigidBody *pCircle1)
{
    glm::vec2 tangentVector;
    tangentVector.y = -(pCircle1->GetPosition().x - pCircle->GetPosition().x);
    tangentVector.x = (pCircle1->GetPosition().y - pCircle->GetPosition().y);
    tangentVector = glm::normalize(tangentVector);

    glm::vec2 relativeVelocity;
    relativeVelocity = glm::vec2(pCircle1->Velocity.x - pCircle->Velocity.x,
                                 pCircle1->Velocity.y - pCircle->Velocity.y);
    float length = glm::dot(relativeVelocity, tangentVector);

    glm::vec2 velocityComponentOnTangent;
    velocityComponentOnTangent = tangentVector * length;

    glm::vec2 velocityComponentPerpToTan;
    velocityComponentPerpToTan = relativeVelocity - velocityComponentOnTangent;
    pCircle1->Velocity -= velocityComponentPerpToTan;
    pCircle->Velocity += velocityComponentPerpToTan;
}

bool Simulation::movingTowardsEachother(RigidBody *objA, RigidBody *objB)
{
    return glm::dot(objA->GetPosition() - objB->GetPosition(), objB->Velocity) > 0 or
           glm::dot(objB->GetPosition() - objA->GetPosition(), objA->Velocity) > 0;
}
