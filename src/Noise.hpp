#pragma once
#pragma once
#include "Fundamentals.hpp"

#include <cstdio>
#include <cmath>
#include <cstdlib>
namespace engine {
    class ENGINE_API PerlinNoise {
    private:
        int randSeed = 0;
        int numOctaves = 7;
        double persistence = 0.5;
        double basePeriod = 400.0f;
        double baseAmplitude = 1;

        V2float randomVector(int ix, int iy);

        V2float randomGradient(int ix, int iy);
        float dotGradient(int ix, int iy, float x, float y);

    public:
        PerlinNoise(float seed);
        void setPersistence(double p);
        void setOctaves(int o);
        void setPeriod(double p);
        void setAmplitude(double a);

        double getPersistence();
        int getOctaves();
        double getPeriod();
        double getAmplitude();

        double getNoise2D(float x, float y);
        double getPerlin2D(int x, int y);
    };
};