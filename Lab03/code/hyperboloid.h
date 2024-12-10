#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

#define PI 3.1416

class Hyperboloid {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    // Constructor
    Hyperboloid(float a = 1.0f, float b = 1.0f, float c = 1.0f, int uSegments = 50, int vSegments = 50)
        : a(a), b(b), c(c), uSegments(uSegments), vSegments(vSegments) {
        generateVertices();
        generateIndices();
        setupBuffers();
    }

    // Destructor
    ~Hyperboloid() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void drawHyperboloid(Shader& shader, glm::mat4 model) const {
        shader.use();
        shader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    float a, b, c;
    int uSegments, vSegments;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    void generateVertices() {
        for (int i = 0; i <= vSegments; ++i) {
            float v = -2.0f + i * (4.0f / vSegments);
            for (int j = 0; j <= uSegments; ++j) {
                float u = j * (2.0f * PI / uSegments);

                // Parametric equations for vertical orientation
                float x = a * cosh(v) * cos(u);
                float z = b * cosh(v) * sin(u);
                float y = c * sinh(v);


                //normals
                float nx = x / (a * a);
                float nz = -z / (c * c);
                float ny = y / (b * b);

                // Add to vertex list
                vertices.push_back(x);
                vertices.push_back(y); // Ensure correct y-axis alignment
                vertices.push_back(z);
                vertices.push_back(nx);
                vertices.push_back(ny);
                vertices.push_back(nz);
            }
        }
    }


    void generateIndices() {
        for (int i = 0; i < vSegments; ++i) {
            for (int j = 0; j < uSegments; ++j) {
                int p1 = i * (uSegments + 1) + j;
                int p2 = p1 + uSegments + 1;

                indices.push_back(p1);
                indices.push_back(p2);
                indices.push_back(p1 + 1);

                indices.push_back(p1 + 1);
                indices.push_back(p2);
                indices.push_back(p2 + 1);
            }
        }
    }

    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
    }
};

#endif /* HYPERBOLOID_H */