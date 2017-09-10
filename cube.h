#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>

class Cube
{
    GLfloat vertices[216];
    glm::vec3 positions[27];
public:
   Cube(){
	GLfloat vertices[] = {
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
	for (unsigned int i=0; i<216; i++)
            this->vertices[i] = vertices[i];
    
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
	for (unsigned int i=0; i<27; i++)
            this->positions[i] = positions[i];

   }

   GLfloat* getVertices(){
	return this->vertices;
   } 

   size_t getSizeOfVertices(){
	return sizeof(this->vertices);
   }
   
   glm::vec3* getPositions(){
	return this->positions;
   }

   unsigned int getNumPositions(){
	return 27;
   }
};
#endif