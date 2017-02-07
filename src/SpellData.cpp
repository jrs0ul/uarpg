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

#include "SpellData.h"
#include <cstring>
#include "Utils.h"
#include "Xml.h"

bool SpellData::load(const char* path){


    Xml data;

    puts("trying to load...");
    if (!data.load(path))
        return false;

    puts("START");

    XmlNode * spls = 0;
    spls = data.root.getNode(L"Spells");
    if (spls){
        for (unsigned long i = 0; i < spls->childrenCount(); i++){
            Spell newspell;
            XmlNode * sp = 0;
            sp = spls->getNode(i);
            if (sp){
                convertLT(sp->getNode(L"Name")->getValue(), newspell.name, 50);

                char tmp[100];
                wcstombs(tmp, sp->getNode(L"Cost")->getValue(), 100);
                newspell.cost = atoi(tmp);
                int useInCombat = 0;
                int useInWorld = 0;
                int needOponent = 0;
                int doesDamage = 0;
                wcstombs(tmp, sp->getNode(L"Level")->getValue(), 100);
                newspell.level = (unsigned)atoi(tmp);

                XmlNode * doesDmg = 0;
                doesDmg = sp->getNode(L"DoesDamage");
                if (doesDmg){
                    wcstombs(tmp, doesDmg->getValue(), 100);
                    doesDamage = atoi(tmp);
                }
                
                wcstombs(tmp, sp->getNode(L"UseInCombat")->getValue(), 100);
                useInCombat = atoi(tmp);
                wcstombs(tmp, sp->getNode(L"UseInWorld")->getValue(), 100);
                useInWorld = atoi(tmp);
                wcstombs(tmp, sp->getNode(L"NeedOponent")->getValue(), 100);
                needOponent = atoi(tmp);
                newspell.useInCombat = (useInCombat) ? true : false;
                newspell.useInWorld  = (useInWorld) ? true : false;
                newspell.needOponent = (needOponent) ? true : false;
                newspell._type = (doesDamage) ? DOESDAMAGE : DOESGOOD;
                wcstombs(newspell.codeLine, sp->getNode(L"Script")->getValue(), 255);

            }

            spells.add(newspell);
        }
    }

    data.destroy();

    return true;


}
//--------------------
void SpellData::destroy(){
    spells.destroy();
}



