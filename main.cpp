#include <string>
#include <iostream>
#include <cmath>
// GLEW - be sure to include it before GLFW b/c it contains the correct OpenGL header includes
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
// glm is header only library so don't need to link it
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

using std::cout;
using std::endl;
using std::string;
using std::min;
using std::max;
using std::sin;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool mouseBtnIsDown = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 

glm::mat4 model;
float yaw = 0.0f;
float pitch = 0.0f;

// mouse positions
float lastX = 400;
float lastY = 300;
bool firstMouse = true;

float fov = 45.0f;

GLfloat textureMixAmount = 0.2f;
float screenWidth = 800.0f;
float screenHeight = 600.0f;

void initTexture(GLuint &texture, GLenum textureUnit, GLenum wrapType)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadImage(GLuint &texture, const string& imageFilePath, GLenum pixelDataFormat)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(imageFilePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelDataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
	std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

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
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
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

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.1f, 0.1f, 0.1f, 
        0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        0.5f,  0.5f, -0.5f, 0.1f, 0.1f, 0.1f,
        0.5f,  0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f,  0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,

        -0.5f, -0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        0.5f, -0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f,  0.5f,  0.5f, 0.1f, 0.1f, 0.1f,
        -0.5f, -0.5f,  0.5f, 0.1f, 0.1f, 0.1f,

        -0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f,  0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f, -0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,

        0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        0.5f,  0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        0.5f, -0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,

        -0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        0.5f, -0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        0.5f, -0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        0.5f, -0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f, -0.5f,  0.5f, 0.1f, 0.1f, 0.1f,
        -0.5f, -0.5f, -0.5f, 0.1f, 0.1f, 0.1f,

        -0.5f,  0.5f, -0.5f, 0.1f, 0.1f, 0.1f,
        0.5f,  0.5f, -0.5f,  0.1f, 0.1f, 0.1f,
        0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        0.5f,  0.5f,  0.5f,  0.1f, 0.1f, 0.1f,
        -0.5f,  0.5f,  0.5f, 0.1f, 0.1f, 0.1f,
        -0.5f,  0.5f, -0.5f,  0.1f, 0.1f, 0.1f
    };

    // create buffer for vertex shader
    GLuint VBO; // vertex buffer objects
    glGenBuffers(1, &VBO);
    // create a vertex array object to switch between
    // vertex data and attribute configurations more easily
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    bindVertices(VAO, VBO, vertices, sizeof(vertices));

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // the "game loop"
    while(!glfwWindowShouldClose(window))
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
        // note that we're translating the scene in the reverse direction of where we want to move
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // projection matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), screenWidth/screenHeight, 0.1f, 100.0f);

        GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    	// model matrix
        glm::vec4 right = glm::inverse(model) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        model = glm::rotate(model, pitch, glm::vec3(right.x, right.y, right.z));
	glm::vec4 up = glm::inverse(model) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        model = glm::rotate(model, yaw, glm::vec3(up.x, up.y, up.z)); 
        GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // draw the triangle
        drawVertices(shader.Program, VAO);

        // uses double buffering to prevent flickering images
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // close the window
    glfwTerminate();
    return 0;

}
