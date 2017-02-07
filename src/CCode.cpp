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
#include "CCode.h"
#include <cstring>
#include <cstdio>
#include <cwchar>
#include "Utils.h"

    void CodeLine::init(int cnum, const char *_code){
        num = cnum;
        if (_code){
            int len = (int)strlen(_code);
            if (len > 0){
                code = (char *)malloc(len + 1);
                strcpy(code, _code);
            }
        }
    }
    //-------------------------------
    void CodeLine::clear(){
        if (code){
            free(code);
            code = 0;
        }
    }
   //-------------------------------
    void strltrim(const char *in, char * out, int len){
       int index = 0;
        int inlen = (int)strlen(in);
        strcpy(out,"");
        while (((in[index]==' ') || (in[index]=='\t')) && (index < inlen)){
            index++;
        }
        if (inlen-index){
            memcpy(out, &in[index], inlen - index);
            out[inlen - index] = '\0';
        }

    }


  //========================================================
    bool CCode::load(const char *path){


        char * mbuf = 0;
        if (!ReadFileData(path, &mbuf))
            return false;

        //printf("strlen %d\n", (int)strlen(mbuf));

        wchar_t * buff = 0;
        buff = (wchar_t *)malloc((strlen(mbuf) + 1) * sizeof(wchar_t));
        //puts(mbuf);--ok
        UTF8toWchar(mbuf, buff);
        free(mbuf);


        if (buff){
            //TODO: do not limit lines to 255 characters
            char buf[255];
            char buf2[255];
            int count = 0;
            wchar_t * tmp = 0;
            wchar_t * ptmp = 0;

            //printf("|%ls|\n", buff);
            tmp = _wcstok(buff, L"\n\r", &ptmp);
            while (tmp) {

                CodeLine line;
                //printf("wcs |%ls|\n", tmp);
                convertLT(tmp, buf, 255);
                //wcstombs(buf, tmp, 255);
                //puts(buf);
                strltrim(buf, buf2, 255);

                count++;
                line.init(count, buf2);
                //puts(buf2);

                code.add(line);

                tmp = _wcstok(0, L"\n\r", &ptmp);
            }

            free(buff);
        }

        return true;
    }
//----------------------
    void CCode::clear(){

        for(unsigned long i = 0; i < code.count(); i++){
            code[i].clear();
        }
        code.destroy();

        running = false; lineIndex = -1;

  }
//---------------------------------
    bool CCode::nextLine(){

        if (lineIndex < (int)(code.count()-1)){
            //printf("%d %d\n",lineIndex, code.count());
            lineIndex++;
            return true;
        }


        return false;
    }
//----------------------------------
    bool CCode::goTo(unsigned long line){
        if (line - 2 > code.count())
            return false;

        lineIndex = line - 2 ;
        return true;

    }
