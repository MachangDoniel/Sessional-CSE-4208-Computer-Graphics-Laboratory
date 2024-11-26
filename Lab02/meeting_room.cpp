//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//  modified by Badiuzzaman on 3/11/24.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera.h"
#include "camera.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Time management
double lastKeyPressTime = 0.0;
const double keyPressDelay = 0.2; // delay in seconds

// draw object functions
void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX = 0.0, float posY = 0.0, float posz = 0.0, float rotX = 0.0, float rotY = 0.0, float rotZ = 0.0, float scX = 1.0, float scY = 1.0, float scZ = 1.0);
void draw_Table(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 all_mat);
void draw_Chair(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 all_mat);
void draw_Room(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans);
void draw_Fan(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 rot);
void draw_Door(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 rotation);
void draw_TV(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans);
//void stagebinet(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans);
// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 900;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

//variables
glm::mat4 transformation(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * scaleMatrix;
    return model;
}


bool doorOpen = false;
float fr = 0.0;//variable for fan rotation
bool fanON = false;//fan ON OFF variable
float fanPower = 0.0;//fan regulator
float rotDoor = 0.0;
const float ROTATE_INCREMENT = glm::radians(1.0f); // 5 degrees


// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float eyeX = 0.0, eyeY = 0.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 cam = glm::vec3(4.0f, 1.0f, -2.0f);


// camera
Camera camera(cam);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    Shader constantShader("vertexShader.vs", "fragmentShaderV2.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,
        4, 5, 7,
        7, 6, 4,
        8, 9, 10,
        10, 11, 8,
        12, 13, 14,
        14, 15, 12,
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20,
    };


    unsigned int VBOdec, VAOdec, EBOdec;
    glGenVertexArrays(1, &VAOdec);
    glGenBuffers(1, &VBOdec);
    glGenBuffers(1, &EBOdec);
    glBindVertexArray(VAOdec);
    glBindBuffer(GL_ARRAY_BUFFER, VBOdec);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //for decagon2
    unsigned int VBOdec2, VAOdec2, EBOdec2;
    glGenVertexArrays(1, &VAOdec2);
    glGenBuffers(1, &VBOdec2);
    glGenBuffers(1, &EBOdec2);
    glBindVertexArray(VAOdec2);
    glBindBuffer(GL_ARRAY_BUFFER, VBOdec2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec2);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ourShader.use();
    //constantShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        //constantShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();

        //glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);
        //constantShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, modelCentered, rotation, translateFan, translateFanBack;
        translateMatrix = identityMatrix;

        //draw room
        draw_Room(ourShader, VAO, identityMatrix);

        draw_TV(ourShader, VAO, identityMatrix);


        //draw fan
        if (fanON && fanPower == 1.0) {
            translateFan = glm::translate(identityMatrix, glm::vec3(-1.9f, -0.5f, -1.83f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-fr), glm::vec3(0.0f, 1.0f, 0.0f));
            translateFanBack = glm::translate(identityMatrix, glm::vec3(1.9f, 0.5f, 1.83f));
            draw_Fan(ourShader, VAO, identityMatrix, translateFanBack * rotateYMatrix * translateFan);
            fr += 1.0;
        }
        else if (fanON && fanPower == 2.0) {
            translateFan = glm::translate(identityMatrix, glm::vec3(-1.9f, -0.5f, -1.83f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-fr), glm::vec3(0.0f, 1.0f, 0.0f));
            translateFanBack = glm::translate(identityMatrix, glm::vec3(1.9f, 0.5f, 1.83f));
            draw_Fan(ourShader, VAO, identityMatrix, translateFanBack * rotateYMatrix * translateFan);
            fr += 2.0;
        }
        else if (fanON && fanPower == 3.0) {
            translateFan = glm::translate(identityMatrix, glm::vec3(-1.9f, -0.5f, -1.83f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-fr), glm::vec3(0.0f, 1.0f, 0.0f));
            translateFanBack = glm::translate(identityMatrix, glm::vec3(1.9f, 0.5f, 1.83f));
            draw_Fan(ourShader, VAO, identityMatrix, translateFanBack * rotateYMatrix * translateFan);
            fr += 3.0;
        }
        else if (fanON && fanPower == 4.0) {
            translateFan = glm::translate(identityMatrix, glm::vec3(-1.9f, -0.5f, -1.83f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-fr), glm::vec3(0.0f, 1.0f, 0.0f));
            translateFanBack = glm::translate(identityMatrix, glm::vec3(1.9f, 0.5f, 1.83f));
            draw_Fan(ourShader, VAO, identityMatrix, translateFanBack * rotateYMatrix * translateFan);
            fr += 4.0;
        }
        else if (fanON && fanPower == 5.0) {
            translateFan = glm::translate(identityMatrix, glm::vec3(-1.9f, -0.5f, -1.83f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-fr), glm::vec3(0.0f, 1.0f, 0.0f));
            translateFanBack = glm::translate(identityMatrix, glm::vec3(1.9f, 0.5f, 1.83f));
            draw_Fan(ourShader, VAO, identityMatrix, translateFanBack * rotateYMatrix * translateFan);
            fr += 5.0;
        }
        else {
            translateFan = glm::translate(identityMatrix, glm::vec3(-1.9f, -0.5f, -1.83f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-fr), glm::vec3(0.0f, 1.0f, 0.0f));
            translateFanBack = glm::translate(identityMatrix, glm::vec3(1.9f, 0.5f, 1.83f));
            draw_Fan(ourShader, VAO, identityMatrix, translateFanBack * rotateYMatrix * translateFan);
        }

        //door open
        if (doorOpen == 2.0) {
            // draw door without any rotation
            draw_Door(ourShader, VAO, identityMatrix, identityMatrix);
        }
        else if (doorOpen == 1.0) {
            // door opening
            if (rotDoor < 90.0f) {
                rotDoor += 0.5f; // increment rotation angle
            }
            else {
                rotDoor = 90.0f; // cap it at 90 degrees
            }
            rotation = glm::rotate(identityMatrix, glm::radians(-rotDoor), glm::vec3(0.0f, 1.0f, 0.0f));
            draw_Door(ourShader, VAO, identityMatrix, rotation);
        }
        else if (doorOpen == 0.0) {
            // door closing
            if (rotDoor > 0.0f) {
                rotDoor -= 0.5f; // decrease rotation angle
            }
            else {
                rotDoor = 0.0f; // cap it at 0 degrees
            }
            rotation = glm::rotate(identityMatrix, glm::radians(-rotDoor), glm::vec3(0.0f, 1.0f, 0.0f));
            draw_Door(ourShader, VAO, identityMatrix, rotation);
        }


        //draw chair and table
        float t = 0.0;
        for (int i = 0; i < 1; i++)
        {
            for (int j = 0; j < 1; j++) {
                translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 1.0f));

                draw_Table(ourShader, VAO, identityMatrix, translateMatrix);
            }
            t += 2.0;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-t, 0.0f, 0.0f));
        }
        translateMatrix = identityMatrix;
        t = 0.0;
        for (int i = 0; i < 1; i++)
        {
            for (int j = 0; j < 5; j++) {
                translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 1.0f));

                draw_Chair(ourShader, VAO, identityMatrix, translateMatrix);
            }
            t += 2.0;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-t, 0.0f, 0.0f));
        }


        // render boxes

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime < keyPressDelay) return;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        camera.toggleBirdsEyeView();
        lastKeyPressTime = currentTime;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        camera.rotateAroundTarget(ROTATE_INCREMENT, 0.0f); // Rotate horizontally
    }


    //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) camera.ProcessKeyboard(P_UP, deltaTime); // Increase Pitch
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) camera.ProcessKeyboard(Y_LEFT, deltaTime); // Yaw Left
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) camera.ProcessKeyboard(R_LEFT, deltaTime); // Roll Left
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.ProcessKeyboard(Y_LEFT, deltaTime); // Increase Pitch
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.ProcessKeyboard(Y_RIGHT, deltaTime); // Decrease Pitch
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) camera.ProcessKeyboard(R_RIGHT, deltaTime); // Increase Yaw
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) camera.ProcessKeyboard(R_LEFT, deltaTime); // Decrease Yaw
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.ProcessKeyboard(P_UP, deltaTime); // Increase Roll
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.ProcessKeyboard(P_DOWN, deltaTime); // Increase Roll
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (fanON) {
            fanON = false;
            fanPower = 0.0;
        }
        else{
            fanON = true;
            fanPower = 1.0;
        }
        lastKeyPressTime = currentTime;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        if (doorOpen) {
            doorOpen = false;
        }
		else{
			doorOpen = true;
		}
        lastKeyPressTime = currentTime;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //basic_camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    basic_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void draw_Room(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans) {
    shaderProgram.use();

    //floor
    shaderProgram.setVec4("color", glm::vec4(0.4f, 0.72f, 0.69f, 1.0f)); //color
    glm::mat4  scaleMatrix, model, modelCentered, translateMatrix;
    translateMatrix = glm::translate(parentTrans, glm::vec3(-1.5f, -1.1f, -4.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(14.0f, 0.2f, 26.0f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //back wall 1
    shaderProgram.setVec4("color", glm::vec4(0.55f, 0.906f, 0.55f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-1.5f, -1.0f, 7.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 8.0f, 4.0f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //back wall 3
    shaderProgram.setVec4("color", glm::vec4(0.55f, 0.906f, 0.55f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-1.5f, 1.0f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 4.0f, 3.0f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //back wall 2
    shaderProgram.setVec4("color", glm::vec4(0.55f, 0.906f, 0.55f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-1.5f, -1.0f, -4.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 8.0f, 19.5f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right side wall 1
    shaderProgram.setVec4("color", glm::vec4(0.0f, 0.769f, 0.627f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-1.5f, -1.0f, -4.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(14.0f, 8.0f, 0.2f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //left side wall 1
    shaderProgram.setVec4("color", glm::vec4(0.961f, 0.769f, 0.627f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-1.5f, -1.0f, 9.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(14.0f, 8.0f, 0.2f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //front side wall 1
    shaderProgram.setVec4("color", glm::vec4(1.0f, 0.906f, 0.635f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(5.5f, -1.0f, -4.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 8.0f, 26.0f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //ceiling
    shaderProgram.setVec4("color", glm::vec4(0.9f, 0.849f, 0.929f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-2.00f, 3.0f, -4.25f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(16.0f, 0.2f, 28.0f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_TV(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans) {
    shaderProgram.use();

    //TV
    //shaderProgram.setVec4("color", glm::vec4(0.58f, 0.925f, 0.949f, 1.0f)); //color
    glm::mat4  scaleMatrix, model, modelCentered, translateMatrix, rotateYMatrix;


    //TV background
    shaderProgram.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(2.5f, 0.55f, 8.9f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 3.0f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // right
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(2.5f, 0.55f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 3.0f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // left
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(4.0f, 0.55f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 3.0f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // up
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(2.5f, 1.95f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 0.2f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // down
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(2.5f, 0.55f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 0.2f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //TV background
    shaderProgram.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-0.5f, 0.55f, 8.9f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 3.0f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // right
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-0.5f, 0.55f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 3.0f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // left
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(1.0f, 0.55f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 3.0f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // up
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-0.5f, 1.95f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 0.2f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // down
    shaderProgram.setVec4("color", glm::vec4(0.051f, 0.329f, 0.349f, 1.0f)); //color
    translateMatrix = glm::translate(parentTrans, glm::vec3(-0.5f, 0.55f, 8.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 0.2f, 0.1f));
    model = scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));
    shaderProgram.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}
void draw_Fan(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 rot) {
    shaderProgram.use();

    //fan base

    shaderProgram.setVec4("color", glm::vec4(0.78f, 0.75f, 0.75f, 1.0f)); //color
    glm::mat4  scaleMatrix, model, modelCentered, translateMatrix, rotateYMatrix;
    translateMatrix = glm::translate(parentTrans, glm::vec3(1.7f, 0.3f, 1.65f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.00f, 0.50f, 1.0f));
    model = translate(parentTrans, glm::vec3(-1.3f, 2.0f, 0.55f)) * rot * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //fan base rod
    shaderProgram.setVec4("color", glm::vec4(0.561f, 0.561f, 0.561f, 1.0f)); //color

    translateMatrix = glm::translate(parentTrans, glm::vec3(1.82f, 0.5f, 1.83f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 1.3f, 0.2f));
    model = translate(parentTrans, glm::vec3(-1.3f, 2.0f, 0.55f)) * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //fan blade 1
    shaderProgram.setVec4("color", glm::vec4(0.69f, 0.69f, 0.69f, 1.0f)); //color

    translateMatrix = glm::translate(parentTrans, glm::vec3(2.2f, 0.5f, 1.74f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(2.0f, 0.2f, 0.6f));
    model = translate(parentTrans, glm::vec3(-1.3f, 2.0f, 0.55f)) * rot * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //fan blade 2
    shaderProgram.setVec4("color", glm::vec4(0.69f, 0.69f, 0.69f, 1.0f)); //color

    translateMatrix = glm::translate(parentTrans, glm::vec3(1.7f, 0.5f, 2.05f));
    rotateYMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(rotateYMatrix, glm::vec3(2.0f, 0.2f, 0.6f));
    model = translate(parentTrans, glm::vec3(-1.3f, 2.0f, 0.55f)) * rot * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //fan blade 3
    shaderProgram.setVec4("color", glm::vec4(0.69f, 0.69f, 0.69f, 1.0f)); //color

    translateMatrix = glm::translate(parentTrans, glm::vec3(2.1f, 0.5f, 2.0f));
    rotateYMatrix = glm::rotate(translateMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(rotateYMatrix, glm::vec3(2.0f, 0.2f, 0.6f));
    model = translate(parentTrans, glm::vec3(-1.3f, 2.0f, 0.55f)) * rot * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //fan blade 4
    shaderProgram.setVec4("color", glm::vec4(0.69f, 0.69f, 0.69f, 1.0f)); //color

    translateMatrix = glm::translate(parentTrans, glm::vec3(1.79f, 0.5f, 1.75f));
    rotateYMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(rotateYMatrix, glm::vec3(2.0f, 0.2f, 0.6f));
    model = translate(parentTrans, glm::vec3(-1.3f, 2.0f, 0.55f)) * rot * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_Door(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 rotation)
{
    shaderProgram.use();

    //door leaf
    shaderProgram.setVec4("color", glm::vec4(.90f, .90f, 0.90f, 1.0f)); //color
    glm::mat4  scaleMatrix, model, modelCentered, translateMatrix, rotateYMatrix;
    rotateYMatrix = glm::rotate(parentTrans, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(rotateYMatrix, glm::vec3(2.5f, 4.0f, 0.2f));
    model = translate(parentTrans, glm::vec3(-1.5f, -1.0f, 7.0f)) * rotation * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_Table(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 all_mat)
{
    shaderProgram.use();

    //table
    shaderProgram.setVec4("color", glm::vec4(0.2f, 0.604f, 0.145f, 1.0f)); //color
    glm::mat4  scaleMatrix, model, modelCentered, translateMatrix;
    scaleMatrix = glm::scale(parentTrans, glm::vec3(4.0f, 0.2f, 10.0f));
    model = all_mat * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg_1
    shaderProgram.setVec4("color", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    translateMatrix = glm::translate(parentTrans, glm::vec3(0.9f, 0.0f, 0.2f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = all_mat * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg_base1
    shaderProgram.setVec4("color", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    translateMatrix = glm::translate(parentTrans, glm::vec3(1.20f, -1.0f, 0.2f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(-1.0f, 0.2f, 0.2f));
    model = all_mat * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg_2
    translateMatrix = glm::translate(parentTrans, glm::vec3(0.90f, 0.0f, 3.2f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = all_mat * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg_base1
    shaderProgram.setVec4("color", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    translateMatrix = glm::translate(parentTrans, glm::vec3(1.20f, -1.0f, 3.2f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(-1.0f, 0.2f, 0.2f));
    model = all_mat * scaleMatrix;
    //modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void drawPart(Shader shaderProgram,unsigned int VAO,glm::mat4 parentTrans,glm::mat4 all_mat,glm::vec3 translation,glm::vec3 scale,glm::vec4 color) {
    shaderProgram.use();

    // Set color
    shaderProgram.setVec4("color", color);

    // Add translation along the x-axis
    translation.x += 2.0f;

    // Create transformation matrix
    glm::mat4 translateMatrix = glm::translate(parentTrans, translation);
    glm::mat4 scaleMatrix = glm::scale(translateMatrix, scale);
    glm::mat4 model = all_mat * scaleMatrix;

    // Pass transformation matrix to the shader
    shaderProgram.setMat4("model", model);

    // Render the part
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_Chair(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, glm::mat4 all_mat) {
    glm::vec4 chairColor = glm::vec4(0.9f, 0.9f, 0.8f, 1.0f);

    // Chair base
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.00f, -0.5f, 0.4f), glm::vec3(1.2f, 0.2f, 1.2f), chairColor);

    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.65f, -0.5f, 0.4f), glm::vec3(1.2f, 0.2f, 1.2f), chairColor);


    // Chair legs
    glm::vec3 legScale = glm::vec3(0.2f, -1.0f, 0.2f);

    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.00f, -0.5f, 0.4f), legScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.50f, -0.5f, 0.4f), legScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.00f, -0.5f, 0.9f), legScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.50f, -0.5f, 0.9f), legScale, chairColor);

    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.65f, -0.5f, 0.4f), legScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.15f, -0.5f, 0.4f), legScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.65f, -0.5f, 0.9f), legScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.15f, -0.5f, 0.9f), legScale, chairColor);


    // Chair upper vertical parts
    glm::vec3 upperVerticalScale = glm::vec3(0.2f, -1.5f, 0.2f);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.50f, 0.35f, 0.9f), upperVerticalScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.50f, 0.35f, 0.4f), upperVerticalScale, chairColor);

    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.65f, 0.35f, 0.9f), upperVerticalScale, chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.65f, 0.35f, 0.4f), upperVerticalScale, chairColor);



    // Chair upper horizontal parts
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.50f, 0.3f, 0.4f), glm::vec3(0.2f, 0.2f, 1.2f), chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(1.50f, 0.0f, 0.4f), glm::vec3(0.2f, 1.2f, 1.2f), chairColor);

    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.65f, 0.3f, 0.4f), glm::vec3(0.2f, 0.2f, 1.2f), chairColor);
    drawPart(shaderProgram, VAO, parentTrans, all_mat, glm::vec3(-1.65f, 0.0f, 0.4f), glm::vec3(0.2f, 1.2f, 1.2f), chairColor);

}
