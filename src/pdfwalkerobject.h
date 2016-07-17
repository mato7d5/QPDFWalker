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

#ifndef PDFWALKEROBJECT_H
#define PDFWALKEROBJECT_H

#include <Object.h>

class PDFWalkerObject
{
protected:
    ObjType mObjType = objNull;
    int mNumber = -1;
    int mGen = -1;

public:
    PDFWalkerObject();
    virtual ~PDFWalkerObject() { }

    virtual bool isIndirect() const { return mNumber > 0; }
    virtual void setDirect() { mNumber = mGen = -1 ; }

    virtual void setObjType(ObjType type) { mObjType = type; }
    virtual ObjType type() const { return mObjType; }

    virtual void setNumber(int num) { mNumber = num; }
    virtual int number() const { return mNumber; }

    virtual void setGeneration(int gen) { mGen = gen; }
    virtual int generation() const { return mGen; }
};

#endif // PDFWALKEROBJECT_H
