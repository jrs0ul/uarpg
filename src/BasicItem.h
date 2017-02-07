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
#ifndef _BASIC_ITEM_H
#define _BASIC_ITEM_H


/*Basic Item - will be use to form inventory items info and spells*/
enum BasicItemEffect {DOESGOOD, DOESDAMAGE};
//------------------------------
class BasicItem{
public:
    char            name[50];       //Item name
    int             cost;            //Item cost
    bool            useInCombat;    //Use item in combat
    bool            useInWorld;     //Use item while not fighting
    char            codeLine[255];  //Line of script
    bool            needOponent;    //do you need someone to use item on ?
    BasicItemEffect _type;

    BasicItem(){
        useInCombat = true;
        useInWorld = true;
        _type = DOESGOOD;
    }
};






#endif //_BASIC_ITEM_H
