
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
#include <cstdio>
#include "npc.h"
#include "Utils.h"
#include "Xml.h"

Npc::Npc(){
    clock = 0;
    job = 0;
    use_ai = false;
    walk = false;
    _ID = 255;
    futureposx = 0;
    futureposy = 0;
    strncpy(modelname, "", 255);
    strncpy(texturename, "", 255);
    strncpy(spritename, "", 255);
}

//------------------------------------
void Npc::init(float dx, float dy,
               unsigned char djob,
               unsigned Nr,
               const char* dspeech, 
               const char* sname,
               const char* modname,
               const char* texname,
               unsigned dframe){
    _x = dx; _y = dy;


    //does npc depends on random AI ?
    if (djob == 4)
        use_ai = true;
    else
        use_ai = false;
    job=djob;
    _ID = Nr;
    clock = 0;

    strncpy(speech, dspeech, 200);
    strncpy(spritename, sname, 255);
    strncpy(modelname, modname, 255);
    strncpy(texturename, texname, 255);
    pdir.x = 0;
    pdir.y = 0;
    defframe = dframe;
    walk=false;
    stepcount=1;
 }

//--------------------------

void Npc::movexy(float dx, float dy){

    if (stepcount==1)
        walk=false;
    else stepcount--;

    _x = dx ;
    _y = dy ;
}
//-------------------------
void Npc::draw(PicsContainer& pic, int spriteindex,
                int scrposx, int scrposy,
                int scrollnumx, int scrollnumy){



    unsigned char frm;

    if ((pdir.x != 0)||(pdir.y != 0)){

        if (pdir.x){
            if (pdir.x == 1)
                defframe = 6;
            else
                defframe = 9;
        }

        if (pdir.y){
            if (pdir.y == 1)
                defframe = 0;
            else
                defframe = 3;
        }

        if ((clock < 10) && (clock > 0)){
            frm = 0;
        }
        else{
            if ((clock < 32) && (clock > 20))
                frm=2;
            else
                frm=1;
        }

    }
    else

        frm=1;

    if (defframe > 9 )
        defframe = 0;
    pic.draw(spriteindex,
             scrposx+_x,
             scrposy+_y,
             defframe + frm);


}

//-----------------------------------------------
void Npc::draw3D(unsigned textureID, 
                 SubsetCollection& model,
                 bool useVBO){

    float angle = 180.0f;
        if (pdir.x){
            if (pdir.x == 1)
                angle = -90.0f;
            else
                angle = 90.0f;
    }

    if (pdir.y){
        if (pdir.y == 1)
            angle = 180.0f;
        else
            angle = 0.0f;
    }

    glPushMatrix();
    glTranslatef(x()/4.0f + 4.0f, 0.0f, y()/4.0f + 4.0f);
    glRotatef(angle, 0 , 1.0f, 0);


    glBindTexture(GL_TEXTURE_2D, textureID);

    glColor3f(1.0f,1.0f,1.0f);
    model.draw(0, defframe + clock/3);


    glPopMatrix();
}
//------------------------------------------------

unsigned Npc::obstacle(Map& map, NpcArray& dudes){


    unsigned char newx=(unsigned char)(_x+pdir.x);
    unsigned char newy=(unsigned char)(_y+pdir.y);

    unsigned i=0;
    while (( i < dudes.count())&&((dudes.getNpc(i)->x() != newx)||
            (dudes.getNpc(i)->x() != newy)))
        i++;
    if (i < dudes.count())
        return dudes.getNpc(i)->_ID;

    return 0;

}
//-----------------------
    void Npc::inctimer(){

        clock++;
        movexy(_x + pdir.x * 1.0f,
               _y + pdir.y * 1.0f);

        if (clock == 32){
            clock = 0;
        }
    }
//------------------------
bool Npc::redir(unsigned char dir, Map& map,
                NpcArray& dudes, int tilesize){
    int ty = 0;
    int tx = 0;

    translatedir(dir, tx, ty);
    int newx = (int)_x + tx * tilesize;
    int newy = (int)_y + ty * tilesize;

    pdir.x = tx;
    pdir.y = ty;

    int newPosInGridX = newx / tilesize;
    int newPosInGridY = newy / tilesize;


    unsigned z = 0;
    bool eitigalima = true;

    while ((z < dudes.count()) &&
            (
             ((dudes.getNpc(z)->x() / tilesize != newPosInGridX)
             ||(dudes.getNpc(z)->y() / tilesize != newPosInGridY)) &&
             ((dudes.getNpc(z)->futureposx != newPosInGridX)
             ||(dudes.getNpc(z)->futureposy != newPosInGridY))
            )
           )

        z++;

    if (z < dudes.count()){
        if ((job != 255) || (dudes.getNpc(z)->job != 255)){

            eitigalima = false;
        }
        else{

            eitigalima = true;
        }
    }
    else
        eitigalima = true;

    //printf("%d map w: %d\n",(newx/32),map.width);
    if (
        ( (unsigned)newPosInGridX > map.width() - 1) || ( newPosInGridX < 0) ||
        ( (unsigned)newPosInGridY > map.height() - 1) || ( newPosInGridY < 0) ||
        (!eitigalima)
    )
        return false;
    else
        if (!map.isWalkable(newPosInGridX, newPosInGridY))
            return false;


     if ((stepcount > 0) && (!walk)){
         walk = true;
         futureposx = newPosInGridX;
         futureposy = newPosInGridY;
     }

     if (job == 4){ //npc stop by the player

        unsigned z = 0;

        while ((z < dudes.count()) &&
                (
                 ((!CirclesColide(dudes.getNpc(z)->x(),
                                  dudes.getNpc(z)->y(),
                                  tilesize / 2, _x, _y, tilesize)) ||
                  (dudes.getNpc(z)->job != 255))
                )
               )
        z++;
        if (z < dudes.count()){

            walk = false;
            pdir.x = 0;
            pdir.y = 0;
            futureposx = -1;
            futureposy = -1;
        }
    }

    return true;
}

//==================================
//            NPC ARRAY
//==================================
bool NpcArray::load(const char* filename, const char* basepath,
                    bool useVBO){

        Xml data;

        int dx = 0;
        int dy = 0;
        int job = 0;
        char speech[255] = {0};
        char spritename[255] = {0};
        char modelname[255] = {0};
        char texturename[255] = {0};
        unsigned dframe = 0;

        if (!data.load(filename))
            return false;
        
        XmlNode * _npcs = 0;
        _npcs = data.root.getNode(L"Npcs");
        if (_npcs){
            for (unsigned long i = 0; i < _npcs->childrenCount(); i++){
                XmlNode * npc = 0;
                npc = _npcs->getNode(i);
                if (npc){
                    char buf[255];
                    wcstombs(buf, npc->getAttribute(0)->getValue(), 255);
                    job = atoi(buf);
                    XmlNode * pos = 0;
                    pos = npc->getNode(L"Pos");
                    if (pos){
                        wcstombs(buf, pos->getAttribute(0)->getValue(), 255);
                        dx = atoi(buf);
                        wcstombs(buf, pos->getAttribute(1)->getValue(), 255);
                        dy = atoi(buf);
                    }
                    XmlNode * sprite = 0;
                    sprite = npc->getNode(L"Sprite");
                    if (sprite){
                        wcstombs(spritename, sprite->getValue(), 255);
                    }

                    XmlNode * model = 0;
                    model = npc->getNode(L"Model");
                    if (model){
                        wcstombs(modelname, model->getValue(), 255);
                    }
                    XmlNode * texture = 0;
                    texture = npc->getNode(L"Texture");
                    if (texture){
                        wcstombs(texturename, texture->getValue(), 255);
                    }

                    XmlNode * _dframe = 0;
                    _dframe = npc->getNode(L"DefaultFrame");
                    if (_dframe){
                        char buf[255];
                        wcstombs(buf, _dframe->getValue(), 255);
                        dframe = (unsigned)atoi(buf);
                    }

                    XmlNode * script = 0;
                    script = npc->getNode(L"Script");
                    if (script){
                        convertLT(script->getValue(), speech, 255);
                    }

                    Npc tmp;
                    tmp.init(dx * 32, dy * 32,job,
                             i + 1, speech,
                             spritename,
                             modelname,
                             texturename,
                             dframe);

                    npcs.add(tmp);
                }
            }
        }

        data.destroy();
        //------------------------------
        loadModels(basepath, useVBO);
        loadTextures(basepath);
        return true;

}
//-----------------------------------------------
Npc * NpcArray::getArray(){
    if (npcs.count() > 0)
        return (Npc*)npcs.getData();
    return 0;
}
//-----------------------------------------------
Npc* NpcArray::getNpc(unsigned long index){
    if (index < npcs.count())
        return &npcs[index];
    return 0;
}
//-----------------------------------------------
void NpcArray::loadModels(const char* basepath, bool useVBO){

    for (unsigned i = 0; i < npcs.count(); i++){
        if (npcs[i].job != 255){
            if (strlen(npcs[i].modelname) > 1){
                bool alreadyHasIt = false;
                for (unsigned a = 0; a < models.count(); a++){
                    if (strcmp(npcs[i].modelname, models[a].name) == 0){
                        alreadyHasIt = true;
                        npcs[i].modelIndex = a;
                        puts("has it");
                        break;
                    }
                }
                if (!alreadyHasIt){
                    NpcModel newmodel;
                    char buf[255];
                    sprintf(buf, "%smodels/%s", basepath, npcs[i].modelname);
                    puts(buf);
                    if (newmodel.dud.load(buf, useVBO)){
                        strncpy(newmodel.name, npcs[i].modelname, 255);
                        npcs[i].modelIndex = models.count();
                        models.add(newmodel);
                    }
                    else{
                        sprintf(buf, "base/models/%s", npcs[i].modelname);
                        if (!newmodel.dud.load(buf, useVBO)){
                            printf("failed to load %s\n", buf);
                            npcs[i].modelIndex = 666;
                        }
                        else{
                            strncpy(newmodel.name, npcs[i].modelname, 255);
                            npcs[i].modelIndex = models.count();
                            models.add(newmodel);
                            printf("nps index %u, model index %u\n", i, npcs[i].modelIndex);
                        }
                    }
                }
            }
        
        }
    }
}

//----------------------------------------------
void NpcArray::loadTextures(const char* basepath){

    for (unsigned i = 0; i < npcs.count(); i++){

        int texIndex = textures.count();

        if (npcs[i].job != 255){
            if (strlen(npcs[i].texturename)){

                puts(npcs[i].texturename);
                int res = textures.findByName(npcs[i].texturename);
                if (res != -1)
                    npcs[i].textureIndex = (unsigned)res;
                else{
                    if (!textures.loadFile(npcs[i].texturename,
                                           texIndex, 512, 512, basepath, 1))
                        printf("failed to load %s\n",
                               npcs[i].texturename);
                    printf("loaded %s\n", npcs[i].texturename);
                    npcs[i].textureIndex = (unsigned)texIndex;
                }
                printf("npc: %u tex index: %u\n", i, npcs[i].textureIndex);
                
            }
        }
    }
}
//-----------------------------------------------
bool NpcArray::saveBin(FILE * f){

        if (!f)
            return false;

        unsigned long _count_ = count();
        fwrite(&_count_, sizeof(unsigned long), 1, f);

        for (unsigned long i = 0; i < count(); i++){

            //bool fail = false;
            int _x = (int)(npcs[i].x()/32);
            int _y = (int)(npcs[i].y()/32);
            int job = 255;
            if (npcs[i].ID() != 255)
                job = npcs[i].job;
            
            fwrite(&_x, sizeof(int), 1, f);
            fwrite(&_y, sizeof(int), 1, f);
            fwrite(&job, sizeof(int), 1, f);

            int spritenameLen = (int)strlen(npcs[i].spritename);
            fwrite(&spritenameLen, sizeof(int), 1, f);
            if (spritenameLen)
                fwrite(npcs[i].spritename, sizeof(char), spritenameLen, f);

            int modelnameLen = (int)strlen(npcs[i].modelname);
            fwrite(&modelnameLen, sizeof(int), 1, f);
            if (modelnameLen)
                fwrite(npcs[i].modelname, sizeof(char), modelnameLen, f);

            int texturenameLen = (int)strlen(npcs[i].texturename);
            fwrite(&texturenameLen, sizeof(int), 1, f);
            if (texturenameLen)
                fwrite(npcs[i].texturename, sizeof(char), texturenameLen, f);

            int scriptLen = (int)strlen(npcs[i].speech);
            fwrite(&scriptLen, sizeof(int), 1, f);
            if (scriptLen)
                fwrite(npcs[i].speech, sizeof(char), scriptLen, f);

            unsigned dframe = npcs[i].defaultFrame();
            fwrite(&dframe, sizeof(unsigned), 1, f);


        }
        
        return true;

}
//---------------------------------------
bool NpcArray::loadBin(FILE * f, const char * basepath){

        if (!f)
            return false;
        unsigned long _count_ = 0;
        if (!fread(&_count_, sizeof(unsigned long), 1, f))
            return false;
        printf("count %lu\n", _count_);
        int dx, dy, job;
        char speech[255];
        char spritename[255];
        char modelname[255];
        char texturename[255];

        for (unsigned long i = 0; i < _count_; i++){

            Npc newone;
            if (!fread(&dx, sizeof(int), 1, f))
                return false;
            if (!fread(&dy, sizeof(int), 1, f))
                return false;
            if (!fread(&job, sizeof(int), 1, f))
                return false;

            int spriteLen = 0;
            if (!fread(&spriteLen, sizeof(int), 1, f))
                return false;
            if (spriteLen){
                if (!fread(&spritename, sizeof(char), spriteLen, f))
                    return false;
                spritename[spriteLen] = 0;
            }

            int modelLen = 0;
            if (!fread(&modelLen, sizeof(int), 1, f))
                return false;
            if (modelLen){
                if (!fread(&modelname, sizeof(char), modelLen, f))
                    return false;
                modelname[modelLen] = 0;
            }

            int textureLen = 0;
            if (!fread(&textureLen, sizeof(int), 1, f))
                return false;
            if (textureLen){
                if (!fread(&texturename, sizeof(char), textureLen, f))
                    return false;
                texturename[textureLen] = 0;
            }




            int speechlen = 0;
            if (!fread(&speechlen, sizeof(int), 1, f))
                return false;
            if (speechlen){
                if (!fread(&speech, sizeof(char), speechlen, f))
                    return false;
                speech[speechlen] = 0;
            }

            unsigned dframe = 0;
            if (!fread(&dframe, sizeof(unsigned), 1, f))
                return false;

            newone.init(dx*32, dy*32, job, i+1, speech, 
                        spritename, modelname, texturename,
                        dframe);
            npcs.add(newone);
        }//for loop

        loadModels(basepath);
        loadTextures(basepath);
        
        return true;
}
//-------------------------------
    bool NpcArray::kill(unsigned kID){

        if (npcs.count()){

            unsigned _ID = 0;
            while ((npcs[_ID].ID() != kID)&&(_ID < npcs.count()))
                _ID++;

            if (_ID < npcs.count()){

                printf("memb index: %u\n", _ID);
                npcs.remove(_ID);
                return true;
            }
        }
        return false;
    }
//----------------------------------
void NpcArray::add(Npc& newGuy, const char* basepath,
                   bool loadstuff, bool useVBO){

    npcs.add(newGuy);
    if (loadstuff){
        loadModels(basepath, useVBO);
        loadTextures(basepath);
    }

}
//-----------------------------------
unsigned NpcArray::getTextureName(unsigned index){
    if (index > npcs.count())
        return 0;
    unsigned texIndex = npcs[index].textureIndex;
    return textures.getname(texIndex);

}
//-----------------------------------
void NpcArray::destroy(){

    npcs.destroy();
    for (unsigned i = 0; i < models.count(); i++){
        models[i].dud.destroy();
    }
    models.destroy();
    textures.destroy();
}

//---------------------------
void translatedir(unsigned dir, int& x, int& y){
    switch (dir){
        case 1:{  //down
            x = 0; y = 1;
        }
        break;
        case 2:{   //up
            x = 0; y = -1;
        }
        break;
        case 3:{
            x = 1; y = 0;
        }
        break;
        case 4:{
            x = -1; y = 0;
        }
        break;
        default:{
            x = 0; y = 0;
        }
    }
}

