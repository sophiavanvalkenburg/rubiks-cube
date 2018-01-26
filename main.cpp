#include <string>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "RubiksCube.h"
#include "CubeModel.h"
#include "util.h"
#include "RenderEngine.h"
#include "State.h"
#include "Window.h"
#include "Debug.h"
#include "InputHandler.h"


float normalizedDeviceCoord(float coord, float maxCoord, float dir){
    // coord: mouse x or y coord
    // maxCoord: screen width or height
    // dir: 1 or -1
    return dir * ( 2 * coord / maxCoord - 1.0f );
}

glm::vec3 screenSpaceToWorldSpace(double xpos, double ypos){
    // convert mouse to clip coords
    float ndcX = normalizedDeviceCoord(xpos, State::screenWidth, 1.0f);
    float ndcY = normalizedDeviceCoord(ypos, State::screenHeight, -1.0f);
    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    // convert clip to camera coords
    glm::vec4 camCoords = glm::inverse(State::projection) * clipCoords;
    camCoords = glm::vec4(camCoords.x, camCoords.y, -1.0f, 0.0f);
    // convert camera to world coords
    glm::vec4 worldCoordsTmp = glm::inverse(State::view) * camCoords;
    //glm::vec3 worldCoords = glm::normalize(glm::vec3(worldCoordsTmp.x / worldCoordsTmp.w, worldCoordsTmp.y / worldCoordsTmp.w, worldCoordsTmp.z / worldCoordsTmp.w));
    glm::vec3 worldCoords = glm::normalize(glm::vec3(worldCoordsTmp.x, worldCoordsTmp.y, worldCoordsTmp.z));
    return worldCoords;
}

float getNearestValidAngle(float angle){
    return (M_PI_4 / 2 ) * std::round(2 * angle / M_PI_4);
}

bool intersectPlane(glm::vec3 &out, glm::vec3 origin, glm::vec3 intersectRay, glm::vec3 planeNormal, float planeDistance)
{
    // origin: camera position
    // intersectRay: direction of ray from origin
    // planeNormal: direction of plane (collision direction)
    // planeDistance: distance from plane to origin (will be negative since planeNormal is pointing away from plane)
    float rayAndNormalDotProduct = glm::dot(intersectRay, planeNormal);
    if (std::abs(rayAndNormalDotProduct) <= MIN_EPSILON) return false;
    float originAndNormalDotProduct = glm::dot(origin, planeNormal);
    float intersectDistance = -1 * (originAndNormalDotProduct + planeDistance) / rayAndNormalDotProduct;
    if (intersectDistance < 0) return false;
    glm::vec3 intersectPoint = origin + glm::vec3(intersectRay.x * intersectDistance, intersectRay.y * intersectDistance, intersectRay.z * intersectDistance);
    copyVec3(out, intersectPoint);
    return true;
}


bool intersectSubCube(glm::vec3 &out, SubCube *subcube, glm::mat4 rotationMatrix, glm::vec3 origin, glm::vec3 intersectRay, float planeDistance)
{
    const std::vector<glm::vec3> normals = CubeModel::getFaceNormals();
    const std::vector<glm::vec3> facesMinMax = CubeModel::getFacesMinMax();
    glm::vec3 intersectPoint;
    for (unsigned int i=0; i<normals.size(); i++){
        glm::vec3 normal = mat4xVec3(glm::vec3(), rotationMatrix, normals[i]);
        if (!intersectPlane(intersectPoint, origin, intersectRay, normal, planeDistance)) continue;
        copyVec3(out, intersectPoint);
        glm::vec3 max = facesMinMax[2 * i];
        glm::vec3 min = facesMinMax[2 * i + 1];
        glm::vec3 intersectPointInv = mat4xVec3(glm::vec3(), glm::inverse(subcube->modelMatrix), intersectPoint);
        if (lte(intersectPointInv.x, max.x) && lte(intersectPointInv.y, max.y) && lte(intersectPointInv.z, max.z) &&
          gte(intersectPointInv.x, min.x) && gte(intersectPointInv.y, min.y) && gte(intersectPointInv.z, min.z)) 
        {
            return true;
        }
    }
    return false;
}


bool testIntersectSubcube(glm::vec3 &out, SubCube *subcube, glm::mat4 rotationMatrix, glm::vec3 origin, float planeDistance)
{
    glm::vec3 intersectRay;
    copyVec3(intersectRay, State::mouseWorldPos);
    bool hit = intersectSubCube(out, subcube, rotationMatrix, origin, intersectRay, planeDistance);

    hits.push_back(out);
    if (hit){
        printVec3(out);
        std::cout << "HIT" << std::endl;
    }
    return hit;
}

void updateCubeMatrix()
{
    glm::vec3 right = mat4xVec3(glm::vec3(), State::rubiksCube.viewMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 up = mat4xVec3(glm::vec3(), State::rubiksCube.viewMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    State::rubiksCube.modelMatrix = glm::rotate(State::rubiksCube.modelMatrix, State::rubiksCube.pitchAngle, right);
    State::rubiksCube.modelMatrix = glm::rotate(State::rubiksCube.modelMatrix, State::rubiksCube.yawAngle, up);
    State::rubiksCube.viewMatrix = glm::inverse(State::rubiksCube.modelMatrix);
}

void drawSubCube(Shader &shader, SubCube *subcube, unsigned int subcubeId, GLuint &VAO)
{
    GLfloat isSelectedVal = subcube->isSelected ? 1.0f : 0.0f;
    GLuint isSelectedLoc = glGetUniformLocation(shader.Program, "isSelected");
    glUniform1f(isSelectedLoc, isSelectedVal);
    GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(subcube->modelMatrix));
    drawCubeVertices(VAO, 36);
}

void setSubCubeTransformationMatrix(glm::mat4 &out, SubCube *subcube, unsigned int subcubeId, glm::mat4 subcubeModelMatrix, glm::mat4 &rotationMatrix){
    glm::mat4 transformMatrix = glm::mat4();
    glm::mat4 subcubeTranslateMatrix = glm::translate(transformMatrix, subcube->getPosition());
    if (State::rubiksCube.faceContainsSubCube(0, subcubeId)){
        glm::vec3 faceCenter = State::rubiksCube.getFaceCenter(0);
        glm::mat4 translateSubCubeMatrix;
        glm::mat4 inverseTranslateSubCubeMatrix;
        translateSubCubeMatrix = glm::translate(translateSubCubeMatrix, faceCenter);
        inverseTranslateSubCubeMatrix = glm::inverse(translateSubCubeMatrix);
        rotationMatrix = State::rubiksCube.modelMatrix * inverseTranslateSubCubeMatrix * subcubeModelMatrix * translateSubCubeMatrix; 
        transformMatrix = rotationMatrix * subcubeTranslateMatrix;
        copyMat4(out, transformMatrix);
    } else {
        rotationMatrix = State::rubiksCube.modelMatrix;
        transformMatrix = State::rubiksCube.modelMatrix * subcubeTranslateMatrix;
        copyMat4(out, transformMatrix);
    }
}

void drawCubes(Shader &shader, GLuint &VAO, GLuint &VBO, const size_t cubeVerticesSize, const GLfloat *cubeVertices)
{
    bindVertices(VAO, VBO, cubeVertices, cubeVerticesSize);
    updateCubeMatrix();

    // plane distance works since we don't need to intersect the inside of the cube
    float planeDistance = -1.0f * (CubeModel::getCubeSideLengths().z + State::rubiksCube.getSubCubeMargin());
    glm::vec3 origin = State::cameraPosition;
    std::vector<SubCube*>* subcubes = State::rubiksCube.getSubCubes();
    float faceRotationAngle = getNearestValidAngle(State::faceRotationAngle);
    float shortestLength = 100.0f;
    SubCube *closestSelectedSubCube = NULL;
    for (int i=0; i<subcubes->size(); i++){
        SubCube *subcube = (*subcubes)[i];
        subcube->setRotationOnAxis(faceRotationAngle, State::faceRotationAxisEnum);
        glm::mat4 localRotationMatrix = subcube->getRotationMatrix();
        // keep the subcube selected if you're moving the face
        if (!State::faceRotationBtnIsDown || (State::faceRotationBtnIsDown && !subcube->isSelected)) subcube->isSelected = false;
        glm::mat4 rotationMatrix;
        setSubCubeTransformationMatrix(subcube->modelMatrix, subcube, i, localRotationMatrix, rotationMatrix); 
        glm::vec3 intersectPoint;
        if (!State::faceRotationBtnIsDown){
            if (testIntersectSubcube(intersectPoint, subcube, rotationMatrix, origin, planeDistance)){
                float length = glm::length(origin - intersectPoint);
                if (length < shortestLength){
                    shortestLength = length;
                    closestSelectedSubCube = subcube;
                }
            }
        } else {
            
        }
    }
    if (!State::faceRotationBtnIsDown && closestSelectedSubCube) closestSelectedSubCube->isSelected = true;
    for (int i=0; i<subcubes->size(); i++){
        drawSubCube(shader, (*subcubes)[i], i, VAO);
    }
}

void beginLoop(GLFWwindow *window) 
{
    float currentFrame = glfwGetTime();
    State::deltaTime = currentFrame - State::lastFrame;
    State::lastFrame = currentFrame;

    processInput(window);

    // makes the background teal
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // need glClear to flush the color buffer because 
    // it does not clear the buffer automatically 
    // so you get a weird image
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void endLoop(GLFWwindow *window)
{
    // uses double buffering to prevent flickering images
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main()
{
    GLFWwindow* window = initWindow();
    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    // if glewExperimental = GL_FALSE, maybe OpenGL will be wonky?
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    setWindowParams(window);

    glEnable(GL_DEPTH_TEST);

    Shader shader("vertex.vs", "fragment.fs");

    // create buffer for vertex shader
    GLuint VBO; // vertex buffer objects
    glGenBuffers(1, &VBO);
    // create a vertex array object to switch between
    // vertex data and attribute configurations more easily
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    shader.Use();

    GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(State::view));

    GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(State::projection));

    const size_t cubeVerticesSize = CubeModel::getSizeOfVertices();
    const GLfloat *cubeVertices = CubeModel::getVertices();

    // the "game loop"
    while(!glfwWindowShouldClose(window)){
        beginLoop(window);
        State::mouseWorldPos = screenSpaceToWorldSpace(State::mouseX, State::mouseY);
        drawCubes(shader, VAO, VBO, cubeVerticesSize, cubeVertices);
        endLoop(window);
    }

    // deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // close the window
    glfwTerminate();
    return 0;

}
