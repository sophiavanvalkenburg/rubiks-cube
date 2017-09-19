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
    public:
        SubCube() = default;
        SubCube(glm::vec3 position, unsigned int id){
            this->position = position;
            this->id = id;
        }

        glm::vec3 getPosition(){
            return this->position;
        }

        unsigned int getId(){
            return this->id;
        }
};

class CubeFace
{
    private:
        std::vector<unsigned int> subcubeIds;
        glm::vec3 center;

        float getPositionAverage(float centerPosition, float newPosition, int numSubCubes){
            return (numSubCubes * centerPosition + newPosition) / (numSubCubes + 1);
        }

    public:
        CubeFace() = default;

        unsigned int getSize(){
            return this->subcubeIds.size();
        }

        glm::vec3 getCenter(){
            return this->center;
        }

        bool containsSubCube(unsigned int subcubeId){
            for (unsigned int i=0; i<this->subcubeIds.size(); i++){
                if (this->subcubeIds[i] == subcubeId) return true;
            }
            return false;
        }

        void addSubCube(SubCube s){
            unsigned int numSubCubes = this->subcubeIds.size();
            glm::vec3 subcubePos = s.getPosition();
            float centerX = getPositionAverage(this->center.x, subcubePos.x, numSubCubes);
            float centerY = getPositionAverage(this->center.y, subcubePos.y, numSubCubes);
            float centerZ = getPositionAverage(this->center.z, subcubePos.z, numSubCubes);
            this->center.x = centerX;
            this->center.y = centerY;
            this->center.z = centerZ;
            this->subcubeIds.push_back(s.getId());
        }

        void print(){
            for (unsigned int i=0; i<this->subcubeIds.size(); i++){
                std::cout << this->subcubeIds[i] << " ";
            }
            std::cout << std::endl;
        }
};

class Cube
{
    GLfloat vertices[216] = {
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

    std::vector<SubCube> subcubes;

    std::vector<CubeFace> faces;
    
    public:
        Cube(){
            this->initSubCubes();
            this->initFaces(); 
        }
        GLfloat* getVertices(){
            return this->vertices;
        } 

        size_t getSizeOfVertices(){
            return sizeof(this->vertices);
        }

        std::vector<SubCube> getSubCubes(){
            return this->subcubes;
        }

        bool faceContainsSubCube(unsigned int faceId, unsigned int subcubeId){
            return this->faces[faceId].containsSubCube(subcubeId);
        }

        glm::vec3 getFaceCenter(unsigned int faceId){
            return this->faces[faceId].getCenter();
        }

        void printFaces(){
            for (unsigned int i=0; i<this->faces.size(); i++){
                this->faces[i].print();
            }
        }

    private:
        void addSubCubeToFace(std::map<float, unsigned int> &posMap, float posCoord, SubCube s){
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
        }
        void initFaces(){
            // construct the faces by grouping together all subcubes that share an X, Y, or Z value
            std::map<float, unsigned int> xPosToIndexMap;
            std::map<float, unsigned int> yPosToIndexMap;
            std::map<float, unsigned int> zPosToIndexMap;
            for (unsigned int i=0; i<this->subcubes.size(); i++){
                SubCube s = this->subcubes[i];
                glm::vec3 pos = s.getPosition();
                addSubCubeToFace(xPosToIndexMap, pos.x, s);
                addSubCubeToFace(yPosToIndexMap, pos.y, s);
                addSubCubeToFace(zPosToIndexMap, pos.z, s);
            }
            printFaces();
        }
        void initSubCubes(){
            std::vector<glm::vec3> positions = {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.325f, 0.0f, 0.0f),
                glm::vec3(-0.325f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.325f, 0.0f),
                glm::vec3(0.0f, -0.325f, 0.0f),
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
                this->subcubes.push_back(SubCube(positions[i], i));
            }
        }
};
#endif
