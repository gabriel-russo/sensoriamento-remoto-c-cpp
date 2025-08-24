#include "boost/filesystem.hpp"
#include "gdal/gdal_priv.h"
#include <errno.h>
#include <fstream>
#include <iostream>

#define LOG(msg) std::cout << msg << std::endl;
#define ERR(msg) std::cerr << msg << std::endl;

using namespace boost::filesystem;
using namespace std;

CPLErr write_row_digital_numbers(ofstream &out, const vector<int> &data)
{
    if (!out.is_open() || !data.size())
    {
        return CE_Fatal;
    }

    std::string line;
    line.reserve((data.size() * 2));

    for (auto digital_number : data)
    {
        line += to_string(digital_number);
        line += " ";
    }

    line.pop_back();

    out << line;
    out << "\n";

    return CE_None;
}

int main(int argc, char *argv[])
{
    LOG("Iniciando...");

    if (argc < 2)
    {
        ERR("Uso: numero_digital <meu_raster.tif> <arq_destino.txt>");
        exit(1);
    }

    GDALAllRegister();

    if (!exists(argv[1]))
    {
        ERR("Arquivo raster não encontrado! Verifique o argumento.");
        exit(1);
    }

    const path file = argv[1];
    ofstream txt;
    txt.open(argv[2]);

    LOG("Abrindo dataset...");
    GDALDataset *ds = GDALDataset::FromHandle(GDALOpen(file.c_str(), GA_ReadOnly));

    LOG("Lendo banda no canal 1...");
    GDALRasterBand *raster = ds->GetRasterBand(1);

    LOG("Lendo dimensões da banda...");
    const int Ncolumns = raster->GetXSize();
    const int Nrows = raster->GetYSize();

    printf("Banda com dimensões de: %d x %d\n", Nrows, Ncolumns);

    LOG("Iniciando extração de números digitais...");

    CPLErr err_returned;

    vector<int> scanline(Ncolumns);
    for (int i = 0; i < Nrows; i++)
    {
        err_returned = raster->RasterIO(GDALRWFlag::GF_Read, 0, i, Ncolumns, 1, scanline.data(), Ncolumns, 1, GDT_Int32, 0, 0, nullptr);

        if (err_returned)
        {
            ERR("Error ao ler dados da banda!. Verifique a integridade do arquivo.");
            GDALClose(ds);
            txt.close();
            exit(1);
        }

        err_returned = write_row_digital_numbers(txt, scanline);

        if (err_returned)
        {
            ERR("Error ao escrever dados no txt!. Verifique.");
            GDALClose(ds);
            txt.close();
            exit(1);
        }
    }

    GDALClose(ds);
    txt.close();
    LOG("Finalizando...");
    return 0;
}
