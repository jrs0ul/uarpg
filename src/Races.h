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
#ifndef RACES_H
#define RACES_H



#include <cstdio>
#include "DArray.h"





 struct Resist{
   int fire;    //atsparumas ugniai
   int ice;     //salciui
   int bolt;    //zaibui
   int poison;  //nuodams
 };


  struct Xability{
   int max,min;
  };

  struct BasicAbilities{
   Xability hp;
   Xability mp;
   Xability atck;
   Xability dfns;
   Xability agil;
   Xability magp;
   Xability sprt;
  };

  struct BonusUps{
   unsigned  hp;
   unsigned  mp;
   unsigned  atck;
   unsigned  dfns;
   unsigned  agil;
   unsigned  magp;
   unsigned  sprt;
  };
  //--

    struct Xrace{
        char           name[50];
        unsigned       maxsex; //kiek lyciu
        int            espeed;     //evolution speed
        BasicAbilities startp[2];
        BonusUps       bonusp[2];
        Resist         resistense;
        bool           isPlayable;

        Xrace(){
            strcpy(name, "");
            maxsex = 1;
            espeed = 1;
            isPlayable = true;
        }
    };
 //------


    class RaceList{
        DArray<Xrace> races;
    public:
        //loads xml with race information
        unsigned load(const char*);
        void destroy(){races.destroy();}
        unsigned count(){return races.count();}
        Xrace * get(unsigned index){ if (index< races.count()) return &races[index]; return 0;}
    };




#endif //RACES_H

