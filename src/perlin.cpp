#include "perlin.h"
#include <algorithm>
#include <random>
#include <imgui.h>

Perlin::Perlin() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist;

    seed = dist(rng);
    shuffle();
}

Perlin::Perlin(unsigned int seed) : seed(seed) {
    shuffle();
}

bool Perlin::ImGui() {
    // 0 to remove step
    bool changed = false;
    if (ImGui::InputInt("Seed", &seed, 0)) {
        shuffle();
        changed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Random")) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist;

        seed = dist(rng);
        shuffle();
        changed = true;
    }
    if (ImGui::SliderFloat("Persistence", &persistence, 0.0, 1.0))
        changed = true;
    if (ImGui::SliderInt("Octaves", &octaves, 0, 20))
        changed = true;
    if (ImGui::DragFloat("Scale", &scale, 0.05, 0.01, 100))
        changed = true;
    return changed;
}

void Perlin::shuffle() {
    p.clear();
    for (int i = 0; i < 256; i++) {
        p.push_back(i);
    }

    // shuffle using the given seed
    std::shuffle(p.begin(), p.end(), std::default_random_engine(seed));

    // Duplicate the permutation vector
    p.insert(p.end(), p.begin(), p.end());
}

float Perlin::octaveNoise(float x, float y) {
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency, y * frequency) * amplitude;
        
        amplitude *= persistence;
        frequency *= 2;
    }
    return total * scale;
}

float Perlin::noise(float x, float y) {
    int X = (int) std::floor(x) & 255;
    int Y = (int) std::floor(y) & 255;
    int g1 = p[p[X] + Y];
    int g2 = p[p[X + 1] + Y];
    int g3 = p[p[X] + Y + 1];
    int g4 = p[p[X + 1] + Y + 1];

    x -= std::floor(x);
    y -= std::floor(y);

    float u = fade(x);
    float v = fade(y);

    float x1Interp = lerp(u, grad(g1, x, y), grad(g2, x - 1, y));
    float x2Interp = lerp(u, grad(g3, x, y - 1), grad(g4, x - 1, y - 1));

    return lerp(v, x1Interp, x2Interp);
}

float Perlin::lerp(float amount, float left, float right) {
    return left + amount * (right - left);
}

float Perlin::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float Perlin::grad(int hash, float x, float y) {
    int h = hash & 7;
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

