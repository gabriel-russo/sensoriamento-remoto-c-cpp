#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "rasterascii.h"
#include <QFileDialog>
#include <string>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->scene = new QGraphicsScene();
    this->ui->graphicsView->setScene(this->scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showRaster(std::string path)
{
    RasterAscii::Raster *raster_data = RasterAscii::read_ascii(path);

    QImage raster(raster_data->Ncols, raster_data->Nrows, QImage::Format_Grayscale16);

    RasterAscii::print_statistics(*raster_data);

    RasterAscii::to_8bit_grayscale(*raster_data);

    for (int i = 0; i < raster_data->Nrows; i++)
    {
        for (int j = 0; j < raster_data->Ncols; j++)
        {
            raster.setPixelColor(j, i, QColor::fromHsl(0, 0, raster_data->data[i * raster_data->Ncols + j]));
        }
    }

    this->scene->addPixmap(QPixmap::fromImage(raster, Qt::AutoColor));
    this->scene->setBackgroundBrush(Qt::gray);
    this->ui->graphicsView->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

// Nome do método deve seguir o padrão abaixo para funcionar.
// void on_<object name>_<signal name>(<signal parameters>);
// https://doc.qt.io/qt-6/qmetaobject.html#connectSlotsByName
void MainWindow::on_actionCarregar_triggered()
{
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter(tr("Raster as text (*.txt)"));

    if (fileDialog.exec())
    {
        this->showRaster(fileDialog.selectedFiles().at(0).toStdString());
    }
}
