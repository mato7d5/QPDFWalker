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
#include <QList>
#include <QListWidgetItem>
#include "pdfwalkerobject.h"
#include "pdfwalkerdictionary.h"

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

    loadCatalog();
}

WalkerWindow::~WalkerWindow()
{
    delete ui;
}

//slots
void WalkerWindow::pdfObjectClickedSlot(const ViewItemData& data) {
    if (data.object->isRef()) {
        mNextViewWindowIndex = data.currentViewIndex + 1;
        loadObject(data.object->getRefNum(), data.object->getRefGen());
    }
    else {
        mNextViewWindowIndex = data.currentViewIndex + 1;
        loadObject(data.object);
    }
}

//methods
void WalkerWindow::addNewViewWindow() {
    ++mViewWindowCount;

    PDFDataView* view = new PDFDataView(this);
    ui->horizontalLayout->addWidget(view);
    mDataViews.push_back(view);
}

void WalkerWindow::objectToView(const QString& title, PDFWalkerObject* object) {
    if (mNextViewWindowIndex == mViewWindowCount)
        addNewViewWindow();

    for (int i = mNextViewWindowIndex; i < mDataViews.size(); ++i)
        mDataViews[i]->clearView();

    if (object != nullptr) {
        mDataViews[mNextViewWindowIndex]->setCaption(title);

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
            for (int i = 0; i < arrayItems.size(); ++i) {
                QString str = QString("[%1]").arg(i);
                QListWidgetItem* widgetItem = new QListWidgetItem(str);
                ViewItemData itemData = { arrayItems[i], mNextViewWindowIndex };
                mDataViews[mNextViewWindowIndex]->addItem(widgetItem, itemData);
            }
        }

        connect(mDataViews[mNextViewWindowIndex], SIGNAL(pdfObjectClicked(const ViewItemData&)), this, SLOT(pdfObjectClickedSlot(const ViewItemData&)));
        ++mNextViewWindowIndex;
    }
}

void WalkerWindow::loadCatalog() {
    while (mNextViewWindowIndex > 0) {
        //clear content of listwidgets
        --mNextViewWindowIndex;
    }

    auto catalog = mWalker->catalog();
    QString title = QString::fromUtf8("Catalog [%1 %2]").arg(catalog->number()).arg(catalog->generation());
    objectToView(title, catalog.get());
}

void WalkerWindow::loadObject(int number, int gen) {
    auto obj = mWalker->pdfWalkerObject(number, gen);
    QString title = PDFWalker::PDFWalkerObjectTitle(obj.get());
    objectToView(title, obj.get());
}

void WalkerWindow::loadObject(ObjectSharedPtr obj) {
   auto object = mWalker->pdfWalkerObject(obj);
   QString title = PDFWalker::PDFWalkerObjectTitle(object.get());
   objectToView(title, object.get());
}
