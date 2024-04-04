#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "perlin.h"
#include "shader.h"
#include <vector>
class Terrain {
public:
    Terrain();
    Terrain(int width, int height);
    void generate_terrain();
    void set_dimensions(int width, int height);
    int size();
    bool ImGui();
    void set_shader_data(Shader &shader);
    Perlin &noise();

private:
    void init_arrays();

    // NOTE: KEEP width and height in this order for ImGui.
    int width;
    int height;
    float triangle_size = 0.005;
    
    Perlin perlin;

    unsigned int VAO, EBO;
    unsigned int VBO[2];
    std::vector<int> indices;
    std::vector<float> vertices;
    glm::vec3 colors[5];
    float heights[5];
};

#endif  // TERRAIN_H_
