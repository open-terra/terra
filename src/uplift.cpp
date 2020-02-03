#include "terra/uplift.hpp"

#include "terra/math/floor.hpp"

using namespace terra;

uplift::uplift() :
    uplifts(), uplift_map(nullptr), points(nullptr), heights(nullptr)
{
}

uplift::uplift(const terra::bitmap& uplift_map,
               const std::vector<terra::vec2>& points,
               std::vector<double>& heights) :
    uplifts(), uplift_map(&uplift_map), points(&points), heights(&heights)
{
    const size_t bitmap_width = uplift_map.width;
    std::vector<uint8_t> uplift = uplift_map.get_channel(terra::bitmap::channel::alpha);

    uplifts.reserve(points.size());
    for (const auto& p : points)
    {
        // TODO probably use something other than a simple point sample here.
        const size_t px = math::floor<size_t>(p.x);
        const size_t py = math::floor<size_t>(p.y);
        uplifts.push_back(uplift[(py * bitmap_width) + px]);
    }
}

void uplift::update()
{
    for (size_t i = 0; i < points->size(); ++i)
    {
        (*this->heights)[i] += this->uplifts[i];
    }
}

#ifdef USE_OPENCL
// REQUIRES that "uplift.cl" is loaded and built.
void uplift::update(terra::compute::engine_cl& engine)
{
    const size_t heights_length = heights->size();

    // init buffers
    cl::Context context = engine.get_context();
    cl::CommandQueue queue = engine.get_queue();
    cl::Buffer buffer_uplifts(context, CL_MEM_READ_ONLY, heights_length);
    cl::Buffer buffer_heights(context, CL_MEM_READ_WRITE, heights_length);

    // write buffers
    queue.enqueueWriteBuffer(buffer_uplifts,
                             CL_TRUE,
                             0,
                             heights_length,
                             uplifts.data());
    queue.enqueueWriteBuffer(buffer_heights,
                             CL_TRUE,
                             0,
                             heights_length,
                             heights->data());

    // create kernel and set args
    cl::Kernel kernel_update = cl::Kernel(engine.get_program(), "uplift_update");
    kernel_update.setArg(0, buffer_uplifts);
    kernel_update.setArg(1, buffer_heights);

    // run kernel
    queue.enqueueNDRangeKernel(kernel_update,
                               cl::NullRange,
                               cl::NDRange(heights_length),
                               cl::NullRange);

    // read result
    queue.enqueueReadBuffer(buffer_heights, CL_TRUE, 0, heights_length, this->heights->data());
}
#endif
