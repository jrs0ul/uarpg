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

#ifndef MAPHEADERDLG_H
#define MAPHEADERDLG_H

#include <gtk/gtk.h>
#include "../../src/Worldmap.h"

    class MapHeadDlg{
        
        GtkWidget *eTileset;
        GtkWidget *lTileset;
        GtkWidget *eMusic;
        GtkWidget *lMusic;

        GtkWidget *eCHeight;
        GtkWidget *lCHeight;
        GtkWidget *eCDistance;
        GtkWidget *lCDistance;
        GtkWidget *eCPitch;
        GtkWidget *lCPitch;

        void buildEntryWithTitle(const char* title,
                                 GtkWidget** entry,
                                 GtkWidget** label,
                                 GtkWidget** container);

        
    public:    
        GtkWidget *ok_button;
        GtkWidget *dialog;
        GtkWidget *is3D;
    
        void  build();
        void  show();
        void  hide();
        void  toggle3D(Map &map);
        void  getTileset(char* tileset);
        void  getMusic(char* music);

        float getCamPitch();
        float getCamHeight();
        float getCamDistance();

        void  setCamPitch(float pitch);
        void  setCamHeight(float height);
        void  setCamDistance(float dist);
        
        void  setTileset(const char* tileset);
        void  setMusic(const char* music);
    };
    
      


#endif //MAPHEADERDLG_H
