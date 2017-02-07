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
#include "BatlleSelect.h"


#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL.h>
    #else
        #include <SDL/SDL.h>
    #endif
#else
  #include <SDL/SDL.h>
#endif

//================================================
void BattleSelect::selectAll(Squad& fighters,
                             unsigned herocount,
                             unsigned enemycount){

    for (unsigned i = 0; i < herocount; i++){
        layout.add(fighters.findFighter(HERO, i));
    }

    for (unsigned i = 0; i < enemycount; i++){
        layout.add(fighters.findFighter(MONSTER, i));
    }

}
//--------------------------------------------
//alive, initiator cannot be selected
void BattleSelect::selectOponentForAtack(Squad& fighters,
                                         unsigned herocount,
                                         unsigned enemycount,
                                         unsigned initiator){

    for (unsigned i = 0; i < herocount; i++){
        unsigned index = fighters.findFighter(HERO, i);
        if (index < fighters.count()){
            Fighter * f = 0;
            f = fighters.get(index);
            if (f){
                if ((f->hp > 0) && (f->nr != initiator))
                    layout.add(index);
            }
        }
    }

    layoutstate = layout.count();

    for (unsigned i = 0; i < enemycount; i++){
        unsigned index = fighters.findFighter(MONSTER, i);
        if (index < fighters.count()){
            Fighter * f = 0;
            f = fighters.get(index);
            if (f){
                if (f->hp > 0)
                    layout.add(index);
            }
        }

    }
}

//------------------------------------
void BattleSelect::set(Squad& fighters,
                       unsigned herocount, 
                       unsigned enemycount,
                       int heroStatX,
                       int enemyStatX,
                       SelectionTypes selectionType,
                       unsigned initiator){

    selected = false;
    canceled = false;

    _enemyStatX = enemyStatX;
    _heroStatX = heroStatX;

    layout.destroy();

    layoutstate = 0;

    switch(selectionType){
        case DAMAGE:{
            selectOponentForAtack(fighters, herocount, enemycount, initiator);
        }
        break;
        default:{
            
            selectAll(fighters, herocount, enemycount);
        }
    }


    state = layout[layoutstate];
    printf("laystate %u state %u\n", layoutstate, state);
    if (state >= (int)fighters.count())
        state = (int)fighters.count() - 1;
    printf("-state now %u\n", state);


    pressedkey = 0;

    Fighter * f = 0;
    f = fighters.get(state);
    if (f){
        pos.set(f->pos.x(), 3.3f,
                f->pos.z());

        if (f->type == 1)
            pos2D.set(_heroStatX, 36 + f->nr * 36, 0);
        else
            pos2D.set(_enemyStatX, 36 + f->nr * 36, 0);
    }

    printf("state now %u\n", state);
}

//-----------------------------------------
    void BattleSelect::getinput(int key, Squad& fighters){



        if (key != pressedkey) {


            state = layout[layoutstate];

            if (pressedkey == 13)
                selected = true;      //enter

            if (pressedkey == 27)
                canceled = true;

            switch(key){

                case SDLK_UP:{
                    if (layoutstate > 0) //up
                        layoutstate--;
                    else
                        layoutstate= layout.count() - 1;
                }
                break;

                case SDLK_DOWN:{

                    layoutstate++;
                    if (layoutstate >= layout.count()  ) //down
                        layoutstate= 0;

                }
                break;

                default: pressedkey = 0;
            }

            state = layout[layoutstate];
            //printf("%d\n",layoutstate);
            Fighter * f = 0;
            f = fighters.get(state);
            if (f){
                pos.set(f->pos.x(), 3.5f, f->pos.z());

                if (f->type == 1)
                    pos2D.set(_heroStatX, 36 + f->nr * 36, 0);
                else
                    pos2D.set(_enemyStatX, 36 + f->nr * 36, 0);
            }

            

            pressedkey = key;
        }



    }
 //-----------------------------------------
 void BattleSelect::draw3DPointer(SubsetCollection& pointer, 
                         bool outline){

    glTranslatef(pos.x(), pos.y(), pos.z());
    pointer.draw(0);

    if (outline)
        pointer.drawOutline(0);
 }

//-------------------------------------------
void BattleSelect::drawFlatPointer(PicsContainer& pics, int picindex){

    pics.draw(picindex, pos2D.v[0], pos2D.v[1], 0);

}

