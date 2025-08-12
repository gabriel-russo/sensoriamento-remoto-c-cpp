#include <iostream>
#include <gdal/gdal_priv.h>
#include <errno.h>

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

  GDALDataset *r_ds = GDALDataset::FromHandle(GDALOpen(banda_r, GA_ReadOnly));

  if (!r_ds)
  {
    std::cout << "Erro ao ler dataset!" << std::endl;
    return 1;
  }

  std::cout << "Gdal DS: " << r_ds->GetDriver()->GetDescription() << std::endl;

  r_ds->Close();

  return 0;
}
