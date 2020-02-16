#include "login_dialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoginDialog)
{
  ui->setupUi(this);
  ui->lineEdit_password->setEchoMode(QLineEdit::Password);
  ui->pushButton_login->setFocus();
  //光标定位
  ui->lineEdit_usr_name->setFocus();
}

LoginDialog::~LoginDialog()
{
  delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
  if (ui->lineEdit_usr_name->text().trimmed() == tr("qy") &&
      ui->lineEdit_password->text().trimmed() == tr("666666")) {
    accept();//关闭窗体，并设置返回值为Accepted
  }
  else
  {                              //标题              内容                OK按钮
    QMessageBox::warning(this, tr("警告！"),tr("用户名或密码错误！"),QMessageBox::Yes);
    //QMessageBox::information(this,"提示","用户名或密码错误！");
    ui->lineEdit_usr_name->clear();
    ui->lineEdit_password->clear();

    //光标定位
    ui->lineEdit_usr_name->setFocus();
  }
}

void LoginDialog::on_pushButton_cancel_clicked()
{
    reject();
}
