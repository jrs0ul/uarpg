/***************************************************************************
 *
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
#include "StatGain.h"
#include "Coolframe.h"
#include "Text.h"


void StatsGainWindow::draw(Party & party, PicsContainer& pics, int font, int frame){

    pics.draw(-1, getX()*1.0f, getY()*1.0f, 0, false, _width*1.0f, _height*1.0f,
               0, COLOR(0.0f, 0.0f, 1.0f), COLOR(0.0f, 0.0f, 0.0f));

    
    for (unsigned i = 0; i < party.count(); i++){
        pics.draw(pics.findByName(party.member(i)->faceTileSet),
                               getX()+10, getY()+30+i*80, party.member(i)->faceID);
        WriteText(getX()+10, getY()+10+i*80, pics, font, party.member(i)->name);

        char buf[255];

        switch(state){

            case 0 :{

                sprintf(buf,"%d EXP", stats[i].oldEXP);
                WriteText(getX()+100, getY()+10+i*80, pics, font, buf);
                if (stats[i].gainEXP){
                    sprintf(buf,"+%d", stats[i].gainEXP);
                    WriteText(getX()+200, getY()+10+i*80, pics, font, buf, 
                              1.0f, 1.0f, COLOR(1.0f, 1.0f, 0.0f, 1.0f));
                }

                if (stats[i].oldLevel != stats[i].newLevel){
                    sprintf(buf,"Level %d ->", stats[i].oldLevel);
                    WriteText(getX()+100, getY()+30+i*80, pics, font, buf);
                    sprintf(buf,"Level %d", stats[i].newLevel);
                    WriteText(getX()+220, getY()+30+i*80, pics, font, buf, 
                              1.0f, 1.0f, COLOR(1.0f, 1.0f, 0.0f, 1.0f));
                    sprintf(buf,"hp +%d mp +%d atck +%d def +%d", 
                            stats[i].hpUp, stats[i].mpUp,
                            stats[i].atckUp, stats[i].defUp);
                    WriteText(getX()+100, getY()+50+i*80, pics, font, buf, 
                              0.9f, 0.9f, COLOR(1.0f, 1.0f, 0.0f, 1.0f));
                    sprintf(buf,"agil +%d magp +%d sprt +%d" ,
                            stats[i].agilUp, stats[i].magpUp, stats[i].sprtUp);
                    WriteText(getX()+100, getY()+65+i*80, pics, font, buf, 
                              0.9f, 0.9f, COLOR(1.0f, 1.0f, 0.0f, 1.0f));

                    if (stats[i].newspells){
                        sprintf(buf,"+%d spells!", stats[i].newspells);
                        WriteText(getX()+290, getY()+10+i*80, pics, font, buf, 
                                  0.9f, 0.9f, COLOR(1.0f, 1.0f, 0.0f, 1.0f));
                    }
                }
                else{
                    sprintf(buf,"Level %d", stats[i].oldLevel);
                    WriteText(getX()+100, getY()+30+i*80, pics, font, buf);
                }
            } break;
            case 1:{
                sprintf(buf,"%d EXP", stats[i].oldEXP + stats[i].gainEXP);
                WriteText(getX()+100, getY()+10+i*80, pics, font, buf);
                sprintf(buf,"Level %d", stats[i].newLevel);
                WriteText(getX()+100, getY()+30+i*80, pics, font, buf);

            }
        }
    }
    Coolframe(getX(), getY(), (unsigned)width(), (unsigned)height(), pics, frame);
};
//------------------------
void StatsGainWindow::getInput(unsigned key){
    if (pressedkey != key){
        if ((pressedkey == 13) ||
             (pressedkey == 27)){

            if (active()){
                switch(state){
                    case 0: {state = 1;
                             puts("STATE 1");} break;
                    case 1: {
                                deactivate();
                                destroy();
                                state = 0;
                                puts("STATE 0");
                            } 
                }
            }
        }
            

        pressedkey = key;
    }
}
