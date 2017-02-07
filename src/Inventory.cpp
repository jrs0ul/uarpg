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

#include "Inventory.h"
#include <cstring>



    unsigned int Inventory::getItemCount(unsigned rusis){

        unsigned item_count = 0;
        for (unsigned i = 0; i < items.count(); i++){
        if (items[i].kind == (int)rusis)
            item_count += items[i].count;
        }
            return item_count;
    }

//-----------------------------------
    unsigned Inventory::getItemCountByName(const char* name, ItemData& data){

        unsigned int item_count = 0;
        for (unsigned i = 0; i < items.count(); i++){
     if (strcmp(data.info[items[i].kind].name,name)==0)
      item_count += items[i].count;
        }
        return item_count;
    }
//------------------------------

void Inventory::addItem(Item *itm, ItemData& data){

    if (items.count() == 0){
        Item naujas;
        naujas.set(itm->kind, itm->health(), 1);
        items.add(naujas);
    }
    else{
        int itemkind = itm->kind;
        //char buf[255];
        //sprintf(buf, "item kind %d, item data count %d",
        //itemkind, data.info.count());
        //puts(buf);

        if (((data.info[itemkind].isarmor)||(data.info[itemkind].isweapon))
            ||(getItemCount(itemkind)==0)){

            Item naujas;
            naujas.set(itm->kind, itm->health(), 1);
            items.add(naujas);
        }
        else{

         int kuris=findItem(itm->kind);

         items[kuris].count++;
        }

    }

}

//------------------------
bool Inventory::removeItemByIndex(unsigned int index){
    if (index<items.count()){
        if ((items.count()==1)&&(items[index].count==1)){
            destroy();
        }
        else{

            if (items[index].count==1){

                items.remove(index);
            }
            else{
             items[index].count--;
            }

        }
        return true;
    }
    return false;
}
//---------------------------
    bool Inventory::removeItemByKind(unsigned kind){

        int index = findItem(kind);
        if (index < 0)
            return false;
        removeItemByIndex(index);
        return true;
    }

//---------------------------

    void Inventory::destroy(){
        items.destroy();
    }

//-------------------
    Item* Inventory::getItem(unsigned int index){
        Item* result=0;
        if (index<items.count()){
            result=&items[index];
        }
        return result;
    }

//-------------------
    unsigned int Inventory::totalItemCount(){
        int result = 0;
    for (unsigned i = 0; i < items.count(); i++){
        result += items[i].count;
    }
        return result;
    }
//-------------------
    unsigned int Inventory::itemWeight(ItemData& itemz){

        unsigned  weight = 0;
        for (unsigned i = 0; i < items.count(); i++){
        weight += itemz.info[items[i].kind].weight * items[i].count;
        }

        return weight;
    }
//---------------------------
    int Inventory::findItem(unsigned kind){
        unsigned result = 0;
        if (items.count()){
            while ((items[result].kind != (int)kind)
                    &&(result < items.count()))
                result++;
        }
        if (result == items.count())
            return -1;
        return result;
    }
//-----------------------------------------------
    int Inventory::findItemByName(const char* name,ItemData& data){
        unsigned result = 0;
        if (items.count()){
            bool found = false;
            while ((!found) && (result < items.count())){
                if (strcmp(data.info[items[result].kind].name, name) == 0)
                    found=true;
                else
                    result++;
            } 
        }
        if (result == items.count())
            return -1;
        return result;
    }
