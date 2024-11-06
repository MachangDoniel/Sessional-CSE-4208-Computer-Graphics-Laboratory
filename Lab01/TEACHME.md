# Lab 01: Introduction to OpenGL and Basic Transformations
## Introduction
This lab introduces fundamental concepts of Computer Graphics and explores key libraries and tools, such as OpenGL, GLSL, GLFW, GLUT, and GLAD. The goal is to create a simple graphical application to understand transformations, shaders, and OpenGL pipeline basics.

### Introduction to Computer Graphics

Computer Graphics is a field of computer science focused on generating, manipulating, and rendering images and visual content. It underpins applications like video games, simulations, and data visualizations. By using mathematical models and algorithms, computer graphics enables the creation of complex scenes, realistic textures, and interactive visualizations. Tools like OpenGL and libraries such as GLFW and GLUT offer APIs to manage these graphics at low levels, making them fundamental in rendering pipelines.
### OpenGL

OpenGL (Open Graphics Library) is a widely-used open-source graphics API designed to interact with the GPU to create 2D and 3D graphics. It provides a rich set of functions for rendering, which are essential for game development, simulations, and visual effects. In this lab, OpenGL was used to render simple shapes and apply transformations like translation, rotation, and scaling. Key features include:

- Vertex specification: Defining shapes with vertices.
- Transformation matrices: Applying transformations to models.
- Shaders: Enabling custom processing of vertices and fragments.

### GLSL (OpenGL Shading Language)

GLSL (OpenGL Shading Language) is a C-like language designed for programming shaders within OpenGL. Shaders are small programs that run on the GPU and handle the rendering pipeline stages:

- Vertex Shader: Transforms vertex positions.
- Fragment Shader: Determines the color of each pixel. GLSL allows us to manipulate how graphics are rendered by defining how vertices and fragments (pixels) should behave, enabling effects such as lighting, textures, and transformations.

### GLFW

GLFW (Graphics Library Framework) is a library designed to handle window creation, input management, and context creation for OpenGL applications. It is lightweight and cross-platform, simplifying the setup required to build OpenGL applications. Key features of GLFW include:

- Window management: Creating and managing OpenGL windows.
- Input handling: Capturing keyboard and mouse inputs.
- Event handling: Managing window events like resizing.

### GLUT

GLUT (OpenGL Utility Toolkit) is an older utility library designed to simplify OpenGL program development by providing a minimalistic set of tools for creating windows, handling user inputs, and managing basic events. While GLFW has mostly replaced it for modern applications, GLUT is still useful for basic OpenGL applications and educational purposes.

### GLAD

GLAD (GL Loader Generator) is a library that manages OpenGL extensions and loads function pointers required for OpenGL function calls. Since OpenGL drivers need to expose functions dynamically, GLAD loads these function pointers for the application, enabling us to call OpenGL functions without directly linking them at compile time.
### Transformation Code

This lab applied transformations using OpenGL’s GLM (OpenGL Mathematics) library for matrices. Different transformation matrices for translation, rotation, and scaling were combined into a model matrix, which was then passed to the vertex shader for positioning the vertices of the object.
Example Transformation Code:

```cpp
glm::mat4 translationMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
glm::mat4 rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
glm::mat4 scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
```
Drawing Modes

OpenGL provides multiple modes for drawing primitive shapes by specifying different drawing methods. These modes include:

- `GL_LINES`: Draws lines between pairs of vertices.
- `GL_LINE_STRIP`: Draws a connected series of line segments.
- `GL_LINE_LOOP`: Similar to GL_LINE_STRIP but closes the loop.
- `GL_TRIANGLES`: Draws a series of triangles.
- `GL_TRIANGLE_STRIP`: Draws a connected strip of triangles.
- `GL_TRIANGLE_FAN`: Draws triangles connected in a fan pattern from the first vertex.

In this lab, the `GL_TRIANGLE_STRIP` mode was used to create a **pentagon** shape with vertices arranged as a strip. Other modes allow flexibility in how vertices are connected, offering a variety of shapes and structures for rendering.

```cpp
// Drawing example
glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
```
### Conclusion

This lab provided a hands-on introduction to creating a simple graphics application with OpenGL and explored basic transformations and shaders. Understanding the OpenGL rendering pipeline and the functionality of GLSL, GLFW, and GLAD is essential in computer graphics and opens up possibilities for more complex applications in the future.