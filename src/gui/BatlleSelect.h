/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul                                          *
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
#ifndef BATLLE_SELECT_H
#define BATLLE_SELECT_H


#include "BasicUiControl.h"
#include "../TextureLoader.h"
#include "../models/rm2.h"
#include "../Combat.h"


enum SelectionTypes {HEALING, DAMAGE};
//-------------------------------------------
//hand&arrow which selects oponent in battle
class BattleSelect: public ActiveControl{
        Vector3D pos;
        Vector3D pos2D;
        DArray<int> layout;
        unsigned layoutstate;
        int _enemyStatX;
        int _heroStatX;

        void selectAll(Squad& fighters,
                       unsigned herocount,
                       unsigned enemycount);
        //alive, initiator cannot be selected
        void selectOponentForAtack(Squad& fighters,
                       unsigned herocount,
                       unsigned enemycount,
                       unsigned initiator);

    public:
        int  state;
        int  pressedkey;
        int  defstate;
        bool selected;
        bool canceled;
        int  enemycount;

        void set(Squad& fighters,
                 unsigned herocount, unsigned enemycount,
                 int heroStatX, int enemyStatX,
                 SelectionTypes selectionType = HEALING,
                 unsigned initiator = 0);
        void getinput(int key, Squad& fighters);
        void draw3DPointer(SubsetCollection& pointer,
                           bool outline = true);

        void drawFlatPointer(PicsContainer& pics, int picindex);


        void destroy(){layout.destroy();}
};

#endif //BATLLE_SELECT_H
