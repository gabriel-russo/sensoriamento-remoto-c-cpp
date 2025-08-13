// Copyright (C) 2025 Gabriel Russo
// See end of file for extended copyright information.

#include <iostream>
#include <memory>
#include <errno.h>
#include <gdal/gdal_priv.h>

int main(int argc, const char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Especificar parametros <r> <g> <b>" << std::endl;
    return 1;
  }

  // Registrar drivers para leitura de dados
  GDALAllRegister();

  const char *banda_r = argv[1];
  std::cout << "Banda vermelha: " << banda_r << std::endl;

  const char *banda_g = argv[2];
  std::cout << "Banda verde: " << banda_g << std::endl;

  const char *banda_b = argv[3];
  std::cout << "Banda azul: " << banda_b << std::endl;

  std::unique_ptr<GDALDataset> r_ds(GDALDataset::FromHandle(GDALOpen(banda_r, GA_ReadOnly)));

  if (!r_ds)
  {
    std::cout << "Erro ao ler dataset!" << std::endl;
    return 1;
  }

  printf(
      "---> Informações do Driver:\n Nome curto: %s | Nome longo: %s\n",
      r_ds->GetDriver()->GetDescription(),
      r_ds->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));

  printf(
      "---> Estrutura:\n Linhas: %d | Colunas: %d | Bandas: %d\n",
      r_ds->GetRasterXSize(), r_ds->GetRasterYSize(), r_ds->GetRasterCount());

  // Chamada do método pra pegar o SRC pode retornar nulo, cuidado.
  printf(
      "---> SRC: %s\n",
      r_ds->GetProjectionRef());

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
