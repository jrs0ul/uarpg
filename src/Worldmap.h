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
#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "TextureLoader.h"
#include "models/rm2.h"
#include "VertexObject.h"


enum LayerTypes {LAYER1, LAYER2, LAYER3D};

struct Square{

    bool walkable;

    unsigned tilenum;    //index in frames array
    unsigned frames[10]; //if tile animated it will have some frames
    unsigned count;      //how many frames it will have
};

//---------------------
struct Cell{
    Square Layers[3]; //cell layers
    unsigned groupindex;
};
//----------------------
struct Asset{
    Vector3D position;
    Vector3D rotations; //rotation around x, y, z axis
    Vector3D scale;
    unsigned assetId;
};

//----------------------
class Map{

    int tilesize;
    unsigned _width;
    unsigned _height;

    Cell** map;

    DArray<Asset> assets;

    VertexObject vo;

    bool isMap3D;

    Vector3D calcUVS(PicData * p, unsigned long x, unsigned long y,
                     unsigned layer);

public:
    char name[255];
    char tileset[255];
    char music[255];

    float camHeight;
    float camDistance;
    float camPitch;

    unsigned cellsize;

                  Map();

    bool          is3D(){return isMap3D;}
    void          setMap3D(bool useVBO = true);
    void          setMap2D();

    void          addAsset(Asset& a);
    void          removeAsset(unsigned long index);
    Asset *       getAsset(unsigned long index);
    unsigned long assetCount(){return assets.count();}
    void          scaleAsset(unsigned long index, Vector3D scale);
    void          rotateAsset(unsigned long index, Vector3D rotation);
    void          moveAsset(unsigned long index, Vector3D shift);


    void          setAssetRotation(unsigned long index, Vector3D s);
    void          setAssetScale(unsigned long index, Vector3D r);
    void          setAssetPosition(unsigned long index, Vector3D p);
    void          buildAssetMatrix(unsigned long index, FlatMatrix& m);

    //draws 2D map in sprite batch
    void draw(int x, int y,
              PicsContainer& tileset,
              unsigned picindex,
              unsigned layer,
              int viewportWidth,
              int viewportHeight,
              COLOR c = COLOR(1.0f, 1.0f, 1.0f));

    //allocates memory for floor tiles
    void alloc3DMap(bool useVBO = true);
    void destroy3DMap();


    //Draws 3D plane with same tiles as in draw()
    void draw3DPlane(PicsContainer& tileset,
                unsigned picindex,
                unsigned layer,
                bool useVBO = true,
                bool showCollision = false);
    //Draws 3D subsets as tiles
    void draw3DTiles(Vector3D frustumCenter,
                       float frustumRadius,
                       SubsetCollection& models,
                       PicsContainer& pics,
                       unsigned picindex,
                       unsigned layer,
                       bool useVBO = true,
                       bool showCollision = false);
    void draw3DAssets(SubsetCollection& models,
                      PicsContainer& pics,
                      unsigned picIndex,
                      bool useVBO = true);
    void draw3DAssetsSelected(SubsetCollection& models,
                              const unsigned * items,
                              unsigned long _count);

    //draws mini map
    void drawMiniMap(int x, int y,
                     PicsContainer& tileset,
                     float tilesize);

    bool load(const char* failas, bool useVBO = true);
    bool save(const char* failas);

    //updates animated tiles
    void     update(unsigned level);
    //is tile walkable
    bool     isWalkable(unsigned x, unsigned y);
    //sets tile walkable
    void     setWalkable(unsigned x, unsigned y, bool status);
    //get monster group
    unsigned getMonsterGroup(unsigned x, unsigned y);
    //sets monster group
    void     setMonsterGroup(unsigned x, unsigned y, unsigned index);
    void     resize(unsigned nx, unsigned ny);

    unsigned width(){return _width;}
    unsigned height(){return _height;}
    //changes map tile
    void     setTile(unsigned x, unsigned y, unsigned level, unsigned tile);
    void     setTile(unsigned x, unsigned y, unsigned level, Square & s);
    //gets map tile
    unsigned getTile(unsigned x, unsigned y, unsigned level);
    Square   _getTile(unsigned x, unsigned y, unsigned level);
    //destroys map data
    void     destroy();
};

#endif //WORLDMAP_H

