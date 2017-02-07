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

//--------------------------

void MapHeadDlg::toggle3D(Map &map){

    if (map.is3D()){
        map.setMap2D();
    }
    else
        map.setMap3D(false);

}
//---------------------------
void MapHeadDlg::getTileset(char* tileset){
    strcpy(tileset, gtk_entry_get_text(GTK_ENTRY(eTileset)));
}
//------------------------------------
void MapHeadDlg::getMusic(char* music){
    strcpy(music, gtk_entry_get_text(GTK_ENTRY(eMusic)));
}
//---------------------------
void MapHeadDlg::setTileset(const char* tileset){
    gtk_entry_set_text( GTK_ENTRY(eTileset), tileset);
}
//--------------------------------
void MapHeadDlg::setMusic(const char* music){
    gtk_entry_set_text( GTK_ENTRY(eMusic), music);
}
//---------------------
void MapHeadDlg::show(){
    gtk_widget_show_all (dialog);
}
//--------------------------------------
void MapHeadDlg::hide(){
    gtk_widget_hide_all (dialog);
}
//-----------------------------------
float MapHeadDlg::getCamPitch(){ 
    return atof(gtk_entry_get_text(GTK_ENTRY(eCPitch)));
}
//-----------------------------------
float MapHeadDlg::getCamHeight(){
    return atof(gtk_entry_get_text(GTK_ENTRY(eCHeight)));
}
//-----------------------------------
float MapHeadDlg::getCamDistance(){
    return atof(gtk_entry_get_text(GTK_ENTRY(eCDistance)));
}
//-----------------------------------
void MapHeadDlg::setCamPitch(float pitch){
    char buf[255];
    sprintf(buf, "%.4f", pitch);
    gtk_entry_set_text( GTK_ENTRY(eCPitch), buf);

}
//-----------------------------------
void MapHeadDlg::setCamHeight(float height){
    char buf[255];
    sprintf(buf, "%.4f", height);
    gtk_entry_set_text( GTK_ENTRY(eCHeight), buf);

}
//-----------------------------------
void MapHeadDlg::setCamDistance(float dist){
    char buf[255];
    sprintf(buf, "%.4f", dist);
    gtk_entry_set_text( GTK_ENTRY(eCDistance), buf);

}
//---------------------------
 void MapHeadDlg::buildEntryWithTitle(const char* title,
                                      GtkWidget** entry,
                                      GtkWidget** label,
                                      GtkWidget** container){
    *label = gtk_label_new(title);
    gtk_box_pack_start(GTK_BOX(*container), *label, FALSE, TRUE, 0);
    *entry        = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(*container), *entry, FALSE, TRUE, 0);


 }



//--------------------------
void MapHeadDlg::build(){

    dialog = gtk_dialog_new();
    //gtk_dialog_set_has_separator(GTK_DIALOG(dialog), false); 
    gtk_window_set_title(GTK_WINDOW(dialog), "Map Header");      
    gtk_widget_set_usize (GTK_WIDGET (dialog), 200, 300);

    GtkWidget * content_area = GTK_DIALOG (dialog)->vbox;

    buildEntryWithTitle("Tileset:", &eTileset, &lTileset, &content_area);
    buildEntryWithTitle("Music:", &eMusic, &lMusic, &content_area);


    is3D = gtk_check_button_new_with_label("Is map 3D ?");
    gtk_box_pack_start(GTK_BOX(content_area), is3D ,FALSE, TRUE, 0);
    
    GtkWidget* separator = gtk_hseparator_new ();
    gtk_box_pack_start(GTK_BOX(content_area), separator, FALSE, TRUE, 0);

    buildEntryWithTitle("Camera height above ground:", &eCHeight, &lCHeight, &content_area);
    buildEntryWithTitle("Camera distance from hero:", &eCDistance, &lCDistance, &content_area);
    buildEntryWithTitle("Camera pitch:", &eCPitch, &lCPitch, &content_area);

    ok_button = gtk_dialog_add_button(GTK_DIALOG(dialog), "OK", GTK_RESPONSE_YES);

}

