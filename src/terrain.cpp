#include "terrain.h"
#include "perlin.h"
#include <GL/glew.h>

Terrain::Terrain() : width(100), height(100) { }

Terrain::Terrain(int w, int h) : width(w), height(h) { }

int Terrain::size() {
    return vertices.size();
}

void Terrain::set_dimensions(int w, int h) {
    width = w;
    height = h;
}

void Terrain::generate_terrain() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    // Create vertices
    vertices.clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float x = (float)j * 0.07;
            float z = (float)i * 0.07;
            float noise_value = perlin.octaveNoise(x, z, 7, 0.5f) * 2;
            vertices.push_back(j);
            vertices.push_back(noise_value);
            vertices.push_back(i);
            // std::cout << j<< " " << perlin.octaveNoise(j, i, 4, 0.5f) << " " << i<< std::endl;
        }
    }

    // Create indices
    std::vector<unsigned int> indices;
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

    // Create VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO, VBO, EBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}
