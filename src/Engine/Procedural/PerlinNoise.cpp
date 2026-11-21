
#include "Engine/Procedural/PerlinNoise.h"

#include <algorithm>
#include <cmath>

namespace Engine {

    PerlinNoise::PerlinNoise(std::uint32_t seed) {
        std::array<std::uint8_t, 256> values{};

        for (int i = 0; i < 256; ++i) {
            values[i] = static_cast<std::uint8_t>(i);
        }

        if (seed == 0) {
            seed = 0x6D2B79F5u;
        }

        for (int i = 255; i > 0; --i) {
            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;

            const int j = static_cast<int>(seed % (i + 1));

            std::swap(values[i], values[j]);
        }

        for (int i = 0; i < 512; ++i) {
            m_permutation[i] = values[i & 255];
        }
    }

    float PerlinNoise::Fade(float t) {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    float PerlinNoise::Lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    float PerlinNoise::Gradient(int hash, float x, float z) {
        switch (hash & 7) {
            case 0: return (x + z) * 0.70710678f;
            case 1: return (-x + z) * 0.70710678f;
            case 2: return (x - z) * 0.70710678f;
            case 3: return (-x - z) * 0.70710678f;
            case 4: return x;
            case 5: return -x;
            case 6: return z;
            default: return -z;
        }
    }

    float PerlinNoise::GetNoise(float x, float z) const {
        const int xFloor = static_cast<int>(std::floor(x));
        const int zFloor = static_cast<int>(std::floor(z));

        const float localX = x - static_cast<float>(xFloor);
        const float localZ = z - static_cast<float>(zFloor);

        const int xi = static_cast<int>(
            static_cast<std::uint32_t>(xFloor) & 255u
        );

        const int zi = static_cast<int>(
            static_cast<std::uint32_t>(zFloor) & 255u
        );

        const int xi1 = (xi + 1) & 255;
        const int zi1 = (zi + 1) & 255;

        const int aa = m_permutation[m_permutation[xi] + zi];
        const int ab = m_permutation[m_permutation[xi] + zi1];
        const int ba = m_permutation[m_permutation[xi1] + zi];
        const int bb = m_permutation[m_permutation[xi1] + zi1];

        const float n00 = Gradient(aa, localX, localZ);
        const float n10 = Gradient(ba, localX - 1.0f, localZ);
        const float n01 = Gradient(ab, localX, localZ - 1.0f);
        const float n11 = Gradient(bb, localX - 1.0f, localZ - 1.0f);

        const float u = Fade(localX);
        const float v = Fade(localZ);

        const float nx0 = Lerp(n00, n10, u);
        const float nx1 = Lerp(n01, n11, u);

        return Lerp(nx0, nx1, v) * 1.41421356f;
    }

    float PerlinNoise::GetFractalNoise(
        float x, 
        float z,
        int octaves, 
        float lacunarity, 
        float persistence
    ) const {
        octaves = std::clamp(octaves, 1, 16);

        if (lacunarity <= 0.0f) {
            lacunarity = 2.0f;
        }

        persistence = std::clamp(persistence, 0.0f, 1.0f);

        float total = 0.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float maxAmplitude = 0.0f;

        for (int i = 0; i < octaves; ++i) {
            total += GetNoise(
                x * frequency,
                z * frequency
            ) * amplitude;

            maxAmplitude += amplitude;

            amplitude *= persistence;
            frequency *= lacunarity;
        }

        return total / maxAmplitude;
    }

}
