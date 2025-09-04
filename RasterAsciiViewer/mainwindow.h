#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void showRaster(std::string path);

  private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

  private slots:
    void on_actionCarregar_triggered();
};
#endif // MAINWINDOW_H
