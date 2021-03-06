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

#include "pdfdataview.h"
#include <QVBoxLayout>
#include <QSizePolicy>
#include <utility>

#define DATA_VIEW_WIDTH     120
#define DATA_VIEW_HEIGHT    160

PDFDataView::PDFDataView(QWidget *parent) : QWidget(parent)
{
    mLabel = new QLabel(this);
    mListWidget = new QListWidget(this);

    mLabel->setMaximumSize(DATA_VIEW_WIDTH, mLabel->height());
    mListWidget->setMaximumSize(DATA_VIEW_WIDTH, DATA_VIEW_HEIGHT);
    mListWidget->setMinimumSize(DATA_VIEW_WIDTH, DATA_VIEW_HEIGHT);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(mLabel);
    layout->addWidget(mListWidget);

    connect(mListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listWidgetClicked(QListWidgetItem*)));
    connect(mListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetDoubleClicked(QListWidgetItem*)));

    setLayout(layout);
}

//slots
void PDFDataView::listWidgetClicked(QListWidgetItem* item) {
    auto itemData = mItems.find(item);

    if (itemData != mItems.end() && itemData->second.GetObject() != nullptr) {
        emit pdfObjectClicked(itemData->second);
    }
}

void PDFDataView::listWidgetDoubleClicked(QListWidgetItem* item) {
    auto itemData = mItems.find(item);

    if (itemData != mItems.end() && itemData->second.GetObject() != nullptr) {
        emit pdfObjectDoubleClicked(itemData->second);
    }
}

//methods
void PDFDataView::addItem(QListWidgetItem* item, ViewItemData data) {
    mItems.insert(std::make_pair(item, std::move(data)));
    mListWidget->addItem(item);
}

void PDFDataView::clearView() {
    mLabel->setText("");
    mListWidget->clear();
    mItems.clear();
}
