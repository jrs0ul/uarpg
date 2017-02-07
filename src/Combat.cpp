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
#include "Combat.h"



    const Action * const PartyActions::act(unsigned long index){
        if (index < acts.count()){
            return &acts[index];
        }
        return 0;

    }
//------------------------------------------------

    void PartyActions::addEmpty(unsigned _count){
        Action a;
        for (unsigned i = 0; i < _count; i++){
            acts.add(a);
        }
    }
//------------------------------------------------
    void    PartyActions::set(unsigned long index, CombatAction _act,
                              unsigned oponent,
                              unsigned itemID){

        if (index < acts.count()){
            acts[index].action = _act;
            acts[index].oponent = oponent;
            acts[index].itemID = itemID;
        }
        
    }

//-------------------------------------------------

    void Fighter::makeDir(Vector3D newpos, int steps){

        dir = newpos - pos;

        float v = dir.length() /steps;

        dir.normalize();

        dir = Vector3D(dir.x() * v, dir.y() * v, dir.z() * v);

    }


//================================================================

    Fighter * Squad::get(unsigned long index){
        if (index < fighters.count())
            return &fighters[index];
        return 0;
    }
    //-------------------------------------------------

    void Squad::build(Party& jparty, Party& eparty,
                      PartyActions& eaction,
                      PartyActions& paction){


        fighters.destroy();

        //heroes
        float sumRadius = 0.0f;
        for (unsigned long i = 0; i < jparty.count(); i++){
            sumRadius += jparty.member(i)->modelRadius;
        }
        float pos = (sumRadius/2.0f);
        for (unsigned long i = 0; i < jparty.count(); i++){

            //it's because models scaled down by 0.5
            pos -= (jparty.member(i)->modelRadius * 0.5f);
            Fighter newhero;

            if (!jparty.member(i)->escaped()){

                newhero.init(i, HERO,
                             jparty.member(i)->agil,
                             jparty.member(i)->hp.def,
                             0, 
                             4, pos,
                             0, false);
                newhero.dir = Vector3D(0, 0, 0);
                fighters.add(newhero);
                
            }

            pos -= (jparty.member(i)->modelRadius * 0.5f);
        }
        //enemy
        sumRadius = 0.0f;
        for (unsigned long i = 0; i < eparty.count(); i++){
            sumRadius += eparty.member(i)->modelRadius;
        }
        pos = (sumRadius/2.0f);
        printf("sumradius: %2.3f\n", sumRadius);
        for (unsigned long i = 0; i < eparty.count(); i++){

            //it's because models scaled down by 0.5
            pos -= (eparty.member(i)->modelRadius * 0.5f);
            if (!eparty.member(i)->escaped()){
                Fighter newmonster;

                newmonster.init(i, MONSTER,
                                eparty.member(i)->agil,
                                eparty.member(i)->hp.def,
                                0, 
                                -4, pos,
                                1, false);

                newmonster.dir = Vector3D(0, 0, 0);
                fighters.add(newmonster);
            }
            pos -= (eparty.member(i)->modelRadius * 0.5f);
        }

        for (unsigned a = 0; a < fighters.count(); a++) {
            for (unsigned i = 0; i < fighters.count() - 1; i++) {
                if (fighters[i].agility < fighters[i + 1].agility)  {
                    Fighter k1 = fighters[i];
                    Fighter k2 = fighters[i + 1];
                    fighters[i] = k2;
                    fighters[i + 1] = k1;
                }
            }
        }
       
    }
//--------------------------------------------------------------------
unsigned Squad::findFighter(FighterType tipas, unsigned nr){
  unsigned  i = 0;
    while (((fighters[i].nr != nr)||(fighters[i].type != tipas)) && (i< fighters.count()))
     i++;

     return i;
}
//--------------------------------

void Squad::colectHpMp(Party& jparty, Party& eparty){

    for (unsigned i = 0; i < fighters.count(); i++){
        switch(fighters[i].type){
            case HERO:{
                fighters[i].hp = jparty.member(fighters[i].nr)->hp.def;
                fighters[i].mp = jparty.member(fighters[i].nr)->mp.def;
            }
            break;
            default:{
                fighters[i].hp = eparty.member(fighters[i].nr)->hp.def;
                fighters[i].mp = eparty.member(fighters[i].nr)->mp.def;
            }
            break;
        }
    }
 }
//--------------------------------------------


