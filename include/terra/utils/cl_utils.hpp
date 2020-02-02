#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "cl/cl2.hpp"

namespace terra::utils
{
    struct cl_data
    {
        cl::Context context;
        cl::CommandQueue queue;
        cl::Program::Sources sources;
        cl::Program program;
    };

    void add_source(cl::Program::Sources& sources, const std::string& file_name)
    {
        std::ifstream file(file_name);
        if (file.good())
        {
            std::string* source_code = new std::string(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
            sources.push_back((*source_code).c_str());
            // TODO should we not delete this
            // delete source_code;
        }
    }

    cl::Context get_context(int platform_id, int device_id)
    {
        std::vector<cl::Platform> platforms;

        cl::Platform::get(&platforms);

        for (size_t i = 0; i < platforms.size(); ++i)
        {
            std::vector<cl::Device> devices;
            platforms[i].getDevices(static_cast<cl_device_type>(CL_DEVICE_TYPE_ALL), &devices);

            for (size_t j = 0; j < devices.size(); ++j)
            {
                if ((i == platform_id) && (j == device_id))
                {
                    return cl::Context({ devices[j] });
                }
            }
        }

        return cl::Context();
    }
}