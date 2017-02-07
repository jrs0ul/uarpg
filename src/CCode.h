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
#ifndef CODE_H
#define CODE_H

#include "DArray.h"


struct CodeLine{
    int num;
    char* code;
    void init(int cnum, const char* kodas);
    void clear();
    
    CodeLine(){
        code = 0;
    }
    

};

class CCode{
      int  lineIndex;
      bool running;

  public:
      DArray<CodeLine> code;


                  CCode(){running = false; lineIndex = -1;}
      bool        isRunning() {return running;}
      void        runNew(){running = true; lineIndex = -1;}
      bool        nextLine();
      bool        goTo(unsigned long line);
      bool        load(const char* path);
      void        clear();
      void        stop(){running = false;}
      int         currentLineNum() {return lineIndex;}
      const char* currentLine(){return code[lineIndex].code;}
  };

#endif //CODE_H

