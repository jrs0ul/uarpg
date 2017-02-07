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

#ifndef INVENTORY_H
#define INVENTORY_H


#include <cstdio>
#include "Item.h"

//Characters inventory
 class Inventory{
    //Array of item groups (Item element has count)
    DArray<Item> items;

  public:
    //adds SINGLE item to inventory(if itm has count it's simply ignored)
    void          addItem(Item *itm, ItemData& data); 
    //removes SINGLE item (not whole group)
    bool          removeItemByIndex(unsigned int index);
    //same as above, but first finds item of specified kind
    bool          removeItemByKind(unsigned kind);
    //return pointer to specified item group index
    Item*         getItem(unsigned index);
    void          destroy();
    unsigned      getItemCount(unsigned rusis);    //gauna item rushies kieki
    unsigned      getItemCountByName(const char* name, ItemData& data);
    unsigned      totalItemCount(); //grazina visu itemu skaiciu
    unsigned      itemWeight(ItemData& itemz); //grazina visu itemu svori
    //finds item by its kind index
    int           findItem(unsigned kind); 
    int           findItemByName(const char* name, ItemData& data);
    unsigned long itemGroupCount(){return items.count();}

};



#endif //INVENTORY_H
