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

#include "TileTool.h"
#include "../../src/gui/Text.h"


//------------------------------------------
void TileTool::set(int _x, int _y, int w, int h,
                     unsigned itemCount){

    x = _x;
    y = _y;
    width = w;
    height = h;

    int posX = 5;
    moveCameraButton.init( x + posX, y + 5, 32, 32);
    pickTilesButton.init(x + posX, y + 40, 32, 32);
    rotateAssetButton.init(x + posX, y + 75, 32, 32);

    posX = 40;
    moveAssetButton.init(x + posX, y + 40, 32, 32);
    placeTilesButton.init( x + posX, y + 5, 32, 32);
    scaleAssetButton.init(x + posX, y + 75, 32, 32);

    placeTilesButton.pressed = false;
    moveCameraButton.pressed = true;

    posX = 100;
    Layer0.init(x + posX, y + 10, 16, 16);
    Layer0.pressed = true;
    Layer1.init(x + posX, y + 26, 16, 16);
    Layer3D.init(x + posX, y + 42, 16, 16);
    Collision.init(x + posX, y + 58, 16, 16);
    MonsterGroups.init(x + posX, y + 74, 16, 16);
    Assets.init(x + posX, y + 90, 16, 16);

    posX = 80;
    showLayer0.init(x + posX, y + 10, 16, 16);
    showLayer0.pressed = true;
    showLayer1.init(x + posX, y + 26, 16, 16);
    showLayer1.pressed = true;
    showLayer3D.init(x + posX, y + 42, 16, 16);
    showLayer3D.pressed = true;
    showCollision.init(x + posX, y + 58, 16, 16);
    showMonsterGroups.init(x + posX, y + 74, 16, 16);
    showMonsterGroups.pressed = true;
    showAssets.init(x + posX, y + 90, 16, 16);
    showAssets.pressed = true;

    AB.build(x + 215, y + 5, width - 215 * 2, 100, 50, itemCount);

    animAddFrame.init(x + (width - 110), y + 5, 16);
    animRemoveFrame.init(x + (width - 110), y + 40, 16);
    prevFrame.init(x + (width - 170), y + 70, 16, 16);
    nextFrame.init(x + (width - 135), y + 70, 16, 16);

    saveButton.init(x + (width - 75), y + 5, 32, 32);
    openButton.init(x + (width - 35), y + 5, 32, 32);


    /*    

        title = gtk_label_new( "Monster group:");
        GtkWidget * title2;

        gtk_entry_set_text( GTK_ENTRY(MGEdit),
                            "0");
*/
}
//------------------------------------------
void TileTool::draw(PicsContainer& pics, SubsetCollection & assets,
                    Square& sq,
                    int mousex, int mousey){

    pics.draw(-1, x, y, 0, false, width*1.0f, 110.0f, 0.0f,
              COLOR(0.8f, 0.8f, 0.8f, 0.5f),
              COLOR(0.8f, 0.8f, 0.8f, 0.5f));
    moveCameraButton.draw(pics, 2, (moveCameraButton.pressed)? 3 : 1);
    placeTilesButton.draw(pics, 2, (placeTilesButton.pressed)? 2 : 0);
    moveAssetButton.draw(pics, 2, (moveAssetButton.pressed)? 7 : 8);
    rotateAssetButton.draw(pics, 2, (rotateAssetButton.pressed)? 9 : 10);
    scaleAssetButton.draw(pics, 2, (scaleAssetButton.pressed)? 11 : 12);

    pickTilesButton.draw(pics, 2, (pickTilesButton.pressed)? 4 : 5);

    Layer0.draw(pics, 3, (Layer0.pressed) ? 1: 0);
    Layer1.draw(pics, 3, (Layer1.pressed) ? 1: 0);
    Layer3D.draw(pics, 3, (Layer3D.pressed) ? 1: 0);
    Collision.draw(pics, 3, (Collision.pressed) ? 1: 0);
    MonsterGroups.draw(pics, 3, (MonsterGroups.pressed) ? 1: 0);
    Assets.draw(pics, 3, (Assets.pressed) ? 1: 0);

    showLayer0.draw(pics, 3, (showLayer0.pressed) ? 8: 9);
    showLayer1.draw(pics, 3, (showLayer1.pressed) ? 8: 9);
    showLayer3D.draw(pics, 3, (showLayer3D.pressed) ? 8: 9);
    showCollision.draw(pics, 3, (showCollision.pressed) ? 8: 9);
    showMonsterGroups.draw(pics, 3, (showMonsterGroups.pressed) ? 8: 9);
    showAssets.draw(pics, 3, (showAssets.pressed) ? 8: 9);
    
    WriteText(x + 120, y + 10, pics, 1, "Layer 0", 0.6f, 1.0f);
    WriteText(x + 120, y + 26, pics, 1, "Layer 1", 0.6f);
    WriteText(x + 120, y + 42, pics, 1, "Layer 3D", 0.6f);
    WriteText(x + 120, y + 58, pics, 1, "Collision", 0.6f);
    WriteText(x + 120, y + 74, pics, 1, "Monster Groups", 0.6f);
    WriteText(x + 120, y + 90, pics, 1, "3D Assets", 0.6f);

    char buf[50];
    animAddFrame.draw(pics, 3, 6);
    sprintf(buf, "%d", sq.count);
    WriteText(x + (width - 110), y + 40, pics, 1, buf);
    animRemoveFrame.draw(pics, 3, 7);
    prevFrame.draw(pics, 3, 4);
    sprintf(buf, "%d", sq.tilenum);
    WriteText(x + (width - 150), y + 75, pics, 1, buf);
    nextFrame.draw(pics, 3, 5);

    saveButton.draw(pics, 2, 6);
    openButton.draw(pics, 2, 13);


    if ((Layer3D.pressed) || (Assets.pressed)){
        AB.drawAssets(assets, pics, 0, mousex, mousey,sq.frames[sq.tilenum] );
    }
    else{
        AB.drawTiles(pics, 0, mousex, mousey, sq.frames[sq.tilenum] + 1);
        pics.draw(0, x + (width - 160), y + 30, sq.frames[sq.tilenum], false);
    }


    
}
//------------------------------------------
bool TileTool::isPointerOnTop(int px, int py){
    if ((px > x) && (px < x + width)
            && (py > y) && (py < y + height))

        return true;

    return false;
}
//-------------------------------------------
void TileTool::moveCameraButton_clicked(){
    moveCameraButton.pressed = true;
    draw3D = false;
    pickTilesButton.pressed = false;
    placeTilesButton.pressed = false;
    scaleAssetButton.pressed = false;
    rotateAssetButton.pressed = false;
    moveAssetButton.pressed = false;
}
//-------------------------------------------
void TileTool::placeTilesButton_clicked(){
    placeTilesButton.pressed = true;
    draw3D = true;
    moveCameraButton.pressed = false;
    pickTilesButton.pressed = false;
    scaleAssetButton.pressed = false;
    rotateAssetButton.pressed = false;
    moveAssetButton.pressed = false;
}
//-------------------------------------------
void TileTool::pickTilesButton_clicked(){
    pickTilesButton.pressed = true;
    moveCameraButton.pressed = false;
    placeTilesButton.pressed = false;
    scaleAssetButton.pressed = false;
    rotateAssetButton.pressed = false;
    moveAssetButton.pressed = false;
    draw3D = true;
}
//-------------------------------------------
void TileTool::moveAssetButton_clicked(){

    moveAssetButton.pressed = true;
    moveCameraButton.pressed = false;
    placeTilesButton.pressed = false;
    pickTilesButton.pressed = false;
    scaleAssetButton.pressed = false;
    rotateAssetButton.pressed = false;
    draw3D = false;
}
//-------------------------------------------
void TileTool::rotateAssetButton_clicked(){
    rotateAssetButton.pressed = true;
    moveCameraButton.pressed = false;
    placeTilesButton.pressed = false;
    pickTilesButton.pressed = false;
    scaleAssetButton.pressed = false;
    moveAssetButton.pressed = false;
    draw3D = false;
}
//-------------------------------------------
void TileTool::scaleAssetButton_clicked(){
    scaleAssetButton.pressed = true;
    moveCameraButton.pressed = false;
    placeTilesButton.pressed = false;
    pickTilesButton.pressed = false;
    moveAssetButton.pressed = false;
    rotateAssetButton.pressed = false;
    draw3D = false;
}
//-------------------------------------------
void TileTool::layer0Button_clicked(){
    Layer0.pressed = true;
    Layer1.pressed = false;
    Layer3D.pressed = false;
    Collision.pressed = false;
    MonsterGroups.pressed = false;
    Assets.pressed = false;
}
//-------------------------------------------
void TileTool::layer1Button_clicked(){
    Layer1.pressed = true;
    Layer0.pressed = false;
    Layer3D.pressed = false;
    Collision.pressed = false;
    MonsterGroups.pressed = false;
    Assets.pressed = false;
}
//-------------------------------------------
void TileTool::layer3DButton_clicked(){
    Layer3D.pressed = true;
    Layer1.pressed = false;
    Layer0.pressed = false;
    Collision.pressed = false;
    MonsterGroups.pressed = false;
    Assets.pressed = false;
}
//-------------------------------------------
void TileTool::collisionButton_clicked(){
    Collision.pressed = true;
    Layer1.pressed = false;
    Layer0.pressed = false;
    Layer3D.pressed = false;
    MonsterGroups.pressed = false;
    Assets.pressed = false;
}
//-------------------------------------------
void TileTool::monsterGroupButton_clicked(){
    MonsterGroups.pressed = true;
    Layer1.pressed = false;
    Layer0.pressed = false;
    Collision.pressed = false;
    Layer3D.pressed = false;
    Assets.pressed = false;
}
//-------------------------------------------
void TileTool::assetButton_clicked(){
    Assets.pressed = true;
    MonsterGroups.pressed = false;
    Layer1.pressed = false;
    Layer0.pressed = false;
    Collision.pressed = false;
    Layer3D.pressed = false;
}
//-------------------------------------------
void TileTool::resetAssetBrowser(unsigned itemCount){
    
    AB.build(x + 215, y + 5, width - 215 * 2, 100, 50, itemCount);
}
