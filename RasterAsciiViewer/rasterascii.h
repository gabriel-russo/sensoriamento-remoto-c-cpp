#ifndef RASTERASCII_H
#define RASTERASCII_H

#include <filesystem>
#include <vector>

namespace RasterAscii
{

struct Raster
{
    signed int Ncols = 0;
    signed int Nrows = 0;
    signed int max = 0;
    signed int min = 0;
    std::vector<signed int> data;
};

Raster *read_ascii(std::filesystem::path file_path);
void print_statistics(Raster &raster);
void set_statistics(Raster &raster);
void gamma(Raster &raster, float factor);
void to_8bit_grayscale(Raster &raster);

} // namespace RasterAscii

#endif // RASTERASCII_H
