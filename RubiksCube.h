#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <map>
#include <glm/glm.hpp>

enum class Axis {X, Y, Z};

class SubCube
{
    private:
        unsigned int id;
        glm::vec3 position;
        glm::mat4 localRotationMatrix;
        glm::mat4 worldRotationMatrix;
        glm::mat4 modelMatrix;
        std::map<Axis, unsigned int> faceMap;
        std::vector<glm::mat4> transformMatrixHistory;
        glm::mat4 getFaceRotationMatrixForAxis(Axis axis);

    public:
        bool isSelected;
        bool isTapped;
        SubCube(glm::vec3 position, unsigned int id);
        void setPosition(glm::vec3 pos);
        void setLocalRotationMatrix(glm::mat4 rotationMatrix);
        void setWorldRotationMatrix(glm::mat4 rotationMatrix);
        void setModelMatrix(glm::mat4 modelMatrix);
        glm::vec3 getPosition();
        glm::mat4 getLocalRotationMatrix();
        glm::mat4 getWorldRotationMatrix();
        glm::mat4 getModelMatrix();
        void updateModelMatrix();
        glm::mat4 getFaceRotationMatrix();
        glm::mat4 multiplyTransformMatrixHistory();
        unsigned int getId();
        unsigned int getFace(Axis axis);
        void addFace(Axis axis, unsigned int faceId);
};

class CubeFace
{
    private:
        unsigned int id;
        std::vector<unsigned int> subcubeIds;
        glm::vec3 center;
        glm::vec3 rotation;
        float getPositionAverage(float centerPosition, float newPosition, int numSubCubes);

    public:
        CubeFace(unsigned int id);
        unsigned int getId();
        unsigned int getSize();
        glm::vec3 getCenter();
        bool containsSubCube(unsigned int subcubeId);
        void addSubCube(SubCube *s);
        void setRotationOnAxis(Axis axis, float angleOffset);
        glm::mat4 getRotationMatrix(Axis axis);
        void print();
};


class RubiksCube
{
    public:
        RubiksCube();
        float subcubeMargin;
        std::vector<SubCube*>* getSubCubes();
        CubeFace* getFace(unsigned int id);
        void printFaces();
        void updateModelMatrix();
        void updateFaceRotation(Axis axis, float angleOffset);
        void updateSubCubePositionsAndRotations();
        unsigned int getSelectedSubCubeId();
        void setSelectedSubCubeId(unsigned int id);
        unsigned int getSelectedFaceId();
        void setSelectedFaceId(unsigned int id);
        float getPitchAngle();
        void setPitchAngle(float angle);
        float getYawAngle();
        void setYawAngle(float angle);
        glm::mat4 getModelMatrix();
        void setModelMatrix(glm::mat4 modelMatrix);
        glm::mat4 getViewMatrix();
        void setViewMatrix(glm::mat4 viewMatrix);
    private:
        std::vector<SubCube*> subcubes;
        std::vector<CubeFace*> faces;
        unsigned int selectedSubCubeId;
        unsigned int selectedFaceId;
        float pitchAngle;
        float yawAngle;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        unsigned int getPosMapValue(std::map<float, unsigned int> &posMap, float posCoord);
        void addSubCubeToFace(std::map<float, unsigned int> &posMap, Axis axis, float posCoord, SubCube *s);
        void initFaces();
        void initSubCubes();
        const static std::vector<const glm::vec3> subcubePositions;
};
#endif
