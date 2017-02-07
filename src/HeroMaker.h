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
#ifndef HERO_MAKER_H
#define HERO_MAKER_H



    class HeroMaker{ //pagalbinis objektas kuriant herojus
    public:
        unsigned heroIndex; //HeroToCreate
        bool raceSelected;
        bool sexSelected;
        bool classSelected;
        bool dicesRolled;
        bool nameWriten;
        bool nexthero;
        bool faceSelected;
        void reset();
    };

    //------------------------------------
    void HeroMaker::reset(){

        raceSelected  = false;
        sexSelected   = false;
        classSelected = false;
        nameWriten    = false;
        dicesRolled   = false;
        nexthero      = false;
        faceSelected  = false;
    }


#endif //HERO_MAKER_H
