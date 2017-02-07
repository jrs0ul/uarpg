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

#ifndef _EVENTARRAY_H
#define _EVENTARRAY_H

#include "DArray.h"

    struct Event{
        int x,y;
        unsigned type;
        char code[255];

        Event(){
            x = 0;
            y = 0;
            type = 0;
            code[0] = '\0';
        }
    };

    //-----------------

    class EventArray{
    public:
        DArray<Event> entries;

        bool isSpecialEvent;
        Event specialEvent; //mapo pakrascio entry

        bool load(const char* name);
        void destroy();
        unsigned long count(){return entries.count();}
  };


#endif //_EVENTARRAY_H
