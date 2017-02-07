/***************************************************************************
 *   Copyright (C) 2009 by jrs0ul                                          *
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
#include <cstdio>
#include "TilesetInfo.h"

 void CTilesetInfo::load(const char *name){
        FILE* f;
        f=fopen(name,"rt");
        if (f){
            if (!fscanf(f,"%s\n",tilesetname))
                return;
            if (!fscanf(f,"%s\n",tilesetpic))
                return;
            if (!fscanf(f,"%s\n",tilesetmesh))
                return;
            if (!fscanf(f,"%u\n",&tilecount))
                return;
            for (unsigned i=0;i<tilecount;i++){
                unsigned tmp = 0;
                if (!fscanf(f,"%u", &tmp))
                    return;
                if (tmp)
                    walkable[i] = true;
                else
                    walkable[i] = false;

            }
            fclose(f);
        }

    }

