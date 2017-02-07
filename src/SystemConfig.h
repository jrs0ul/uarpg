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
#ifndef _SYSTEMCONFIG_H
#define _SYSTEMCONFIG_H

#include <cstring>

    class SystemConfig{


    public:
        int width;
        int height;
        unsigned bits;
        bool useSound;
        bool isWindowed;
        bool limitFPS;
        bool useGamepad;
        char customSoundDevice[255];
        bool quitApp;
        bool reloadApp;
        bool gamePaused;
        bool showDebugText;
        bool drawOutlines;
        char basePath[255];
        bool hasGLSL;
        bool hasFBO;
        bool useShadows;
        bool disableGLSL;
        bool needReboot;


        SystemConfig(){

            width = 640;
            height = 480;
            bits = 32;
            quitApp = false;
            reloadApp = true;
            useSound = true;
            limitFPS = false;
            strcpy(customSoundDevice, "");
            strcpy(basePath, "base/");
            isWindowed = true;
            gamePaused = false;
            showDebugText = false;
            useGamepad = false;
            drawOutlines = true;
            hasGLSL = false;
            hasFBO = false;
            useShadows = true;
            disableGLSL = false;
            needReboot = false;

        }


        bool load(const char * config);
        bool write(const char * config);

    };



#endif //SYSTEMCONFIG_H
