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

#include "pdfdataview.h"
#include <QVBoxLayout>
#include <QSizePolicy>
#include <utility>

PDFDataView::PDFDataView(QWidget *parent) : QWidget(parent)
{
    mLabel = new QLabel(this);
    mListWidget = new QListWidget(this);

    mLabel->resize(200, mLabel->height());
    mListWidget->resize(200, 200);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(mLabel);
    layout->addWidget(mListWidget);

    connect(mListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listWidgetClicked(QListWidgetItem*)));
    connect(mListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetDoubleClicked(QListWidgetItem*)));

//    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    setLayout(layout);
}

//slots
void PDFDataView::listWidgetClicked(QListWidgetItem* item) {
    auto itemData = mItems.find(item);

    if (itemData != mItems.end() && itemData->second.object != nullptr) {
        emit pdfObjectClicked(itemData->second);
    }
}

void PDFDataView::listWidgetDoubleClicked(QListWidgetItem* item) {
    auto itemData = mItems.find(item);

    if (itemData != mItems.end() && itemData->second.object != nullptr) {
        emit pdfObjectDoubleClicked(itemData->second);
    }
}

//methods
void PDFDataView::addItem(QListWidgetItem* item, ViewItemData data) {
    mItems.insert(std::make_pair(item, data));
    mListWidget->addItem(item);
}

void PDFDataView::clearView() {
    mLabel->setText("");
    mListWidget->clear();
    mItems.clear();
}
