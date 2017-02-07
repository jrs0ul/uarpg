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
#include "Gui.h"
#include "Text.h"
#include "../Utils.h"
#include "Coolframe.h"
#include <cstdio>
#include <cmath>


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
//TFaceSelect metodu realizacija
//--------------------------------------------
    void FaceSelect::init(unsigned int dx,unsigned int dy, bool skipd, Party& party){

        setpos(dx,dy);
        skipdead = skipd;
        selected = false;
        canceled = false;
        unsigned long i = 0;

        if (skipdead){
            if (party.count()){
                while ((party.member(i)->state == DEAD) && ( i + 1 < party.count() ))
                    i++;
            }
        }
        face = i;
        pressedkey = 0;
    }
//--------------------------------------------
    void FaceSelect::getInput(unsigned key, Party& party){

        if (key!=pressedkey) {

            if (pressedkey==13) selected=true;
            if (pressedkey==27) canceled=true;

            unsigned char i=0;

            switch(key){


                case SDLK_LEFT:{
                    if (skipdead){
                        i = (face == 0) ? party.count() - 1 : face-1;

                        while (party.member(i)->state == DEAD)
                            i = (i==0) ? party.count() - 1 : i - 1;

                        face = i;
                    }
                    else
                        face = (face==0) ? party.count() - 1 : face - 1 ;

                }
                break;

                case SDLK_RIGHT:{
                    if (skipdead) {
                        i =  (face == party.count() - 1) ? i = 0 : face + 1;

                        while (party.member(i)->state == DEAD)
                            i = (i==party.count() - 1) ? 0 : i + 1;

                        face = i;
                    }
                    else
                        face = (face==party.count() - 1) ? 0 : face + 1;

                }

                break;

                default:   pressedkey = 0;
            }
            pressedkey = key;
        }

    }

//--------------------------------------------
void FaceSelect::draw(PicsContainer &pics, int pic){
    //int jump=70;
    int frame=0;

  if ((rand()%10)==1)
    frame=0;
  else
    frame=1;

    pics.draw(pic, 12 + getX() + 70 * face, getY()+16,frame);

}
//----------------------------------------------
    void FaceSelect::reset(Party& party){

        selected = false;
        canceled = false;
        int i = 0;

        if (skipdead)
        while (party.member(i)->state == DEAD)
            i++;
        face = i;
    }

//================================================
void CreatureInfo(int x, int y,
                       Creature& crtr,
                       RaceList& racelist, 
                       ItemData& items,
                       KlassList& klases,
                       SMenu sexlist, //WTF
                       int pic, int font,
                       PicsContainer& pics){


        char klase[50];
        unsigned char money;

        strncpy(klase, klases.get(crtr.klass)->name, 50);


        pics.draw(-1, x, y, 0, false, 400,260, 0,
                  COLOR(0,0,1.0f), COLOR(0,0,0.0f));

        char buf[100];
        sprintf(buf,"Name: %s",crtr.name);
        WriteText(x + 7, y + 5, pics, font,buf);
        sprintf(buf, "Race: %s", racelist.get(crtr.race)->name);
        WriteText(x+7,y+25, pics,font,buf);
        sprintf(buf,"Sex: %s",sexlist.items[crtr.sex].name);
        WriteText(x+7,y+40, pics,font,buf);
        sprintf(buf,"Class: %s",klase);
        WriteText(x+7,y+55, pics, font,buf);
        sprintf(buf,"HP: %d/%d",crtr.hp.def,crtr.hp.max);
        WriteText(x+7,y+75, pics,font,buf);
        sprintf(buf,"MP: %d/%d",crtr.mp.def,crtr.mp.max);
        WriteText(x+7,y+90, pics,font,buf);
        sprintf(buf,"Atack: %d",crtr.sumAtack(items));
        WriteText(x+7,y+110, pics,font,buf);
        sprintf(buf,"Defense: %d",crtr.sumDefense(items));
        WriteText(x+7,y+125, pics,font,buf);
        sprintf(buf,"Agility: %d",crtr.sumAgil(items));
        WriteText(x+7,y+140, pics,font,buf);
        sprintf(buf,"Spirit: %d",crtr.sprt);
        WriteText(x+7,y+155, pics,font,buf);
        sprintf(buf,"Magpower: %d",crtr.magp);
        WriteText(x+7,y+170, pics,font,buf);


        money=crtr.inventory.getItemCount(11);
        sprintf(buf,"Money: %d",money);
        WriteText(x+7,y+190, pics,font,buf);
        sprintf(buf,"Exp: %d",crtr.exp);
        WriteText(x+7,y+205, pics,font,buf);

        sprintf(buf,"Lev: %d",crtr.level());
        WriteText(x+7,y+220, pics,font,buf);
        sprintf(buf,"Exp to lev %d: %d",crtr.level()+1,crtr.needexp());
        WriteText(x+7,y+235, pics,font,buf);

        WriteText(x+230,y+5, pics,font,"Right hand:");
        if (crtr.rhand.kind!=-1){
            sprintf(buf, "%s %d%c", 
                    items.info[crtr.rhand.kind].name,
                    crtr.rhand.health(),'%');
            WriteText(x+230,y+20, pics,font,buf);
        }
        else
            WriteText(x+230,y+20, pics,font,"--empty--");

        WriteText(x+230,y+40, pics,font,"Left hand:");
        if (crtr.lhand.kind!=-1){
            sprintf(buf, "%s %d%c",
                    items.info[crtr.lhand.kind].name,
                    crtr.lhand.health(), '%');
            WriteText(x+230,y+55, pics,font,buf);
        }
        else
            WriteText(x+230,y+55, pics,font,"--empty--");
        WriteText(x+230,y+80, pics,font,"Armor: ");

        if (crtr.armor.kind!=-1){
            sprintf(buf, "%s %d%c", 
                    items.info[crtr.armor.kind].name, 
                    crtr.armor.health(),'%');
            WriteText(x+230,y+95, pics,font,buf);
        }
        else
            WriteText(x+230,y+95, pics, font,"--empty--");

        sprintf(buf,"Bag weight: %d",crtr.inventory.itemWeight(items));
        WriteText(x+230,y+130, pics, font,buf);
        sprintf(buf,"Can carry: %d",crtr.stamina());
        WriteText(x+230,y+150, pics,font,buf);

    Coolframe(x, y, 400, 260, pics, pic);
}

//---------------------------------------------------------

    void FightersStatus(unsigned x,
                        unsigned y,
                        Party& partis, bool showstats,
                        PicsContainer & pics, int remelis,
                        int fontas, int icons){

        int maxBarWidth = 70;
        for (unsigned long i = 0; i < partis.count(); i++){

            Creature * m = partis.member(i);

            WriteText(x + 10 + 1, 
                      y + 15 + 36 * i + 1, pics, fontas, m->name, 
                      1.0, 1, COLOR(0, 0, 0, 1.0f));

            WriteText(x+10, y+15+36*i, pics, fontas, m->name);

            char buf[50];

            //HP bar
            int posx = 30;

            if ((showstats)||(m->known))
                sprintf(buf,"%d/%d", m->hp.def, m->hp.max);
            else
                strcpy(buf,"?/?");

            pics.draw(icons, x+ posx - 20, y +34+36*i, 3);
            pics.draw( -1, x + posx, y +35+36*i, 3, false, maxBarWidth, 16, 0,
                      COLOR(1,0,0,0.4f), COLOR(1,0,0,0.4f));
            
            unsigned barWidth = m->hp.max ? 
                                (maxBarWidth/100.0f) 
                                * (m->hp.def / (m->hp.max / 100.0f)) : 0;
            unsigned textWidth = strlen(buf)*10;
            unsigned textpos = maxBarWidth/2 - textWidth / 2;

            if ((showstats) || (m->known))
                pics.draw( -1, x + posx, y +35+(36*i), 3, false,
                           m->hp.max ? 0.6f * (m->hp.def / (m->hp.max / 100.0f)) : 0,
                           16, 0, 
                           COLOR(1,0,0,0.8f), COLOR(1,0,0,0.8f));

            WriteText(x + posx + textpos + 1, y +37+(36*i)+1,
                      pics,fontas, buf, 0.8f,0.8f,
                      COLOR(0.0f, 0.0f, 0.0f, 1.0f));
            WriteText(x + posx + textpos , y+37+(36*i), 
                      pics, fontas, buf, 0.8f, 0.8f,
                      COLOR(1.0f, 1.0f, 1.0f, 1.0f));


            //MP bar
            

            posx = 125;

            if ((showstats) || (m->known))
                sprintf(buf,"%d/%d", m->mp.def, m->mp.max);
            else
                strcpy(buf,"?/?");

            pics.draw(icons, x + posx - 20, y + 34 +(36 * i), 4);

            pics.draw( -1, x + posx, y + 35 + (36 * i), 3, false,
                      maxBarWidth, 16, 0,
                      COLOR(0, 0, 1, 0.4f), COLOR(0, 0, 1, 0.4f));


            barWidth = m->mp.max ? 
                                (maxBarWidth/100.0f)
                                * (m->mp.def / (m->mp.max / 100.0f)) : 0;
            textWidth = strlen(buf)*10;
            textpos = maxBarWidth/2 - textWidth / 2;
            if ((showstats) || (m->known))
                pics.draw( -1, x + posx, y + 35 + (36 * i), 3, false,
                          barWidth,
                          16, 0,
                          COLOR(0, 0, 1, 0.8f), COLOR(0, 0, 1, 0.8f));
            WriteText(x + posx + textpos + 1, 
                      y + 37 + (36 * i) + 1,
                      pics, fontas, buf, 0.8f,0.8f,
                      COLOR(0.0f, 0.0f, 0.0f, 1.0f));
            WriteText(x + posx + textpos,
                      y + 37 + (36 * i),
                      pics, fontas, buf, 0.8f, 0.8f,
                      COLOR(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }


//**********************************************************



