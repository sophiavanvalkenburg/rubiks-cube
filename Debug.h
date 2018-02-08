#ifndef DEBUG_H
#define DEBUG_H

#include "shader.h"
#include "State.h"
#include "RenderEngine.h"
#include "Util.h"
#include "RubiksCube.h"

std::vector<glm::vec3> mouseClicks;
std::vector<glm::vec3> hits;

void setSubCubeTapped()
{
    std::vector<SubCube*> *subcubes = State::rubiksCube.getSubCubes();
    SubCube *selectedSubCube = (*subcubes)[State::rubiksCube.getSelectedSubCubeId()];
    selectedSubCube->setIsTapped(!selectedSubCube->getIsTapped());
}

void drawHits(Shader &shader, GLuint &VAO, GLuint &VBO)
{
    unsigned int nVertices = hits.size() * 6;
    GLfloat pointVertices[nVertices];
    for (unsigned int i=0; i<hits.size(); i++){
        unsigned int pInd = i * 6;
        pointVertices[pInd] = hits[i].x;
        pointVertices[pInd + 1] = hits[i].y;
        pointVertices[pInd + 2] = hits[i].z;
        pointVertices[pInd + 3] = 0.0f;
        pointVertices[pInd + 4] = 0.0f;
        pointVertices[pInd + 5] = 0.0f;
    } 
    bindVertices(VAO, VBO, pointVertices, sizeof(pointVertices));
    drawPointVertices(VAO, nVertices);
}

void drawMouseClicks(Shader &shader, GLuint &VAO, GLuint &VBO)
{
    glm::vec3 origin;
    Util::copyVec3(origin, State::cameraPosition);
    origin.z -= 0.01; // slightly in front of camera so we can see it
    glm::vec3 cam = Util::mat4xVec3(glm::vec3(), State::rubiksCube.getViewMatrix(), origin);

    for (unsigned int i=0; i<mouseClicks.size(); i++){
        GLfloat lineVertices[12] = {
            cam.x, cam.y, cam.z, 1.0f, 1.f, 1.0f, 
            mouseClicks[i].x, mouseClicks[i].y, mouseClicks[i].z,  1.0f, 1.0f, 1.0f
        };
        bindVertices(VAO, VBO, lineVertices, sizeof(lineVertices));
        drawLineVertixes(VAO, 2);
    }
}

#endif