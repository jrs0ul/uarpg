/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul   *
 *   jrs0ul@gmail.com   *
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
#ifndef CREDITSDIALOG_H
#define CREDITSDIALOG_H

#include "BasicUiControl.h"
#include "../TextureLoader.h"

class CreditsDialog: public BasicControl{

    unsigned      pressedkey;
    char * data;

public:
    int           width;
    int           height;

    //loads text from file
    bool          load(const char* path);
    
    void          draw(PicsContainer &pics, unsigned long frm,
                       unsigned long font);
    void          getInput(unsigned key);

    CreditsDialog(){
        data = 0;
    }

    void reset(){
        if (data){
            free(data);
            data = 0;
        }
        pressedkey = 0;
    }

};

#endif //CREDITSDIALOG_H
