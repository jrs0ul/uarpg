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
#ifndef OPENED_CHESTS_H
#define OPENED_CHESTS_H

#include "DArray.h"

    struct Chest{

        char mapname[255];
        unsigned x;
        unsigned y;
        unsigned layer;
        unsigned tilenum;

        void set(unsigned nx, unsigned ny, const char * name, 
                 unsigned tile, unsigned _layer){
            x = nx; y = ny;
            tilenum = tile;
            layer = _layer;
            strncpy(mapname, name, 255);
        }
    };



    class OpenedChests{
        DArray<Chest> chests;
    public:

        Chest * get(unsigned x, unsigned y, const char* name);
        Chest * get(unsigned index){
            if (index < chests.count())
                return &chests[index];
            return 0;
        }
        //loads binary portion from savegame
        bool loadBin(FILE * f);
        bool saveBin(FILE * f);
        void destroy(){chests.destroy();}
        unsigned long count(){return chests.count();}
        void add(Chest& chest){chests.add(chest);}

    };



#endif //OPENED_CHESTS_H


