#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LoginDialog(QWidget *parent = 0);
  ~LoginDialog();

private slots:
  void on_pushButton_login_clicked();

  void on_pushButton_cancel_clicked();

private:
  Ui::LoginDialog *ui;
};

#endif // LOGIN_DIALOG_H
