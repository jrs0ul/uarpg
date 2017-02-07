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
#include "EventArray.h"
#include "Xml.h"
#include "Utils.h"

    void EventArray::destroy(){
        entries.destroy();
        isSpecialEvent = false;
    }

    //-----------------------------------------------------------
    bool EventArray::load(const char* name){

        Xml data;

        if (!data.load(name))
            return false;

        XmlNode * events = 0;
        events = data.root.getNode(L"Events");

        if (events){
            XmlNode * points = 0;
            points = events->getNode(L"Points");
            if (points){
                for (unsigned long i = 0; i < points->childrenCount(); i++){
                    Event newevent;
                    XmlNode * point = 0;
                    point = points->getNode(i);
                    if (point){
                        char buf[255];
                        wcstombs(buf, point->getAttribute(0)->getValue(),255);
                        newevent.x = atoi(buf);
                        wcstombs(buf, point->getAttribute(1)->getValue(),255);
                        newevent.y = atoi(buf);
                        wcstombs(buf, point->getAttribute(2)->getValue(),255);
                        newevent.type = (unsigned)atoi(buf);
                        XmlNode * script = 0;
                        script = point->getNode(L"Script");
                        if (script){
                            convertLT(script->getValue(), newevent.code, 255);
                        }
                    }
                    entries.add(newevent);
                }
            }
            XmlNode * border = 0;
            border = events->getNode(L"Border");
            if (border){
                XmlNode * script = 0;
                script = border->getNode(L"Script");
                if (script){
                    isSpecialEvent = true;
                    convertLT(script->getValue(), specialEvent.code, 255);
                }
            }
        }

       
        data.destroy();

        return true;

    }

