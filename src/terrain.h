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

    int width;
    int height;
private:
    Perlin perlin;
    unsigned int VAO, VBO, EBO;
    std::vector<int> indices;
    std::vector<float> vertices;
};

#endif  // TERRAIN_H_
