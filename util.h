#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <glm/glm.hpp>

void copyVec3(glm::vec3 &out, glm::vec3 &v){
    out.x = v.x;
    out.y = v.y;
    out.z = v.z;
}

void printVec3(glm::vec3 &v){
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

glm::vec3 mat4xVec3(glm::vec3 out, const glm::mat4 m, const glm::vec3 v)
{
    glm::vec4 tmp = m * glm::vec4(v.x, v.y, v.z, 1.0f);
    glm::vec3 v1 = glm::vec3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);
    copyVec3(out, v1);
    return out;
}

bool eq(float a, float b){
    return a - b < MIN_EPSILON && b - a < MIN_EPSILON;
}

bool gte(float a, float b){
    return eq(a, b) || a - b > MIN_EPSILON;
}

bool lte(float a, float b){
    return eq(a, b) || a - b < MIN_EPSILON;
}

#endif