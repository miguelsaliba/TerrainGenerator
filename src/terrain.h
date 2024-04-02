#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "perlin.h"
#include <vector>
class Terrain {
public:
    Terrain();
    Terrain(int width, int height);
    void generate_terrain();
    void set_dimensions(int width, int height);
    int size();
    void ImGui();
    Perlin &noise();

private:
    // NOTE: KEEP width and height in this order for ImGui.
    int width;
    int height;
    float triangle_size = 0.025;
    
    Perlin perlin;

    unsigned int VAO, EBO;
    unsigned int VBO[2];
    std::vector<int> indices;
    std::vector<float> vertices;
};

#endif  // TERRAIN_H_
