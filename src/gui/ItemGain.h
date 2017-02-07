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

#ifndef ITEM_GAIN_H
#define ITEM_GAIN_H


#include "BasicUiControl.h"
#include "../Party.h"
#include "../TextureLoader.h"
/*
 * Shows dialog window with items that party
 * looted from enemies
 */


struct DialogItem{
    unsigned member;
    char itemName[100];

    DialogItem(){
        member = 0;
        strcpy(itemName, "");
    }
};

class ItemGainWindow: public BasicControl{
    int _width;
    int _height;
    unsigned pressedkey;
    DArray<DialogItem> items;
public:
    ItemGainWindow(){
        deactivate();
        _width = 440;
        _height = 340;
        pressedkey = 0;
    }

    void setPos(int nx, int ny){
        setpos(nx, ny);
    }

    void draw(Party & party, PicsContainer& pics, int font, int frame);
    void getInput(unsigned key);

    int width(){return _width;}
    int height(){return _height;}

    void addItem(unsigned member, const char* item);
    void destroy(){items.destroy();}
    unsigned long itemCount(){return items.count();}


};


#endif //ITEM_GAIN_H

