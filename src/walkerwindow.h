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

#ifndef WALKERWINDOW_H
#define WALKERWINDOW_H

#include <podofo/podofo.h>

#include <QDialog>
#include <QString>
#include <QList>
#include <QHBoxLayout>
#include <memory>
#include "pdfwalker.h"
#include "pdfdataview.h"
#include "scrollareaviewport.h"
#include "global.h"

namespace Ui {
class WalkerWindow;
}

class WalkerWindow : public QDialog
{
    Q_OBJECT
private:
    std::shared_ptr<PDFWalker> mWalker;
    QList<PDFDataView*> mDataViews;
    int mViewWindowCount;
    int mNextViewWindowIndex;
    ScrollAreaViewPort* mViewPort;
    QHBoxLayout* mLayout;

    void addNewViewWindow();
    void objectToView(const PoDoFo::PdfObject& in_object);
    void loadTrailer();
    void loadObject(PoDoFo::pdf_objnum number, PoDoFo::pdf_gennum gen);
    void loadObject(const PoDoFo::PdfObject* object);

public:
    explicit WalkerWindow(std::shared_ptr<PDFWalker> walker, QWidget *parent = 0);
    ~WalkerWindow();

private:
    Ui::WalkerWindow *ui;

private slots:
    void pdfObjectClickedSlot(const ViewItemData& data);
    void pdfObjectDoubleClickedSlot(const ViewItemData& data);
    void lastViewWindowIndexSlot(int idx);
};

#endif // WALKERWINDOW_H
