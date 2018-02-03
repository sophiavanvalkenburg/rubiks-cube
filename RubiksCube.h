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
        glm::vec3 rotation;
        std::map<Axis, unsigned int> faceMap;

    public:
        bool isSelected;
        glm::mat4 modelMatrix;
        SubCube(glm::vec3 position, unsigned int id);
        glm::vec3 getPosition();
        glm::vec3* getRotation();
        void setRotationOnAxis(Axis axis, float angleOffset);
        glm::mat4 getRotationMatrix(Axis axis);
        unsigned int getId();
        unsigned int getFace(Axis axis);
        void addFace(Axis axis, unsigned int faceId);
};

class CubeFace
{
    private:
        std::vector<unsigned int> subcubeIds;
        glm::vec3 center;
        float getPositionAverage(float centerPosition, float newPosition, int numSubCubes);

    public:
        CubeFace() = default;
        unsigned int getSize();
        glm::vec3 getCenter();
        bool containsSubCube(unsigned int subcubeId);
        void addSubCube(SubCube *s);
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
        bool faceContainsSubCube(unsigned int faceId, unsigned int subcubeId);
        glm::vec3 getFaceCenter(unsigned int faceId);
        void printFaces();
        void updateSubCubeRotationAngles(Axis axis, float angleOffset);
    private:
        std::vector<SubCube*> subcubes;
        std::vector<CubeFace> faces;
        void addSubCubeToFace(std::map<float, unsigned int> &posMap, Axis axis, float posCoord, SubCube *s);
        void initFaces();
        void initSubCubes();
};
#endif
