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
#include "Strategy.h"

    void Strategy::destroy(){

        attackItems.destroy();
        attackSpells.destroy();
        restoreHPItems.destroy();
        restoreHPSpells.destroy();
        restoreMPItems.destroy();
        restoreMPSpells.destroy();
        antidoteItems.destroy();
        antidoteSpells.destroy();
    }
    //---------------------------------
    void _sort(StrategyElement * m, unsigned count){

        for (unsigned a = 0; a < count; a++) {
            for (unsigned i = 0; i < count - 1; i++) {
                if (m[i].weight < m[i + 1].weight){
                    StrategyElement k1 = m[i];
                    StrategyElement k2 = m[i + 1];
                    m[i] = k2;
                    m[i + 1] = k1;
                }
            }
        }
    }
    //-----------------------------

    void Strategy::sort(){
        if (attackItems.count())
            _sort((StrategyElement*)attackItems.getData(), attackItems.count());
        if (attackSpells.count())
            _sort((StrategyElement*)attackSpells.getData(), attackSpells.count());
        if (restoreHPItems.count())
            _sort((StrategyElement*)restoreHPItems.getData(), restoreHPItems.count());
        if (restoreHPSpells.count())
            _sort((StrategyElement*)restoreHPSpells.getData(), restoreHPSpells.count());
        if (restoreMPItems.count())
            _sort((StrategyElement*)restoreMPItems.getData(), restoreMPItems.count());
        if (restoreMPSpells.count())
            _sort((StrategyElement*)restoreMPSpells.getData(), restoreMPSpells.count());
        if (antidoteItems.count())
            _sort((StrategyElement*)antidoteItems.getData(), antidoteItems.count());
        if (antidoteSpells.count())
            _sort((StrategyElement*)antidoteSpells.getData(), antidoteSpells.count());
    }

    //-------------------------------------
    void Strategy::copyFrom(Strategy& s){

        helpFriends = s.helpFriends;
        useItemForHP = s.useItemForHP;
        //HP
        for (unsigned i = 0; i < s.restoreHPItems.count(); i++){
                StrategyElement e;
                e.index = s.restoreHPItems[i].index;
                e.weight = s.restoreHPItems[i].weight;
                restoreHPItems.add(e);
        }
        useSpellForHP = s.useSpellForHP;
        for (unsigned i = 0; i < s.restoreHPSpells.count(); i++){
                StrategyElement e;
                e.index = s.restoreHPSpells[i].index;
                e.weight = s.restoreHPSpells[i].weight;
                restoreHPSpells.add(e);
        }
        //MP
        useItemForMP = s.useItemForMP;
        for (unsigned i = 0; i < s.restoreMPItems.count(); i++){
                StrategyElement e;
                e.index = s.restoreMPItems[i].index;
                e.weight = s.restoreMPItems[i].weight;
                restoreMPItems.add(e);
        }
        useSpellForMP = s.useSpellForMP;
        for (unsigned i = 0; i < s.restoreMPSpells.count(); i++){
                StrategyElement e;
                e.index = s.restoreMPSpells[i].index;
                e.weight = s.restoreMPSpells[i].weight;
                restoreMPSpells.add(e);
        }
        //Attack
        useItemToAttack = s.useItemToAttack;
        for (unsigned i = 0; i < s.attackItems.count(); i++){
                StrategyElement e;
                e.index = s.attackItems[i].index;
                e.weight = s.attackItems[i].weight;
                attackItems.add(e);
        }
        useSpellToAttack = s.useSpellToAttack;
        for (unsigned i = 0; i < s.attackSpells.count(); i++){
                StrategyElement e;
                e.index = s.attackSpells[i].index;
                e.weight = s.attackSpells[i].weight;
                attackSpells.add(e);
        }
        //Antidote
        useItemForAntidote = s.useItemForAntidote;
        for (unsigned i = 0; i < s.antidoteItems.count(); i++){
                StrategyElement e;
                e.index = s.antidoteItems[i].index;
                e.weight = s.antidoteItems[i].weight;
                antidoteItems.add(e);
        }
        useSpellForAntidote = s.useSpellForAntidote;
        for (unsigned i = 0; i < s.antidoteSpells.count(); i++){
                StrategyElement e;
                e.index = s.antidoteSpells[i].index;
                e.weight = s.antidoteSpells[i].weight;
                antidoteSpells.add(e);
        }
       
    }
