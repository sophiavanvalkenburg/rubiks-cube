#include "State.h"
#include <glm/glm.hpp>

std::vector<glm::vec3> State::mouseClicks;
std::vector<glm::vec3> State::hits;

float State::deltaTime = 0.0f;
float State::lastFrame = 0.0f;

/*** input ***/
double State::mouseX;
double State::mouseY;
glm::vec3 State::mouseWorldPos;
bool State::mouseBtnIsDown = false;
bool State::faceRotationBtnIsDown = false;

/*** cube movement ***/
glm::mat4 State::cubeModel;
float State::cubeLastX = 400;
float State::cubeLastY = 300;
bool State::cubeFirstMouse = true;
float State::cubeYaw = 0.0f;
float State::cubePitch = 0.0f;

/*** face movement ***/
glm::vec3 State::X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 State::Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 State::Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 State::faceRotationAxis = State::X_AXIS;
float State::faceRotationAngle = 0.0f;
float State::faceLastY = 300;
bool State::faceFirstMouse = true;