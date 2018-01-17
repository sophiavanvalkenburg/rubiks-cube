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
        const static glm::vec3 boundingMin;
        const static glm::vec3 boundingMax;
        static glm::vec3 getNormal(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2);
    public:
        const static GLfloat* getVertices();
        const static std::vector<glm::vec3> getFacesMinMax();
        const static std::vector<glm::vec3> getFaceNormals();
        const static size_t getSizeOfVertices();
};
#endif