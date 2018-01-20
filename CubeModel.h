#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class CubeModel
{
    private:
        const static GLfloat vertices[];
        const static std::vector<glm::vec3> facesMinMax;
        const static std::vector<glm::vec3> faceNormals;
    public:
        const static glm::vec3 boundingMin;
        const static glm::vec3 boundingMax;
        const static GLfloat* getVertices();
        const static std::vector<glm::vec3> getFacesMinMax();
        const static std::vector<glm::vec3> getFaceNormals();
        const static size_t getSizeOfVertices();
        const static glm::vec3 getCubeSideLengths();
};
#endif