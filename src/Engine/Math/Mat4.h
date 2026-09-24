#pragma once
#include "Engine/Math/Vec4.h"
#include <cmath>

namespace Engine {

    struct Mat4 {
        float m[4][4];

        Mat4() {
            m[0][0] = 1.0f;
            m[0][1] = 0.0f;
            m[0][2] = 0.0f;
            m[0][3] = 0.0f;
            m[1][0] = 0.0f;
            m[1][1] = 1.0f;
            m[1][2] = 0.0f;
            m[1][3] = 0.0f;
            m[2][0] = 0.0f;
            m[2][1] = 0.0f;
            m[2][2] = 1.0f;
            m[2][3] = 0.0f;
            m[3][0] = 0.0f;
            m[3][1] = 0.0f;
            m[3][2] = 0.0f;
            m[3][3] = 1.0f;
        }

        static Mat4 Translation(float x, float y, float z) {
            Mat4 TranslatedMatrix;

            TranslatedMatrix.m[0][3] = x;
            TranslatedMatrix.m[1][3] = y;
            TranslatedMatrix.m[2][3] = z;

            return TranslatedMatrix;
         }


        Vec4 operator*(const Vec4& vector) const {
            return {
                m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z + m[0][3] * vector.w,
                m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z + m[1][3] * vector.w,
                m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z + m[2][3] * vector.w,
                m[3][0] * vector.x + m[3][1] * vector.y + m[3][2] * vector.z + m[3][3] * vector.w
            };
        }

        Mat4 operator*(const Mat4& other) const {
            Mat4 result;

            for (int row = 0; row < 4; row ++) {
                for (int column = 0; column < 4; column ++) {
                    float sum =
                        m[row][0] * other.m[0][column] +
                        m[row][1] * other.m[1][column] +
                        m[row][2] * other.m[2][column] +
                        m[row][3] * other.m[3][column];

                    result.m[row][column] = sum;
                }
            }

            return result;
        }

        static Mat4 RotationX(float angle) {
            Mat4 result;

            float c = std::cos(angle);
            float s = std::sin(angle);

            result.m[1][1] = c;
            result.m[1][2] = -s;
            result.m[2][1] = s;
            result.m[2][2] = c;

            return result;
        }

        static Mat4 RotationY(float angle) {
            Mat4 result;

            float c = std::cos(angle);
            float s = std::sin(angle);

            result.m[0][0] = c;
            result.m[2][0] = -s;
            result.m[0][2] = s;
            result.m[2][2] = c;

            return result;
        }

        static Mat4 RotationZ(float angle) {
            Mat4 result;

            float c = std::cos(angle);
            float s = std::sin(angle);

            result.m[0][0] = c; 
            result.m[0][1] = -s; 
            result.m[1][0] = s;
            result.m[1][1] = c;

            return result;
        }

    };
}