#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "State.h"
#include "RubiksCube.h"
#include "Util.h"
#include "Debug.h"

void setRotationAxis(GLFWwindow* window){

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        State::faceRotationAxis = State::X_AXIS;
        State::faceRotationAxisEnum = Axis::X;
        State::faceRotationBtnIsDown = true;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        State::faceRotationAxis = State::Y_AXIS;
        State::faceRotationAxisEnum = Axis::Y;
        State::faceRotationBtnIsDown = true;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        State::faceRotationAxis = State::Z_AXIS;
        State::faceRotationAxisEnum = Axis::Z;
        State::faceRotationBtnIsDown = true;
    } else {
        State::faceRotationBtnIsDown = false;
        State::faceFirstMouse = true;
    }
}

void resetAll()
{
    State::rubiksCube.modelMatrix = glm::mat4();
    mouseClicks = std::vector<glm::vec3>();
    hits = std::vector<glm::vec3>();
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * State::deltaTime;

    // escape key closes application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        resetAll(); 

    setRotationAxis(window);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        State::mouseBtnIsDown = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        State::mouseBtnIsDown = false;
        State::cubeFirstMouse = true;
        State::rubiksCube.pitchAngle = 0.0f;
        State::rubiksCube.yawAngle = 0.0f;

        mouseClicks.push_back(Util::mat4xVec3(glm::vec3(), State::rubiksCube.viewMatrix, State::mouseWorldPos));
    }

}

float calculatePositionOffset(float newPos, float oldPos){

    float offset = newPos - oldPos;
    float sensitivity = 0.5f;
    offset *= sensitivity;

    if (std::abs(offset) < 1)
        offset = 0;

    return offset;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos){

    State::mouseX = xpos;
    State::mouseY = ypos;

    if (State::mouseBtnIsDown){
        if (State::faceRotationBtnIsDown){
            if (State::faceFirstMouse){
                State::faceLastY = ypos;
                State::faceFirstMouse = false;
                State::rubiksCube.updateSubCubePositions();
            }
            float yoffset = calculatePositionOffset(ypos, State::faceLastY);
            State::faceLastY = ypos;
            State::rubiksCube.updateSubCubeRotationAngles(State::faceRotationAxisEnum, glm::radians(yoffset));
        } else {
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

            State::rubiksCube.yawAngle = glm::radians(xoffset); 
            State::rubiksCube.pitchAngle = glm::radians(yoffset);
        }
    } 
}

#endif