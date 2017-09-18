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
#include "cube.h"

using std::cout;
using std::endl;
using std::string;
using std::min;
using std::max;
using std::sin;
using std::abs;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool mouseBtnIsDown = false;
bool faceRotationBtnIsDown = false;

/*** cube movement ***/
glm::mat4 cubeModel;
float cubeLastX = 400;
float cubeLastY = 300;
bool cubeFirstMouse = true;
float cubeYaw = 0.0f;
float cubePitch = 0.0f;

/*** face movement ***/
glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 faceRotationAxis = X_AXIS;
float faceRotationAngle = 0.0f;
float faceLastY = 300;
bool faceFirstMouse = true;

float screenWidth = 800.0f;
float screenHeight = 600.0f;

void bindVertices(GLuint &VAO, GLuint &VBO, GLfloat *vertices, size_t vertices_sz) 
{
    // bind the VAO
    glBindVertexArray(VAO);
    // send vertex data to vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_sz, vertices, GL_STATIC_DRAW);
    // tell OpenGL how to interpret the vertex data
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Texture attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // unbind the VBO - we can do this because it was registered as VBO
    // when we called glVertexAttribPointer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind the VAO
    glBindVertexArray(0);

}

void drawVertices(GLuint &shaderProgram, GLuint &VAO)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void setRotationAxis(GLFWwindow* window){

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        faceRotationAxis = X_AXIS;
        faceRotationBtnIsDown = true;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        faceRotationAxis = Y_AXIS;
        faceRotationBtnIsDown = true;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        faceRotationAxis = Z_AXIS;
        faceRotationBtnIsDown = true;
    } else {
        faceRotationBtnIsDown = false;
        faceFirstMouse = true;
    }
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;

    // escape key closes application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    setRotationAxis(window);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        mouseBtnIsDown = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        mouseBtnIsDown = false;
        cubeFirstMouse = true;
        cubePitch = 0.0f;
        cubeYaw = 0.0f;
    }

}

float calculateOffset(float newPos, float oldPos){

    float offset = newPos - oldPos;
    float sensitivity = 0.5f;
    offset *= sensitivity;

    if (std::abs(offset) < 1)
        offset = 0;

    return offset;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

    if (mouseBtnIsDown){
        if (cubeFirstMouse)
        {
            cubeLastX = xpos;
            cubeLastY = ypos;
            cubeFirstMouse = false;
        }

        float xoffset = calculateOffset(xpos, cubeLastX);
        float yoffset = calculateOffset(ypos, cubeLastY);

        cubeLastX = xpos;
        cubeLastY = ypos;

        cubeYaw = glm::radians(xoffset); 
        cubePitch = glm::radians(yoffset);

    } else if (faceRotationBtnIsDown){
       if (faceFirstMouse){
           faceLastY = ypos;
           faceFirstMouse = false;
       }
       float yoffset = calculateOffset(ypos, faceLastY);
       faceLastY = ypos;
       faceRotationAngle += glm::radians(yoffset);
    }
}

GLFWwindow* initWindow()
{
    // initialize GLFW and add window hints which will configure the window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // can use 3.3 with OSX 10.9+
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // required for mac
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // creating the window
    // nullptr is c++11 -- for earlier versions use NULL
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Rubik's Cube", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    return window;
}

void setWindowParams(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

float getNearestValidAngle(float angle){
    return (M_PI_4 / 2 ) * std::round(2 * angle / M_PI_4);
}

void loop(GLFWwindow *window, Shader &shader, Cube& cube, GLuint &VAO)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    // makes the background teal
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // need glClear to flush the color buffer because 
    // it does not clear the buffer automatically 
    // so you get a weird image
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    shader.Use();

    // view matrix
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), screenWidth/screenHeight, 0.1f, 100.0f);

    GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // model matrix
    glm::vec4 right = glm::inverse(cubeModel) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    cubeModel = glm::rotate(cubeModel, cubePitch, glm::vec3(right.x, right.y, right.z));
    glm::vec4 up = glm::inverse(cubeModel) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    cubeModel = glm::rotate(cubeModel, cubeYaw, glm::vec3(up.x, up.y, up.z)); 

    std::vector<SubCube> subcubes = cube.getSubCubes();

    glm::mat4 subcubeModel;
    subcubeModel = glm::rotate(subcubeModel, getNearestValidAngle(faceRotationAngle), faceRotationAxis);
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
            transformSubCubeModel = cubeModel * inverseTranslateSubCubeModel * subcubeModel * translateSubCubeModel * subcubePositionModel;
        } else {
            transformSubCubeModel = cubeModel * subcubePositionModel;
        }
        GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformSubCubeModel));
        // draw the mini cube
        drawVertices(shader.Program, VAO);
    }
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
    Cube cube;

    GLfloat *vertices = cube.getVertices();

    // create buffer for vertex shader
    GLuint VBO; // vertex buffer objects
    glGenBuffers(1, &VBO);
    // create a vertex array object to switch between
    // vertex data and attribute configurations more easily
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    bindVertices(VAO, VBO, vertices, cube.getSizeOfVertices());

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // the "game loop"
    while(!glfwWindowShouldClose(window))
        loop(window, shader, cube, VAO);

    // deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // close the window
    glfwTerminate();
    return 0;

}
