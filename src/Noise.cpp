#include "pch.h"
#include "Noise.hpp"

// Hash
engine::V2float engine::PerlinNoise::randomVector(int ix, int iy) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1));

    // Create the vector from the angle
    V2float v;
    v.x = (int)(sin(random) * 10000);
    v.y = (int)(tan(random) * 10000);

    return v;
}

// Hash
engine::V2float engine::PerlinNoise::randomGradient(int ix, int iy) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443 + randSeed;

    b ^= a << s | a >> w - s;
    b *= 1911520717 + randSeed;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    V2float v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}

float engine::PerlinNoise::dotGradient(int ix, int iy, float x, float y) {
	V2float gradient = randomGradient(ix, iy);

    float dx = x - (float)ix;
    float dy = y - (float)iy;

    return (dx * gradient.x + dy * gradient.y);
}

engine::PerlinNoise::PerlinNoise(float seed) {
    randSeed = seed;
    return;
}

void engine::PerlinNoise::setPersistence(double p) {
    persistence = p;
}

void engine::PerlinNoise::setOctaves(int o) {
    numOctaves = o;
}

void engine::PerlinNoise::setPeriod(double p) {
    basePeriod = p;
}

void engine::PerlinNoise::setAmplitude(double a) {
    baseAmplitude = a;
}

float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
}

double engine::PerlinNoise::getPersistence() {
    return persistence;
}

int engine::PerlinNoise::getOctaves() {
    return numOctaves;
}

double engine::PerlinNoise::getPeriod() {
    return basePeriod;
}

double engine::PerlinNoise::getAmplitude() {
    return baseAmplitude;
}

double engine::PerlinNoise::getNoise2D(float x, float y) {
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float sx = x - (float)x0;
	float sy = y - (float)y0;

    float n0 = dotGradient(x0,y0,x,y);
    float n1 = dotGradient(x1,y0,x,y);
    float ix0 = interpolate(n0, n1, sx);

    n0 = dotGradient(x0, y1, x, y);
    n1 = dotGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);

    float value = interpolate(ix0, ix1, sy);
    return value;
}

double engine::PerlinNoise::getPerlin2D(int x, int y) {
    
    float noise = 0;

    float freq = 1;
    float amp = 1;
    for (int o = 0; o < numOctaves; ++o) {
        noise += getNoise2D(x * freq / basePeriod, y * freq / basePeriod) * amp * baseAmplitude;
        freq *= 2;
        amp *= .5f;
    }

    if (noise > 1.0f) {
        return 1.0f;
    } else if (noise < -1.0f) {
        return -1.0f;
    }
    return noise;
}
