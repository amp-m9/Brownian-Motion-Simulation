//
// Created by andri on 04/11/2021.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // state.
    unsigned int ID;
    // constructor
    Shader(){ }
    // compiles shader from sourcecode.
    void Compile(const char *vertexPath, const char *fragmentPath, const char *geometrySource = nullptr);
    // sets current shader as active
    Shader &Use();
    // utility functions.
    void SetBool(const char *name, bool value, bool useShader = false);
    void SetInteger(const char *name, int value, bool useShader = false);
    void SetFloat(const char *name, float value, bool useShader = false);
    void SetVec2f(const char *name, const glm::vec2 &value, bool useShader = false);
    void SetVec2f(const char *name, float x, float y, bool useShader = false);
    void SetVec3f(const char *name, const glm::vec3 &value, bool useShader = false);
    void SetVec3f(const char *name, float x, float y, float z, bool useShader = false);
    void SetVec4f(const char *name, const glm::vec4 &value, bool useShader = false);
    void SetVec4f(const char *name, float x, float y, float z, float w, bool useShader = false);
//    void SetMat2(const char *name, const glm::mat2 &mat, bool useShader = false);
//    void SetMat3(const char *name, const glm::mat3 &mat, bool useShader = false);
    void SetMat4(const char *name, const glm::mat4 &mat, bool useShader = false);

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif //SHADER_H
