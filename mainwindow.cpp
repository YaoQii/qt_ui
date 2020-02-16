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
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(ChangeMode()));
  //timer->start(100);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::updateDockTest(bool)
{
    ui->actionshow_dock1->setChecked(curvature_dock_->isVisible());
}

void MainWindow::ChangeMode() {
//  if (login_dialog_ != nullptr && login_dialog_->exec() == QDialog::Accepted) {
//
//  } else if (login_dialog_->exec() == QDialog::Rejected) {

//  }
}

void MainWindow::on_actionshow_dock1_triggered(bool checked)
{
  curvature_dock_->setVisible(checked);
}

void MainWindow::on_actiontest_triggered()
{
  //弹出登录窗口
  login_dialog_ = new LoginDialog();
  if(login_dialog_->exec()==QDialog::Accepted){
    ui->actiontest->setText("开发者模式");
  }
  delete login_dialog_;
}
