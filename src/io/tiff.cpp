#include "terra/io/tiff.hpp"

#include <exception>
#include <iostream>
#include <fstream>

#include "tiff.h"
#include "tiffio.hxx"

#include "terra/types/bitmap.hpp"

using namespace terra::io::tiff;

terra::bitmap load_tiff(const std::string filepath)
{
    std::ifstream tiff_file;
    tiff_file.open(filepath);

    TIFF* tiff = TIFFStreamOpen("", &tiff_file);
    if (tiff)
    {
        uint32_t w, h;
        size_t num_pixels;
        
        TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h);
        num_pixels = w * h;

        std::unique_ptr<terra::argb_t[]> raster = std::make_unique<terra::argb_t[]>(num_pixels);
        if (TIFFReadRGBAImageOriented(tiff, w, h, reinterpret_cast<std::uint32_t*>(raster.get()), ORIENTATION_TOPLEFT))
        {
            throw std::runtime_error("failed to read tiff rgba image");
        }

        TIFFClose(tiff);

        return terra::bitmap(w, h, num_pixels, raster);
    }

    return terra::bitmap();
}
