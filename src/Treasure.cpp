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
#include "Treasure.h"
#include <cstdio>

    bool OpenedChests::loadBin(FILE * f){


        if (!f)
            return false;
        
        unsigned long _count = 0;
        if (!fread(&_count, sizeof(unsigned long), 1, f))
            return false;
        for (unsigned long i = 0; i < _count; i++){

            Chest newchest;
            if (!fread(&newchest.x, sizeof(unsigned), 1, f))
                return false;
            if (!fread(&newchest.y, sizeof(unsigned), 1, f))
                return false;
            if (!fread(&newchest.layer, sizeof(unsigned), 1, f))
                return false;
            if (!fread(&newchest.tilenum, sizeof(unsigned), 1, f))
                return false;
            int namelen = 0;
            if (!fread(&namelen, sizeof(int), 1, f))
                return false;
            if (namelen){
                if (!fread(newchest.mapname, sizeof(char), namelen, f))
                    return false;
                newchest.mapname[namelen] = 0;
            }

            chests.add(newchest);
        }

        return true;

    }

//--------------------------------------------

    bool OpenedChests::saveBin(FILE * f){
        
        if (!f)
            return false;
       
        unsigned long _count = chests.count();
        fwrite(&_count, sizeof(unsigned long), 1, f);
        for (unsigned long i = 0; i < _count; i++){
            fwrite(&chests[i].x, sizeof(unsigned), 1, f);
            fwrite(&chests[i].y, sizeof(unsigned), 1, f);
            fwrite(&chests[i].layer, sizeof(unsigned), 1, f);
            fwrite(&chests[i].tilenum, sizeof(unsigned), 1, f);
            int mapnamelen = (int)strlen(chests[i].mapname);
            fwrite(&mapnamelen, sizeof(int), 1, f);
            if (mapnamelen)
                fwrite(&chests[i].mapname, sizeof(char), mapnamelen, f);
        }

        return true;


    }
//----------------------------------------------
    Chest * OpenedChests::get(unsigned x, unsigned y, const char* name){

        for (unsigned long i = 0; i < chests.count(); i++){
            if ((chests[i].x == x) && (chests[i].x == y) && (strcmp(name, chests[i].mapname) == 0))
                return &chests[i];


        }

        return 0;

    }





