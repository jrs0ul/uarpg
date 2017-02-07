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

#include "Creature.h"
#include "Xml.h"
#include "Utils.h"


    int Creature::sumAgil(ItemData& ginklai){

        int tmp = agil;

        if (lhand.kind != -1)
            tmp += ginklai.info[lhand.kind].agility;
        if (rhand.kind != -1)
            tmp += ginklai.info[rhand.kind].agility;
        if (armor.kind != -1)
            tmp += ginklai.info[armor.kind].agility;

        return tmp;
    }
//------------------------------

    int Creature::sumDefense(ItemData& ginklai){

        int tmp=defn;
        if (lhand.kind != -1)
        tmp += ginklai.info[lhand.kind].defense;
        if (rhand.kind != -1)
            tmp += ginklai.info[rhand.kind].defense;
        if (armor.kind != -1){
            int fulldefense = ginklai.info[armor.kind].defense;
            tmp += (int)((fulldefense/100.0f) * armor.health());
            //printf("full defense:%d default: %d armor health:%d\n", fulldefense, (int)((fulldefense/100.0f) * armor.health()), armor.health() );
        }
        return tmp;
    }
//-------------------------------

    int Creature::sumAtack(ItemData& ginklai){
        int tmp = atck;
        if (lhand.kind != -1)
            tmp += ginklai.info[lhand.kind].damage;
        if (rhand.kind != -1)
            tmp += ginklai.info[rhand.kind].damage;
        return tmp;
    }
//-------------------------------
//
    void Creature::kill(){
        if (state != DEAD)
            state = DEAD;
    }

//--------------------------------

    int Creature::enablespells(KlassList& klasses, SpellData& spellinfo){

        int kiek=0;
        for (unsigned i = 0; i < klasses.get(klass)->spells.count(); i++)
            if ((!spells[i].enabled) &&
                ((int)spellinfo.spells[spells[i].index].level <= level())){
                spells[i].enabled = true;
                kiek++;
             }
        return kiek;
    }


 //-------------------------
    int Creature::restorePoints(bool _hp,int points){

        int * def = 0; 
        int * max=0;

    if (_hp){

        def=&hp.def; max=&hp.max;
    }
    else{
        def=&mp.def; max=&mp.max;
    }

    if (*def<*max)  {

        if (*def+points>*max){
            int senas=*def;
            *def=*max;
            return *max-senas;
        }
        else {
            *def+=points;
            return points;
        }
    }
   return 0;
}

//----------------------------------
void Creature::resurect(unsigned char strength){

    if (state == DEAD){
        state = HEALTHY;
        //restores hp acording strength
        hp.def = (int)ceil((hp.max / 100.0) * strength); 
    }
}
 //-------------------------
    int Creature::drainPoints(Creature& victim, int strength, bool drainmp){

        puts("executing drain points");
        Points* myPoints     = 0;
        Points* victimPoints = 0;
        int power = 0;
        int drained = 0;

        if (drainmp) {
            puts("mp");
            victimPoints = &(victim.mp);
            myPoints     = &(this->mp);
        }
        else {
            puts("hp");
            victimPoints = &(victim.hp);
            myPoints     = &(this->hp);
        }

        printf("victim p: %d/%d my p: %d/%d\n",
               victimPoints->def, victimPoints->max,
               myPoints->def, myPoints->def);
        if (magp>victim.sprt)
            power = (magp * strength) -
                    ( rand() % (victim.sprt + 1)) * strength;
        else
            power = 0;

        printf("Power: %d\n", power);

        if ((victimPoints->def - power) >= 0){
            puts("o_O");
            drained = power;
        }
        else {
            drained = victimPoints->def;
            puts("all right");
        }

        if (myPoints->def + drained > myPoints->max)
            drained = myPoints->max - myPoints->def;




        /*victim p: 4/10 my p: 45/45
        Power: 55
        Drained: -45 */

        printf("Drained: %d\n", drained);

        if (drained){
            victimPoints->def -= drained;

            if (myPoints->def + drained < myPoints->max)
                myPoints->def += drained;
            else
                myPoints->def = myPoints->max;

            return drained;
        } else
            return 0;

 }
    //--------------------------
    int Creature::elementalDamage(int power, const char* name){
        int resistns = 0;

        if (strcmp(name,"fire")==0)
            resistns=resistense.fire;
        else
        if (strcmp(name,"poison")==0){
            resistns=resistense.poison;
            if (state == HEALTHY)
                state = POISONED;

            power = (int)((hp.max / 100.0f) * power);
        }
        else
            if (strcmp(name,"ice")==0)
                resistns=resistense.ice;
            else
            if (strcmp(name,"bolt")==0)
                resistns=resistense.bolt;

            int dmg = power - resistns;
            if (dmg < 0)
                dmg = dmg * -1;
            hp.def = hp.def - dmg;
        return dmg;
    }

    //-----------------------------

    int Creature::attack(Creature& victim, ItemData& ginklai){

        int CombatDmg,
            victimDefense;

        int vhsumAg = victim.sumAgil(ginklai) / 2;
        int hsumAg = sumAgil(ginklai) / 2;

        //puola
        if (rand()%(vhsumAg + 1) > rand()%(hsumAg + 1))
            return -1;  //victim dodged

        if (on_defense)
            victimDefense = victim.sumDefense(ginklai);
        else
            victimDefense = victim.sumDefense(ginklai) / 2;

        CombatDmg = sumAtack(ginklai) - rand()%(victimDefense + 1);

        if (CombatDmg <= 0) 
            return 0;//miss victim

        int randomVictimArrmorDamage = rand()%3;
        if (victim.armor.kind != -1)
            victim.armor.takeDamage(randomVictimArrmorDamage);

        victim.hp.def -= CombatDmg;
        return CombatDmg; //hits victim

    }
 //-------------------------

 void Creature::defend(){
  on_defense=!on_defense;
 }

 //-------------------------
 unsigned int Creature::stamina(){

   return hp.max+atck*2;
 }

//--------------------------
    int Creature::level(){

        if (!espeed)
            return 0;

        if ((exp >= 0) && (exp < 2 * espeed))
            return 1;

        int i = 2;
        int p = exp / espeed;
        while (p >= i) {
            p -= i;
            if (p >= i + 1)
                i++;
        }

        return i;
    }
//------------------------
    int Creature::needexp(){


        Creature tmpslug;  //tmp padaras


  int tmp=level();  //isimenam leveli
  tmpslug.exp=exp;
  tmpslug.espeed=espeed;
  while (tmpslug.level()<tmp+1)  //didinam tmpslugo exp kol persok i kita
   tmpslug.exp++;


  return tmpslug.exp-exp;
 }
 //------------------------------
    void Creature::rollDice(RaceList& rases, KlassList& klases){

        Xrace * rc = rases.get(race);

        BasicAbilities* startp = &rc->startp[sex];
        XKlass* profesija = klases.get(klass);
        BonusUps* bonusp = &rc->bonusp[sex];

        //rsistance to nature elements
        resistense = rc->resistense;

        int hp_int = (startp->hp.max - startp->hp.min)+1; //hp interval
        int mp_int = (startp->mp.max-startp->mp.min)+1; //mp interval

        int at_int = (startp->atck.max - startp->atck.min) + 1;
        int de_int = (startp->dfns.max - startp->dfns.min) + 1;
        int ag_int = (startp->agil.max - startp->agil.min) + 1;
        int mgp_int = (startp->magp.max - startp->magp.min) + 1;
        int spr_int = (startp->sprt.max - startp->sprt.min) + 1;

        int levtmp = level()-1;


        //generuojam hp.max
        hp.max = (levtmp*(profesija->hp))+((rand()%(hp_int))+startp->hp.min);


          //levtmp*(classbonus+racebonus)+randomp+raceminimump
        atck = (levtmp*(profesija->atck + bonusp->atck) +
                (rand()%at_int)+startp->atck.min);
        defn = (levtmp*(profesija->dfns + bonusp->dfns) +
                (rand()%de_int)+startp->dfns.min);
        agil = (levtmp*(profesija->agil + bonusp->agil) +
                (rand()%ag_int)+startp->agil.min);
        sprt = (levtmp*(profesija->sprt + bonusp->sprt) +
                (rand()%spr_int)+startp->sprt.min);

        if (profesija->spells.count()){   //does class have some spells
            mp.max = (levtmp*(profesija->mp+bonusp->mp)) +
                    (rand()%mp_int+(startp->mp.min));
            magp = (levtmp*(profesija->magp+bonusp->magp)) +
                (rand()%mgp_int+(startp->magp.min));

            //adds spells to array
            for (unsigned long a = 0; a < profesija->spells.count(); a++){
                SpellFrame s;
                s.index = profesija->spells[a];
                s.enabled = false;
                spells.add(s);
            }

        }
        else {
            mp.max = 0;
            magp=0;
        }

        hp.def=hp.max;
        mp.def=mp.max;

        lhand.set(-1,0,0);
        rhand.set(-1,0,0);
        armor.set(-1,0,0);

    }
 //-----------------------------------------------
    void Creature::levelUp(RaceList &rases, KlassList& klases){

        Xrace * rc = rases.get(race);
        XKlass* clasb = klases.get(klass);
        BonusUps* raceb = &rc->bonusp[sex];

        hp.max += clasb->hp + raceb->hp;

        if (clasb->spells.count() > 0)
            mp.max+=clasb->mp + raceb->mp;
        atck += clasb->atck + raceb->atck;
        defn += clasb->dfns + raceb->dfns;
        agil += clasb->agil + raceb->agil;

        if (clasb->spells.count() > 0)
            magp += clasb->magp + raceb->magp;
        sprt += clasb->sprt + raceb->sprt;
  }
 //-------------------------------------------------
 void Creature::reset(bool dknown){
    exp = 0;
    inventory.destroy();
    state = HEALTHY;
    known = dknown;
 }
 //------------------------------------------------
    bool Creature::giveItems(Creature& draugas, unsigned int index,
                             unsigned char count, ItemData& item_data){

        bool tmp = false;

    if (draugas.inventory.itemWeight(item_data)<draugas.stamina())
        tmp=true;
    else
        tmp=false;

  if (tmp){
   for (int i=0;i<count;i++){
    Item* daiktas=inventory.getItem(index);
    draugas.inventory.addItem(daiktas,item_data);
    inventory.removeItemByIndex(index);
   }
  }
   return tmp;
 }

//----------------------------------------------------------
    bool Creature::loadFromFile(const char* path, KlassList& klasses,
                                SpellData& spellinfo,
                                ItemData& itemData){

        Xml herodata;
        puts(path);
        if (!herodata.load(path))
            return false;

        state = HEALTHY;
        known    = true;
        on_defense = false;
        helpme.called = false;
        helpme.needhelp = false;

        _escaped = false;

        XmlNode * hero = 0;
        hero = herodata.root.getNode(L"Hero");
        if (hero){
            XmlNode * node = 0;
            node = hero->getNode(L"Name");
            if (node){
                convertLT(node->getValue(), name, 150);
            }
            node = 0;
            node = hero->getNode(L"Model");
            if (node){
                wcstombs(modelfile, node->getValue(), 255);
            }
            node = 0;
            node = hero->getNode(L"Texture");
            if (node){
                wcstombs(texturefile, node->getValue(), 255);
            }
            node = 0;
            node = hero->getNode(L"Sprite");
            if (node){
                wcstombs(npcsprite, node->getValue(), 255);
            }
            node = 0;
            node = hero->getNode(L"Race");
            if (node){
                char buf[100];
                wcstombs(buf, node->getValue(), 100);
                race = (unsigned)atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"Sex");
            if (node){
                char buf[100];
                wcstombs(buf, node->getValue(), 100);
                sex = (unsigned)atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"Class");
            if (node){
                char buf[100];
                wcstombs(buf, node->getValue(), 100);
                klass = (unsigned)atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"HPmax");
            if (node){
                char buf[255];
                wcstombs(buf, node->getValue(), 255);
                hp.max = atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"MPmax");
            if (node){
                char buf[255];
                wcstombs(buf, node->getValue(), 255);
                mp.max = atoi(buf);
            }
            hp.def = hp.max;
            mp.def = mp.max;

            node = 0;
            node = hero->getNode(L"EXP");
            if (node){
                char buf[255];
                wcstombs(buf, node->getValue(), 255);
                exp = atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"Stats");
            if (node){
                char buf[255];
                wcstombs(buf, node->getAttribute(0)->getValue(), 255);
                atck = atoi(buf);
                wcstombs(buf, node->getAttribute(1)->getValue(), 255);
                defn = atoi(buf);
                wcstombs(buf, node->getAttribute(2)->getValue(), 255);
                agil = atoi(buf);
                wcstombs(buf, node->getAttribute(3)->getValue(), 255);
                sprt = atoi(buf);
                wcstombs(buf, node->getAttribute(4)->getValue(), 255);
                magp = atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"EvolvSpeed");
            if (node){
                char buf[255];
                wcstombs(buf, node->getValue(), 255);
                espeed = atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"Radius");
            if (node){
                char buf[255];
                wcstombs(buf, node->getValue(), 255);
                modelRadius = atof(buf);
            }

            node = 0;
            node = hero->getNode(L"Face");
            if (node){
                wcstombs(faceTileSet, node->getAttribute(0)->getValue(), 255);
                char buf[255];
                wcstombs(buf, node->getAttribute(1)->getValue(), 255);
                faceID = (unsigned)atoi(buf);
            }
            node = 0;
            node = hero->getNode(L"Resistence");
            if (node){
                char buf[255];
                wcstombs(buf, node->getAttribute(0)->getValue(), 255);
                resistense.bolt = atoi(buf);
                wcstombs(buf, node->getAttribute(1)->getValue(), 255);
                resistense.fire = atoi(buf);
                wcstombs(buf, node->getAttribute(2)->getValue(), 255);
                resistense.ice = atoi(buf);
                wcstombs(buf, node->getAttribute(3)->getValue(), 255);
                resistense.poison = atoi(buf);
            }


            node = 0;
            node = hero->getNode(L"Items");
            if (node){

                for (unsigned i = 0; i < node->childrenCount(); i++){

                    XmlNode * item = 0;
                    item = node->getNode(i);
                    if (item){

                        char buf[255];
                        wcstombs(buf, item->getAttribute(0)->getValue(), 255);
                        unsigned itemID = (unsigned)atoi(buf);
                        wcstombs(buf, item->getAttribute(1)->getValue(), 255);
                        unsigned count = (unsigned)atoi(buf);

                        if ((itemID < itemData.info.count()) && (count > 0)){
                            Item item;
                            if ((itemData.info[itemID].isweapon) 
                                || (itemData.info[itemID].isarmor))
                                item.set(itemID, 100, 1);
                            else
                                item.set(itemID, 0, 1);
                            for(unsigned i = 0; i < count; i++)
                                inventory.addItem(&item, itemData);
                        }
                    }

                }

            } //if node


        }


        
        spells.destroy();
        for (unsigned long a = 0; a < klasses.get(klass)->spells.count(); a++){

            SpellFrame s;
            s.enabled = false;
            s.index = klasses.get(klass)->spells[a];
            spells.add(s);

        }

        enablespells(klasses, spellinfo);
        armor.set(-1, 0, 0);
        lhand.set(-1, 0, 0);
        rhand.set(-1, 0, 0);

        herodata.destroy();
        return true;
    }

//--------------------------------------------
    int Creature::equipItem(unsigned inventoryindex,
                            ItemSlots slotName,
                            ItemData& itemData){


        Item* itm = 0;
        itm = inventory.getItem(inventoryindex);

        Item * slot = 0;
        switch (slotName){

            case RIGHTHAND: slot = &rhand; break;
            case LEFTHAND: slot = &lhand; break;
            case ARRMOR: slot = &armor; break;
        };

        if ((itm) && (slot)){

            int itemID = itm->kind;

            if (slot->kind != -1)
                return -1;

            if ((
                 (!itemData.info[itemID].isarmor) &&
                 (!itemData.info[itemID].isweapon)
                )||
                ((itemData.info[itemID].isarmor) && (slotName != ARRMOR))||
                ((itemData.info[itemID].isweapon) && (slotName == ARRMOR))
                ||(atck + itemData.info[itemID].damage < 0)
                ||(defn + itemData.info[itemID].defense < 0)
                ||(agil + itemData.info[itemID].agility < 0))
                    return 0;

            slot->set(itemID, itm->health(), itm->count);
            inventory.removeItemByIndex(inventoryindex);
            puts("equiped");

            return 1;


        }

        return -2;

    }
//------------------------------------------------
    bool Creature::unequipItem(ItemSlots slotname, ItemData& itemData){

        Item * slot = 0;
        switch (slotname){
            case RIGHTHAND: slot = &rhand; break;
            case LEFTHAND: slot = &lhand; break;
            case ARRMOR: slot = &armor; break;
        }
        if (slot){
            if (slot->kind != -1)  {
                Item tmp;
                tmp.set(slot->kind, slot->health(), slot->count);
                inventory.addItem(&tmp, itemData);
                slot->set(-1,0,0);
                return true;

            }
            return false;
        }
        return false;
    }
//------------------------------------------------------
    bool Creature::hasSpell(unsigned spellIndex){

        for (unsigned  i = 0; i < spells.count(); i++){

           if ((spells[i].index == spellIndex) && (spells[i].enabled))
               return true;
        }

        return false;

    }
//---------------------------------------------------
    bool Creature::canCast(unsigned spellIndex, SpellData& sd){
        if ((hasSpell(spellIndex)) && 
                (mp.def >= sd.spells[spellIndex].cost))
            return true;

        return false;
    }
//----------------------------------------------------
    unsigned Creature::spellCount(){
        return spells.count();
    }
//-----------------------------------------------------
    SpellFrame* Creature::getSpell(unsigned index){
         
        if (index < spells.count()){
            return &spells[index];
        }

        return 0;
    }
//-----------------------------------------------
    void Creature::addSpell(SpellFrame& s){
        spells.add(s);
    }
//---------------------------------------------
    void Creature::destroy(){

        spells.destroy();
        inventory.destroy();
    }


