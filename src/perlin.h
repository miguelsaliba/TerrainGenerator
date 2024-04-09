#pragma once

#include <vector>

// 2D version of Ken Perlin's noise algorithm:
// https://mrl.cs.nyu.edu/~perlin/noise/

class Perlin {
public:
    Perlin();
    Perlin(unsigned int seed);
    float noise(float x, float y);
    float octaveNoise(float x, float y);
    void shuffle();
    bool ImGui();
    void set_octaves(int octaves) { this->octaves = octaves; };
    void set_persistence(float persistence) { this->persistence = persistence; };
    void set_scale(float scale) { this->scale = scale; };

private:
    int seed;
    int octaves = 10;
    float persistence = 0.512;
    float scale = 15;
    float featureSize = 1;
    static float lerp(float amount, float left, float right);
    static float grad(int hash, float x, float y);
    static float fade(float t);
    std::vector<int> p;
};
