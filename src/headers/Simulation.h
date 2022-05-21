//
// Created by andri on 07/12/2021.
//

#ifndef BROWNIANMOTION_SIMULATION_H
#define BROWNIANMOTION_SIMULATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window_maker.h"
#include "vector"
#include "RigidBody.h"
#include <thread>
#include <future>
#include <iostream>
#include <fstream>

enum SimState {
    SIM_ACTIVE,
    SIM_PAUSED
};
class Simulation
{
public:
    SimState    State;
    unsigned int Width, Height;
    std::vector<RigidBody *> Particles;
    std::ofstream performance;
    int totalParticles;
    int activeParticles = 20;
    float speed = 1;
    bool logging = false;
    int Logs = 0;
    Simulation(unsigned int width, unsigned int height);
    ~Simulation();

    void Init();

    void Update(float dt);
    void Render(Window *window);
    void CollisionUpdate(float dt);

    void CollisionResolution(RigidBody *pCircle, RigidBody *pCircle1);
    bool movingTowardsEachother(RigidBody *objA, RigidBody *objB);
    float * getSpeed();
    void TrackParticle(float time);
    void ConvertArrToVec(std::promise<std::vector<glm::vec2>> && p);
};


#endif //BROWNIANMOTION_SIMULATION_H
