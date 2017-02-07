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

#ifndef STATS_GAIN_H
#define STATS_GAIN_H


#include "BasicUiControl.h"
#include "../Party.h"
#include "../TextureLoader.h"
/*
 * Shows a dialog window with information
 * how much XP, HP and other stuff party gained
 */


struct Stat{
    int oldEXP;
    int gainEXP;
    int oldLevel;
    int newLevel;
    int hpUp;
    int mpUp;
    int atckUp;
    int defUp;
    int agilUp;
    int magpUp;
    int sprtUp;
    int newspells;

   Stat(){
       oldEXP = 0;
       gainEXP = oldEXP;
       oldLevel = 0;
       newLevel = oldLevel;
       hpUp = mpUp = atckUp = defUp = agilUp = magpUp = sprtUp = 0;
       newspells = 0;

   }
};

class StatsGainWindow: public BasicControl{
    int _width;
    int _height;
    unsigned pressedkey;
    //0 - shows how many exp and shit gained
    //1 - merges with existing points and shows
    unsigned state;

    DArray<Stat> stats;

public:

    StatsGainWindow(){
        deactivate();
        _width = 400;
        _height = 330;
        pressedkey = 0;
        state = 0;
    }

    void setPos(int nx, int ny){
        setpos(nx, ny);
    }

    void draw(Party & party, PicsContainer& pics, int font, int frame);
    void getInput(unsigned key);

    int width(){return _width;}
    int height(){return _height;}

    void addStat(Stat& s){stats.add(s);}
    void destroy(){stats.destroy();}

};



#endif //STATS_GAIN_H

