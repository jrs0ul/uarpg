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
#include "MapHeaderDlg.h"
#include "../../src/gui/Text.h"


//--------------------------

void MapHeadDlg::toggle3D(Map &map){

    if (map.is3D()){
        map.setMap2D();
    }
    else
        map.setMap3D(false);

}
//-------------------------------------
bool MapHeadDlg::isPointerOnTop(int px, int py){
    if ((px > x) && (px < x + width)
            && (py > y) && (py < y + height))

        return true;

    return false;
}

//---------------------------
void MapHeadDlg::getTileset(char* tileset){
    strcpy(tileset, eTileset);
}
//------------------------------------
void MapHeadDlg::getMusic(char* music){
    strcpy(music, eMusic);
}
//---------------------------
void MapHeadDlg::setTileset(const char* tileset){
    strncpy(eTileset, tileset, 256);
}
//--------------------------------
void MapHeadDlg::setMusic(const char* music){
    strncpy(eMusic, music, 256);
}
/*
//---------------------
void MapHeadDlg::show(){
    gtk_widget_show_all (dialog);
}
//--------------------------------------
void MapHeadDlg::hide(){
    gtk_widget_hide_all (dialog);
}
*/
//-----------------------------------
float MapHeadDlg::getCamPitch(){ 
    return atof(eCPitch);
}
//-----------------------------------
float MapHeadDlg::getCamHeight(){
    return atof(eCHeight);
}
//-----------------------------------
float MapHeadDlg::getCamDistance(){
    return atof(eCDistance);
}
//-----------------------------------
void MapHeadDlg::setCamPitch(float pitch){
    sprintf(eCPitch, "%.4f", pitch);
    
}
//-----------------------------------
void MapHeadDlg::setCamHeight(float height){
    sprintf(eCHeight, "%.4f", height);
}
//-----------------------------------
void MapHeadDlg::setCamDistance(float dist){
    sprintf(eCDistance, "%.4f", dist);

}
/*//---------------------------
 void MapHeadDlg::buildEntryWithTitle(const char* title,
                                      GtkWidget** entry,
                                      GtkWidget** label,
                                      GtkWidget** container){
    *label = gtk_label_new(title);
    gtk_box_pack_start(GTK_BOX(*container), *label, FALSE, TRUE, 0);
    *entry        = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(*container), *entry, FALSE, TRUE, 0);


 }


*/
//--------------------------
void MapHeadDlg::set(int _x, int _y, unsigned w,
                     unsigned h){
    x = _x; y = _y;
    width = w; height = h;
    is3D.set( x + 5, y + 6, 16, 16);
    CHeightButton.set(x + 5, y + 55, 16, 16);
    CDistanceButton.set(x + 5, y + 70, 16, 16);
    CPitchButton.set(x + 5, y + 85, 16, 16);
/*    dialog = gtk_dialog_new();

    buildEntryWithTitle("Tileset:", &eTileset, &lTileset, &content_area);
    buildEntryWithTitle("Music:", &eMusic, &lMusic, &content_area);

    ok_button = gtk_dialog_add_button(GTK_DIALOG(dialog), "OK", GTK_RESPONSE_YES);
*/

}

//--------------------------
void MapHeadDlg::draw(PicsContainer& pics){

    pics.draw(-1, x, y, 0, false, width * 1.0f, height * 1.0f, 0.0f,
              COLOR(0.8f, 0.8f, 0.8f, 0.5f),
              COLOR(0.8f, 0.8f, 0.8f, 0.5f));
    
    if (is3D.pressed)
        is3D.draw(pics, 3, 1);
    else
        is3D.draw(pics, 3, 0);

    WriteText(x + 25, y + 10, pics, 1, "Is map 3D ?", 0.5f);

    char buf[512];
    sprintf(buf, "Tileset name: %s", eTileset);
    WriteText(x + 25, y + 25, pics, 1, buf, 0.5f);
    sprintf(buf, "Music: %s", eMusic);
    WriteText(x + 25, y + 40, pics, 1, buf, 0.5f);
    CHeightButton.draw(pics, 3, 10);
    sprintf(buf, "Camera height: %s", eCHeight);
    WriteText(x + 25, y + 55, pics, 1, buf, 0.5f);
    CDistanceButton.draw(pics, 3, 10);
    sprintf(buf, "Camera distance: %s", eCDistance);
    WriteText(x + 25, y + 70, pics, 1, buf, 0.5f);
    CPitchButton.draw(pics, 3, 10);
    sprintf(buf, "Camera pitch: %s", eCPitch);
    WriteText(x + 25, y + 85, pics, 1, buf, 0.5f);

}

