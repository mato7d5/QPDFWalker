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

#include "pdfwalkerstream.h"
#include "pdfwalker.h"
#include <Stream.h>
#include <Dict.h>
#include <utility>

PDFWalkerStream::PDFWalkerStream() : mObject(nullptr)
{
    mObjType = objStream;
}

PDFWalkerStream::~PDFWalkerStream() {
    if (mObject)
        mObject->free();
}

//methods
std::shared_ptr<PDFWalkerDictionary> PDFWalkerStream::getStreamDict() {
    if (mObject == nullptr)
        return nullptr;

    Dict* streamDict = mObject->getStream()->getDict();
    if (streamDict == nullptr)
        return nullptr;

    std::shared_ptr<PDFWalkerDictionary> ret = std::make_shared<PDFWalkerDictionary> ();

    for (int i = 0; i < streamDict->getLength(); ++i) {
        PDFWalkerDictionary::DictionaryData data;
        data.key = streamDict->getKey(i);
        ObjectSharedPtr value = std::make_shared<Object> ();
        streamDict->getValNF(i, value.get());
        data.value = value;

        ret->addItem(data);
    }

    return ret;
}