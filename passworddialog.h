#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

private:
    QString mPassword;

public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();

    QString password() const { return mPassword; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
