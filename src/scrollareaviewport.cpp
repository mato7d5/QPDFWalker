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

#include "scrollareaviewport.h"

ScrollAreaViewPort::ScrollAreaViewPort(QWidget *parent) : QWidget(parent), mLastWindowIndex(0)
{

}

//methods
void ScrollAreaViewPort::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    emit lastWindowIndexChanged(mLastWindowIndex);
}
