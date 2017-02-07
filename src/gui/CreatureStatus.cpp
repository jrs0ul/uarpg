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

#include "CreatureStatus.h"
#include "Text.h"


  void CreatureStatus::change(unsigned int dx,unsigned int dy, const char* dname,
                                 int dface, unsigned pID){
        setpos(dx,dy);
        hp.max=0;
        hp.def=0;
        mp.max=0;
        mp.def=0;
        strcpy(name,dname);
        face=dface;
        showhpmp=false;
        enemyface=false;
        picID=pID;
    }
//---------------------------------
    void CreatureStatus::draw(PicsContainer& pics, int frame, int mark, int font, bool drawname){

        unsigned char frm;

        frm = (enemyface)? 1 : 0;

        pics.draw(frame, getX(), getY(), frm);

        if (showhpmp){
            pics.draw(-1, getX()+5, getY()+16, 0, false, 3, 50, 0,
                      COLOR(0.5f, 0.5f, 0.5f), COLOR(0.0f, 0.0f, 0.0f)); //hp holder
            if (mp.max > 0)
                pics.draw(-1, getX()+62, getY()+16, 0, false, 3, 50, 0,
                          COLOR(0.5f, 0.5f, 0.5f), COLOR(0.0f, 0.0f, 0.0f));//mp holder

            if (hp.def > 0) {
                int siz = (int)((hp.def/(hp.max/100.0f))*0.50); //hp bar
                pics.draw(-1, getX()+5, (getY()+66)-siz, 0, false, 3, siz,
                          0, COLOR(1.0f,0,0), COLOR(0.0f,0,0));
        }

        if (mp.def > 0){
            int siz = (int)((mp.def/(mp.max/100.0f))*0.50); //mp bar
            pics.draw(-1, getX()+62, (getY()+66)-siz, 0, false, 3, siz, 0,
                      COLOR(0,0,1.0f), COLOR(0.0f,0,0));
        }
    }

    if (drawname){
        WriteText(getX()+2, getY() + 2, pics, font, name,
                  0.8f, 0.8f, COLOR(0.0f, 0.0f, 0.0f, 1.0f));       //name
        WriteText(getX() + 1, getY() + 1, pics, font, name,
                  0.8f, 0.8f, COLOR(0.5f, 0.5f, 0.5f, 1.0f));
    }


    pics.draw(face,getX()+12, getY()+16,picID);

        if (dead)
            pics.draw(mark, getX()+25,getY()+30, 1);


        if ((poisoned)&&(!dead))
            pics.draw(mark, getX()+25,getY()+30, 0);


    }

