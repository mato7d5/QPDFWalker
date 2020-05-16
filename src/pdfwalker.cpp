/*
Copyright 2016 - 2020 Martin Mancuska <mmancuska@gmail.com.>
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
#include "passworddialog.h"
#include <utility>

#include <QString>

PDFWalker::PDFWalker(const std::string& fileName) : mFileName(fileName) {
    mPDFDoc = std::make_unique<PoDoFo::PdfMemDocument> ();
    bool encrypted = false;

    try {
        mPDFDoc->Load(mFileName.c_str());
    }
    catch (const PoDoFo::PdfError &e) {
        if (e.GetError() == PoDoFo::ePdfError_InvalidPassword) {
            encrypted = true;
        }
        else
            throw PDFWalkerException("Cannot open file.");
    }

    if (encrypted) {
        for (int i = 0; i < 3; ++i) {
            std::unique_ptr<PasswordDialog> dlg(new PasswordDialog);

            if (dlg->exec() == QDialog::Rejected)
                throw PDFWalkerException("Encrypted PDF. Please, enter the password!");

            std::string password = dlg->password().toStdString();
            mPDFDoc->SetPassword(password);

            try {
                mPDFDoc->Load(mFileName.c_str());
                break;
            }
            catch (const PoDoFo::PdfError& e) {
                if (e.GetError() == PoDoFo::ePdfError_InvalidPassword)
                    continue;
                else
                    throw PDFWalkerException("Cannot open file.");
            }
        }
    }

    if (!mPDFDoc->IsLoaded())
        throw PDFWalkerException("Cannot open file.");
}

PDFWalker::~PDFWalker() {

}

//methods
const PoDoFo::PdfObject* PDFWalker::getReferencedObject(const PoDoFo::PdfReference &reference) {
    return mPDFDoc->GetObjects().GetObject(reference);
}

QString PDFWalker::objTypeString(const PoDoFo::PdfObject& object) {
    if (object.IsBool())
        return "Bool";
    else if (object.IsNumber())
        return "Int";
    else if (object.IsReal())
        return "Real";
    else if (object.IsString())
        return "String";
    else if (object.IsName())
        return "Name";
    else if (object.IsNull())
        return "Null";
    else if (object.IsArray())
        return "Array";
    else if (object.IsDictionary())
        return "Dictionary";
    else if (object.IsRawData())
        return "Stream";
    else if (object.IsHexString())
        return "HexString";
    else if (object.IsEmpty())
        return "Empty";
    else
        return "";

    return "";
}
