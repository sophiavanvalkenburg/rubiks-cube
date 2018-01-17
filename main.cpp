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

glm::vec3 createWorldRay(double xpos, double ypos, glm::mat4 projection, glm::mat4 view){
    // convert mouse to clip coords
    float ndcX = normalizedDeviceCoord(xpos, State::screenWidth, 1.0f);
    float ndcY = normalizedDeviceCoord(ypos, State::screenHeight, -1.0f);
    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    // convert clip to camera coords
    glm::vec4 camCoords = glm::inverse(projection) * clipCoords;
    camCoords = glm::vec4(camCoords.x, camCoords.y, -1.0f, 0.0f);
    // convert camera to world coords
    glm::vec4 worldCoordsTmp = glm::inverse(view) * camCoords;
    //glm::vec3 worldCoords = glm::normalize(glm::vec3(worldCoordsTmp.x / worldCoordsTmp.w, worldCoordsTmp.y / worldCoordsTmp.w, worldCoordsTmp.z / worldCoordsTmp.w));
    glm::vec3 worldCoords = glm::normalize(glm::vec3(worldCoordsTmp.x, worldCoordsTmp.y, worldCoordsTmp.z));
    return worldCoords;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos){

    State::mouseX = xpos;
    State::mouseY = ypos;

    if (State::mouseBtnIsDown){
        if (State::cubeFirstMouse)
        {
            State::cubeLastX = xpos;
            State::cubeLastY = ypos;
            State::cubeFirstMouse = false;
        }

        float xoffset = calculatePositionOffset(xpos, State::cubeLastX);
        float yoffset = calculatePositionOffset(ypos, State::cubeLastY);

        State::cubeLastX = xpos;
        State::cubeLastY = ypos;

        State::cubeYaw = glm::radians(xoffset); 
        State::cubePitch = glm::radians(yoffset);

    } else if (State::faceRotationBtnIsDown){
       if (State::faceFirstMouse){
           State::faceLastY = ypos;
           State::faceFirstMouse = false;
       }
       float yoffset = calculatePositionOffset(ypos, State::faceLastY);
       State::faceLastY = ypos;
       State::faceRotationAngle += glm::radians(yoffset);
    }
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
    planeNormal = mat4xVec3(glm::vec3(), State::cubeModel, planeNormal);
    float rayAndNormalDotProduct = glm::dot(intersectRay, planeNormal);
    if (std::abs(rayAndNormalDotProduct) <= MIN_EPSILON) return false;
    float originAndNormalDotProduct = glm::dot(origin, planeNormal);
    float intersectDistance = -1 * (originAndNormalDotProduct + planeDistance) / rayAndNormalDotProduct;
    if (intersectDistance < 0) return false;
    glm::vec3 intersectPoint = origin + glm::vec3(intersectRay.x * intersectDistance, intersectRay.y * intersectDistance, intersectRay.z * intersectDistance);
    copyVec3(out, intersectPoint);
    return true;
}


bool intersectSubCube(glm::vec3 &out, glm::vec3 origin, glm::vec3 intersectRay)
{
    const std::vector<glm::vec3> normals = CubeModel::getFaceNormals();
    const std::vector<glm::vec3> facesMinMax = CubeModel::getFacesMinMax();
    glm::vec3 intersectPoint;
    for (unsigned int i=0; i<normals.size(); i++){
        glm::vec3 normal = normals[i];
        if (!intersectPlane(intersectPoint, origin, intersectRay, normal, CubeModel::boundingMin.z)) continue;
        // test bounds for each pane
        glm::vec3 max = facesMinMax[2 * i];
        glm::vec3 min = facesMinMax[2 * i + 1];
        glm::vec3 intersectPointInv = mat4xVec3(glm::vec3(), glm::inverse(State::cubeModel), intersectPoint);
        if (lte(intersectPointInv.x, max.x) && lte(intersectPointInv.y, max.y) && lte(intersectPointInv.z, max.z) &&
          gte(intersectPointInv.x, min.x) && gte(intersectPointInv.y, min.y) && gte(intersectPointInv.z, min.z)) 
        {
            copyVec3(out, intersectPoint);
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

void testIntersectPlane()
{
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
    
    glm::vec3 intersectRay;
    copyVec3(intersectRay, State::mouseWorldPos);
    glm::vec3 testPoint;
    bool hit = intersectSubCube(testPoint, camPos, intersectRay);
    if (hit){
        std::cout << "HIT" << std::endl;
        State::hits.push_back(testPoint);
    } else {
        std::cout << "NO HIT" << std::endl;
    }
}

void drawCubes(Shader &shader, RubiksCube &cube, glm::mat4 view, glm::mat4 projection, GLuint &VAO, GLuint &VBO)
{
    size_t cubeVerticesSize = CubeModel::getSizeOfVertices();
    const GLfloat *cubeVertices = CubeModel::getVertices();
    bindVertices(VAO, VBO, cubeVertices, cubeVerticesSize);

    // model matrix

    glm::mat4 cubeModelInv = glm::inverse(State::cubeModel);
    glm::vec3 right = mat4xVec3(glm::vec3(), cubeModelInv, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 up = mat4xVec3(glm::vec3(), cubeModelInv, glm::vec3(0.0f, 1.0f, 0.0f));
    State::cubeModel = glm::rotate(State::cubeModel, State::cubePitch, right);
    State::cubeModel = glm::rotate(State::cubeModel, State::cubeYaw, up); 

    std::vector<SubCube> subcubes = cube.getSubCubes();

    glm::mat4 subcubeModel;
    subcubeModel = glm::rotate(subcubeModel, getNearestValidAngle(State::faceRotationAngle), State::faceRotationAxis);

    for (int i=0; i<subcubes.size(); i++){
        SubCube subcube = subcubes[i];
        glm::mat4 transformSubCubeModel;
        glm::mat4 subcubePositionModel = glm::translate(transformSubCubeModel, subcube.getPosition());
        if (cube.faceContainsSubCube(0, i)){
            glm::vec3 faceCenter = cube.getFaceCenter(0);
            glm::mat4 translateSubCubeModel;
            glm::mat4 inverseTranslateSubCubeModel;
            translateSubCubeModel = glm::translate(translateSubCubeModel, faceCenter);
            inverseTranslateSubCubeModel = glm::inverse(translateSubCubeModel);
            transformSubCubeModel = State::cubeModel * inverseTranslateSubCubeModel * subcubeModel * translateSubCubeModel * subcubePositionModel;
        } else {
            transformSubCubeModel = State::cubeModel * subcubePositionModel;
        }
        testIntersectPlane();
        GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformSubCubeModel));
        // draw the mini cube
        drawCubeVertices(VAO, 36);
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
    RubiksCube cube;

    // create buffer for vertex shader
    GLuint VBO; // vertex buffer objects
    glGenBuffers(1, &VBO);
    // create a vertex array object to switch between
    // vertex data and attribute configurations more easily
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

        // the "game loop"
    while(!glfwWindowShouldClose(window)){
        beginLoop(window);
        // view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), State::screenWidth/State::screenHeight, 0.1f, 100.0f);

        shader.Use();

        GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        State::mouseWorldPos = createWorldRay(State::mouseX, State::mouseY, projection, view);

        drawCubes(shader, cube, view, projection, VAO, VBO);
        endLoop(window);
    }

    // deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // close the window
    glfwTerminate();
    return 0;

}
