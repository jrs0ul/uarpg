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
#ifndef _TEXTSCREEN_H_
#define _TEXTSCREEN_H_


#include "BasicUiControl.h"
#include "../TextureLoader.h"


struct buffstring{
    char s[31];
};


//================================================
//console object
    class TextScreen:public BasicControl{

        unsigned _height;
        unsigned _width;

        unsigned oldkey;

        DArray<buffstring> buffer;
        unsigned bufpy;
        unsigned bufpx;  //buferio pointeriai
        char lines[22][31]; //visible lines

    public:

        bool wait;//wait for key press ?


        void init(unsigned newx,unsigned newy,
                  unsigned height_, unsigned width_);
        //puts string s to buffer
        void     putText(const char* s);
        //puts text from file into the buffer
        void     readFile(const char* path);
        /*jei bufpx<>0 tai spausdina eilute jei bufpy<>0 tai ima nauja eilute
        is buferio*/
        void     type(unsigned int key);
        //draws text box
        void     draw(PicsContainer& pics, int remelis, int font, int rod);
        //clears text lines
        void     clear();
        void     setWidth(unsigned int newwidth){ _width = newwidth; }
        void     setHeight(unsigned int newheight) {_height = newheight; }
        unsigned width(){return _width; }
        unsigned height(){return _height; }

                 TextScreen():BasicControl(){ oldkey = 0;}

        void     destroy(){buffer.destroy();}
    };



#endif //_TEXTSCREEN_H_
