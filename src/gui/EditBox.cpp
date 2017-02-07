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

#include "EditBox.h"
#include "Coolframe.h"
#include "Text.h"

void EditBox::init(unsigned int dx, unsigned int dy,
                   const char* dtitl, unsigned char dmaxl){
    setpos(dx,dy);
    strcpy(title, dtitl);
    maxlength = dmaxl;
    entered = false;
    strcpy(_text, "");
    pressedKey = 0;
}
//------------------------------------
void EditBox::addchar(char c){

    unsigned ilgis = (int)strlen(_text);
    if (ilgis < maxlength){

        _text[ilgis] = c;
        _text[ilgis + 1] = '\0';
    }
}
//-----------------------------------
void EditBox::delchar(){

    int ilgis = (int)strlen(_text);
    if (ilgis > 0)
        _text[ilgis - 1] = '\0';
}
//------------------------------------
void EditBox::getInput(unsigned key){
    if (pressedKey!=key){
        if ( pressedKey == 13 )
            entered = true;
        if ( pressedKey == 27 )
            canceled = true;

    unsigned ilgis = (unsigned)strlen(_text);
    pressedKey = key;
    if ((key > 31) && (key < 127)){
        if (ilgis < maxlength){
            _text[ilgis] = (char)key;
            _text[ilgis + 1] = '\0';
        }
    }
    else
        if (key==8){ //backspace
            if (ilgis>0){
                _text[ilgis-1]='\0';
            }
            else 
                if (key!=13) 
                    pressedKey=0;
        }
    }
}
//------------------------------------
void EditBox::draw(PicsContainer &pics, int font, int frame, 
                   COLOR c1, COLOR c2, bool drawFrame,
                   bool cursorBlink){

    int ilgis;
    if (strlen(title) > maxlength)
        ilgis = (int)strlen(title);
    else
        ilgis = maxlength;

    pics.draw(-1, getX(), getY(), 0, false, ilgis * 12,
              45, 0, c1, c2);
    WriteText(getX() + 2, getY() + 2, pics, font, title);

    int rnd = (rand() % 100) % 5;
    if (( rnd == 0)||(!cursorBlink)){
        char tmp[255];
        sprintf(tmp, "%s_", _text);
        WriteText(getX() + 5, getY() + 18, pics, font, tmp);
    }
    else
        WriteText(getX() + 5, getY() + 18, pics, font, _text);

    if (drawFrame)
        Coolframe(getX(), getY(), ilgis * 12, 45, pics, frame);

}
//----------------------------------------------
void EditBox::reset(){
    strcpy(_text,"");
    entered = false;
    canceled = false;
    pressedKey = 0;
}
//------------------------------------------
void EditBox::incDecNumber(unsigned key){

    int num = atoi(_text);
    char buf[100];

    if (pressedKey != key){
        switch(pressedKey){
            case SDLK_UP: {
                sprintf(buf,"%d", num);
                if (strlen(buf) <= maxlength)
                    num++;
            }break;
            case SDLK_DOWN: {
                if (num > 0)
                    num--;
            } break;
        }
        sprintf(buf,"%d", num);
        strncpy(_text, buf, 255);
    }

}
