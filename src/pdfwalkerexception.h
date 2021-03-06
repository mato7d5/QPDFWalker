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

#ifndef PDFWALKEREXCEPTION_H
#define PDFWALKEREXCEPTION_H

#include <exception>
#include <string>

class PDFWalkerException : public std::exception {
private:
    std::string msg;

public:
    PDFWalkerException(const std::string& inMsg) : msg(inMsg) { }
    ~PDFWalkerException() { }

    const char* what() const noexcept override { return msg.c_str(); }
};

#endif // PDFWALKEREXCEPTION_H
