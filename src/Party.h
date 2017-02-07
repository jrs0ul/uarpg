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

#ifndef PARTY_H
#define PARTY_H


#include "Creature.h"
#include "Worldmap.h"
#include "npc.h"
#include "Vectors.h"
#include "DArray.h"
#include "models/rm2.h"
#include "TextureLoader.h"



struct pnt{
    int x;
    int y;
};


struct FighterModel{
        SubsetCollection fighter;
        char name[255];
};

//------------------
//Party of characters
class Party{

        DArray<Creature> members;

    public:

        //fighter meshes, used in combat so far
        DArray<FighterModel> models;
        PicsContainer modelTextures;

        int steps;     //zingsniai
        int x,y;

        bool isBoss;

        DArray<Vector3D> membersxy;  //parcio memberiu koordai
        DArray<unsigned> mdir;    //parcio memberiu dirai

        //just alive party members
        unsigned  alive();
        //alive party members currently in combat field
        unsigned  present();
        //resets escaped party members
        void      resetEscaped();
        //return party members count
        unsigned  count(){return members.count();}
        //gets party member pointer from array
        Creature* member(unsigned index);
        void      destroy();

        //Kills party member, decreases alive member count
        void      killMember(unsigned index);

        //resurects member
        void      resurectMember(unsigned index, int percent);

        //adds new fighter to party, if loadmodel = true 
        //model loaded to models array, using Creature::modelfile string
        void      addMember(Creature& noob, const char* basepath,
                            bool loadmodel = true);
        void      removeMember(unsigned index, bool removemodel = true);

        //Restores HP and MP of all members
        void      rest();

        //gal situos metodus reiks mest prie npc ?
        //cia reikalingi kad gyvatele partis eitu

        //incializuojamas memberiu kordinaciu masyvas
        void membersxyInit(int dx,int dy);
        //perstumia kordinaciu masyva
        void membersxyPush(int dx,int dy);
        //isvalo dirus
        void mdirInit();

        void destroyModels(){

            for (unsigned long i = 0; i < models.count(); i++)
                models[i].fighter.destroy();
            models.destroy();
        }

        Party(){
            isBoss = false;
        }

};

//===============================

#endif //PARTY_H

