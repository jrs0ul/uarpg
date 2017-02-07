/***************************************************************************
 *   Copyright (C) 2009 by jrs0ul                                          *
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

#ifndef TILE_TOOL_H
#define TILE_TOOL_H

#include "../../src/gui/Button.h"
#include "../../src/TextureLoader.h"
#include "AssetBrowser.h"
#include "../../src/models/rm2.h"
#include "../../src/Worldmap.h"

class TileTool{

    int x;
    int y;
    int width;
    int height;

public:
   /*
        GtkWidget *MGEdit;
*/

    AssetBrowser AB;
    Button moveCameraButton;
    Button placeTilesButton;
    Button pickTilesButton;
    Button moveAssetButton;
    Button rotateAssetButton;
    Button scaleAssetButton;

    Button Layer0;
    Button Layer1;
    Button Layer3D;
    Button Collision;
    Button MonsterGroups;
    Button Assets;

    Button showLayer0;
    Button showLayer1;
    Button showLayer3D;
    Button showCollision;
    Button showMonsterGroups;
    Button showAssets;

    Button animAddFrame;
    Button animRemoveFrame;
    Button prevFrame;
    Button nextFrame;

    Button saveButton;
    Button openButton;

    bool picktiles;
    bool draw3D;


    TileTool(){picktiles = false; draw3D = false;}

    void set(int _x, int _y, int w, int h,
               unsigned itemCount);
    void draw(PicsContainer& pics, SubsetCollection & assets,
              Square& sq,
              int mousex, int mousey);
    bool isPointerOnTop(int px, int py);

    void destroy(){AB.destroy();}


    void moveCameraButton_clicked();
    void placeTilesButton_clicked();
    void pickTilesButton_clicked();
    void moveAssetButton_clicked();
    void rotateAssetButton_clicked();
    void scaleAssetButton_clicked();

    void layer0Button_clicked();
    void layer1Button_clicked();
    void layer3DButton_clicked();
    void collisionButton_clicked();
    void monsterGroupButton_clicked();
    void assetButton_clicked();

    void resetAssetBrowser(unsigned itemCount);

};


//==========================================



#endif //TILE_TOOL_H
