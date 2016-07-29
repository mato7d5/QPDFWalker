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

#ifndef STREAMDATADIALOG_H
#define STREAMDATADIALOG_H

#include "global.h"
#include <QDialog>

namespace Ui {
class StreamDataDialog;
}

class StreamDataDialog : public QDialog
{
    Q_OBJECT
private:
    const ObjectSharedPtr& mStreamObj;

public:
    explicit StreamDataDialog(const ObjectSharedPtr& streamObj, QWidget *parent = 0);
    ~StreamDataDialog();

private slots:
    void on_uiCloseBtn_clicked();
    void on_uiClipboardBtn_clicked();
    void on_uiSaveToFileBtn_clicked();

private:
    Ui::StreamDataDialog *ui;
};

#endif // STREAMDATADIALOG_H
