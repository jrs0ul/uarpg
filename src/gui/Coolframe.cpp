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

#include "Coolframe.h"
#include "../Utils.h"


void Coolframe(int x, int y,
               unsigned w, unsigned h,
               PicsContainer& pics, int index){

    unsigned i;
    const int elementSize = 8;
    unsigned widthIterations = (unsigned)_round((w * 1.0f) /
                                                (elementSize * 1.0f));
    unsigned heightIterations = (unsigned)_round((h * 1.0f) / 
                                                (elementSize * 1.0f));
    
    for (i = 0; i < heightIterations; i++){
        pics.draw(index, 
                  x - elementSize / 2,
                  y + ((i + 1) * 8) - 8,
                  3);

        pics.draw(index,
                  x + w - elementSize / 2,
                  y + (( i + 1) * elementSize) - elementSize,
                  2);
    }

    //top
    for ( i = 1; i <= widthIterations; i++){
        pics.draw(index, 
                  x + (i * elementSize) - elementSize,
                  y - elementSize / 2,
                  0);
    //bottom
        pics.draw(index, 
                  x + (i * elementSize) - elementSize,
                  y + h - elementSize / 2,
                  1);
    }


    //Corners
    pics.draw(index, 
              x + w - elementSize / 2,
              y + h - elementSize / 2,
              4); 
    pics.draw(index, 
              x - elementSize / 2,
              y + h - elementSize / 2,
              5);
    pics.draw(index, 
              x - elementSize / 2,
              y - elementSize / 2,
              6);
    pics.draw(index,
              x + w - elementSize / 2,
              y - elementSize / 2,
              7);

}

