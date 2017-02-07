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
#ifndef GUI_H
#define GUI_H


#include "BasicUiControl.h"
#include "../Party.h"
#include "../TextureLoader.h"
#include "MenuStruct.h" //structure of menu data



//====================================================
 class FaceSelect:public BasicControl{
 public:
  bool selected;
  bool canceled;
  bool skipdead; //selector skips dead members
  unsigned char face;
  unsigned pressedkey;
  void init(unsigned int dx,unsigned int dy, bool skipd, Party& party);
  void getInput(unsigned key, Party& party);
  void draw(PicsContainer &pics, int pic);
  void reset(Party& party);
 };

//====================================================
//displays information about hero
void CreatureInfo(int x, int y,
                  Creature& crtr,
                  RaceList& racelist,
                  ItemData& items,
                  KlassList& klases,
                  SMenu sexlist,
                  int pic, int font,
                  PicsContainer& pics
                 );

void Fade(float r, float g, float b,unsigned width, unsigned height);

void FightersStatus(unsigned x, unsigned y, Party& partis, bool showstats,
                    PicsContainer & pics, int remelis, int fontas,
                    int icons);
//********************************************




#endif // GUI_H


