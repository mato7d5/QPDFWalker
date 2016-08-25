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
#include <QByteArray>

namespace Ui {
class StreamDataDialog;
}

class StreamDataDialog : public QDialog
{
    Q_OBJECT
private:
    QByteArray mStreamData;

    enum class DisplayMode { Text = 0, Base64 = 1, Hex = 2 };
    enum class DisplayEncoding { Latin1 = 0, Unicode = 1, PDFDocEncoding = 2, Ascii = 3 };

public:
    explicit StreamDataDialog(const ObjectSharedPtr& streamObj, QWidget *parent = 0);
    ~StreamDataDialog();

private slots:
    void on_uiCloseBtn_clicked();
    void on_uiClipboardBtn_clicked();
    void on_uiSaveToFileBtn_clicked();
    void on_uiDisplayModeCombo_currentIndexChanged(int index);

    void on_uiDisplayEncodingCombo_currentIndexChanged(int index);

private:
    Ui::StreamDataDialog *ui;
};

#endif // STREAMDATADIALOG_H
