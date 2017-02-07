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
#include "savegame.h"
#include "variables.h"

    bool LoadState(Party& partis, Party & innparty,
                   NpcArray& npcs,
                   CVariableArray& vars, OpenedChests& locks,
                   char* map, const char* path, ItemData& data,
                   const char* BasePath){

        FILE * savegame = 0;

        char buf[255];

        partis.destroy();

        savegame = fopen(path,"rb");
        if (!savegame) 
            return false;

        //load time
        time_t lol;
        if (!fread(&lol, sizeof(time_t), 1, savegame)){
            fclose(savegame);
            return false;
        }


        if (!fread(&partis.steps, sizeof(int), 1, savegame)){
            fclose(savegame);
            return false;
        }
        unsigned count = 0;
        if (!fread(&count, sizeof(unsigned), 1, savegame)){
            fclose(savegame);
            return false;
        }
        if (!fread(&partis.x, sizeof(int), 1, savegame)){
            fclose(savegame);
            return false;
        }
        if (!fread(&partis.y, sizeof(int), 1, savegame)){
            fclose(savegame);
            return false;
        }
         if (!fread(map, sizeof(char), 255, savegame)){
            fclose(savegame);
            return false;
        }



        for(unsigned i = 0; i < count; i++){
            Creature newone;

            CreatureData c;
            if (!fread(&c, sizeof(CreatureData), 1, savegame)){
                fclose(savegame);
                return false;
            }

            newone.agil        = c.agil;
            newone.armor       = c.armor;
            newone.atck        = c.atck;
            newone.state       = c.state;
            newone.defn        = c.defn;
            newone.espeed      = c.espeed;
            newone.exp         = c.exp;
            newone.hp          = c.hp;
            newone.klass       = c.klass;
            newone.lhand       = c.lhand;
            newone.magp        = c.magp;
            newone.mp          = c.mp;
            newone.race        = c.race;
            newone.resistense  = c.resistense;
            newone.rhand       = c.rhand;
            newone.sex         = c.sex;
            newone.faceID      = c.faceID;
            newone.sprt        = c.sprt;
            newone.modelRadius = c.modelRadius;
            strcpy(newone.name, c.name);
            strcpy(newone.modelfile, c.modelfile);
            strcpy(newone.texturefile, c.texturefile);
            strcpy(newone.faceTileSet, c.faceTileSet);
            strncpy(newone.npcsprite, c.npcsprite, 255);

            partis.addMember(newone, BasePath);
          
            sprintf(buf, "%s.tga", newone.texturefile);
            partis.modelTextures.loadFile(buf, 
                                          partis.count() - 1, 512, 512, BasePath, 1);
        }

        //loading spells & items
        for (unsigned z = 0; z < count; z++){

            unsigned spellcount = 0;
            if (!fread(&spellcount, sizeof(unsigned), 1, savegame)){
                fclose(savegame);
                return false;
            }
            for (unsigned i = 0; i < spellcount; i++){
                SpellFrame tmp;
                if (!fread(&tmp, sizeof(SpellFrame), 1, savegame)){
                    fclose(savegame);
                    return false;
                }
                partis.member(z)->addSpell(tmp);

            }

    

            unsigned igcount=0;
            if (!fread(&igcount, sizeof(unsigned), 1, savegame)){
                fclose(savegame);
                return false;
            }
            for (unsigned i = 0; i < igcount; i++){
                Item tmp;
                if (!fread(&tmp, sizeof(Item), 1, savegame)){
                    fclose(savegame);
                    return false;
                }
                for (unsigned a = 0; a < tmp.count; a++)
                    partis.member(z)->inventory.addItem(&tmp, data);
            }

        }

        //loading inn party-------
        count = 0;
        if (!fread(&count, sizeof(unsigned), 1, savegame)){
            fclose(savegame);
            return false;
        }
        for(unsigned i = 0; i < count; i++){
            Creature newone;

            CreatureData c;
            if (!fread(&c, sizeof(CreatureData), 1, savegame)){
                fclose(savegame);
                return false;
            }

            newone.agil       = c.agil;
            newone.armor      = c.armor;
            newone.atck       = c.atck;
            newone.state      = c.state;
            newone.defn       = c.defn;
            newone.espeed     = c.espeed;
            newone.exp        = c.exp;
            newone.hp         = c.hp;
            newone.klass      = c.klass;
            newone.lhand      = c.lhand;
            newone.magp       = c.magp;
            newone.mp         = c.mp;
            newone.race       = c.race;
            newone.resistense = c.resistense;
            newone.rhand      = c.rhand;
            newone.sex        = c.sex;
            newone.faceID     = c.faceID;
            newone.sprt       = c.sprt;
            strcpy(newone.name, c.name);
            strcpy(newone.modelfile, c.modelfile);
            strcpy(newone.texturefile, c.texturefile);
            strcpy(newone.faceTileSet, c.faceTileSet);
            strncpy(newone.npcsprite, c.npcsprite, 255);

            innparty.addMember(newone, BasePath, false);
          
        }
        //loading  spells & items
        for (unsigned z = 0; z < count; z++){

             unsigned spellcount = 0;
            if (!fread(&spellcount, sizeof(unsigned), 1, savegame)){
                fclose(savegame);
                return false;
            }
            for (unsigned i = 0; i < spellcount; i++){
                SpellFrame tmp;
                if (!fread(&tmp, sizeof(SpellFrame), 1, savegame)){
                    fclose(savegame);
                    return false;
                }
                innparty.member(z)->addSpell(tmp);

            }

            unsigned igcount=0;

            if (!fread(&igcount, sizeof(unsigned), 1, savegame)){
                fclose(savegame);
                return false;
            }
            for (unsigned i = 0; i < igcount; i++){
                Item tmp;
                if (!fread(&tmp, sizeof(Item), 1, savegame)){
                    fclose(savegame);
                    return false;
                }
                for (unsigned a = 0; a < tmp.count; a++)
                    innparty.member(z)->inventory.addItem(&tmp, data);
            }

        }
        //---------------

        //loading variables
        unsigned varcount = 0;
        char tmpname[255];
        char tmpvalue[255];

        if (!fread(&varcount, sizeof(unsigned), 1, savegame)){
            fclose(savegame);
            return(false);
        }
        for (unsigned i = 0; i < varcount; i++){
            int namelen = 0;
            int valuelen = 0;
            if (!fread(&namelen, sizeof(int), 1, savegame)){
                fclose(savegame);
                return false;
            }
            if (namelen > 253)
                namelen = 253;
            if (!fread(tmpname, sizeof(char), namelen, savegame)){
                fclose(savegame);
                return false;
            }
            tmpname[namelen] = 0;

            if (!fread(&valuelen, sizeof(int), 1, savegame)){
                fclose(savegame);
                return false;
            }
            if (valuelen > 253)
                valuelen = 253;
            if (!fread(tmpvalue, sizeof(char), valuelen, savegame)){
                fclose(savegame);
                return false;
            }
            tmpvalue[valuelen] = 0;

            CVariable var(tmpname,tmpvalue);
            vars.add(var);

        }
        npcs.destroy();
        npcs.loadBin(savegame, BasePath);
        locks.loadBin(savegame);
            
        fclose(savegame);

        return true;

    }
//--------------------------------------------------------------------------
    bool SaveState(Party& partis, Party& innparty,
                   NpcArray& npcs,
                   CVariableArray& vars,
                   OpenedChests& locks, const char* map,
                   const char* path){

        FILE * savegame = 0;


        time_t rawtime;
        time ( &rawtime );

        savegame = fopen(path, "wb+");

        if (!savegame)
            return false;


        //save time
        if (!fwrite(&rawtime, sizeof(time_t), 1, savegame)){
            fclose(savegame);
            return false;
        }


        //party steps
        if (!fwrite(&partis.steps, sizeof(int), 1, savegame)){
            fclose(savegame);
            return false;
        }
        //meber count
        unsigned count = partis.count();
        if (!fwrite(&count, sizeof(unsigned), 1, savegame)){
            fclose(savegame);
            return false;
        }
        //x
        if (!fwrite(&partis.x, sizeof(int), 1, savegame)){
            fclose(savegame);
            return false;
        }
        //y
        if (!fwrite(&partis.y, sizeof(int), 1, savegame)){
            fclose(savegame);
            return false;
        }
        //current map name
        if (!fwrite(map, sizeof(char), 255, savegame)){
            fclose(savegame);
            return false;
        }

        for (unsigned i = 0; i < partis.count(); i++){

            CreatureData * member = (CreatureData *)partis.member(i);
            if (!fwrite(member, sizeof(CreatureData), 1, savegame)){
                fclose(savegame);
                return false;
            }

        }

        //saving  spells & items
        for (unsigned a = 0; a < partis.count(); a++){

            unsigned spellcount = partis.member(a)->spellCount();
            if (!fwrite(&spellcount, sizeof(unsigned), 1, savegame)){
                fclose(savegame);
                return false;
            }
            for (unsigned i = 0; i < spellcount; i++){
                SpellFrame * s = partis.member(a)->getSpell(i);
                if (!fwrite(s, sizeof(SpellFrame), 1, savegame)){
                    fclose(savegame);
                    return false;
                }
            }

            unsigned igcount = partis.member(a)->inventory.itemGroupCount();
            if (!fwrite(&igcount, sizeof(unsigned), 1, savegame)){
                fclose(savegame);
                return false;
            }
            for (unsigned i = 0; i < igcount; i++){
                Item* item = partis.member(a)->inventory.getItem(i);
                if (!fwrite(item, sizeof(Item), 1, savegame)){
                    fclose(savegame);
                    return false;
                }
            }
        }

        //saving inn party----------------
        count = innparty.count();
        if (!fwrite(&count, sizeof(unsigned), 1, savegame)){
            fclose(savegame);
            return false;
        }

        for (unsigned i = 0; i < innparty.count(); i++){

            CreatureData * member = (CreatureData *)innparty.member(i);
            if (!fwrite(member, sizeof(CreatureData), 1, savegame)){
                fclose(savegame);
                return false;
            }

        }
        //saving inn party items
        //well it's simillar to previous, maybe we should make a function ?
        for (unsigned a = 0; a < innparty.count(); a++){
            unsigned igcount = innparty.member(a)->inventory.itemGroupCount();
            if (!fwrite(&igcount, sizeof(unsigned), 1, savegame)){
                fclose(savegame);
                return false;
            }
            for (unsigned i = 0; i < igcount; i++){
                Item* item = innparty.member(a)->inventory.getItem(i);
                if (!fwrite(item, sizeof(Item), 1, savegame)){
                    fclose(savegame);
                    return false;
                }
            }
        }

        //--------------
        //saving variables
        unsigned varcount = vars.count();
        if (!fwrite(&varcount, sizeof(unsigned), 1, savegame)){
            fclose(savegame);
            return false;
        }
        for (unsigned i = 0; i < vars.count(); i++){
            int namelen = (int)strlen(vars.name(i));
            int valuelen = (int)strlen(vars.value(i));
            
            if (!fwrite(&namelen,      sizeof(int), 1, savegame)){
                fclose(savegame);
                return false;
            }
            if (!fwrite(vars.name(i),  sizeof(char), namelen, savegame)){
                fclose(savegame);
                return false;
            }
            if (!fwrite(&valuelen,     sizeof(int), 1, savegame)){
                fclose(savegame);
                return false;
            }
            if (!fwrite(vars.value(i), sizeof(char), valuelen, savegame)){
                fclose(savegame);
                return false;
            }

        }

        npcs.saveBin(savegame);
        printf("Opened locks: %lu\n", locks.count());
        locks.saveBin(savegame);

        fclose(savegame);
        

        return true;

    }




