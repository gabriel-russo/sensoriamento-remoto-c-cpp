#include "mainwindow.h"
#include "rasterascii.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "RasterAsciiViewer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    // Definindo elementos
    MainWindow *window = new MainWindow();
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *viewer = new QGraphicsView(scene, window);

    // Configurando janela
    window->setWindowTitle("RasterAsciiViewer - Visualizador de raster em texto");
    window->setWindowIcon(QIcon(":/images/icon.svg"));
    window->show();

    // Processando txt
    RasterAscii::Raster raster_data = RasterAscii::read_ascii("/band15.txt");

    QImage raster(raster_data.Ncols, raster_data.Nrows, QImage::Format_Grayscale8);

    RasterAscii::print_statistics(raster_data);

    RasterAscii::to_8bit_grayscale(raster_data);

    for (int i = 0; i < raster_data.Nrows; i++)
    {
        for (int j = 0; j < raster_data.Ncols; j++)
        {
            raster.setPixel(j, i, raster_data.data[i * raster_data.Ncols + j]);
        }
    }

    scene->addPixmap(QPixmap::fromImage(raster, Qt::MonoOnly));
    scene->setBackgroundBrush(Qt::white);

    // Config do visualizador
    viewer->resize(window->width(), window->height());
    viewer->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    viewer->show();

    return a.exec();
}
