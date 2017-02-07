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
#ifndef STRATEGY_H
#define STRATEGY_H

#include "DArray.h"

//strategy element for desirable items and spells
    struct StrategyElement{
        unsigned index;
        float weight;
    };
//-------------------------------------
//AI properties and decisions
    struct Strategy{

        bool useItemForHP;
        bool useSpellForHP;
        bool useItemToAttack;
        bool useSpellToAttack;
        bool useItemForAntidote;
        bool useSpellForAntidote;
        bool useSpellForMP;
        bool useItemForMP;
        bool helpFriends;

        DArray<StrategyElement> attackItems;
        DArray<StrategyElement> attackSpells;
        DArray<StrategyElement> restoreHPItems;
        DArray<StrategyElement> restoreHPSpells;
        DArray<StrategyElement> restoreMPSpells;
        DArray<StrategyElement> restoreMPItems;
        DArray<StrategyElement> antidoteItems;
        DArray<StrategyElement> antidoteSpells;

        Strategy(){
            useSpellForHP = false;
            useItemForHP = false;
            useSpellToAttack = false;
            useItemToAttack = false;
            useSpellForMP = false;
            useSpellForMP = false;
            useSpellForAntidote = false;
            useItemForAntidote = false;
            helpFriends = false;

        }

        void sort();
        void copyFrom(Strategy& s);

        void destroy();

    };


#endif //STRATEGY_H
