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


//------------------------------------------
    void TileTool::build(GdkGLContext * ContextToShare3D){

        dialog = gtk_dialog_new();
        gtk_dialog_set_has_separator(GTK_DIALOG(dialog), false);
        gtk_window_set_title(GTK_WINDOW(dialog), "Tiles");
        gtk_widget_set_usize (GTK_WIDGET (dialog), 140, 600);

        adj = gtk_adjustment_new (0.0, 0.0, 0.0, 1, 1, 1);
        HSB = gtk_hscrollbar_new (GTK_ADJUSTMENT (adj));
        gtk_range_set_update_policy (GTK_RANGE (HSB),
                                 GTK_UPDATE_CONTINUOUS);

        adjv = gtk_adjustment_new (0.0, 0.0, 0.0, 1, 1, 1);
        VSB = gtk_vscrollbar_new (GTK_ADJUSTMENT (adjv));
        gtk_range_set_update_policy (GTK_RANGE (VSB),
                                 GTK_UPDATE_CONTINUOUS);



        MGEdit = gtk_entry_new();


        GtkWidget * content_area = GTK_DIALOG (dialog)->vbox;
        DA = gtk_drawing_area_new();
        gtk_widget_set_size_request(DA, 100, 64);

        gtk_widget_set_events (DA, GDK_EXPOSURE_MASK|GDK_LEAVE_NOTIFY_MASK
             | GDK_BUTTON_PRESS_MASK
             | GDK_POINTER_MOTION_MASK
             | GDK_POINTER_MOTION_HINT_MASK);

        LayerSelect = gtk_combo_box_new_text();

        GtkWidget * title;
        title = gtk_label_new( "Monster group:");
        GtkWidget * title2;
        title2 = gtk_label_new( "Map Layer:");

        gtk_entry_set_text( GTK_ENTRY(MGEdit),
                            "0");

        picker = gtk_check_button_new_with_label ("Pick tile");


        gtk_combo_box_append_text(GTK_COMBO_BOX(LayerSelect), "Layer 0");
        gtk_combo_box_append_text(GTK_COMBO_BOX(LayerSelect), "Layer 1");
        gtk_combo_box_append_text(GTK_COMBO_BOX(LayerSelect), "Layer 3");
        gtk_combo_box_append_text(GTK_COMBO_BOX(LayerSelect), "MonsterGroups");
        gtk_combo_box_append_text(GTK_COMBO_BOX(LayerSelect), "Collision");

        hbox = gtk_hbox_new (FALSE, 1);

        gtk_container_add(GTK_CONTAINER(content_area), hbox);

        gtk_container_add(GTK_CONTAINER(hbox), DA);
        gtk_box_pack_end(GTK_BOX(hbox), VSB, FALSE, TRUE, 0);




        anim_DA = gtk_drawing_area_new();
        gtk_widget_set_size_request(anim_DA, 32, 32);
        gtk_widget_set_events (anim_DA, GDK_EXPOSURE_MASK);
        anim_hbox = gtk_hbox_new (FALSE, 1);
        gtk_container_add(GTK_CONTAINER(anim_hbox), anim_DA);
        anim_vbox = gtk_vbox_new (FALSE, 1);
        anim_remf = gtk_button_new_with_label ("-");
        anim_count = gtk_label_new ("1");
        anim_addf = gtk_button_new_with_label("+");
        gtk_container_add(GTK_CONTAINER(anim_vbox), anim_remf);
        gtk_container_add(GTK_CONTAINER(anim_vbox), anim_count);
        gtk_container_add(GTK_CONTAINER(anim_vbox), anim_addf);

        gtk_box_pack_end(GTK_BOX(anim_hbox), anim_vbox, FALSE, TRUE, 0);

        anim_prevf = gtk_button_new_with_label("<");
        anim_current = gtk_label_new("0");
        anim_nextf = gtk_button_new_with_label (">");
        anim_hbox2 = gtk_hbox_new (FALSE, 1);
        gtk_container_add(GTK_CONTAINER(anim_hbox2), anim_prevf);
        gtk_container_add(GTK_CONTAINER(anim_hbox2), anim_current);
        gtk_container_add(GTK_CONTAINER(anim_hbox2), anim_nextf);
        GtkWidget* separator = gtk_hseparator_new ();


        BuildOpenGLArea(&visualArea3D, ContextToShare3D);
        gtk_widget_set_size_request(visualArea3D, 100, 100);

        GtkWidget* hb = gtk_hbox_new (FALSE, 1); 


        adjpn = gtk_adjustment_new (0.0, 0.0, 0.0, 1, 1, 1);
        prevNext3DTile = gtk_hscrollbar_new (GTK_ADJUSTMENT (adjpn));
        gtk_range_set_update_policy (GTK_RANGE (prevNext3DTile),
                                 GTK_UPDATE_CONTINUOUS);
        /*next3DTile = gtk_button_new_with_label(">");
        prev3DTile = gtk_button_new_with_label("<");
        gtk_container_add(GTK_CONTAINER(hb), prev3DTile);
        gtk_container_add(GTK_CONTAINER(hb), next3DTile);*/
        gtk_container_add(GTK_CONTAINER(hb), prevNext3DTile);


        GtkWidget* hb2 = gtk_hbox_new (FALSE, 1); 
        GtkWidget* image = gtk_image_new_from_file("camera.png");
        GtkWidget* image2 = gtk_image_new_from_file("brush.png");
        moveCameraButton = gtk_toggle_button_new();
        placeTilesButton = gtk_toggle_button_new();
        gtk_button_set_image(GTK_BUTTON(moveCameraButton), image);
        gtk_button_set_image(GTK_BUTTON(placeTilesButton), image2);
        gtk_container_add(GTK_CONTAINER(hb2), moveCameraButton);
        gtk_container_add(GTK_CONTAINER(hb2), placeTilesButton);


        gtk_box_pack_end(GTK_BOX(content_area), hb2, FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), hb, FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), visualArea3D, FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), anim_hbox2, FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), anim_hbox, FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), separator, FALSE, TRUE, 10);
        gtk_box_pack_end(GTK_BOX(content_area), picker, FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), LayerSelect,FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), title2,FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), MGEdit,FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), title,FALSE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(content_area), HSB,FALSE, TRUE, 0);


        /*GtkWidget *anim_pixbuf;
        GtkWidget *anim_nextf;
        GtkWidget *anim_prevf;*/

    }

//------------------------------------------
    int TileTool::getAdjValue(){
        return (int)GTK_ADJUSTMENT(adj)->value;
    }
//------------------------------------------
    void TileTool::resize(GtkWidget *widget){
        gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
        gdk_window_process_updates (widget->window, FALSE);


        setupAdjustments();
    }
//------------------------------------------
    void TileTool::update(GtkWidget *widget){

        int width = gdk_pixbuf_get_width (pixbuf);
        int height = gdk_pixbuf_get_height (pixbuf);

        int scrx = (int)GTK_ADJUSTMENT(adj)->value;
        int scry = (int)GTK_ADJUSTMENT(adjv)->value;

        printf("w %d h %d\n", width, height);

        int drawwidth = (DA->allocation.width < width) ? DA->allocation.width : width;
        int drawheight = (DA->allocation.height < height) ? DA->allocation.height : height;
        //int htiles = width/32;
        //int srcy=((int)GTK_ADJUSTMENT(adj)->value)/htiles;
        //int srcx=((int)GTK_ADJUSTMENT(adj)->value)-(htiles*srcy);

        gdk_draw_pixbuf(widget->window,
                        NULL,
                        pixbuf,
                        scrx, scry,
                        0,0,drawwidth, drawheight,
                        GDK_RGB_DITHER_NONE,0,0);
    }

 //-----------------------------------------
    void TileTool::update_anim(GtkWidget *widget, unsigned frame){

        int width = gdk_pixbuf_get_width (pixbuf);
        //int height = gdk_pixbuf_get_height (pixbuf);

        int hframes = width/32;
        int hf = frame/hframes;
        int startx = (frame - hframes * hf)*32;
        int starty = hf * 32;

        gdk_draw_pixbuf(widget->window,
                        NULL,
                        pixbuf,
                        startx, starty,
                        0, 0, 32, 32,
                        GDK_RGB_DITHER_NONE,0,0);
    }
//-----------------------------------------
    void TileTool::sbChanged(){


       gdk_window_invalidate_rect (DA->window, &DA->allocation, FALSE);
       gdk_window_process_updates (DA->window, FALSE);
    }
//-------------------------------------------
    void TileTool::setupAdjustments(){

        int width = gdk_pixbuf_get_width (pixbuf);
        int height = gdk_pixbuf_get_height (pixbuf);
        gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), 0);
        GTK_ADJUSTMENT(adj)->lower = 0;
        GTK_ADJUSTMENT(adj)->page_size = (DA->allocation.width < width) ? DA->allocation.width : width;
        GTK_ADJUSTMENT(adj)->upper = width;

        gtk_adjustment_set_value(GTK_ADJUSTMENT(adjv), 0);
        GTK_ADJUSTMENT(adjv)->lower = 0;
        GTK_ADJUSTMENT(adjv)->page_size = (DA->allocation.height < height) ? DA->allocation.height : height;
        GTK_ADJUSTMENT(adjv)->upper = height;
    }

//-------------------------------------------
    void TileTool::loadPixbuf(gchar * s){


        gint h = 0;
        gint w = 0;
        gdk_pixbuf_get_file_info(s,&w, &h);
        printf("width:%d height:%d\n", w,h);

        pixbuf = gdk_pixbuf_new_from_file(s, NULL);
        gdk_window_invalidate_rect (DA->window, &DA->allocation, FALSE);
        gdk_window_process_updates (DA->window, FALSE);

        gdk_window_invalidate_rect (anim_DA->window, &anim_DA->allocation, FALSE);
        gdk_window_process_updates (anim_DA->window, FALSE);

        setupAdjustments();

    }



