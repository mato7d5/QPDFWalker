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

#ifndef PDFWALKERSTREAM_H
#define PDFWALKERSTREAM_H

#include "global.h"
#include "pdfwalkerobject.h"
#include "pdfwalkerdictionary.h"

class PDFWalkerStream : public PDFWalkerObject
{
private:
    ObjectSharedPtr mObject;

public:
    PDFWalkerStream();
    ~PDFWalkerStream();

    void setValue(Object* obj) { mObject.reset(obj); }
    ObjectSharedPtr value() const { return mObject; }
    std::shared_ptr<PDFWalkerDictionary> getStreamDict();
};

#endif // PDFWALKERSTREAM_H