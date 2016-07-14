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

#ifndef PDFDATAVIEW_H
#define PDFDATAVIEW_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QString>

#include <map>
#include <memory>

#include "global.h"

class PDFDataView : public QWidget
{
    Q_OBJECT

private:
    QLabel* mLabel;
    QListWidget* mListWidget;
    std::map<QListWidgetItem*, ViewItemData> mItems;

public:
    explicit PDFDataView(QWidget *parent = 0);

    void setCaption(const QString& caption) { mLabel->setText(caption); }
    void addItem(QListWidgetItem* item, ViewItemData data);
    void clearView();

signals:
    void pdfObjectClicked(const ViewItemData& obj);
    void pdfObjectDoubleClicked(const ViewItemData& obj);

private slots:
    void listWidgetClicked(QListWidgetItem* item);
    void listWidgetDoubleClicked(QListWidgetItem* item);
};

#endif // PDFDATAVIEW_H
