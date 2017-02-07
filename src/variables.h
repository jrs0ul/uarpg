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
#ifndef VARIABLES_H
#define VARIABLES_H

#include <cstring>
#include "DArray.h"

class CVariable{
 public:
   char name[256];
   char value[256];

   CVariable(const char* newname, const char* newvalue);

  };

//================================================

class CVariableArray{

    DArray<CVariable> variables;

    long findByName(const char* name);

public:

 CVariableArray(){/*head=0;*/}
 void add(CVariable& newvar);
 char * getValue(const char* name);
 void remove(const char* name);
 void destroy();
 unsigned long count(){return variables.count();}
 char * name(unsigned long index){return variables[index].name;}
 char * value(unsigned long index){return variables[index].value;}
 //void toarray(char**& names,char**& values,int& count);
};



#endif //VARIABLES_H

