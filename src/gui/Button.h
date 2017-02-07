/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul                                          *
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
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "../TextureLoader.h"
#include "BasicUiControl.h"

class Button: public BasicControl{

    int height;
    int width;

 public:
    bool pressed;

    void init(int dx = 0,
             int dy = 0,
             int w = 50,
             int h = 50){
        setpos(dx, dy);
        width = w; height = h;
        pressed = false;
    }
    void draw(PicsContainer& pics,
              unsigned picindex, unsigned frame = 0,
              COLOR c = COLOR(1.0f, 1.0f, 1.0f, 1.0f));
    bool isPointerOnTop(int px, int py);
    void set(int dx, int dy, int w, int h){ setpos(dx, dy);
                                            width = w; height = h;}
    void shiftstate();
};
#endif //_BUTTON_H_
