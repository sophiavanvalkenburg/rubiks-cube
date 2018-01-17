#ifndef STATE_H
#define STATE_H

#include <vector>
#include <glm/glm.hpp>

class State {

public:

    static std::vector<glm::vec3> mouseClicks;
    static std::vector<glm::vec3> hits;

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
    static glm::mat4 cubeModel;
    static float cubeLastX;
    static float cubeLastY;
    static bool cubeFirstMouse;
    static float cubeYaw;
    static float cubePitch;

    /*** face movement ***/
    static glm::vec3 X_AXIS;
    static glm::vec3 Y_AXIS;
    static glm::vec3 Z_AXIS;
    static glm::vec3 faceRotationAxis;
    static float faceRotationAngle;
    static float faceLastY;
    static bool faceFirstMouse;
};

#endif