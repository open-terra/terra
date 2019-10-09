#pragma once

#include <cstdint>
#include <random>

#include "Utils/Singleton.hpp"

namespace Terra
{
    class Random : public Utils::Singleton<Random>
    {
    private:
        std::random_device randomDevice;
        std::mt19937 merseneEngine;
        std::uniform_real_distribution<double> uniformNormal;

    public:
        Random();
        ~Random();

        int64_t Int64();
        double Real();
    };
}
