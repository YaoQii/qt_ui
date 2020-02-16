#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) ,
  glWidget(new GLWidget(this))
{
  ui->setupUi(this);
  setCentralWidget(glWidget);
  curvature_dock_ = new CurvatureDock(this); //设置寄存主this
  curvature_dock_->setAllowedAreas(Qt::BottomDockWidgetArea); //location
  addDockWidget(Qt::BottomDockWidgetArea, curvature_dock_);
  curvature_dock_->setVisible(false);
  ui->actionshow_dock1->setChecked(curvature_dock_->isVisible());
  //set a thread to get the window's status(close or open),
  //and update action's checked
  connect(curvature_dock_, SIGNAL(visibilityChanged(bool)),
          this, SLOT(updateDockTest(bool)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::updateDockTest(bool)
{
    ui->actionshow_dock1->setChecked(curvature_dock_->isVisible());
}

void MainWindow::on_actionshow_dock1_triggered(bool checked)
{
  curvature_dock_->setVisible(checked);
}
