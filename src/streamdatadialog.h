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

#ifndef STREAMDATADIALOG_H
#define STREAMDATADIALOG_H

#include <podofo/podofo.h>

#include "global.h"

#include <QDialog>
#include <QByteArray>

#include <memory>

namespace Ui {
class StreamDataDialog;
}

class StreamDataDialog : public QDialog
{
    Q_OBJECT
private:
    std::shared_ptr<QByteArray> mStreamData { nullptr };
    bool mStreamImage;

    enum class DisplayMode { Text = 0, Base64 = 1, Hex = 2 };
    enum class DisplayEncoding { Latin1 = 0, Unicode = 1, PDFDocEncoding = 2, Ascii = 3 };

public:
    explicit StreamDataDialog(const PoDoFo::PdfObject& streamObj, const std::shared_ptr<QByteArray>& stream_data, QWidget *parent = 0);
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
