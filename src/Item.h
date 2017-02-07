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
#ifndef _ITEM_H
#define _ITEM_H

#include "BasicItem.h"
#include "DArray.h"

//====================================
//Item information, inherits name,cost and script line
    struct ItemInfo: public BasicItem{   
        unsigned weight;                 //Item weight
        bool     isweapon;               //is this a weapon ?
        bool     isarmor;                //is this an armor ?

        //weapons and arrmor
        int damage;
        int defense;
        int agility;
    };
//=================================
    class ItemData{
    public:
        DArray<ItemInfo> info;
        ItemData(){}
        bool load(const char* path);
        void destroy();
    };
//====================================



    class Item{  //Inventory item
            int      _health;
        public:
            int      kind; //item kind
            unsigned count;
                 Item(){}
            void set(int nkind, int hd, unsigned ncount);
            bool set(const char * name,
                     int hd,
                     unsigned ncount,
                     ItemData& data);
            void takeDamage(int value){if (_health > 0) _health -= value;}
            void repair(int value){if (_health < 100) _health += value;}
            int health(){return _health;}

    };

#endif //_ITEM_H
