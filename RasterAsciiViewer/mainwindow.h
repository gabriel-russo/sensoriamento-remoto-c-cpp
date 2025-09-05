#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rasterascii.h"
#include <QGraphicsView>
#include <QMainWindow>

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
    void showRaster();

  private slots:
    void on_actionCarregar_triggered();
    void on_actionGamma_triggered();
    void on_actionNegativo_triggered();
};
#endif // MAINWINDOW_H
