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
const unsigned int SCR_HEIGHT = 1000;
float rotateAngle = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;

float rotateAngle2 = 0.0;
float translate_X2 = 0.0;
float translate_Y2 = 0.0;
float scale_X2 = 1.0;
float scale_Y2 = 1.0;

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



     //flower
    float flower[] = {
        //   xf       yf       zf
        // Peduncle
		-0.02f, -0.30f, +0.0f, //1
		+0.00f, -0.30f, +0.0f, //2
		+0.00f, -0.20f, +0.0f, //3
		+0.02f, -0.20f, +0.0f, //4
        +0.01f, -0.10f, +0.0f, //5
        +0.03f, -0.10f, +0.0f, //6
        +0.01f, -0.00f, +0.0f, //7
        +0.03f, -0.00f, +0.0f, //8
		+0.00f, +0.10f, +0.0f, //9
		+0.02f, +0.10f, +0.0f, //10
		-0.01f, +0.20f, +0.0f, //11
		+0.01f, +0.20f, +0.0f, //12
		-0.01f, +0.30f, +0.0f, //13
        +0.01f, +0.30f, +0.0f //14

        // flower


		

    };

    int flowerSize = sizeof(flower) / sizeof(flower[0]) /3;

    //tub of flower
    float tub[] = {

        //   xf       yf       zf
		-0.20f, -0.30f, +0.0f, //1, 5
		+0.20f, -0.30f, +0.0f, //2
		+0.20f, -0.32f, +0.0f, //3, 9
        -0.20f, -0.32f, +0.0f, //4, 6, 10
        -0.20f, -0.30f, +0.0f, //5, 1
        -0.20f, -0.32f, +0.0f, //6, 4, 10
        -0.10f, -0.70f, +0.0f, //7
		+0.10f, -0.70f, +0.0f, //8
        +0.20f, -0.32f, +0.0f, //9, 3
        -0.20f, -0.32f, +0.0f, //10, 4, 6
    };

    int tubSize = sizeof(tub) / sizeof(tub[0]) / 3;

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // Set up VBO and VAO for the flower
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flower), flower, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind the buffer
    glBindVertexArray(0);  // Unbind the VAO

    // Set up VBO and VAO for the tub
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tub), tub, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind the buffer
    glBindVertexArray(0);  // Unbind the VAO


    
    
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


        // for object1
        {
            glm::mat4 translationMatrix;
            glm::mat4 rotationMatrix;
            glm::mat4 scaleMatrix;
            glm::mat4 modelMatrix;
            glm::mat4 identityMatrix = glm::mat4(1.0f);
            translationMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
            rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
            modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
            //modelMatrix = rotationMatrix * scaleMatrix;



            // get matrix's uniform location and set matrix
            glUseProgram(shaderProgram);
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));


            // get matrix's uniform location and set matrix
            // glUseProgram(shaderProgram);
            // unsigned int transformLoc2 = glGetUniformLocation(shaderProgram, "transform");
            // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix2));



            // draw flower
            glBindVertexArray(VAO[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawArrays(GL_LINES, 0, 4);
            //glDrawArrays(GL_LINE_STRIP, 0, 9);
            //glDrawArrays(GL_LINE_LOOP, 0, 6);
            //glDrawArrays(GL_TRIANGLES, 0, 6);

            //glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
            // glDrawArrays(GL_TRIANGLES, 0, 3);

            // task
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, flowerSize);
        }




        // for object2
        {
            glm::mat4 translationMatrix2;
            glm::mat4 rotationMatrix2;
            glm::mat4 scaleMatrix2;
            glm::mat4 modelMatrix2;
            glm::mat4 identityMatrix2 = glm::mat4(1.0f);
            translationMatrix2 = glm::translate(identityMatrix2, glm::vec3(translate_X2, translate_Y2, 0.0f));
            rotationMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle2), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(scale_X2, scale_Y2, 1.0f));
            modelMatrix2 = translationMatrix2 * rotationMatrix2 * scaleMatrix2;
            //modelMatrix2 = rotationMatrix * scaleMatrix;

            // get matrix's uniform location and set matrix
            glUseProgram(shaderProgram);
            unsigned int transformLoc2 = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(modelMatrix2));


            // get matrix's uniform location and set matrix
            // glUseProgram(shaderProgram);
            // unsigned int transformLoc2 = glGetUniformLocation(shaderProgram, "transform");
            // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix2));



            // draw our first triangle
            glBindVertexArray(VAO[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawArrays(GL_LINES, 0, 4);
            //glDrawArrays(GL_LINE_STRIP, 0, 9);
            //glDrawArrays(GL_LINE_LOOP, 0, 6);
            //glDrawArrays(GL_TRIANGLES, 0, 6);

            //glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
            // glDrawArrays(GL_TRIANGLES, 0, 3);

            // task
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, tubSize);
        }




        // Only creates the boundary, not color
         // glPolygonMode(GL_FROT_AND_BACK, GL_LINE)
         //glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
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

    // flower
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        translate_X += static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        translate_X -= static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        translate_Y += static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        translate_Y -= static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        rotateAngle += static_cast<float>(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        rotateAngle -= static_cast<float>(0.1);
    }
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		scale_X += static_cast<float>(0.001);
		scale_Y += static_cast<float>(0.001);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		scale_X -= static_cast<float>(0.001);
		scale_Y -= static_cast<float>(0.001);
	}

    // tub
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        translate_X2 += static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        translate_X2 -= static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        translate_Y2 += static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        translate_Y2 -= static_cast<float>(0.001);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        rotateAngle2 += static_cast<float>(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        rotateAngle2 -= static_cast<float>(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
		scale_X2 += static_cast<float>(0.001);
		scale_Y2 += static_cast < float>(0.001);
    }
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		scale_X2 -= static_cast < float>(0.001);
		scale_Y2 -= static_cast < float>(0.001);
	}


    // Refresh
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		translate_X = static_cast < float>(0.0);
		translate_Y = static_cast < float>(0.0);
		rotateAngle = static_cast < float>(0.0);
		scale_X = static_cast < float>(1.0);
		scale_Y = static_cast < float>(1.0);

		translate_X2 = static_cast < float>(0.0);
		translate_Y2 = static_cast < float>(0.0);
		rotateAngle2 = static_cast < float>(0.0);
		scale_X2 = static_cast < float>(1.0);
		scale_Y2 = static_cast < float>(1.0);
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}