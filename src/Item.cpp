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
#include "Item.h"
#include "Xml.h"
#include "Utils.h"

//===================================
//Loads item information
bool ItemData::load(const char* path){

    Xml data;

    if (!data.load(path))
        return false;

    XmlNode * items = 0;

    items = data.root.getNode(L"Items");
    if (items){
        unsigned long itemCount = 0;
        itemCount = items->childrenCount();
        printf("Item count %lu\n", itemCount);
        for (unsigned long i = 0; i < itemCount; i++){

            ItemInfo newItem;

            XmlNode * item = 0;
            item = items->getNode(i);
            if (item){
                convertLT(item->getNode(L"Name")->getValue(), newItem.name, 50);
                int isWeapon = 0;
                int isArmor = 0;
                int needop = 0;
                int combat = 0;
                int map = 0;

                char tmp[100];

                wcstombs(tmp, item->getNode(L"Cost")->getValue(), 100);
                newItem.cost = atoi(tmp);
                wcstombs(tmp, item->getNode(L"Weight")->getValue(), 100);
                newItem.weight = (unsigned)atoi(tmp);
                wcstombs(tmp, item->getNode(L"isWeapon")->getValue(), 100);
                isWeapon = atoi(tmp);
                wcstombs(tmp, item->getNode(L"isArmor")->getValue(), 100);
                isArmor = atoi(tmp);
                wcstombs(tmp, item->getNode(L"needOponent")->getValue(), 100);
                needop = atoi(tmp);
                wcstombs(tmp, item->getNode(L"useInCombat")->getValue(), 100);
                combat = atoi(tmp);
                wcstombs(tmp, item->getNode(L"useInMap")->getValue(), 100);
                map = atoi(tmp);
                XmlNode * doesDmg = 0;
                int doesDamage = 0;
                doesDmg = item->getNode(L"DoesDamage");
                if (doesDmg){
                    wcstombs(tmp, doesDmg->getValue(), 100);
                    doesDamage = atoi(tmp);
                }

                newItem.isweapon = (isWeapon == 1) ? true : false;
                newItem.isarmor = (isArmor == 1) ? true : false;
                newItem.needOponent = (needop == 1) ? true : false;
                newItem.useInCombat = (combat == 1) ? true : false;
                newItem.useInWorld = (map == 1) ? true : false;
                newItem._type = (doesDamage)? DOESDAMAGE : DOESGOOD;
                    

                XmlNode * bonus = 0;
                bonus = item->getNode(L"EquipBonus");
                if (bonus){
                    puts("bonuses");
                    wcstombs(tmp, bonus->getAttribute(0)->getValue(), 100);
                    newItem.damage = atoi(tmp);
                    wcstombs(tmp, bonus->getAttribute(1)->getValue(), 100);
                    newItem.defense = atoi(tmp);
                    wcstombs(tmp, bonus->getAttribute(2)->getValue(), 100);
                    newItem.agility = atoi(tmp);
                }
                wcstombs(newItem.codeLine, item->getNode(L"Script")->getValue(), 255);

            }
            info.add(newItem);
        }
    }

    data.destroy();
    return true;

}
//------------------
    void ItemData::destroy(){
        info.destroy();
    }
//===================================
    void Item::set(int nkind, int hd, unsigned ncount){
        kind = nkind;
        _health = hd;
        count = ncount;
    }
//-----------------------------------
    bool Item::set(const char * name, int hd, unsigned ncount, ItemData& data){
        unsigned i=0;
        while ((strcmp(data.info[i].name, name) != 0) && (i < data.info.count())){
            /*char tmp[100];
            sprintf(tmp,"%d", strlen(data.info[i].name));
            puts(data.info[i].name);
            puts(tmp);
            sprintf(tmp,"%d", strlen(name));
            puts(name);
            puts(tmp);*/
            //puts(data.info[i].name);
            //puts(name);
            i++;
        }
        if (i == data.info.count())
            return false;

        //puts("FOUND IT");

        kind = i; _health = hd; count = ncount;
            return true;
}


