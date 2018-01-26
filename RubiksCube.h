#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <map>
#include <glm/glm.hpp>

class SubCube
{
    private:
        unsigned int id;
        glm::vec3 position;
        glm::vec3 rotation;

    public:
        bool isSelected;
        glm::mat4 modelMatrix;
        SubCube(glm::vec3 position, unsigned int id);
        glm::vec3 getPosition();
        glm::vec3* getRotation();
        void setRotationOnAxis(float angle, Axis axis);
        glm::mat4 getRotationMatrix();
        unsigned int getId();
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
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        float pitchAngle;
        float yawAngle;
        const float getSubCubeMargin();
        std::vector<SubCube*>* getSubCubes();
        bool faceContainsSubCube(unsigned int faceId, unsigned int subcubeId);
        glm::vec3 getFaceCenter(unsigned int faceId);
        void printFaces();

    private:
        std::vector<SubCube*> subcubes;
        std::vector<CubeFace> faces;
        void addSubCubeToFace(std::map<float, unsigned int> &posMap, float posCoord, SubCube *s);
        void initFaces();
        void initSubCubes();
};
#endif
