#ifndef DEBUG_H
#define DEBUG_H

#include "shader.h"
#include "State.h"
#include "RenderEngine.h"
#include "util.h"

void drawHits(Shader &shader, GLuint &VAO, GLuint &VBO)
{
    unsigned int nVertices = State::hits.size() * 6;
    GLfloat pointVertices[nVertices];
    for (unsigned int i=0; i<State::hits.size(); i++){
        unsigned int pInd = i * 6;
        pointVertices[pInd] = State::hits[i].x;
        pointVertices[pInd + 1] = State::hits[i].y;
        pointVertices[pInd + 2] = State::hits[i].z;
        pointVertices[pInd + 3] = 0.0f;
        pointVertices[pInd + 4] = 0.0f;
        pointVertices[pInd + 5] = 0.0f;
    } 
    bindVertices(VAO, VBO, pointVertices, sizeof(pointVertices));
    drawPointVertices(VAO, nVertices);
}

void drawMouseClicks(Shader &shader, GLuint &VAO, GLuint &VBO)
{
    glm::vec3 cam = mat4xVec3(glm::vec3(), glm::inverse(State::cubeModel), glm::vec3(0.0f, 0.0f, 2.99f));

    for (unsigned int i=0; i<State::mouseClicks.size(); i++){
        GLfloat lineVertices[12] = {
            cam.x, cam.y, cam.z, 1.0f, 1.f, 1.0f, 
            State::mouseClicks[i].x, State::mouseClicks[i].y, State::mouseClicks[i].z,  1.0f, 1.0f, 1.0f
        };
        bindVertices(VAO, VBO, lineVertices, sizeof(lineVertices));
        drawLineVertixes(VAO, 2);
    }
}

#endif