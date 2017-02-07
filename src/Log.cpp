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
#include <ctime>
#include <cstdio>
#include <cstring>


#include "Log.h"

    void Log::write(const char * rant){
        FILE * log;

        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        puts(rant);
        log = fopen(filename, "at+");

        if (log){

            char buf[100];
            //sprintf(buf,"%s", asctime(timeinfo));
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);

            buf[strlen(buf)-1]=0;
            fprintf(log,"%s||", buf);
            fprintf(log, "%s\n", rant);
            fclose(log);
        }

    }
    //-------------------------
    void Log::create(const char* _filename){
        FILE* log;

        log = fopen(_filename, "wt+");
        strncpy(filename, _filename, 214);
        fclose(log);

    }
