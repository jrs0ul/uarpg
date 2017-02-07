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
#ifndef INTERPRETER_H
#define INTERPRETER_H


#include "variables.h"
#include "DArray.h"

struct conststring{
    char s[255];
};

    struct Arguments{
        DArray<conststring> variables;
        unsigned count() {return variables.count();}
        Arguments();
        void clear();
        void getArguments(const char* cmdline, unsigned index,CVariableArray* vars);
    };



void GetCommand(const char* cmdline,char* result);
bool GetValue(const char* cmdline, unsigned char& index, const char* separators, char* result, char* separator);


#endif //INTERPRETER_H



