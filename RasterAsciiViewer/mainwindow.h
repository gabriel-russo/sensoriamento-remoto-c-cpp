#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rasterascii.h"
#include <QGraphicsView>
#include <QMainWindow>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    RasterAscii *raster;
    std::string raster_file_path = {};
    void showRaster();

  private slots:
    void on_actionLoad_triggered();
    void on_actionReload_triggered();
    void on_actionGamma_triggered();
    void on_actionNegative_triggered();
    void on_actionLogarithm_triggered();
    void on_actionThreshold_triggered();
};
#endif // MAINWINDOW_H
