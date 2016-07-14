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

#include "walkerwindow.h"
#include "ui_walkerwindow.h"
#include "streamdatadialog.h"
#include <QList>
#include <QListWidgetItem>

WalkerWindow::WalkerWindow(std::shared_ptr<PDFWalker> walker, QWidget *parent) :
    QDialog(parent),
    mWalker(walker),
    mViewWindowCount(5),
    mNextViewWindowIndex(0),
    ui(new Ui::WalkerWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < mViewWindowCount; ++i) {
        PDFDataView* view = new PDFDataView(this);
        ui->horizontalLayout->addWidget(view);

        mDataViews.push_back(view);
    }

    loadTrailer();
}

WalkerWindow::~WalkerWindow()
{
    delete ui;
}

//slots
void WalkerWindow::pdfObjectClickedSlot(const ViewItemData& data) {
    if (data.object->isStream()) {
        return; // do nothing for stream data. they are handled by double click slot.
    }
    else {
        if (data.object->isRef()) {
            mNextViewWindowIndex = data.currentViewIndex + 1;
            loadObject(data.object->getRefNum(), data.object->getRefGen());
        }
        else {
            mNextViewWindowIndex = data.currentViewIndex + 1;
            loadObject(data.object);
        }
    }
}

void WalkerWindow::pdfObjectDoubleClickedSlot(const ViewItemData& data) {
    if (data.object->isStream()) {
        StreamDataDialog dlg(data.object, this);
        dlg.exec();
    }
}

//methods
void WalkerWindow::addNewViewWindow() {
    ++mViewWindowCount;

    PDFDataView* view = new PDFDataView(this);
    ui->horizontalLayout->addWidget(view);
    mDataViews.push_back(view);
}

void WalkerWindow::objectToView(PDFWalkerObject* object) {
    if (mNextViewWindowIndex == mViewWindowCount)
        addNewViewWindow();

    for (int i = mNextViewWindowIndex; i < mDataViews.size(); ++i)
        mDataViews[i]->clearView();

    if (object != nullptr) {
        QString title;
        if (object->isIndirect())
            title.append(QString::fromUtf8("%1 [%2 %3]").arg(PDFWalker::objTypeString(object->type())).arg(object->number()).arg(object->generation()));
        else
            title.append(QString::fromUtf8("%1").arg(PDFWalker::objTypeString(object->type())));

        if (object->type() == objDict) {
            PDFWalkerDictionary* dict =  static_cast<PDFWalkerDictionary*> (object);
            for (const auto& item : dict->items()) {
                QListWidgetItem* i = new QListWidgetItem(item.key);
                ViewItemData itemData = { item.value, mNextViewWindowIndex };
                mDataViews[mNextViewWindowIndex]->addItem(i, itemData);
            }
        }

        if (object->type() == objName) {
            PDFWalkerName* name = static_cast<PDFWalkerName*> (object);
            QListWidgetItem* i = new QListWidgetItem(name->value());
            ViewItemData itemData = { nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(i, itemData);
        }

        if (object->type() == objArray) {
            PDFWalkerArray* array = static_cast<PDFWalkerArray*> (object);
            auto arrayItems = array->items();
            auto arrayItemsCount = arrayItems.size();

            for (int i = 0; i < arrayItemsCount; ++i) {
                QString str = QString("[%1]").arg(i);
                QListWidgetItem* widgetItem = new QListWidgetItem(str);
                ViewItemData itemData = { arrayItems[i], mNextViewWindowIndex };
                mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
            }

            title.append(QString::fromUtf8(" (%1)").arg(arrayItemsCount));
        }

        if (object->type() == objString) {
            PDFWalkerString* str = static_cast<PDFWalkerString*> (object);
            QListWidgetItem* widgetItem = new QListWidgetItem(str->value());
            ViewItemData itemData = { nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
        }

        if (object->type() == objInt) {
            PDFWalkerNumber<int>* number = static_cast<PDFWalkerNumber<int>*> (object);
            QListWidgetItem* widgetItem = new QListWidgetItem(number->value());
            ViewItemData itemData = { nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
        }

        if (object->type() == objInt64) {
            PDFWalkerNumber<long long>* number = static_cast<PDFWalkerNumber<long long>*> (object);
            QListWidgetItem* widgetItem = new QListWidgetItem(number->value());
            ViewItemData itemData = { nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
        }

        if (object->type() == objReal) {
            PDFWalkerNumber<double>* number = static_cast<PDFWalkerNumber<double>*> (object);
            QListWidgetItem* widgetItem = new QListWidgetItem(number->value());
            ViewItemData itemData = { nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
        }

        if (object->type() == objBool) {
            PDFWalkerBoolean* boolObj =  static_cast<PDFWalkerBoolean*> (object);
            QListWidgetItem* widgetItem = new QListWidgetItem(boolObj->value());
            ViewItemData itemData = { nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
        }

        if (object->type() == objNull) {
            QListWidgetItem* widgetItem = new QListWidgetItem("Null");
            ViewItemData itemData = { nullptr, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
        }

        if (object->type() == objStream) {
            PDFWalkerStream* streamObj = static_cast<PDFWalkerStream*> (object);
            ObjectSharedPtr value = streamObj->value();
            ObjectSharedPtr valueCopy = std::make_shared<Object> ();
            value->copy(valueCopy.get());

            QListWidgetItem* dataWI = new QListWidgetItem("[Data]");
            ViewItemData dataWID = { valueCopy, mNextViewWindowIndex };
            mDataViews[mNextViewWindowIndex]->addItem(dataWI, dataWID);

            auto streamDict = streamObj->getStreamDict();
            for (const auto& item : streamDict->items()) {
                QListWidgetItem* i = new QListWidgetItem(item.key);
                ViewItemData itemData = { item.value, mNextViewWindowIndex };
                mDataViews[mNextViewWindowIndex]->addItem(i, itemData);
            }

            connect(mDataViews[mNextViewWindowIndex], SIGNAL(pdfObjectDoubleClicked(const ViewItemData&)), this, SLOT(pdfObjectDoubleClickedSlot(const ViewItemData&)), Qt::UniqueConnection);
        }

        mDataViews[mNextViewWindowIndex]->setCaption(title);

        connect(mDataViews[mNextViewWindowIndex], SIGNAL(pdfObjectClicked(const ViewItemData&)), this, SLOT(pdfObjectClickedSlot(const ViewItemData&)), Qt::UniqueConnection);
        ++mNextViewWindowIndex;
    }
}

void WalkerWindow::loadTrailer() {
    while (mNextViewWindowIndex > 0) {
        //clear content of listwidgets
        --mNextViewWindowIndex;
    }

    auto trailerDict = mWalker->trailerDictionary();
    QString title = QString::fromUtf8("Trailer");
    objectToView(trailerDict.get());
}

void WalkerWindow::loadObject(int number, int gen) {
    auto obj = mWalker->pdfWalkerObject(number, gen);

    if (obj) {
        objectToView(obj.get());
    }
}

void WalkerWindow::loadObject(ObjectSharedPtr obj) {
   auto object = mWalker->pdfWalkerObject(obj);

   if (object) {
       objectToView(object.get());
   }
}
