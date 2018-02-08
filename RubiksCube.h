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
        std::map<Axis, unsigned int> faceMap;

    public:
        glm::mat4 localRotationMatrix;
        bool isSelected;
        bool isTapped;
        glm::mat4 modelMatrix;
        std::vector<glm::mat4> transformMatrixHistory;
        SubCube(glm::vec3 position, unsigned int id);
        void setPosition(glm::vec3 pos);
        glm::vec3 getPosition();
        glm::mat4 getLocalRotationMatrix(Axis axis);
        glm::mat4 getFaceRotationMatrix(Axis axis);
        glm::mat4 getTransformationMatrix(glm::mat4 &rotationMatrix);
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
        void clearRotation();
        void setRotationOnAxis(Axis axis, float angleOffset);
        glm::mat4 getRotationMatrix(Axis axis);
        void print();
};


class RubiksCube
{
    public:
        RubiksCube();
        unsigned int selectedSubCubeId;
        unsigned int selectedFaceId;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        float pitchAngle;
        float yawAngle;
        const float getSubCubeMargin();
        std::vector<SubCube*>* getSubCubes();
        CubeFace* getFace(unsigned int id);
        bool faceContainsSubCube(unsigned int faceId, unsigned int subcubeId);
        glm::vec3 getFaceCenter(unsigned int faceId);
        void printFaces();
        void updateFaceRotation(Axis axis, float angleOffset);
        void updateSubCubePositionsAndRotations();
    private:
        std::vector<SubCube*> subcubes;
        std::vector<CubeFace*> faces;
        unsigned int getPosMapValue(std::map<float, unsigned int> &posMap, float posCoord);
        void addSubCubeToFace(std::map<float, unsigned int> &posMap, Axis axis, float posCoord, SubCube *s);
        void initFaces();
        void initSubCubes();
};
#endif
