#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "RubiksCube.h"
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

void SubCube::setRotationOnAxis(Axis axis, float angleOffset)
{
    switch(axis){
        case Axis::X:
            this->rotation.x += angleOffset; 
            break;
        case Axis::Y: 
            this->rotation.y += angleOffset;
            break;
        case Axis::Z: 
            this->rotation.z += angleOffset;
            break;
    }
};

glm::mat4 SubCube::getRotationMatrix()
{
    glm::mat4 x = glm::rotate(glm::mat4(), Util::getNearestValidAngle(this->rotation.x), State::X_AXIS);
    glm::mat4 y = glm::rotate(glm::mat4(), Util::getNearestValidAngle(this->rotation.y), State::Y_AXIS);
    glm::mat4 z = glm::rotate(glm::mat4(), Util::getNearestValidAngle(this->rotation.z), State::Z_AXIS);
    return x * y * z;
};

unsigned int SubCube::getId()
{
    return this->id;
};

void SubCube::addFace(Axis axis, unsigned int faceId)
{
    this->faceMap[axis] = faceId;
}

unsigned int SubCube::getFace(Axis axis)
{
    return this->faceMap[axis];
}


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

void RubiksCube::updateSubCubeRotationAngles(Axis axis, float angleOffset)
{
    for (unsigned int i = 0; i < this->subcubes.size(); i++)
    {
        SubCube *s = this->subcubes[i];
        if (s->getFace(axis) == this->selectedFaceId){
            s->setRotationOnAxis(axis, angleOffset);
        }
    }
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

void RubiksCube::addSubCubeToFace(std::map<float, unsigned int> &posMap, Axis axis, float posCoord, SubCube *s)
{
    unsigned int faceIndex = posMap[posCoord];
    if (faceIndex == 0){
        faceIndex = this->faces.size() + 1; // + 1 because we need to reserve 0 for checking map
        posMap[posCoord] = faceIndex;                    
        CubeFace cubeface;
        this->faces.push_back(cubeface);
    }
    unsigned int faceId = faceIndex - 1;
    CubeFace cubeface = this->faces[faceId];
    cubeface.addSubCube(s);
    s->addFace(axis, faceId);
    this->faces[faceId] = cubeface;
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
        addSubCubeToFace(xPosToIndexMap, Axis::X, pos.x, s);
        addSubCubeToFace(yPosToIndexMap, Axis::Y, pos.y, s);
        addSubCubeToFace(zPosToIndexMap, Axis::Z, pos.z, s);
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
