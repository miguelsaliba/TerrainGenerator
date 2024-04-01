#include "terrain.h"
#include "perlin.h"
#include <GL/glew.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <imgui.h>

Terrain::Terrain() : width(100), height(100) { }

Terrain::Terrain(int w, int h) : width(w), height(h) { }

int Terrain::size() {
    return indices.size();
}

void Terrain::set_dimensions(int w, int h) {
    width = w;
    height = h;
}

void Terrain::generate_terrain() {
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &VBO[1]);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    // Create vertices
    vertices.clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float x = (float)j * triangle_size;
            float z = (float)i * triangle_size;
            float noise_value = perlin.octaveNoise(x, z, 7, 0.5f) * 2;
            vertices.push_back(j);
            vertices.push_back(noise_value);
            vertices.push_back(i);
            // std::cout << j<< " " << perlin.octaveNoise(j, i, 4, 0.5f) << " " << i<< std::endl;
        }
    }

    // Create indices
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            indices.push_back(i       * width + j    );
            indices.push_back(i       * width + j + 1);
            indices.push_back((i + 1) * width + j + 1);
            indices.push_back(i       * width + j    );
            indices.push_back((i + 1) * width + j + 1);
            indices.push_back((i + 1) * width + j    );
        }
    }

    // Create normals
    std::vector<glm::vec3> normals(vertices.size()/3, glm::vec3(0.0f));
    for (int i = 0; i < indices.size(); i += 3) {
        glm::vec3 v1(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
        glm::vec3 v2(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]);
        glm::vec3 v3(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        normals[indices[i]] += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
    }
    
    // Create VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO);

    // Bind VAO, VBO, EBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

Perlin &Terrain::noise() {
    return perlin;
}

void Terrain::ImGui() {
    ImGui::InputInt2("Position", &width);
    ImGui::InputFloat("Triangle size", &triangle_size);
}
