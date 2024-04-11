#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "chunk.h"
#include "perlin.h"
#include "shader.h"
#include <memory>
#include <map>
#include <utility>
#include <vector>
#include <mutex>

using std::map;
using std::pair;
using std::unique_ptr;

class Terrain {
public:
    Terrain();
    Terrain(int width, int height);
    void set_uniforms(Shader &shader);
    void set_heights(float water, float sand, float grass, float rock, float snow);
    Perlin &noise();
    bool needs_update(glm::vec3 position);
    void update(glm::vec3 position);
    void draw();
    void bind();
    void bind_all();
    void delete_chunks();
    bool ImGui();

    void generate_terrain();

private:
    void init_arrays();

    // NOTE: KEEP width and height in this order for ImGui.
    int last_chunk_x;
    int last_chunk_z;
    float triangle_size = 0.005;
    int render_distance = 5;
    int chunk_size = 200;
    float texture_interval = 25;

    bool texture_enabled = false;

    mutable std::mutex mutex;
    std::vector<unique_ptr<Chunk>> to_delete;
    std::vector<unique_ptr<Chunk>> not_bound;
    map<pair<int, int>, unique_ptr<Chunk>> chunks;
    Perlin perlin;

    glm::vec3 colors[5];
    float heights[5];
};

#endif  // TERRAIN_H_
