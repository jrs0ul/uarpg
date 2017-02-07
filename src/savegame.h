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

#ifndef SAVEGAME_H
#define SAVEGAME_H


#include "Party.h"
#include "variables.h"
#include "Treasure.h"

//still needed ??
/*struct SaveGame{
    CreatureData dParty[4];      //Party members
    int x, y;                   //coordinates in the map
    unsigned steps;             //steps
    unsigned alive;             //how many alive
    unsigned count;             //member count
    char currentmap[255];       //current ma
 };*/

 bool LoadState(Party& partis,
                Party& innparty,
                NpcArray& npcs,
                CVariableArray& vars,
                OpenedChests& locks, char* map, const char* path, 
                ItemData& data, const char* BasePath);
 bool SaveState(Party& partis,
                Party& innparty,
                NpcArray& npcs,
                CVariableArray& vars,
                OpenedChests& locks, const char* map,
                const char* path);



#endif //SAVEGAME_H

