#include "rasterascii.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

RasterAscii::Raster *RasterAscii::read_ascii(std::filesystem::path file_path)
{
    if (!std::filesystem::exists(file_path))
    {
        return nullptr;
    }

    std::ifstream file(file_path);

    if (!file.is_open())
    {
        return nullptr;
    }

    RasterAscii::Raster *raster = new RasterAscii::Raster();
    std::string line;

    std::string metadata;
    std::getline(file, metadata);
    metadata += '\0';

    std::string str_buf;
    str_buf.reserve(3);

    short int sep_count = 0;
    for (const char &c : metadata)
    {
        if (c == ';' || c == '\0')
        {
            if (sep_count == 0)
                raster->Nrows = std::stoi(str_buf);
            if (sep_count == 1)
                raster->Ncols = std::stoi(str_buf);
            if (sep_count == 2)
                raster->min = std::stoi(str_buf);
            if (sep_count == 3)
                raster->max = std::stoi(str_buf);
            sep_count++;
            str_buf = "";
            continue;
        }
        str_buf += c;
    }

    raster->data.reserve(raster->Ncols * raster->Nrows);
    line.reserve(256);

    while (std::getline(file, line))
    {
        line += '\0';
        for (const char &c : line)
        {
            if (c == ' ' || c == '\0')
            {
                raster->data.push_back(std::stoi(str_buf));
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
