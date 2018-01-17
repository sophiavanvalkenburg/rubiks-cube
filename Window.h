#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include "State.h"

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
    GLFWwindow* window = glfwCreateWindow(State::screenWidth, State::screenHeight, "Rubik's Cube", nullptr, nullptr);
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

#endif