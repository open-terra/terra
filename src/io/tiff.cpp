#include "terra/io/tiff.hpp"

#include <exception>
#include <iostream>
#include <fstream>

#include "tiffio.hxx"

#include "terra/utils/template_helpers.hpp"

using namespace terra::io::tiff;

template<typename T, terra::utils::enable_if_type_equal_t<T, terra::rgba>>
terra::bitmap<T> load_tiff(const std::string filepath)
{
    std::ifstream tiff_file;
    tiff_file.open(filepath);

    TIFF* tiff = TIFFStreamOpen("", &tiff_file);
    if (tiff)
    {
        uint32 w, h;
        uint8_t bpc;
        size_t npixels;
        std::vector<uint32_t> raster;
        
        TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h);
        TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bpc);
        npixels = w * h;
        raster.reserve(npixels);
        if (TIFFReadRGBAImage(tiff, w, h, &raster[0], 0))
        {
            throw std::runtime_error("failed to read tiff rgba image");
        }

        TIFFClose(tiff);
    }
}