#include "terrain.h"
#include "perlin.h"
#include <GL/glew.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <imgui.h>

Terrain::Terrain() : width(100), height(100) {
    init_arrays();
}

Terrain::Terrain(int w, int h) : width(w), height(h) {
    perlin = Perlin(14);
    init_arrays();
}

void Terrain::init_arrays() {
    colors[0] = glm::vec3(0.2f, 0.3f, 0.8f); // water
    colors[1] = glm::vec3(0.6f, 0.6f, 0.1f); // sand
    colors[2] = glm::vec3(0.3f, 0.5f, 0.2f); // grass
    colors[3] = glm::vec3(0.2f, 0.2f, 0.2f); // rock
    colors[4] = glm::vec3(0.8f, 0.8f, 0.8f); // snow
    heights[0] = -20.0f; // water
    heights[1] = -10.0f; // sand
    heights[2] = 0.0f;  // grass
    heights[3] = 15.0f;  // rock
    heights[4] = 200.0f;  // snow
}

int Terrain::size() {
    return indices.size();
}

void Terrain::set_dimensions(int w, int h) {
    width = w;
    height = h;
}

void Terrain::generate_terrain() {
    glDeleteBuffers(2, &VBO[0]);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    // Create vertices
    vertices.clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float x = (float)j * triangle_size;
            float z = (float)i * triangle_size;
            float noise_value = perlin.octaveNoise(x, z) * 2;
            vertices.push_back(j);
            vertices.push_back(noise_value);
            vertices.push_back(i);
            // std::cout << j<< " " << perlin.octaveNoise(j, i, 4, 0.5f) << " " << i<< std::endl;
        }
    }

    // Create indices
    indices.clear();
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);
}

Perlin &Terrain::noise() {
    return perlin;
}

bool Terrain::ImGui() {
    bool changed = false;
    if (ImGui::InputInt2("Size", &width))
        changed = true;
    if (ImGui::InputFloat("Triangle size", &triangle_size))
        changed = true;
    ImGui::ColorEdit3("Water", &colors[0][0]);
    ImGui::ColorEdit3("Sand", &colors[1][0]);
    ImGui::ColorEdit3("Grass", &colors[2][0]);
    ImGui::ColorEdit3("Rock", &colors[3][0]);
    ImGui::ColorEdit3("Snow", &colors[4][0]);
    ImGui::DragFloat("Water height", &heights[0], 0.1, -50, 50);
    ImGui::DragFloat("Sand height", &heights[1], 0.1, -50, 50);
    ImGui::DragFloat("Grass height", &heights[2], 0.1, -50, 50);
    ImGui::DragFloat("Rock height", &heights[3], 0.1, -50, 50);
    ImGui::DragFloat("Snow height", &heights[4], 0.1, -50, 50);
    return changed;
}

void Terrain::set_shader_data(Shader &shader) {
    shader.setFloatArray("heights", 5, heights);
    shader.setVec3Array("colors", 5, colors);
}
