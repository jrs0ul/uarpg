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
#include <cmath>
#include "OnScreenBoard.h"
#include "Coolframe.h"
#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL.h>
    #else
        #include <SDL/SDL.h>
    #endif
#else
  #include <SDL/SDL.h>
#endif


    void   OnScreenKeyboard::setChars(const unsigned char * newchars){
        if (newchars){
            size_t len = 0;
            len = (strlen((const char*)newchars) > 254) ? 254 : strlen((const char*)newchars);
            memcpy(characters, newchars, len);
            characters[len] = '-';
            characters[len + 1] = 'v';
            count = len + 2;
        }
    }
//---------------------------
    void   OnScreenKeyboard::setRowLen(unsigned newLen){
        if (newLen < count)
            rowLength = newLen;
        else
            rowLength = count;

    }
//---------------------------
    void   OnScreenKeyboard::draw(PicsContainer &pics, unsigned long frm,
                                  unsigned long rod,
                                  unsigned long font){

        pics.draw(-1, getX(), getY(), 0, false, 
                  width, height, 0,
                  COLOR(0,0,0.9f,0.8f), COLOR(0,0,0.0f,0.8f));

        unsigned rows = (unsigned)ceilf((count*1.0f)/rowLength);
        unsigned index = 0;
        for (unsigned i = 0; i < rows; i++){
            for (unsigned a = 0; a < rowLength; a++){
                if(index < count){
                    if (index == state){
                        pics.draw(-1, getX()+10+a*18, getY()+10+i*18, 0, false, 
                                  16, 16, 0,
                                  COLOR(1,0,0.0f,0.8f), COLOR(1,0,0.0f,0.8f));
                    }


                    if (index == count - 2){
                        pics.draw(rod, getX()+10+a*18, getY()+10+i*18,
                                  5, false, 
                                  1.0f, 1.0f, 0,
                                  COLOR(1,1,1.0f,1.0f), COLOR(1,1,1.0f,1.0f));

                    }
                    else
                        if (index == count - 1){
                            pics.draw(rod, getX()+10+a*18, getY()+10+i*18,
                                      4, false, 
                                      1.0f, 1.0f, 0,
                                      COLOR(1,1,1.0f,1.0f), COLOR(1,1,1.0f,1.0f));
                        }
                        else
                            pics.draw(font, getX()+10+a*18, getY()+10+i*18,
                                      characters[index]-32, false, 
                                      1.0f, 1.0f, 0,
                                      COLOR(1,1,1.0f,1.0f), COLOR(1,1,1.0f,1.0f));
                    index++;
                }
            }

        }


    Coolframe(getX(),getY(),width,height, pics, frm);
}
//---------------------------
    unsigned char OnScreenKeyboard::getInput(unsigned key){

        if (key != pressedkey){

            
            if (pressedkey == 27) {
                canceled = true;      //esc
            }
            
            if (pressedkey == 13){

                if (state == count - 1){
                    entered = true;
                    pressedkey = key;
                    return 0;
                }

                if (state == count - 2){
                    pressedkey = key;
                    return 8;
                }

                pressedkey = key;
                return characters[state];
            }


            if (count){

               printf("%u\n", key); 
                switch(key) {

                case 17:{ //up
                    if (state > rowLength)
                        state -= rowLength;
                }  break;
                case 18:{ //down
                    if (state + rowLength < count)
                        state += rowLength;

                }  break;
                case 19:{ //left
                    if (state < count - 1)
                        state++;
                }  break;

                case 20:{ //right
                    if (state > 0)
                        state--;
                } break;

                
                default: pressedkey = 0;
                }
            }


            pressedkey = key;
        }

        return 0;

    }

