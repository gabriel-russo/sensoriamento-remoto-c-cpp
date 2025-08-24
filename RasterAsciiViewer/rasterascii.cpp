#include "rasterascii.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

RasterAscii::Raster RasterAscii::read_ascii(std::filesystem::path file_path)
{
    if (!std::filesystem::exists(file_path))
    {
        return RasterAscii::Raster();
    }

    std::ifstream file(file_path);

    if (!file.is_open())
    {
        return RasterAscii::Raster();
    }

    RasterAscii::Raster raster;
    std::string line;
    std::string str_buf;

    raster.Nrows = 50;
    raster.Ncols = 50;

    raster.data.reserve(raster.Ncols * raster.Nrows);
    line.reserve(raster.Ncols * 5);
    str_buf.reserve(3);

    while (std::getline(file, line))
    {
        for (int i = 0; i < line.size(); i++)
        {
            const char c = line[i];

            if (c == ' ' || (i + 1) == line.size())
            {
                raster.data.push_back(std::stoi(str_buf));

                str_buf = "";

                continue;
            }

            str_buf += c;
        }
    }

    return raster;
}

void RasterAscii::print_statistics(RasterAscii::Raster &raster)
{
    std::cout << "Matrix: " << raster.Nrows << " x " << raster.Ncols << std::endl;
    std::cout << "Min: " << raster.min << std::endl;
    std::cout << "Max: " << raster.max << std::endl;
}

void RasterAscii::set_statistics(RasterAscii::Raster &raster)
{
    raster.min = *std::min_element(raster.data.begin(), raster.data.end());
    raster.max = *std::max_element(raster.data.begin(), raster.data.end());
}

void RasterAscii::gamma(RasterAscii::Raster &raster, float factor)
{
    for (int i = 0; i < raster.data.size(); i++)
    {
        raster.data[i] = std::clamp<int>(std::ceil(raster.data[i] * factor), 0, 255);
    }
}

void RasterAscii::to_8bit_grayscale(RasterAscii::Raster &raster)
{
    for (int i = 0; i < raster.data.size(); i++)
    {
        raster.data[i] = ((raster.data[i] - raster.min) / (raster.max - (raster.min * 1.0))) * 255;
    }
}
