#include "terra/uplift.hpp"

#include <limits>

#include "terra/math/floor.hpp"

using namespace terra;

bitmap_uplift::bitmap_uplift(const terra::bitmap& bitmap) : 
    width(bitmap.get_width()), raster(bitmap.size())
{
    const uint8_t* raster_view = bitmap.get();
    std::copy(raster_view, raster_view + bitmap.size(), raster.data());
}

tfloat bitmap_uplift::at(const terra::vec2& p) const
{
    // TODO probably use something other than a floor here.
    const size_t px = math::floor<size_t>(p.x);
    const size_t py = math::floor<size_t>(p.y);
    const uint8_t val = this->raster[(py * width) + px];
    return static_cast<tfloat>(val) / std::numeric_limits<uint8_t>::max();
}

tfloat terra::noise_uplift::at(const terra::vec2& p) const
{
    // TODO probably use something other than a floor here.
    auto point = p * this->scale;
    const size_t px = std::min(math::floor<size_t>(point.x), this->width - 1);
    const size_t py = std::min(math::floor<size_t>(point.y), this->height - 1);
    return this->noise_map[(py * this->width) + px];
}

terra::linear_uplift::linear_uplift(size_t width,
                                    size_t height,
                                    tfloat min,
                                    tfloat max,
                                    orient orientation) :
    width(width), height(height), min(min), diff(max - min),
    orientation(orientation)
{
}

tfloat terra::linear_uplift::at(const terra::vec2& p) const
{
    tfloat uplift = this->min;
    switch(this->orientation)
    {
        case terra::linear_uplift::orient::up:
            uplift += this->diff * (p.y / height);
            break;
        case terra::linear_uplift::orient::down:
            uplift += this->diff * (1.0f - (p.y / height));
            break;
        case terra::linear_uplift::orient::left:
            uplift += this->diff * (p.x / width);
            break;
        case terra::linear_uplift::orient::right:
            uplift += this->diff * (1.0f - (p.x / width));
            break;
    }

    return uplift;
}

uplift::uplift() : factor(0.0f), uplifts(0), points(nullptr), heights(nullptr)
{
}

void uplift::update()
{
    for (size_t i = 0; i < points->size(); ++i)
    {
        (*this->heights)[i] += factor * this->uplifts[i];
    }
}

#ifdef TERRA_USE_OPENCL
// REQUIRES that "uplift.cl" is loaded and built.
void  uplift::update(terra::compute::engine_cl& engine)
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
