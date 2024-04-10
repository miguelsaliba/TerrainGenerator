#ifndef INCLUDE_SRC_CHUNK_H_
#define INCLUDE_SRC_CHUNK_H_

#include "perlin.h"
#include <GL/glew.h>

class Chunk {
public:
    Chunk() = default;
    Chunk(int size, int offset_x, int offset_z, Perlin &perlin, float triangle_size, float texture_interval);
    ~Chunk();
    void generate();
    void draw();
    void bind_data();
    std::pair<int, int> get_coords() const;

private:
    std::vector<float> vertex_data;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;
    int size;
    int offset_x, offset_z;
    float triangle_size = 0.005;
    float texture_interval = 100;
    Perlin *perlin;
};

#endif  // INCLUDE_SRC_CHUNK_H_
