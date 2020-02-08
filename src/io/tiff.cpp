#include "terra/io/tiff.hpp"

#include <exception>
#include <iostream>
#include <fstream>

#include "tiff.h"
#include "tiffio.hxx"

#include "terra/types/bitmap.hpp"

terra::bitmap terra::io::tiff::load_tiff(const std::string filepath)
{
    std::ifstream tiff_file;
    tiff_file.open(filepath);

    TIFF* tiff = TIFFStreamOpen("", &tiff_file);
    if (tiff)
    {
        uint32_t w, h, bps, spp;
        size_t num_pixels;
        
        TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h);
        TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bps);
        TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &spp);
        num_pixels = w * h;

        size_t sample_count = num_pixels * (bps / 8) * spp;
        terra::dynarray<uint8_t> raster(sample_count);
        if (bps == 8 && spp == 4)
        {
            if (TIFFReadRGBAImageOriented(tiff, w, h, reinterpret_cast<std::uint32_t*>(raster.data()), ORIENTATION_TOPLEFT))
            {
                throw std::runtime_error("failed to read tiff rgba image");
            }
        }
        else
        {
            uint32_t scanline = TIFFScanlineSize(tiff);
            terra::dynarray<uint8_t> buffer(scanline);
            for (size_t sample = 0; sample < spp; ++sample)
            {
                for (size_t row = 0; row < h; ++row)
                {
                    TIFFReadScanline(tiff, buffer.data(), row);
                    if (bps == 8)
                    {
                        for (size_t col = 0; col < w; ++w)
                        {
                            raster[(row * w) + sample] = buffer[col];
                        }
                    }
                    else if (bps == 16)
                    {
                        uint16_t* raster_view = reinterpret_cast<uint16_t*>(raster.data());
                        uint16_t* buf_view = reinterpret_cast<uint16_t*>(buffer.data());
                        for (size_t col = 0; col < w; ++w)
                        {
                            raster_view[(row * w) + sample] = buf_view[col];
                        }
                    }
                    else if (bps == 32)
                    {
                        uint32_t* raster_view = reinterpret_cast<uint32_t*>(raster.data());
                        uint32_t* buf_view = reinterpret_cast<uint32_t*>(buffer.data());
                        for (size_t col = 0; col < w; ++w)
                        {
                            raster_view[(row * w) + sample] = buf_view[col];
                        }
                    }
                }
            }
        }

        TIFFClose(tiff);

        return terra::bitmap(w, h, bps, spp, num_pixels, raster);
    }

    return terra::bitmap();
}
