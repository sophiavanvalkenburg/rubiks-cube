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


std::vector<glm::vec3> mouseClicks;
std::vector<glm::vec3> hits;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool mouseBtnIsDown = false;
bool faceRotationBtnIsDown = false;

double mouseX;
double mouseY;
float MIN_EPSILON = 1e-6;
glm::vec3 mouseWorldPos;

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

void copyVec3(glm::vec3 &out, glm::vec3 &v){
    out.x = v.x;
    out.y = v.y;
    out.z = v.z;
}

void printVec3(glm::vec3 &v){
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

void bindVertices(GLuint &VAO, GLuint &VBO, const GLfloat *vertices, size_t vertices_sz) 
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

void drawCubeVertices(GLuint &VAO)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void drawLineVertixes(GLuint &VAO)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    glBindVertexArray(0);
}

void drawPointVertices(GLuint &VAO, GLuint nVertices)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, nVertices);
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

void resetAll()
{
    cubeModel = glm::mat4();
    mouseClicks = std::vector<glm::vec3>();
    hits = std::vector<glm::vec3>();
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;

    // escape key closes application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        resetAll(); 

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

        glm::vec4 mouseClick4 = glm::inverse(cubeModel) * glm::vec4(mouseWorldPos.x, mouseWorldPos.y, mouseWorldPos.z, 1.0f) ;
        mouseClicks.push_back(glm::vec3(mouseClick4.x / mouseClick4.w, mouseClick4.y / mouseClick4.w, mouseClick4.z / mouseClick4.w));
        //mouseClicks.push_back(glm::vec3(mouseWorldPos.x, mouseWorldPos.y, mouseWorldPos.z));
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

float normalizedDeviceCoord(float coord, float maxCoord, float dir){
    // coord: mouse x or y coord
    // maxCoord: screen width or height
    // dir: 1 or -1
    return dir * ( 2 * coord / maxCoord - 1.0f );
}

glm::vec3 createWorldRay(double xpos, double ypos, glm::mat4 projection, glm::mat4 view){
    // convert mouse to clip coords
    float ndcX = normalizedDeviceCoord(xpos, screenWidth, 1.0f);
    float ndcY = normalizedDeviceCoord(ypos, screenHeight, -1.0f);
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

    mouseX = xpos;
    mouseY = ypos;

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


bool intersectPlane(glm::vec3 &out, glm::vec3 origin, glm::vec3 intersectRay, glm::vec3 planeNormal, float planeDistance)
{
    // origin: camera position
    // intersectRay: direction of ray from origin
    // planeNormal: direction of plane (collision direction)
    // planeDistance: distance from plane to origin (will be negative since planeNormal is pointing away from plane)
    float rayAndNormalDotProduct = glm::dot(intersectRay, planeNormal);
    if (std::abs(rayAndNormalDotProduct) <= MIN_EPSILON) return false;
    float originAndNormalDotProduct = glm::dot(origin, planeNormal);
    float intersectDistance = -1 * (originAndNormalDotProduct + planeDistance) / rayAndNormalDotProduct;
    if (intersectDistance < 0) return false;
    glm::vec3 intersectPoint = origin + glm::vec3(intersectRay.x * intersectDistance, intersectRay.y * intersectDistance, intersectRay.z * intersectDistance);
    copyVec3(out, intersectPoint);
    return true;
}

bool eq(float a, float b){
    return a - b < MIN_EPSILON && b - a < MIN_EPSILON;
}

bool gte(float a, float b){
    return eq(a, b) || a - b > MIN_EPSILON;
}

bool lte(float a, float b){
    return eq(a, b) || a - b < MIN_EPSILON;
}
/*
bool pointIsInCube(glm::vec3 point){
    std::vector<glm::vec3> normals = CubeModel::getFaceNormals();
    std::vector<glm::vec3> pointsOnFaces = CubeModel::getPointsOnFaces();
    //glm::vec4 point4 = cubeModel * glm::vec4(point.x, point.y, point.z, 1.0f);
    //point = glm::vec3(point4.x / point4.w, point4.y / point4.w, point4.z / point4.w);
    bool res = true;
    for (unsigned int i=0; i<normals.size(); i++){
        glm::vec4 normal4 = cubeModel * glm::vec4(normals[i].x, normals[i].y, normals[i].z, 1.0f);
        glm::vec3 normal = glm::vec3(normal4.x / normal4.w, normal4.y / normal4.w, normal4.z / normal4.w);
        glm::vec4 pointOnFace4 = cubeModel * glm::vec4(pointsOnFaces[i].x, pointsOnFaces[i].y, pointsOnFaces[i].z, 1.0f);
        glm::vec3 pointOnFace = glm::vec3(pointOnFace4.x / pointOnFace4.w, pointOnFace4.y / pointOnFace4.w, pointOnFace4.z / pointOnFace4.w);
        glm::vec3 pointDiff = glm::vec3(pointOnFace.x - point.x, pointOnFace.y - point.y, pointOnFace.z - point.z);
        std::cout << i << std::endl;
        float dot = glm::dot(pointDiff, normal);
        std::cout << dot << std::endl;
        if (dot < 0) res = false;
    }
    return res;
}
*/

bool intersectSubCube(glm::vec3 &out, glm::vec3 origin, glm::vec3 intersectRay)
{
    const std::vector<glm::vec3> normals = CubeModel::getFaceNormals();
    const std::vector<glm::vec3> facesMinMax = CubeModel::getFacesMinMax();
    glm::vec3 intersectPoint;
    for (unsigned int i=0; i<1; i++){
        if (!intersectPlane(intersectPoint, origin, intersectRay, normals[i], -0.15f)) continue;
        // test bounds for each plane
        glm::vec4 max4 = cubeModel * glm::vec4(facesMinMax[i].x, facesMinMax[i].y, facesMinMax[i].z, 1.0f);
        glm::vec4 min4 = cubeModel * glm::vec4(facesMinMax[i + 1].x, facesMinMax[i + 1].y, facesMinMax[i + 1].z, 1.0f);
        glm::vec3 max = glm::vec3(max4.x / max4.w, max4.y / max4.w, max4.z / max4.w);
        glm::vec3 min = glm::vec3(min4.x / min4.w, min4.y / min4.w, min4.z / min4.w);
        std::cout << std::endl;
        printVec3(min);
        //printVec3(intersectPoint);
        printVec3(max);
        if (lte(intersectPoint.x, max.x) && lte(intersectPoint.y, max.y) && lte(intersectPoint.z, max.z) &&
          gte(intersectPoint.x, min.x) && gte(intersectPoint.y, min.y) && gte(intersectPoint.z, min.z)) 
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

void drawHits(Shader &shader, GLuint &VAO, GLuint &VBO)
{
    unsigned int nVertices = hits.size() * 6;
    GLfloat pointVertices[nVertices];
    for (unsigned int i=0; i<hits.size(); i++){
        unsigned int pInd = i * 6;
        pointVertices[pInd] = hits[i].x;
        pointVertices[pInd + 1] = hits[i].y;
        pointVertices[pInd + 2] = hits[i].z;
        pointVertices[pInd + 3] = 0.0f;
        pointVertices[pInd + 4] = 0.0f;
        pointVertices[pInd + 5] = 0.0f;
    } 
    bindVertices(VAO, VBO, pointVertices, sizeof(pointVertices));
    drawPointVertices(VAO, nVertices);
}

void drawMouseClicks(Shader &shader, GLuint &VAO, GLuint &VBO)
{
    //GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));

    glm::vec4 cam4 = glm::inverse(cubeModel) * glm::vec4(0.0f, 0.0f, 2.99f, 1.0f);
    glm::vec3 cam = glm::vec3(cam4.x / cam4.w, cam4.y / cam4.w, cam4.z / cam4.w);

    for (unsigned int i=0; i<mouseClicks.size(); i++){
        GLfloat lineVertices[12] = {
            cam.x, cam.y, cam.z, 1.0f, 1.f, 1.0f, 
            mouseClicks[i].x, mouseClicks[i].y, mouseClicks[i].z,  1.0f, 1.0f, 1.0f
        };
        bindVertices(VAO, VBO, lineVertices, sizeof(lineVertices));
        drawLineVertixes(VAO);
    }
}

void testIntersectPlane()
{
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
    
    glm::vec3 intersectRay = mouseWorldPos;
    glm::vec3 testPoint;
    bool hit = intersectSubCube(testPoint, camPos, intersectRay);
    if (hit){
        std::cout << "HIT" << std::endl;
        glm::vec3 out;
        copyVec3(out, testPoint);
        hits.push_back(out);
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
    glm::vec4 right = glm::inverse(cubeModel) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    cubeModel = glm::rotate(cubeModel, cubePitch, glm::vec3(right.x, right.y, right.z));
    glm::vec4 up = glm::inverse(cubeModel) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    cubeModel = glm::rotate(cubeModel, cubeYaw, glm::vec3(up.x, up.y, up.z)); 

    std::vector<SubCube> subcubes = cube.getSubCubes();

    glm::mat4 subcubeModel;
    subcubeModel = glm::rotate(subcubeModel, getNearestValidAngle(faceRotationAngle), faceRotationAxis);

    mouseWorldPos = createWorldRay(mouseX, mouseY, projection, view);

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
        testIntersectPlane();
        GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformSubCubeModel));
        // draw the mini cube
        drawCubeVertices(VAO);
    }
    
}

void beginLoop(GLFWwindow *window) 
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

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

        // the "game loop"
    while(!glfwWindowShouldClose(window)){
        beginLoop(window);
        // view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth/screenHeight, 0.1f, 100.0f);

        shader.Use();

        GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        mouseWorldPos = createWorldRay(mouseX, mouseY, projection, view);

        drawCubes(shader, cube, view, projection, VAO, VBO);
        drawMouseClicks(shader, VAO, VBO);
        drawHits(shader, VAO, VBO);
        endLoop(window);
    }

    // deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // close the window
    glfwTerminate();
    return 0;

}
