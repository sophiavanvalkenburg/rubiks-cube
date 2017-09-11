#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>

class SubCube
{
    glm::vec3 position;
    float pitchAngle;
    float yawAngle;
    float rollAngle;
    public:
        SubCube() = default;
        SubCube(glm::vec3 position){
            this->position = position;
            this->pitchAngle = 0.0f;
            this->yawAngle = 0.0f;
            this->rollAngle = 0.0f;
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
    
    public:
        Cube(){
            this->initSubCubes();
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
    private:
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
