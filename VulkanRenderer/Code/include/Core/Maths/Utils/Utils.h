#pragma once

#define _USE_MATH_DEFINES
#include "math.h"
#include <limits>

// Simple colors

constexpr auto RED = "\33[31m";
constexpr auto GREEN = "\33[32m";
constexpr auto BLUE = "\33[34m";
constexpr auto YELLOW = "\33[33m";

// Light colors

constexpr auto L_RED = "\33[91m";
constexpr auto L_GREEN = "\33[92m";
constexpr auto L_BLUE = "\33[94m";
constexpr auto L_YELLOW = "\33[93m";

// End color (White)

constexpr auto END = "\033[0m";

namespace Math
{
    namespace Utils
    {
        /// <summary> Return a verry small number </summary>
        /// <returns></returns>
        constexpr const float EPSILON = std::numeric_limits<float>::epsilon();

        /// <summary> Converts an angle from degres to radians </summary> 
        /// <param name="_angle"> : Angle you want to convert</param>
        /// <returns></returns>
        [[nodiscard]] constexpr __forceinline const float DegToRad(const float& _angle)
        {
            return static_cast<const float>((_angle)*M_PI / 180.0);
        }

        /// <summary> Converts an angle from radians to degres </summary>
        /// <param name="_angle"> : Angle you want to convert</param>
        /// <returns></returns>
        [[nodiscard]] constexpr __forceinline const float RadToDeg(const float& _angle)
        {
            return static_cast<const float>((_angle) * 180.0 / M_PI);
        }
    }
}