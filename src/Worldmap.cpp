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
#include "Worldmap.h"
#include "Utils.h"
#include <cstdio>
#include <cstring>


Map::Map(){
    _width = _height = 0;
    map = 0;
    isMap3D = false;
    tilesize = 32;
    strcpy(name,"");
    strcpy(tileset,"");
    strcpy(music, "");
    camPitch = 0.77f;
    camHeight = 20.0f;
    camDistance = 20.0f;
}
//---------------------------------------
void Map::setMap3D(bool useVBO){
    alloc3DMap(useVBO);
    isMap3D = true;
}
//---------------------------------------
void Map::setMap2D(){
    destroy3DMap();
    isMap3D = false;
}

//---------------------------------------
void Map::alloc3DMap(bool useVBO){

    destroy3DMap();
    float * vertexData = 0;
    int vsize = (_height * _width + 1) * (12 + 8 + 16);
    if (vsize){
        vertexData = (float*)malloc (vsize * sizeof(float));
        vo.init(vertexData, VPOSITION|VTEXCOORD0|VCOLOR,
                (_height * _width + 1)*4, true, useVBO);

        free(vertexData);
    }
}
//---------------------------------------
void Map::destroy3DMap(){
    vo.destroy();
}
//---------------------------------------
Asset * Map::getAsset(unsigned long index){
    if (index < assets.count())
        return &assets[index];
    return 0;
}
//----------------------------------------
void Map::draw3DTiles(Vector3D frustumCenter,
                        float frustumRadius,
                        SubsetCollection& models,
                        PicsContainer& pics,
                        unsigned picindex,
                        unsigned layer,
                        bool useVBO,
                        bool ShowCollision){


    float tw = 8.0f;
    float htw = 4.0f;

    glDisable(GL_LIGHTING);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, pics.getname(picindex));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

      
    if (map){
        for (unsigned a = 0; a < _height; a++){
            for (unsigned i = 0; i < _width; i++){

                if (map[a][i].Layers[layer].frames[0]){

                    glPopMatrix();
                    glPushMatrix();


                    float x = tw * (i + 1) - htw;
                    float z = tw * (a + 1) - htw;

                    /*if (CollisionSphere_Sphere(frustumCenter,
                                               frustumRadius,
                                               Vector3D(x, 0.0f, z),
                                               5.0f
                                              )
                        ){*/

                        glTranslatef(x, 0.0f, z);

                        unsigned frame = map[a][i].Layers[layer].tilenum;
                        if (frame >= map[a][i].Layers[layer].count)
                            frame = 0;

                        unsigned tile = map[a][i].Layers[layer].frames[frame] - 30;

                        if (ShowCollision){
                            if ((!map[a][i].Layers[0].walkable) && (ShowCollision))
                                glColor3f(1.0f,0.0f,0.0f);
                            else
                                glColor3f(1.0f,1.0f,1.0f);
                        }
                        models.draw(tile, 0);
                   // } //frustum collision check

                }
            }
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glEnable(GL_LIGHTING);

    glPopMatrix();

}

//---------------------------------------
Vector3D Map::calcUVS(PicData * p, unsigned long x, unsigned long y,
                 unsigned layer){

    int frame = 0;

    unsigned frm = map[y][x].Layers[layer].tilenum;
    if (frm >= map[y][x].Layers[layer].count)
        frm = 0;
    frame = map[y][x].Layers[layer].frames[frm] - 30;
                    
    int hf = frame/p->hframes;

    int startx =  (frame-p->hframes*hf) * p->twidth;
    int starty = hf * p->theight;

    float halfPixelX = ((1.0f / p->width) / 2.0f);
    float halfPixelY = ((1.0f / p->height) / 2.0f);
    float height = p->height * 1.0f;
    float width = p->width * 1.0f;

    float u1 = (startx * 1.0f) / width + halfPixelX;
    float u2 = (((startx + p->twidth) * 1.0f) / width) - halfPixelX;
    float v1 = ((( p->height - starty) * 1.0f ) / height) - halfPixelY;
    float v2 = ((( p->height - starty - p->theight ) * 1.0f) / height) + halfPixelY;

    return Vector3D(u1, u2, v1, v2);
}
//------------------------------------------
void Map::addAsset(Asset& a){

    assets.add(a);
}
//------------------------------------------
void Map::moveAsset(unsigned long index, Vector3D shift){
    if (index > assets.count())
        return;
    assets[index].position = assets[index].position + shift;
}
//------------------------------------------
void Map::scaleAsset(unsigned long index, Vector3D scale){
    if (index > assets.count())
        return;
    Asset* a = &assets[index];
    a->scale = Vector3D((a->scale.v[0]) * scale.v[0],
                        (a->scale.v[1]) * scale.v[1],
                        (a->scale.v[2]) * scale.v[2]);
    //printf("%f %f %f\n", a->scale.v[0], a->scale.v[1], a->scale.v[2]);
}
//------------------------------------------
void Map::rotateAsset(unsigned long index, Vector3D rotation){
    const float pi2 = 6.28318531f;
    if (index > assets.count())
        return;

    Asset * a = &assets[index];
    a->rotations = a->rotations + rotation;
    if (fabs(a->rotations.v[0]) > pi2)
        a->rotations.v[0] = 0;
    if (fabs(a->rotations.v[1]) > pi2)
        a->rotations.v[1] = 0;
    if (fabs(a->rotations.v[1]) > pi2)
        a->rotations.v[1] = 0;
}
//------------------------------------------
void Map::removeAsset(unsigned long index){
    assets.remove(index);
}
//------------------------------------------
void Map::setAssetRotation(unsigned long index, Vector3D r){
    if (index > assets.count())
        return;
    assets[index].rotations = r;
}
//------------------------------------------
void Map::setAssetScale(unsigned long index, Vector3D r){
    if (index > assets.count())
        return;
    assets[index].scale = r;
}
//--------------------------------------------
void Map::setAssetPosition(unsigned long index, Vector3D p){

    if (index > assets.count())
        return;
    assets[index].position = p;
}
//-------------------------------------------
void Map::buildAssetMatrix(unsigned long index,
                           FlatMatrix& m){

    if (index > assets.count())
        return;
    Asset * a = &assets[index];
    FlatMatrix stm;
    MatrixTranslationScale( a->position, a->scale, stm.m);
    FlatMatrix rmx, rmy, rmz;
    MatrixRotationAxis(a->rotations.v[0], Vector3D(1,0,0), rmx.m);
    MatrixRotationAxis(a->rotations.v[1], Vector3D(0,1,0), rmy.m);
    MatrixRotationAxis(a->rotations.v[2], Vector3D(0,0,1), rmz.m);

    FlatMatrix rot = rmx * rmy * rmz;
    m = rot * stm;

    //MatrixFlat2Matrix(fm, m);

}

//------------------------------------------
void Map::draw3DAssets(SubsetCollection& models,
                      PicsContainer& pics,
                      unsigned picIndex,
                      bool useVBO){



    glBindTexture(GL_TEXTURE_2D, pics.getname(picIndex));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    for (unsigned i = 0; i < assets.count(); i++){
        glPushMatrix();

        FlatMatrix stm;
        MatrixTranslationScale( assets[i].position, 
                                assets[i].scale, stm.m);
        FlatMatrix rmx, rmy, rmz;
        MatrixRotationAxis(assets[i].rotations.v[0], Vector3D(1.0f, 0, 0), rmx.m);
        MatrixRotationAxis(assets[i].rotations.v[1], Vector3D(0, 1.0f, 0), rmy.m);
        MatrixRotationAxis(assets[i].rotations.v[2], Vector3D(0, 0, 1.0f), rmz.m);

        FlatMatrix rot = rmx * rmy * rmz;
        FlatMatrix fm = rot * stm;

        glMultMatrixf(fm.m);

        models.draw(assets[i].assetId);


        glPopMatrix();
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );


}
//----------------------------------------------------

void Map::draw3DAssetsSelected(SubsetCollection& models,
                          const unsigned * items,
                          unsigned long _count){
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
    glDisable(GL_TEXTURE_2D);

    for (unsigned long i = 0; i < _count; i++){
        glPushMatrix();

        FlatMatrix stm;
        MatrixTranslationScale( assets[items[i]].position, 
                                assets[items[i]].scale, stm.m);
        FlatMatrix rmx, rmy, rmz;
        MatrixRotationAxis(assets[items[i]].rotations.v[0],
                           Vector3D(1.0f, 0, 0), rmx.m);
        MatrixRotationAxis(assets[items[i]].rotations.v[1],
                           Vector3D(0, 1.0f, 0), rmy.m);
        MatrixRotationAxis(assets[items[i]].rotations.v[2],
                           Vector3D(0, 0, 1.0f), rmz.m);

        FlatMatrix rot = rmx * rmy * rmz;
        FlatMatrix fm = rot * stm;

        glMultMatrixf(fm.m);

        models.draw(assets[items[i]].assetId);
        glPopMatrix();
    }
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);


}

//----------------------------------------
void Map::draw3DPlane(PicsContainer& tileset,
                      unsigned picindex,
                      unsigned layer,
                      bool useVBO,
                      bool showCollision){

    glDisable(GL_LIGHTING);
    if (map){

        glBindTexture(GL_TEXTURE_2D, tileset.getname(picindex));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        float tw = 8.0f;

        unsigned count = 0;
        float * _verts = 0;
        PicData * p = tileset.getInfo(picindex);


        _verts = vo.lock();

        for (unsigned long i = 0; i < _width; i++){
            for (unsigned long a = 0; a <_height; a++){

                if (map[a][i].Layers[layer].frames[0]){

                    unsigned long base = count * (12 + 8 + 16);
                    Vector3D uv = calcUVS(p, i, a, layer);

                    size_t colorSize = sizeof(float)*4;

                    COLOR cColor;
                    cColor = ((!map[a][i].Layers[0].walkable)&&(showCollision)) ? 
                             COLOR(1.0f, 0.0f, 0.0f, 1.0f) :
                             COLOR(1.0f, 1.0f, 1.0f, 1.0f);

                    _verts[base + 0] = tw * i ;
                    _verts[base + 1] = 0.0f;
                    _verts[base + 2] = tw * a ;

                    memcpy(&_verts[base + 3], cColor.c, colorSize);

                    _verts[base + 7] = uv.v[0];
                    _verts[base + 8] = uv.v[2];

                    //----------------------------------------------


                    _verts[base + 9] = tw * i + tw;
                    _verts[base + 10] = 0.0f;
                    _verts[base + 11] = tw * a ;

                    memcpy(&_verts[base + 12], cColor.c, colorSize);

                    _verts[base + 16] = uv.v[1];
                    _verts[base + 17] = uv.v[2];

                    //--------------------------------------

                    _verts[base + 18] = tw * i + tw;
                    _verts[base + 19] = 0.0f;
                    _verts[base + 20] = tw * a + tw;

                    memcpy(&_verts[base + 21], cColor.c, colorSize);

                    _verts[base + 25] = uv.v[1];
                    _verts[base + 26] = uv.v[3];


                    //---------------------------------------

                    _verts[base + 27]     = tw * i ;
                    _verts[base + 28] = 0.0f;
                    _verts[base + 29] = tw * a + tw;

                    memcpy(&_verts[base + 30], cColor.c, colorSize);

                    _verts[base + 34] = uv.v[0];
                    _verts[base + 35] = uv.v[3];


                    count++;
                }
            }
        }
        
        vo.unlock();
        vo.bind();
        glDrawArrays(GL_QUADS, 0, 4 * count);
        vo.unbind();

    }

   
    glEnable(GL_LIGHTING);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

}
//----------------------------------------
void Map::draw(int x, int y,
                   PicsContainer& tileset,
                   unsigned picindex,
                   unsigned layer,
                   int viewportWidth,
                   int viewportHeight,
                   COLOR c){

        for (unsigned a = 0; a < _height; a++){
            for (unsigned i = 0; i < _width; i++){

                if (map[a][i].Layers[layer].frames[0]){

                    int shiftX = i * tilesize + x ;
                    int shiftY = a * tilesize + y ;

                    if ( ((shiftX > -32) && ( shiftX < viewportWidth )) &&
                         ((shiftY > -32) && ( shiftY < viewportHeight ))
                       )

                        tileset.draw(0,
                                    (int)(i) * tilesize + x,
                                    (int)(a) * tilesize + y,
                                    map[a][i].Layers[layer].frames[map[a][i].Layers[layer].tilenum] - 30,
                                    false,
                                    1.0f,
                                    1.0f, 0.0f, c, c);
                }
            }
        }


}
//---------------------------------
void Map::drawMiniMap(int x, int y,
                      PicsContainer& tileset,
                      float tilesize){

    for (unsigned a = 0; a < _height; a++){

        for (unsigned i = 0; i < _width; i++){

            COLOR c = (!map[a][i].Layers[0].walkable) ?
                      COLOR(0.5f, 0.5f, 0.5f, 0.8f) :
                      COLOR(0.8f, 0.8f, 0.8f, 0.8f);
            tileset.draw(-1,
                         (int)(i) * tilesize + x,
                         (int)(a) * tilesize + y,
                         0,
                         false,
                         tilesize,
                         tilesize, 
                         0.0f, c, c);

        }
    }


}
//---------------------------------

void Map::update(unsigned level){

        for (unsigned a = 0; a < _height; a++){
            for (unsigned i = 0; i < _width; i++){

                if (map[a][i].Layers[level].count > 1){
                    map[a][i].Layers[level].tilenum ++;

                    if (map[a][i].Layers[level].tilenum >= map[a][i].Layers[level].count){
                        map[a][i].Layers[level].tilenum = 0;
                    }
                }

            }
        }

}

//----------------------------
bool Map::load(const char* failas, bool useVBO){

    FILE* f;


    strcpy(name,failas);

    f = fopen(name,"rb");
    if (f == 0)
        return false;

    char tmp[100];
    int result = 0;

    int nmaxx=0, nmaxy=0;
    result = fread(&nmaxx,sizeof(int),1,f);
    sprintf(tmp,"width = %d",nmaxx);
    puts(tmp);
    result = fread(&nmaxy,sizeof(int),1,f);
    sprintf(tmp,"height = %d",nmaxy);
    puts(tmp);

    int ilgis=0;

    result = fread(&ilgis,sizeof(int),1,f);
    sprintf(tmp,"tilest name len = %d",ilgis);
    puts(tmp);
    memset(&tileset,0,sizeof(char)*255);
    result = fread(&tileset,sizeof(char),ilgis,f);
    puts(tileset);

    result = fread(&ilgis,sizeof(int),1,f);
    sprintf(tmp,"music name len = %d",ilgis);
    puts(tmp);
    memset(&music,0,sizeof(char)*255);
    result = fread(&music, sizeof(char),ilgis,f);
    puts(music);
    result = fread(&camHeight, sizeof(float), 1, f);
    result = fread(&camDistance, sizeof(float), 1, f);
    result = fread(&camPitch, sizeof(float), 1, f);

    //---
    unsigned is3d = 0;
    result = fread(&is3d, sizeof(unsigned), 1, f);
    printf("is3D? %u\n", is3d);
    isMap3D = (is3d)? true : false;


    resize(nmaxx,nmaxy);

    if (isMap3D){
        alloc3DMap(useVBO);
    }


    for (unsigned a = 0; a < _height;a++){
        for(unsigned i = 0; i < _width;i++){
            result = fread(&map[a][i],sizeof(Cell),1,f);
        }
    }

    //---------------
    //assets
    unsigned long assetCount = 0;
    assets.destroy();
    result = fread(&assetCount, 4, 1, f);
    for (unsigned long i = 0; i < assetCount; i++){
        Asset a;
        
        if (!fread(a.position.v, sizeof(float), 4, f)){//pos;
            fclose(f);
            return false;
        }
        //a.position.v[3] = 0.0f;
        if (!fread(a.rotations.v, sizeof(float), 4, f)){ //rotation
            fclose(f);
            return false;
        }
        //a.rotations.v[3] = 0.0f;
        if (!fread(a.scale.v, sizeof(float), 4, f)){ //scale
            //fclose(f);
            return false;
        }
        //a.scale.v[3] = 0.0f;
        int index = 0;
        if (!fread(&index, sizeof(int), 1, f)){ //assetID
            fclose(f);
            return false;
        }
        a.assetId = (unsigned)index;
        assets.add(a);
    }


    fclose(f);


    return true;
}
//----------------------
bool Map::save(const char* failas){

    FILE* f = 0;

    f = fopen(failas,"wb+");
    if (!f)
        return false;

    if (!fwrite(&_width, sizeof(unsigned), 1, f)){
        fclose(f);
        return false;
    }
    if (!fwrite(&_height, sizeof(unsigned), 1, f)){
        fclose(f);
        return false;
    }

    unsigned ilgis = (unsigned)strlen(tileset);
    int result = 0;

    result = fwrite(&ilgis,sizeof(unsigned),1,f);
    result = fwrite(tileset,sizeof(char),ilgis,f);

    ilgis = (unsigned)strlen(music);
    result = fwrite(&ilgis, sizeof(unsigned),1,f);
    result = fwrite(music, sizeof(char),ilgis,f);

    result = fwrite(&camHeight, sizeof(float), 1, f);
    result = fwrite(&camDistance, sizeof(float), 1, f);
    result = fwrite(&camPitch, sizeof(float), 1, f);

    unsigned is3d = 0;
    is3d = (isMap3D)? 1 : 0;
    //printf("is3D? %u\n", is3d);
    result = fwrite(&is3d, sizeof(unsigned), 1, f);

    for (unsigned a = 0; a < _height; a++){
        for (unsigned i = 0; i < _width; i++){

            map[a][i].Layers[0].tilenum = 0;
            map[a][i].Layers[1].tilenum = 0;
            if (!fwrite(&map[a][i], sizeof(Cell), 1, f)){
                fclose(f);
                return false;
            }
        }
    }
    //write assets
    unsigned long ac = assets.count();
    result = fwrite(&ac, 4, 1, f);
    for (unsigned i = 0; i < assets.count(); i++){

        if (!fwrite(assets[i].position.v, sizeof(float), 4, f)){//pos;
            fclose(f);
            return false;
        }
        if (!fwrite(assets[i].rotations.v, sizeof(float), 4, f)){ //rotation
            fclose(f);
            return false;
        }
        if (!fwrite(assets[i].scale.v, sizeof(float), 4, f)){ //scale
            fclose(f);
            return false;
        }
        if (!fwrite(&assets[i].assetId, sizeof(int), 1, f)){ //assetID
            fclose(f);
            return false;
        }

    }

    fclose(f);

    return true;
}
//--------------------------------------------

void Map::resize(unsigned nx, unsigned ny){
        Cell** tmp=0;

        if ((nx != _width) || (ny != _height)){

            if (map){
                tmp = new Cell*[_height];
                for (unsigned i = 0; i < _height; i++){
                    tmp[i] = new Cell[_width];
                    memcpy(tmp[i], map[i], _width * sizeof(Cell));
                }
            }

            if (map){
                for (unsigned i = 0; i < _height;i++)
                    delete []map[i];
                delete []map;
                map=0;
            }

            map = new Cell*[ny];
            for (unsigned i=0;i<ny;i++){
                map[i]=new Cell[nx];
                memset(map[i],0,nx*sizeof(Cell));
                if (tmp){
                    if (i < _height){
                        int to;
                        if (nx < _width)
                            to=nx; else to = _width;
                            memcpy(map[i],tmp[i],to*sizeof(Cell));
                    }
                }
            }

  if (tmp){
   for (unsigned i=0;i < _height;i++)
    delete []tmp[i];
   delete []tmp;
  }

  _height = ny; _width = nx;
 }

}
//-------------------------
void Map::destroy(){
    if (map){
        for (unsigned i=0;i < _height;i++)
            delete []map[i];
        delete []map;
        map=0; _height=0; _width=0;
    }

    assets.destroy();

    destroy3DMap();
}
//-----------------------------
    void Map::setTile(unsigned x, unsigned y, unsigned level, unsigned tile){
        if ((x < _width) && (y < _height) && (level < 3) && (level >= 0)){
            map[y][x].Layers[level].frames[0] = tile;

           // printf("tile: %u, layer: %u\n", tile, level);

        }
    }
 //-----------------------------
    void Map::setTile(unsigned x, unsigned y, unsigned level, Square & s){
        if ((x < _width) && (y < _height) && (level < 3) && (level >= 0)){
            map[y][x].Layers[level] = s;

        }
    }
 //-----------------------------
    unsigned Map::getTile(unsigned x, unsigned y, unsigned level){
        if ((x < _width) && (y < _height) && (level < 2) && (level >= 0)){
            return map[y][x].Layers[level].frames[0];

        }
        return 0;
    }
//-----------------------------
    Square  Map::_getTile(unsigned x, unsigned y, unsigned level){
        Square empty;
        empty.tilenum = 0;
        empty.walkable =false;
        empty.count = 0;
        memset(empty.frames, 0, sizeof(unsigned)*10);
        if ((x < _width) && (y < _height) && (level < 2) && (level >= 0)){
            return map[y][x].Layers[level];

        }
        return empty;
    }

 //-----------------------------
    bool Map::isWalkable(unsigned x, unsigned y){
        if ((x < _width) && (y < _height))
            if (map[y][x].Layers[0].walkable)
                return true;
        return false;
    }
//------------------------------
    void Map::setWalkable(unsigned x, unsigned y, bool status){
        if ((x < _width) && (y < _height)){
            map[y][x].Layers[0].walkable = status;
        }
    }

//------------------------------
    unsigned Map::getMonsterGroup(unsigned x, unsigned y){
        if ((x < _width) && (y < _height))
            return map[y][x].groupindex;
        return 0;
    }
//-------------------------------
    void    Map::setMonsterGroup(unsigned x, unsigned y, unsigned index){
        if ((x < _width) && (y < _height))
            map[y][x].groupindex = index;
    }


