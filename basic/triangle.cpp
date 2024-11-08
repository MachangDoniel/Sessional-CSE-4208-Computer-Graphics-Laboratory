//
//  main.cpp
//  triangle
//
//  Created by Nazirul Hasan on 26/8/23.
//  modified
//

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
const unsigned int SCR_HEIGHT = 800;
float rotateAngle = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float translate_X2 = 1.0;
float translate_Y2 = 1.0;
float rotateAngle2 = 0.0;
float scale_X2 = 0.3;
float scale_Y2 = 0.3;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
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
    float vertices[] = {
         -0.8f,  0.2f, 0.0f,
        -0.8f, -0.8f, 0.0f,
         0.2f, -0.8f, 0.0f,
         0.2f,  0.2f, 0.0f
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations
        /*glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(translate_X, translate_Y, 0.0f));
        trans = glm::rotate(trans, glm:: radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans,glm::vec3(scale_X, scale_Y, 1.0));*/
        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 modelMatrix;

        glm::mat4 translationMatrix_2;
        glm::mat4 scaleMatrix_2;
        glm::mat4 modelMatrix_2;
        glm::mat4 identityMatrix = glm::mat4(1.0f);


        translationMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        modelMatrix_2 = translationMatrix * rotationMatrix * scaleMatrix;




        //modelMatrix = rotationMatrix * scaleMatrix;

        // get matrix's uniform location and set matrix
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));


        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_LINES, 0, 6);
        //glDrawArrays(GL_LINE_STRIP, 0, 6);
        //glDrawArrays(GL_LINE_LOOP, 0, 6);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        // glBindVertexArray(0); // no need to unbind it every time




        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------


        translationMatrix_2 = glm::translate(identityMatrix, glm::vec3(1.0, 1.0, 0.0f));
        scaleMatrix_2 = glm::scale(identityMatrix, glm::vec3(0.3, 0.3, 1.0f));
        modelMatrix_2 = translationMatrix_2 * scaleMatrix_2;

        unsigned int transformLoc_2 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc_2, 1, GL_FALSE, glm::value_ptr(modelMatrix_2));


        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix_2));

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP, 0, 4);


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

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) translate_X += -0.001;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) translate_Y += -0.001;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) rotateAngle += 0.1;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) rotateAngle -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y2 += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y2 -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X2 -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X2 += 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) rotateAngle2 += 0.1;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) rotateAngle2 -= 0.1;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
