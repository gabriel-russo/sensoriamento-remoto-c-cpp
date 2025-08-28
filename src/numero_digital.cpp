#include "gdal/gdal_priv.h"
#include <errno.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#define TEXT 1
#if TEXT

#define GREEN "\033[01;32m"
#define YELLOW "\033[01;33m"
#define RED "\033[01;31m"
#define RESET "\033[00m"

#define INFO(msg) std::cout << GREEN << "[INFO]: " << msg << RESET << std::endl;
#define WARN(msg) std::cout << YELLOW << "[WARN]: " << RESET << std::endl;
#define ERR(msg) std::cerr << RED << "[ERROR]: " << msg << RESET << std::endl;

#endif

using namespace std;
using namespace std::filesystem;

static CPLErr write_scanline_into_file(ofstream &out, const vector<int> &data)
{
    if (!out.is_open() || !data.size())
    {
        return CE_Fatal;
    }

    string line;
    // Supondo que todos os números possuam 3 dígitos, reserve o suficiente para escrever
    // os 3 dígitos e mais os espaços entre os números
    line.reserve((data.size() * 3) + data.size());

    for (auto digital_number : data)
    {
        line += to_string(digital_number);
        line += " ";
    }

    // Remove o espaço extra no final da string e substitui por quebra de linha
    line.pop_back();
    line += "\n";

    out << line;

    return CE_None;
}

int main(int argc, char *argv[])
{
    INFO("Iniciando.")

    if (argc < 2)
    {
        ERR("Uso: numero_digital <meu_raster.tif> <arq_destino.txt>.")
        return 1;
    }

    GDALAllRegister();

    if (!exists(argv[1]))
    {
        ERR("Arquivo raster não encontrado! Verifique o argumento.")
        return 1;
    }

    const path file = argv[1];
    ofstream txt(argv[2]);

    INFO("Abrindo dataset.")

    GDALDataset *ds = GDALDataset::FromHandle(GDALOpen(file.c_str(), GA_ReadOnly));

    if (ds == NULL)
    {
        ERR("Error ao abrir dataset!.")
        return 1;
    }

    INFO("Lendo banda.")

    if (ds->GetRasterCount() > 1)
    {
        WARN("Raster contém múltiplas bandas, porém apenas o primeiro canal será processado.")
    }

    GDALRasterBand *raster = ds->GetRasterBand(1);

    INFO("Lendo dimensões e numero mínimo e máximo da banda.")

    const int Ncolumns = raster->GetXSize();
    const int Nrows = raster->GetYSize();
    double minmax[2];
    raster->ComputeRasterMinMax(1, minmax);

    INFO("Escrevendo metadados no arquivo de texto.")

    txt << Nrows << ';' << Ncolumns << ';' << int(minmax[0]) << ';' << int(minmax[1]) << "\n";

    INFO("Iniciando extração de números digitais.")

    CPLErr err_returned;

    vector<int> scanline(static_cast<size_t>(Ncolumns));
    for (int i = 0; i < Nrows; i++)
    {
        // A partir da coluna 0, da linha i; leia a matriz de 1 linha e Ncolumns; Escreva no array scanline
        // a quantidade de dados de 1 por Ncolumns do tipo GDT_Int32 (mesmo que int); com flags opcionais desativadas;
        err_returned = raster->RasterIO(GDALRWFlag::GF_Read, 0, i, Ncolumns, 1, scanline.data(), Ncolumns, 1, GDT_Int32,
                                        0, 0, nullptr);

        if (err_returned)
        {
            ERR("Error ao ler dados da banda. Verifique a integridade do arquivo.")
            GDALClose(ds);
            txt.close();
            return 1;
        }

        err_returned = write_scanline_into_file(txt, scanline);

        if (err_returned)
        {
            ERR("Error ao escrever dados no txt!. Verifique.")
            GDALClose(ds);
            txt.close();
            return 1;
        }
    }

    GDALClose(ds);
    txt.close();
    INFO("Finalizando.")
    return 0;
}
