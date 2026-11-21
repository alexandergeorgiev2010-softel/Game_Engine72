#pragma once

#include <array>
#include <cstdint>

namespace Engine {

    class PerlinNoise {
    public:
        explicit PerlinNoise(std::uint32_t seed = 1337);

        float GetNoise(float x, float z) const;

        float GetFractalNoise(
            float x,
            float z,
            int octaves = 5,
            float lacunarity = 2.0f,
            float persistence = 0.5f
        ) const;

    private:
        std::array<std::uint8_t, 512> m_permutation{};

        static float Fade(float t);
        static float Lerp(float a, float b, float t);
        static float Gradient(int hash, float x, float z);
    };

}
