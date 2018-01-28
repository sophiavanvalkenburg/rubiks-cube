#include "Util.h"
#include <glm/glm.hpp>
#include <iostream>

const float Util::MIN_EPSILON = 1e-6;

float Util::getNearestValidAngle(float angle){
    return (M_PI_4 / 2 ) * std::round(2 * angle / M_PI_4);
}

void Util::copyMat4(glm::mat4 &out, glm::mat4 &in){
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            out[i][j] = in[i][j];
        }
    }
}

void Util::copyVec3(glm::vec3 &out, glm::vec3 &v){
    out.x = v.x;
    out.y = v.y;
    out.z = v.z;
}

void Util::printVec3(glm::vec3 &v){
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

glm::vec3 Util::mat4xVec3(glm::vec3 out, const glm::mat4 m, const glm::vec3 v)
{
    glm::vec4 tmp = m * glm::vec4(v.x, v.y, v.z, 1.0f);
    glm::vec3 v1 = glm::vec3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);
    copyVec3(out, v1);
    return out;
}

bool Util::eq(float a, float b){
    return a - b < MIN_EPSILON && b - a < MIN_EPSILON;
}

bool Util::gte(float a, float b){
    return eq(a, b) || a - b > MIN_EPSILON;
}

bool Util::lte(float a, float b){
    return eq(a, b) || a - b < MIN_EPSILON;
}