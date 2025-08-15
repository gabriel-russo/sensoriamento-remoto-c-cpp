// Copyright (C) 2025 Gabriel Russo
// See end of file for extended copyright information.

#include <iostream>
#include <memory>
#include <errno.h>
#include <boost/filesystem.hpp>
#include <gdal/gdal_priv.h>

void divider()
{
  std::cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫" << std::endl;
}

/* Nota: Será usado o Sistema Internacional - SI para conversão de tamanho de arquivos */

float kilobytes(uintmax_t bytes)
{
  return bytes / 1000;
}

float megabytes(uintmax_t bytes)
{
  // Divido para um milhão.
  return bytes / 1000000;
}

float gigabytes(uintmax_t bytes)
{
  // Dividido para um bilhão.
  return bytes / 1000000000;
}

std::string data_type_description(int datatype_enum_int)
{
  switch (datatype_enum_int)
  {
  case 0:
    return "Desconhecido - Tipo não especificado";
  case 1:
    return "Byte - 256 níveis de cinza (0 a 255) - Somente positivo";
  case 2:
    return "UInt16 - 65536 níveis (0 a 65535) - Somente positivo";
  case 3:
    return "Int16 - 65536 níveis (-32768 a 32767) - Negativo e positivo";
  case 4:
    return "UInt32 - 4294967296 níveis (0 a 4294967295) - Somente positivo";
  case 5:
    return "Int32 - 4294967296 níveis (-2147483648 a 2147483647) - Negativo e positivo";
  case 6:
    return "Float32 - Valores reais de precisão simples - Negativo e positivo";
  case 7:
    return "Float64 - Valores reais de precisão dupla - Negativo e positivo";
  case 8:
    return "CInt16 - Complexo (2xInt16) - 65536 níveis por componente - Negativo e positivo";
  case 9:
    return "CInt32 - Complexo (2xInt32) - 4294967296 níveis por componente - Negativo e positivo";
  case 10:
    return "CFloat32 - Complexo (2xFloat32) - Valores reais - Negativo e positivo";
  case 11:
    return "CFloat64 - Complexo (2xFloat64) - Valores reais - Negativo e positivo";
  case 12:
    return "UInt64 - 18446744073709551616 níveis (0 a 18446744073709551615) - Somente positivo";
  case 13:
    return "Int64 - 18446744073709551616 níveis (-9223372036854775808 a 9223372036854775807) - Negativo e positivo";
  case 14:
    return "Int8 - 256 níveis (-128 a 127) - Negativo e positivo";
  default:
    return "Desconhecido - Tipo não identificado";
  }
}

int main(int argc, const char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Especificar parametros. Ex.: raster meu_raster.tif" << std::endl;
    return 1;
  }

  // Registrar drivers para leitura de dados
  GDALAllRegister();

  const boost::filesystem::path raster_file = argv[1];

  if (!boost::filesystem::exists(raster_file))
  {
    std::cerr << "ERROR: Raster não foi encontrado." << std::endl;
    return 1;
  };

  std::unique_ptr<GDALDataset> r_ds(GDALDataset::FromHandle(GDALOpen(raster_file.c_str(), GA_ReadOnly)));

  if (!r_ds)
  {
    std::cerr << "ERROR: não foi possível ler raster!" << std::endl;
    return 1;
  }

  divider();

  std::cout << "┣ " << raster_file.filename().string() << std::endl;
  std::cout << "┣━━━━━      Informações do raster       ━━━━━┫" << std::endl;
  std::cout << "┣ Tipo do número digital do píxel: " << data_type_description(r_ds->GetRasterBand(1)->GetRasterDataType()) << std::endl;
  std::cout << "┣ Quantidade de Linhas: " << r_ds->GetRasterYSize() << std::endl;
  std::cout << "┣ Quantidade de Colunas: " << r_ds->GetRasterXSize() << std::endl;
  std::cout << "┣ Quantidade de Bandas: " << r_ds->GetRasterCount() << std::endl;
  std::cout << "┣ Pixels sem valor são interpretados com: " << r_ds->GetRasterBand(1)->GetNoDataValue() << std::endl;

  for (int i = 1; i <= r_ds->GetRasterCount(); i++)
  {
    double minmax[2] = {-999, -999};
    r_ds->GetRasterBand(i)->ComputeRasterMinMax(1, minmax);
    printf("┣ Banda %d - Valor mínimo de pixel é de %.2f e máximo de %.2f\n", i, minmax[0], minmax[1]);
  }

  std::cout << "┣━━━━━      Informações do arquivo      ━━━━━┫" << std::endl;
  const uintmax_t file_size = boost::filesystem::file_size(raster_file);

  if (kilobytes(file_size) >= 1000000)
  {
    std::cout << "┣ Tamanho do arquivo aprox.: " << gigabytes(file_size) << " GB" << std::endl;
  }

  if (kilobytes(file_size) >= 1000 && kilobytes(file_size) < 1000000)
  {
    std::cout << "┣ Tamanho do arquivo aprox.: " << megabytes(file_size) << " MB" << std::endl;
  }

  if (kilobytes(file_size) < 1000)
  {
    std::cout << "┣ Tamanho do arquivo aprox.: " << kilobytes(file_size) << " kb" << std::endl;
  }

  std::cout << "┣ Nome curto do driver: " << r_ds->GetDriver()->GetDescription() << std::endl;
  std::cout << "┣ Nome longo do driver: " << r_ds->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME) << std::endl;
  std::cout << "┣━━━ Informações do sistema de referência ━━━┫" << std::endl;

  if (r_ds->GetSpatialRef() != NULL)
  {
    std::cout << "┣ Nome: " << r_ds->GetSpatialRef()->GetName() << std::endl;
    std::cout << "┣ Código: " << r_ds->GetSpatialRef()->GetAuthorityName("PROJCS") << ":" << r_ds->GetSpatialRef()->GetAuthorityCode("PROJCS") << std::endl;
  }

  divider();

  return 0;
}

// MIT License
//
// Copyright (C) 2025 Gabriel Russo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
