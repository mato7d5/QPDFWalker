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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "podofo/base/PdfObject.h"
#include "podofo/base/PdfStream.h"
#include "podofo/base/PdfMemStream.h"
#include <memory>
#include <cstring>

class ViewItemData {
private:
    PoDoFo::PdfObject* object { nullptr };
    int currentViewIndex { -1 } ;
    bool release { false };
    char* stream_data { nullptr };
    int stream_data_size { 0 };

public:
    explicit ViewItemData(int idx) :
        currentViewIndex { idx }
    {

    }

    ViewItemData(PoDoFo::PdfObject* obj, int idx) :
        object {obj},
        currentViewIndex {idx}
    {

    }

    ViewItemData(const PoDoFo::PdfObject& obj, int idx) :
        currentViewIndex(idx)
    {
        object = new PoDoFo::PdfObject(obj);
        release = true;
    }

    ViewItemData(const PoDoFo::PdfObject& obj, char* stream, int stream_size, int idx) :
        currentViewIndex { idx },
        stream_data { stream },
        stream_data_size { stream_size }
    {
        object = new PoDoFo::PdfObject(obj);
        release = true;
    }

    ~ViewItemData() {
        if (release) {
            if (object)
                delete object;
        }

        if (stream_data)
            free(stream_data);
    }

    ViewItemData(const ViewItemData& data) {
        if (data.object)
            object = new PoDoFo::PdfObject(*data.object);

        stream_data = (char*) malloc(data.stream_data_size);
        std::memcpy(stream_data, data.stream_data, data.stream_data_size);
        stream_data_size = data.stream_data_size;
        currentViewIndex = data.currentViewIndex;
        release = true;
    }

    ViewItemData& operator=(const ViewItemData& data) {
        if (this != &data) {
            if (release) {
                if (this->object)
                    delete this->object;
            }

            if (stream_data)
                free(stream_data);

            this->object = new PoDoFo::PdfObject(*data.object);
            this->stream_data = (char*) malloc(data.stream_data_size);
            std::memcpy(stream_data, data.stream_data, data.stream_data_size);
            this->stream_data_size = data.stream_data_size;
            this->currentViewIndex = data.currentViewIndex;
            this->release = data.release;
        }

        return *this;
    }

    ViewItemData(ViewItemData&& data) {
        object = data.object;
        stream_data = data.stream_data;
        stream_data_size = data.stream_data_size;
        currentViewIndex = data.currentViewIndex;
        release = data.release;

        data.object = nullptr;
        data.stream_data = nullptr;
        data.stream_data_size = 0;
        data.currentViewIndex = 0;
        data.release = false;
    }

    ViewItemData& operator=(ViewItemData&& data) {
        if (this != &data) {
            if (release) {
                if (this->object)
                    delete this->object;
            }

            if (this->stream_data)
                free(this->stream_data);

            this->object = data.object;
            this->stream_data = data.stream_data;
            this->stream_data_size = data.stream_data_size;
            this->currentViewIndex = data.currentViewIndex;
            this->release = data.release;

            data.object = nullptr;
            data.stream_data = nullptr;
            data.stream_data_size = 0;
            data.currentViewIndex = 0;
            data.release = false;
        }

        return *this;
    }

    const PoDoFo::PdfObject* GetObject() const noexcept { return object; }
    const char* GetStreamData() const noexcept { return stream_data; }
    int GetStreamLength() const noexcept { return stream_data_size; }
    int GetCurrentViewIndex() const noexcept { return currentViewIndex; }
    bool HasStream() const noexcept { return stream_data_size > 0; }
};

#endif // GLOBAL_H
