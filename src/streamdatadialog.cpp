/*
Copyright 2016 - 2018 Martin Mancuska <mmancuska@gmail.com>
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
#include <QClipboard>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QScrollArea>
#include <QPixmap>

StreamDataDialog::StreamDataDialog(const ObjectSharedPtr& streamObj, QWidget *parent) :
    QDialog(parent),
    mStreamImage(false),
    ui(new Ui::StreamDataDialog)
{
    ui->setupUi(this);

    //display mode combobox
    ui->uiDisplayModeCombo->insertItem(static_cast<int> (DisplayMode::Text), tr("Text"));
    ui->uiDisplayModeCombo->insertItem(static_cast<int> (DisplayMode::Base64), tr("Base64"));
    ui->uiDisplayModeCombo->insertItem(static_cast<int> (DisplayMode::Hex), tr("Hex"));

    //display encoding combobox
    ui->uiDisplayEncodingCombo->insertItem(static_cast<int> (DisplayEncoding::Latin1), tr("Latin1"));
    ui->uiDisplayEncodingCombo->insertItem(static_cast<int> (DisplayEncoding::Unicode), tr("Unicode"));
    ui->uiDisplayEncodingCombo->insertItem(static_cast<int> (DisplayEncoding::PDFDocEncoding), tr("PDFDocEncoding"));
    ui->uiDisplayEncodingCombo->insertItem(static_cast<int> (DisplayEncoding::Ascii), tr("Ascii"));

    Stream* stream = streamObj->getStream();
    Dict* streamDict = stream->getDict();
    ObjectUniquePtr subtype = std::make_unique<Object> ();
    *subtype = streamDict->lookup("Subtype");

    if (subtype->isName())
    {
        std::string name = subtype->getName();
        if (name == "Image")
            mStreamImage = true;
    }

    bool imageLoaded = false;

    if (mStreamImage) {
        QPixmap image;
        StreamKind kind = stream->getKind();

        if (kind == StreamKind::strDCT) {
            Stream* nextStream = stream->getNextStream();

            if (nextStream) {
                auto size = nextStream->getBaseStream()->getLength();
                Guchar* buf = new Guchar[size];
                nextStream->reset();
                nextStream->doGetChars(size, buf);

                imageLoaded = image.loadFromData(buf, size);

                if (imageLoaded) {
                    ui->uiStreamData->setEnabled(false);
                    ui->uiDisplayEncodingCombo->setEnabled(false);
                    ui->uiDisplayModeCombo->setEnabled(false);
                    ui->uiDecodedLength->setText("N/A");
                    ui->uiClipboardBtn->setEnabled(false);
                    ui->uiSaveToFileBtn->setEnabled(false);

                    QLabel* imageViewer = new QLabel(this);
                    QScrollArea* scrollArea = new QScrollArea;

                    imageViewer->setBackgroundRole(QPalette::Base);
                    imageViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
                    imageViewer->setScaledContents(true);

                    scrollArea->setBackgroundRole(QPalette::Dark);
                    scrollArea->setWidget(imageViewer);

                    imageViewer->setPixmap(image);
                    imageViewer->adjustSize();

                    ui->verticalLayout_2->replaceWidget(ui->uiStreamData, scrollArea);
                    ui->uiStreamData->setVisible(false);
                }

                delete[] buf;
            }
        }
        else if (kind == StreamKind::strJPX) {
            Stream* nextStream = stream->getNextStream();
            auto size = nextStream->getBaseStream()->getLength();
            Guchar* buf = new Guchar[size];
            nextStream->reset();
            nextStream->doGetChars(size, buf);

            imageLoaded = image.loadFromData(buf, size);

            if (imageLoaded) {
                ui->uiStreamData->setEnabled(false);
                ui->uiDisplayEncodingCombo->setEnabled(false);
                ui->uiDisplayModeCombo->setEnabled(false);
                ui->uiDecodedLength->setText("N/A");
                ui->uiClipboardBtn->setEnabled(false);
                ui->uiSaveToFileBtn->setEnabled(false);

                QLabel* imageViewer = new QLabel(this);
                QScrollArea* scrollArea = new QScrollArea;

                imageViewer->setBackgroundRole(QPalette::Base);
                imageViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
                imageViewer->setScaledContents(true);

                scrollArea->setBackgroundRole(QPalette::Dark);
                scrollArea->setWidget(imageViewer);

                imageViewer->setPixmap(image);
                imageViewer->adjustSize();

                ui->verticalLayout_2->replaceWidget(ui->uiStreamData, scrollArea);
                ui->uiStreamData->setVisible(false);
            }

            delete[] buf;
        }
        else if (kind == StreamKind::strJBIG2) {
            Stream* nextStream = stream->getNextStream();
            auto size = nextStream->getBaseStream()->getLength();
            Guchar* buf = new Guchar[size];
            nextStream->reset();
            nextStream->doGetChars(size, buf);

            imageLoaded = image.loadFromData(buf, size);

            if (imageLoaded) {
                ui->uiStreamData->setEnabled(false);
                ui->uiDisplayEncodingCombo->setEnabled(false);
                ui->uiDisplayModeCombo->setEnabled(false);
                ui->uiDecodedLength->setText("N/A");
                ui->uiClipboardBtn->setEnabled(false);
                ui->uiSaveToFileBtn->setEnabled(false);

                QLabel* imageViewer = new QLabel(this);
                QScrollArea* scrollArea = new QScrollArea;

                imageViewer->setBackgroundRole(QPalette::Base);
                imageViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
                imageViewer->setScaledContents(true);

                scrollArea->setBackgroundRole(QPalette::Dark);
                scrollArea->setWidget(imageViewer);

                imageViewer->setPixmap(image);
                imageViewer->adjustSize();

                ui->verticalLayout_2->replaceWidget(ui->uiStreamData, scrollArea);
                ui->uiStreamData->setVisible(false);
            }

            delete[] buf;
        }
    }

    if (!imageLoaded) {
        // load the stream data
        int c;

        streamObj->streamReset();
        while ((c = streamObj->streamGetChar()) != EOF)
            mStreamData.append(static_cast<char> (c));

        streamObj->streamClose();

        ui->uiStreamData->insertPlainText(mStreamData.data());
        ui->uiDisplayModeCombo->setCurrentIndex(static_cast<int> (DisplayMode::Text));
        ui->uiDisplayEncodingCombo->setCurrentIndex(static_cast<int> (DisplayEncoding::Latin1));

        ui->uiDecodedLength->setText(QString("%1").arg(mStreamData.length()));
    }

    ui->uiEncodedLength->setText(QString("%1").arg(stream->getBaseStream()->getLength()));
}

StreamDataDialog::~StreamDataDialog()
{
    delete ui;
}

void StreamDataDialog::on_uiCloseBtn_clicked()
{
    close();
}

void StreamDataDialog::on_uiClipboardBtn_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->uiStreamData->toPlainText());
}

void StreamDataDialog::on_uiSaveToFileBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), QDir::homePath(), tr("All Files (*.*"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot save stream data to the file"));
        return;
    }

    QTextStream fileStream(&file);
    fileStream << ui->uiStreamData->toPlainText();

    file.close();
}

void StreamDataDialog::on_uiDisplayModeCombo_currentIndexChanged(int index)
{
    DisplayMode mode = static_cast<DisplayMode> (index);

    switch (mode) {
    case DisplayMode::Text:
        ui->uiStreamData->clear();
        ui->uiStreamData->insertPlainText(mStreamData.data());
        break;

    case DisplayMode::Base64:
        ui->uiStreamData->clear();
        ui->uiStreamData->insertPlainText(mStreamData.toBase64());
        break;
    case DisplayMode::Hex:
        ui->uiStreamData->clear();
        ui->uiStreamData->insertPlainText(mStreamData.toHex());
        break;
    default:
        break;
    }
}

void StreamDataDialog::on_uiDisplayEncodingCombo_currentIndexChanged(int index)
{
    DisplayEncoding encoding = static_cast<DisplayEncoding> (index);

    switch (encoding) {
    case DisplayEncoding::Latin1:
    case DisplayEncoding::PDFDocEncoding:
        ui->uiStreamData->clear();
        ui->uiStreamData->insertPlainText(QString::fromLatin1(mStreamData.data()));
        break;
    case DisplayEncoding::Unicode:
        ui->uiStreamData->clear();
        ui->uiStreamData->insertPlainText(QString::fromUtf16(reinterpret_cast<ushort*> (mStreamData.data())));
        break;
    case DisplayEncoding::Ascii:
        ui->uiStreamData->clear();
        ui->uiStreamData->insertPlainText(QString::fromStdString(mStreamData.data()));
        break;
    default:
        break;
    }
}
