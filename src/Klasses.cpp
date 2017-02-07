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
#include "Klasses.h"
#include "Xml.h"
#include <cstdio>
#include "Utils.h"

//--
bool KlassList::load(const char* path){

    Xml data;
    if (!data.load(path))
        return false;

    XmlNode * base = 0;
    base = data.root.getNode(L"Classes");
    if (base){
        
        for (unsigned i = 0; i < base->childrenCount(); i++){

            XmlNode * klass = 0;
            klass = base->getNode(i);
            if (klass){
                    
                XKlass newClass;
                convertLT(klass->getNode(L"Name")->getValue(),
                          newClass.name, 30);
                int playable = 0;
                char tmpnum[30];
                wcstombs(tmpnum, klass->getNode(L"Playable")->getValue(), 30);
                playable = atoi(tmpnum);
                newClass.isPlayable = (playable) ? true : false;

                XmlNode * bonus = 0;
                bonus = klass->getNode(L"Bonus");
                if (bonus){
                    wcstombs(tmpnum, bonus->getNode(L"hp")->getValue(), 30);
                    newClass.hp = atoi(tmpnum);
                    wcstombs(tmpnum, bonus->getNode(L"mp")->getValue(), 30);
                    newClass.mp = atoi(tmpnum);
                    wcstombs(tmpnum, bonus->getNode(L"attack")->getValue(), 30);
                    newClass.atck = atoi(tmpnum);
                    wcstombs(tmpnum, 
                             bonus->getNode(L"deffense")->getValue(), 30);
                    newClass.dfns = atoi(tmpnum);
                    wcstombs(tmpnum, 
                             bonus->getNode(L"agility")->getValue(), 30);
                    newClass.agil = atoi(tmpnum);
                    wcstombs(tmpnum, 
                             bonus->getNode(L"magpower")->getValue(), 30);
                    newClass.magp = atoi(tmpnum);
                    wcstombs(tmpnum, bonus->getNode(L"spirit")->getValue(), 30);
                    newClass.sprt = atoi(tmpnum);
                }

                XmlNode * _spells = 0;
                _spells = klass->getNode(L"Spells");
                if (_spells) {
                    for (unsigned long a = 0; a < _spells->childrenCount(); a++){
                        unsigned spi = 0;
                        wcstombs(tmpnum, _spells->getNode(a)->getValue(), 30);
                        spi = (unsigned)atoi(tmpnum);
                        newClass.spells.add(spi);
                    }
                }

                jobs.add(newClass);
            }
        }
    }

    data.destroy();
    return true;
}
//------------------------------
XKlass * KlassList::get(unsigned long index){

    if (index < jobs.count()){

        return &jobs[index];
    }
    return 0;
}
//------------------------------
void KlassList::destroy(){
    for (unsigned long i = 0; i < jobs.count(); i++){
        jobs[i].destroy();
    }
    jobs.destroy();
}


