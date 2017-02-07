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


#ifndef CREATURE_H
#define CREATURE_H


#include <cmath>
#include <cstring>
#include <cstdlib>
#include "Races.h"
#include "Klasses.h"
#include "Inventory.h"
#include "SpellData.h"
#include "Strategy.h"





    struct Points{
        int max;
        int def;
    };

    //-------------------------
    struct SpellFrame{
        unsigned index;
        bool enabled;

        SpellFrame(){
            index = 0;
            enabled = false;
        }
    };
    //------------------------

    struct HelpCall{ //siunciamas pranesimas naparnikams
        bool needhelp; //reikia pagalbos
        bool called;   //kviete
        unsigned wantedItemID;  //ID of wanted Item
        unsigned caller;   //kas prase
    };

    enum ItemSlots {RIGHTHAND, LEFTHAND, ARRMOR};
    enum CreatureStates {HEALTHY, POISONED, DEAD};

//-------------------------------------
    class CreatureData{
    public:
        char  name[150];  //Creature name

        char modelfile[255];
        char texturefile[255];
        char npcsprite[255];

        float modelRadius;

        unsigned race;
        unsigned sex; 
        unsigned klass;

        Points hp;     //health
        Points mp;     //chi

        int atck;   //atack
        int defn;   //deffense
        int agil;  //agility
        int sprt;   //magic defense
        int magp;   //magic strength

        int exp;      //exp
        int espeed;   //learning(evolution) speed

        Resist resistense; //resistence to elements

        Item lhand;        //left
        Item rhand;        //right hads
        Item armor;        //arrmor

        CreatureStates state; //healthy,poisoned,dead ?


        char faceTileSet[255];
        unsigned int faceID;


        CreatureData(){
            exp = 0;
            state = HEALTHY;

        }

    };


//**********************
    class Creature:public CreatureData{
        //is creature esacaped from combat
        bool _escaped;
        DArray<SpellFrame> spells;
    public:

        
        bool known;     //show status in combat ?
        bool on_defense;//is deffense set ?
        //value of creatures experiance
        //helps calculate exp gain on victory
        float expValue; 


        Inventory inventory;   //creature inventory
        Strategy _AI;

        HelpCall helpme;  //Help call


        Creature():CreatureData(){
            _escaped = false;
            expValue = 1.0f;
        }
        //is creature escaped?
        bool escaped(){return _escaped;}
        void escape(){if (!_escaped) _escaped = true;}
        void resetEscape(){if (_escaped) _escaped = false;}

        int  enablespells(KlassList& klasses, SpellData& spellinfo);

        //statuses including weapon/armour bonuses
        int  sumAgil(ItemData& ginklai);
        int  sumDefense(ItemData& ginklai);
        int  sumAtack(ItemData& ginklai);

        //kills creature
        void kill();
        //resurects from dead, gives some hp
        void resurect(unsigned char strength);


        int restorePoints(bool _hp,int points);

        //spellu drain ir osmos pagrindas; strength- burto galingumas
        int drainPoints(Creature& victim, int strength, bool drainmp);

        int elementalDamage(int power,const char* name);

        //creature atacks victim, function returns damage
        int attack(Creature& victim, ItemData& ginklai);
        //sets deffense on
        void defend();
        //so far it's only weight that creature can carry
        unsigned int  stamina();
        //returns creature level
        int  level();
        //returns how much experiance still needed to reach next level
        int  needexp();
        //generates random hp,mp etc. stats
        void rollDice (RaceList& rases, KlassList& klases);
        //adds bonuses to stats when creature reaches next level
        void levelUp(RaceList& rases, KlassList& klases);
        //resets such fields as dead, poisoned, exp and money
        void reset(bool dknown);
        // Gives item to another creature, if this creature cannot carry given
        // items, false returned
        bool giveItems(Creature& draugas,
                       unsigned  index,
                       unsigned char count,
                       ItemData& item_data);

        bool loadFromFile(const char* path, KlassList& klasses,
                          SpellData& spellinfoi, ItemData& itemData);

        /*Equips weapon or arrmor
        *returns: -2 - no such item in invantory
        *         -1 - slot already occupied
        *          0 - can't equip this
        *          1 - ok
        */
        int equipItem(unsigned inventoryindex, ItemSlots slotName, ItemData& itemData);

        bool unequipItem(ItemSlots slotname, ItemData& itemData);

        bool        hasSpell(unsigned spellIndex);
        bool        canCast(unsigned spellIndex, SpellData& sd);
        unsigned    spellCount();
        SpellFrame* getSpell(unsigned index);
        void        addSpell(SpellFrame& s);

    
        //deletes spells and items
        void        destroy();
};
//***********************************************************}



#endif //CREATURE_H

