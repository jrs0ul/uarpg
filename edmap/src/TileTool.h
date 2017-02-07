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

#include "gtkUtils.h"

    class TileTool{

    public:
        GtkWidget *DA;
        GtkWidget *HSB;
        GtkWidget *VSB;
        GtkWidget *prevNext3DTile;
        GtkWidget *hbox;
        GtkObject *adj;
        GtkObject *adjv;

        GtkObject *adjpn;

        GtkWidget *MGEdit;

        GtkWidget *LayerSelect;


        GdkPixbuf *pixbuf;

        GtkWidget *dialog;

        GtkWidget * picker;


        GtkWidget *visualArea3D;
        GdkGLContext *GLContext;
        GdkGLDrawable *GLDrawable;
        //GtkWidget *next3DTile;
        //GtkWidget *prev3DTile;


        GtkWidget *moveCameraButton;
        GtkWidget *placeTilesButton;



        GtkWidget *anim_vbox;
        GtkWidget *anim_hbox;
        GtkWidget *anim_hbox2;
        GtkWidget *anim_DA;
        GtkWidget *anim_pixbuf;
        GtkWidget *anim_nextf;
        GtkWidget *anim_prevf;
        GtkWidget *anim_remf;
        GtkWidget *anim_addf;
        GtkWidget *anim_current;
        GtkWidget *anim_count;



        bool picktiles;
        bool draw3D;


        TileTool(){picktiles = false; draw3D = false;}


        void build(GdkGLContext * ContextToShare3D = NULL);

        void update(GtkWidget *widget);
        void update_anim(GtkWidget *widget, unsigned frame);

        void resize(GtkWidget *widget);

        int getAdjValue();

        void sbChanged();

        GtkWidget *get3DWidget(){return visualArea3D;}
        GdkGLContext  * getGLContext(){return GLContext;}
        GdkGLDrawable * getGLDrawable(){ return GLDrawable;}
        void initGLControl(GtkWidget* DA){
            GLContext = gtk_widget_get_gl_context(DA);
            GLDrawable = gtk_widget_get_gl_drawable(DA);
        }



        void setupAdjustments();

        void loadPixbuf(gchar * s);

        void picktoggle(){
            if (picktiles)
                picktiles = false;
            else
                picktiles = true;
        }


    };



//==========================================



#endif //TILE_TOOL_H
