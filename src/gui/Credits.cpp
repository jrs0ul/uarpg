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
#include "Credits.h"
#include "Coolframe.h"
#include "Text.h"
#include "../Utils.h"

void CreditsDialog::getInput(unsigned key){
    
    if (key != pressedkey){
        if (pressedkey == 27) {
            deactivate();      //esc
        }
        else
        if (pressedkey == 13){
            deactivate();
        }

        pressedkey = key;
    }

}
//--------------------------------------------
bool CreditsDialog::load(const char* path){

    return ReadFileData(path, &data);
}
//--------------------------------------------
void CreditsDialog::draw(PicsContainer &pics, unsigned long frm,
                       unsigned long font){

    pics.draw(-1, getX(), getY(), 0, false, width, height, 0,
              COLOR(0.0f ,0.0f, 1.0f, 0.8f), 
              COLOR(0.0f, 0.0f, 0.0f, 0.5f));

    if (data){
        char * backup = (char * )malloc(strlen(data)+1);
        strcpy(backup, data);
    
        char * s = 0;
        s = strtok(backup, "\n");

        int index = 0;
        while (s){
            WriteText(getX() + 10, getY() + 10 + index * 20, pics, font, s,
                      1.0f, 1.0f, COLOR(1.0f, 1.0f, 1.0f, 1.0f));
            s = strtok(0, "\n");
            index ++;
        }

        free(backup);
    }

    Coolframe(getX(), getY(), width, height, pics, frm);

}

