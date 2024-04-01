#pragma once

#include <vector>

// 2D version of Ken Perlin's noise algorithm:
// https://mrl.cs.nyu.edu/~perlin/noise/

class Perlin {
public:
    Perlin();
    Perlin(unsigned int seed);
    float noise(float x, float y);
    float octaveNoise(float x, float y, int octaves, float persistence);
    void shuffle();
    void ImGui();


private:
    int seed;
    static float lerp(float amount, float left, float right);
    static float grad(int hash, float x, float y);
    static float fade(float t);
    std::vector<int> p;
};
