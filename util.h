#ifndef UTIL_H
#define UTIL_H

#include <glm/glm.hpp>

class Util
{
    public:
        static const float MIN_EPSILON;
        static float getNearestValidAngle(float angle);
        static void copyMat4(glm::mat4 &out, glm::mat4 &in);
        static void copyVec3(glm::vec3 &out, glm::vec3 &v);
        static void printVec3(glm::vec3 &v);
        static glm::vec3 mat4xVec3(glm::vec3 out, const glm::mat4 m, const glm::vec3 v);
        static bool eq(float a, float b);
        static bool gte(float a, float b);
        static bool lte(float a, float b);
};

#endif
