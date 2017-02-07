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
#ifndef MONSTERGROUP_H
#define MONSTERGROUP_H

#include "DArray.h"
#include "Strategy.h"
#include "Xml.h"
#include <cstring>


//Item possed by a monster
struct MonsterItem{
    unsigned index;    //Item index
    unsigned mincount; //minimal count of this item
    unsigned maxcount; //maximal count of it
};

//===============================================
// An enemy monster
struct Monster{
    unsigned minlevel;          //minimal experiance level
    unsigned maxlevel;          //maximal experiance level
    float    expValue;          //how much experiance of this monster 
                                //is valuable
    unsigned race;              //monster race
    unsigned sex;               //male/female or something else
    unsigned klass;             //monster class
    float    radius;            //occupied space radius
    char     model[255];        //monster 3D model name
    char     modeltexture[255]; //enemy texture
    char     name[255];         //enemy name

    Strategy ai;
    DArray<MonsterItem> inventory; //monster possesions
    int      rightHand;            //Item equiped on right hand
    int      leftHand;             //Item equiped on left hand
    int      arrmor;               //armor weared by monster


    Monster(){
        expValue = 1.0f;
        rightHand = -1;
        leftHand = -1;
        arrmor = -1;
    }

    void init(unsigned minlev, unsigned mxnlev,
              float expVal,
              unsigned nrace, unsigned nsex, unsigned nklass,
              float rad,
              const char* nmodel, const char* mtexture,
              const char * mname){
        minlevel = minlev;
        maxlevel = mxnlev;
        race = nrace;
        sex = nsex;
        klass = nklass;
        expValue = expVal;
        radius = rad;
        strncpy(model, nmodel, 255);
        strncpy(modeltexture, mtexture, 255);
        strncpy(name, mname, 255);
    }
    //-------------
    void destroy(){
        inventory.destroy();
        ai.destroy();
    }
};

//============================================
//Group of monsters
struct MonsterGroup{

    //different monsters
    DArray<Monster> monsters;

    //random monsters count
    int mcount;
    //is monster group a boss ?
    bool isBoss;
    char arenaName[255]; //special arena for monsters

    MonsterGroup(){
        mcount = 0;
        isBoss = false;
    }

    void destroy();
    void add(unsigned minlev, unsigned mxnlev,
             float expval,
             unsigned nrace, unsigned nsex, unsigned nklass,
             float nrad,
             const char* nmodel, const char* mtexture, const char * mname){
        Monster newmon;
        newmon.init(minlev, mxnlev, expval, nrace,
                    nsex, nklass, nrad, nmodel, mtexture, mname);
        monsters.add(newmon);
    }
};

//==============================================
//List of monster groups. Single for each map.
class MGroupList{

    DArray<MonsterGroup> groups;

    void LoadStrategy(XmlNode * monster, Monster* m);
public:

    MonsterGroup * get(unsigned long index);
    bool           load(const char* path);
    void           destroy();
    unsigned long  count(){return groups.count();}
};


#endif //MONSTERGROUP_H

