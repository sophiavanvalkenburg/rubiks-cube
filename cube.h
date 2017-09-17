#ifndef CUBE_H
#define CUBE_H

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

    SubCube subcubes[NUM_SUBCUBES];

    SubCube* xFaces[3];
    SubCube* yFaces[3];
    SubCube* zFaces[3];
    
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

        SubCube* getSubCubes(){
            return this->subcubes;
        }

        bool faceContainsSubCube(uint faceId, uint subcubeId){
            uint ids[9] = {2, 6, 8, 11, 15, 17, 20, 24, 26};
            for (uint i=0; i<9; i++){
                if (ids[i] == subcubeId) return true;
            }
            return false;
        }

        glm::vec3 getFaceCenter(uint faceId){
            return glm::vec3(-0.325f, 0.0f, 0.0f);
        }
    private:
        void initFaces(){
            SubCube xFace1[9] = {
                this->subcubes[2],
                this->subcubes[6],
                this->subcubes[8],
                this->subcubes[11],
                this->subcubes[15],
                this->subcubes[17],
                this->subcubes[20],
                this->subcubes[24],
                this->subcubes[26] 
            };
            this->xFaces[0] = xFace1; 
        }
        void initSubCubes(){
            glm::vec3 positions[] = {
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
            for (unsigned int i=0; i<NUM_SUBCUBES; i++){
                this->subcubes[i] = SubCube(positions[i]);
            }
        }
};
#endif
