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
#include "SystemConfig.h"
#include <cwchar>
#include <cstdio>
#include "Xml.h"


    bool SystemConfig::load(const char * config){
        
        Xml conf;


        if (!conf.load(config))
            return false;

        XmlNode * settings = 0;
        settings = conf.root.getNode(L"Settings");
        if (settings){

            wchar_t tmp[255];
            char ctmp[255];

            XmlNode * nod = 0;
            nod = settings->getNode(L"width");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                width = atoi(ctmp);
            }
            nod = 0;

            nod = settings->getNode(L"height");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                height = atoi(ctmp);
            }
            nod = 0;

            nod = settings->getNode(L"fullscreen");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                int fs = 0;
                fs = atoi(ctmp);

                isWindowed = (!fs)? true : false;
            }
            nod = 0;

            nod = settings->getNode(L"limitfps");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                int in = 0;
                in = atoi(ctmp);
                limitFPS = (in) ? true : false;

            }
            nod = 0;

            
            nod = settings->getNode(L"usesound");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                int usesnd = 0;
                usesnd = atoi(ctmp);
                useSound = (usesnd)? true : false;
            }
            nod = 0;


            nod = settings->getNode(L"shadows");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                int useShad = 0;
                useShad = atoi(ctmp);
                useShadows = (useShad)? true : false;
            }
            nod = 0;

            nod = settings->getNode(L"drawoutline");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                int drawout = 0;
                drawout = atoi(ctmp);
                drawOutlines = (drawout)? true : false;
            }
            nod = 0;

            nod = settings->getNode(L"disableGLSL");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                int disGLSL = 0;
                disGLSL = atoi(ctmp);
                disableGLSL = (disGLSL)? true : false;
            }
            nod = 0;

            nod = settings->getNode(L"usepad");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                int usepad = 0;
                usepad = atoi(ctmp);
                useGamepad = (usepad)? true : false;
            }
            nod = 0;
            wcscpy(tmp, L"");

            nod = settings->getNode(L"sdevice");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                if (strlen(ctmp) > 3)
                    strcpy(customSoundDevice, ctmp);
                else
                    strcpy(customSoundDevice, "");
                printf("snd device [%s]\n", ctmp);
            }
            nod =  0;

            nod = settings->getNode(L"basedir");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                strcpy(basePath, ctmp);

                printf("basepath [%s]\n", ctmp);

                FILE * test = 0;
                char path[255];
                sprintf(path, "%sscripts/begining.gsc", basePath);
                test = fopen(path, "rt");
                if (test){
                    fclose(test);
                }
                else{
                    strcpy(basePath, "base/");
                }

            }
            nod = 0;
        }

        conf.destroy();

        return true;
    }
//------------------------------------------
    bool SystemConfig::write(const char * config){

        char buf[255];
        wchar_t wbuf[255];
        Xml conf;

        XmlNode Settings;
        Settings.setName(L"Settings");
        Settings.setValue(L"\n");

        XmlNode _width;
        sprintf(buf, "%d", width);
        mbstowcs(wbuf, buf, 255);
        _width.setName(L"width");
        _width.setValue(wbuf);
        Settings.addChild(_width);

        XmlNode _height;
        sprintf(buf, "%d", height);
        mbstowcs(wbuf, buf, 255);
        _height.setName(L"height");
        _height.setValue(wbuf);
        Settings.addChild(_height);

        XmlNode fullscreen;
        fullscreen.setName(L"fullscreen");
        if (isWindowed)
            fullscreen.setValue(L"0");
        else
            fullscreen.setValue(L"1");
        Settings.addChild(fullscreen);

        XmlNode limitfps;
        limitfps.setName(L"limitfps");
        if (limitFPS)
            limitfps.setValue(L"1");
        else
            limitfps.setValue(L"0");
        Settings.addChild(limitfps);

        XmlNode usesound;
        usesound.setName(L"usesound");
        if (useSound)
            usesound.setValue(L"1");
        else
            usesound.setValue(L"0");
        Settings.addChild(usesound);

        XmlNode usepad;
        usepad.setName(L"usepad");
        if (useGamepad)
            usepad.setValue(L"1");
        else
            usepad.setValue(L"0");
        Settings.addChild(usepad);

        XmlNode disGLSL;
        disGLSL.setName(L"disableGLSL");
        if (disableGLSL)
            disGLSL.setValue(L"1");
        else
            disGLSL.setValue(L"0");
        Settings.addChild(disGLSL);


        XmlNode drawout;
        drawout.setName(L"drawoutline");
        if (drawOutlines)
            drawout.setValue(L"1");
        else
            drawout.setValue(L"0");
        Settings.addChild(drawout);


        XmlNode sdevice;
        sdevice.setName(L"sdevice");
        if (strlen(customSoundDevice) < 3)
            sdevice.setValue(L" ");
        else{
            mbstowcs(wbuf, customSoundDevice, 255);
            sdevice.setValue(wbuf);
        }
        Settings.addChild(sdevice);

        XmlNode basedir;
        basedir.setName(L"basedir");
        mbstowcs(wbuf, basePath, 255);
        basedir.setValue(wbuf);
        Settings.addChild(basedir);

        XmlNode shadows;
        shadows.setName(L"shadows");
        if (useShadows)
            shadows.setValue(L"1");
        else
            shadows.setValue(L"0");
        Settings.addChild(shadows);


       conf.root.addChild(Settings);


        if (!conf.write(config)){
            conf.destroy();
            return false;
        
        }
        puts("destroying");
        conf.destroy();
        puts("done");


        return true;
    }

