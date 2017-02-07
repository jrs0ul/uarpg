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

#include "variables.h"


    CVariable::CVariable(const char *newname, const char *newvalue){

        strncpy(name,newname, 255);
        strncpy(value,newvalue, 255);
    }


//------------------------------------
    long  CVariableArray::findByName(const char* name){
        long index = 0;
        while ((index < (long)variables.count()) && (strcmp(name, variables[index].name) != 0))
            index ++;
        if (index < (long)variables.count())
            return index;
        return -1;
    }
//-----------------------------------------------
    void CVariableArray::add(CVariable& newvar){

        long target = -1;

        if (variables.count() == 0)
            variables.add(newvar);
        else{
            target = findByName(newvar.name);
            if (target > -1){
                strncpy(variables[(unsigned long)target].value, newvar.value, 255);
            }
            else{
                variables.add(newvar);
            }
        }
    }


//------------------------------------
    char* CVariableArray::getValue(const char* name){

        long index = -1;
        index = findByName(name);
        if (index > -1)
            return variables[(unsigned long)index].value;

        return 0;
    }
//---------------------------------------------
    void CVariableArray::remove(const char* name){

        long index = -1;
        index = findByName(name);
        if (index > -1){
            variables.remove((unsigned long)index);
        }

    }



//---------------------------

void CVariableArray::destroy(){

    printf ("variables count = %lu\n",variables.count());
    for (unsigned long i = 0; i < variables.count(); i++)
        puts(variables[i].name);

	variables.destroy();
}


