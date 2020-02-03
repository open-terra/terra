#include "terra/compute/engine_cl.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace terra::compute;

inline cl::Device get_device(size_t device_idx = 0)
{
    auto device_list = engine_cl::get_device_list();
    if (device_list.size() > device_idx)
    {
        return device_list[device_idx].second;
    }

    return cl::Device();
}

engine_cl::engine_cl() :
    device(get_device()), context(device), queue(context, device), sources(),
    program()
{
}

engine_cl::engine_cl(size_t device_idx) :
    device(get_device(device_idx)), context(device), queue(context, device),
    sources(), program()
{
}

void engine_cl::init()
{
    this->program = cl::Program(this->context, this->sources);

    try
    {
        program.build();
    }
    catch (const cl::Error& err)
    {
        std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
        std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
        std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;

        throw err;
    }
}

void engine_cl::add_source(const std::string& file_name)
{
    std::ifstream file(file_name);
    if (file.good())
    {
        std::string* source_code = new std::string(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
        this->sources.push_back((*source_code).c_str());
        // TODO should we not delete this
        // delete source_code;
    }
}

std::vector<std::pair<cl::Platform, cl::Device>> engine_cl::get_device_list()
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
