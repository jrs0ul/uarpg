/***************************************************************************
 *
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
#include "ItemGain.h"
#include "Coolframe.h"
#include "Text.h"

void ItemGainWindow::draw(Party & party, PicsContainer& pics, int font, int frame){

    int Xstep = 110;
    
    pics.draw(-1, getX()*1.0f, getY()*1.0f, 0, false, _width*1.0f, _height*1.0f,
               0, COLOR(0.0f, 0.0f, 1.0f), COLOR(0.0f, 0.0f, 0.0f));


    for (unsigned i = 0; i < party.count(); i++){
        WriteText(getX()+10 + i*Xstep, getY()+10, pics, font, party.member(i)->name);

        int count = 0;
        for (unsigned a = 0; a < items.count(); a++){
            if (items[a].member == i){
                WriteText(getX()+10 + i*Xstep, getY()+30 + count*18, pics, font, items[a].itemName);
                count ++;
            }
        }

    }



    Coolframe(getX(), getY(), (unsigned)width(), (unsigned)height(), pics, frame);


}
//----------------------------------
void ItemGainWindow::getInput(unsigned key){
    if (pressedkey != key){
        if ((pressedkey == 13) ||
            (pressedkey == 27)){
            if (active()){
                deactivate();
                destroy();
            }
        }
            

        pressedkey = key;
    }

}
//----------------------------------
void ItemGainWindow::addItem(unsigned member, const char* item){
    DialogItem i;
    i.member = member;
    strncpy(i.itemName, item, 100);
    items.add(i);
}
