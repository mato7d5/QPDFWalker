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

#ifndef PDFWALKERNUMBER_H
#define PDFWALKERNUMBER_H

#include <QString>
#include <type_traits>
#include "pdfwalkerobject.h"

template<typename T>
class PDFWalkerNumber : public PDFWalkerObject
{
private:
    QString mValue;

public:
    PDFWalkerNumber();

    void setValue(const QString& value) { mValue = value; }
    QString value() const { return mValue; }
};

template<typename T>
PDFWalkerNumber<T>::PDFWalkerNumber() : PDFWalkerObject() {
    static_assert(std::is_arithmetic<T>::value, "None numerical type.");

    if (std::is_integral<T>::value) {
        if (sizeof(T) == sizeof(int))
            mObjType = objInt;
        if (sizeof(T) == sizeof(long long))
            mObjType = objInt64;
    }

    if (std::is_floating_point<T>::value)
        mObjType = objReal;
}

#endif // PDFWALKERNUMBER_H
