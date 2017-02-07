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
#ifndef CREATURE_STATUS_H
#define CREATURE_STATUS_H


#include "Gui.h"

//Veikeju ar siap NPC snukiu statusai

    class CreatureStatus:public BasicControl{


    public:
        Points hp,mp;
        unsigned picID;
        bool showhpmp;
        bool enemyface;
        char name[100];
        int face;
        bool dead;
        bool poisoned;

        CreatureStatus() { face=0; dead = false; poisoned = false;}
        void change(unsigned int dx, unsigned int dy, const char* dname,
                     int dface, unsigned pID);
        void draw(PicsContainer& pics, int frame, int mark, int font, bool drawname = true);
        ~CreatureStatus(){face=0;}

    };

#endif //CREATURE_STATUS_H
