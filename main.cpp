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


bool intersectSubCube(glm::vec3 &out, glm::vec3 origin, glm::vec3 intersectRay, glm::mat4 transformMatrix, glm::mat4 rotationMatrix, float planeDistance)
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
        glm::vec3 intersectPointInv = mat4xVec3(glm::vec3(), glm::inverse(transformMatrix), intersectPoint);
        if (lte(intersectPointInv.x, max.x) && lte(intersectPointInv.y, max.y) && lte(intersectPointInv.z, max.z) &&
          gte(intersectPointInv.x, min.x) && gte(intersectPointInv.y, min.y) && gte(intersectPointInv.z, min.z)) 
        {
            return true;
        }
    }
    return false;
}

/*
SubCube getIntersectedSubCube(glm::vec3 mouseWorldPos){
    std::vector<glm::vec3> planes = {
        glm::vec3()   
    }    
}
*/

bool testIntersectPlane(glm::vec3 intersectRay, glm::mat4 transformMatrix, glm::mat4 rotationMatrix, float planeDistance)
{
    glm::vec3 testPoint;
    bool hit = intersectSubCube(testPoint, State::cameraPosition, intersectRay, transformMatrix, rotationMatrix, planeDistance);
    hits.push_back(testPoint);
    if (hit){
        printVec3(testPoint);
        std::cout << "HIT" << std::endl;
    } else {
        //std::cout << "NO HIT" << std::endl;
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

void drawSubCube(Shader &shader, SubCube &subcube, unsigned int subcubeId, glm::mat4 subcubeModelMatrix, GLuint &VAO)
{
    glm::vec3 intersectRay;
    copyVec3(intersectRay, State::mouseWorldPos);
    glm::mat4 transformSubCubeMatrix;
    glm::mat4 rotationMatrix;
    glm::mat4 subcubeTranslateMatrix = glm::translate(transformSubCubeMatrix, subcube.getPosition());
    if (State::rubiksCube.faceContainsSubCube(0, subcubeId)){
        glm::vec3 faceCenter = State::rubiksCube.getFaceCenter(0);
        glm::mat4 translateSubCubeMatrix;
        glm::mat4 inverseTranslateSubCubeMatrix;
        translateSubCubeMatrix = glm::translate(translateSubCubeMatrix, faceCenter);
        inverseTranslateSubCubeMatrix = glm::inverse(translateSubCubeMatrix);
        rotationMatrix = State::rubiksCube.modelMatrix * inverseTranslateSubCubeMatrix * subcubeModelMatrix * translateSubCubeMatrix; 
        transformSubCubeMatrix = rotationMatrix * subcubeTranslateMatrix;
    } else {
        rotationMatrix = State::rubiksCube.modelMatrix;
        transformSubCubeMatrix = State::rubiksCube.modelMatrix * subcubeTranslateMatrix;
    }
    // plane distance works since we don't need to intersect the inside of the cube
    float planeDistance = -1.0f * (CubeModel::getCubeSideLengths().z + State::rubiksCube.getSubCubeMargin());
    bool hit = testIntersectPlane(intersectRay, transformSubCubeMatrix, rotationMatrix, planeDistance);
    GLfloat hitVal = hit ? 1.0f : 0.0f;
    GLuint isSelectedLoc = glGetUniformLocation(shader.Program, "isSelected");
    glUniform1f(isSelectedLoc, hitVal);
    GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformSubCubeMatrix));
    // draw the mini cub
    drawCubeVertices(VAO, 36);
}

void drawCubes(Shader &shader, GLuint &VAO, GLuint &VBO, const size_t cubeVerticesSize, const GLfloat *cubeVertices)
{
    bindVertices(VAO, VBO, cubeVertices, cubeVerticesSize);
    updateCubeMatrix();
    glm::mat4 subcubeModelMatrix;
    subcubeModelMatrix = glm::rotate(subcubeModelMatrix, getNearestValidAngle(State::faceRotationAngle), State::faceRotationAxis);
    std::vector<SubCube> subcubes = State::rubiksCube.getSubCubes();
    for (int i=0; i<subcubes.size(); i++){
        SubCube subcube = subcubes[i];
        drawSubCube(shader, subcube, i, subcubeModelMatrix, VAO);
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
