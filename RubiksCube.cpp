#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "RubiksCube.h"
#include "State.h"
#include "Util.h"

/*** SubCube ***/

SubCube::SubCube(const glm::vec3 position, unsigned int id)
{
    Util::copyVec3(this->position, position);
    this->localRotationMatrix = glm::mat4();
    this->worldRotationMatrix = glm::mat4();
    this->modelMatrix = glm::mat4();
    this->id = id;
    this->isSelected = false;
    this->transformMatrixHistory = std::vector<glm::mat4>();
};

bool SubCube::getIsTapped()
{
    return this->isTapped;
}

void SubCube::setIsTapped(bool tapped)
{
    this->isTapped = tapped;
}

bool SubCube::getIsSelected()
{
    return this->isSelected;
}

void SubCube::setIsSelected(bool selected)
{
    this->isSelected = selected;
}

glm::vec3 SubCube::getPosition()
{
    return this->position;
};

glm::mat4 SubCube::getLocalRotationMatrix()
{
    return this->localRotationMatrix;
}

glm::mat4 SubCube::getWorldRotationMatrix()
{
    return this->worldRotationMatrix;
}

glm::mat4 SubCube::getModelMatrix()
{
    return this->modelMatrix;
}

void SubCube::setPosition(glm::vec3 pos)
{
    Util::copyVec3(this->position, pos);
}

void SubCube::setLocalRotationMatrix(glm::mat4 rotationMatrix)
{
    Util::copyMat4(this->localRotationMatrix, rotationMatrix);
}

void SubCube::setWorldRotationMatrix(glm::mat4 rotationMatrix)
{
    Util::copyMat4(this->worldRotationMatrix, rotationMatrix);
}

void SubCube::setModelMatrix(glm::mat4 modelMatrix)
{
    Util::copyMat4(this->modelMatrix, modelMatrix);
}

glm::mat4 SubCube::getFaceRotationMatrixForAxis(Axis axis)
{
    unsigned int faceId = this->getFace(axis);
    CubeFace *face = State::rubiksCube.getFace(faceId);
    return face->getRotationMatrix(axis);
};

glm::mat4 SubCube::getFaceRotationMatrix()
{
    glm::mat4 xRotationMatrix = this->getFaceRotationMatrixForAxis(Axis::X);
    glm::mat4 yRotationMatrix = this->getFaceRotationMatrixForAxis(Axis::Y);
    glm::mat4 zRotationMatrix = this->getFaceRotationMatrixForAxis(Axis::Z);
    return xRotationMatrix * yRotationMatrix * zRotationMatrix;
};

void SubCube::updateModelMatrix(){
    unsigned int subcubeId = this->getId();
    glm::mat4 subcubeTranslateMatrix = glm::translate(glm::mat4(), this->getPosition());
    glm::mat4 faceRotationMatrix = this->getFaceRotationMatrix();
    glm::mat4 cubeModelMatrix = State::rubiksCube.getModelMatrix();
    this->modelMatrix = cubeModelMatrix * faceRotationMatrix * subcubeTranslateMatrix * this->localRotationMatrix;
    this->worldRotationMatrix = cubeModelMatrix * faceRotationMatrix * this->localRotationMatrix;
};

glm::mat4 SubCube::multiplyTransformMatrixHistory()
{
    glm::mat4 matrix = glm::mat4();
    for (unsigned int i = 0; i < this->transformMatrixHistory.size(); i++)
    {
        matrix = transformMatrixHistory[i] * matrix;
    }
    return matrix;
}

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

CubeFace::CubeFace(unsigned int id)
{
    this->rotation = glm::vec3();
    this->id = id;
};

unsigned int CubeFace::getId()
{
    return this->id;
}

float CubeFace::getPositionAverage(float centerPosition, float newPosition, int numSubCubes)
{
    return (numSubCubes * centerPosition + newPosition) / (numSubCubes + 1);
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
    float centerX = this->getPositionAverage(this->center.x, subcubePos.x, numSubCubes);
    float centerY = this->getPositionAverage(this->center.y, subcubePos.y, numSubCubes);
    float centerZ = this->getPositionAverage(this->center.z, subcubePos.z, numSubCubes);
    this->center.x = centerX;
    this->center.y = centerY;
    this->center.z = centerZ;
    this->subcubeIds.push_back(s->getId());
};

void CubeFace::setRotationOnAxis(Axis axis, float angleOffset)
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

glm::mat4 CubeFace::getRotationMatrix(Axis axis)
{
    glm::mat4 matrix;
    switch(axis){
       case Axis::X:
            matrix = glm::rotate(glm::mat4(), Util::getNearestValidAngle(this->rotation.x), State::X_AXIS);
            break;
        case Axis::Y: 
            matrix = glm::rotate(glm::mat4(), Util::getNearestValidAngle(this->rotation.y), State::Y_AXIS); 
            break;
        case Axis::Z: 
            matrix = glm::rotate(glm::mat4(), Util::getNearestValidAngle(this->rotation.z), State::Z_AXIS);
            break; 
    }
    return matrix;
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
    this->subcubeMargin = 0.025f;
};

unsigned int RubiksCube::getSelectedSubCubeId(){
    return this->selectedSubCubeId;
}

void RubiksCube::setSelectedSubCubeId(unsigned int id){
    this->selectedSubCubeId = id;
}

unsigned int RubiksCube::getSelectedFaceId(){
    return this->selectedFaceId;
}

void RubiksCube::setSelectedFaceId(unsigned int id){
    this->selectedFaceId = id;
}

float RubiksCube::getPitchAngle(){
    return this->pitchAngle;
}

void RubiksCube::setPitchAngle(float angle){
    this->pitchAngle = angle;
}

float RubiksCube::getYawAngle(){
    return this->yawAngle;
}

void RubiksCube::setYawAngle(float angle){
    this->yawAngle = angle;
}

glm::mat4 RubiksCube::getModelMatrix(){
    return this->modelMatrix;
}

void RubiksCube::setModelMatrix(glm::mat4 modelMatrix){
    Util::copyMat4(this->modelMatrix, modelMatrix);   
}

glm::mat4 RubiksCube::getViewMatrix(){
    return this->viewMatrix;
}

void RubiksCube::setViewMatrix(glm::mat4 viewMatrix){
    Util::copyMat4(this->viewMatrix, viewMatrix);
}

std::vector<SubCube*>* RubiksCube::getSubCubes()
{
    return &this->subcubes;
};

CubeFace* RubiksCube::getFace(unsigned int id)
{
    return this->faces[id];
}

void RubiksCube::updateModelMatrix()
{
    glm::vec3 right = Util::mat4xVec3(glm::vec3(), this->viewMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 up = Util::mat4xVec3(glm::vec3(), this->viewMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    this->modelMatrix = glm::rotate(this->modelMatrix, State::rubiksCube.getPitchAngle(), right);
    this->modelMatrix = glm::rotate(this->modelMatrix, State::rubiksCube.getYawAngle(), up);
    this->viewMatrix = glm::inverse(this->modelMatrix);
}

void RubiksCube::updateFaceRotation(Axis axis, float angleOffset)
{
    for (unsigned int i = 0; i < this->faces.size(); i++)
    {
        CubeFace *face = this->faces[i];
        if (face->getId() == this->selectedFaceId){
            face->setRotationOnAxis(axis, angleOffset);
        }
    }
};

void RubiksCube::updateSubCubePositionsAndRotations()
{
    for (unsigned int i = 0; i < this->subcubes.size(); i++)
    {
        SubCube *s = this->subcubes[i];
        glm::vec3 oldPos = s->getPosition();
        glm::mat4 faceRotationMatrix = s->getFaceRotationMatrix(); 
        glm::vec3 newPos = Util::mat4xVec3(glm::vec3(), faceRotationMatrix, oldPos);
        s->setPosition(newPos);
        s->setLocalRotationMatrix(faceRotationMatrix * s->getLocalRotationMatrix());
    }

    // reset faces
    this->faces.clear();
    this->initFaces();
}

void RubiksCube::printFaces()
{
    for (unsigned int i=0; i<this->faces.size(); i++){
        this->faces[i]->print();
    }
};

unsigned int RubiksCube::getPosMapValue(std::map<float, unsigned int> &posMap, float posCoord)
{
    unsigned int val = posMap[posCoord];
    if (!val){
        for(std::map<float, unsigned int>::iterator iter = posMap.begin(); iter != posMap.end(); ++iter)
        {
            if (!iter->second) continue; // ensure value is not 0 (reserved for checking map)
            float key =  iter->first;
            if (Util::eq(key, posCoord)){
                val = iter->second;
                break;
            }
        }
    }
    return val;
}

void RubiksCube::addSubCubeToFace(std::map<float, unsigned int> &posMap, Axis axis, float posCoord, SubCube *s)
{
    unsigned int faceIndex = this->getPosMapValue(posMap, posCoord);
    if (faceIndex == 0){
        faceIndex = this->faces.size() + 1; // + 1 because we need to reserve 0 for checking map
        posMap[posCoord] = faceIndex;                    
        CubeFace *cubeface = new CubeFace(faceIndex - 1);
        this->faces.push_back(cubeface);
    }
    unsigned int faceId = faceIndex - 1;
    CubeFace *cubeface = this->faces[faceId];
    cubeface->addSubCube(s);
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
    for (unsigned int i=0; i<RubiksCube::subcubePositions.size(); i++){
        SubCube *s = new SubCube(RubiksCube::subcubePositions[i], i);
        this->subcubes.push_back(s);
    }
};

const std::vector<const glm::vec3> RubiksCube::subcubePositions = {
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
