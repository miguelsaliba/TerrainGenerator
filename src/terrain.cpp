#include "terrain.h"
#include "chunk.h"
#include "perlin.h"
#include <GL/glew.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <imgui.h>
#include <utility>

using std::pair;

Terrain::Terrain() {
    init_arrays();
}

Terrain::Terrain(int w, int h) {
    perlin = Perlin(14);
    init_arrays();
}

void Terrain::init_arrays() {
    colors[0] = glm::vec3(0.2f, 0.3f, 0.8f); // water
    colors[1] = glm::vec3(0.7f, 0.6f, 0.3f); // sand
    colors[2] = glm::vec3(0.3f, 0.5f, 0.2f); // grass
    colors[3] = glm::vec3(0.2f, 0.2f, 0.2f); // rock
    colors[4] = glm::vec3(0.8f, 0.8f, 0.8f); // snow
    heights[0] = -20.0f; // water
    heights[1] = -10.0f; // sand
    heights[2] = 0.0f;  // grass
    heights[3] = 15.0f;  // rock
    heights[4] = 200.0f;  // snow
}

void Terrain::draw() {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto &chunk : chunks) {
        if (chunk.second)
            chunk.second->draw();
    }
}

void Terrain::generate_terrain() {
    std::lock_guard<std::mutex> lock(mutex);
    chunks.clear();
    not_bound.clear();
    to_delete.clear();
    last_chunk_x -= 1;
}

void Terrain::bind_all() {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto &chunk : chunks) {
        if (chunk.second) {
            chunk.second->bind_data();
        }
    }
}

bool Terrain::needs_update(glm::vec3 position) {
    int x = position.x / chunk_size;
    int z = position.z / chunk_size;
    return x != last_chunk_x || z != last_chunk_z;
}

void Terrain::update(glm::vec3 position) {
    int x = position.x / chunk_size;
    int z = position.z / chunk_size;
    last_chunk_x = x;
    last_chunk_z = z;
    // Move chunks that are out of the render distance to the to_delete vector.
    mutex.lock();
    for (auto it = chunks.begin(); it != chunks.end();) {
        if (abs(it->first.first - x) > render_distance || abs(it->first.second - z) > render_distance) {
            to_delete.push_back(std::move(it->second));
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
    mutex.unlock();

    for (int i = x - render_distance; i < x + render_distance; i++) {
        for (int j = z - render_distance; j < z + render_distance; j++) {
            if (chunks.find(pair<int, int>(i, j)) == chunks.end()) {
                unique_ptr<Chunk> chunk = std::make_unique<Chunk>(chunk_size, i, j, perlin, triangle_size);
                chunk->generate();
                not_bound.emplace_back(std::move(chunk));
            }
        }
    }
}

// Bind the chunks that are not bound and move them to the chunks map.
void Terrain::bind() {
    std::lock_guard<std::mutex> chunks_lock(mutex);
    while (!not_bound.empty()) {
        unique_ptr<Chunk> chunk = std::move(not_bound.back());
        chunk->bind_data();
        chunks.insert(std::make_pair(chunk->get_coords(), std::move(chunk)));
        not_bound.pop_back();
    }
}

void Terrain::delete_chunks() {
    to_delete.clear();
}

Perlin &Terrain::noise() {
    return perlin;
}

void Terrain::set_heights(float water, float sand, float grass, float rock, float snow) {
    heights[0] = water;
    heights[1] = sand;
    heights[2] = grass;
    heights[3] = rock;
    heights[4] = snow;
}

bool Terrain::ImGui() {
    bool changed = false;
    if (ImGui::InputInt("Chunk Size", &chunk_size))
        changed = true;
    if (ImGui::InputFloat("Triangle size", &triangle_size))
        changed = true;
    if (ImGui::InputInt("Render distance", &render_distance))
        changed = true;
    ImGui::ColorEdit3("Water", &colors[0][0]);
    ImGui::ColorEdit3("Sand", &colors[1][0]);
    ImGui::ColorEdit3("Grass", &colors[2][0]);
    ImGui::ColorEdit3("Rock", &colors[3][0]);
    ImGui::ColorEdit3("Snow", &colors[4][0]);
    ImGui::DragFloat("Water height", &heights[0], 0.1, -100, 100);
    ImGui::DragFloat("Sand height", &heights[1], 0.1, -100, 100);
    ImGui::DragFloat("Grass height", &heights[2], 0.1, -100, 100);
    ImGui::DragFloat("Rock height", &heights[3], 0.1, -100, 100);
    ImGui::DragFloat("Snow height", &heights[4], 0.1, -100, 100);
    return changed;
}

void Terrain::set_uniforms(Shader &shader) {
    shader.setFloatArray("heights", 5, heights);
    shader.setVec3Array("colors", 5, colors);
}
