#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#define CL_HPP_ENABLE_EXCEPTIONS
#include "cl/cl2.hpp"

namespace terra::compute
{
    class engine_cl
    {
        cl::Device device;
        cl::Context context;
        cl::CommandQueue queue;
        cl::Program::Sources sources;
        cl::Program program;

    public:
        engine_cl();
        engine_cl(size_t device_idx = 0);

        static std::vector<std::pair<cl::Platform, cl::Device>> get_device_list();

        void init();

        void add_source(const std::string& file_name);

        constexpr cl::Context& get_context()
        {
            return this->context;
        }
        constexpr cl::CommandQueue& get_queue()
        {
            return this->queue;
        }
        constexpr cl::Program::Sources& get_sources()
        {
            return this->sources;
        }
        constexpr cl::Program& get_program()
        {
            return this->program;
        }
    };
}