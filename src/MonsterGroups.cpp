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
#include "MonsterGroups.h"
#include <cstdio>
#include "Utils.h"



void MonsterGroup::destroy(){

    for (unsigned i = 0; i < monsters.count(); i++){
        monsters[i].destroy();
    }
    monsters.destroy();
}
//======================================================
     void getStrategyNode(XmlNode * strategy,
                          const wchar_t* nodeName, 
                          DArray<StrategyElement>&  mas,
                          bool& flag){
        char buf[100];
        XmlNode * strategyNode = 0;
        strategyNode = strategy->getNode(nodeName);
        if (strategyNode){
            flag = true;
            for (unsigned i = 0; i < strategyNode->childrenCount(); i++){
                XmlNode * item = 0;
                item = strategyNode->getNode(i);
                if (item){
                    StrategyElement e;

                    wcstombs(buf, 
                             item->getAttribute(0)->getValue(), 100);
                    e.index = (unsigned)atoi(buf);
                    wcstombs(buf, 
                             item->getAttribute(1)->getValue(), 100);
                    e.weight = atof(buf);
                    mas.add(e);
                }
            }
        }
    }

//-----------------------------------------------------------
    void MGroupList::LoadStrategy(XmlNode * monster, Monster* m){

        XmlNode * strategy = 0;

        strategy = monster->getNode(L"Strategy");
        if (strategy){
            char buf[100];
            wcstombs(buf, strategy->getAttribute(0)->getValue(), 100);
            m->ai.helpFriends = (atoi(buf)) ? false : true;
            //Items to restore HP
            getStrategyNode(strategy, L"HPItems", 
                            m->ai.restoreHPItems, m->ai.useItemForHP);
            getStrategyNode(strategy, L"MPItems",
                            m->ai.restoreMPItems, m->ai.useItemForMP);
            getStrategyNode(strategy, L"HPSpells", 
                            m->ai.restoreHPSpells, m->ai.useSpellForHP);
            getStrategyNode(strategy, L"MPSpells", 
                            m->ai.restoreMPSpells, m->ai.useSpellForMP);
            getStrategyNode(strategy, L"AntidoteItems", 
                            m->ai.antidoteItems, m->ai.useItemForAntidote);
            getStrategyNode(strategy, L"AntidoteSpells", 
                            m->ai.antidoteSpells, m->ai.useSpellForAntidote);
            getStrategyNode(strategy, L"AttackItems", 
                            m->ai.attackItems, m->ai.useItemToAttack);
            getStrategyNode(strategy, L"AttackSpells", 
                            m->ai.attackSpells, m->ai.useSpellToAttack);
            

            m->ai.sort();

        } //strategy

    }

//--------------------------------------
bool MGroupList::load(const char *path){

    Xml data;

    if (!data.load(path))
        return false;

    puts("loading monster group..");

    XmlNode * _groups = 0;
    _groups = data.root.getNode(L"MonsterGroups");

    if (_groups){

        groups.destroy();
        for (unsigned long i = 0; i < _groups->childrenCount(); i++){
            MonsterGroup newgroup;
            XmlNode * group = 0;
            group = _groups->getNode(i);
            if (group){
                char buf[256];
                wcstombs(buf, group->getAttribute(0)->getValue(), 255);
                newgroup.mcount = atoi(buf);


                XmlNode * _arenaName = 0;
                _arenaName = group->getNode(L"ArenaName");
                if (_arenaName){
                    wcstombs(newgroup.arenaName, _arenaName->getValue(), 255);
                }

                XmlNode * boss = 0;
                boss = group->getNode(L"Boss");
                if (boss){
                    wcstombs(buf, boss->getValue(), 255);
                    if (atoi(buf)){
                        newgroup.isBoss = true;
                    }
                }

                XmlNode * monsters = 0;
                monsters = group->getNode(L"Monsters");
                if (monsters){
                    for (unsigned long a = 0; a < monsters->childrenCount(); a++){
                        XmlNode * monster = 0;
                        monster = monsters->getNode(a);
                        if (monster){

                            puts("monster!");

                            int ras = 0;
                            int min = 0;
                            int max = 1;
                            int kls = 0;
                            int sx = 0;
                            float eval = 1.0f;
                            char model[255];
                            char texture[255];
                            char mname[255];

                            convertLT(monster->getNode(L"Name")->getValue(),
                                      mname, 255);
                            wcstombs(buf, 
                                     monster->getAttribute(0)->getValue(),
                                     255);
                            ras = atoi(buf);
                            XmlNode * level = 0;
                            level = monster->getNode(L"Level");
                            if (level){
                                wcstombs(buf,
                                         level->getAttribute(0)->getValue(),
                                         255);
                                min = atoi(buf);
                                wcstombs(buf, 
                                         level->getAttribute(1)->getValue(),
                                         255);
                                max = atoi(buf);
                            }
                            wcstombs(buf,
                                     monster->getNode(L"ExpVal")->getValue(),
                                     255);
                            eval = atof(buf);
                            wcstombs(buf,
                                     monster->getAttribute(1)->getValue(),
                                     255);
                            sx = atoi(buf);
                            wcstombs(buf,
                                     monster->getAttribute(2)->getValue(),
                                     255);
                            kls = atoi(buf);

                            XmlNode * radius = 0;
                            float rad = 0.0f;
                            radius = monster->getNode(L"Radius");
                            if (radius){
                                wcstombs(buf, 
                                         radius->getValue(),
                                         255);
                                rad = atof(buf);
                            }
                            wcstombs(model,
                                     monster->getNode(L"Model")->getValue(),
                                     255);
                            wcstombs(texture,
                                     monster->getNode(L"Texture")->getValue(),
                                     255);


                            newgroup.add(min, max, eval, ras, sx,
                                         kls, rad, model, texture, mname);

                            XmlNode * items = 0;
                            items = monster->getNode(L"Items");

                            if (items){
                                for (unsigned long z = 0; z < items->childrenCount(); z++ ){
                                    XmlNode * itm = 0;
                                    MonsterItem mi;
                                    itm = items->getNode(z);
                                    char buf[255];
                                    wcstombs(buf, itm->getAttribute(0)->getValue(), 255);
                                    mi.index = (unsigned)atoi(buf);
                                    wcstombs(buf, itm->getAttribute(1)->getValue(), 255);
                                    mi.mincount = (unsigned)atoi(buf);
                                    wcstombs(buf, itm->getAttribute(2)->getValue(), 255);
                                    mi.maxcount = (unsigned)atoi(buf);

                                    newgroup.monsters[newgroup.monsters.count() - 1].inventory.add(mi);
                                }
                            } //if (items)

                            XmlNode * equiped = 0;
                            equiped = monster->getNode(L"Equiped");
                            if (equiped){
                                char buf[100];
                                unsigned index = newgroup.monsters.count() - 1;
                                wcstombs(buf, equiped->getAttribute(0)->getValue(), 100);
                                newgroup.monsters[index].rightHand = atoi(buf);
                                wcstombs(buf, equiped->getAttribute(1)->getValue(), 100);
                                newgroup.monsters[index].leftHand = atoi(buf);
                                wcstombs(buf, equiped->getAttribute(2)->getValue(), 100);
                                newgroup.monsters[index].arrmor = atoi(buf);
                            }
                            //Loads enemy strategy
                            Monster* m = &newgroup.monsters[newgroup.monsters.count() - 1];
                            LoadStrategy(monster, m);
                        }
                    }
                }
            }
            groups.add(newgroup);
        }
    }
    data.destroy();

    return true;
}
//---------------------------------------------
void MGroupList::destroy(){
    for(unsigned long i = 0; i < groups.count(); i++){
        groups[i].destroy();
    }
    groups.destroy();
}
//----------------------------------------------
MonsterGroup * MGroupList::get(unsigned long index){
    if (index < groups.count()){
        return &groups[index];
    }
    return 0;
}
