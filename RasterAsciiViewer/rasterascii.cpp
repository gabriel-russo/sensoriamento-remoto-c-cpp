#include "rasterascii.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

signed int RasterAscii::getColumns() const
{
    return this->Ncols;
}

signed int RasterAscii::getRows() const
{
    return this->Nrows;
}

signed int RasterAscii::getMin() const
{
    return this->min;
}

signed int RasterAscii::getMax() const
{
    return this->max;
}

signed int RasterAscii::getPixel(int line, int column) const
{
    // O vector guarda a matriz achatada, utilize a fórmula:
    // index = row ⋅ nCols + col
    return this->data[line * this->Ncols + column];
}

bool RasterAscii::read_ascii(std::string file_path)
{
    if (!std::filesystem::exists(file_path))
    {
        return false;
    }

    std::ifstream file(file_path);

    if (!file.is_open())
    {
        return false;
    }

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
                this->Nrows = std::stoi(str_buf);
            if (sep_count == 1)
                this->Ncols = std::stoi(str_buf);
            if (sep_count == 2)
                this->min = std::stoi(str_buf);
            if (sep_count == 3)
                this->max = std::stoi(str_buf);
            sep_count++;
            str_buf = "";
            continue;
        }
        str_buf += c;
    }

    this->data.reserve(this->Ncols * this->Nrows);
    line.reserve(256);

    while (std::getline(file, line))
    {
        line += '\0';
        for (const char &c : line)
        {
            if (c == ' ' || c == '\0')
            {
                this->data.push_back(std::stoi(str_buf));
                str_buf = "";
                continue;
            }
            str_buf += c;
        }
    }

    return true;
}

void RasterAscii::print_statistics() const
{
    std::cout << "Matrix: " << this->Nrows << " x " << this->Ncols << std::endl;
    std::cout << "Min: " << this->min << std::endl;
    std::cout << "Max: " << this->max << std::endl;
}

signed int RasterAscii::to_8bit_grayscale(signed int pixel) const
{
    return ((pixel - this->min) / (this->max - (this->min * 1.0))) * 255;
}

void RasterAscii::to_8bit_grayscale()
{
    for (int &pixel : this->data)
    {
        pixel = ((pixel - this->min) / (this->max - (this->min * 1.0))) * 255;
    }
}

float RasterAscii::normalize_pixel(signed int pixel) const
{
    return ((pixel - this->min) / (this->max - (this->min * 1.0)));
}

/* Algoritmos de processamento digital de imagens.

Fontes:
 - Processamento digital de imagens, 3ª edição, 2010.
 - https://www.lcad.icmc.usp.br/~jbatista/procimg/pre_proc.pdf

*/

/* Transformada exponencial (gama)
    s = c ⋅ r^ε

    onde,
    c -> constante correção gama
    r -> píxel
    ε -> Fator de transformação

    Gonzalez (2010, p. 71)
*/
void RasterAscii::gamma(signed short int c, float epsilon)
{
    signed int max = 0;
    signed int min = 0;

    for (signed int &pixel : this->data)
    {
        pixel = (c * std::pow(this->normalize_pixel(pixel), epsilon)) * this->max;

        if (max < pixel)
        {
            max = pixel;
        }

        if (min > pixel)
        {
            min = pixel;
        }
    }
    this->max = max;
    this->min = min;
}

/* Transformada Negativa
    s = (L - 1) - r

    onde,
    L -> Níveis de cinza (Aqui usamos o max, pois não trabalhamos com faixas de intensidade)
    r -> píxel

    Gonzalez (2010, p. 70)
*/
void RasterAscii::negative()
{
    signed int max = 0;
    signed int min = 0;

    for (signed int &pixel : this->data)
    {
        pixel = std::clamp<signed int>((this->max - 1) - pixel, 0, this->max);

        if (max < pixel)
        {
            max = pixel;
        }

        if (min > pixel)
        {
            min = pixel;
        }
    }

    this->max = max;
    this->min = min;
}

/* Transformada logarítmica
    s = c ⋅ log(1 + r)

    onde,
    c -> constante de correção
    r -> píxel

    Gonzalez (2010, p. 71)
*/
void RasterAscii::logarithm(signed short int c)
{
    signed int max = 0;
    signed int min = 0;

    for (signed int &pixel : this->data)
    {
        pixel = static_cast<signed int>((c * std::log(1 + this->normalize_pixel(pixel))) * this->max);

        if (max < pixel)
        {
            max = pixel;
        }

        if (min > pixel)
        {
            min = pixel;
        }
    }

    this->max = max;
    this->min = min;
}

/* Transformada de limiarização
    s = T(r)

    onde,
    T -> Função de transformação
    r -> píxel

    Gonzalez (2010, p. 70)
*/
void RasterAscii::threshold(signed int value)
{
    signed int max = 0;
    signed int min = 0;

    for (signed int &pixel : this->data)
    {
        pixel = pixel < value ? 0 : 255;

        if (max < pixel)
        {
            max = pixel;
        }

        if (min > pixel)
        {
            min = pixel;
        }
    }

    this->max = max;
    this->min = min;
}
