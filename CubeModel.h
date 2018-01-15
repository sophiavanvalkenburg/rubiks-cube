#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class CubeModel
{
    private:
        const static GLfloat vertices[];
    public:
        const static GLfloat* getVertices();
        static size_t getSizeOfVertices();
        static std::vector<glm::vec3> getBoundingBox();

};
#endif