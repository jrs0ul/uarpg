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


#ifndef _COMBAT_H
#define _COMBAT_H



#include "Vectors.h"
#include "Party.h"

//Some actions used to set different animations
enum CombatAction {NOP, ATACK, CAST, USE, DODGE};
enum FighterType {DUMMY, HERO, MONSTER};


    struct Action{

        CombatAction action;

        //oponent figter index in Squad::fighters
        unsigned oponent;
        //Item or spell which is must be used during animation
        unsigned itemID;

        Action(){

            oponent = 0;
            action = NOP;
            itemID = 0;
        }
    };

 //---------------------------------------
    class PartyActions{

        DArray<Action> acts;

    public:
        const Action *  const act(unsigned long index);
        void            addEmpty(unsigned _count);
        void            destroy(){acts.destroy();}
        unsigned long   count(){return acts.count();}
        void            set(unsigned long index, CombatAction _act,
                            unsigned oponent,
                            unsigned itemID = 0);
    };


//-----------------------------------------
    struct ActiveFighter{
        int partyIndex; //active fighter in party
        unsigned activeFighter; //active fighter in Squad::fighters
        bool isEnemy;
    };

    //==========================================
    struct CombatAnimationControl{
        bool CombatAnimationRun;
        //Current animation frame [0 .. MaxCombatFrame]
        unsigned CombatFrame;
        //Combat animation speed, frame count for one atack/action
        unsigned MaxCombatFrame;

        CombatAnimationControl(){
            MaxCombatFrame = 240;
        }

        int percent(){ return  (int)ceil(CombatFrame/(MaxCombatFrame/100.0f));}
        void reset() {CombatFrame = 0;}
    };


//----------------------------------------------
//Representation of single fighter as collection of data used for combat animation
class Fighter{

public:
    unsigned      nr;
    FighterType   type;
    int           agility;
    bool          escaped;
    int           hp;
    int           mp;

        //Current position of a fighter in arena
        Vector3D pos;
        //Starting position of a fighter
        Vector3D startPos;
        //Fighter movement direction + some speed
        Vector3D dir;
        unsigned frame;
        int idleTic;

        // Action veiksmas;
        Fighter(){
            idleTic = 0;
            nr = 0;
            type = HERO;
            agility = 0;
            startPos = Vector3D(0.0f, 0.0f, 0.0f);
            pos = startPos;
            dir = pos;
            frame = 0;
            escaped = false;
            hp = 0;
            mp = 0;
        }

        void init( unsigned nnr, FighterType ntip,
                   int nvik, int nhp, int nmp,
                   float nx, float ny,
                   unsigned nfrm, bool escape){
            nr = nnr;
            type = ntip;
            agility = nvik;
            hp = nhp;
            mp = nmp;
            startPos = Vector3D(nx, 0, ny);
            pos = startPos;
            frame = nfrm;
            escaped = escape;
        }

        void move(){
            pos = pos + dir;
        }

        void makeDir(Vector3D newpos, int steps);


    };



//========================================
//
//Representation of all fighters in arena
class Squad{
    DArray<Fighter> fighters;
public:
    Fighter *       get(unsigned long index);
    unsigned long   count(){return fighters.count();}
    void            build(Party& jparty, Party& eparty,
                          PartyActions& eaction,
                          PartyActions& paction);
    //finds a fighter index in squad
    //nr - fighter index in real party
    unsigned        findFighter(FighterType type, unsigned nr);
    void            colectHpMp(Party& jparty, Party& eparty);
    void            destroy(){fighters.destroy();}
};


//-----------------------------



#endif //_COMBAT_H

