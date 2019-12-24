/*
Copyright 2016 - 2019 Martin Mancuska <mmancuska@gmail.com>
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

#ifndef PDFWALKERDICTIONARY_H
#define PDFWALKERDICTIONARY_H

#include "pdfwalkerobject.h"
#include "global.h"
#include <QList>

class PDFWalkerDictionary : public PDFWalkerObject
{
public:
    struct DictionaryData {
        QString key;
        ObjectSharedPtr value;
    };

private:
    QList<DictionaryData> mItems;

public:
    PDFWalkerDictionary();

    virtual void addItem(DictionaryData item);
    virtual const QList<DictionaryData>& items() const { return mItems; }
};

#endif // PDFWALKERDICTIONARY_H
