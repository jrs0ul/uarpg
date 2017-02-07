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
#include "Races.h"
#include "Utils.h"

    //------------------------------------------------
    unsigned RaceList::load(const char* path){

        Xml racefile;
        if (!racefile.load(path))
            return 1;

        puts("let's rock");
        XmlNode * n = 0;
        //printf("total nodes %d\n", racefile.root.childrenCount());
        n = racefile.root.getNode(L"RaceList");
        if (n){
            printf("total races %lu\n", n->childrenCount());
            for (unsigned long i = 0; i < n->childrenCount(); i++){
                Xrace newRace;
                char tmpnum[30];

                XmlNode * r = 0;
                r = n->getNode(i);
                if (r){
                    convertLT(r->getNode(L"Name")->getValue(), newRace.name, 12);
                    unsigned playable = 0;
                    wcstombs(tmpnum, r->getNode(L"Playable")->getValue(), 30);
                    playable = (unsigned)atoi(tmpnum);
                    newRace.isPlayable = (playable == 0) ? false:true;
                    wcstombs(tmpnum, r->getNode(L"EvolvSpeed")->getValue(), 30);
                    newRace.espeed = atoi(tmpnum);

                    XmlNode * res = 0;
                    res = r->getNode(L"Resistance");
                    if (res){
                        wcstombs(tmpnum, res->getNode(L"Fire")->getValue(), 30);
                        newRace.resistense.fire = atoi(tmpnum);
                        wcstombs(tmpnum, res->getNode(L"Ice")->getValue(), 30);
                        newRace.resistense.ice = atoi(tmpnum);
                        wcstombs(tmpnum, res->getNode(L"Bolt")->getValue(), 30);
                        newRace.resistense.bolt = atoi(tmpnum);
                        wcstombs(tmpnum, res->getNode(L"Poison")->getValue(), 30);
                        newRace.resistense.poison = atoi(tmpnum);
                    }
                    XmlNode * sexlist = 0;
                    sexlist = r->getNode(L"Sexes");

                    if (sexlist){
                        newRace.maxsex = sexlist->childrenCount();
                        for (unsigned long a = 0; a < newRace.maxsex; a++){
                            XmlNode * s = 0;
                            s = sexlist->getNode(L"Sex");
                            if (s){
                                XmlNode * bonus = 0;
                                bonus = s->getNode(L"BonusPoints");
                                if (bonus){
                                    wcstombs(tmpnum, bonus->getNode(L"hp")->getValue(), 30);
                                    newRace.bonusp[a].hp  = (unsigned)atoi(tmpnum);
                                    wcstombs(tmpnum, bonus->getNode(L"mp")->getValue(), 30);
                                    newRace.bonusp[a].mp = (unsigned)atoi(tmpnum);
                                    wcstombs(tmpnum, bonus->getNode(L"attack")->getValue(), 30);
                                    newRace.bonusp[a].atck = (unsigned)atoi(tmpnum);
                                    wcstombs(tmpnum, bonus->getNode(L"deffense")->getValue(), 30);
                                    newRace.bonusp[a].dfns = (unsigned)atoi(tmpnum);
                                    wcstombs(tmpnum, bonus->getNode(L"agility")->getValue(), 30);
                                    newRace.bonusp[a].agil = (unsigned)atoi(tmpnum);
                                    wcstombs(tmpnum, bonus->getNode(L"magpower")->getValue(), 30);
                                    newRace.bonusp[a].magp = (unsigned)atoi(tmpnum);
                                    wcstombs(tmpnum, bonus->getNode(L"spirit")->getValue(), 30);
                                    newRace.bonusp[a].sprt = (unsigned)atoi(tmpnum);
                                }

                                XmlNode * startp = 0;
                                startp = s->getNode(L"StartPoints");
                                if (startp){
                                    XmlNode * stat = startp->getNode(L"hp");
                                    wcstombs(tmpnum, stat->getAttribute(0)->getValue(), 30);
                                    newRace.startp[a].hp.max   = atoi(tmpnum);
                                    wcstombs(tmpnum, stat->getAttribute(1)->getValue(), 30);
                                    newRace.startp[a].hp.min   = atoi(tmpnum);

                                    stat = startp->getNode(L"mp");
                                    wcstombs(tmpnum, stat->getAttribute(0)->getValue(), 30);
                                    newRace.startp[a].mp.max   = atoi(tmpnum);
                                    wcstombs(tmpnum, stat->getAttribute(1)->getValue(), 30);
                                    newRace.startp[a].mp.min   = atoi(tmpnum);

                                    stat = startp->getNode(L"attack");
                                    wcstombs(tmpnum, stat->getAttribute(0)->getValue(), 30);
                                    newRace.startp[a].atck.max = atoi(tmpnum);
                                    wcstombs(tmpnum, stat->getAttribute(1)->getValue(), 30);
                                    newRace.startp[a].atck.min = atoi(tmpnum);

                                    stat = startp->getNode(L"deffense");
                                    wcstombs(tmpnum, stat->getAttribute(0)->getValue(), 30);
                                    newRace.startp[a].dfns.max = atoi(tmpnum);
                                    wcstombs(tmpnum, stat->getAttribute(1)->getValue(), 30);
                                    newRace.startp[a].dfns.min = atoi(tmpnum);

                                    stat = startp->getNode(L"agility");
                                    wcstombs(tmpnum, stat->getAttribute(0)->getValue(), 30);
                                    newRace.startp[a].agil.max = atoi(tmpnum);
                                    wcstombs(tmpnum, stat->getAttribute(1)->getValue(), 30);
                                    newRace.startp[a].agil.min = atoi(tmpnum);

                                    stat = startp->getNode(L"magpower");
                                    wcstombs(tmpnum, stat->getAttribute(0)->getValue(), 30);
                                    newRace.startp[a].magp.max = atoi(tmpnum);
                                    wcstombs(tmpnum, stat->getAttribute(1)->getValue(), 30);
                                    newRace.startp[a].magp.min = atoi(tmpnum);

                                    stat = startp->getNode(L"spirit");
                                    wcstombs(tmpnum, stat->getAttribute(0)->getValue(), 30);
                                    newRace.startp[a].sprt.max = atoi(tmpnum);
                                    wcstombs(tmpnum, stat->getAttribute(1)->getValue(), 30);
                                    newRace.startp[a].sprt.min = atoi(tmpnum);
                                }
                            }

                        }
                    }

                }

                races.add(newRace);

            }

        }

        racefile.destroy();

        return 0;
    }



