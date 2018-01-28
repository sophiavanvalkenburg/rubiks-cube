#include "RubiksCube.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "State.h"
#include "Util.h"

/*** SubCube ***/

SubCube::SubCube(glm::vec3 position, unsigned int id)
{
    this->position = position;
    this->rotation = glm::vec3();
    this->id = id;
    this->isSelected = false;
    this->modelMatrix = glm::mat4();
};

glm::vec3 SubCube::getPosition()
{
    return this->position;
};

glm::vec3* SubCube::getRotation()
{
    return &this->rotation;
};

void SubCube::setRotationOnAxis(float angleOffset, Axis axis)
{
    float angle;
    switch(axis){
        case Axis::X:
            angle = this->rotation.x + angleOffset; 
            this->rotation.x = Util::getNearestValidAngle(angle);
            break;
        case Axis::Y: 
            angle = this->rotation.y += angleOffset;
            this->rotation.y = Util::getNearestValidAngle(angle);
            break;
        case Axis::Z: 
            angle = this->rotation.z += angleOffset;
            this->rotation.z = Util::getNearestValidAngle(angle);
            break;
    }
};

glm::mat4 SubCube::getRotationMatrix()
{
    glm::mat4 x = glm::rotate(glm::mat4(), this->rotation.x, State::X_AXIS);
    glm::mat4 y = glm::rotate(glm::mat4(), this->rotation.y, State::Y_AXIS);
    glm::mat4 z = glm::rotate(glm::mat4(), this->rotation.z, State::Z_AXIS);
    return x * y * z;
};

unsigned int SubCube::getId()
{
    return this->id;
};


/*** CubeFace ***/

float CubeFace::getPositionAverage(float centerPosition, float newPosition, int numSubCubes)
{
    return (numSubCubes * centerPosition + newPosition) / (numSubCubes + 1);
};

unsigned int CubeFace::getSize()
{
    return subcubeIds.size();
};

glm::vec3 CubeFace::getCenter()
{
    return this->center;
};

bool CubeFace::containsSubCube(unsigned int subcubeId)
{
    for (unsigned int i=0; i<this->subcubeIds.size(); i++){
        if (this->subcubeIds[i] == subcubeId) return true;
    }
    return false;
};

void CubeFace::addSubCube(SubCube *s)
{
    unsigned int numSubCubes = this->subcubeIds.size();
    glm::vec3 subcubePos = s->getPosition();
    float centerX = getPositionAverage(this->center.x, subcubePos.x, numSubCubes);
    float centerY = getPositionAverage(this->center.y, subcubePos.y, numSubCubes);
    float centerZ = getPositionAverage(this->center.z, subcubePos.z, numSubCubes);
    this->center.x = centerX;
    this->center.y = centerY;
    this->center.z = centerZ;
    this->subcubeIds.push_back(s->getId());
};

void CubeFace::print()
{
    for (unsigned int i=0; i<this->subcubeIds.size(); i++){
        std::cout << this->subcubeIds[i] << " ";
    }
    std::cout << std::endl;
};


/*** RubiksCube ***/

RubiksCube::RubiksCube()
{
    this->initSubCubes();
    this->initFaces();  
    this->pitchAngle = 0.0f;
    this->yawAngle = 0.0f;
};

const float RubiksCube::getSubCubeMargin()
{
    return 0.025f;
};

std::vector<SubCube*>* RubiksCube::getSubCubes()
{
    return &this->subcubes;
};

bool RubiksCube::faceContainsSubCube(unsigned int faceId, unsigned int subcubeId)
{
    return this->faces[faceId].containsSubCube(subcubeId);
};

glm::vec3 RubiksCube::getFaceCenter(unsigned int faceId)
{
    return this->faces[faceId].getCenter();
};

void RubiksCube::printFaces()
{
    for (unsigned int i=0; i<this->faces.size(); i++){
        this->faces[i].print();
    }
};

void RubiksCube::addSubCubeToFace(std::map<float, unsigned int> &posMap, float posCoord, SubCube *s)
{
    unsigned int faceIndex = posMap[posCoord];
    if (faceIndex == 0){
        faceIndex = this->faces.size() + 1; // + 1 because we need to reserve 0 for checking map
        posMap[posCoord] = faceIndex;                    
        CubeFace cubeface;
        this->faces.push_back(cubeface);
    }
    CubeFace cubeface = this->faces[faceIndex - 1];
    cubeface.addSubCube(s);
    this->faces[faceIndex - 1] = cubeface;
};

void RubiksCube::initFaces()
{
    // construct the faces by grouping together all subcubes that share an X, Y, or Z value
    std::map<float, unsigned int> xPosToIndexMap;
    std::map<float, unsigned int> yPosToIndexMap;
    std::map<float, unsigned int> zPosToIndexMap;
    for (unsigned int i=0; i<this->subcubes.size(); i++){
        SubCube *s = this->subcubes[i];
        glm::vec3 pos = s->getPosition();
        addSubCubeToFace(xPosToIndexMap, pos.x, s);
        addSubCubeToFace(yPosToIndexMap, pos.y, s);
        addSubCubeToFace(zPosToIndexMap, pos.z, s);
    }
};

void RubiksCube::initSubCubes()
{
    std::vector<glm::vec3> positions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.325f, 0.0f, 0.0f),
        glm::vec3(-0.325f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.325f, 0.0f),
        glm::vec3(0.0f, -0.325f,0.0f),
        glm::vec3(0.325f, 0.325f, 0.0f),
        glm::vec3(-0.325f, 0.325f, 0.0f),
        glm::vec3(0.325f, -0.325f, 0.0f),
        glm::vec3(-0.325f, -0.325f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.325f), 
        glm::vec3(0.325f, 0.0f, 0.325f),
        glm::vec3(-0.325f, 0.0f, 0.325f), 
        glm::vec3(0.0f, 0.325f, 0.325f),
        glm::vec3(0.0f, -0.325f, 0.325f),
        glm::vec3(0.325f, 0.325f, 0.325f),
        glm::vec3(-0.325f, 0.325f, 0.325f),
        glm::vec3(0.325f, -0.325f, 0.325f),
        glm::vec3(-0.325f, -0.325f, 0.325f),
        glm::vec3(0.0f, 0.0f, -0.325f), 
        glm::vec3(0.325f, 0.0f, -0.325f),
        glm::vec3(-0.325f, 0.0f, -0.325f), 
        glm::vec3(0.0f, 0.325f, -0.325f),
        glm::vec3(0.0f, -0.325f, -0.325f),
        glm::vec3(0.325f, 0.325f, -0.325f),
        glm::vec3(-0.325f, 0.325f, -0.325f),
        glm::vec3(0.325f, -0.325f, -0.325f),
        glm::vec3(-0.325f, -0.325f, -0.325f)
        
    };
    for (unsigned int i=0; i<positions.size(); i++){
        SubCube *s = new SubCube(positions[i], i);
        this->subcubes.push_back(s);
    }
};
