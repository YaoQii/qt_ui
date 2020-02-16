#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include "dock_widget/curvature_dock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionshow_dock1_triggered(bool checked);

  void updateDockTest(bool);
private:
  GLWidget *glWidget;
  Ui::MainWindow *ui;
  CurvatureDock *curvature_dock_;
};

#endif // MAINWINDOW_H
