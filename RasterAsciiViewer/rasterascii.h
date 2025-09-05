#ifndef RASTERASCII_H
#define RASTERASCII_H

#include <string>
#include <vector>

class RasterAscii
{
  private:
    signed int Ncols = 0;
    signed int Nrows = 0;
    signed int max = 0;
    signed int min = 0;
    std::vector<signed int> data;

  public:
    RasterAscii();
    ~RasterAscii();
    signed int getColumns();
    signed int getRows();
    signed int getPixel(int line, int column);

    // IO
    bool read_ascii(std::string file_path);

    // Processamento de imagens
    void to_8bit_grayscale();
    signed int to_8bit_grayscale(signed int pixel);

    // Pré-processamento
    void gamma(signed short int c, float epsilon);
    void negative();

    // Status
    void print_statistics();
};

#endif // RASTERASCII_H
