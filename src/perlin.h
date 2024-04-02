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
    void ImGui();


private:
    int seed;
    int octaves = 6;
    float persistence = 0.5;
    float scale = 2;
    static float lerp(float amount, float left, float right);
    static float grad(int hash, float x, float y);
    static float fade(float t);
    std::vector<int> p;
};
