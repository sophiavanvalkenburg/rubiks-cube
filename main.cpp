#include <string>
#include <iostream>
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

glm::mat4 cubeModel;
float yaw = 0.0f;
float pitch = 0.0f;

// mouse positions
float lastX = 400;
float lastY = 300;
bool firstMouse = true;

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


void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;

    // escape key closes application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        mouseBtnIsDown = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        mouseBtnIsDown = false;
        firstMouse = true;
        pitch = 0.0f;
        yaw = 0.0f;
    }

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

    if (mouseBtnIsDown){
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;

        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.5f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        if (std::abs(xoffset) < 1)
            xoffset = 0;
        if (std::abs(yoffset) < 1)
            yoffset = 0;

        yaw = glm::radians(xoffset); 
        pitch = glm::radians(yoffset);

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

void loop(GLFWwindow *window, Shader &shader, unsigned int numMiniCubes, SubCube* subcubes, GLuint &VAO)
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
    cubeModel = glm::rotate(cubeModel, pitch, glm::vec3(right.x, right.y, right.z));
    glm::vec4 up = glm::inverse(cubeModel) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    cubeModel = glm::rotate(cubeModel, yaw, glm::vec3(up.x, up.y, up.z)); 

    for (int i=0; i<numMiniCubes; i++){
        glm::mat4 miniCubeModel;
        miniCubeModel = cubeModel * glm::translate(miniCubeModel, subcubes[i].getPosition());
        GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(miniCubeModel));
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
    SubCube* subcubes = cube.getSubCubes();
    unsigned int numMiniCubes = cube.getNumPositions();

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
        loop(window, shader, numMiniCubes, subcubes, VAO);

    // deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // close the window
    glfwTerminate();
    return 0;

}
