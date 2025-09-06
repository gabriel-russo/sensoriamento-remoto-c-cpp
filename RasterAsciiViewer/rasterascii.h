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
    std::vector<float> p_data;

  public:
    RasterAscii() = default;
    ~RasterAscii() = default;
    signed int getColumns() const;
    signed int getRows() const;
    signed int getMin() const;
    signed int getMax() const;
    signed int getPixel(int line, int column) const;

    // IO
    bool read_ascii(std::string file_path);

    // Processamento de imagens
    void to_8bit_grayscale();
    signed int to_8bit_grayscale(signed int pixel) const;
    float normalize_pixel(signed int pixel) const;

    // Transformadas
    void gamma(signed short int c, float epsilon);
    void negative();
    void logarithm(signed short int c);
    void threshold(signed int value);

    // Status
    void print_statistics() const;
};

#endif // RASTERASCII_H
