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

#ifndef NPC_H
#define NPC_H

#include <cstring>
#include "Worldmap.h"
#include "TextureLoader.h"


//=============================
//The Object of Non Player Character


struct Pnt{
    int x;
    int y;

    Pnt(){
        x = 0;
        y = 0;
    }
};
//----------------
    struct NpcModel{
        SubsetCollection dud;
        char name[255];
    };


//-----------------
class NpcArray;
//----------------
//
class Npc {

    float _x, _y;
    bool walk;

    int futureposx, futureposy;
    //npc inner clock, used for animation
    int clock;

    unsigned defframe;
    unsigned _ID;

public:

    Pnt pdir; //direction
    bool use_ai; //does npc uses ai

    int stepcount;
    unsigned job;   //current job
    char spritename[255];
    //3d model name
    char modelname[255];
    //3d models texture
    char texturename[255];
    //npc speech text or some other script
    char speech[200];

    //index in model array
    unsigned modelIndex;
    unsigned textureIndex;


    float x() {return _x;}
    float y() {return _y;}
    void setX(float nx) {_x = nx;}
    void setY(float ny) {_y = ny;}
    void stop(){if (walk) walk = false;}
    void go(){if (!walk) walk = true;}
    bool isWalk(){return walk;}

    Npc();

    void     init(float dx, float dy,
                  unsigned char djob,
                  unsigned Nr,
                  const char* dspeech,
                  const char* sname,
                  const char* modname,
                  const char* texname,
                  unsigned dframe = 0);
    void     movexy(float dx, float dy);

    //draws 2d sprite in 2d map
    void     draw(PicsContainer& pic, int spriteIndex,
                  int scrposx, int scrposy,
                  int scrollnumx, int scrollnumy);
    //draws 3d model in 3d world
    void     draw3D(unsigned textureID,
                    SubsetCollection& model,
                    bool useVBO = true);

    //obstacle towards
    unsigned obstacle(Map &map,NpcArray& dudes);
    //increases clock value
    void     inctimer();
    //changes npc direction in AI
    bool     redir(unsigned char dir, Map& map,
                   NpcArray& dudes, int tilesize = 32);
    //sets npc clock to some value
    void     setClock(int c){clock = c;}
    int      getClock(){return clock;}
    void     setFrame(unsigned frame){defframe = frame;}
    unsigned defaultFrame(){return defframe;}
    //only changes direction for animation
    void     redir(int x, int y){pdir.x = x; pdir.y = y;}
    unsigned ID(){return _ID;}
};

//=============================================
class NpcArray{
    DArray<Npc> npcs;
    DArray<NpcModel> models;
    PicsContainer textures;

    void loadModels(const char* basepath,
                    bool useVBO = true);
    void loadTextures(const char* basepath);
public:
    Npc*          getArray();
    unsigned long count(){return npcs.count();}
    Npc*          getNpc(unsigned long index);
    //loads all npcs from xml file
    bool          load(const char* filename,
                       const char* basepath,
                       bool useVBO = true);
    //loads binary data from savegame
    bool          loadBin(FILE * f, const char * basepath);
    //destroys npc which has _ID
    bool          kill(unsigned _ID);
    //adds new npc
    void          add(Npc& newGuy,
                      const char * basepath,
                      bool loadstuff = false,
                      bool useVBO = true);
    void          destroy();
    //saves binary data of all npcs
    bool          saveBin(FILE * f);
    NpcModel*     getModel(unsigned index){return &models[index];}
    unsigned long modelCount(){return models.count();}
    unsigned      getTextureName(unsigned index);

};
//=============================================
void translatedir(unsigned dir,int& x, int& y);


#endif //NPC_H


