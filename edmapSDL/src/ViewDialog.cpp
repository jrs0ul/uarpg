/***************************************************************************
 *   Copyright (C) 2009 by jrs0ul                                          *
 *   jrs0ul@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "ViewDialog.h"
#include "../../src/gui/Text.h"


void ViewDialog::set(int _x, int _y, int w, int h){
    setpos(_x, _y);
    width = w; height = h;
    showGrid.init(getX() + 10, getY() + 10, 16, 16);
    showNpc.init(getX() + 10, getY() + 26, 16, 16);
    snapToGrid.init(getX() + 10, getY() + 42, 16, 16);
}
//--------------------------------------
void ViewDialog::draw(PicsContainer& pics){
    pics.draw(-1, getX(), getY(), 0, false, (float)width, (float)height, 0.0f,
              COLOR(0.8f, 0.8f, 0.8f, 0.5f),
              COLOR(0.8f, 0.8f, 0.8f, 0.5f));
    showGrid.draw(pics, 3, (showGrid.pressed) ? 1: 0);
    showNpc.draw(pics, 3, (showNpc.pressed) ? 1: 0);
    snapToGrid.draw(pics, 3, (snapToGrid.pressed) ? 1: 0);
    WriteText(getX() + 30, getY() + 12, pics, 1, "Show grid", 0.5f);
    WriteText(getX() + 30, getY() + 28, pics, 1, "Show Npc", 0.5f);
    WriteText(getX() + 30, getY() + 44, pics, 1, "Snap to grid", 0.5f);
}
//-----------------------------------------
bool ViewDialog::isPointerOnTop(int px, int py){
    if ((px > getX()) && (px < getX() + width)
            && (py > getY()) && (py < getY() + height))

        return true;

    return false;
}


