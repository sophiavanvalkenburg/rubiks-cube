#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <glm/glm.hpp>

class SubCube
{
    glm::vec3 position;
    public:
        SubCube() = default;
        SubCube(glm::vec3 position){
            this->position = position;
        }

        glm::vec3 getPosition(){
            return this->position;
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

    static const int NUM_SUBCUBES = 27;

    std::vector<SubCube> subcubes;

    std::vector<std::vector<unsigned int>> faces;
    
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

        unsigned int getNumPositions(){
            return NUM_SUBCUBES;
        }

        std::vector<SubCube> getSubCubes(){
            return this->subcubes;
        }

        bool faceContainsSubCube(unsigned int faceId, unsigned int subcubeId){
            for (unsigned int i=0; i<9; i++){
                if (this->faces[0][i] == subcubeId) return true;
            }
            return false;
        }

        glm::vec3 getFaceCenter(unsigned int faceId){
            return glm::vec3(-0.325f, 0.0f, 0.0f);
        }
    private:
        void initFaces(){
            this->faces = {
                {2, 6, 8, 11, 15, 17, 20, 24, 26}
            };
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
                this->subcubes.push_back(SubCube(positions[i]));
            }
        }
};
#endif
