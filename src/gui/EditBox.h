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
#ifndef EDITBOX_H
#define EDITBOX_H


#include "BasicUiControl.h"
#include "../TextureLoader.h"

    class EditBox:public BasicControl{

        char         _text[255];

    public:
        unsigned     maxlength;
        bool         entered;
        bool         canceled;

        char         title[20];
        unsigned     pressedKey;
        void         init(unsigned int dx, unsigned int dy,
                          const char* dtitl, unsigned char dmaxl);
        void         getInput(unsigned key);
        void         draw(PicsContainer &pics, //picture container
                          int font,            //font index in container
                          int frame,            //a frame picture index
                          COLOR c1 = COLOR(0.0f, 0.0f, 0.5f, 0.9f),
                          COLOR c2 = COLOR(0.0f, 0.0f, 1.0f, 0.8f),
                          bool drawFrame = true,
                          bool cursorBlink = true
                         );
        void         reset();
        const char * text(){return _text;}
        void         settext(const char* nt){strncpy(_text, nt, 255);}
        void         addchar(char c);
        void         delchar();
        //if text is number up/down keys increases or decreases it
        void         incDecNumber(unsigned key);

    };


#endif //EDITBOX_H

