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
#include "TextScreen.h"
#include <cstring>
#include <cmath>
#include <cstdio>
#include "../Utils.h"
#include "Coolframe.h"
#include "Text.h"


    void TextScreen::init(unsigned newx, unsigned newy,
                          unsigned height_, unsigned width_){
        setpos(newx,newy);
        _height = height_;
        _width = width_;
        bufpy = 0;
        bufpx = 0;
        wait  = false;
        deactivate();
    }

//------------------------------------
    void TextScreen::putText(const char* s){

        int lineLen = 29;
        char * backup = (char *)malloc(strlen(s) + 1);
        strncpy(backup, s, strlen(s));
        backup[strlen(s)] = 0;
        char * word = 0;
        word = strtok(backup, " ");
        buffstring nbs;
        int index = 0;
        while (word){

            if (index + (int)strlen(word) > lineLen){
                nbs.s[index] = 0;
                buffer.add(nbs);
                index = 0;
                bufpy++;
            }

            for (int i = 0; i < (int)strlen(word); i++){
                nbs.s[index] = word[i];
                index ++;
                if (index > lineLen){
                    nbs.s[index] = 0;
                    buffer.add(nbs);
                    index = 0;
                    bufpy++;
                }

            }
            nbs.s[index] = ' ';
            index ++;
            if (index > lineLen){
                nbs.s[index] = 0;
                buffer.add(nbs);
                index = 0;
                bufpy++;
            }
            
            word = strtok(0, " ");
        }
        if (index > 0){
            nbs.s[index - 1] = 0;
            buffer.add(nbs);
            bufpy++;
        }else{
            int len = strlen(buffer[buffer.count()-1].s);

            buffer[buffer.count()-1].s[len - 1] = 0;
        }

        free(backup);


        if (!active())
            activate();
    }
//-------------------------------------
 void TextScreen::readFile(const char* path){
  FILE* f;

  f = fopen(path,"rt");
  if (f==0){
   char buf[31];
   sprintf(buf,"Cant't find %s %c",path,'#');
   putText(buf);
  }
  else {
   char s[50];
   char c=0;
   int i = 0;

   while (c!=EOF){
    while ((c!='\n')&&(c!=EOF)){
     c=fgetc(f);
     if (c!='\n')
      s[i]=c;
     i++;
    }
    if (c=='\n')
      c=' ';
    s[i-1]='\0';
    putText(s);
    strcpy(s,"");
    i=0;
   }
   fclose(f);
  }
 }
//--------------------------------
    void TextScreen::draw(PicsContainer& pics, int remelis, int font, int rod){

        pics.draw(-1, getX(), getY(),
                  0, false, 
                  _width, _height + 16, 0,
                  COLOR(0,0,0.5f,1), COLOR(0,0,0.0f,1));


        int talpa = _height/19;

        for (int i = 0; i <= talpa - 1;i++)
            WriteText(getX()+6, getY()+i*16,
                      pics, font, lines[i + (21 - talpa)]);


        char tmp[255];
        unsigned char i=0;

        for (i = 0; i < (strlen(lines[21]) - bufpx); i++){

            tmp[i] = lines[21][i];
        }
        tmp[i]='\0';

        if (bufpx>0)
            WriteText(getX()+6, getY() + talpa * 16,
                      pics, font, tmp);

        if (wait){
            if ((rand() % 100) % 10 == 0)
                pics.draw(rod, getX() + 6, getY() + _height - 2, 3, false);
            else
                pics.draw(rod, getX() + 6, getY() + _height - 8, 3, false);
        }
        Coolframe(getX(),getY(), _width, _height + 16,pics, remelis);
    }

//------------------------------------
    void TextScreen::type(unsigned int key){


        if ((bufpy==0)&&(bufpx==0)&&(!wait))
            deactivate();

        if ((bufpy)&&(bufpx==0)){

            strcpy(lines[21],buffer[0].s);
            bufpx=(int)strlen(lines[21]);

            buffer.remove(0);
            bufpy--;

        }

        if ((key != oldkey) && (oldkey != 0) &&
            ((oldkey == SDLK_RETURN)||(oldkey==SDLK_SPACE)||
             (oldkey==SDLK_ESCAPE))&&(wait))
            wait = false;

        oldkey = key;


        if ((bufpx)&&(!wait)){

            if (bufpx==1)
                if (lines[21][strlen(lines[21])-1]=='#')  {
                    lines[21][strlen(lines[21])-1]=' ';
                    wait=true;//paspaudimo tikrinimas
                }

            bufpx--;

            if (!bufpx){

                for (int i=0;i<21;i++)
                    strcpy(lines[i],lines[i+1]);    //pastumia texto ekrana
            }

        }

    }

//-----------------------------------
 void TextScreen::clear(){
  for (int i = 0; i < 21; i++)
   strcpy(lines[i]," ");
 }
//------------------------------------
