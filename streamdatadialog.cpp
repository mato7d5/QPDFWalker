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

#include "streamdatadialog.h"
#include "ui_streamdatadialog.h"
#include <QByteArray>

StreamDataDialog::StreamDataDialog(const ObjectSharedPtr& streamObj, QWidget *parent) :
    QDialog(parent),
    mStreamObj(streamObj),
    ui(new Ui::StreamDataDialog)
{
    ui->setupUi(this);

    // load stream data
    QByteArray ar;
    int c;

    mStreamObj->streamReset();
    while ((c = mStreamObj->streamGetChar()) != EOF)
        ar.append(static_cast<char> (c));

    mStreamObj->streamClose();

    ui->uiStreamData->insertPlainText(ar.data());
}

StreamDataDialog::~StreamDataDialog()
{
    delete ui;
}

void StreamDataDialog::on_uiCloseBtn_clicked()
{
    close();
}