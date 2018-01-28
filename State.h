#ifndef STATE_H
#define STATE_H

#include <vector>
#include <glm/glm.hpp>
#include "RubiksCube.h"

class State {

public:

    static RubiksCube rubiksCube;

    static glm::vec3 cameraPosition;
    static glm::mat4 view;
    static glm::mat4 projection;
    static float screenWidth;
    static float screenHeight;
    static float deltaTime;
    static float lastFrame;
    static bool mouseBtnIsDown;
    static bool faceRotationBtnIsDown;

    static double mouseX;
    static double mouseY;
    static glm::vec3 mouseWorldPos;

    /*** cube movement ***/
    static float cubeLastX;
    static float cubeLastY;
    static bool cubeFirstMouse;

    /*** face movement ***/
    static glm::vec3 X_AXIS;
    static glm::vec3 Y_AXIS;
    static glm::vec3 Z_AXIS;
    static glm::vec3 faceRotationAxis;
    static Axis faceRotationAxisEnum;
    static float faceLastY;
    static bool faceFirstMouse;
};

#endif