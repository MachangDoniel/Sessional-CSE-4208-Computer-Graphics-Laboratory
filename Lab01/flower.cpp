#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

float rotateAngle = 0.0;
float translate_X = -0.15;
float translate_Y = -0.15;
float scale_X = 1.0;
float scale_Y = 1.0;

float tub_rotateAngle = 0.0;
float tub_translate_X = 0.0;
float tub_translate_Y = 0.0;
float tub_scale_X = 1.0;
float tub_scale_Y = 1.0;

float flower_rotateAngle = 0.0;
float flower_translate_X = 0.0;
float flower_translate_Y = 0.0;
float flower_scale_X = 1.0;
float flower_scale_Y = 1.0;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 objectColor;\n"
"void main()\n"
"{\n"
"   FragColor = objectColor;\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Assignment 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //triangle

// Define the vertex data with two decimal places
    float vertices[] = {
        // tub vertices
        0.29f, 0.48f, 0.0f,
        0.27f, 0.45f, 0.0f,
        0.25f, 0.42f, 0.0f,
        0.26f, 0.37f, 0.0f,
        0.27f, 0.32f, 0.0f,
        0.29f, 0.28f, 0.0f,
        0.31f, 0.23f, 0.0f,
        0.32f, 0.19f, 0.0f,
        0.33f, 0.16f, 0.0f,
        0.34f, 0.13f, 0.0f,
        0.34f, 0.11f, 0.0f,
        0.34f, 0.10f, 0.0f,
        0.36f, 0.09f, 0.0f,
        0.40f, 0.08f, 0.0f,
        0.44f, 0.08f, 0.0f,
        0.48f, 0.09f, 0.0f,
        0.51f, 0.10f, 0.0f,
        0.51f, 0.12f, 0.0f,
        0.51f, 0.15f, 0.0f,
        0.52f, 0.18f, 0.0f,
        0.53f, 0.21f, 0.0f,
        0.56f, 0.24f, 0.0f,
        0.57f, 0.27f, 0.0f,
        0.58f, 0.30f, 0.0f,
        0.58f, 0.33f, 0.0f,
        0.59f, 0.36f, 0.0f,
        0.60f, 0.41f, 0.0f,
        0.59f, 0.45f, 0.0f,
        0.57f, 0.48f, 0.0f,
        0.56f, 0.52f, 0.0f,
        0.56f, 0.53f, 0.0f,
        0.53f, 0.54f, 0.0f,
        0.50f, 0.54f, 0.0f,
        0.42f, 0.54f, 0.0f,
        0.38f, 0.54f, 0.0f,
        0.33f, 0.54f, 0.0f,
        0.29f, 0.53f, 0.0f,
        0.29f, 0.51f, 0.0f,

        // stick vertices
        0.41f, 0.54f, 0.0f,
        0.43f, 0.54f, 0.0f,
        0.41f, 0.66f, 0.0f,
        0.43f, 0.66f, 0.0f,

        // flower vertices
        0.44f, 0.80f, 0.0f,
        0.42f, 0.67f, 0.0f,
        0.39f, 0.65f, 0.0f,
        0.36f, 0.65f, 0.0f,
        0.34f, 0.66f, 0.0f,
        0.30f, 0.67f, 0.0f,
        0.29f, 0.69f, 0.0f,
        0.28f, 0.71f, 0.0f,
        0.24f, 0.72f, 0.0f,
        0.22f, 0.73f, 0.0f,
        0.22f, 0.75f, 0.0f,
        0.20f, 0.77f, 0.0f,
        0.21f, 0.79f, 0.0f,
        0.22f, 0.80f, 0.0f,
        0.22f, 0.82f, 0.0f,
        0.21f, 0.84f, 0.0f,
        0.23f, 0.86f, 0.0f,
        0.24f, 0.89f, 0.0f,
        0.27f, 0.90f, 0.0f,
        0.30f, 0.90f, 0.0f,
        0.30f, 0.91f, 0.0f,
        0.32f, 0.93f, 0.0f,
        0.34f, 0.94f, 0.0f,
        0.38f, 0.95f, 0.0f,
        0.41f, 0.94f, 0.0f,
        0.43f, 0.93f, 0.0f,
        0.46f, 0.94f, 0.0f,
        0.48f, 0.95f, 0.0f,
        0.52f, 0.94f, 0.0f,
        0.55f, 0.93f, 0.0f,
        0.57f, 0.91f, 0.0f,
        0.57f, 0.89f, 0.0f,
        0.60f, 0.89f, 0.0f,
        0.62f, 0.88f, 0.0f,
        0.64f, 0.87f, 0.0f,
        0.64f, 0.85f, 0.0f,
        0.65f, 0.84f, 0.0f,
        0.65f, 0.81f, 0.0f,
        0.63f, 0.80f, 0.0f,
        0.63f, 0.79f, 0.0f,
        0.64f, 0.78f, 0.0f,
        0.65f, 0.76f, 0.0f,
        0.65f, 0.75f, 0.0f,
        0.63f, 0.74f, 0.0f,
        0.63f, 0.73f, 0.0f,
        0.63f, 0.71f, 0.0f,
        0.60f, 0.70f, 0.0f,
        0.58f, 0.70f, 0.0f,
        0.56f, 0.69f, 0.0f,
        0.56f, 0.68f, 0.0f,
        0.55f, 0.67f, 0.0f,
        0.52f, 0.66f, 0.0f,
        0.50f, 0.66f, 0.0f,
        0.48f, 0.65f, 0.0f,
        0.45f, 0.65f, 0.0f,
        0.44f, 0.65f, 0.0f,
        0.42f, 0.67f, 0.0f
    };



    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.52f, 0.87f, 0.91f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations (total)
        /*glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(translate_X, translate_Y, 0.0f));
        trans = glm::rotate(trans, glm:: radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans,glm::vec3(scale_X, scale_Y, 1.0));*/
        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        translationMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        // create transformations (tub)
        glm::mat4 tub_translationMatrix;
        glm::mat4 tub_rotationMatrix;
        glm::mat4 tub_scaleMatrix;
        glm::mat4 tub_modelMatrix;
        glm::mat4 tub_identityMatrix = glm::mat4(1.0f);
        tub_translationMatrix = glm::translate(modelMatrix, glm::vec3(tub_translate_X, tub_translate_Y, 0.0f));
        tub_rotationMatrix = glm::rotate(modelMatrix, glm::radians(tub_rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        tub_scaleMatrix = glm::scale(modelMatrix, glm::vec3(tub_scale_X, tub_scale_Y, 1.0f));
        tub_modelMatrix = tub_translationMatrix * tub_rotationMatrix * tub_scaleMatrix;


        // create transformations (flower)
        glm::mat4 flower_translationMatrix;
        glm::mat4 flower_rotationMatrix;
        glm::mat4 flower_scaleMatrix;
        glm::mat4 flower_modelMatrix;
        glm::mat4 flower_identityMatrix = glm::mat4(1.0f);
        flower_translationMatrix = glm::translate(modelMatrix, glm::vec3(flower_translate_X, flower_translate_Y, 0.0f));
        flower_rotationMatrix = glm::rotate(modelMatrix, glm::radians(flower_rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        flower_scaleMatrix = glm::scale(modelMatrix, glm::vec3(flower_scale_X, flower_scale_Y, 1.0f));
        flower_modelMatrix = flower_translationMatrix * flower_rotationMatrix * flower_scaleMatrix;


        glUseProgram(shaderProgram);


        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(tub_modelMatrix));
        //glUniform4f(colorLoc, 1.0f, 0.73f, 0.51f, 1.0f);
        glUniform4f(colorLoc, 0.90f, 0.70f, 0.20f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 37);


        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(flower_modelMatrix));

        //tub shape
        //glDrawArrays(GL_LINE_STRIP, 25, 14);
        glUniform4f(colorLoc, 0.20f, 0.50f, 0.42f, 1.0f);

        glDrawArrays(GL_TRIANGLE_FAN, 38, 4);
        glUniform4f(colorLoc, 0.950f, 0.00f, 0.00f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 42, 57);
        //glDrawArrays(GL_TRIANGLE_FAN, 68, 18);
        //glUniform4f(colorLoc, 1.0f, 0.84f, 0.2f, 1.0f);
        //glDrawArrays(GL_TRIANGLE_FAN, 86, 28);*/
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 23);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 24);





        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //translation (total)
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) translate_X += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) translate_X -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) translate_Y += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) translate_Y -= 0.001f;

    // Scale up when "Z" key is pressed
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) scale_X += 0.001f, scale_Y += 0.001f;

    // Scale down when "X" key is pressed
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        scale_X -= 0.001f;
        scale_Y -= 0.001f;

        if (scale_X < 0.1f) scale_X = 0.1f;
        if (scale_Y < 0.1f) scale_Y = 0.1f;
    }

    // Rotate clockwise when "E" key is pressed
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) rotateAngle -= 0.20f;

    // Rotate counterclockwise when "Q" key is pressed
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) rotateAngle += 0.20f;



    //translation (tub)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) tub_translate_X += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) tub_translate_X -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) tub_translate_Y += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) tub_translate_Y -= 0.001f;

    // Scale up when "C" key is pressed
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) tub_scale_X += 0.001f, tub_scale_Y += 0.001f;


    // Scale down when "V" key is pressed
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        tub_scale_X -= 0.001f;
        tub_scale_Y -= 0.001f;

        if (tub_scale_X < 0.1f) tub_scale_X = 0.1f;
        if (tub_scale_Y < 0.1f) tub_scale_Y = 0.1f;
    }

    // Rotate clockwise when "E" key is pressed
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) tub_rotateAngle -= 0.20f;

    // Rotate counterclockwise when "Q" key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) tub_rotateAngle += 0.20f;


    //translation (ice cream)
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) flower_translate_X += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) flower_translate_X -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) flower_translate_Y += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) flower_translate_Y -= 0.001f;

    // Scale up when "B" key is pressed
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        flower_scale_X += 0.001f;
        flower_scale_Y += 0.001f;
    }

    // Scale down when "N" key is pressed
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        flower_scale_X -= 0.001f;
        flower_scale_Y -= 0.001f;

        if (flower_scale_X < 0.1f) flower_scale_X = 0.1f;
        if (flower_scale_Y < 0.1f) flower_scale_Y = 0.1f;
    }

    // Rotate clockwise when "U" key is pressed
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) flower_rotateAngle -= 0.20f;

    // Rotate counterclockwise when "Y" key is pressed
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) flower_rotateAngle += 0.20f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
