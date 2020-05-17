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

#include "walkerwindow.h"
#include "ui_walkerwindow.h"
#include "streamdatadialog.h"
#include <QList>
#include <QListWidgetItem>
#include <QScrollBar>

WalkerWindow::WalkerWindow(std::shared_ptr<PDFWalker> walker, QWidget *parent) :
    QDialog(parent),
    mWalker(walker),
    mViewWindowCount(5),
    mNextViewWindowIndex(0),
    ui(new Ui::WalkerWindow)
{
    ui->setupUi(this);

    mViewPort = new ScrollAreaViewPort(this);
    mLayout = new QHBoxLayout();

    mViewPort->setLayout(mLayout);

    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setWidget(mViewPort);

    connect(mViewPort, SIGNAL(lastWindowIndexChanged(int)), this, SLOT(lastViewWindowIndexSlot(int)), Qt::UniqueConnection);

    for (int i = 0; i < mViewWindowCount; ++i) {
        PDFDataView* view = new PDFDataView(this);
        mLayout->addWidget(view);

        mDataViews.push_back(view);
    }

    loadTrailer();

    setWindowTitle(walker->fileName());
}

WalkerWindow::~WalkerWindow()
{
    delete ui;
}

//slots
void WalkerWindow::pdfObjectClickedSlot(const ViewItemData& data) {
    if (!data.stream_data) {
        if (data.object->IsReference()) {
            mNextViewWindowIndex = data.currentViewIndex + 1;
            const auto& reference = data.object->GetReference();
            loadObject(reference.ObjectNumber(), reference.GenerationNumber());
        }
        else {
            mNextViewWindowIndex = data.currentViewIndex + 1;
            loadObject(data.object.get());
        }
    }
}

void WalkerWindow::pdfObjectDoubleClickedSlot(const ViewItemData& data) {
    if (data.stream_data) {
        StreamDataDialog dlg(*data.object, data.stream_data, this);
        dlg.exec();
    }
}

void WalkerWindow::lastViewWindowIndexSlot(int idx) {
    ui->scrollArea->ensureWidgetVisible(mDataViews[idx]);
}

//methods
void WalkerWindow::addNewViewWindow() {
    ++mViewWindowCount;

    PDFDataView* view = new PDFDataView(this);
    mLayout->addWidget(view);
    mDataViews.push_back(view);
}

void WalkerWindow::objectToView(const PoDoFo::PdfObject& in_object) {
    bool viewPortResized = false;

    if (mNextViewWindowIndex == mViewWindowCount) {
        addNewViewWindow();
        viewPortResized = true;
    }

    for (int i = mNextViewWindowIndex; i < mDataViews.size(); ++i)
        mDataViews[i]->clearView();

    const PoDoFo::PdfObject* referenced_object = nullptr;
    QString title;
    if (in_object.IsReference()) {
        const PoDoFo::PdfReference& reference = in_object.GetReference();
        title.append(QString::fromUtf8("%1 [%2 %3]").arg(PDFWalker::objTypeString(in_object)).arg(reference.ObjectNumber()).arg(reference.GenerationNumber()));
        referenced_object = mWalker->getReferencedObject(reference);
    }
    else
        title.append(QString::fromUtf8("%1").arg(PDFWalker::objTypeString(in_object)));

    const PoDoFo::PdfObject& object = referenced_object ? *referenced_object : in_object;

    if (object.IsDictionary() && !object.HasStream()) {
        const PoDoFo::PdfDictionary& dict = object.GetDictionary();

        for (const auto& key : dict.GetKeys()) {
            QListWidgetItem* i = new QListWidgetItem(key.first.GetName().c_str());
            std::shared_ptr<PoDoFo::PdfObject> copy_obj(new PoDoFo::PdfObject(*key.second));
            ViewItemData itemData { copy_obj, nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(i, itemData);
        }
    }
    else if (object.IsName()) {
        const PoDoFo::PdfName& name = object.GetName();
        QListWidgetItem* i = new QListWidgetItem(name.GetName().c_str());
        ViewItemData itemData { nullptr, nullptr, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(i, itemData);
    }
    else if (object.IsArray()) {
        const PoDoFo::PdfArray& array = object.GetArray();

        int idx = 0;
        for (const auto& array_item : array) {
            QString str = QString("[%1]").arg(idx++);
            QListWidgetItem* widgetItem = new QListWidgetItem(str);
            std::shared_ptr<PoDoFo::PdfObject> copy_obj (new PoDoFo::PdfObject(array_item));

            ViewItemData itemData { copy_obj, nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
        }

        title.append(QString::fromUtf8(" (%1)").arg(array.GetSize()));
    }
    else if (object.IsString() || object.IsHexString()) {
        const PoDoFo::PdfString& str = object.GetString();
        QListWidgetItem* widgetItem = new QListWidgetItem(str.GetString());
        ViewItemData itemData { nullptr, nullptr, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
    }
    else if (object.IsNumber()) {
        std::string value = std::to_string(object.GetNumber());
        QListWidgetItem* widgetItem = new QListWidgetItem(value.c_str());
        ViewItemData itemData { nullptr, nullptr, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
    }
    else if (object.IsReal()) {
        std::string value = std::to_string(object.GetReal());
        QListWidgetItem* widgetItem = new QListWidgetItem(value.c_str());
        ViewItemData itemData { nullptr, nullptr, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
    }
    else if (object.IsBool()) {
        QListWidgetItem* widgetItem = new QListWidgetItem(object.GetBool() ? "True" : "False");
        ViewItemData itemData { nullptr, nullptr, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
    }
    else if (object.IsNull()) {
        QListWidgetItem* widgetItem = new QListWidgetItem("Null");
        ViewItemData itemData { nullptr, nullptr, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
    }
    else if (object.IsEmpty()) {
        QListWidgetItem* widgetItem = new QListWidgetItem("Empty");
        ViewItemData itemData { nullptr, nullptr, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
    }
    else if (object.HasStream()) {
        const PoDoFo::PdfStream* stream = object.GetStream();
        PoDoFo::PdfMemoryOutputStream output;

        stream->GetFilteredCopy(&output);
        std::shared_ptr<QByteArray> stream_data(new QByteArray);
        char* data = output.TakeBuffer();

        if (data) {
            stream_data->append(data);
            free(data);
        }

        QListWidgetItem* dataWI = new QListWidgetItem("[Data]");
        std::shared_ptr<PoDoFo::PdfObject> copy_obj(new PoDoFo::PdfObject(object));

        ViewItemData dataWID { copy_obj, stream_data, mNextViewWindowIndex };
        mDataViews[mNextViewWindowIndex]->addItem(dataWI, dataWID);

        const PoDoFo::PdfDictionary& dict = object.GetDictionary();
        for (const auto& key : dict.GetKeys()) {
            QListWidgetItem* i = new QListWidgetItem(key.first.GetName().c_str());
            std::shared_ptr<PoDoFo::PdfObject> copy_obj(new PoDoFo::PdfObject(*key.second));
            ViewItemData itemData { copy_obj, nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(i, itemData);
        }

        connect(mDataViews[mNextViewWindowIndex], SIGNAL(pdfObjectDoubleClicked(const ViewItemData&)), this, SLOT(pdfObjectDoubleClickedSlot(const ViewItemData&)), Qt::UniqueConnection);
    }

    mDataViews[mNextViewWindowIndex]->setCaption(title);
    mViewPort->setLastWindowIndex(mNextViewWindowIndex);

    if (!viewPortResized)
        ui->scrollArea->ensureWidgetVisible(mDataViews[mNextViewWindowIndex]);

    connect(mDataViews[mNextViewWindowIndex], SIGNAL(pdfObjectClicked(const ViewItemData&)), this, SLOT(pdfObjectClickedSlot(const ViewItemData&)), Qt::UniqueConnection);
    ++mNextViewWindowIndex;
}

void WalkerWindow::loadTrailer() {
    while (mNextViewWindowIndex > 0) {
        //clear content of listwidgets
        --mNextViewWindowIndex;
    }

    auto trailerDict = mWalker->getTrailer();
    objectToView(trailerDict);
}

void WalkerWindow::loadObject(PoDoFo::pdf_objnum number, PoDoFo::pdf_gennum gen) {
    PoDoFo::PdfReference obj(number, gen);
    objectToView(obj);
}

void WalkerWindow::loadObject(const PoDoFo::PdfObject* object) {
   if (object) {
       objectToView(*object);
   }
}
