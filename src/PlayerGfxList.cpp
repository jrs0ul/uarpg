
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
#include "Xml.h"
#include "PlayerGfxList.h"



bool PlayerGfxList::load(const char * filename, RaceList& rl, KlassList& kl){


    Xml data;

    if (!data.load(filename))
        return false;

    XmlNode * n = 0;

    n = data.root.getNode(L"Gfx");


    if (n){

        printf("node count %lu\n", n->childrenCount());

        unsigned long index = 0;
        for (unsigned i = 0; i < rl.count(); i++){

            if (rl.get(i)->isPlayable){

                Classes ncls;

                for (unsigned a = 0; a < kl.count(); a++){

                    if (kl.get(a)->isPlayable){

                        Sexes nsx;
                        for (unsigned b = 0; b < rl.get(i)->maxsex; b++){
                            ResourseNames ns;

                            XmlNode * blob = 0;
                            blob = n->getNode(index);

                            if (blob){
                                XmlNode * txt = 0;
                                txt = blob->getNode(L"Model");
                                if (txt)
                                    wcstombs(ns.m,
                                             txt->getValue(),
                                             254); //model
                                txt = 0;
                                txt = blob->getNode(L"Texture");
                                if (txt)
                                    wcstombs(ns.t,
                                             txt->getValue(),
                                             254); //model texture
                                txt = 0;
                                txt = blob->getNode(L"Sprite");
                                if (txt)
                                    wcstombs(ns.s,
                                             txt->getValue(),
                                             255); //2d sprite in map
                                txt = 0;
                                txt = blob->getNode(L"Face");
                                if (txt)
                                    wcstombs(ns.f,
                                             txt->getValue(),
                                             255); //heros face picture set
                                txt = 0;
                                txt = blob->getNode(L"Radius");
                                if (txt){
                                    char buf[255];
                                    wcstombs(buf,
                                             txt->getValue(),
                                             255);
                                    ns.radius = atof(buf);

                                }

                                nsx.sex.add(ns);
                                puts("ok");
                            }

                            index ++;

                        }
                        ncls.clas.add(nsx);
                    }

                }

                races.add(ncls);
            }

        }



    }

    puts("destroying");
    data.destroy();
    puts("everything ok");

    return true;

}


//--------------------------------------------------------

void PlayerGfxList::destroy(){

    for (unsigned i = 0; i < races.count(); i++){
        for (unsigned a = 0; a < races[i].clas.count(); a++){
            races[i].clas[a].sex.destroy();
        }
        races[i].clas.destroy();
    }

    races.destroy();
}
