#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
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

    std::vector<std::pair<cl::Platform, cl::Device>> get_device_list()
    {
        // Find all devices
        std::vector<std::pair<cl::Platform, cl::Device>> all_device_list;
        std::vector<cl::Platform> platform_list;
        cl::Platform::get(&platform_list);
        for (const auto& platform : platform_list)
        {
            std::vector<cl::Device> device_list;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &device_list);
            for (const auto& device : device_list)
            {
                all_device_list.emplace_back(std::make_pair(platform, device));
            }
        }

        // Sort devices by type
        std::vector<std::pair<cl::Platform, cl::Device>> ret;
        constexpr cl_device_type type_list[] = {CL_DEVICE_TYPE_GPU, CL_DEVICE_TYPE_ACCELERATOR, CL_DEVICE_TYPE_ALL};
        for (auto type : type_list)
        {
            // Find all devices of type 'type' and move them into `ret`
            for (auto it = all_device_list.begin(); it != all_device_list.end();)
            {
                if (it->second.getInfo<CL_DEVICE_TYPE>() & type)
                {
                    ret.emplace_back(*it);
                    it = all_device_list.erase(it); // `.erase()` returns the next valid iterator
                }
                else
                {
                    ++it;
                }
            }
        }
        return ret;
    }

    cl::Context get_context()
    {
        auto device_list = get_device_list();
        if (device_list.size() > 0)
        {
            return cl::Context(device_list[0].second);
        }

        return cl::Context();
    }
}