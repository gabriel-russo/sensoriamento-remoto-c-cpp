#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "rasterascii.h"
#include <QFileDialog>
#include <QInputDialog>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->raster = new RasterAscii();
    this->scene = new QGraphicsScene();
    this->ui->graphicsView->setScene(this->scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showRaster()
{
    QImage raster(this->raster->getColumns(), this->raster->getRows(), QImage::Format_Grayscale16);

    this->raster->print_statistics();

    for (int i = 0; i < this->raster->getRows(); i++)
    {
        for (int j = 0; j < this->raster->getColumns(); j++)
        {
            raster.setPixelColor(j, i,
                                 QColor::fromHsl(0, 0, this->raster->to_8bit_grayscale(this->raster->getPixel(i, j))));
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
        if (!fileDialog.selectedFiles().empty())
        {
            this->raster->read_ascii(fileDialog.selectedFiles().at(0).toStdString());
            this->showRaster();
        }
    }
}

void MainWindow::on_actionGamma_triggered()
{
    double value =
        QInputDialog::getDouble(this, tr("Adicionar brilho gamma"), tr("Valor do gamma [0.0 - 2.0]"), 0, 0, 2., 5);

    if (value)
    {
        this->raster->gamma(1, value);
        this->showRaster();
    }
}

void MainWindow::on_actionNegativo_triggered()
{
    this->raster->negative();
    this->showRaster();
}
