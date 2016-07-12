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

#include "pdfwalker.h"
#include <XRef.h>
#include <Stream.h>
#include <utility>

PDFWalker::PDFWalker(const std::string& fileName) {
    mFileName = new GooString(fileName.c_str());

    mPDFDoc = std::make_unique<PDFDoc> (mFileName);
    if (!mPDFDoc->isOk())
        throw PDFWalkerException("Cannot open file!");

    //catalog
    XRef* xref = mPDFDoc->getXRef();
    auto catalogDict = pdfWalkerObject(xref->getRootNum(), xref->getRootGen());
    mCatalog.reset(dynamic_cast<PDFWalkerDictionary*> (catalogDict.release()));
}

PDFWalker::~PDFWalker() {

}

//methods
QString PDFWalker::objTypeString(ObjType type) {
    switch (type) {
    case objBool:
        return "Boolean";
    case objInt:
        return "Integer";
    case objReal:
        return "Real";
    case objString:
        return "String";
    case objName:
        return "Name";
    case objNull:
        return "Null";
    case objArray:
        return "Array";
    case objDict:
        return "Dictionary";
    case objStream:
        return "Stream";
    case objRef:
        return "Reference";
    case objCmd:
        return "Command name";
    case objError:
        return "Error";
    case objEOF:
        return "EOF";
    case objNone:
        return "None";
    case objInt64:
        return "Integer 64 (Poppler object)";
    default:
        return "";
    }

    return "";
}

QString PDFWalker::PDFWalkerObjectTitle(PDFWalkerObject* obj) {
    if (obj->isIndirect())
        return QString::fromUtf8("%1 [%2 %3]").arg(objTypeString(obj->type())).arg(obj->number()).arg(obj->generation());

    return QString::fromUtf8("%1").arg(objTypeString(obj->type()));
}

void PDFWalker::loadDictionaryObject(Object* source, PDFWalkerDictionary* dest) {
    for (int i = 0; i < source->dictGetLength(); ++i) {
        PDFWalkerDictionary::DictionaryData data;
        data.key = source->dictGetKey(i);
        ObjectSharedPtr value = std::make_shared<Object> ();
        source->dictGetValNF(i, value.get());
        data.value = value;

        dest->addItem(data);
    }
}

void PDFWalker::loadNameObject(Object* source, PDFWalkerName* dest) {
    QString value = source->getName();
    dest->setValue(value);
}

void PDFWalker::loadArrayObject(Object* source, PDFWalkerArray* dest) {
    for (int i = 0; i < source->arrayGetLength(); ++i) {
        ObjectSharedPtr object = std::make_shared<Object> ();
        source->arrayGetNF(i, object.get());

        dest->addItem(object);
    }
}

void PDFWalker::loadStringObject(Object* source, PDFWalkerString* dest) {
    QString value(source->getString()->getCString());
    dest->setValue(value);
}

void PDFWalker::loadBooleanObject(Object* source, PDFWalkerBoolean* dest) {
    dest->setValue(source->getBool() ? "True" : "False");
}

void PDFWalker::loadStreamObject(Object* source, PDFWalkerStream* dest) {
    Object* sourceCopy = new Object;
    source->copy(sourceCopy);
    dest->setValue(sourceCopy);
}

std::unique_ptr<PDFWalkerObject> PDFWalker::pdfWalkerObject(int number, int gen) {
    std::unique_ptr<PDFWalkerObject> ret(nullptr);

    XRef* xref = mPDFDoc->getXRef();
    Object obj;
    obj.initNull();

    if (xref->fetch(number, gen, &obj)) {
        if (obj.isDict()) {
            ret.reset(new PDFWalkerDictionary());
            loadDictionaryObject(&obj, dynamic_cast<PDFWalkerDictionary*> (ret.get()));
            ret->setNumber(number);
            ret->setGeneration(gen);
        }

        if (obj.isName()) {
            ret.reset(new PDFWalkerName());
            loadNameObject(&obj, dynamic_cast<PDFWalkerName*> (ret.get()));
            ret->setNumber(number);
            ret->setGeneration(gen);
        }

        if (obj.isArray()) {
            ret.reset(new PDFWalkerArray());
            loadArrayObject(&obj, dynamic_cast<PDFWalkerArray*> (ret.get()));
            ret->setNumber(number);
            ret->setGeneration(gen);
        }

        if (obj.isString()) {
            ret.reset(new PDFWalkerString());
            loadStringObject(&obj, dynamic_cast<PDFWalkerString*> (ret.get()));
            ret->setNumber(number);
            ret->setGeneration(gen);
        }

        if (obj.isInt() || obj.isInt64() || obj.isReal()) {
            switch (obj.getType()) {
            case objInt:
                ret.reset(new PDFWalkerNumber<int>());
                loadNumberObject<int>(&obj, dynamic_cast<PDFWalkerNumber<int>*> (ret.get()));
                break;
            case objInt64:
                ret.reset(new PDFWalkerNumber<long long>());
                loadNumberObject<long long>(&obj, dynamic_cast<PDFWalkerNumber<long long>*> (ret.get()));
            case objReal:
                ret.reset(new PDFWalkerNumber<double>());
                loadNumberObject<double>(&obj, dynamic_cast<PDFWalkerNumber<double>*> (ret.get()));
            default:
                return nullptr;
            }

            ret->setNumber(number);
            ret->setGeneration(gen);
        }

        if (obj.isBool()) {
            ret.reset(new PDFWalkerBoolean());
            loadBooleanObject(&obj, dynamic_cast<PDFWalkerBoolean*> (ret.get()));
            ret->setNumber(number);
            ret->setGeneration(gen);
        }

        if (obj.isStream()) {
            ret.reset(new PDFWalkerStream());
            loadStreamObject(&obj, dynamic_cast<PDFWalkerStream*> (ret.get()));
            ret->setNumber(number);
            ret->setGeneration(gen);
        }
    }

    return ret;
}

std::unique_ptr<PDFWalkerObject> PDFWalker::pdfWalkerObject(const ObjectSharedPtr& object) {
    std::unique_ptr<PDFWalkerObject> ret(nullptr);

    if (object->isDict()) {
        ret.reset(new PDFWalkerDictionary());
        loadDictionaryObject(object.get(), dynamic_cast<PDFWalkerDictionary*> (ret.get()));
        ret->setDirect();
    }

    if (object->isName()) {
        ret.reset(new PDFWalkerName());
        loadNameObject(object.get(), dynamic_cast<PDFWalkerName*> (ret.get()));
        ret->setDirect();
    }

    if (object->isArray()) {
        ret.reset(new PDFWalkerArray());
        loadArrayObject(object.get(), dynamic_cast<PDFWalkerArray*> (ret.get()));
        ret->setDirect();
    }

    if (object->isString()) {
        ret.reset(new PDFWalkerString());
        loadStringObject(object.get(), dynamic_cast<PDFWalkerString*> (ret.get()));
        ret->setDirect();
    }

    if (object->isInt() || object->isInt64() || object->isReal()) {
        switch (object->getType()) {
        case objInt:
            ret.reset(new PDFWalkerNumber<int>());
            loadNumberObject<int>(object.get(), dynamic_cast<PDFWalkerNumber<int>*> (ret.get()));
            break;
        case objInt64:
            ret.reset(new PDFWalkerNumber<long long>());
            loadNumberObject<long long>(object.get(), dynamic_cast<PDFWalkerNumber<long long>*> (ret.get()));
        case objReal:
            ret.reset(new PDFWalkerNumber<double>());
            loadNumberObject<double>(object.get(), dynamic_cast<PDFWalkerNumber<double>*> (ret.get()));
        default:
            return nullptr;
        }

        ret->setDirect();
    }

    if (object->isBool()) {
        ret.reset(new PDFWalkerBoolean());
        loadBooleanObject(object.get(), dynamic_cast<PDFWalkerBoolean*> (ret.get()));
        ret->setDirect();
    }

    if (object->isStream()) {
        ret.reset(new PDFWalkerStream());
        loadStreamObject(object.get(), dynamic_cast<PDFWalkerStream*> (ret.get()));
        ret->setDirect();
    }

    return ret;
}
