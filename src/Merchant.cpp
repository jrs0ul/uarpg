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
#include "Merchant.h"
#include "Xml.h"

    bool Merchant::load(const char * path){

        destroy();

        Xml data;

        if (!data.load(path))
            return false;
        XmlNode * shop = 0;
        shop = data.root.getNode(L"Shop");
        if (shop){
            for (unsigned long i = 0; i < shop->childrenCount(); i++){
                XmlNode * item = 0;
                item = shop->getNode(i);
                if (item){
                    Sellable s;
                    char buf[255];
                    wcstombs(buf, item->getAttribute(0)->getValue(), 255);
                    s.itemIndex = (unsigned)atoi(buf);
                    wcstombs(buf, item->getAttribute(1)->getValue(), 255);
                    s.markup = atoi(buf);
                    items.add(s);

                }
            }
        }
    

        data.destroy();

        return true;
    }
//-------------------------------------
    void Merchant::buildMenu(SelectMenu& menu, ItemData& itemdata){
        SMenu stuff;

        for (unsigned long i = 0; i < items.count(); i++){
            MenuItem itm;
            sprintf(itm.name, "%s %dG", itemdata.info[items[i].itemIndex].name,
                    itemdata.info[items[i].itemIndex].cost +  items[i].markup);
            stuff.add(itm);
        }
        menu.destroy();
        stuff.setTitle(" ");
        menu.init(300, 10, stuff, 0);
        stuff.destroy();

    }
//-------------------------------------
    void Merchant::destroy(){
        items.destroy();
    }
//-------------------------------------
    Sellable * Merchant::get(unsigned long i){
        if (i < items.count()){
            return &items[i];
        }
        return 0;
    }

