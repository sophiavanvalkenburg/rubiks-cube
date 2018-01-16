#include "CubeModel.h"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

const GLfloat CubeModel::vertices[] = {
    /*** P:180 / Y:0 ***/
    -0.15f, -0.15f, -0.15f, 0.0f, 1.0f, 1.0f, 
    0.15f, -0.15f, -0.15f,  0.0f, 1.0f, 1.0f,
    0.15f,  0.15f, -0.15f, 0.0f, 1.0f, 1.0f,
    0.15f,  0.15f, -0.15f,  0.0f, 1.0f, 1.0f,
    -0.15f,  0.15f, -0.15f,  0.0f, 1.0f, 1.0f,
    -0.15f, -0.15f, -0.15f,  0.0f, 1.0f, 1.0f,

    /*** P:0 / Y:0 ***/
    -0.15f, -0.15f,  0.15f,  1.0f, 0.0f, 0.0f,
    0.15f, -0.15f,  0.15f,  1.0f, 0.0f, 0.0f,
    0.15f,  0.15f,  0.15f,  1.0f, 0.0f, 0.0f,
    0.15f,  0.15f,  0.15f,  1.0f, 0.0f, 0.0f,
    -0.15f,  0.15f,  0.15f, 1.0f, 0.0f, 0.0f,
    -0.15f, -0.15f,  0.15f, 1.0f, 0.0f, 0.0f,

    /*** P:0 / Y:90 ***/
    -0.15f,  0.15f,  0.15f,  0.0f, 1.0f, 0.0f,
    -0.15f,  0.15f, -0.15f,  0.0f, 1.0f, 0.0f,
    -0.15f, -0.15f, -0.15f,  0.0f, 1.0f, 0.0f,
    -0.15f, -0.15f, -0.15f,  0.0f, 1.0f, 0.0f,
    -0.15f, -0.15f,  0.15f,  0.0f, 1.0f, 0.0f,
    -0.15f,  0.15f,  0.15f,  0.0f, 1.0f, 0.0f,

    /*** P:0 / Y:270 ***/
    0.15f,  0.15f,  0.15f,  0.0f, 0.0f, 1.0f,
    0.15f,  0.15f, -0.15f,  0.0f, 0.0f, 1.0f,
    0.15f, -0.15f, -0.15f,  0.0f, 0.0f, 1.0f,
    0.15f, -0.15f, -0.15f,  0.0f, 0.0f, 1.0f,
    0.15f, -0.15f,  0.15f,  0.0f, 0.0f, 1.0f,
    0.15f,  0.15f,  0.15f,  0.0f, 0.0f, 1.0f,

    /*** P:270 / Y:0 ***/
    -0.15f, -0.15f, -0.15f,  1.0f, 0.0f, 1.0f,
    0.15f, -0.15f, -0.15f,  1.0f, 0.0f, 1.0f,
    0.15f, -0.15f,  0.15f,  1.0f, 0.0f, 1.0f,
    0.15f, -0.15f,  0.15f,  1.0f, 0.0f, 1.0f,
    -0.15f, -0.15f,  0.15f, 1.0f, 0.0f, 1.0f,
    -0.15f, -0.15f, -0.15f, 1.0f, 0.0f, 1.0f,

    /*** P:90 / Y:0 ***/
    -0.15f,  0.15f, -0.15f, 1.0f, 1.0f, 0.0f,
    0.15f,  0.15f, -0.15f,  1.0f, 1.0f, 0.0f,
    0.15f,  0.15f,  0.15f,  1.0f, 1.0f, 0.0f,
    0.15f,  0.15f,  0.15f,  1.0f, 1.0f, 0.0f,
    -0.15f,  0.15f,  0.15f, 1.0f, 1.0f, 0.0f,
    -0.15f,  0.15f, -0.15f,  1.0f, 1.0f, 0.0f
};

const glm::vec3 CubeModel::boundingMin = glm::vec3(-0.15f, -0.15f, -0.15f);
const glm::vec3 CubeModel::boundingMax = glm::vec3(0.15f, 0.15f, 0.15f);

const std::vector<glm::vec3> CubeModel::facesMinMax = {
    CubeModel::boundingMax, glm::vec3(-1.0f * CubeModel::boundingMax.x, -1.0f * CubeModel::boundingMax.y, CubeModel::boundingMax.z),
    CubeModel::boundingMax, glm::vec3(-1.0f * CubeModel::boundingMax.x, CubeModel::boundingMax.y, -1.0f * CubeModel::boundingMax.z),
    CubeModel::boundingMax, glm::vec3(CubeModel::boundingMax.x, -1.0f * CubeModel::boundingMax.y, -1.0f * CubeModel::boundingMax.z),
    glm::vec3(-1.0f * CubeModel::boundingMin.x, -1.0f * CubeModel::boundingMin.y, CubeModel::boundingMin.z), CubeModel::boundingMin,
    glm::vec3(-1.0f * CubeModel::boundingMin.x, CubeModel::boundingMin.y, -1.0f * CubeModel::boundingMin.z), CubeModel::boundingMin,
    glm::vec3(CubeModel::boundingMin.x, -1.0f * CubeModel::boundingMin.y, -1.0f * CubeModel::boundingMin.z), CubeModel::boundingMin
};


const GLfloat* CubeModel::getVertices(){
    return CubeModel::vertices;
};

const std::vector<glm::vec3> CubeModel::getFacesMinMax(){
    return CubeModel::facesMinMax;
}

glm::vec3 CubeModel::getNormal(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2){
    glm::vec3 edge1 = glm::vec3(vert0.x - vert1.x, vert0.y - vert1.y, vert0.z - vert1.z);
    glm::vec3 edge2 = glm::vec3(vert0.x - vert2.x, vert0.y - vert2.y, vert0.z - vert2.z);
    glm::vec3 cross = glm::cross(edge1, edge2);
    return glm::normalize(cross);
}

const std::vector<glm::vec3> CubeModel::getFaceNormals(){
    const glm::vec3 max = CubeModel::boundingMax;
   
    glm::vec3 v1 = glm::vec3(-1.0f * max.x, max.y, max.z);
    glm::vec3 v2 = glm::vec3(max.x, -1.0f * max.y, max.z);
    glm::vec3 v3 = glm::vec3(max.x, max.y, -1.0f * max.z);
    glm::vec3 normal1 = CubeModel::getNormal(max, v1, v2);
    glm::vec3 normal2 = -1.0f * CubeModel::getNormal(max, v1, v3);
    glm::vec3 normal3 = CubeModel::getNormal(max, v2, v3);
    glm::vec3 normal4 = glm::normalize(glm::vec3(-1.0f * normal1.x, -1.0f * normal1.y, -1.0f * normal1.z));
    glm::vec3 normal5 = glm::normalize(glm::vec3(-1.0f * normal2.x, -1.0f * normal2.y, -1.0f * normal2.z));
    glm::vec3 normal6 = glm::normalize(glm::vec3(-1.0f * normal3.x, -1.0f * normal3.y, -1.0f * normal3.z));
    
    const std::vector<glm::vec3> normals = { normal1, normal2, normal3, normal4, normal5, normal6 };
    return normals;
};

const size_t CubeModel::getSizeOfVertices(){
    return sizeof(CubeModel::vertices);
};
