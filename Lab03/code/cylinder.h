#ifndef CYLINDER_H
#define CYLINDER_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#define PI 3.1416

class Cylinder {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // Constructor
    Cylinder(float baseRadius = 0.3f, float topRadius = 0.3f, float height = 1.0f, int sectorCount = 50,
        glm::vec3 amb = glm::vec3(1.0f, 0.5f, 0.0f),
        glm::vec3 diff = glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3 spec = glm::vec3(1.0f, 0.0f, 0.0f),
        float shiny = 32.0f)
        : verticesStride(24) {
        set(baseRadius, topRadius, height, sectorCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &cylinderVAO);
        glBindVertexArray(cylinderVAO);

        // Vertex Buffer Object (VBO)
        unsigned int cylinderVBO;
        glGenBuffers(1, &cylinderVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBufferData(GL_ARRAY_BUFFER, getVertexSize(), getVertices(), GL_STATIC_DRAW);

        // Element Buffer Object (EBO)
        unsigned int cylinderEBO;
        glGenBuffers(1, &cylinderEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndices(), GL_STATIC_DRAW);

        // Vertex Attribute Pointers
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        int stride = getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);               // Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3)); // Normal

        // Unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Destructor
    ~Cylinder() {}

    // Set cylinder properties
    void set(float baseRadius, float topRadius, float height, int sectors, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny) {
        this->baseRadius = baseRadius > 0 ? baseRadius : 1.0f;
        this->topRadius = topRadius > 0 ? topRadius : 1.0f;
        this->height = height > 0 ? height : 1.0f;
        this->sectorCount = sectors > 2 ? sectors : 3; // Minimum 3 sectors for valid geometry
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // Draw the cylinder
    void drawCylinder(Shader& lightingShader, glm::mat4 model) const {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

private:
    unsigned int cylinderVAO;
    float baseRadius, topRadius, height;
    int sectorCount;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    std::vector<float> coordinates;
    int verticesStride;

    // Build vertices and normals
    void buildCoordinatesAndIndices() {
        float sectorStep = 2 * PI / sectorCount;
        float sectorAngle;

        // Top and bottom vertices
        for (int i = 0; i <= 1; ++i) {
            float y = (i == 0) ? height / 2 : -height / 2;
            float radius = (i == 0) ? topRadius : baseRadius;

            for (int j = 0; j < sectorCount; ++j) {
                sectorAngle = j * sectorStep;
                float x = radius * cosf(sectorAngle);
                float z = radius * sinf(sectorAngle);

                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                // Approximate normals
                glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
            }
        }

        // Indices for side surfaces
        for (int i = 0; i < sectorCount; ++i) {
            int topIndex = i;
            int bottomIndex = i + sectorCount;

            indices.push_back(topIndex);
            indices.push_back(bottomIndex);
            indices.push_back((i + 1) % sectorCount + sectorCount);

            indices.push_back(topIndex);
            indices.push_back((i + 1) % sectorCount + sectorCount);
            indices.push_back((i + 1) % sectorCount);
        }

        // Top cap
        int topCenter = 2 * sectorCount;
        coordinates.push_back(0.0f); coordinates.push_back(height / 2); coordinates.push_back(0.0f); // Top center
        normals.push_back(0.0f); normals.push_back(1.0f); normals.push_back(0.0f);
        for (int i = 0; i < sectorCount; ++i) {
            indices.push_back(topCenter);
            indices.push_back(i);
            indices.push_back((i + 1) % sectorCount);
        }

        // Bottom cap
        int bottomCenter = 2 * sectorCount + 1;
        coordinates.push_back(0.0f); coordinates.push_back(-height / 2); coordinates.push_back(0.0f); // Bottom center
        normals.push_back(0.0f); normals.push_back(-1.0f); normals.push_back(0.0f);
        for (int i = 0; i < sectorCount; ++i) {
            indices.push_back(bottomCenter);
            indices.push_back(i + sectorCount);
            indices.push_back((i + 1) % sectorCount + sectorCount);
        }
    }

    void buildVertices() {
        for (size_t i = 0; i < coordinates.size(); i += 3) {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }
    }

    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const float* getVertices() const { return vertices.data(); }
    const unsigned int* getIndices() const { return indices.data(); }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }
    int getVerticesStride() const { return verticesStride; }
};

#endif /* CYLINDER_H */