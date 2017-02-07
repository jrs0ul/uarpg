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

#include "Party.h"



    void Party::destroy(){

        for (unsigned i = 0; i < members.count(); i++){
            members[i].destroy();
        }

        members.destroy();
        destroyModels();
        membersxy.destroy();
        mdir.destroy();
        modelTextures.destroy();
    }
//------------------------------------------
    unsigned Party::alive(){
        unsigned result = 0;
        for (unsigned i = 0; i < members.count(); i++)
            if (members[i].state != DEAD)
                result++;
        return result;
    }
//------------------------------------------
    unsigned Party::present(){
        unsigned result = 0;
        for (unsigned i = 0; i < members.count(); i++)
            if ((members[i].state != DEAD) && (!members[i].escaped()))
                result++;
        return result;
    }
//------------------------------------------
    void Party::resetEscaped(){
        for (unsigned i = 0; i < members.count(); i++)
            members[i].resetEscape();
    }

//------------------------------------------
Creature* Party::member(unsigned index){

    if (index < members.count()){

        return &members[index];
    }
    return 0;
}
//--------------------------------------

    void Party::killMember(unsigned index){
        if ( index < members.count()) {
            members[index].kill();

        }
    }
//----------------------------
    void Party::resurectMember(unsigned index, int percent){
        if (index < members.count()){
            members[index].resurect(percent);

        }
    }
//-----------------------------
    void Party::rest(){

        for (unsigned i = 0; i < members.count(); i++)
        if (members[i].state != DEAD){
            members[i].hp.def = members[i].hp.max;
            members[i].mp.def = members[i].mp.max;
        }
    }
//------------------------------
   void Party::addMember(Creature& noob, const char * basepath,
                         bool loadmodel){

        members.add(noob);
        Vector3D v(0,0,0);
        membersxy.add(v);
        unsigned a = 0;
        mdir.add(a);

        if (loadmodel){
            char buf[255];
            FighterModel nmd;
            sprintf(buf, "%smodels/%s.rm2", basepath, noob.modelfile);
            if (!nmd.fighter.load(buf)){
                sprintf(buf, "base/models/%s.rm2", noob.modelfile);
                if (!nmd.fighter.load(buf)){
                    puts("epic fail!");
                }
            }

            strcpy(nmd.name, noob.modelfile);
            models.add(nmd);

            sprintf(buf, "%s.tga", noob.texturefile);
            modelTextures.loadFile(buf, count() - 1, 512, 512, basepath, 1);
        }
   }
//-------------------------------

    void Party::removeMember(unsigned index, bool removemodel){
        if (index < members.count()){
            members.remove(index);
            membersxy.remove(index);
            mdir.remove(index);
            if (removemodel){
                models.remove(index);
                modelTextures.remove(index);
            }
        }

    }
//-----------------------------
void Party::membersxyInit(int dx, int dy){

    for (unsigned i = 0; i < membersxy.count(); i++){
        membersxy[i].set((float)dx,(float)dy,0);
        printf("init %d: %d %d\n", i, dx, dy);
    }
}
//---------------------------------

void Party::membersxyPush(int dx,int dy){

    for (unsigned i = membersxy.count() - 1; i > 1; i--){
        membersxy[i].set(membersxy[i-1].x(), membersxy[i-1].y(), 0);
    }
    membersxy[1].set(dx, dy, 0);
}
//---------------------------------
    void Party::mdirInit(){

        for (unsigned i = 0; i < mdir.count(); i++)
            mdir[i] = 0;

    }



//================================




