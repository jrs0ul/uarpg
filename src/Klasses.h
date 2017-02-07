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
#ifndef KLASSES_H
#define KLASSES_H

#include "DArray.h"




struct XKlass{

   char name[50]; //class name

   int hp  ;
   int mp  ;
   int atck;
   int dfns;
   int agil;
   int sprt;
   int magp;

   bool isPlayable;         //is class playable ?

   DArray<unsigned> spells; //indexes of available spells

   void destroy(){spells.destroy();}
};

//-------------------------------------

class KlassList{

   DArray<XKlass> jobs;

public:

   bool          load(const char* path);
   void          destroy();
   unsigned long count(){return jobs.count();}
   XKlass *      get(unsigned long index);
};




#endif //KLASSES_H

