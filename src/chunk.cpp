#include "chunk.h"

#include <glm/glm.hpp>
#include <vector>

Chunk::Chunk(int size, int offset_x, int offset_z, Perlin &perlin, float triangle_size, float texture_interval)
    : size(size), offset_x(offset_x), offset_z(offset_z), perlin(&perlin), triangle_size(triangle_size), texture_interval(texture_interval)
{
    vertex_data.reserve(size * size * 6);
    indices.reserve((size - 1) * (size - 1) * 6);
}

Chunk::~Chunk() {
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}

std::pair<int, int> Chunk::get_coords() const {
    return std::make_pair(offset_x, offset_z);
}

void Chunk::generate() {
    int start_x = offset_x * size - 1;
    int start_z = offset_z * size - 1;
    int chunk_size = size + 2;

    vertex_data.clear();
    indices.clear();
    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < chunk_size; j++) {
            float x = (float)(start_x + j) * triangle_size;
            float z = (float)(start_z + i) * triangle_size;
            float u = (float)(start_x + j) / texture_interval;
            float v = (float)(start_z + i) / texture_interval;
            float noise_value = perlin->octaveNoise(x, z) * 2;
            vertex_data.push_back(start_x + j);
            vertex_data.push_back(noise_value);
            vertex_data.push_back(start_z + i);
            vertex_data.push_back(0.0f);
            vertex_data.push_back(0.0f);
            vertex_data.push_back(0.0f);
            vertex_data.push_back(u);
            vertex_data.push_back(v);
        }
    }

    for (int i = 0; i < chunk_size-1; i++) {
        for (int j = 0; j < chunk_size-1; j++) {
            indices.push_back(i       * chunk_size + j    );
            indices.push_back(i       * chunk_size + j + 1);
            indices.push_back((i + 1) * chunk_size + j + 1);
            indices.push_back(i       * chunk_size + j    );
            indices.push_back((i + 1) * chunk_size + j + 1);
            indices.push_back((i + 1) * chunk_size + j    );
        }
    }


    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0(vertex_data[indices[i] * 8], vertex_data[indices[i] * 8 + 1], vertex_data[indices[i] * 8 + 2]);
        glm::vec3 v1(vertex_data[indices[i + 1] * 8], vertex_data[indices[i + 1] * 8 + 1], vertex_data[indices[i + 1] * 8 + 2]);
        glm::vec3 v2(vertex_data[indices[i + 2] * 8], vertex_data[indices[i + 2] * 8 + 1], vertex_data[indices[i + 2] * 8 + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        for (int j = 0; j < 3; ++j) {
            unsigned int index = indices[i + j];
            vertex_data[index * 8 + 3] += normal.x;
            vertex_data[index * 8 + 4] += normal.y;
            vertex_data[index * 8 + 5] += normal.z;
        }
    }
}

void Chunk::bind_data() {
    // Create VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO, VBO, EBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Chunk::draw() {
    int chunk_size = size + 2;
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, chunk_size * chunk_size * 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
