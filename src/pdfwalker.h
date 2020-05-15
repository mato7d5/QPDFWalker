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
#include "pdfwalkerobject.h"
#include "pdfwalkerdictionary.h"
#include "pdfwalkername.h"
#include "pdfwalkerarray.h"
#include "pdfwalkerstring.h"
#include "pdfwalkernumber.h"
#include "pdfwalkerboolean.h"
#include "pdfwalkerstream.h"
#include "pdfwalkerexception.h"

class PDFWalker final
{
private:
    std::string mFileName;
    std::unique_ptr<PoDoFo::PdfMemDocument> mPDFDoc;
    PoDoFo::PdfDictionary* mTrailerDictionary;

    static void loadDictionaryObject(Object* source, PDFWalkerDictionary* dest);
    static void loadNameObject(Object* source, PDFWalkerName* dest);
    static void loadArrayObject(Object* source, PDFWalkerArray* dest);
    static void loadStringObject(Object* source, PDFWalkerString* dest);
    static void loadBooleanObject(Object* source, PDFWalkerBoolean* dest);
    static void loadStreamObject(Object* source, PDFWalkerStream* dest);

    template<typename T>
    static void loadNumberObject(Object* soruce, PDFWalkerNumber<T>* dest);

public:
    PDFWalker(const std::string& fileName);
    ~PDFWalker();

    const char* fileName() const { return mFileName.c_str(); }

  /*  std::shared_ptr<PDFWalkerDictionary> trailerDictionary() { return mTrailerDictionary; }

    std::unique_ptr<PDFWalkerObject> pdfWalkerObject(int number, int gen);
    std::unique_ptr<PDFWalkerObject> pdfWalkerObject(const ObjectSharedPtr& object);
*/
    static QString objTypeString(const PoDoFo::PdfObject& object);

    // PODOFO
    const PoDoFo::PdfDictionary& getTrailer() { return mPDFDoc->GetTrailer()->GetDictionary(); }
};

template<typename T>
void PDFWalker::loadNumberObject(Object* source, PDFWalkerNumber<T>* dest) {
    static_assert(std::is_arithmetic<T>::value, "None numerical type.");

    T num;

    if (std::is_integral<T>::value) {
        if (sizeof(T) == sizeof(int))
            num = source->getInt();
        if (sizeof(T) == sizeof(long long))
            num = source->getInt64();
    }

    if (std::is_floating_point<T>::value)
        num = source->getReal();

    QString value = QString("%1").arg(num);
    dest->setValue(value);
}

#endif // PDFWALKER_H
