#include "State.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RubiksCube.h"

RubiksCube State::rubiksCube = RubiksCube();

float State::screenWidth = 800.0f;
float State::screenHeight = 600.0f;

glm::vec3 State::cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
// view matrix
glm::mat4 State::view = glm::lookAt(State::cameraPosition, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
// projection matrix
glm::mat4 State::projection = glm::perspective(glm::radians(45.0f), State::screenWidth/State::screenHeight, 0.1f, 100.0f);

float State::deltaTime = 0.0f;
float State::lastFrame = 0.0f;

/*** input ***/
double State::mouseX;
double State::mouseY;
glm::vec3 State::mouseWorldPos;
bool State::mouseBtnIsDown = false;
bool State::faceRotationBtnIsDown = false;

/*** cube movement ***/
float State::cubeLastX = 400;
float State::cubeLastY = 300;
bool State::cubeFirstMouse = true;

/*** face movement ***/
glm::vec3 State::X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 State::Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 State::Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 State::faceRotationAxis = State::X_AXIS;
Axis State::faceRotationAxisEnum = Axis::X;
float State::faceRotationAngle = 0.0f;
float State::faceLastY = 300;
bool State::faceFirstMouse = true;