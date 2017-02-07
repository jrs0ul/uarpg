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
#ifndef PLAYER_GFX_LIST_H
#define PLAYER_GFX_LIST_H

#include "DArray.h"
#include "Klasses.h"
#include "Races.h"

    struct ResourseNames{
        char m[255]; //model
        char t[255]; //texture
        char s[255]; //sprite
        char f[255]; //face
        float radius; //model radius
    };

    struct Sexes{
        DArray<ResourseNames> sex;
    };

    struct Classes{
        DArray<Sexes> clas;
    };

    class PlayerGfxList{
    public:
        DArray<Classes> races;

        bool load(const char* filename, RaceList& rl, KlassList& kl);
        void destroy();
    };
    //PML.races[race].clas[class].sex[sex];

#endif //PLAYER_MODEL_LIST_H
