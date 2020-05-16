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

#ifndef PDFWALKER_H
#define PDFWALKER_H

#include <podofo/podofo.h>

#include <string>
#include <memory>
#include <utility>

#include <QString>
#include <QList>

#include "global.h"
#include "pdfwalkerexception.h"

class PDFWalker final
{
private:
    std::string mFileName;
    std::unique_ptr<PoDoFo::PdfMemDocument> mPDFDoc;
    PoDoFo::PdfDictionary* mTrailerDictionary;

public:
    PDFWalker(const std::string& fileName);
    ~PDFWalker();

    const char* fileName() const { return mFileName.c_str(); }
    static QString objTypeString(const PoDoFo::PdfObject& object);

    const PoDoFo::PdfObject* getReferencedObject(const PoDoFo::PdfReference& reference);

    const PoDoFo::PdfDictionary& getTrailer() { return mPDFDoc->GetTrailer()->GetDictionary(); }
};

#endif // PDFWALKER_H
