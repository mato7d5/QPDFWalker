/*
Copyright 2016 Martin Mancuska <martin@borg.sk>
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

#include "maindialog.h"
#include "ui_maindialog.h"
#include "walkerwindow.h"
#include "aboutdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include <memory>
#include <utility>

#include "pdfwalker.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_uiOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select PDF File"), QDir::homePath(), tr("pdf files (*.pdf)"));

    if (!fileName.isEmpty()) {
        mPdfFiles.append(fileName);
        ui->uiFilesList->addItem(fileName);
        ui->uiCloseFile->setEnabled(true);
        ui->uiWalk->setEnabled(true);
    }
}

void MainDialog::on_uiCloseFile_clicked()
{
    auto selected = ui->uiFilesList->selectedItems();

    if (!selected.isEmpty()) {
        for (auto& item : selected) {
            auto fileName = item->data(Qt::DisplayRole).toString();
            mPdfFiles.removeAt(mPdfFiles.indexOf(fileName));
            ui->uiFilesList->takeItem(ui->uiFilesList->row(item));
        }
    }

    if (mPdfFiles.isEmpty()) {
        ui->uiCloseFile->setEnabled(false);
        ui->uiWalk->setEnabled(false);
    }
}

void MainDialog::on_uiWalk_clicked()
{
    auto selected = ui->uiFilesList->selectedItems();
    auto firstItem = selected.at(0);
    auto fileName = firstItem->data(Qt::DisplayRole).toString();

    try {
        std::shared_ptr<PDFWalker> walker = std::make_shared<PDFWalker> (fileName.toStdString());
        WalkerWindow wallkerWindow(walker);

        wallkerWindow.exec();

    }
    catch (PDFWalkerException& ex) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open file."), QMessageBox::Ok);
    }

}

void MainDialog::on_uiAbout_clicked()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainDialog::on_uiQuit_clicked()
{
    qApp->exit();
}