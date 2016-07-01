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

#ifndef PDFWALKER_H
#define PDFWALKER_H

#include <PDFDoc.h>

#include <string>
#include <memory>
#include <utility>
#include <exception>

#include <QString>
#include <QList>

#include "global.h"
#include "pdfwalkerobject.h"
#include "pdfwalkerdictionary.h"
#include "pdfwalkername.h"
#include "pdfwalkerarray.h"
#include "pdfwalkerstring.h"
#include "pdfwalkernumber.h"

class PDFWalkerException : public std::exception {
private:
    std::string msg;

public:
    PDFWalkerException(const std::string& inMsg) : msg(inMsg) { }
    ~PDFWalkerException() { }

    const char* what() const noexcept override { return msg.c_str(); }
};

class PDFWalker final
{
private:
    GooString* mFileName;
    std::unique_ptr<PDFDoc> mPDFDoc;
    std::shared_ptr<PDFWalkerDictionary> mCatalog;

    static QString objTypeString(ObjType type);

    static void loadDictionaryObject(Object* source, PDFWalkerDictionary* dest);
    static void loadNameObject(Object* source, PDFWalkerName* dest);
    static void loadArrayObject(Object* source, PDFWalkerArray* dest);
    static void loadStringObject(Object* source, PDFWalkerString* dest);

    template<typename T>
    static void loadNumberObject(Object* soruce, PDFWalkerNumber<T>* dest);

public:
    PDFWalker(const std::string& fileName);
    ~PDFWalker();

    std::shared_ptr<PDFWalkerDictionary> catalog() { return mCatalog; }

    std::unique_ptr<PDFWalkerObject> pdfWalkerObject(int number, int gen);
    std::unique_ptr<PDFWalkerObject> pdfWalkerObject(const ObjectSharedPtr& object);

    static QString PDFWalkerObjectTitle(PDFWalkerObject* obj);
};

template<typename T>
void PDFWalker::loadNumberObject(Object* source, PDFWalkerNumber<T>* dest) {
    QString value = QString("%1").arg(source->getInt());
    dest->setValue(value);
}

#endif // PDFWALKER_H
