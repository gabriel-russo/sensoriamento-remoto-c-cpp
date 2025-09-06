#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "rasterascii.h"
#include <QFileDialog>
#include <QInputDialog>

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
    QImage raster(this->raster->getColumns(), this->raster->getRows(), QImage::Format_Grayscale8);

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
void MainWindow::on_actionLoad_triggered()
{
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter(tr("Raster as text (*.txt)"));

    if (fileDialog.exec())
    {
        if (!fileDialog.selectedFiles().empty())
        {
            std::string path = fileDialog.selectedFiles().at(0).toStdString();

            if (this->raster->read_ascii(path))
            {
                this->raster_file_path = path;
                this->showRaster();
            }
        }
    }
}

void MainWindow::on_actionReload_triggered()
{
    if (!this->raster_file_path.empty())
    {
        delete this->raster;
        this->raster = new RasterAscii();
        this->raster->read_ascii(this->raster_file_path);
        this->showRaster();
    }
}

void MainWindow::on_actionGamma_triggered()
{
    double value =
        QInputDialog::getDouble(this, tr("Adicionar brilho gamma"), tr("Valor do gamma [0.0 - 30.0]"), 0, 0, 30., 2);

    if (value)
    {
        this->raster->gamma(1, value);
        this->showRaster();
    }
}

void MainWindow::on_actionNegative_triggered()
{
    this->raster->negative();
    this->showRaster();
}

void MainWindow::on_actionLogarithm_triggered()
{
    this->raster->logarithm(1);
    this->showRaster();
}

void MainWindow::on_actionThreshold_triggered()
{
    int value = QInputDialog::getInt(this, tr("Escolher limiar"), tr("Valor do limiar"), 128, this->raster->getMin(),
                                     this->raster->getMax());

    if (value)
    {
        this->raster->threshold(value);
        this->showRaster();
    }
}
