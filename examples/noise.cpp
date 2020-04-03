#include "terra/terra.hpp"

int main(int argc, char** argv)
{
    terra::noise::ridged_noise source(2552);
    source.set_octaves(8);
    auto noise = source.noise(23, 5, 2552, 512, 512, 1, 0.5);

    auto colors_bitmap = terra::io::load_image("resources/terrain_colors.png");
    auto* colors = colors_bitmap.get<terra::rgb_t>();

    terra::coloriser coloriser;
    auto noise_bitmap = coloriser.raster(512, 512, -1.0f, 1.0f, noise, colors);
    terra::io::write_image("noise.png", noise_bitmap);

    return 0;
}
