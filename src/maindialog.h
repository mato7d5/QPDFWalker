/*
Copyright 2016 - 2020 Martin Mancuska <mmancuska@gmail.com>
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 3,
as published bythe Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the
Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QSettings>

class QListWidgetItem;

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT
private:
    QSettings mSettings;
    QList<QString> mPdfFiles;
    Ui::MainDialog *ui;
    QString mLastOpenedDir;

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void on_uiOpenFile_clicked();
    void on_uiCloseFile_clicked();
    void on_uiWalk_clicked();
    void on_uiAbout_clicked();
    void on_uiQuit_clicked();
    void on_uiFilesList_itemDoubleClicked(QListWidgetItem *item);
};

#endif // MAINDIALOG_H
