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

#ifdef WIN32
    #ifdef  _MSC_VER
        #define _CRT_SECURE_NO_DEPRECATE 1
        #pragma comment(lib,"SDL.lib")
        #pragma comment(lib,"SDLmain.lib")
        #pragma comment(lib,"OpenGl32.lib")
        #pragma comment(lib,"GLU32.lib")
        #pragma comment(lib,"openal32.lib")

        #ifdef _DEBUG
            #pragma comment(lib,"ogg_d.lib")
            #pragma comment(lib,"vorbis_d.lib")
            #pragma comment(lib,"vorbisfile_d.lib")
        #else
            #pragma comment(lib,"ogg.lib")
            #pragma comment(lib,"vorbis.lib")
            #pragma comment(lib,"vorbisfile.lib")
        #endif
    #endif
#endif


#ifdef __APPLE__
#include <limits.h>
#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>
#endif

#include "main.h"
//==========="some" globals :)=====================
SubsetCollection Pointer3D;
SubsetCollection DummyFighter;
DCamera         Cam3D;
const char      MainMenuMusic[] = "04 - Metal Jig";
const char      Version[] = "0.900";
const unsigned  MaxSaveStates = 10;

SoundSystem SS;

PicsContainer Pics;

SDL_Joystick *Joy = 0;
OggStream Stream;

unsigned char keys[11]={0};

int MouseX, MouseY; //relative mouse coords
int _MouseX, _MouseY;

SpellData Spells;  //spell information

KlassList klases; //Classes information DB
RaceList Rases; //Races DB
ItemData itemData; //Items DB

//Party of heroes
Party jparty;
//enemies
Party eparty;
//Party in an inn
Party innparty;

TextScreen Console,//console
           conversation; //console for conversations with npc

ActiveFighter  AF;
unsigned       Combat_beginclock = 0;
const unsigned IntroPathTicks = 180;
const unsigned OutroPathTicks = 250;
unsigned       Combat_Introclock = 0;
unsigned       VictoryClock = 0;
unsigned       Combat_groupindex = 0;

int StepsToEncounter;

enum GameStates GameState = TITLEMENU;

bool
bVisiKovesi,
atacked,
shut_down,
weldone,
gameover,
next_step,
code_enter_enabled,
code_enter,
visible_enemy,
Cheat_NoEnemy,
PartyControl   //can I control party ?
;

MenuLoader ML;
Squad Entit; //fighters in combat field
Map map;

MGroupList MonsterGroups;

CombatAnimationControl CAnimControl;

PartyActions HeroActions;
PartyActions EnemyActions;

unsigned long picc;

HeroMaker hMaker;//helper class for creating new hero

SelectMenu ActionSelect,    //hero action selection in combat
           InnMenu,         //Inn menu
           SmithMeniu,      //blacksmith menu
           merchantmeniu,   //Shop menu
           InnPartyMeniu,
           MerchantItems,   //Shop items menu
           //gitemmeniu,      //items menu in adventuring mode
           orderselect,     //root menu in adventure mode
           invsel ,         //use, equip, give ...
           EquipBody,       //lhand,rhand, armor
           OptionMenu,      //Option menu in main menu
           optmeniu,        //ingame options meniu: quit
           SpellMenu,       //hero spell menu
           QuitMenu,        //quit Y/N ?
           Mainmenu,        //The main menu
           CreateHeroMenu,  //Party creation menu
           racesel,         //race menu in new hero creation
           sexsel,          //sexes menu in new hero creation
           classsel,        //class selection menu
           SelectNewFace,
           ItemMenu,        //Inventory menu
           NewGameMenu,     //create hero or use default?
           Savemenu,
           ModMenu          //menu of all available mods
           ;

FaceSelect partyfaces;//Party member selection

BattleSelect Combatselect;


EditBox     CodeEdit,       //field to enter a script line
            NameEnter,      //field for hero name
            ItemCountedit;  //enter how many items

int globalKEY, oldKEY; //global keyboard status

unsigned    g_daiktas,
            g_item_count, //how many items will be given(adventuring)
            G_MemberIndex,
            G_OponentIndex,//oponent index in adventuring
            CasterIndex;

//shop's global variable:
unsigned char gs_daiktas;
//------------------------
bool HeroActionSelect;
bool HeroActionDone;
//-----------------------------

int Selection; //Selection in combat

bool SYSTEM_filesloaded = false; //are generic images loaded ?
unsigned long SYSTEM_imagefilescount = 0; //count of preloaded images


NpcArray npcnation;
OpenedChests EmptyChests;

bool ENCsmith, ENCmersh, ENCinn;

EventArray Events; //Active points in map

int mapposx, mapposy;//map coordinates in screen
char Currentmap[255];
bool IsCurrentMap3D = false;

bool Victory = false;//battle is won
//--------------------
bool AtackOnce = true;
bool CastOnce  = true;
bool UseOnce = true;
bool DodgeOnce = true;
bool Dodge2Once = true;
bool ShowEntries = false;
//--------------------

CreatureStatus Talker; //talker face

bool isTalking = false;//talking with npc ?

SystemConfig Sys; //system configuration settings

float fader = 0.0f; // fade-in coefficient

CVariableArray Kintamieji;
DArray<CCode> ScriptStack;
long ActiveScript = -1;

int MapPushdirx = 0;
int MapPushdiry = 0;
int GlobalMapPushtimer = 0;
int MapPushsteps = 0;

Camera Cam;

PicsContainer Tailsetas;
SubsetCollection Tileset3D;

int OldJoyX = 0;
int OldJoyY = 0;
int JoyX = 0;
int JoyY = 0;

PlayerGfxList PML; //list of player models, textures etc.

//home path
char HPath[255];

ParticleSystems PS;
//window which displays how much xp fighters gained
StatsGainWindow StatGain;
//window which displays looted items
ItemGainWindow  ItemGain;

CArena Arena;

//array of script function pointers
Functions Funcs;

Merchant Merch;
Skybox Sky;

Log GameLog;
//type text without keyboard
OnScreenKeyboard Board;

CreditsDialog CreditWindow;


DArray<ShaderProgram> GLSLprog;
//-------------------
const int ShadowMapSize = 256;
GLuint ShadowMapTexture = 0;
GLuint fboId = 0;
const double biasMatrix[] = {0.5f, 0.0f, 0.0f, 0.0f,
                             0.0f, 0.5f, 0.0f, 0.0f,
                             0.0f, 0.0f, 0.5f, 0.0f,
                             0.5f, 0.5f, 0.5f, 1.0f};

float CameraProjectionMatrix[16];
float LightProjectionMatrix[16];
float LightViewMatrix[16];

Vector3D LightPos;
Vector3D LightTo;

SDL_Surface * WindowIcon = 0;

float FrustumRadius = 0;

int Options_Width = 640;
int Options_Heigt = 480;

//==================================================
void MakeNextEncounterSteps(){
    //TODO: make it more sophisticated
    int minStepCount = 4;
    int maxStepCount = 8;
    StepsToEncounter = (rand() % (maxStepCount - minStepCount)) + minStepCount;
}
//----------------------------------
void QuitApp(){
    Sys.quitApp = true;
}
//------------------------------------
void MoveMap(int x, int y, int charx, int chary){

        int testx = mapposx-x;
        int testy = mapposy-y;

        int maxx = (map.width() * 32)-Sys.width;
        int maxy = (map.height() * 32)-Sys.height;

        if (x<0){
            if (charx > (int)(map.width()*32-(Sys.width/2)))
                x = 0;
            if (testx > 0)
                x = 0;
        }
        if (x>0){
            if (charx < (Sys.width / 2))
                x = 0;
            if (testx < -maxx)
                x = 0;
        }
        if (y<0){
            if (chary > (int)(map.height() * 32-(Sys.height/2)))
                y = 0;
            if (testy > 0)
                y = 0;
        }
        if (y>0){
            if (chary < (Sys.height/2))
                y = 0;
            if (testy < -maxy)
                y = 0;
        }

        mapposx -= x;
        mapposy -= y;
}
//-------------------------------------
void CenterMap(int x,int y){

    int mapwidth = map.width() * 32;
    int mapheight = map.height() * 32;

    int xas = x; int yas = y;

    if (mapwidth > Sys.width){
        if ((xas + Sys.width/2 > mapwidth))
            xas = mapwidth - Sys.width;
        else
            if (xas-Sys.width/2 < 0){
                xas = 0;
            }
            else
                xas -= Sys.width/2;
    }
    else
        xas = -(Sys.width/2-(mapwidth/2));
    //-------------
    if (mapheight > Sys.height){
        if (yas + Sys.height/2 > mapheight){
            yas = mapheight - Sys.height;
        }
        else
            if (yas-Sys.height/2 < 0){
                yas = 0;
            }
            else
                yas -= Sys.height/2;
    }
    else
         yas= -(Sys.height/2 - (mapheight/2));

    mapposx = -xas;
    mapposy = -yas;
}
//--------------------------------------------------
void DrawNpcs3D(ShaderProgram * generic,
                ShaderProgram * justcolor,
                bool useGLSL = true){

    //glColor4f(1,1,1,1);

    glActiveTexture(GL_TEXTURE0);

    int firstHero = -1;

    for (unsigned man = 0; man < npcnation.count(); man++){
        Npc * dud = 0;
        dud = npcnation.getNpc(man);
        if (dud){

            if (useGLSL)
                generic->use(); //tex'n'color 

            if (dud->job == 255){

                if (firstHero == -1)
                    firstHero = man;
                dud->draw3D(jparty.modelTextures.getname(man - firstHero),
                            jparty.models[man - firstHero].fighter);
            }

            else{

                if ((dud->modelIndex < npcnation.modelCount())
                     &&(npcnation.modelCount() > 0)){

                    NpcModel * model = 0;
                    model = npcnation.getModel(dud->modelIndex);
                    if (model){
                        dud->draw3D(npcnation.getTextureName(man),
                                    model->dud);
                    }
                }
                else{
                    if (Sys.hasGLSL)
                        justcolor->use(); //just color
                    dud->draw3D(0, DummyFighter);
                }
            }
            glPopMatrix();
        }
    }
}
//--------------------------------------------------
void DrawMap3D(){

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    glLoadMatrixf(CameraProjectionMatrix);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor4f(1,1,1,1);
    float m2[16];
    Vector3D target = Cam3D.pos() + Cam3D.dir();
    MatrixLookAt(Cam3D.pos(), target,  Cam3D.up(), m2);
    glMultMatrixf(m2);

    glPushMatrix();
    double m[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, m);
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    glLoadMatrixd(m);
    if (Sys.hasGLSL)
        GLSLprog[1].use(); //tex'n'color
    Sky.draw(Pics);

    glPopMatrix();
    glPushMatrix();

    map.draw3DPlane(Tailsetas, 0, 0);
    map.draw3DPlane(Tailsetas, 0, 1);

    Vector3D frustumCenter = CalcFrustumSphereCenter(1.0f,
                                                     100.0f,
                                                     Cam3D.pos(), Cam3D.dir());

    map.draw3DTiles(frustumCenter,
                      FrustumRadius,
                      Tileset3D, Tailsetas, 0, 2);

    map.draw3DAssets(Tileset3D, Tailsetas, 0);

    glDisable(GL_LIGHTING);
    if (Sys.hasGLSL)
        DrawNpcs3D(&GLSLprog[1], &GLSLprog[0]);
    else
        DrawNpcs3D(0, 0, false);

    /*glPopMatrix();
    glPushMatrix();
    glTranslatef(fCenter.v[0], fCenter.v[1],
                 fCenter.v[2]);
    DummyCircle(fRadius);*/
    glPopMatrix();
}
  
//--------------------------------------------------
void DrawMap(float x, float y){

    ShaderProgram* jc = 0;
    ShaderProgram* tc = 0;
        if (Sys.hasGLSL){
            jc = &GLSLprog[0];
            tc = &GLSLprog[1];
            tc->use();
        }

        Pics.drawBatch(jc, tc, 666);
        map.draw( (int)x,
                  (int)y,
                  Tailsetas,
                  0,
                  0,
                  Sys.width, Sys.height,
                  COLOR(fader, fader, fader));
        Tailsetas.drawBatch(jc, tc, 666);

        for (unsigned man = 0; man < npcnation.count(); man++){
            Npc * dud = 0;
            dud = npcnation.getNpc(man);
            if (dud){
                dud->draw(Pics,
                         Pics.findByName(dud->spritename),
                         (int)x, (int)y,
                         0, 0);
            }

        }

      
        Pics.drawBatch(jc, tc, 666);
        map.draw( (int)x,
                  (int)y,
                  Tailsetas,
                  0,
                  1,
                  Sys.width, Sys.height,
                  COLOR(fader,fader,fader));
        Tailsetas.drawBatch(jc, tc, 666);

        if (ShowEntries){
            for (unsigned ent = 0; ent < Events.entries.count(); ent++){
                float shift = rand()%4;
                Pics.draw(Pics.findByName("rod.tga"), 
                          (Events.entries[ent].x * 32.0f) + x + 16.0f,
                          (Events.entries[ent].y * 32.0f) + y + 8.0f + shift,
                          3, true, 1.1f, 1.1f, 0.0f, 
                          COLOR(0.0f, 0.0f, 0.0f, 1.0f),
                          COLOR(0.0f, 0.0f, 0.0f, 1.0f));
                Pics.draw(Pics.findByName("rod.tga"), 
                          (Events.entries[ent].x * 32.0f) + x + 16.0f,
                          (Events.entries[ent].y * 32.0f) + y + 8.0f + shift,
                          3, true, 1.0f, 1.0f, 0.0f, 
                          COLOR(1.0f, 0.0f, 0.0f, 1.0f),
                          COLOR(1.0f, 0.0f, 0.0f, 1.0f));

            }
        Pics.drawBatch(jc, tc, 666);
    }
}

//-------------------------------------------
bool LoadObjects(const char* s){

    char path[214];
    sprintf(path, "%sraces.xml", s);
    GameLog.write("Loading races...");
    if (Rases.load(path))
        return false;

        //read class info
        sprintf(path, "%sclasses.xml", s);
        GameLog.write("Loading classes...");
        if (!klases.load(path))
            return false;

        sprintf(path, "%splayergfx.xml", s);
        GameLog.write("Loading player gfx info");
        if (!PML.load(path, Rases, klases))
            return false;

        sprintf(path, "%sspells.xml", s);
        GameLog.write("Loading spells");
        if (!Spells.load(path))
            return false;


        /*sprintf(path, "%sobjects.dat", s);
        GameLog.write("Loading weapon info...");
        if (!Arminfo.load(path))
            return false;*/

    //reads items
    sprintf(path, "%sitems.xml", s);
    GameLog.write("Loading items...");
    if (!itemData.load(path))
        return false;

    return true;
}
//-------------------------------------------------
void PartyStatus(int x,int y, Party& partis, bool enemy, bool vertical){

    CreatureStatus * prt = new CreatureStatus[partis.count()];

    int ix = 0;
    int iy = ix;
    bool visible;
    for (unsigned i = 0; i < partis.count(); i++) {

        Creature * m = partis.member(i);

        visible = (!((m->known == false) && (visible_enemy == false)
                    && (enemy == true)));

        prt[i].change(x + ix, y + iy, m->name,
                      Pics.findByName(m->faceTileSet), m->faceID);
        prt[i].showhpmp = visible;
        prt[i].enemyface = enemy;
        prt[i].dead = (m->state == DEAD) ? true : false;
        prt[i].poisoned = (m->state == POISONED)? true : false;
        prt[i].mp.def = m->mp.def;  prt[i].hp.def = m->hp.def;
        prt[i].mp.max = m->mp.max;  prt[i].hp.max = m->hp.max;

        prt[i].draw(Pics,
                    Pics.findByName("status.tga"), 
                    Pics.findByName("mark.tga"),
                    Pics.findByName("charai.tga"));

        if (!vertical)
            ix=ix+70;
        else iy=iy+75;
    }

    delete []prt;
}
//-----------------------------------------------
void EnemyGenerate( Party& part, MonsterGroup& posibleEnemies){

    part.destroy();
    int enemytomake = posibleEnemies.mcount;

    part.destroyModels();
    part.isBoss = posibleEnemies.isBoss;

    LoadArena(posibleEnemies.arenaName);

    for (int a = 0; a < enemytomake; a++){
        Creature newone;
        int kuris = rand()%posibleEnemies.monsters.count();

        Monster * monster = &posibleEnemies.monsters[kuris];

        newone.state = HEALTHY;

        newone.known = false;
        newone.race = monster->race;
        newone.expValue = monster->expValue;

        Xrace * rc = Rases.get(newone.race);

        newone.espeed = rc->espeed;   //evolution speed

        newone.modelRadius = monster->radius;

        //------------
        FighterModel newModel;
        char buf[255];
        sprintf(buf, "%smodels/%s.rm2", Sys.basePath,
                monster->model);

        if (!newModel.fighter.load(buf)){
            sprintf(buf, "base/models/%s.rm2", monster->model);
            newModel.fighter.load(buf);
        }
        strcpy(newModel.name, monster->model);
        part.models.add(newModel);

        sprintf(buf, "%s.tga", monster->modeltexture);
        part.modelTextures.loadFile(buf, a, 512, 512, Sys.basePath, 1);
        //-------
        //generates exp
        newone.exp=0;
        for (unsigned i = 0; i < monster->maxlevel; i++){
            newone.exp += rc->espeed*i;
        }
        newone.exp += rand() % 10;

        printf("newone exp: %d\n", newone.exp);

        newone.sex = monster->sex;

        newone.helpme.needhelp = false;
        newone.helpme.called = false;

        newone.klass = monster->klass;

        strncpy(newone.name, monster->name, 150);

        newone.destroy();
        newone.rollDice(Rases, klases);
        newone.enablespells(klases, Spells);

        //enemy strategy
        newone._AI.copyFrom(monster->ai);
        //enemy items

        for (unsigned z = 0; z < monster->inventory.count(); z++){
            MonsterItem * mi = &(monster->inventory[z]);
            Item itm;
            itm.set(mi->index, 0, 1);
            unsigned count = (unsigned)(rand()%(mi->maxcount - mi->mincount)
                                        + mi->mincount);
            for (unsigned i = 0; i < count; i++){
                newone.inventory.addItem(&itm, itemData);
            }
            
            int health = 0;
            int ecount = 0;
            if (monster->rightHand > -1){
                health = 100;
                ecount = 1;
            }
            newone.rhand.set(monster->rightHand, health, count);
            health = 0;
            ecount = 0;
            if (monster->leftHand > -1){
                health = 100;
                ecount = 1;
            }
            newone.lhand.set(monster->leftHand, health, count);
            health = 0;
            ecount = 0;
            if (monster->arrmor > -1){
                health = 100;
                ecount = 1;
            }
            newone.armor.set(monster->arrmor, health, count);
        }
        part.addMember(newone, Sys.basePath, false);
    }
}
//------------------------------------------

    int Attack( unsigned myNumber, unsigned oponentNumber,
                unsigned myType, unsigned oponentType){

        Creature * me = 0;
        Creature * oponent = 0;
        Party * oponentParty = 0;
        if (myType == 1)
            me = jparty.member(myNumber);
        else
            me = eparty.member(myNumber);

        if (oponentType == 1){
            oponent = jparty.member(oponentNumber);
            oponentParty = &jparty;
        }
        else{
            oponent = eparty.member(oponentNumber);
            oponentParty = &eparty;
        }

        char buf[255];
        int dmg = me->attack(*oponent, itemData);

        if (oponent->armor.kind != -1){
            if (oponent->armor.health() <= 0){
                oponent->unequipItem(ARRMOR, itemData);
                sprintf(buf,"%s arrmor is destroyed!.", oponent->name);
                Console.putText(buf);
            }
        }

        switch(dmg){
            case -1: {
                sprintf(buf,"%s dodged.", oponent->name);
                Console.putText(buf);//jei isvenge
            }
            break;
        }

        if (oponent->hp.def <= 0)  {
            oponentParty->killMember(oponentNumber);
            sprintf(buf,"%s died", oponent->name);
            Console.putText(buf);
        }

        return dmg;
    }

    //----------------------------------------------------------
    void PoisonEffect( Party& partis, unsigned index){
        char buf[50];
        Creature * cr = 0;
        cr = partis.member(index);
        if (cr){
            if (cr->state == POISONED)  {
                int dmg = (int)(((cr->hp.max / 100.0f) * 11) - 
                                cr->resistense.poison);
                if (dmg <= 0)
                    dmg = 1;
                cr->hp.def -= dmg;
                sprintf(buf, "Poison does %dp dmg to %s", dmg, cr->name);
                Console.putText(buf);
                if (cr->hp.def <= 0)  {
                    partis.killMember(index);
                    sprintf(buf,"%s died", cr->name);
                    Console.putText(buf);
                }
            }
        }
    }
//-----------------------------------------------------------------
    void ScanCreature(Creature& crtr){
        char buf[60];

        sprintf(buf,"%s is on %d level.",crtr.name,crtr.level());

        Console.putText(buf);
        sprintf(buf,"%d/%d HP",crtr.hp.def,crtr.hp.max);

        Console.putText(buf);
        sprintf(buf,"%d/%d MP",crtr.mp.def,crtr.mp.max);

        Console.putText(buf);
        sprintf(buf,"Atck %d",crtr.sumAtack(itemData));

        Console.putText(buf);
        sprintf(buf,"Def %d",crtr.sumDefense(itemData));

        Console.putText(buf);
        sprintf(buf,"Agil %d",crtr.sumAgil(itemData));

        Console.putText(buf);

        char rhand[100];
        char lhand[100];
        char armor[100];
        char empty[] = "--empty--";
        if (crtr.rhand.kind != -1)
            strcpy(rhand, itemData.info[crtr.rhand.kind].name);
        else
            strcpy(rhand, empty);

        if (crtr.lhand.kind != -1)
            strcpy(lhand, itemData.info[crtr.lhand.kind].name);
        else
            strcpy(lhand, empty);

        if (crtr.armor.kind != -1)
            strcpy(armor, itemData.info[crtr.armor.kind].name);
        else
            strcpy(armor, empty);
        sprintf(buf,"Uses: %s, %s", rhand, lhand);

        Console.putText(buf);
        sprintf(buf,"Armor: %s", armor);

        Console.putText(buf);
        if (!crtr.known)
            crtr.known=true;
    }

    //------------------------
    void End_combat(){

        if (Victory){
            Cam.x = (int)npcnation.getNpc(npcnation.count()-jparty.count())->x();
            Cam.y = (int)npcnation.getNpc(npcnation.count()-jparty.count())->y();
            Focus3DCam(Cam.x, Cam.y, 
                       map.camHeight, map.camDistance, map.camPitch );

            GameState = WALKING;
            char buf[30];
            strcpy(buf,"Let(\"Victory\",\"true\"");
            Interpret(buf);
            Console.init(100,0,160,19*18);
        }
        else{
            if (jparty.alive()){
                GameState = WALKING;
                Console.init(100,0,160,19*18);
            }
        }

        next_step = true;
        jparty.steps = 0;
        MakeNextEncounterSteps();
        CAnimControl.reset();
    }

//----------------------------
    void CastSpell(int spellindex, Party& casterparty, int casterindex,
                   Party& oponentparty, int oponentindex){

        char buf[255];

        Creature * cpm = casterparty.member(casterindex);

        printf("mp def: %d, spell cost: %d\n", 
                cpm->mp.def, Spells.spells[spellindex].cost);

        sprintf(buf,"%s tried to cast %s", 
                cpm->name, Spells.spells[spellindex].name);
        Console.putText(buf);

        if (strcmp(Spells.spells[spellindex].codeLine,".")!=0){
            puts(Spells.spells[spellindex].codeLine);
            int oponent=0;

            if(&oponentparty == &jparty)
                oponent = oponentindex;
            else
                oponent = jparty.count() + oponentindex;

            sprintf(buf,"Let(\"oponent\",\"%d\")", oponent);
            Interpret(buf);

            int caster = 0;
            if(&casterparty == &jparty)
                caster=casterindex;
            else
                caster = jparty.count() + casterindex;

            sprintf(buf,"Let(\"caster\",\"%d\")",caster);
            Interpret(buf);

            sprintf(buf,"Let(\"casterMagP\",\"%d\")", cpm->magp);
            Interpret(buf);

            puts(Spells.spells[spellindex].codeLine);
            Interpret(Spells.spells[spellindex].codeLine);

        }
        //printf("Caster ID %d\n", casterindex);
        cpm->mp.def = cpm->mp.def - Spells.spells[spellindex].cost;
    }
    //----------------------------------------------------------------
    void UseItem( Creature& crtr,  unsigned itemID,
                  Party& partis, unsigned index){

        char buf[250];
        bool use = false;

        Item* itm = 0;
        itm = crtr.inventory.getItem(itemID);

        Creature * oponent = partis.member(index);
        //TODO: we need better solution
        if (oponent == &crtr){
            puts(crtr.name);
            printf("itemID %u\n", itm->kind);
            sprintf(buf,"%s tried %s", crtr.name,
                    itemData.info[itm->kind].name);
            Console.putText(buf);
        }
        else{

            sprintf(buf,"%s tried %s on %s", crtr.name, 
                    itemData.info[itm->kind].name, oponent->name);
            Console.putText(buf);
        }
        puts(buf);

        if (strcmp(itemData.info[itm->kind].codeLine, ".") != 0){
            int op = 0;
            if (&partis == &jparty)
                op = index;
            else
                op = jparty.count() + index;
            char buf[20];
            sprintf(buf,"Let(\"oponent\",\"%d\")",op);
            Interpret(buf);
            Interpret(itemData.info[itm->kind].codeLine);
            use=true;
        }

        if (use)  {
            crtr.inventory.removeItemByIndex(itemID);

            if (oponent->hp.def <= 0)  {
                partis.killMember(index);
                sprintf(buf,"%s died.", oponent->name);
                Console.putText(buf);

            }
        }
    }
 
//---------------------------------------------------------------
    void SendCall(unsigned itemID, const char* textas, unsigned kas){

        Creature* dude = eparty.member(kas);

        if (!dude->helpme.needhelp) {

            dude->helpme.needhelp = true;

            for (unsigned i = 0; i < eparty.count(); i++) {

                Creature * memb = eparty.member(i);

                if ((i != kas) && (memb->state != DEAD))  {
                    memb->helpme.called = true;
                    memb->helpme.caller = kas;
                    memb->helpme.wantedItemID = itemID;
                    char buf[255];
                    sprintf(buf, "%s calls for %s", dude->name,
                            itemData.info[itemID].name);
                    Console.putText(buf);
                }
            }
        }
    }
//-----------------------------------------------------------------
    void HelpCaller(unsigned z, unsigned name, unsigned & caller){

        Creature * dude = eparty.member(z);
        caller = dude->helpme.caller;
        Creature * whocalled = eparty.member(caller);

        dude->helpme.called = false;

        if (whocalled->state != DEAD)  {
            int index = dude->inventory.findItem(name);
            EnemyActions.set(z, USE, 
                             Entit.findFighter(MONSTER, caller), index);
            whocalled->helpme.needhelp = false;
        }
        for (unsigned i = 0; i < eparty.count(); i++)
            if ((eparty.member(i)->helpme.wantedItemID == 0 ) 
                    && (eparty.member(i)->helpme.called))
                eparty.member(i)->helpme.called = false;
    }
//------------------------------------------------------------------------

    int PickVictim(bool hp){

        int tmp = -1;
        if (jparty.count()){
            tmp = rand() % jparty.count();
            if (hp){
                while ((jparty.member(tmp)->hp.def <= 0) ||
                        (jparty.member(tmp)->escaped()))
                    tmp = rand() % jparty.count();
            }
            else{
                while ((jparty.member(tmp)->mp.def<=0) ||
                        (jparty.member(tmp)->escaped()))
                    tmp = rand()%jparty.count();

            }
        }

        return tmp;
  }

   //----------------------------------------------------
    void EnemyAI(unsigned index){

        unsigned caller = 0;
        char buf[40];

        Creature* me = eparty.member(index); //monster pointer
        Strategy* AI = &me->_AI;

        //hp when u need to use hp up
        int alert_hp = (int)(((me->hp.max*1.0f)/100.0f)*50.0f);
        //mp when u need to use mp up
        int alert_mp = (int)(((me->mp.max*1.0f)/100.0f)*50.0f);
        //hp when u need to setup defense
        int defense_hp = (int)(((me->hp.max*1.0f)/100.0f)*10.0f);

        if (me->on_defense)
            me->on_defense = false;

        int antidote = me->inventory.findItem(3);
        int poison = me->inventory.findItem(2);

        //******** mp is running low for priest or mage ****
        if ((me->mp.max > 0) && (me->mp.def <= alert_mp)){ 

            if (AI->useSpellForMP){

                for (unsigned i = 0; i < AI->restoreMPSpells.count(); i++){
                    if (me->canCast(AI->restoreMPSpells[i].index, Spells)){
                        EnemyActions.set(index, CAST, 
                                         Entit.findFighter(HERO, PickVictim(false)),
                                         AI->restoreMPSpells[i].index);
                        return;
                    }
                }
            }
            if (AI->useItemForMP){

                for (unsigned i = 0; i < AI->restoreMPItems.count(); i++){
                    int item_ = me->inventory.findItem(AI->restoreMPItems[i].index);
                    if (item_ != -1){
                        EnemyActions.set(index, USE, 
                                         Entit.findFighter(MONSTER, index), item_);
                        return;
                    }
                }
            }
        }
        //*****helping friends*******************
        if (AI->helpFriends){

            int wantedItem = 0;
            wantedItem = me->inventory.findItem(me->helpme.wantedItemID);
            if ((me->helpme.called) && (wantedItem != -1)){
                HelpCaller(index, 0, caller);
                return;
            }
        }
        //********poison healing********************
        if (me->state == POISONED){

            if (me->canCast(4, Spells)){

                EnemyActions.set(index, CAST, 
                                 Entit.findFighter(MONSTER, index),
                                 4);
                return;
            }

            if (antidote != -1){
                EnemyActions.set(index, USE, 
                                 Entit.findFighter(MONSTER, index), antidote);
                return;
            }
            
            SendCall(3, "cure", index); //ask for antidote
        } 
        //********* healing ********
        if (me->hp.def <= alert_hp){
            if (AI->useSpellForHP){

                for (unsigned i = 0; i < AI->restoreHPSpells.count(); i++){
                    if (me->canCast(AI->restoreHPSpells[i].index, Spells)){
                        EnemyActions.set(index, CAST,
                                         Entit.findFighter(MONSTER, index),
                                         AI->restoreHPSpells[i].index);
                        return;
                    }
                }
            }

            if (AI->useItemForHP){
                for (unsigned i = 0; i < AI->restoreHPItems.count(); i++){
                    int item_ = me->inventory.findItem(AI->restoreHPItems[i].index);
                    if (item_ != -1){
                        EnemyActions.set(index, USE,  
                                         Entit.findFighter(MONSTER, index),
                                         item_);
                        return;
                    }
                }
                if (me->helpme.needhelp == false)
                    SendCall(0, "heal", caller);
            }
        } 
        //*******deffense**********
        if (me->hp.def < defense_hp)  {
            me->defend();
            sprintf(buf,"%s sets defense", me->name);
            Console.putText(buf);
            return;
        }
        //**********cast something*************
        if (AI->useSpellToAttack){
            for (unsigned i = 0; i < AI->attackSpells.count(); i++){
                if (me->canCast(AI->attackSpells[i].index, Spells)){
                    unsigned victim = PickVictim(true);
                    EnemyActions.set(index, CAST, 
                                     Entit.findFighter(HERO, victim),
                                     AI->attackSpells[i].index);
                    return;
                }
            }
        }

        if (poison != -1) {  //i have poison ?

            unsigned i = 0;
            while ((i < jparty.count())
                && (jparty.member(i)->state != HEALTHY))
                i++;
            
            if (jparty.member(i)->state == HEALTHY){
                EnemyActions.set(index, USE,  
                                 Entit.findFighter(HERO, i-1),
                                 poison);
                return;
            }
        }
        //physical atack
        if (jparty.alive())  {
            unsigned victim = PickVictim(true);
            EnemyActions.set(index, ATACK, Entit.findFighter(HERO, victim));

        }

    }

    //------------------------------------------------------------------------
    void CreateItemsMenu(unsigned x, unsigned y,
                          SelectMenu& ItemsMeniu,
                          Creature* dude
                         ){

        char countstr[5];
        char buf[30];

        SMenu meniu;

        if (dude){

            for (unsigned i=0; i < dude->inventory.itemGroupCount(); i++){

                Item* itm = 0;

                itm = dude->inventory.getItem(i);

                if (itm){

                    MenuItem nmi;


                    if ((itm->count) > 1)
                        sprintf(countstr, "%d", itm->count);
                    else
                        strcpy(countstr,"");

                    if ((itemData.info[itm->kind].isweapon) || 
                        (itemData.info[itm->kind].isarmor)){

                        sprintf(nmi.name, "%-3s %-s %d%c", countstr,
                                itemData.info[itm->kind].name, itm->health(),'%');
                        nmi.icon = (itemData.info[itm->kind].isarmor) ? 2 : 1;
                    }
                    else{
                        sprintf(nmi.name, "%-3s %s", countstr, 
                                itemData.info[itm->kind].name);
                        nmi.icon = 0;
                    }

                    nmi.id = itm->kind;


                    meniu.add(nmi);

                }
            }


            sprintf(buf,"%s's inventory:", dude->name);
            ItemsMeniu.destroy();
            meniu.setTitle(buf);
            ItemsMeniu.init(x, y, meniu, 0, 150);
            meniu.destroy();
        }

    }

  //-----------------------------------------------
    void CreateSpellMenu( SelectMenu& selectmeniu, Creature* dude, bool combat){
        SMenu meniu;

        if (dude){
            for (unsigned i = 0; i < dude->spellCount(); i++){
                MenuItem nmi;
                SpellFrame * s = dude->getSpell(i);

                if (s->enabled){

                    Spell * spell = &(Spells.spells[s->index]);

                    if (((combat)&&(spell->useInCombat))
                        ||((!combat)&&(spell->useInWorld))
                       ){

                        sprintf(nmi.name, "%s %3d MP",
                                spell->name, spell->cost);
                        //puts(nmi.opt);
                        nmi.id = s->index;
                        meniu.add(nmi);
                    }
                }

            }

            selectmeniu.destroy();
            if (meniu.count()){
                meniu.setTitle("Spells:");
                selectmeniu.init(180,80, meniu, 0);

            }
            meniu.destroy();
        }

    }
    //----------------------------------------------
    void CreateActionMenu(unsigned kas, SelectMenu& selMeniu){

        SMenu stuffas;

        Creature* hero = 0;

        hero = jparty.member(kas);

        if (hero){


            stuffas.add(ML.data[4].items[0]);

            //if hero class can't cast, don't include CAST in menu
            if (klases.get(hero->klass)->spells.count() > 0)
                stuffas.add(ML.data[4].items[1]);

            for (unsigned i = 1; i < 3; i++)    
                stuffas.add(ML.data[4].items[i+1]);

            //do not include RUN in boss combat
            if (!eparty.isBoss)
                stuffas.add(ML.data[4].items[4]);

            stuffas.add(ML.data[4].items[5]); //skip

            char buf[20];
            sprintf(buf,"%s's turn:", hero->name);

            ActionSelect.destroy();
            stuffas.setTitle(buf);
            ActionSelect.init((Sys.width/100)*72, Sys.height - 180, stuffas, 0);
            stuffas.destroy();
        }
    }


//----------------------------------------
void S_Atck(unsigned kas, bool& baigta){

        unsigned char sel;
        Party* party=0;


        if (!Combatselect.selected){
            if (!Combatselect.active())
                Combatselect.activate();
            Combatselect.getinput(globalKEY, Entit);
        }
        else {

            //unsigned firstEnemyIndex = Entit.findFighter(2, 1);
            sel = Entit.get(Combatselect.state)->nr;
            //printf("selected: %d\n", sel);
            if (Entit.get(Combatselect.state)->type == HERO) {
                party = &jparty;
            }
            else {
                party = &eparty;
            }

            if (party->member(sel)->state != DEAD)  {
                HeroActions.set(kas, ATACK, Combatselect.state);
                Combatselect.set(//firstEnemyIndex - 1,
                                 Entit, jparty.count(),
                                 eparty.count(),
                                 5, Sys.width - 200);
                Combatselect.deactivate();
                baigta = true;
            }
            else{
                Combatselect.set(//firstEnemyIndex - 1, 
                                 Entit, jparty.count(),
                                 eparty.count(),
                                 5, Sys.width - 200);
            }
        }

        if (Combatselect.canceled){
            //unsigned firstEnemyIndex = Entit.findFighter(2, 1);
            Combatselect.set(//firstEnemyIndex - 1,
                             Entit, jparty.count(),
                             eparty.count(),
                             5, Sys.width - 200);
            Combatselect.deactivate();
            ActionSelect.reset();
        }

}
//----------------------------------------------------
    void S_Cast(unsigned char kas, bool& baigta){


        if (!SpellMenu.selected) {
            if (!SpellMenu.active())
                SpellMenu.activate();
            SpellMenu.getInput(globalKEY, SS, _MouseX, _MouseY);

            if (SpellMenu.selected){
                unsigned spellID = SpellMenu.selection.items[SpellMenu.state].id;
                if (Spells.spells[spellID]._type == DOESDAMAGE){
                    Combatselect.set(Entit,
                                     jparty.count(),
                                     eparty.count(),
                                     5, Sys.width - 200,
                                     DAMAGE,
                                     kas); //select only alive

                }

            }
        }//spell finaly selected
        else {

            SpellMenu.deactivate();
            unsigned spellID = SpellMenu.selection.items[SpellMenu.state].id;
            //enough MP ?
            if (jparty.member(kas)->mp.def >= Spells.spells[spellID].cost){ 

                if ((!Combatselect.selected)
                        && (Spells.spells[spellID].needOponent)){
                    if (!Combatselect.active())
                        Combatselect.activate();
                    Combatselect.getinput(globalKEY, Entit);
                }
                else { //oponent selected
                    //Item/spell
                    HeroActions.set(kas, CAST, Combatselect.state, spellID); 

                    // HA_prieso_nr = Combatselect.state;

                    Combatselect.set(//jparty.count(), 
                                     Entit,
                                     jparty.count(), eparty.count(),
                                     5, Sys.width - 200);
                    Combatselect.deactivate();

                    baigta = true;
                    SpellMenu.reset();
                }

                if ((Combatselect.canceled)
                     && (Spells.spells[spellID].needOponent)){
                    Combatselect.set(//jparty.count(), 
                                     Entit,
                                     jparty.count(), eparty.count(),
                                     5, Sys.width - 200);
                    Combatselect.deactivate();
                    SpellMenu.reset();
                }

            }else{
                Console.putText("not enough MP");
                SpellMenu.reset();
                SpellMenu.deactivate();
            }


        }
        if (SpellMenu.canceled){
            SpellMenu.reset();
            SpellMenu.deactivate();
            ActionSelect.reset();
        }

    }
//---------------------------------------
void S_Use(unsigned char kas, bool& baigta){

    if (!ItemMenu.selected)  {

        if (!ItemMenu.active())
            ItemMenu.activate();
        ItemMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
        if (ItemMenu.selected){
            int itemID = ItemMenu.state;
            ItemInfo * item = 0;
            item = &(itemData.info[jparty.member(kas)->inventory.getItem(itemID)->kind]);
            if (item->_type == DOESDAMAGE){
                Combatselect.set(Entit,
                                 jparty.count(),
                                 eparty.count(),
                                 5, Sys.width - 200,
                                 DAMAGE,
                                 kas); //select only alive

            }

        }
    }
    else{ // item finally selected
        ItemMenu.deactivate();
        Creature * hero = 0;
        hero = jparty.member(kas);

        int itemID = ItemMenu.state;
        ItemInfo * item = 0;
        item = &(itemData.info[hero->inventory.getItem(itemID)->kind]);
        if (!item->useInCombat){
            Console.putText("Can't use this in combat!");
            ItemMenu.reset();
            return;
        }

        if ((!Combatselect.selected)
            && (item->needOponent)){
            if (!Combatselect.active())
                Combatselect.activate();
                Combatselect.getinput(globalKEY, Entit);
        }
        else{

            HeroActions.set(kas, USE,
                            Combatselect.state, itemID); //Item/spell

            Combatselect.set(//jparty.count(), 
                             Entit,
                             jparty.count(), eparty.count(),
                             5, Sys.width - 200);
            Combatselect.deactivate();

            baigta = true;
            ItemMenu.reset();
        }

        if ((Combatselect.canceled)
            && (item->needOponent)){
            Combatselect.set(Entit,
                             jparty.count(), eparty.count(),
                             5, Sys.width - 200);
            Combatselect.deactivate();
            ItemMenu.reset();
        }
    }
    if (ItemMenu.canceled)  {
        ItemMenu.reset();
        ItemMenu.deactivate();
        ActionSelect.reset();
    }
}
//----------------------------------------------
void S_Escape(unsigned kas, bool& over){
        int enemyAgility = 0;
        unsigned enemyCount = 0;
        int averageEnemyAgility = 0;
        char buf[100];

        Creature * hero = 0;
        hero = jparty.member(kas);
        if (hero){

            for (unsigned i = 0; i < Entit.count(); i++) {
                if (Entit.get(i)->type == 2)  {
                    enemyAgility += Entit.get(i)->agility;
                    enemyCount++;
                }
            }
            averageEnemyAgility = (enemyCount) ? 
                                  (enemyAgility / enemyCount) : 0;
            printf("hero agil %d average enemy %d\n",
                   hero->agil, averageEnemyAgility);
            if (hero->agil > averageEnemyAgility)  {
                sprintf(buf, "%s runs away", hero->name);
                Console.putText(buf);
                hero->escape();

            }
            else{
                sprintf(buf, "%s cannot escape", hero->name);
                Console.putText(buf);
            }

        }

        over = true;
    }

    //----------------------------------------------
    void HeroAction(unsigned kas, bool& baigta){

        char buf[50];
        //''''''''''''''''''''''''

        Creature* hero = jparty.member(kas);

        if (hero->on_defense)
            hero->on_defense = false;


        if (!ActionSelect.selected){ //action still not selected

            if (!ActionSelect.active())
                ActionSelect.activate();

            if (!QuitMenu.active())
                ActionSelect.getInput(globalKEY, SS, _MouseX, _MouseY);
            
            if (ActionSelect.selected){
                Selection = ActionSelect.state;
                ActionSelect.deactivate();

                //Cast action only for character who can cast spells
                if (klases.get(hero->klass)->spells.count() == 0)    
                    if (Selection > 0)  Selection++;

                if ((eparty.isBoss) && (Selection == 4))
                    Selection++;

                if (Selection == 0){

                    Combatselect.set(Entit,
                                     jparty.count(),
                                     eparty.count(),
                                     5, Sys.width - 200,
                                     DAMAGE,
                                     kas); //select only alive
                }

            }
        }
        else { //selected

            switch(Selection){
            case 0:S_Atck(kas, baigta); break;//atack
            case 1:S_Cast(kas, baigta); break;    //jei pasirinko cast
            case 2:{
                hero->defend();
                sprintf(buf, "%s sets defense", hero->name);
                Console.putText(buf);
                baigta = true;
            }
            break;
            case 3:S_Use(kas, baigta); break;  //use
            case 4: S_Escape(kas, baigta); break;
            case 5:{
                sprintf(buf, "%s skips turn", hero->name);
                Console.putText(buf);
                baigta = true;
            }
        }

      }


        if (ActionSelect.canceled) {
            if (!QuitMenu.active())
                QuitMenu.activate();
        }

    }

  //-------------------------------------------
    void EquipItem(unsigned sav, unsigned itemmeniuindex, ItemSlots slot){

        Creature * hero = 0;
        hero = jparty.member(sav);

        if (hero){
            switch(hero->equipItem(itemmeniuindex, slot, itemData)){
                case -2:{
                    Console.putText("Oops something went wrong!");
                } break;
                case -1:{
                    Console.putText("Must unequip first");
                } break;
                case 0:{
                    Console.putText("You can't equip this");
                } break;

                default: {
                    char buf[40];
                    Item * itm = 0;
                    switch (slot){
                        case RIGHTHAND: itm = &hero->rhand; break;
                        case LEFTHAND: itm = &hero->lhand; break;
                        case ARRMOR: itm = &hero->armor; break;
                    }
                    if (itm){
                        sprintf(buf,"%s equips %s", 
                                hero->name, itemData.info[itm->kind].name);
                        Console.putText(buf);
                    }
                }
            }
        }
    }

 //------------------------------------------------------
    void UnequipItem(unsigned who, ItemSlots slot){

        Creature * hero = 0;
        hero = jparty.member(who);
        if (hero){
            if (!hero->unequipItem(slot, itemData))
                Console.putText("Must equip first");
        }
    }

//-------------------------------------------------------------------------
    void M_EquipLHand(){
        EquipItem(G_MemberIndex, g_daiktas, LEFTHAND);
        EquipBody.deactivate();
        EquipBody.reset();
    }
    //------------------
    void M_EquipRHand(){
        EquipItem(G_MemberIndex, g_daiktas, RIGHTHAND);
        EquipBody.deactivate();
        EquipBody.reset();
    }
    //------------------
    void M_EquipArrmor(){
        EquipItem(G_MemberIndex, g_daiktas, ARRMOR);
        EquipBody.deactivate();
        EquipBody.reset();
    }
    //------------------------------------------
    void Inventory_Use(){
        Item* itm =  0;

        itm = jparty.member(G_MemberIndex)->inventory.getItem(g_daiktas);

        if (itm){

            //don't let to use item in adventure mode if it's not
            //suposed to be used
            if (!itemData.info[itm->kind].useInWorld){

                G_OponentIndex = 255;
                invsel.deactivate();
                invsel.reset();
                invsel.activate();

                return;
            }
        }

        if (G_OponentIndex == 255)  {

            partyfaces.getInput(globalKEY, jparty);
            if (!partyfaces.active())
                partyfaces.activate();

            if (partyfaces.selected){
                G_OponentIndex = partyfaces.face;
                partyfaces.reset(jparty);
                partyfaces.deactivate();
            }

            if (partyfaces.canceled){
                partyfaces.reset(jparty);
                partyfaces.deactivate();
                invsel.reset();
                invsel.activate();
            }

        }
        else{ //G_OponentIndex != 255

            UseItem(*jparty.member(G_MemberIndex),
                    g_daiktas, jparty, G_OponentIndex);

            ItemMenu.reset();
            G_OponentIndex = 255;
            G_MemberIndex = 255;
            invsel.reset();
            orderselect.reset();
        }
    }
    //------------------------------------------
    void Inventory_Give(){
        char buf[255];
        Item* itm = 0;
        Creature * dude = 0;
        dude = jparty.member(G_MemberIndex);
        itm = dude->inventory.getItem(ItemMenu.state);

        if (G_OponentIndex == 255)  {
            partyfaces.getInput(globalKEY,jparty);
            if (!partyfaces.active())
                partyfaces.activate();

            if (partyfaces.selected)  {
                G_OponentIndex = partyfaces.face;
                partyfaces.deactivate();
                g_item_count = 1;
                ItemCountedit.settext("1");
                partyfaces.reset(jparty);
            }

            if (partyfaces.canceled){
                partyfaces.reset(jparty);
                partyfaces.deactivate();
                invsel.reset();
                invsel.activate();
            }
        }
        else {
            //how much we will give
            if ((!ItemCountedit.entered) && (itm->count > 1)) {  

                ItemCountedit.incDecNumber((unsigned)globalKEY);
                ItemCountedit.getInput(globalKEY);
                if (!ItemCountedit.active()){
                    ItemCountedit.activate();
                }

                if (ItemCountedit.entered)  {
                    g_item_count = atoi(ItemCountedit.text());
                    if ((g_item_count > itm->count)){
                        ItemCountedit.reset();
                    }
                    else{
                        ItemCountedit.deactivate();
                    }
                }
            }
            else{
                ItemCountedit.reset();
                Item* tmp = dude->inventory.getItem(ItemMenu.state);
                int state = tmp->kind;
                if (dude->giveItems(*jparty.member(G_OponentIndex),
                ItemMenu.state, g_item_count, itemData)){

                    sprintf(buf,"%s puts %d %s into bag",
                    jparty.member(G_OponentIndex)->name,
                    g_item_count,
                    itemData.info[state].name);

                    Console.putText(buf);

                    g_daiktas=255;
                    ItemCountedit.reset();
                }
                else{
                    sprintf(buf,"%s cannot carry more", 
                            jparty.member(G_OponentIndex)->name);
                }

                ItemMenu.reset();
                G_OponentIndex = 255;
                G_MemberIndex = 255;
                invsel.reset();
                orderselect.reset();
            }
        }
    }
    //------------------------------------------
    void Inventory_Throw(){
        char buf[255];
        Item* itm = 0;
        Creature * dude = 0;
        dude = jparty.member(G_MemberIndex);
        itm = dude->inventory.getItem(ItemMenu.state);

        sprintf(buf,"%s throws %s away",
                dude->name, itemData.info[itm->kind].name);
        Console.putText(buf);
        dude->inventory.removeItemByIndex(ItemMenu.state);

        ItemMenu.reset();
        G_OponentIndex = 255;
        G_MemberIndex = 255;
        invsel.reset();
        orderselect.reset();
    }

    //------------------------------------------
    void HandleInventory(){

        if (G_MemberIndex == 255)  {

            if (!partyfaces.active())
                partyfaces.activate();

            partyfaces.getInput(globalKEY,jparty);    //select your hero

            if (partyfaces.selected)  {
                G_MemberIndex = partyfaces.face;
                partyfaces.deactivate();
                CreateItemsMenu(200,90, ItemMenu, jparty.member(G_MemberIndex));
                ItemMenu.activate();
            }

            if (partyfaces.canceled)  {
                partyfaces.reset(jparty);
                partyfaces.deactivate();
                orderselect.reset();
                G_MemberIndex = 255;
            }
        }

        else { //if face selected
            if (ItemMenu.canceled)  {
                ItemMenu.reset();
                ItemMenu.deactivate();
                partyfaces.reset(jparty);
                G_MemberIndex = 255;
            }

            if (!ItemMenu.selected)  {
                ItemMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
                if (ItemMenu.selected)  {

                    g_daiktas = ItemMenu.state;
                    partyfaces.skipdead = false;
                    partyfaces.reset(jparty);
                    ItemMenu.deactivate();

                    if (!invsel.active()){
                        invsel.activate();
                    }
                    EquipBody.selection.items[0].function = M_EquipLHand;
                    EquipBody.selection.items[1].function = M_EquipRHand;
                    EquipBody.selection.items[2].function = M_EquipArrmor;
                }
            }

            if (!invsel.selected)  {
                //we will choose what to do with selected item
                if (invsel.active())
                    invsel.getInput(globalKEY, SS, _MouseX, _MouseY);
                if (invsel.selected)  {
                    invsel.deactivate();
                }
            }
            else { //invsel selected


                switch(invsel.state){
                    case 0: Inventory_Use();
                    break;
                    //==================================
                    case 1: {   //Equip

                        int result = 0;
                        result = EquipBody.run(globalKEY, SS, _MouseX, _MouseY);
                        if (result < 0){ //canceled
                            invsel.reset();
                            invsel.activate();
                        }else
                        if (result){ //selected
                            ItemMenu.reset();
                            G_OponentIndex = 255;
                            G_MemberIndex = 255;
                            invsel.reset();
                            orderselect.reset();
                        }
                    }
                    break;
                    case 2: Inventory_Give();   //give
                    break;
                    case 3: Inventory_Throw();  //throw
                    break;
                    //=================================
                    default:{
                        invsel.reset();
                        partyfaces.reset(jparty);
                        partyfaces.deactivate();
                        ItemMenu.reset();
                        G_OponentIndex = 255;
                        G_MemberIndex = 255;
                        orderselect.reset();
                    }

                } //switch

            }//invsel selected

            if (invsel.canceled)  {
                //G_MemberIndex = 255;
                G_OponentIndex = 255;

                partyfaces.reset(jparty);           //jei atsaukem inv meniu
                partyfaces.deactivate();
                ItemMenu.reset();
                invsel.reset();
                invsel.deactivate();

            }
      }

   }



  //----------------------------------

    void ExitYesNo(){

        QuitMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
        if (QuitMenu.selected){

            switch(QuitMenu.state){
                case 0:{
                           //resets global variables and goes to title
                            ResetGlobals(); 
                            Mainmenu.reset();
                            Savemenu.reset();
                        }
                        break;

                default:{

                         }

            }
            QuitMenu.reset();
            QuitMenu.deactivate();

        }

    }

//--------------------------------------
//TODO: hmm maybe we should put this shit as method to Party class
    void TakeLoot(){

        int count;
        //we'll put items of defeated enemies here
        Inventory enemyStuff;
        for (unsigned b = 0; b < eparty.count(); b++)  {

            Creature* deadEnemy = eparty.member(b);

            for (unsigned i=0; i < deadEnemy->inventory.itemGroupCount(); i++){

                Item* tmp = deadEnemy->inventory.getItem(i);
                for (unsigned a = 0; a < tmp->count; a++)
                    enemyStuff.addItem(tmp, itemData);
            }

            Item wep;
            if (deadEnemy->lhand.kind != -1){
                wep.set(deadEnemy->lhand.kind, 
                        deadEnemy->lhand.health(), deadEnemy->lhand.count);
                enemyStuff.addItem(&wep, itemData);
            }
            if (deadEnemy->rhand.kind != -1){
                wep.set(deadEnemy->rhand.kind, 
                        deadEnemy->rhand.health(), deadEnemy->lhand.count);
                enemyStuff.addItem(&wep,itemData);
            }
            if (deadEnemy->armor.kind != -1){
                wep.set(deadEnemy->armor.kind, 
                        deadEnemy->armor.health(), deadEnemy->armor.count);
                enemyStuff.addItem(&wep,itemData);
            }
        }

        //-------------------
       /* for (unsigned long i = 0; i < enemyStuff.itemGroupCount(); i++){
            puts(itemData.info[enemyStuff.getItem(i)->kind].name);
            printf ("%u\n", enemyStuff.getItem(i)->count);
        }*/


        if (enemyStuff.itemGroupCount() > 0)  {
            printf("Item group count : %lu\n", enemyStuff.itemGroupCount());
            DArray<int> itemsToRemove;

            for (unsigned i = 0; i < jparty.count(); i++){
                printf("hero %u\n", i);
                Creature* hero = jparty.member(i);
                if (hero->state != DEAD)  { //if hero's alive
                    char buf[50];

                    for (unsigned w = 0; w < enemyStuff.itemGroupCount(); w++ ){
                        printf("item group %u\n", w);
                        Item* tmp = 0;
                        tmp = enemyStuff.getItem(w);
                        if (tmp){

                            count = tmp->count;
                            if (hero->inventory.itemWeight(itemData)
                                + (itemData.info[tmp->kind].weight * count) < 
                                hero->stamina()){

                                //prints stuff
                                if (count > 1){
                                  //  sprintf(buf,"%s gains %d %s", hero->name,
                                  //  count,
                                  //      itemData.info[tmp->kind].name);
                                  //   Console.putText(buf);

                                    sprintf(buf, "%d %ss", 
                                            count,
                                            itemData.info[tmp->kind].name);
                                    ItemGain.addItem(i, buf);

                                }
                                else{
                                   // sprintf(buf,"%s gains %s", hero->name,
                                  //      itemData.info[tmp->kind].name);
                                  //  Console.putText(buf);

                                    sprintf(buf, "%s",
                                            itemData.info[tmp->kind].name);
                                    ItemGain.addItem(i, buf);
                                }

                                for (int a = 0; a < count; a++){
                                    hero->inventory.addItem(tmp,itemData);
                                    //lets put it to array because if we try to 
                                    //destroy items right now we fuck up "w"
                                    //index
                                    itemsToRemove.add(tmp->kind);
                                }
                                puts(buf);

                            }//wheight
                        }
                    }

                    for (unsigned long z = 0; z < itemsToRemove.count(); z++){
                        enemyStuff.removeItemByKind(itemsToRemove[z]);
                    }
                    itemsToRemove.destroy();

                } //dead ?
            } //end of heroes for
        }

        //Console.putText("press a key...#");
        enemyStuff.destroy();
   }
//-----------------------------------------------------------
bool OpenMusic(const char * name){

    char tmp[255];
    sprintf(tmp, "%smusic/%s.ogg", Sys.basePath, name);

    if (!Stream.open(tmp)){
        puts("Loading from base/");
        sprintf(tmp, "%smusic/%s.ogg", "base/", name);
        if (!Stream.open(tmp))
            return false;
    }
    return true;
}
//----------------------------------------------------------
void PlayMusic(const char* name){
    if (Sys.useSound){
        Stream.release();
        if (OpenMusic(name))
            Stream.playback();
    }
}

//-----------------------------------------------------------
    void OnVictory(){


        char buf[50];

        Console.putText("Party defeated all enemies !");

        int enemyExp = 0;
        //int partyExp = 0;
        int pinigai  = 0;

        for (unsigned i = 0; i < eparty.count(); i++)  {

            Creature* deadEnemy = eparty.member(i);

            enemyExp += deadEnemy->exp;
            //adds coin(11) count
            pinigai  += deadEnemy->inventory.getItemCount(11);

            if (pinigai){
                //finds coins
                int indx = deadEnemy->inventory.findItem(11);

                for (int a = 0; a < pinigai; a++)
                    deadEnemy->inventory.removeItemByIndex(indx);
            }

        }
        //--------debug loging
        sprintf(buf, "enemy had %d coins", pinigai);
        GameLog.write(buf);

        //--------debug loging

        // for (unsigned i = 0; i < jparty.count(); i++){
        //    partyExp += jparty.member(i)->exp;
        //}


        sprintf(buf, "enemy exp sum: %d", enemyExp);
        GameLog.write(buf);
        //---

        
        int dalis = 0;
        int liko = 0;

        ItemGain.destroy();

        if (pinigai)  {

            sprintf(buf,"money from enemy : %d", pinigai);
            GameLog.write(buf);

            dalis = pinigai / jparty.alive();
            liko = pinigai - (dalis * jparty.alive());

            sprintf(buf,"party alive members : %d", jparty.alive());
            GameLog.write(buf);
            sprintf(buf,"money part : %d", dalis);
            GameLog.write(buf);
            sprintf(buf,"money left : %d", liko);
            GameLog.write(buf);

            sprintf(buf,"Party gains %d gold :", pinigai);
            Console.putText(buf);

            for (unsigned i = 0; i < jparty.count(); i++){

                Creature * dude = 0;
                dude = jparty.member(i);

                unsigned long money = dude->inventory.getItemCount(11);
                if (dude->state != DEAD){

                    Item babkes;
                    //coin
                    babkes.set(11, 0, 1);
                    for (int a = 0; a < dalis; a++)
                        dude->inventory.addItem(&babkes, itemData);
                }
                if (liko)  {
                    int i = 0;
                    while (dude->state == DEAD)
                        i++;
                    Item babkes;
                    //coin
                    babkes.set(11, 0, 1);

                    for (int a = 0; a < liko; a++)
                        dude->inventory.addItem(&babkes, itemData);
                }
                
                unsigned long moneyafter = dude->inventory.getItemCount(11);
                int moneydiff = (int)moneyafter - (int)money;
                if (moneydiff){
                    sprintf(buf, "%d gold", moneydiff);
                    ItemGain.addItem(i, buf);
                }
            } //for
        }


            for (unsigned i =0; i < jparty.count(); i++){

                Stat newstat;

                Creature* hero = jparty.member(i);

                newstat.oldEXP = hero->exp;
                newstat.gainEXP = 0;
                newstat.oldLevel = hero->level();
                newstat.newLevel = newstat.oldLevel;

                if ((hero->state != DEAD) && (!hero->escaped()))  {

                    int expGain = 0;
                    //expGain = enemyExp - hero->exp;

                     for (unsigned i = 0; i < eparty.count(); i++)  {

                        Creature* deadEnemy = eparty.member(i);
                        expGain += (int)(deadEnemy->exp * 
                                         deadEnemy->expValue);
                     }

                    if (expGain <= 0)
                        expGain = 2 * eparty.count();

                    newstat.gainEXP = expGain;

                    int oldlev = hero->level();
                    if ((hero->needexp() <= expGain) && (hero->level() < 100)){
                        hero->exp += expGain;
                        int lev = hero->level();
                        newstat.newLevel = lev;

                        int hp = hero->hp.max; int mp = hero->mp.max;
                        int at = hero->atck;   int df = hero->defn;
                        int ag = hero->agil;   int mg = hero->magp;
                        int sp = hero->sprt;

                        for (int i = 0; i < (lev - oldlev); i++)
                            hero->levelUp(Rases, klases);

                        int spelsenabled = hero->enablespells(klases, Spells);

                        hp = hero->hp.max - hp; mp = hero->mp.max - mp;
                        at = hero->atck - at;   df = hero->defn - df;
                        ag = hero->agil - ag;   mg = hero->magp - mg;
                        sp = hero->sprt - sp;
                        newstat.hpUp = hp;
                        newstat.mpUp = mp;
                        newstat.atckUp = at;
                        newstat.defUp = df;
                        newstat.agilUp = ag;
                        newstat.magpUp = mg;
                        newstat.sprtUp = sp;
                        newstat.newspells = spelsenabled;
                    }
                    else
                        if (expGain < hero->needexp())
                          hero->exp += expGain;
                }
                StatGain.addStat(newstat);
    }

    StatGain.setPos(Sys.width/2 - StatGain.width()/2, 50);
    ItemGain.setPos(Sys.width/2 - ItemGain.width()/2, 50);
    StatGain.activate();

    TakeLoot();
    PlayMusic(map.music);
}

//----------------------------------
//TODO: somehow remove this function
void SharedItems(Creature& crtr){

   crtr.inventory.destroy();
    if (crtr.klass<2){      //jei warrioris ar priestas
        Item sword;
        sword.set(13, 100, 1);
        GameLog.write("adding sword");
        crtr.inventory.addItem(&sword,itemData);     //duodam karda
    }
     else{
      Item dagger;
      dagger.set(12, 100, 1);
      GameLog.write("adding dagger");
      crtr.inventory.addItem(&dagger,itemData);    //kitiem dageri duodam
     }
     if (crtr.klass==0){       //jei warioris
      Item shield;

      shield.set(17, 100, 1);
      puts("adding shield");
      crtr.inventory.addItem(&shield,itemData);     //duodam skyda


     }

     Item item;
        
     //leather
     item.set(18, 100, 1);
     puts("adding more");
     crtr.inventory.addItem(&item,itemData);

    //small health up
     item.set(0, 0, 1);
     crtr.inventory.addItem(&item,itemData);
     //key
     item.set(10, 0, 1);
     crtr.inventory.addItem(&item,itemData);
     //antidote
     item.set(3, 0, 1);
     crtr.inventory.addItem(&item,itemData);
     //life water
     item.set(4, 0, 1);
     crtr.inventory.addItem(&item,itemData);

     if ((crtr.klass==1)||(crtr.klass==2))
        //magic up
        item.set(1, 0, 1);
     else 
        //small hp up
         item.set(0, 0, 1);
     crtr.inventory.addItem(&item,itemData);

     crtr.lhand.set(-1,0,0);
     crtr.rhand.set(-1,0,0);
     crtr.armor.set(-1,0,0);
}

//-------------------------------------------
void ApplyOpenedChests(){

        printf("Opened chests: %lu\n", EmptyChests.count());
        for (unsigned i = 0; i < EmptyChests.count(); i++){
            Chest * block = 0;

            block = EmptyChests.get(i);
            if (block){
                if (strcmp(block->mapname, Currentmap) == 0)
                    map.setTile(block->x, block->y, block->layer, block->tilenum);
            }
        }
    }
//----------------------------------------------
bool LoadArena(const char* path){
    char buf[255];
    GameLog.write("loading arena");
    sprintf(buf,"%sarenas/%s.arn", Sys.basePath, path);
    if (!Arena.load(buf, Sys.basePath)){
        sprintf(buf,"base/arenas/%s.arn", path);
        if (!Arena.load(buf, "base/"))
            return false;
    }
    LightPos = Arena.getLightOrigin();
    LightTo = Arena.getLightDest();

    MatrixLookAt(LightPos, LightTo,
                 Vector3D(0,1,0), LightViewMatrix);
    return true;
}

//----------------------------------------------
bool LoadCurrentMap(float startx, float starty,
                    const char* name, bool loadNpcs){

    Events.destroy();
    MonsterGroups.destroy();


    if (strcmp(Currentmap, name) != 0)
        strncpy(Currentmap, name, 255);

    jparty.x = (int)startx;
    jparty.y = (int)starty;

    char buf[255];
    char oldtilesetname[255];
    char oldbgmusicname[255];


    if (map.tileset)
        strncpy(oldtilesetname, map.tileset, 255);

    if (strlen(map.music))
        strncpy(oldbgmusicname, map.music, 255);
    else
        strncpy(oldbgmusicname, MainMenuMusic, 255);

    sprintf(buf, "Loading map %smaps/%s.wrd ...", Sys.basePath, Currentmap);
    GameLog.write(buf);
    sprintf(buf,"%smaps/%s.wrd", Sys.basePath, Currentmap);
    if (!map.load(buf)){
        GameLog.write("Failed...trying base");
        sprintf(buf,"base/maps/%s.wrd", Currentmap);
        if (!map.load(buf))
            return false;
    }
    GameLog.write("done");

    IsCurrentMap3D = (map.is3D())? true : false;

    if (strcmp(map.music, oldbgmusicname)!=0)
        PlayMusic(map.music);


    if (strcmp(map.tileset,oldtilesetname)!=0){
        Tileset3D.destroy();
        sprintf(buf,"Loading tileset %smodels/%s.rm2...",
                Sys.basePath, map.tileset);
        GameLog.write(buf);
            sprintf(buf,"%smodels/%s.rm2", Sys.basePath, map.tileset);

            if (!Tileset3D.load(buf)){
                sprintf(buf,"Loading tileset /base/models/%s.rm2...",
                        map.tileset);
                GameLog.write(buf);
                sprintf(buf,"base/models/%s.rm2", map.tileset);
                if (!Tileset3D.load(buf))
                    GameLog.write("FAIL...");
            }

            sprintf(buf,"Loading tileset %spics/%s.tga...",
                    Sys.basePath, map.tileset);
            GameLog.write(buf);
            sprintf(buf,"%s.tga", map.tileset);
            Tailsetas.loadFile(buf, 0, 32, 32, Sys.basePath, 0);
            GameLog.write("done");

        }

        //Load non player characters
        if (loadNpcs){
            npcnation.destroy();
            sprintf(buf,"%smaps/%s.npc", Sys.basePath, Currentmap);
            GameLog.write("Loading npcs...");
            if(!npcnation.load(buf, Sys.basePath)){
                GameLog.write("Failed, loading from base");
                sprintf(buf,"base/maps/%s.npc", Currentmap);
                npcnation.load(buf, Sys.basePath);
            }
            GameLog.write("done");
        }

        GameLog.write("Loading events...");
        sprintf(buf,"%smaps/%s.ent", Sys.basePath, Currentmap);
        if (!Events.load(buf)){
            GameLog.write("Failed, loading from base");
            sprintf(buf,"base/maps/%s.ent", Currentmap);
            Events.load(buf);
        }
        GameLog.write("done");

        GameLog.write("Loading monster groups...");
        sprintf(buf,"%smaps/%s.mgr", Sys.basePath, Currentmap);
        if (!MonsterGroups.load(buf)){
            GameLog.write("Failed, loading from base");
            sprintf(buf,"base/maps/%s.mgr", Currentmap);
            MonsterGroups.load(buf);
        }
        GameLog.write("done");


        if (loadNpcs){
            Npc tmp;

            sprintf(buf, "%s.tga", jparty.member(0)->npcsprite);
            puts("init tmp");
            tmp.init(jparty.x, jparty.y, 255, npcnation.count() + 1,"",
                     buf, "", "");
            npcnation.add(tmp, "");

        
            for (unsigned i = 1; i < jparty.count(); i++){
                sprintf(buf, "%s.tga", jparty.member(i)->npcsprite);

                tmp.init(npcnation.getNpc(npcnation.count()-i)->x(),
                         npcnation.getNpc(npcnation.count()-i)->y(),
                         255, npcnation.count() + 1,"",
                         buf, "", "");

                puts("add..");
                npcnation.add(tmp, "");
                puts("add ok.");
            }
        }

        jparty.membersxyInit((int)npcnation.getNpc(npcnation.count()-1)->x(),
                             (int)npcnation.getNpc(npcnation.count()-1)->y());

        Cam.x = (int)npcnation.getNpc(npcnation.count()-jparty.count())->x();
        Cam.y = (int)npcnation.getNpc(npcnation.count()-jparty.count())->y();
        CenterMap(Cam.x, Cam.y);
        Focus3DCam(Cam.x, Cam.y,
                   map.camHeight, map.camDistance, map.camPitch );

        puts("Loaded.");
        return true;
    }
//----------------------------------------------
void Onward(const char * path){

    if (!LoadState(jparty, innparty,
                   npcnation,
                   Kintamieji,  EmptyChests,
                   Currentmap, path, itemData,
                   Sys.basePath)){
        M_NewGame();
    }
    else {

        GameState = WALKING;
        char buf[50];

        for (unsigned i = 0; i < jparty.count(); i++){
            Creature* hero = jparty.member(i);
            sprintf(buf, "Let(\"hero%dname\",\"%s\")",i, hero->name);
            Interpret(buf);
            printf("HERO FACE %s\n", hero->faceTileSet);
            sprintf(buf,"Let(\"hero%dface\",\"%s\")",i, hero->faceTileSet);
            Interpret(buf);
            sprintf(buf,"Let(\"hero%dfaceID\",\"%d\")",i, hero->faceID);
            Interpret(buf);
        }
        //Well, all npcs loaded from savegame, so we can disable
        //npc loading (false)
        if (!LoadCurrentMap(jparty.x, jparty.y, Currentmap, false)){
            EmptyChests.destroy();
            Kintamieji.destroy();
            jparty.destroy();
            GameState = CREATEPARTY;
            CreateHeroMenu.activate();
            return;
        }

        for (unsigned i = 0; i < jparty.count(); i++){
            unsigned index = npcnation.count()-(jparty.count() - i); 
            jparty.membersxy[i].set((float)npcnation.getNpc(index)->x(),
                                    (float)npcnation.getNpc(index)->y(),0);
        }
        ApplyOpenedChests();

    }
}

//-------------------------------------------------
void BuyItem (unsigned kind,  bool& stop, SelectMenu& parent) {

        unsigned moneyplace;
        int money;

        if (!partyfaces.selected)  {
            if (!partyfaces.active())
                partyfaces.activate();
            partyfaces.getInput(globalKEY, jparty);

            if (partyfaces.selected)  {
                Creature * dude = 0;
                dude = jparty.member(partyfaces.face);
                partyfaces.deactivate();
                money = dude->inventory.getItemCount(11);
                char buf[30];
                sprintf(buf, "%s %d G buy:",
                        dude->name, money);
                MerchantItems.setTitle(buf);

            }
        }
        else {


            if (!MerchantItems.selected){
                if (!MerchantItems.active())
                    MerchantItems.activate();
                MerchantItems.getInput(globalKEY, SS,
                                       _MouseX, _MouseY);
                if (MerchantItems.selected)
                    MerchantItems.deactivate();
            }
            else{
                if (!ItemCountedit.entered){
                    if (!ItemCountedit.active())
                        ItemCountedit.activate();

                   
                    ItemCountedit.incDecNumber((unsigned)globalKEY);
                    ItemCountedit.getInput(globalKEY);
                   

                    if (ItemCountedit.entered){
                        int count = atoi(ItemCountedit.text());
                        if (count < 1){
                            ItemCountedit.reset();
                        }
                    }
                }

                if (ItemCountedit.canceled){
                    ItemCountedit.reset();
                    ItemCountedit.deactivate();
                    MerchantItems.reset();
                    MerchantItems.activate();
                }
            }

            if (MerchantItems.canceled){
                MerchantItems.deactivate();
                MerchantItems.reset();
                partyfaces.reset(jparty);
            }
        }


        if (partyfaces.canceled){
            partyfaces.reset(jparty);
            partyfaces.deactivate();
            parent.reset();
            parent.activate();

        }

        //-------

        if ((MerchantItems.selected)&&(ItemCountedit.entered))  {

            gs_daiktas = Merch.get(MerchantItems.state)->itemIndex;
            int markup = Merch.get(MerchantItems.state)->markup;
            MerchantItems.deactivate();

            Creature * dude = 0;
            dude = jparty.member(partyfaces.face);

            int itemsWeight = itemData.info[gs_daiktas].weight;
            int inventoryWeight = dude->inventory.itemWeight(itemData);
            int count = atoi(ItemCountedit.text());

            if ( inventoryWeight + itemsWeight * count
                <= (int)dude->stamina())  {

                if (gs_daiktas != 255)  {
                    money = dude->inventory.getItemCount(11);
                    printf("money: %d\n", money);

                    int totalAmount = (itemData.info[gs_daiktas].cost + markup)
                                      * count;

                    if  (money - totalAmount >= 0)  {

                        char buf[50];
                        sprintf(buf, "%s bought %d %s for %d G#",
                                dude->name,
                                count,
                                itemData.info[gs_daiktas].name,
                                totalAmount);

                        Console.putText(buf);
                        Item tmp;
                        tmp.set(gs_daiktas, 100, 1);
                        for (int i = 0; i < count ; i++)
                            dude->inventory.addItem(&tmp, itemData);
                        //finds where money is, 11 - money id
                        moneyplace = dude->inventory.findItem(11);

                        for (int i = 0; i < totalAmount; i++)
                            dude->inventory.removeItemByIndex(moneyplace);
                    }
                    else
                        Console.putText("Not enough money!#");
                }
            }
            else
                Console.putText("You can't carry that much#");

            ItemCountedit.deactivate();
            partyfaces.reset(jparty);
            ItemCountedit.reset();
            stop = true;
            gs_daiktas = 255;
            MerchantItems.reset();
        }
    }
    //---------------------------------------------------
    void SellItem(bool& stop, SelectMenu& parent){

        unsigned char itm;
        int tmp = 0;

        stop = false;
        if (!partyfaces.selected){
            partyfaces.getInput(globalKEY, jparty);
            if (!partyfaces.active())
                partyfaces.activate();

            if (partyfaces.selected)  {
                CreateItemsMenu(10, 50, ItemMenu,
                                jparty.member(partyfaces.face));
                g_item_count = 1;
                partyfaces.deactivate();
            }

            if (partyfaces.canceled){
                parent.reset();
                parent.activate();
                partyfaces.reset(jparty);
                partyfaces.deactivate();
            }
        }
        else {

            Creature * dude = jparty.member(partyfaces.face);

            if (! ItemMenu.selected)  {
                ItemMenu.activate();
                ItemMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
                if (ItemMenu.selected){
                    ItemMenu.deactivate();
                }

            }
            else { //selected item to sell
                unsigned itemID = ItemMenu.selection.items[ItemMenu.state].id;

                if ((!ItemCountedit.entered)&&
                    (dude->inventory.getItemCount(itemID) > 1)){

                    ItemCountedit.incDecNumber((unsigned)globalKEY);
                    ItemCountedit.getInput(globalKEY);
                    ItemCountedit.activate();

                    if (ItemCountedit.entered)  {
                        g_item_count = atoi(ItemCountedit.text());
                        if ((tmp) ||
                            (g_item_count > dude->inventory.getItemCount(itemID))
                            || (!g_item_count)
                            ){
                            ItemCountedit.reset();
                        }
                        else{
                            ItemCountedit.deactivate();
                        }
                    }
                }
                else { //item count entered or it's just one item
                    stop = true;
                    Item* tmp = dude->inventory.getItem(ItemMenu.state);
                    itm = tmp->kind;
                    char buf[50];
                    sprintf(buf,"%s sold %d %s for %dG#", dude->name,
                                g_item_count, itemData.info[itm].name,
                                g_item_count*(itemData.info[itm].cost-1));
                                Console.putText(buf);

                    Item gold;
                    gold.kind = 11;
                    //puts gold in the bag
                    for (unsigned i = 0; i < itemData.info[itm].cost * g_item_count; i++)
                        dude->inventory.addItem(&gold, itemData);


                    for (unsigned i = 0; i < g_item_count; i++)
                        dude->inventory.removeItemByIndex(ItemMenu.state);

                    ItemMenu.reset();
                    partyfaces.reset(jparty);
                    ItemCountedit.reset();
                }

            } //selected item to sell end

            //if item menu is canceled
            if (ItemMenu.canceled){
                ItemMenu.reset();
                ItemMenu.deactivate();
                partyfaces.reset(jparty);
            }
        }
    }
  //-----------------------------------------
    void BuildInnPartyMeniu(){
        SMenu pmenu;

        for (unsigned i = 0; i < innparty.count(); i++){
            MenuItem itm;
            strcpy(itm.name, innparty.member(i)->name);
            pmenu.add(itm);
        }

        InnPartyMeniu.destroy();
        pmenu.setTitle("Heroes at inn:");
        InnPartyMeniu.init(10, 10, pmenu, 0);

    }

  //------------------------------------------
    void TheInn(){

        if (!InnMenu.selected)  {

            if (!InnMenu.active())
                InnMenu.activate();

            InnMenu.getInput(globalKEY, SS, _MouseX, _MouseY);

            if (InnMenu.selected){
                InnMenu.deactivate();
                if (InnMenu.state==1){
                    BuildInnPartyMeniu();
                }
            }

        }
        else {

            switch(InnMenu.state){
            case 0:{
                jparty.rest();
                fader=0;
                char buf[255];
                sprintf(buf, "%sdata/wakeup.txt", Sys.basePath);
                Console.readFile(buf);
                ENCinn=false;
                InnMenu.reset();
            }
            break;

            case 1:{ //add
                //some guys are in the inn at the moment
                if (InnPartyMeniu.selection.count()){

                    if (!InnPartyMeniu.selected){

                        if (!InnPartyMeniu.active())
                            InnPartyMeniu.activate();

                        InnPartyMeniu.getInput(globalKEY, SS, _MouseX, _MouseY);
                        if (InnPartyMeniu.selected){
                            InnPartyMeniu.deactivate();
                        }
                    }
                    else{
                        int guest=InnPartyMeniu.state;
                        GameLog.write("ading member");
                        jparty.addMember(*innparty.member(guest), Sys.basePath);
                        jparty.membersxy[jparty.count() - 1] = jparty.membersxy[jparty.count() - 2];
                        GameLog.write("member added");
                        char buf[50];
                        sprintf(buf,"%s joins the party",innparty.member(guest)->name);
                       Console.putText(buf);
                        GameLog.write("adding npc");
                        Npc noob;
                        sprintf(buf,"%s.tga", innparty.member(guest)->npcsprite);
                        noob.init(npcnation.getNpc(npcnation.count() - 1)->x(),
                                  npcnation.getNpc(npcnation.count() - 1)->y(),
                                  255,
                                  npcnation.count(),
                                  "", buf, "", "");
                        npcnation.add(noob, "");
                        GameLog.write("npc added");
                        GameLog.write("removing member");
                        innparty.removeMember(guest);
                        GameLog.write("member removed");

                        ENCinn=false;
                        InnMenu.reset();
                        GameLog.write("Done");
                    }

                    if (InnPartyMeniu.canceled){
                        InnPartyMeniu.deactivate();
                        InnPartyMeniu.reset();
                        InnMenu.reset();
                    }
                }
                else {//no one's there
                    ENCinn=false;
                    InnMenu.reset();
                }
            }

            break;

            case 2:{ //leave

                if (jparty.count() > 1) {
                    if (!partyfaces.selected){
                        if (!partyfaces.active())
                            partyfaces.activate();

                        partyfaces.getInput(globalKEY,jparty);

                        if (partyfaces.selected)
                            partyfaces.deactivate();
                    }
                    else{
                            int hero=partyfaces.face;

                            innparty.addMember(*jparty.member(hero),
                                               Sys.basePath);

                            char buf[50];
                            sprintf(buf,"%s stays at inn",
                                    jparty.member(hero)->name);
                            Console.putText(buf);
                            jparty.removeMember(hero);
                            printf("hero num: %u\n", hero);
                            unsigned membernpcID = npcnation.getNpc(npcnation.count() 
                                                   - (jparty.count() - (hero-1) ))->ID();
                            printf("npc ID: %u\n", membernpcID);
                            npcnation.kill(membernpcID);
                            for (unsigned long i = 0; i < jparty.count(); i++){
                                npcnation.getNpc(npcnation.count() - (i+1))->setClock(0);
                                npcnation.getNpc(npcnation.count() - (i+1))->stop();
                            }
                            partyfaces.reset(jparty);
                            ENCinn=false;
                            InnMenu.reset();
                    }

                    if (partyfaces.canceled){
                        partyfaces.deactivate();
                        partyfaces.reset(jparty);
                        InnMenu.reset();
                    }

                }
                else{
                    partyfaces.reset(jparty);
                    ENCinn=false;
                    InnMenu.reset();
                }


            }
            break;
        }

    }// selected = true

        if (InnMenu.canceled){
            InnMenu.deactivate();
            InnMenu.reset();
            partyfaces.reset(jparty);
            ENCinn=false;
        }

    }

  //------------------------------------------
    void SmithShop(){

        bool secondary=false;

        if (!SmithMeniu.active())
            SmithMeniu.activate();
        if ((! SmithMeniu.selected)&&(! SmithMeniu.canceled))  {
            SmithMeniu.getInput(globalKEY, SS, _MouseX, _MouseY);
        }
        else {
            switch(SmithMeniu.state){
                case 0:  BuyItem(2,secondary, SmithMeniu); break;   //buy some weapons maybe
                case 1:  SellItem(secondary, SmithMeniu);  break;
                case 2:  secondary = true; break;
            }
            if ((SmithMeniu.canceled)||(secondary))  {
                SmithMeniu.deactivate();
                SmithMeniu.reset();
                partyfaces.reset(jparty);
                partyfaces.deactivate();
                ENCsmith = false;
            }

        }
  }
//----------------------------------------
void Merchantshop(){

        bool secondary=false;
        if (!merchantmeniu.active())
            merchantmeniu.activate();
        if ((!merchantmeniu.selected)&&(! merchantmeniu.canceled))  {
            merchantmeniu.getInput(globalKEY, SS, _MouseX, _MouseY);
            if (merchantmeniu.selected)
                merchantmeniu.deactivate();
        }
        else{
            switch(merchantmeniu.state){
                case 0:  BuyItem(1,secondary, merchantmeniu); break;  //pirksim itemsus
                case 1:  SellItem(secondary, merchantmeniu);
            }
            if ((merchantmeniu.canceled)||(secondary))  {
                merchantmeniu.reset();
                merchantmeniu.deactivate();
                partyfaces.deactivate();
                ENCmersh=false;
            }
        }
    }

//--------------------------------------------
//Party encounters enemy
void Encounter(unsigned index, 
               const char* combatTune){

    atacked = false;
    GameState = COMBAT_INTRO;

    char buf[255];

    strcpy(buf,"Let(\"Victory\",\"false\"");

    PlayMusic(combatTune);

    Interpret(buf);
    if (Console.height() < 160)
        Console.setHeight(160);
    sprintf(buf, "%sdata/encounter.txt", Sys.basePath);
    Console.readFile(buf);

    jparty.resetEscaped();

    char temp[255];
    sprintf(temp, "Generating enemy index: %u", index);
    GameLog.write(temp);
    EnemyGenerate(eparty, *MonsterGroups.get(index));
    GameLog.write("done");

    //PS.addPoisonCloud(Vector3D( 4.0f, 2.0f, -0.5f),
    //                  Pics.getname(Pics.findpic("particles/blood.tga")));

    //Sets initial fighter coordinates in arena
    Entit.build(jparty,eparty, EnemyActions, HeroActions);
    Combatselect.set(//jparty.count(),
                     Entit, jparty.count(),
                     eparty.count(),
                     5, Sys.width - 200);
}
//------------------------------------
void LookForEnemy(){

    unsigned groupindex = 0;

    groupindex = map.getMonsterGroup(jparty.x/32, jparty.y/32);

    if (groupindex != 0){

        if (jparty.steps == StepsToEncounter) {
            if ((!atacked) && (GameState != COMBAT) )
                atacked=true;
        }

        if (atacked){
            GameState = BEGIN_COMBAT;
            if (Sys.useSound)
                SS.playsound(0, false);
            Combat_groupindex = groupindex - 1;
            //Encounter(groupindex - 1);
        }
    }
    else
        jparty.steps=0;
}
 //------------------------------------
void CheckVictory(){
    if (eparty.alive() == 0)  {

        Victory = true;
        GameState = VICTORY;
    }
}

//-----------------------------------
void CMD_SETCOLLISION(Arguments& args){
    if (args.count() < 3){
        GameLog.write("SetCollision: not enough args");
        return;
    }

    int x = (unsigned)atoi(args.variables[0].s);
    int y = (unsigned)atoi(args.variables[1].s);
    int state = atoi(args.variables[2].s);

    map.setWalkable( x, y, (state)? true : false);

}
//-----------------------------------
//Interprets command mvcam(x,y)
//x,y are 2D camera focus

void CMD_MVCAM(Arguments& args){

    unsigned short x = atoi(args.variables[0].s);
    unsigned short y = atoi(args.variables[1].s);
    Cam.x=x;
    Cam.y=y;
    CenterMap(Cam.x,Cam.y);
    Focus3DCam(Cam.x, Cam.y,
               map.camHeight, map.camDistance, map.camPitch );

}
//-----------------------------------
void CMD_PUSHCAM(Arguments& args){

    int dirx,diry,step;

    dirx=atoi(args.variables[0].s);
    diry=atoi(args.variables[1].s);
    step=atoi(args.variables[2].s);

    MapPushdirx = dirx;
    MapPushdiry = diry;
    MapPushsteps = step;

}
//-----------------------------------
void CMD_SETCAMUP(Arguments& args){

    if (args.count() < 3){
        GameLog.write("Not enough arguments for SetCamUp");
        return;
    }
    float x = atof(args.variables[0].s);
    float y = atof(args.variables[1].s);
    float z = atof(args.variables[2].s);
    Cam3D.setup(Vector3D(x, y, z ));
}
//-----------------------------------
void CMD_SETCAMPOS(Arguments& args){
    if (args.count() < 3){
        GameLog.write("Not enough arguments for SetCamPos");
        return;
    }
    float x = atof(args.variables[0].s);
    float y = atof(args.variables[1].s);
    float z = atof(args.variables[2].s);
    Cam3D.setpos(Vector3D(x, y, z));
}
//-----------------------------------
void CMD_SETCAMDIR(Arguments& args){
    if (args.count() < 3){
        GameLog.write("Not enough arguments for SetCamDir");
        return;
    }
    float x = atof(args.variables[0].s);
    float y = atof(args.variables[1].s);
    float z = atof(args.variables[2].s);
    Cam3D.setdir(Vector3D(x, y, z ));

}
//-----------------------------------
void CMD_SETCAMRIGHT(Arguments& args){
    if (args.count() < 3){
        GameLog.write("Not enough arguments for redir SetCamRight");
        return;
    }
    float x = atof(args.variables[0].s);
    float y = atof(args.variables[1].s);
    float z = atof(args.variables[2].s);
    Cam3D.setright(Vector3D(x, y, z));

}
//-----------------------------------
void CMD_REDIR(Arguments& args){
    if (args.count() < 3){
        GameLog.write("Not enough arguments for redir");
        return;
    }
    unsigned id = (unsigned)atoi(args.variables[0].s);
    int x = atoi(args.variables[1].s);
    int y = atoi(args.variables[2].s);

    if ((id <= npcnation.count()))  {
        npcnation.getNpc(id)->redir(x, y);
    }
    else
        GameLog.write("ilegal NPC index");

}

//-----------------------------------
//Interprets command MovNpc(x,y)
//x,y are new coordinates

void CMD_MVNPC(Arguments& args){

    unsigned id=atoi(args.variables[0].s);
    int x=atoi(args.variables[1].s);
    int y=atoi(args.variables[2].s);

    if ((id <= npcnation.count()))  {
        npcnation.getNpc(id)->movexy(x*32, y*32);
    }
    else
        GameLog.write("ilegal NPC index");
}

//------------------------------------
//PushNpc(sprID,dir,count)
    void CMD_PSNPC(Arguments& args){

        unsigned  id    = atoi(args.variables[0].s);
        unsigned  diras = atoi(args.variables[1].s);
        unsigned  count = atoi(args.variables[2].s);

        if (id < npcnation.count()){
            Npc * guy = 0;
            guy = npcnation.getNpc(id);

            if (guy->stepcount == 1)  {
                guy->setClock(0);
                guy->stepcount = count * 32;
                guy->redir(diras, map, npcnation);
            }
        }
        else
            puts("Illegal NPC index");

    }

//------------------------------------------
    void CMD_DRAINHP(Arguments& args){ //DrainHP(caster,oponent,power)

        unsigned caster = (unsigned)atoi(args.variables[0].s);
        unsigned oponent = (unsigned)atoi(args.variables[1].s);
        int power= atoi(args.variables[2].s);

        char buf[100];

        Creature* cas = 0;
        Creature* op = 0;
        unsigned oponentNr = 0;
        Party * p = 0;

        if (caster >= jparty.count()){
            cas = eparty.member(caster - jparty.count());
        }
        else{
            cas = jparty.member(caster);
        }
        if (oponent >= jparty.count()){
            op = eparty.member(oponent - jparty.count());
            oponentNr =(oponent - jparty.count());
            p = &eparty;
        }
        else{
            op = jparty.member(oponent);
            p = &jparty;
            oponentNr = oponent;
        }

        int result = 0;
        result = cas->drainPoints(*op, power, false);
        sprintf(buf,"%s drained %d HP", cas->name,result);
        Console.putText(buf);
        if ((op->hp.def <= 0) && (op->state != DEAD))  {
            p->killMember(oponentNr);
            sprintf(buf,"%s died", op->name);
            Console.putText(buf);
        }


    }
//----------------------------------------------------
    void CMD_DRAINMP(Arguments& args){ //DrainMP(caster,oponent,power)

        unsigned caster = (unsigned)atoi(args.variables[0].s);
        unsigned oponent = (unsigned)atoi(args.variables[1].s);
        int power= atoi(args.variables[2].s);

        char buf[100];

        Creature* cas = 0;
        Creature* op = 0;
       
        if (caster >= jparty.count()){
            cas = eparty.member(caster - jparty.count());
        }
        else{
            cas = jparty.member(caster);
        }
        if (oponent >= jparty.count()){
            op = eparty.member(oponent - jparty.count());
        }
        else{
            op = jparty.member(oponent);
        }

        int result = 0;
        result = cas->drainPoints(*op, power, true);
        sprintf(buf,"%s drained %d MP", cas->name,result);
        Console.putText(buf);
       

    }

//------------------------------------------
    void CMD_RESTOREHP(Arguments& args){


        unsigned oponent = (unsigned)atoi(args.variables[0].s);
        int points = atoi(args.variables[1].s);
        int dif=0;
        printf("oponent: %u\n", oponent);
        printf("points: %d\n", points);
        if (oponent < jparty.count())
            dif = jparty.member(oponent)->restorePoints(true, points);
        else
            dif = eparty.member(oponent - jparty.count())->restorePoints(true, points);

        char buf[40];
        sprintf(buf,"%s changes by %d","hp",dif);
        Console.putText(buf);

    }
//-------------------------------------------
    void CMD_RESTOREMP(Arguments& args){

        unsigned oponent = (unsigned)atoi(args.variables[0].s);
        int points = atoi(args.variables[1].s);
        int dif = 0;
        if (oponent < jparty.count())
            dif = jparty.member(oponent)->restorePoints(false, points);
        else
            dif = eparty.member(oponent - jparty.count())->restorePoints(false, points);

        char buf[40];
        sprintf(buf,"%s changes by %d", "mp", dif);
        Console.putText(buf);

    }

//-----------------------------------------
    void CMD_SCAN(Arguments& args){

        unsigned oponent = (unsigned)atoi(args.variables[0].s);
        //printf("oponent %u\n",oponent);
        if (oponent < jparty.count()){

            ScanCreature(*jparty.member(oponent));

        }
        else
            ScanCreature(*eparty.member(oponent-jparty.count()));

    }
//-------------------------------------------
    void CMD_RESURECT(Arguments& args){
        unsigned oponent = (unsigned)atoi(args.variables[0].s);
        int percent=atoi(args.variables[1].s);

        if (oponent < jparty.count())
            jparty.member(oponent)->resurect(percent);
        else
            eparty.member(oponent - jparty.count())->resurect(percent);
    }
//------------------------------------------------
//Elemental(oponent, Name, power)
//------------------------------------------
    void CMD_ELEMENTAL(Arguments& args){

        unsigned oponent = (unsigned)atoi(args.variables[0].s);

        int power = atoi(args.variables[2].s);
        char buf[50];
        int dmg=0;
        if (oponent < jparty.count()){
            dmg = jparty.member(oponent)->elementalDamage(power, args.variables[1].s);
            sprintf(buf,"%s cause %dp dmg to %s", args.variables[1].s, dmg,
                    jparty.member(oponent)->name);
            Console.putText(buf);
            if ((jparty.member(oponent)->hp.def <= 0) 
                 && (jparty.member(oponent)->state != DEAD))  {

                jparty.killMember(oponent);
                sprintf(buf,"%s died", jparty.member(oponent)->name);
                Console.putText(buf);
            }
        }
        else{
            unsigned opIndex = oponent - jparty.count();

            dmg = eparty.member(opIndex)->elementalDamage(power, 
                                                          args.variables[1].s);
            sprintf(buf, "%s cause %dp dmg to %s", args.variables[1].s, dmg,
                    eparty.member(opIndex)->name);
            Console.putText(buf);
            if ((eparty.member(opIndex)->hp.def <= 0) 
                && (eparty.member(opIndex)->state != DEAD))  {

                eparty.killMember(opIndex);
                sprintf(buf,"%s died", eparty.member(opIndex)->name);
                Console.putText(buf);
            }

        }

    }
//-----------------------------------------
    void CMD_ANTIDOTE(Arguments& args){

        unsigned oponent = (unsigned)atoi(args.variables[0].s);
        if (oponent < jparty.count()){
            if (jparty.member(oponent)->state == POISONED)
                jparty.member(oponent)->state = HEALTHY;
        }
        else{
            unsigned opindex = oponent - jparty.count();
            if (eparty.member(opindex)->state == POISONED)
                eparty.member(opindex)->state = HEALTHY;
        }
    }
//-----------------------------------------------
 void CMD_POISON(Arguments& args){

        unsigned oponent = (unsigned)atoi(args.variables[0].s);
        if (oponent < jparty.count()){
            if (jparty.member(oponent)->state == HEALTHY)
                jparty.member(oponent)->state = POISONED;
        }
        else{
            unsigned opindex = oponent - jparty.count(); 
            if (eparty.member(opindex)->state == HEALTHY)
                eparty.member(opindex)->state = POISONED;
        }
    }

//--------------------------------------------
    void CMD_ESCAPE(Arguments & args){
        End_combat();
        Console.putText("party escaped");
    }

//---------------------------------------------
//Say(Name,whattosay,facename,faceID)
    void CMD_SAY(Arguments& args){

        if (args.count() < 6){
            GameLog.write("Not enough arguments!");
            return;
        }

        if (!conversation.active())  {

            int x = atoi(args.variables[0].s);
            int y = atoi(args.variables[1].s);
            conversation.setpos((int)(x * (Sys.width / 640.0f)) , y * (Sys.height / 480));
            conversation.clear();
            char buf[255];
            sprintf(buf,"%s#", args.variables[3].s);
            conversation.putText(buf);
            Talker.change(conversation.getX() - 80, conversation.getY() - 2,
                          args.variables[2].s, Pics.findByName(args.variables[4].s),
                          atoi(args.variables[5].s));
        } 
    }

//---------------------------------------------------------
    void CMD_INN(Arguments& args){
        ENCinn = true;
    }

//-------------------------------------------

    void CMD_SHOP(Arguments& args){
        if (args.count()){

            char buf[255];
            sprintf(buf, "%s%s", Sys.basePath, args.variables[0].s);
            if (!Merch.load(buf)){
                sprintf(buf, "base/%s", args.variables[0].s);
                if (!Merch.load(buf))
                    return;
            }

            Merch.buildMenu(MerchantItems, itemData);

            ENCmersh = true;
        }
        else
            GameLog.write("Shop: not enough args");
    }
//-------------------------------------------
    void CMD_SMITH(Arguments& args){
        if (args.count()){

            char buf[255];
            sprintf(buf, "%s%s", Sys.basePath, args.variables[0].s);
            if (!Merch.load(buf)){
                sprintf(buf, "base/%s", args.variables[0].s);
                if (!Merch.load(buf))
                    return;
            }

            Merch.buildMenu(MerchantItems, itemData);

            ENCsmith = true;
        }
        else
            GameLog.write("Smith: not enough args");

    }

//--------------------------------------------
//if(someth=someth)

    void CMD_IF(const char * cmdline, unsigned char cmdlen){

        unsigned char i;
        char val1[20];
        char val2[20];

        //GameLog.write("It's if...");

        if (cmdline[cmdlen]=='(')  {
            i=cmdlen+1;
            char sep=0;
            GetValue(cmdline,i,"=",val1,&sep);
            GetValue(cmdline,i,")",val2,&sep);

            //TODO: invetigate, might cause bugs
            if (strcmp(val1,"")!=0){
                //GameLog.write(val1);
                //GameLog.write(val2);
                char* reiksme = Kintamieji.getValue(val1);
                if (reiksme){
                    if (strcmp(reiksme,val2)!=0){
                        ScriptStack[0].nextLine();
                    }

                }
                else
                    ScriptStack[0].nextLine();

            }



        }

    }
//-----------------------------------------
// Exec(somescript.txt)
    void CMD_EXE(Arguments& args){

        char buf[255];
        char buf2[255];


        CCode newscript;
        ScriptStack.add(newscript);
        ActiveScript++;

        printf("active script:%ld\n", ActiveScript);

        sprintf(buf, "%s%s", Sys.basePath, args.variables[0].s);
        bool res = ScriptStack[ActiveScript].load(buf);
        sprintf(buf2, "######Trying to load %s ...", buf);
        GameLog.write(buf2);


        if (res){
            ScriptStack[ActiveScript].runNew();
            sprintf(buf2, "######executing %s ...", buf);
            GameLog.write(buf2);

        }
        else
            GameLog.write("no such script ?");


    }
//--------------------
    void CMD_GOTO(Arguments& args){
        //GameLog.write("it's go to :)");
        //GameLog.write(args->variables[0]);
        int sk=atoi(args.variables[0].s);
        ScriptStack[ActiveScript].goTo(sk);
        //ScriptLineIndex=sk-1;

    }
//--------------------
//give(PartyMember,itemID,E) gives E items to PartyMember
    void CMD_GIVE(Arguments& args){

        if (args.count() < 3){
            GameLog.write("Not enough arguments");
            return;
        }
        
        int dudeID = atoi(args.variables[0].s);
        unsigned itemID = atoi(args.variables[1].s);
        unsigned count = atoi(args.variables[2].s);



        if ((dudeID >= 0) && (dudeID < (int)jparty.count()) && (dudeID >= 0)){

            if ((itemID < itemData.info.count())&&(count>0))  {

                Item item;
                if ((itemData.info[itemID].isweapon) || (itemData.info[itemID].isarmor))
                    item.set(itemID, 100, 1);
                else
                    item.set(itemID, 0, 1);
                for(unsigned i = 0; i < count; i++)
                    jparty.member(dudeID)->inventory.addItem(&item,itemData);

                char buf[40];
                sprintf(buf, "%s gains %d %s#", jparty.member(dudeID)->name, count, itemData.info[itemID].name);
                Console.putText(buf);
            }
        }
        else{
            GameLog.write("Wrong hero ID");
        }
    }

    //--------------------
    //Inject(PartyMember, E). Party member gains amout of E experience points
    void CMD_Inject(Arguments& args){


        unsigned dudeID = atoi(args.variables[0].s);
        int exp=atoi(args.variables[1].s);
        if (dudeID < jparty.count()){

            Creature * dude = jparty.member(dudeID);

            char buf[255];
            sprintf(buf,"%s gains %d exp", dude->name, exp);
            Console.putText(buf);

            if ((dude->needexp()<=exp) && (dude->level()<100))  {
                int oldlev = dude->level();
                dude->exp += exp;
                int lev = dude->level();
                for (int i = 0; i < (lev - oldlev); i++)
                    dude->levelUp(Rases, klases);
                dude->enablespells(klases, Spells);

            } else
                if (dude->needexp() > exp)  dude->exp += exp;
        }
        else
            GameLog.write("Bad hero index");

    }
//---------------------
//pakrauna zemelapi
    void CMD_LoadMap(Arguments& args){


        int x = atoi(args.variables[0].s);
        int y = atoi(args.variables[1].s);

        LoadCurrentMap(x*32, y*32, args.variables[2].s);
        ApplyOpenedChests();

    }

//-------------------------
    //AddNpc(x,y,sprite)
    void CMD_ADDNPC(Arguments& args){

        if (args.count() < 4)
            return;

        int x = atoi(args.variables[0].s);
        int y = atoi(args.variables[1].s);
        unsigned char type = atoi(args.variables[2].s);

        Npc newman;

        char modelname[255];
        char texturename[255];
        unsigned frame = 0;

        if (args.count() > 4 )
            strncpy(modelname, args.variables[4].s, 255);
        else
            strncpy(modelname, "", 255);

        if (args.count() > 5 )
            strncpy(texturename, args.variables[5].s, 255);
        else
            strncpy(texturename, "", 255);

        if (args.count() > 6 )
            frame = (unsigned)atoi(args.variables[6].s);

        newman.init( x*32, y*32, type, 
                     npcnation.count() + 1,"",
                     args.variables[3].s,
                     modelname,
                     texturename,
                     frame
                    );
        npcnation.add(newman, Sys.basePath, true);
    }
//--------------------------
    void CMD_REMOVENPC(Arguments& args){

        int id = atoi(args.variables[0].s);
        if (!npcnation.kill(id))
            GameLog.write("Ther's no such NPC");
    }

//-------------------------
    void CMD_NPCFRAME(Arguments& args){
        unsigned id = (unsigned)atoi(args.variables[0].s);
        unsigned frame = (unsigned)atoi(args.variables[1].s);

        npcnation.getNpc(id)->setFrame(frame);
        npcnation.getNpc(id)->pdir.x = 0;
        npcnation.getNpc(id)->pdir.y = 0;
    }

//------------------------
    void CMD_NOJOY(Arguments& args){

        if (PartyControl)
            puts("===JOY OFF==");
        else
            puts("===JOY ON==");
        PartyControl = !PartyControl;

    }
//-------------------
    void CMD_DESERT(Arguments& args){
        if (!Cheat_NoEnemy)  {
            Console.putText("no enemy on");
            Cheat_NoEnemy = true;
        } else {
            Console.putText("no enemy off");
            Cheat_NoEnemy = false;
            jparty.steps=0;
        }
    }

//------------------
void CMD_INTERNET(Arguments & args){
    if (!visible_enemy)  {
        visible_enemy=true;
        Console.putText("visible enemy stat on");
    } else {
        visible_enemy=false;
        Console.putText("visible enemy stat off");
    }
}
//-------------------
    void CMD_WRITE(Arguments& args){

        Console.putText(args.variables[0].s);

    }
//---------------------
void CMD_FIGHT(Arguments& args){
    GameLog.write("Let's fight");
    if (args.count() <= 0){
        GameLog.write("Not enough arguments");
        return;
    }
    int index = atoi(args.variables[0].s);
    if (index >= (int)MonsterGroups.count()){
        GameLog.write("Wrong monter group index!");
        return;
    }
    if (Sys.useSound)
        SS.playsound(0, false);
    GameState = BEGIN_COMBAT;
    Combat_groupindex = index;
}    
//----------------------------
    void CMD_LET(Arguments& args){

        if (args.count() == 2){
            CVariable var(args.variables[0].s, args.variables[1].s);
            //puts("adding var");
            Kintamieji.add(var);
        }
        else
            puts("Not enough arguments");

    }
//---------------------------------------
    void CMD_ADDPMEMB(Arguments& args){
        Creature pad;

        char buf[255];

        sprintf(buf, "%s%s", Sys.basePath, args.variables[0].s);
        pad.loadFromFile(buf, klases, Spells, itemData);
        jparty.addMember(pad, Sys.basePath);
        puts("LOADED!");

    }
//--------------------------------------------------

    void CMD_OPENBOX(Arguments& args){ //OpenBox(keyID, itemID, count, tile, layer)

        int keyID = atoi(args.variables[0].s);
        int itemID = atoi(args.variables[1].s);
        int count = atoi(args.variables[2].s);
        int tile = atoi(args.variables[3].s);
        int layer = 0;
        if (args.count() > 4)
            layer = atoi(args.variables[4].s);


        Chest newchest;

        int tx = npcnation.getNpc(npcnation.count() - jparty.count())->pdir.x;
        int ty = npcnation.getNpc(npcnation.count() - jparty.count())->pdir.y;
        newchest.x = (unsigned)npcnation.getNpc(npcnation.count() - jparty.count())->x()/32 + tx;
        newchest.y = (unsigned)npcnation.getNpc(npcnation.count() - jparty.count())->y()/32 + ty;
        newchest.layer = (unsigned)layer;
        newchest.tilenum = (unsigned)tile;
        printf("%d %d\n", newchest.x, newchest.y);
        strcpy(newchest.mapname, Currentmap);

        bool cantake = false;

        if (!EmptyChests.get(newchest.x, newchest.x, Currentmap)){

            if (keyID >= 0){ //an item is needed to open a chest
                if (jparty.count()){
                    int index = jparty.member(0)->inventory.findItem((unsigned)keyID);

                    if ( index >= 0){
                        jparty.member(0)->inventory.removeItemByIndex(index);
                        cantake = true;
                    }
                }
            }
            else{
                cantake = true;
            }

            if (cantake){ //can take stuff

                char buf[256];
                if (tile > -1){ //changes the tile
                    map.setTile(newchest.x, newchest.y, 
                                (unsigned)layer, (unsigned)tile);
                    //printf("tile: %d, layer: %d\n", tile, layer);
                }

                EmptyChests.add(newchest);
                puts("Added that chest");

                if ((count > 0) && (itemID >=0)){ //is there items in the box?
                    Item itm;
                    itm.set(itemID, 100, 1);

                    int added = 0;

                   
                    for (int i = 0; i < count; i++){
                        int bagweight = jparty.member(0)->inventory.itemWeight(itemData);
                        int stamina = jparty.member(0)->stamina();
                        if (bagweight + (int)itemData.info[itemID].weight <= stamina){

                            jparty.member(0)->inventory.addItem(&itm, itemData);

                            added++;
                        }
                    }

                    if (added){

                        if (added > 1)
                            sprintf(buf, "%s found %d %ss#", jparty.member(0)->name, added, itemData.info[itemID].name);
                        else
                            sprintf(buf, "%s found %s#", jparty.member(0)->name, itemData.info[itemID].name);
                        Console.putText(buf);
                    }
                    else{
                        Console.putText("Can't carry more.#");
                    }
                }
            }

        }

            //itemData.info[]

    }
//--------------------------------

void CMD_EXITP(Arguments& args){
    shut_down=true;
}

//------------------------------------------------
void MakeFunctionArray(){

    Funcs.add(CMD_DESERT, "desert");
    Funcs.add(CMD_NOJOY, "NoJoy");
    Funcs.add(CMD_FIGHT,  "fight");
    Funcs.add(CMD_INTERNET, "internet");
    Funcs.add(CMD_WRITE, "Write");
    Funcs.add(CMD_Inject, "inject");
    Funcs.add(CMD_PSNPC, "PushNpc");
    Funcs.add(CMD_PUSHCAM, "PushCam");
    Funcs.add(CMD_MVCAM, "MovCam");
    Funcs.add(CMD_REDIR, "Redir");
    Funcs.add(CMD_SETCOLLISION, "SetCollision");

    Funcs.add(CMD_SETCAMUP, "SetCamUp");
    Funcs.add(CMD_SETCAMPOS, "SetCamPos");
    Funcs.add(CMD_SETCAMDIR, "SetCamDir");
    Funcs.add(CMD_SETCAMRIGHT, "SetCamRight");

    Funcs.add(CMD_MVNPC, "MovNpc");
    Funcs.add(CMD_OPENBOX, "OpenBox");
    Funcs.add(CMD_EXITP, "Exitp");
    Funcs.add(CMD_EXE, "Exec");
    Funcs.add(CMD_SAY, "Say");
    Funcs.add(CMD_INN, "Inn");
    Funcs.add(CMD_SHOP, "Shop");
    Funcs.add(CMD_SMITH, "Smith");
    Funcs.add(CMD_GIVE, "Give");
    Funcs.add(CMD_LoadMap, "LoadMap");
    Funcs.add(CMD_ADDNPC, "AddNpc");
    Funcs.add(CMD_NPCFRAME, "NpcFrame");
    Funcs.add(CMD_REMOVENPC, "RemoveNpc");
    Funcs.add(CMD_RESTOREHP, "RestoreHP");
    Funcs.add(CMD_DRAINHP, "DrainHP");
    Funcs.add(CMD_DRAINMP, "DrainMP");
    Funcs.add(CMD_RESTOREMP, "RestoreMP");
    Funcs.add(CMD_SCAN, "Scan");
    Funcs.add(CMD_ESCAPE, "Escape");
    Funcs.add(CMD_RESURECT, "Resurect");
    Funcs.add(CMD_ANTIDOTE, "Antidote");
    Funcs.add(CMD_POISON, "Poison");
    Funcs.add(CMD_ELEMENTAL, "Elemental");
    Funcs.add(CMD_GOTO, "GoTo");
    Funcs.add(CMD_LET, "Let");
    Funcs.add(CMD_ADDPMEMB, "AddPartyMember");
}
//------------------------------------------------
void Interpret(const char* line){

    char cmd[255];
    Arguments args;

    strcpy(cmd,"");

    if (line){
        if ((int)strlen(line) >= 1){
            if (!((line[0] == '/') && (line[1] == '/'))){

                GetCommand(line, cmd); //gets script command name
                //if (strcmp(cmd,"Let")!=0)
                //    puts(cmd);

                if (line[(int)strlen(cmd)] == '(')
                    if (strcmp(cmd,"if") != 0){
                        args.getArguments(line, (int)strlen(cmd), &Kintamieji);
                }

                if (strcmp(cmd,"if")==0)
                    CMD_IF(line,(int)strlen(cmd));
                else{
                    for(unsigned i = 0; i < Funcs.func.count(); i++)
                        if (strcmp(cmd, Funcs.func[i].name) == 0){
                            (*Funcs.func[i].p)(args);
                            break;
                        }
                }
            }
        }
    }
    args.clear();
}

//---------------------------------------------
    void CommandLine(){

        char code[80];

        CodeEdit.getInput(globalKEY);

        if (CodeEdit.entered)  {
            strcpy(code, CodeEdit.text());

            Interpret(code);

            code_enter=false;
            PartyControl=true;
            code_enter_enabled=false;
            CodeEdit.reset();
        }
    }

//---------------------------------------------------------
    void RunScript(){

        if (ScriptStack.count()){
            if (!ScriptStack[ActiveScript].nextLine()){
                ScriptStack[ActiveScript].stop();
                ScriptStack[ActiveScript].clear();
                ScriptStack.remove(ActiveScript);
                ActiveScript--;
            }
            else{
                /*char buf[255];
                sprintf(buf,  "###%d||%s", ScriptStack[ActiveScript].currentLineNum(),
                        ScriptStack[ActiveScript].currentLine());
                GameLog.write(buf);*/
                Interpret(ScriptStack[ActiveScript].currentLine());

            }
        }
    }

//----------------------------------------------------
    unsigned char GetDir(unsigned int key){

        switch(key) {

            case SDLK_UP: return 2; //up
            case SDLK_DOWN: return 1; //down}
            case SDLK_LEFT: return 4;  //left}
            case SDLK_RIGHT: return 3;  //right}
            default: return 0;
        }
    }
//-----------------------------------------------------
    void SetRandomCamPos(){

        char buf[255];
        GameLog.write(buf);
         if (Arena.camPostionCount()){
            unsigned index = rand()%Arena.camPostionCount();
            CameraState * st = 0;

            GameLog.write(buf);

            st = Arena.getPosition(index);
            if (st){
                Cam3D.setpos(st->pos);
                Cam3D.setdir(st->dir);
                Cam3D.setright(st->right);
                Cam3D.setup(st->up);
            }

        }
    }

    //-------------------------------------
    void NextActiveFighter(){

        //puts("getting next");

        SetRandomCamPos();


        AF.activeFighter++;

        if (AF.activeFighter >= Entit.count()){
            AF.activeFighter = 0;

            bVisiKovesi = true;
        }

        unsigned cnt = 0;

        while (((Entit.get(AF.activeFighter)->hp <= 0) || (Entit.get(AF.activeFighter)->escaped))
            && (AF.activeFighter < Entit.count()) && (cnt < Entit.count() + 1)){
            AF.activeFighter++;
            cnt++;
            if (AF.activeFighter >= Entit.count()){
                AF.activeFighter = 0;
                if (!bVisiKovesi)
                    bVisiKovesi = true;
            }
        }
        //puts("got the next");

    }

    //-----------------------------------------------
    void FighterActionSelection(){

        switch (Entit.get(AF.activeFighter)->type) {
            //hero
            case HERO:
                if (jparty.member(AF.partyIndex)->state != DEAD)  {

                    if (! HeroActionSelect)  {
                        HeroActionSelect = true;
                        HeroActionDone = false; //try

                        CreateActionMenu(AF.partyIndex, ActionSelect);
                        CreateItemsMenu(200, 50, ItemMenu, 
                                        jparty.member(AF.partyIndex));
                        CreateSpellMenu(SpellMenu, 
                                        jparty.member(AF.partyIndex), true);

                    }
                    HeroAction(AF.partyIndex, HeroActionDone);

                    if (HeroActionDone){
                        HeroActionSelect = false;
                        puts("Hero action is done");

                        PoisonEffect(jparty, AF.partyIndex);
                    }

                }
            break;
                //enemy-------------------------
            default:
                if (eparty.member(AF.partyIndex)->state != DEAD){
                    EnemyAI(AF.partyIndex);

                    PoisonEffect(eparty, AF.partyIndex);

                    HeroActionDone = true;
                }

        }
    }

//-----------------------------------------------------
void AttackAnimation(Fighter * dude, int percent,
                     PartyActions * action){

    int atackSteps = 30;
    int endFrame = 90;


    float dist;
    unsigned oponentfighter = action->act(dude->nr)->oponent;

    Fighter * oponent = 0;
    oponent = Entit.get(oponentfighter);

    //~~~~~~~~~~~~~~~~~walks to the oponent
    if ((percent > 9) && (percent < 41)){ 

        if (percent == 10){

            float bb = 2.0f;
            if(dude->type == 1)
                bb = 2.0f;
            else
                bb = -2.0f;

            dude->makeDir(oponent->pos + Vector3D(bb, 0.0f, 0.0f),
                          (CAnimControl.MaxCombatFrame / 100) * atackSteps);
        }
        dude->frame = (percent - 10)/3;
        dude->move();
    }
    //~~~~~~~~attack frames
    if ((percent > 40) && (percent < 51)){ 

        dude->frame = (percent - 41)+10;

        if (percent == 45){

            if (AtackOnce){

                unsigned myNumber = dude->nr;
                unsigned oponentNumber = oponent->nr;
                unsigned myType = dude->type;
                unsigned oponentType = oponent->type;

                int dmg = 0;

                dmg = Attack(myNumber, oponentNumber,
                             myType, oponentType);

                AtackOnce = false;

                Vector3D spraydir;

                float plusx = 0.0f;
                if (oponentType == 1){
                    spraydir = Vector3D(-1.0f, 0.0f, 0.0f);
                    plusx = 0.5f;
                }
                else{
                    spraydir = Vector3D(1.0f, 0.0f, 0.0f);
                    plusx = 1.5f;
                }

                if (dmg > 0){

                    char buf[100];
                    sprintf(buf, "%d", dmg);
                    PS.addFloatingNumbers(Vector3D(oponent->pos.x() + plusx,
                                                   1.0f, 
                                                   oponent->pos.z() + 0.7f
                                                  ),
                                          buf,
                                          Pics.getname(Pics.findByName("charai.tga"))
                                          );

                    PS.addBloodStream( Vector3D(oponent->pos.x() + plusx,
                                                2.8f,
                                                oponent->pos.z() + 0.7f 
                                               ),
                                       spraydir,
                                       Pics.getname(Pics.findByName("particles/blood.tga"))
                                     );
                }
                else{
                    if (dmg == 0){
                        PS.addFloatingNumbers(Vector3D(oponent->pos.x() + plusx,
                                                       1.0f, 
                                                       oponent->pos.z() + 0.7f
                                                       ),
                                              "miss",
                                              Pics.getname(Pics.findByName("charai.tga")));
                    }
                    if (dmg < 0){ // oponent tries to dodge
                        if (oponentType == 1)
                            HeroActions.set(oponentNumber, DODGE, 0);
                        else
                            EnemyActions.set(oponentNumber, DODGE, 0);
                    }
                }
            } //AtackOnce
        }
        else{
            AtackOnce = true;
        }
    } //attack frames

    //~~~~~goes back
    if ((percent > 50) && (percent < endFrame + 1)){
            dude->frame = int((percent - 51)/3.9f);

        if (percent == 51){

            dist = (dude->type == 1)? -1.5f : 1.5f;

            dude->makeDir(dude->startPos + Vector3D(dist, 0.0f , 0.0f),
                          ((CAnimControl.MaxCombatFrame / 100) * 40));
        }

        dude->move();

        if (percent == endFrame){
            action->set(dude->nr, NOP, 0);
        }

    }
}
//--------------------------------------------------------------
void    CastAnimation(Fighter * dude, int percent, PartyActions * action){

    unsigned oponentfighter = action->act(dude->nr)->oponent;

    Fighter * oponent = Entit.get(oponentfighter);

    unsigned myNumber = dude->nr;
    unsigned oponentNumber = oponent->nr;
    unsigned myType = dude->type;
    unsigned oponentType = oponent->type;
    unsigned spellID = action->act(dude->nr)->itemID;

    Party* myParty = (myType == 1)? &jparty : &eparty;
    Party* enemyParty = (oponentType == 1)? &jparty : &eparty;

    if ((percent > 40) && (percent < 71)){
        dude->frame = 30 + (percent - 40)/3;
        if (percent == 60){
            if (CastOnce){
                CastOnce = false;

                int oldHP = 0; int newHP = 0;
                int oldPHP = 0; int newPHP = 0;

                oldPHP = myParty->member(myNumber)->hp.def;


                oldHP = enemyParty->member(oponentNumber)->hp.def;
                CastSpell(spellID,
                          *myParty, myNumber, 
                          *enemyParty, oponentNumber); 

                newHP = enemyParty->member(oponentNumber)->hp.def;

                newPHP = myParty->member(myNumber)->hp.def;
                int HPdif = oldHP - newHP; 
                int PHPdif = oldPHP - newPHP;
                char buf[100];
                COLOR c;
                if (HPdif){
                    //if HP increases display numbers in green, else white
                    c = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
                    if (HPdif < 0)
                        c = COLOR(0.0f, 1.0f, 0.0f, 1.0f);
                    sprintf(buf, "%d", abs(HPdif));
                    //display oponent HP difference
                    PS.addFloatingNumbers(Vector3D(oponent->pos.x(),
                                                   1.0f, 
                                                   oponent->pos.z() + 0.7f),
                                          buf,
                                          Pics.getname(Pics.findByName("charai.tga")),
                                          c);
                }
                if (PHPdif){
                    c = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
                    if (PHPdif < 0)
                        c = COLOR(0.0f, 1.0f, 0.0f, 1.0f);
                    sprintf(buf, "%d", abs(PHPdif));
                    //display hero HP difference
                    PS.addFloatingNumbers(Vector3D(dude->pos.x(),
                                                   1.0f, 
                                                   dude->pos.z() + 0.7f
                                                  ),
                                          buf, Pics.getname(Pics.findByName("charai.tga")),
                                          c);

                }

                int bloodpic = Pics.findByName("particles/blood.tga");
                if (bloodpic != -1){
                    PS.addHealingAura( Vector3D(dude->pos.x(),
                                                1.0f,
                                                dude->pos.z()),
                                        Pics.getname(bloodpic)
                                     );

                    PS.addHealingAura( Vector3D(oponent->pos.x(),
                                                1.0f,
                                                oponent->pos.z()),
                                        Pics.getname(bloodpic)
                                     );
                }
            }
        }
        else{
            CastOnce = true;
        }
    }
        
    if (percent == 90){
        action->set(dude->nr, NOP, 0);
    }

}
//-----------------------------------------------------
void    UseAnimation(Fighter * dude, int percent, PartyActions * action){

    unsigned oponentfighter = action->act(dude->nr)->oponent;

    Fighter * oponent = Entit.get(oponentfighter);

    unsigned myNumber = dude->nr;
    unsigned oponentNumber = oponent->nr;
    unsigned myType = dude->type;
    unsigned oponentType = oponent->type;
    unsigned itemID = action->act(dude->nr)->itemID;

    Party* enemyParty = (oponentType == 1)? &jparty : &eparty;

    Creature * me = 0;
    me = (myType == 1) ? jparty.member(myNumber) : eparty.member(myNumber);

    if ((percent > 40) && (percent < 71)){ 
        dude->frame = 30 + (percent - 40)/3;
        if (percent == 60){
            if (UseOnce){
                UseOnce = false;

                int oldHP = 0; int newHP = 0;
                int oldPHP = 0; int newPHP = 0;

                oldPHP = me->hp.def;

                if (itemData.info[me->inventory.getItem(itemID)->kind].isweapon){ //let's equip weapon

                    if (me->rhand.kind == -1){ //let's try to equip on right hand
                        if (me->equipItem(itemID, RIGHTHAND, itemData))
                            Console.putText("weapon equiped on right hand!");
                    }
                    else{
                        if (me->lhand.kind == -1){
                            if (me->equipItem(itemID, LEFTHAND, itemData))
                                Console.putText("weapon equiped on left hand!");
                        }
                        else{
                            me->unequipItem(LEFTHAND, itemData);

                            me->unequipItem(RIGHTHAND, itemData);

                            me->equipItem(me->inventory.itemGroupCount()-1,
                                          LEFTHAND, itemData);
                            if (me->equipItem(itemID, RIGHTHAND, itemData))
                                Console.putText("weapon equiped on right hand!");
                        }
                    }

                }else

                if (itemData.info[me->inventory.getItem(itemID)->kind].isarmor){
                    if (me->armor.kind == -1){ //let's equip item
                        if (me->equipItem(itemID, ARRMOR, itemData))
                            Console.putText("arrmor equiped !");
                    } else{
                        me->unequipItem(ARRMOR, itemData);
                        if (me->equipItem(itemID, ARRMOR, itemData))
                            Console.putText("arrmor equiped !");
                    }

                }

                else{
                    //use normal item

                    oldHP = enemyParty->member(oponentNumber)->hp.def;
                    UseItem(*me, itemID, *enemyParty, oponentNumber);
                    newHP = enemyParty->member(oponentNumber)->hp.def;
                }

                newPHP = me->hp.def;
                int HPdif = oldHP - newHP; 
                int PHPdif = oldPHP - newPHP;
                char buf[100];
                COLOR c;
                if (HPdif){
                    //if HP increases display numbers in green, else white
                    c = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
                    if (HPdif < 0)
                        c = COLOR(0.0f, 1.0f, 0.0f, 1.0f);
                    int characters = Pics.findByName("charai.tga");
                    sprintf(buf, "%d", abs(HPdif));
                    //display oponent HP difference
                    PS.addFloatingNumbers(Vector3D(oponent->pos.x(),
                                                   1.0f, 
                                                   oponent->pos.z() + 0.7f),
                                          buf, Pics.getname(characters),
                                          c);
                }
                if (PHPdif){
                    c = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
                    if (PHPdif < 0)
                        c = COLOR(0.0f, 1.0f, 0.0f, 1.0f);
                    int charcters = Pics.findByName("charai.tga");
                    sprintf(buf, "%d", abs(PHPdif));
                    //display hero HP difference
                    PS.addFloatingNumbers(Vector3D(dude->pos.x(),
                                                   1.0f, 
                                                   dude->pos.z() + 0.7f),
                                          buf, 
                                          Pics.getname(charcters), c);
                }
           }
       } //60 frame
       else{
           UseOnce = true;
       }
    }

    if (percent == 90){
        action->set(dude->nr, NOP, 0);
    }
}

//-----------------------------------------------------
    void    DodgeAnimation(Fighter * dude, int percent, PartyActions * action){

        int dodgeSteps = 5;
        int lim1 = 50;
        int lim2 = 70;

        if ((percent > lim1 - dodgeSteps) && (percent <= lim1)){ 
            if ((percent == (lim1 - dodgeSteps) + 1)&&(DodgeOnce)){

                float bb = 1.0f;
                if(dude->type == 1)
                    bb = 1.0f;
                else
                    bb = -1.0;

                dude->makeDir(dude->startPos + Vector3D(bb, 0.0f, 0.0f),
                                          (CAnimControl.MaxCombatFrame / 100) * dodgeSteps - 1);
                DodgeOnce = false;
            }

            dude->move();

        }


        if ((percent > lim2 - dodgeSteps) && (percent <= lim2)){ 


            if ((percent == (lim2 - dodgeSteps) + 1) && (Dodge2Once)){

                float bb = -1.0f;
                if(dude->type == 1)
                    bb = -1.0f;
                else
                    bb = 1.0;

                dude->dir = Vector3D(dude->dir.x()*-1, dude->dir.y(), dude->dir.z());
                Dodge2Once = false;
            }

            dude->move();



        }
        if (percent == 90){
            action->set(dude->nr, NOP, 0);
            dude->dir = Vector3D(0.0f, 0.0f, 0.0f);
            DodgeOnce = true;
            Dodge2Once = true;
        }
    }

    //================================================================================================
    void CombatAnimationHandling(){

        int percent = CAnimControl.percent();

        Fighter * activeDude = Entit.get(AF.activeFighter);

        int forwardSteps = 9;
        float dist = 1.5f;

        if ((percent > 0) && (percent < 10)){ //run foward

            if (percent == 1){

                if(activeDude->type == 1)
                    dist = -1.5f;
                else
                    dist = 1.5f;

                activeDude->makeDir( Vector3D(activeDude->startPos.x() + dist,
                                              activeDude->startPos.y(),
                                              activeDude->startPos.z()),
                                    (CAnimControl.MaxCombatFrame / 100) * forwardSteps);
            }

            activeDude->move();
        }
        //-----------------------------------------------------------------------------------------

        for (unsigned i = 0; i < Entit.count(); i++){
            PartyActions * actions = 0;

            Fighter * dude = Entit.get(i);

            if (dude->type == 1)
                actions = &HeroActions;
            else
                actions = &EnemyActions;

            switch (actions->act(dude->nr)->action){

                case ATACK: AttackAnimation(dude, percent, actions); break;
                case CAST:  CastAnimation(dude, percent, actions); break;
                case USE:   UseAnimation(dude, percent, actions); break;
                case DODGE: DodgeAnimation(dude, percent, actions); break;

                default:{ //idle animation
                    if ((dude->frame > 29)||(dude->frame < 24))
                        dude->frame = 25;
                    dude->idleTic++;
                    if (dude->idleTic > 20){
                        dude->frame += 1;
                        dude->idleTic = 0;
                    }
                }

            } //end switch
        }

        //-------------------------------------------------------------------------------------------
        if ((percent > 90) && (percent < 100)){

            if (percent == 91){
                activeDude->makeDir(activeDude->startPos,
                                    ((CAnimControl.MaxCombatFrame / 100) * forwardSteps));
            }
            activeDude->move();

            if (percent == 99){ //reset facing direction
               /* if (activeDude->type == 1)
                    activeDude->dir = Vector3D(-1.0f, 0, 0);
                else
                    activeDude->dir = Vector3D(1.0f, 0, 0);*/
            }
        }
    }

    //===================================================================================================žžžžžžžžž
    //if fight == true
    void OnCombat(){

        PS.update();

        if ((!CAnimControl.CombatAnimationRun) && (! gameover) && (!HeroActionSelect))
            CAnimControl.CombatAnimationRun = true;

        if (((Victory) || (jparty.present() == 0)))  {
            End_combat();
            Victory = false;
            return;
        }

        if ((!QuitMenu.active()) && (!gameover)){

            CombatAnimationHandling();

            if ( ((CAnimControl.percent() < 10) && (!HeroActionSelect)) || (HeroActionDone) ){
                CAnimControl.CombatFrame++;
            }
        }


        if ((CAnimControl.percent() >= 10) && (!Victory) && (!HeroActionDone)){

            AF.partyIndex = Entit.get(AF.activeFighter)->nr;

            FighterActionSelection();


        }

        if (CAnimControl.CombatFrame > CAnimControl.MaxCombatFrame) {
            CAnimControl.CombatAnimationRun = false;  //visualizacijos kadru pabaiga
            CAnimControl.CombatFrame = 0;
        }


        if ((!CAnimControl.CombatAnimationRun) && (!Victory)){

            if (bVisiKovesi)  {
                Entit.build(jparty, eparty, EnemyActions, HeroActions);  //padaro fighteriu lenetele
                Combatselect.set(
                                 Entit, jparty.count(),
                                 eparty.count(),
                                 5, Sys.width - 200);
                bVisiKovesi=false;
            }
            Entit.colectHpMp(jparty,eparty);

            HeroActionDone = false;
            NextActiveFighter();
            CheckVictory();
        }
    }

 //--------------------------------------------
    int OnEntry(int x, int y){

        unsigned i = 0;

        if (Events.count()){
            while (((Events.entries[i].x != x) ||
                    (Events.entries[i].y != y) || (Events.entries[i].type != 0))
                    && (i < Events.count()))
                i++;

            if (i < Events.count()){


                if ((Events.entries[i].x == x) && (Events.entries[i].y == y))  {
                    jparty.steps = 0;
                    Interpret(Events.entries[i].code);
                    return 1;
                }
            }
        }

        if ((((unsigned)x == map.width() - 1) || (x == 0) || (y == 0) || ((unsigned)y == map.height() - 1))
            && (Events.isSpecialEvent) && (map.isWalkable(x, y))){

            if (strcmp(Events.specialEvent.code,"") != 0)  {
                jparty.steps = 0;
                Interpret(Events.specialEvent.code);

                return 2;
            }
        }

        return 0;

    }

    //-----------------------------------------------
    void Focus3DCam(float x, float y,
                    float height, float distance, float pitch,
                    float tilesize){

        float halftile = tilesize / 2.0f;

        Cam3D.setpos(Vector3D(x / halftile + halftile,
                              height, y / halftile + distance ));
        Cam3D.setdir(Vector3D(0.0f, 0.0f, -1.0f ));
        Cam3D.setup(Vector3D(0.0f, 1.0f, 0.0f ));
        Cam3D.setright(Vector3D(-1.0f, 0.0f, 0.0f ));
        Cam3D.pitch(pitch);
    }

//----------------------------------------------------
    void Walking(){

        Npc* dudes = 0;
        dudes = npcnation.getArray();
        int tmppx = 0;
        int tmppy = 0;

        if (dudes){

            //generuojam parcio dira pagal keyu

            jparty.mdir[0] = GetDir(globalKEY);

            int tx = 0;
            int ty = 0;

            int first = 0;
            while ((npcnation.getNpc(first)->job != 255)
                    &&(first < (int)npcnation.count()))
                first++;

            translatedir(jparty.mdir[0],tx,ty);

            //printf("dirs %d %d %u\n", tx, ty, jparty.count());

            tmppx = (int)(npcnation.getNpc(first)->x()+16 + tx*32)/32;
            tmppy = (int)(npcnation.getNpc(first)->y()+16 + ty*32)/32;


            if ((!conversation.active()))  {
                if (((Events.count()) || (Events.isSpecialEvent))
                    && ((dudes+first)->getClock() == 0)) {

                    if ((tmppy < (int)map.height()) && (tmppx < (int)map.width())){

                        bool scriptRuns = false;
                        if (ScriptStack.count()){
                            if (ScriptStack[ActiveScript].isRunning())
                                scriptRuns = true;
                        }

                        if(!scriptRuns){
                            if (OnEntry(tmppx, tmppy))

                                return;

                        }
                    }
                }
            }


            //vaiksciojimas gyvatele
            for (unsigned i = 0; i < jparty.count(); i++){
                if ((dudes + first + i)->isWalk()){
                    (dudes + first + i)->inctimer();

                    if ((PartyControl) && (i == 0) && (!conversation.active())){ //moves camera

                        Npc * maindude = dudes + first;

                        MoveMap((dudes + first + i)->pdir.x, (dudes + first + i)->pdir.y,
                                (dudes + first + i)->x(), (dudes + first + i)->y());

                        Focus3DCam(maindude->x(), maindude->y(),

                                   map.camHeight, map.camDistance, map.camPitch );
                    }

                }
            }


            jparty.x = (dudes+first)->x();
            jparty.y = (dudes+first)->y();

            if ((PartyControl) && (!conversation.active())) {
                //first dude

                //printf("%d %u\n",(dudes + first )->getClock(), jparty.mdir[0]);
                if (((dudes+first)->getClock() == 0) && (jparty.mdir[0]!=0))  {
                    //printf("%f %f\n",(dudes + first )->x(), (dudes + first)->y());
                    bool cango = false;
                    cango = (dudes+first)->redir(jparty.mdir[0], map, npcnation);

                    (dudes+first)->stepcount = 32;
                    if (cango){
                        jparty.steps++;

                        if (next_step)  next_step=false;

                        for (unsigned i = 0; i < jparty.count(); i++){
                            if (jparty.member(i)->state == POISONED){
                                PoisonEffect(jparty,i);

                            }

                        }
                    }

            if (cango)
                jparty.membersxyPush((dudes+first)->x(),(dudes+first)->y());

        //other dudes
            for (unsigned i = 1; i < jparty.count(); i++) {
                int dudex = 0; int dudey = 0;
                dudex = (dudes + first + i)->x()/32;
                dudey = (dudes + first + i)->y()/32;

                if (dudex < (int)(jparty.membersxy[i].x()/32)){
                    jparty.mdir[i] = 3;
                   // puts("dir = 3");
                }
                else
                    if (dudex > (int)(jparty.membersxy[i].x()/32)){
                        jparty.mdir[i]=4;
                       // puts("dir = 4");
                    }
                    else
                        if (dudey < (int)(jparty.membersxy[i].y()/32)){
                            jparty.mdir[i]=1;
                           // puts("dir = 1");
                        }
                        else
                            if (dudey > (int)(jparty.membersxy[i].y()/32)){
                                jparty.mdir[i]=2;
                               // puts("dir = 2");
                            }
                            else{
                                jparty.mdir[i]=0;
                                //puts("dir = 0");
                            }


            if ((dudes+first)->getClock() == 0)  {
                (dudes+first+i)->redir(jparty.mdir[i], map, npcnation);
                            (dudes+first+i)->stepcount = 32;
                        }
                    }
                }
            }
        }
    }

//------------------------------------------------
  void HeroInfo(){
    if (partyfaces.skipdead)
     partyfaces.skipdead=false;  //kad negyvus herojus butu galima pazet
    partyfaces.getInput(globalKEY,jparty);
    partyfaces.activate();

    if (partyfaces.canceled)  {
     partyfaces.deactivate();
     orderselect.reset();
     partyfaces.skipdead=true;
     partyfaces.reset(jparty);
    }
  }

//------------------------------------
    void CastOrder(){

        if (! SpellMenu.active())  {

            if (! partyfaces.selected)  {

                if (!partyfaces.active())
                    partyfaces.activate();

                    partyfaces.getInput(globalKEY,jparty);

                    if (partyfaces.selected)  {

                        partyfaces.deactivate();
                        Creature * dude = jparty.member(partyfaces.face);

                        if (klases.get(dude->klass)->spells.count() < 1)  {
                            Console.putText("not a spell caster!#");
                            orderselect.reset();
                            partyfaces.reset(jparty);
                        }
                        else{
                            CasterIndex = partyfaces.face;

                            CreateSpellMenu(SpellMenu, dude, false);
                            SpellMenu.activate();
                            if (SpellMenu.selection.count() == 0){
                                SpellMenu.deactivate();
                                orderselect.reset();
                                partyfaces.reset(jparty);
                                Console.putText("must learn some spells!#");
                            }
                            partyfaces.reset(jparty);
                        }
                    }

                    if (partyfaces.canceled){
                        partyfaces.reset(jparty);
                        partyfaces.deactivate();
                        orderselect.reset();
                    }
            }
        }
        else {  //jei spellmeniu aktyvuotas

            if (!SpellMenu.selected){
                SpellMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
            }
            else { //spell already selected
                if (!partyfaces.selected){

                    if (!partyfaces.active())
                        partyfaces.activate();

                    partyfaces.getInput(globalKEY,jparty);

                }
                else {

                    /*for (unsigned i = 0; i<SpellMenu.selection.count; i++)
                        printf("%u. %d\n", i, SpellMenu.selection.ids[i]);*/

                    CastSpell(SpellMenu.selection.items[SpellMenu.state].id,
                              jparty, CasterIndex, jparty, partyfaces.face);
                    partyfaces.deactivate();
                    orderselect.reset();
                    SpellMenu.reset();
                    partyfaces.reset(jparty);
                    SpellMenu.deactivate();
                }

                if (partyfaces.canceled){
                    partyfaces.reset(jparty);
                    partyfaces.deactivate();
                    SpellMenu.reset();
                }

            }

            if (SpellMenu.canceled){
                SpellMenu.reset();
                SpellMenu.deactivate();

            }
        }
    }
  //-----------------------------------------------
    void M_UnequipItems(){

        if (!partyfaces.selected)  {
            if (!partyfaces.active())
                partyfaces.activate();

            partyfaces.getInput(globalKEY,jparty);

            if (partyfaces.selected)  {
                partyfaces.deactivate();
            }
        }
        else{
            if (!EquipBody.active())
                EquipBody.activate();

                EquipBody.getInput(globalKEY, SS, _MouseX, _MouseY);

                if (EquipBody.selected)  {
                    EquipBody.deactivate();
                    switch (EquipBody.state) {
                        case 0:UnequipItem(partyfaces.face, LEFTHAND);break;
                        case 1:UnequipItem(partyfaces.face, RIGHTHAND);break;
                        case 2:UnequipItem(partyfaces.face, ARRMOR);
                    }
                    EquipBody.reset();
                    orderselect.reset();
                    partyfaces.reset(jparty);
                }

                if (EquipBody.canceled){
                    EquipBody.reset();
                    EquipBody.deactivate();
                    partyfaces.reset(jparty);
                    partyfaces.activate();
                }
            }

            if (partyfaces.canceled){
                partyfaces.reset(jparty);
                partyfaces.deactivate();
                orderselect.reset();
            }
    }
    //------------------------------------
    void M_Options_Ingame(){

        if (!optmeniu.selected)  {
            if (!optmeniu.active())
                optmeniu.activate();

            optmeniu.getInput(globalKEY, SS);

            if (optmeniu.selected)  {

                switch(optmeniu.state) {
                    case 0:{
                        QuitMenu.activate();
                    }
                }
                optmeniu.reset();
                optmeniu.deactivate();
                orderselect.deactivate();
                orderselect.reset();
            }

            if (optmeniu.canceled){
                optmeniu.reset();
                optmeniu.deactivate();
                orderselect.reset();
            }
        }
    }
   //------------------------------------------
    void M_Save(){
        
        char buf[255];

        if (!Savemenu.selected){
            if (!Savemenu.active()){
                CreateSaveStateMenu();
                Savemenu.activate();
            }
            Savemenu.getInput(globalKEY, SS);
            if (Savemenu.selected){
                Savemenu.deactivate();
            }
        }
        else{
            sprintf(buf, "%s.godrpg/%ssaves/game%u.sav", HPath, Sys.basePath, Savemenu.state);

            if (! SaveState(jparty, innparty,
                        npcnation,
                        Kintamieji,
                        EmptyChests,
                        Currentmap,
                        buf))
            GameLog.write("Error!");

            GameLog.write("game saved!");
            Savemenu.reset();
            orderselect.reset();
        }

        if (Savemenu.canceled){
            Savemenu.deactivate();
            Savemenu.reset();
            orderselect.reset();
        }

    }

 //--------------------------------
    void NpcAI(){

        Npc* dude = npcnation.getArray();

        //printf("npc count %d\n",npcnation.count());
        for (unsigned z = 0; z < npcnation.count(); z++){


            char buf[50];
            sprintf(buf,"dude%d",z);
            //int ilg=(int)strlen(buf);

            if (dude->stepcount==1){

                char muf[100];
                char * val = 0;
                val = Kintamieji.getValue(buf);
                if (val){
                    if (strcmp(val,"inactive") != 0){
                        sprintf(muf,"Let(\"%s\",\"inactive\")",buf);
                        Interpret(muf);

                    }
                }
                else{
                    sprintf(muf,"Let(\"%s\",\"inactive\")",buf);
                    Interpret(muf);
                }

            }
            else{
                char muf[100];
                char * val = 0;
                val = Kintamieji.getValue(buf);
                if (val){
                    if (strcmp(val,"active") != 0){
                        sprintf(muf,"Let(\"%s\",\"active\")",buf);
                        Interpret(muf);
                    }
                }
                else{
                    sprintf(muf,"Let(\"%s\",\"active\")",buf);
                    Interpret(muf);
                }
            }

            if (dude->job<255)  {
                if ((dude->job==4)&&(dude->use_ai==false)) {

                    unsigned i=0;

                    while (((npcnation.getNpc(i)->x() != dude->x())||(npcnation.getNpc(i)->job != 255))
                            && (i < npcnation.count())){
                        i++;
                    }
                    if (i < npcnation.count()){
                        if ((npcnation.getNpc(i)->y() != dude->y() + 1)&&(npcnation.getNpc(i)->y() != dude->y() - 1))
                            dude->use_ai=true;
                    }
                    else{
                        unsigned i = 0;
                        while (((npcnation.getNpc(i)->y() != dude->y())||(npcnation.getNpc(i)->job != 255))&&(i < npcnation.count())){
                            i++;
                        }
                        if (i < npcnation.count()){
                            if ((npcnation.getNpc(i)->x() != dude->x() + 1)&&(npcnation.getNpc(i)->x() != dude->x() - 1))
                                dude->use_ai=true;
                        }
                        else  dude->use_ai=true;
                    }

                }

                if (dude->isWalk())
                    dude->inctimer();

                if ((!dude->getClock() > 0) && (dude->use_ai))  {
                    if (dude->redir(rand()%4+1, map, npcnation)){
                        if (dude->isWalk())
                            dude->go();
                        dude->stepcount=32;
                    }
                }
            }
            /*else {
                if (dude->stepcount>1)
                    printf("stepcount %d\n", dude->stepcount);
            }*/
            dude++;
        }

    }
 //-----------------------------------------
    void Talking(){
        Npc* hero = npcnation.getArray();//[npcnation.count-3];
        hero += npcnation.count() - jparty.count();
        unsigned i = 0;
        unsigned npcID = 0;

        while ((i < npcnation.count())&&
                ((npcnation.getNpc(i)->x()/32 != hero->x()/32 + hero->pdir.x)
                    || (npcnation.getNpc(i)->y()/32 != hero->y()/32 + hero->pdir.y)))
            i++;
        if (i < npcnation.count()){
            npcID = npcnation.getNpc(i)->ID();
        }
        else{

            i = 0;
            if (Events.count()){
                while (((Events.entries[i].x != hero->x()/32 + hero->pdir.x)
                        || (Events.entries[i].y!= hero->y()/32 + hero->pdir.y)
                        ||(Events.entries[i].type != 1))&&
                        (i < Events.count()))
                    i++;
                if (i< Events.count()){
                    printf("triger event: %d\n",i);
                    Interpret(Events.entries[i].code);
                    isTalking = false;
                    return;
                }
            }

        }

        if ((npcID>0)&&(npcID!=255))  {

            bool scriptRuns = false;
            if (ScriptStack.count()){
                if (ScriptStack[ActiveScript].isRunning())
                scriptRuns = true;
            }
            printf("gets some npc by id %u\n", npcID);
            Npc * guy = 0;
            guy = npcnation.getNpc(npcID - 1);
            if (guy){
                if ((guy->job != 255)&&(!scriptRuns))  {
                    guy->pdir.x = hero->pdir.x * -1;
                    guy->pdir.y = hero->pdir.y * -1;
                    puts(guy->speech);
                    Interpret(guy->speech);
                }
            }
        }
        isTalking = false;
    }

 //-----------------------------------------------
    //Tai kas vyksta kai zaidi; labai ilga procedura}
    void Game(){

        if (globalKEY == 96){
            if (Console.height() > 72)
                Console.setHeight(72);
            else
                Console.setHeight(160);
        }

        if (((!QuitMenu.active())
            &&(!Combatselect.active())
            &&(!ItemMenu.active())
            &&(!SpellMenu.active())
            &&(!orderselect.active())
            &&(! ENCinn)&&(! ENCsmith)
            &&(! ENCmersh))
            /*||(!PartyControl)*/){

            if (globalKEY == 27)  {

                QuitMenu.reset(); //resetinam quit Y/N
                QuitMenu.activate();
            }
        }


        if (QuitMenu.active())
            ExitYesNo();

        if (GameState == WALKING)  { //```````````````````

            static int tim = 0;
            tim ++;
            if (tim == 50){
                tim = 0;
                map.update(0);
                map.update(1);
                //map.update(2);
            }

            if (conversation.active()) {
               // PartyControl = false;
                conversation.type(oldKEY);
                Interpret("Let(\"conversation\",\"active\")");
            }
            else{
                Interpret("Let(\"conversation\",\"inactive\")");
                //PartyControl = true;
            }
            if (code_enter_enabled)  {
                code_enter=true;
                PartyControl=false;
            }
            if (globalKEY==SDLK_TAB)
                code_enter_enabled=true;

            if ((!orderselect.active())&&(!QuitMenu.active())&&(!weldone)
                &&(!ENCinn)&&(!ENCsmith)&&(!ENCmersh)){  //jei ne action_sel pradzia

                if ((globalKEY==32)&&(! code_enter)&&(PartyControl))
                    orderselect.activate();

                if ((oldKEY == 13) && (globalKEY != oldKEY) 
                    && (!code_enter) && (PartyControl)
                        &&(! conversation.active())){
                    isTalking = true;
                }

                if ((MapPushdirx!=0)||(MapPushdiry!=0)){
                    GlobalMapPushtimer++;

                    mapposx-=MapPushdirx;
                    mapposy-=MapPushdiry;
                    Vector3D campos = Cam3D.pos();
                    Cam3D.setpos(Vector3D(campos.v[0] - (MapPushdirx / 3.0f),
                                          campos.v[1],
                                          campos.v[2] + (MapPushdiry / 3.0f)));
                    Interpret("Let(\"camera\",\"moving\")");
                    if (GlobalMapPushtimer > 32){
                        GlobalMapPushtimer=0;
                        Interpret("Let(\"camera\",\"stop\")");

                        MapPushsteps--;

                        if (MapPushsteps<=0){
                            MapPushdirx=0;
                            MapPushdiry=0;
                        }

                    }
                }
                if (npcnation.count()){
                    //puts("ONCE AGAIN?");
                    Walking(); //reikia pataisyt
                }

            } //jei ne action_sel pabaiga

            if (!Cheat_NoEnemy)
                LookForEnemy();

        } //ifn (!Fight)
        //if SPACE pressed and action menu is active
        if (orderselect.active())
            orderselect.run(globalKEY, SS, _MouseX, _MouseY);

        if ((isTalking) && (npcnation.count()))
            Talking();

        if (ENCsmith)
            SmithShop();
        if (ENCmersh)
            Merchantshop();
        if (ENCinn)
            TheInn();

        NpcAI();

        if ((GameState == COMBAT))
            OnCombat();

        if (code_enter)
            CommandLine();
        
        if (ScriptStack.count()){

            if (ScriptStack[ActiveScript].isRunning())
                RunScript();
        }

        char* reiksme = 0;
        reiksme = Kintamieji.getValue("EndGame");
        if (reiksme){
            if (strcmp(reiksme,"true")==0){
                weldone = true;
            }
        }

    }
//------------------------------------------------
    void ResetGlobals(){


        puts(MainMenuMusic);
        PlayMusic(MainMenuMusic);

        bVisiKovesi = true;

        weldone = false;
        gameover = false;
        atacked = false;
        GameState = TITLEMENU;
        AF.activeFighter = 0;
        code_enter = false; //disables code entering
        visible_enemy = false;
        code_enter_enabled = false;
        Cheat_NoEnemy = false; //enables enemies

        MakeNextEncounterSteps();
        hMaker.heroIndex = 0;
        hMaker.reset();
        G_MemberIndex = 255;
        G_OponentIndex = 255;
        gs_daiktas=0;
        HeroActionSelect=false;
        HeroActionDone=false;
        for (unsigned long i = 0; i < ScriptStack.count(); i++ ){

            ScriptStack[i].clear();
        }
        ActiveScript = -1;

        ScriptStack.destroy();

        PartyControl=true;
        orderselect.reset();

        MapPushdirx = 0;
        MapPushdiry = 0;
        MapPushsteps = 0;

        innparty.destroy();

        Kintamieji.destroy();
        EmptyChests.destroy();

    }

 //-------------------------------------------
    void DrawIntroScreen(){
        Pics.draw(Pics.findByName("bg.tga"), 0, 0,  0,  false,
                    Sys.width/512.0f, Sys.height/256.0f);
        Pics.draw(Pics.findByName("title.tga"),
                    Sys.width - 300, 10,0);
        char buf[30];
        sprintf(buf,"Godrpg engine ver. %s", Version);
        int font = Pics.findByName("charai.tga");
        WriteText(Sys.width/2 - 98, Sys.height - 40, Pics, font,buf,
                  1.0f, 1.0f, COLOR(0.0f, 0.0f, 0.0f, 1.0f));
        WriteText(Sys.width/2 - 100, Sys.height - 42, Pics, font,buf);
        sprintf(buf, "made by jrs0ul 2003-%d", 2009);
        WriteText(Sys.width/2 - 98, Sys.height - 22, Pics, font, buf, 
                  1.0f, 1.0f, COLOR(0.0f, 0.0f, 0.0f, 1.0f));
        WriteText(Sys.width/2 - 100, Sys.height - 20, Pics, font, buf);
    }
//-------------------------------------------
    void M_NewGame(){
        NewGameMenu.activate();
        Mainmenu.deactivate();
        Mainmenu.reset();
    }
//-------------------------------------------
    void M_UseDefaultHero(){
        jparty.destroy();
        Creature pad;
        NewGameMenu.reset();
        char buf[255];
        sprintf(buf, "%sdata/main.chr", Sys.basePath);
        if(!pad.loadFromFile(buf, klases, Spells, itemData)){

            sprintf(buf, "base/data/main.chr");
            if (!pad.loadFromFile(buf, klases, Spells, itemData))
                return;
        }
        jparty.addMember(pad, Sys.basePath);
        NewGameMenu.deactivate();
        StartNewGame();
        Mainmenu.activate();
    }
//-------------------------------------------
    void M_MakeHero(){

        jparty.destroy();
        GameState = CREATEPARTY;
        CreateHeroMenu.activate();
        NewGameMenu.deactivate();
        NewGameMenu.reset();
    }

//-------------------------------------------
void M_Onward(){

    if (!Savemenu.selected){
        /*if (Mainmenu.active())
        Mainmenu.deactivate();*/
        if (!Savemenu.active()){
            CreateSaveStateMenu();
            Savemenu.activate();
        }
        Savemenu.getInput(globalKEY, SS, _MouseX, _MouseY);
        if (Savemenu.selected){
            Savemenu.deactivate();
        }

    }
    else{
        char buf[255];
        sprintf(buf, "%s.godrpg/%ssaves/game%u.sav",
                HPath, Sys.basePath, Savemenu.state);
        FILE * f = 0;
        f = fopen(buf, "rb");
        if (f){
            fclose(f);
            Onward(buf); //loads save game
            Mainmenu.reset();
            //Mainmenu.activate();
        }
        else{
            Savemenu.activate();
        }
        Savemenu.reset();
    }

    if (Savemenu.canceled){
        Savemenu.deactivate();
        Savemenu.reset();
        Mainmenu.reset();
       // Mainmenu.activate();
    }
}
//------------------------------------
void M_Options(){

    if (!OptionMenu.selected){
        if (!OptionMenu.active()){
            OptionMenu.activate();
        }
        OptionMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
        sprintf(OptionMenu.selection.items[0].name,
                "%s%dx%d", ML.data[13].items[0].name,
                Options_Width, Options_Heigt);
        sprintf(OptionMenu.selection.items[1].name,
                "%s%s", ML.data[13].items[1].name,
                (Sys.isWindowed)?"No":"Yes");
        sprintf(OptionMenu.selection.items[2].name,
                "%s%s", ML.data[13].items[2].name,
                (Sys.useShadows)?"Yes":"No");
        OptionMenu.resize();
        sprintf(OptionMenu.selection.items[3].name,
                "%s%s", ML.data[13].items[3].name,
                (Sys.useSound)?"Yes":"No");
         
    }
    else{

        switch(OptionMenu.state){

            case 0:{

                if (Sys.width == 640){
                    Options_Width = 800;
                    Options_Heigt = 600;
                }
                else{
                    Options_Width = 640;
                    Options_Heigt = 480;
                }
                Sys.needReboot = true;
            } break;
            case 1:{
                Sys.isWindowed =! Sys.isWindowed;
                Sys.needReboot = true;
            } break;
            case 2:{
                Sys.useShadows =! Sys.useShadows;
                Sys.needReboot = true;
            } break;
            case 3:{
                Sys.useSound = ! Sys.useSound;
            }
        }

        OptionMenu.reset();
    }

    if (OptionMenu.canceled){

        Sys.width = Options_Width;
        Sys.height = Options_Heigt;
        char path[255];
        sprintf(path, "%s.godrpg/conf.xml", HPath);
        Sys.write(path);

        if (Sys.needReboot){
            
            Sys.needReboot = false;
            Sys.reloadApp = true;
            Sys.quitApp = true;
        }

        OptionMenu.deactivate();
        OptionMenu.reset();
        Mainmenu.reset();
        Mainmenu.activate();
    }
}
//------------------------------------
bool HasBeginningScript(const char * dirname){
    FILE * f = 0;
    char buf[255];
    sprintf(buf,"%sscripts/begining.gsc", dirname);
    f = fopen(buf, "rt");
    if (f){
        fclose(f);
        return true;
    }
    return false;
}
//------------------------------------
void CreateModMenu(){
    SMenu mods;
    ModMenu.destroy();

    char buf[255];
    Lines l;
    GetDirectoryList(".", l);
    for (unsigned long i = 0; i < l.l.count(); i++){

        sprintf(buf, "./%s/", l.l[i].t);
        if (HasBeginningScript(buf)){
            MenuItem itm;
            strncpy(itm.name, l.l[i].t, 255);
            sprintf(buf, "%s/", l.l[i].t);
            strncpy(itm.value, buf, 255);
            mods.add(itm);
        }

    }
    l.l.destroy();
    sprintf(buf, "%s.godrpg/", HPath);
    GetDirectoryList(buf, l);
    for (unsigned long i = 0; i < l.l.count(); i++){
        sprintf(buf, "%s.godrpg/%s/", HPath, l.l[i].t);
        puts(buf);
        if (HasBeginningScript(buf)){
            MenuItem itm;
            strncpy(itm.name, l.l[i].t, 255);
            strncpy(itm.value, buf, 255);
            mods.add(itm);
        }
    }

    l.l.destroy();

    mods.setTitle("Choose mod:");
    ModMenu.init(Sys.width/2 - 50, Sys.height/2 - 20, mods, 0);
    mods.destroy();
}

//------------------------------------
void M_Mods(){

    if (!ModMenu.selected){
        if (!ModMenu.active()){
            ModMenu.activate();
        }
        ModMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
        if (ModMenu.selected){
            ModMenu.deactivate();
        }
    }
    else{

        strcpy(Sys.basePath, ModMenu.selection.items[ModMenu.state].value);
        puts(Sys.basePath);
        char path[255];
        sprintf(path, "%s.godrpg/conf.xml", HPath);
        Sys.write(path);
        Sys.quitApp = true;
        Sys.reloadApp = true;

        ModMenu.reset();
        Mainmenu.reset();
        Mainmenu.activate();
        ModMenu.deactivate();
    }

    if (ModMenu.canceled){
        ModMenu.deactivate();
        ModMenu.reset();
        Mainmenu.reset();
        Mainmenu.activate();
    }
}
//------------------------------------
void M_Credits(){

    if (!CreditWindow.active()){
        CreditWindow.activate();
        char buf[255];
        sprintf(buf, "%scredits.txt", Sys.basePath);
        CreditWindow.load(buf);
    }
    else{
        CreditWindow.getInput(globalKEY);

        if (!CreditWindow.active()){
            CreditWindow.reset();
            Mainmenu.reset();
            Mainmenu.activate();
        }
    }

}
 //===============================================================
  //Some subroutines to create heroes
    void SelectRace (unsigned hero, unsigned key){
        racesel.getInput(globalKEY, SS);

        if (racesel.selected)  {
            puts("Race selected");

            jparty.member(hero)->race = racesel.state;

            sprintf(CreateHeroMenu.selection.items[0].name,
                    "Race: %s", Rases.get(racesel.state)->name);

            if (!hMaker.raceSelected)  hMaker.raceSelected = true;

            if (Rases.get(jparty.member(hero)->race)->maxsex == 1)  {
                puts("select sex");
                jparty.member(hero)->sex = 0;
                if (!hMaker.sexSelected)
                    hMaker.sexSelected = true;
            }
            racesel.reset();
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();
            puts("done");
        }

        if (racesel.canceled){
            racesel.reset();
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();
            hMaker.raceSelected = false;
        }
    }

//-------------------------

    void SelectSex (unsigned hero, unsigned short key){

        if (hMaker.raceSelected){

            if (!sexsel.active())
                sexsel.activate();

            if (Rases.get(jparty.member(hero)->race)->maxsex > 1)  {
                sexsel.getInput(globalKEY, SS, _MouseX, _MouseY);

                if (sexsel.selected)  {
                    jparty.member(hero)->sex = sexsel.state;
                    sprintf(CreateHeroMenu.selection.items[1].name,
                            "Sex: %s", 
                            ML.data[8].items[sexsel.state].name);
                    if (!hMaker.sexSelected)
                        hMaker.sexSelected = true;
                    sexsel.reset();
                    sexsel.deactivate();
                    CreateHeroMenu.reset();
                    CreateHeroMenu.activate();

                }

                if (sexsel.canceled){
                    sexsel.reset();
                    hMaker.sexSelected = false;
                    CreateHeroMenu.reset();
                    sexsel.deactivate();
                    CreateHeroMenu.activate();
                }
            }
            else {
                jparty.member(hero)->sex = 0;
                sprintf(CreateHeroMenu.selection.items[1].name,
                        "Sex: %s", ML.data[8].items[0].name);
                if (!hMaker.sexSelected)
                    hMaker.sexSelected = true;
                CreateHeroMenu.reset();
                sexsel.deactivate();
                CreateHeroMenu.activate();
            }

        }
        else{
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();
        }
    }

  //-------------------------

    void SelectClass (unsigned hero, unsigned key){

        classsel.getInput(globalKEY, SS, _MouseX, _MouseY);

        if (classsel.selected)  {
            jparty.member(hero)->klass = classsel.state;

            sprintf(CreateHeroMenu.selection.items[2].name, "Class: %s",
                    klases.get(classsel.state)->name);
            if (!hMaker.classSelected)
                hMaker.classSelected = true;
            classsel.reset();
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();
        }

        if (classsel.canceled){
            classsel.reset();
            CreateHeroMenu.activate();
            CreateHeroMenu.reset();
            hMaker.classSelected = false;

            if (!hMaker.nameWriten){
                if (CreateHeroMenu.selection.count() > 3)
                    CreateHeroMenu.selection.remove(3);
            }

        }
  }

//-------------------------

    void WriteName (unsigned char hero, unsigned char key){

        NameEnter.getInput(key);
        if (!Board.active())
            Board.activate();
        unsigned char c = 0;
        c = Board.getInput(key);
        if (c){
            if (c == 8)
                NameEnter.delchar();
            else
                NameEnter.addchar((char)c);
        }
        if ((Board.entered) || (Board.canceled))
            Board.deactivate();

        if (Board.entered)  {
            strcpy(jparty.member(hero)->name, NameEnter.text());
            sprintf(CreateHeroMenu.selection.items[3].name, 
                    "Name: %s", NameEnter.text());

            if (!hMaker.nameWriten)
                hMaker.nameWriten = true;
            NameEnter.reset();
            Board.reset();
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();

        }

        if (NameEnter.canceled){
            NameEnter.reset();
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();
            Board.reset();
        }

    }
//-------------------------
    void RollDice (unsigned hero){

        Creature * dude = jparty.member(hero);

        //sets evolution speed
        dude->espeed = Rases.get(dude->race)->espeed; 
        printf("espeed %d\n", dude->espeed);
        dude->rollDice(Rases, klases); //generuojam statusa
        printf("espeed %d\n", dude->espeed);

        if (!hMaker.dicesRolled)
            hMaker.dicesRolled = true;

        CreateHeroMenu.reset();
        CreateHeroMenu.activate();

        SMenu fm;
        strncpy(dude->faceTileSet,
                PML.races[dude->race].clas[dude->klass].sex[dude->sex].f,
                255);

        GameLog.write(PML.races[dude->race].clas[dude->klass].sex[dude->sex].f);

        int tileset = Pics.findByName(dude->faceTileSet);
        unsigned pcount = Pics.getInfo(tileset)->vframes *
                          Pics.getInfo(tileset)->hframes;

        for (unsigned i = 0; i < pcount; i++){

            MenuItem itm;
            strcpy(itm.name, "face");
            itm.icon = i;
            fm.add(itm);
        }

        SelectNewFace.destroy();
        fm.setTitle("Hero face:");
        SelectNewFace.init(10, 10, fm, 0, 250);
        fm.destroy();
    }
  //------------------------------
    void SelectFace(unsigned hero, unsigned key){

        SelectNewFace.getInput(key, SS, _MouseX, _MouseY);

        if (SelectNewFace.selected){
            jparty.member(hero)->faceID = SelectNewFace.state;
            if (!hMaker.faceSelected)
                hMaker.faceSelected = true;
            SelectNewFace.reset();
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();
        }

        if (SelectNewFace.canceled){
            SelectNewFace.reset();
            CreateHeroMenu.reset();
            CreateHeroMenu.activate();
            CreateHeroMenu.selection.remove(CreateHeroMenu.selection.count()-1);
            hMaker.faceSelected = false;
        }
    } 

 //-------------------------------------------------------
    void StartNewGame(){

        char buf[512];

        for (unsigned i = 0; i < jparty.count(); i++){

            Creature* dude = jparty.member(i);
            printf("SPELL COUNT: %u\n", dude->spellCount());

            dude->enablespells(klases, Spells);
            sprintf(buf, "Let(\"hero%dname\",\"%s\")", i, dude->name);
            Interpret(buf);
            sprintf(buf, "Let(\"hero%dface\",\"%s\")", 
                    i, dude->faceTileSet);
            Interpret(buf);
            sprintf(buf,"Let(\"hero%dfaceID\",\"%d\")",
                    i, dude->faceID);
            Interpret(buf);
        }

        GameState = WALKING;


        Interpret("Exec(\"scripts/begining.gsc\")");
 
    }

 //-------------------------------------------------------
    void CreateParty(){

        const int memberscount=1;

        if (! CreateHeroMenu.selected){
            CreateHeroMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
        }
        else {

            CreateHeroMenu.deactivate();

            if (hMaker.heroIndex + 1 > jparty.count()){
                Creature tmp;
                //tmp.exp = 0;

                jparty.addMember(tmp, Sys.basePath, false);
            }

            Creature * dude = 0;
            dude = jparty.member(hMaker.heroIndex);


            switch(CreateHeroMenu.state){
                case 0:SelectRace (hMaker.heroIndex, globalKEY);break;
                case 1:SelectSex  (hMaker.heroIndex, globalKEY);break;
                case 2:SelectClass(hMaker.heroIndex, globalKEY);break;
                case 3:WriteName  (hMaker.heroIndex, globalKEY);break;
                case 4:{
                            if (dude->level() < 2)
                                dude->exp += Rases.get(dude->race)->espeed * 2;
                            RollDice   (hMaker.heroIndex);
                        } break;
                case 5:SelectFace(hMaker.heroIndex, globalKEY);break;
                case 6:{
                    hMaker.nexthero=true;
                    CreateHeroMenu.reset();

                    FighterModel nmd;
                    char buf[255];
                    unsigned hrace = dude->race;

                    sprintf(buf, "%smodels/%s.rm2", Sys.basePath,
                           PML.races[hrace].clas[dude->klass].sex[dude->sex].m);

                    if (!nmd.fighter.load(buf)){
                        sprintf(buf, "base/models/%s.rm2", 
                                PML.races[hrace].clas[dude->klass].sex[dude->sex].m);
                        if (!nmd.fighter.load(buf))
                            GameLog.write("model loading failed!");
                    }
                    jparty.member(hMaker.heroIndex)->modelRadius = 
                        PML.races[hrace].clas[dude->klass].sex[dude->sex].radius;

                    strcpy(nmd.name,
                           PML.races[hrace].clas[dude->klass].sex[dude->sex].m);
                    strcpy(jparty.member(hMaker.heroIndex)->modelfile,
                           PML.races[hrace].clas[dude->klass].sex[dude->sex].m);
                    jparty.models.add(nmd);
                    sprintf(buf, "%s.tga", 
                            PML.races[hrace].clas[dude->klass].sex[dude->sex].t);
                    strcpy(jparty.member(hMaker.heroIndex)->texturefile,
                           PML.races[hrace].clas[dude->klass].sex[dude->sex].t);

                    jparty.modelTextures.loadFile(buf, hMaker.heroIndex,
                                                  512, 512, Sys.basePath, 1);


                    strcpy(jparty.member(hMaker.heroIndex)->npcsprite,
                           PML.races[hrace].clas[dude->klass].sex[dude->sex].s);

                }
            }

            if (hMaker.raceSelected){

                if (CreateHeroMenu.selection.count() < 2)
                    CreateHeroMenu.selection.add(ML.data[7].items[1]);


                if ((hMaker.sexSelected)
                    && (hMaker.classSelected))  {

                        if (CreateHeroMenu.selection.count() < 4 )
                            CreateHeroMenu.selection.add(ML.data[7].items[3]);
                        if (hMaker.nameWriten)  {
                            if (CreateHeroMenu.selection.count() < 5 )
                                CreateHeroMenu.selection.add(ML.data[7].items[4]);
                            if (hMaker.dicesRolled) {
                                if (CreateHeroMenu.selection.count() < 6 )
                                    CreateHeroMenu.selection.add(ML.data[7].items[5]);

                                if (hMaker.faceSelected){
                                    if (CreateHeroMenu.selection.count() < 7 )
                                        CreateHeroMenu.selection.add(ML.data[7].items[6]);
                                }
                            }
                        }
                    }
                }

            if (hMaker.nexthero)  {

                puts("give some shared items");
                SharedItems(*jparty.member(hMaker.heroIndex));

               

                hMaker.heroIndex++;
                hMaker.reset();

                CreateHeroMenu.selection.destroy();

                CreateHeroMenu.selection.add(ML.data[7].items[0]);
                CreateHeroMenu.selection.add(ML.data[7].items[1]);
                CreateHeroMenu.selection.add(ML.data[7].items[2]);
                //MenuItem itm;
                //CreateHeroMenu.selection.add(itm);


                if (hMaker.heroIndex < memberscount - 1){
                    char buf[30];
                    sprintf(buf, "Creating %d hero: ", hMaker.heroIndex + 1);
                    CreateHeroMenu.setTitle(buf);
                }
            }

            if (hMaker.heroIndex > memberscount - 1)  {
                puts("enough");
                hMaker.reset();
                hMaker.heroIndex = 1;
                Mainmenu.activate();

                StartNewGame(); 
            }

        }

        if (CreateHeroMenu.canceled){
            CreateHeroMenu.reset();
            GameState = TITLEMENU;
            Mainmenu.reset();
            CreateHeroMenu.selection.destroy();

            //TODO: ain't very nice
            CreateHeroMenu.init(3, 3, ML.data[7], 0);

            CreateHeroMenu.selection.destroy();

            CreateHeroMenu.selection.add(ML.data[7].items[0]);
            CreateHeroMenu.selection.add(ML.data[7].items[1]);
            CreateHeroMenu.selection.add(ML.data[7].items[2]);

            hMaker.reset();
            NewGameMenu.activate();
        }
    }

//---------------
void GameOver(){

    int width = 180;
    int height = 80;
    int posx = Sys.width/2 - width/2;
    int posy = Sys.height/2 - height/2;

    unsigned chars = Pics.findByName("charai.tga");

    Pics.draw(-1, posx, posy, 0, false, width, height, 0, 
              COLOR(0.0f, 0.0f, 1.0f, 1.0f),
              COLOR(0.0f, 0.0f, 0.0f, 0.5f));
    WriteText(posx + 42, posy + 30, Pics, chars, "GAME OVER");

    Coolframe(posx, posy, width, height, Pics,
              Pics.findByName("frm.tga"));

}
//--------------------------------------------------
void LoadGraphics(){

    Pics.loadFile(picc, Sys.basePath);
    picc++;
    if (picc == SYSTEM_imagefilescount) {
        picc = 0;
        SYSTEM_filesloaded=true;
        fader = 0.0f;
        GameLog.write("Graphics loaded!");
    }
}
//-----------------------------------
    void CreateSaveStateMenu(){
        SMenu states;
        Savemenu.destroy();
        for (unsigned i = 0; i < MaxSaveStates; i++){
            MenuItem itm;
            char buf[255];
            sprintf(buf, "%s.godrpg/%ssaves/game%u.sav", HPath, Sys.basePath, i);
            FILE * f = 0;
            f = fopen(buf, "rb");
            if (f){

                time_t lol;
                if (fread(&lol, sizeof(time_t), 1, f)){
                    tm * timeinfo;
                    timeinfo = localtime (&lol);
                    strftime(buf, sizeof(buf), "%y.%m.%d %H:%M:%S", timeinfo);
                }
                else
                    sprintf(buf, "save %u", i);
                strncpy(itm.name, buf, 255);
                fclose(f);
            }
            else
                strncpy(itm.name, "--empty--", 255);
            states.add(itm);
        }
        states.setTitle("Select state:");
        Savemenu.init(Sys.width/2 - 50, Sys.height/2 - 20, states, 0);
        states.destroy();
    }

//-----------------------------------
void OnBegin(){

    SMenu tmpmen;
    char path[214];

    srand((int)time(0));

    SYSTEM_filesloaded=false;//failai nepakrauti
    GameLog.write("loading picture list");

    sprintf(path, "%spics/piclist.txt", Sys.basePath);

    if (!Pics.initContainer(path))
        QuitApp();
    picc=0;

    SYSTEM_imagefilescount = Pics.count();
    char buf[50];
    sprintf(buf, "Pictures to load: %lu", SYSTEM_imagefilescount);
    GameLog.write(buf);

    GameLog.write("Loading objects");

    sprintf(path, "%sdata/", Sys.basePath);

    LoadObjects(path);

    GameLog.write("Loading menus");

    sprintf(path, "%sdata/menus.xml", Sys.basePath);

    if (!ML.loadMenus(path))
        GameLog.write("can't find menu file");
    else
        GameLog.write("done!");

    ResetGlobals();//resetinam globalius kintamuosius
    strncpy(Currentmap, "map", 255);

    GameState = TITLEMENU;//we'll se the main menu

    partyfaces.init(0, Sys.height - 100, true,jparty);   //snukiu selectoriaus init

    SMenu hrace;
    for (unsigned a = 0; a < Rases.count(); a++){
        if (Rases.get(a)->isPlayable){
            MenuItem itm;
            strcpy(itm.name, Rases.get(a)->name);
            hrace.add(itm);
        }
    }
    GameLog.write("Creating some menus...");
    GameLog.write("Hero menu");

    if (ML.data.count() > 7){
        CreateHeroMenu.init(3, 3, ML.data[7], 0);
        GameLog.write("Done");

        CreateHeroMenu.selection.destroy();

        CreateHeroMenu.selection.add(ML.data[7].items[0]);
        CreateHeroMenu.selection.add(ML.data[7].items[1]);
        CreateHeroMenu.selection.add(ML.data[7].items[2]);
    } 

    hrace.setTitle("choose hero's race");
    racesel.init (10, 10, hrace,0);
    hrace.destroy();

    sexsel.init(10, 10, ML.data[8],0);

    GameLog.write("Class menu");
    for (int a = 0; a < 3;a++){
        MenuItem itm;
        strcpy(itm.name, klases.get(a)->name);
        tmpmen.add(itm);
    }

    classsel.destroy();
    tmpmen.setTitle("choose hero's class :");
    classsel.init(10, 10, tmpmen, 0);


    NameEnter.init(Sys.width/2 - 110, Sys.height/2 - 200, "Enter hero's name:", 7);


    Console.init(100,0,160,19*18);
    conversation.init(90,100,80,19*18);

    SmithMeniu.init(10, Sys.height - 220, ML.data[11],0);
    merchantmeniu.init(10, Sys.height - 220, ML.data[6],0);
    InnMenu.init(10, Sys.height - 220, ML.data[10],0);


    //adventure menu, activated with 'space'
    orderselect.init(Sys.width - 160, 5, ML.data[0],0);
    orderselect.selection.items[0].function = CastOrder;
    orderselect.selection.items[1].function = HandleInventory;
    orderselect.selection.items[2].function = M_UnequipItems;
    orderselect.selection.items[3].function = HeroInfo;
    orderselect.selection.items[4].function = M_Options_Ingame;
    orderselect.selection.items[5].function = M_Save;

    optmeniu.init(380,10, ML.data[1],0);
    invsel.init(0,80, ML.data[2],0);
    EquipBody.init(0,121, ML.data[3],0);

    QuitMenu.init(10, 10, ML.data[9],0);
    //command line
    CodeEdit.init(10,10,"codeline:",40);
    ItemCountedit.init(100, 100, "How much ?:", 4);
    Mainmenu.init(Sys.width/2 - 50, 200, ML.data[5], 1);
    Mainmenu.selection.items[0].function = M_NewGame;
    Mainmenu.selection.items[1].function = M_Onward;
    Mainmenu.selection.items[2].function = M_Options;
    Mainmenu.selection.items[3].function = M_Mods;
    Mainmenu.selection.items[4].function = M_Credits;
    Mainmenu.selection.items[5].function = QuitApp;

    CreateSaveStateMenu();
    CreateModMenu();

    NewGameMenu.init(Sys.width/2 - 50, 200, ML.data[12], 0);
    NewGameMenu.selection.items[0].function = M_UseDefaultHero;
    NewGameMenu.selection.items[1].function = M_MakeHero;


    OptionMenu.init(Sys.width/2 - 50, 200, ML.data[13], 0);

    CreditWindow.width = Sys.width - Sys.width/4;
    CreditWindow.height = Sys.height / 3;
    CreditWindow.setpos(Sys.width/2 - CreditWindow.width/2,
                        Sys.height/2 - CreditWindow.height/2);
    CreditWindow.height += Sys.height/5; 


    Board.setpos(Sys.width/2 - Board.width/2, Sys.height/2 - Board.height/2);
    char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ,.-!0124567890";
    Board.setChars((const unsigned char*)chars);
    Board.setRowLen(13);

    tmpmen.destroy();
    GameLog.write("done");
}
//-----------------------------------------------
    Vector3D MiddleVec(Vector3D start, Vector3D end,
                       unsigned ticksPerNode,
                       unsigned ticks,
                       unsigned currentnode){

        Vector3D dir = end - start;
        float v = (dir.length() / ticksPerNode) * 
                  (ticks - ticksPerNode * currentnode) * 1.0f;
        dir.normalize();
        dir = Vector3D(dir.x() * v , dir.y() * v , dir.z() * v );
        return dir;

    }

    //-------------------------------------------
    void ConsoleRoutine(){
        if (GameState != TITLEMENU){
            if (Console.active()){ //raso konsoleje po viena simboli
                static int t_tmp=0;
                t_tmp++;
                if (t_tmp==5){
                    t_tmp=0;
                    Console.type(oldKEY);
                }
                Interpret("Let(\"console\",\"active\")");
            }
            else{
                Interpret("Let(\"console\",\"inactive\")");
            }
        }

    }

//------------------------------------------------
void OnTimer(){

    if (!SYSTEM_filesloaded){
        LoadGraphics();
    }
    else {

        if (GameState != BEGIN_COMBAT){
            if (fader < 1.0f)
                fader += 0.005f;
        }
        //-------------------------
        ConsoleRoutine(); 

        switch (GameState){

        case TITLEMENU:{
            if (Mainmenu.active()){
                if (Mainmenu.run(globalKEY, SS, _MouseX, _MouseY) < 0)
                    QuitApp();
            }
            if (NewGameMenu.active()){
                if (NewGameMenu.run(globalKEY, SS, _MouseX, _MouseY) < 0)
                    Mainmenu.activate();
            }

        }
        break;

        case CREATEPARTY:{
            CreateParty();       //kuriame herojus
        }
        break;

        case BEGIN_COMBAT:{
            Combat_beginclock ++;
            fader -= 0.005f;
            Cam3D.roll(0.05f);
            if (Combat_beginclock > 180){
                Entit.destroy();
                HeroActions.destroy();
                EnemyActions.destroy();

                CAnimControl.reset();
                Encounter(Combat_groupindex, "fighton");
                HeroActions.addEmpty(jparty.count());
                EnemyActions.addEmpty(eparty.count());
                Console.setpos(5, Sys.height - 180);
                Combat_beginclock = 0;
            }
        } break;

        case COMBAT_INTRO:{
            Combat_Introclock ++;

            if (Arena.introPathCount()){

                unsigned totalSections = Arena.introPathCount() - 1;
                unsigned ticksPerNode = IntroPathTicks /
                                        (totalSections);
                unsigned currentSection = Combat_Introclock / 
                                          ticksPerNode;

                CameraState * cstart = 0;
                CameraState * cend = 0;
                cstart = Arena.getIPathNode(currentSection);
                cend = Arena.getIPathNode(currentSection + 1);

                if ((cstart) && (cend)){

                    Cam3D.setpos(cstart->pos +
                                 MiddleVec(cstart->pos, cend->pos,
                                                   ticksPerNode, 
                                                   Combat_Introclock,
                                                   currentSection));
                            Cam3D.setdir(cstart->dir +
                                         MiddleVec(cstart->dir, cend->dir,
                                                   ticksPerNode,
                                                   Combat_Introclock,
                                                   currentSection));
                            Cam3D.setright(cstart->right +
                                         MiddleVec(cstart->right,
                                                   cend->right,
                                                   ticksPerNode,
                                                   Combat_Introclock,
                                                   currentSection));
                            Cam3D.setup(cstart->up +
                                         MiddleVec(cstart->up,
                                                   cend->up, ticksPerNode,
                                                   Combat_Introclock,
                                                   currentSection));
                        }
                        else
                        if (cstart){
                            Cam3D.setpos(cstart->pos);
                            Cam3D.setdir(cstart->dir);
                            Cam3D.setright(cstart->right);
                            Cam3D.setup(cstart->up);
                        }


                    }

                    if (Combat_Introclock > IntroPathTicks){
                        GameState = COMBAT;
                        Combat_Introclock = 0;
                    }
                } break;


                case VICTORY:{
                    VictoryClock ++;

                    if (Arena.outroPathCount()){

                        unsigned totalSections = Arena.outroPathCount() - 1;
                        unsigned ticksPerNode = OutroPathTicks / 
                                                (totalSections);
                        unsigned currentSection = VictoryClock / ticksPerNode;

                        CameraState * cstart = 0;
                        CameraState * cend = 0;
                        cstart = Arena.getOPathNode(currentSection);
                        cend = Arena.getOPathNode(currentSection + 1);

                        if ((cstart) && (cend)){

                            Cam3D.setpos(cstart->pos +
                                         MiddleVec(cstart->pos, cend->pos,
                                                   ticksPerNode,
                                                   VictoryClock,
                                                   currentSection));
                            Cam3D.setdir(cstart->dir +
                                         MiddleVec(cstart->dir,
                                                   cend->dir, ticksPerNode,
                                                   VictoryClock,
                                                   currentSection));
                            Cam3D.setright(cstart->right +
                                         MiddleVec(cstart->right,
                                                   cend->right,
                                                   ticksPerNode,
                                                   VictoryClock,
                                                   currentSection));
                            Cam3D.setup(cstart->up +
                                         MiddleVec(cstart->up,
                                                   cend->up,
                                                   ticksPerNode,
                                                   VictoryClock,
                                                   currentSection));
                        }
                        else
                        if (cstart){
                            Cam3D.setpos(cstart->pos);
                            Cam3D.setdir(cstart->dir);
                            Cam3D.setright(cstart->right);
                            Cam3D.setup(cstart->up);
                        }

                    }


                    if (VictoryClock > OutroPathTicks){
                        GameState = COMBAT_STATS;
                        VictoryClock = 0;
                        OnVictory();
                    }
                    
                } break;
                
                case COMBAT_STATS:{


                    if (ItemGain.active()){
                        ItemGain.getInput(globalKEY);
                    }

                    if (StatGain.active()){
                        StatGain.getInput(globalKEY);
                        if (!StatGain.active()){
                            if (ItemGain.itemCount())
                                ItemGain.activate();
                        }
                    }

                   
                    if ((!Console.active()) && (!StatGain.active())
                            && (!ItemGain.active())){

                        End_combat();
                        Victory = false;
                    }

            } break;

            default:{
                if (!jparty.alive())
                    gameover = true;
                Game();      //leidziam zaidima

                if (gameover){
                    if (oldKEY)  {
                        ResetGlobals();
                    }
                }

                if  (weldone) {
                    GameState = TITLEMENU;
                    ResetGlobals(); //resetinam globalius kintamuosius

                }
            }//default
        }

        oldKEY=globalKEY;
        globalKEY=0;//resetinam global keyu
    }
}
//----------------------------------
    int FPS (void){
        static int time = 0, FPS = 0, frames = 0, frames0 = 0;
        if ((int)SDL_GetTicks() >= time) {
            FPS = frames-frames0;
            time = SDL_GetTicks()+1000;
            frames0 = frames;
        }
        frames = frames+1;
        return FPS;
    }


//------------------------------------

void CheckKeys(){

    Uint8 * keyz;
    int JoyNum = 0;

    OldJoyX = JoyX;
    OldJoyY = JoyY;

    keyz = SDL_GetKeyState ( NULL );
    JoyNum = SDL_NumJoysticks();


    if ((JoyNum > 0) && (Sys.useGamepad)) {

        //printf("%s\n", SDL_JoystickName(0));

        SDL_JoystickUpdate ();
        JoyX = SDL_JoystickGetAxis(Joy, 0);
        JoyY = SDL_JoystickGetAxis(Joy, 1);
    }


    int bm;
    bm = SDL_GetRelativeMouseState ( &MouseX,&MouseY );
    SDL_GetMouseState(&_MouseX, &_MouseY);


    memset(keys,0,11);

    //------------------------------
    if ( keyz[SDLK_UP] == SDL_PRESSED )
        keys[0] = 1;
    if ( keyz[SDLK_DOWN] == SDL_PRESSED )
        keys[1] = 1;
    if ( keyz[SDLK_RIGHT] == SDL_PRESSED )
        keys[3] = 1;
    if ( keyz[SDLK_LEFT] == SDL_PRESSED )
        keys[2] = 1;
    if ( keyz[SDLK_RETURN] == SDL_PRESSED )
        keys[4] = 1;
    if ( keyz[SDLK_ESCAPE] == SDL_PRESSED )
        keys[5] = 1;
    if ( keyz[SDLK_SPACE] == SDL_PRESSED )
        keys[6] = 1;


    if ((JoyNum) && (Sys.useGamepad)){
        if (JoyY<-1)
            keys[0] = 1;
        if (JoyY>1)
            keys[1] = 1;
        if (JoyX>1)
            keys[3] = 1;
        if (JoyX<-1)
            keys[2] = 1;

        if (SDL_JoystickGetButton (Joy, 0))
            keys[4] = 1;
        if (SDL_JoystickGetButton (Joy, 1))
            keys[5] = 1;
        if (SDL_JoystickGetButton (Joy, 2))
            keys[6] = 1;
    }

    if (bm & SDL_BUTTON ( 1 ))
        keys[4] = 1;


    if (keyz[SDLK_w] == SDL_PRESSED) keys[7] = 1;
    if (keyz[SDLK_a] == SDL_PRESSED) keys[8] = 1;
    if (keyz[SDLK_s] == SDL_PRESSED) keys[9] = 1;
    if (keyz[SDLK_d] == SDL_PRESSED) keys[10] = 1;

}

//-------------------
void DrawSomeText(int fontas){

    char buf[80];

    unsigned x = Sys.width - 250;
    unsigned y = Sys.height - 120;
    sprintf(buf, "pos [%.2f %.2f %.2f]", 
            Cam3D.pos().x(), Cam3D.pos().y(), Cam3D.pos().z());
    WriteText(x, y, Pics, fontas, buf,
              1.0f, 1.0f, COLOR(1.0f, 1.0f, 1.0f, 0.5f));
    sprintf(buf, "dir [%.2f %.2f %.2f]",
            Cam3D.dir().x(), Cam3D.dir().y(), Cam3D.dir().z());
    WriteText(x, y + 20, Pics, fontas, buf,
              1.0f, 1.0f, COLOR(1.0f, 1.0f, 1.0f, 0.5f));
    sprintf(buf, "rig [%.2f %.2f %.2f]", 
            Cam3D.right().x(), Cam3D.right().y(), Cam3D.right().z());
    WriteText(x, y+40, Pics, fontas, buf,
              1.0f, 1.0f, COLOR(1.0f, 1.0f, 1.0f, 0.5f));
    sprintf(buf, "up [%.2f %.2f %.2f]",
            Cam3D.up().x(), Cam3D.up().y(), Cam3D.up().z());
    WriteText(x, y+60, Pics, fontas, buf,
              1.0f, 1.0f, COLOR(1.0f, 1.0f, 1.0f, 0.5f));

    sprintf(buf,"FPS : %d",FPS());
    WriteText(x, y+80, Pics, fontas, buf,
              1.0f, 1.0f, COLOR(1.0f, 1.0f, 1.0f, 0.5f));

}

//---------------------------------
void DrawFighter(unsigned i, 
                 COLOR normal, COLOR poison, bool outline){

    const float raddegrees = 57.2957795f;

    glPopMatrix();
    glPushMatrix();

    Fighter * dude = 0;
    dude = Entit.get(i);

    if (!dude)
        return;


    //heroes party
    bool poisoned = false;
    if (dude->type == HERO){
        if (!jparty.member(dude->nr))
            return;

        if (jparty.member(dude->nr)->state == POISONED)
            poisoned = true;

        //no need to draw escaped member
        if (jparty.member(dude->nr)->escaped())
            return;
    }
    else{

        if (!eparty.member(dude->nr))
            return;

        if (eparty.member(dude->nr)->state == POISONED)
            poisoned = true;

        if (eparty.member(dude->nr)->escaped())
            return;
    }
    if (poisoned){

        glMaterialfv(GL_FRONT, GL_DIFFUSE, poison.c);
        glMaterialfv(GL_FRONT, GL_AMBIENT, poison.c);
    }
    else{
        glMaterialfv(GL_FRONT, GL_DIFFUSE, normal.c);
        glMaterialfv(GL_FRONT, GL_AMBIENT, normal.c);
    }


    if (dude->type == MONSTER){

        glTranslatef(dude->pos.x(), 0.2f, dude->pos.z());

        if (!EnemyActions.act(dude->nr))
            return;

        if (EnemyActions.act(dude->nr)->action != ATACK)
            glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
        else
            glRotatef((3.14f + atan2(dude->dir.x(),
                                     dude->dir.z()))*raddegrees,
                      0.0, 1.0f, 0.0f);

    }
    else{
        glTranslatef(dude->pos.x(), 0.2f, dude->pos.z());


        if (!HeroActions.act(dude->nr))
            return;

        if (HeroActions.act(dude->nr)->action != ATACK)
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        else
            glRotatef((3.14f 
                       + atan2(dude->dir.x(), dude->dir.z())) * raddegrees,
                      0.0, 1.0f, 0.0f);

    }

    glScalef(0.5f, 0.5f, 0.5f);


    /*JustColor.use();
    if (dude->type == 2){
        DummyCircle(eparty.member(dude->nr)->modelRadius);
    }
    else{
        DummyCircle(jparty.member(dude->nr)->modelRadius);
    }*/

    Party * party = 0;
    if (dude->type == HERO)
        party = &jparty;
    else
        party = &eparty;

    Fighter * f = 0;
    f = Entit.get(i);

    glBindTexture(GL_TEXTURE_2D, party->modelTextures.getname(f->nr));

    SubsetCollection* model = 0;
    model = &party->models[f->nr].fighter;

    unsigned frame = f->frame;
    if (party->member(f->nr)->state == DEAD)
        frame = 23;

    if (!outline){
        if (!(model->draw(0, frame)))
            DummyFighter.draw();
    }
    if (outline){
        if (!(model->drawOutline(0, frame))){
            DummyFighter.drawOutline();
        }
    }
}

//-------------------------------
void DrawArenaModels(bool drawBlack = false){

    COLOR black(0.0f, 0.0f, 0.0f, 1.0f);
    COLOR white(1.0f, 1.0f, 1.0f, 1.0f);
    COLOR green(0.0f, 1.0f, 0.0f, 1.0f);
    if (drawBlack){

        Arena.draw(0, false, true);

        for (unsigned i = 0; i < Entit.count(); i++)
            DrawFighter(i, black, black, false);
    }
    else{

        Arena.draw(0, false);

        for (unsigned i = 0; i < Entit.count(); i++)
            DrawFighter(i, white, green, false);
    }

}
//-------------------------------

void DrawArena(bool outline){
    //--------Pass I-------------
    if (Sys.useShadows){
        if (Sys.hasFBO)
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(LightProjectionMatrix);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixf(LightViewMatrix);
        glViewport (0, 0, ShadowMapSize, ShadowMapSize);

        glClear( GL_DEPTH_BUFFER_BIT);

        glCullFace(GL_FRONT);
        glShadeModel(GL_FLAT);
        glColorMask(0, 0, 0, 0);

        glPushMatrix();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        //-scene----------
        DrawArenaModels(true);
        //---------------
        glActiveTexture(GL_TEXTURE1);
        glPopMatrix();

        if (Sys.hasFBO)
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
        else{
            glBindTexture(GL_TEXTURE_2D, ShadowMapTexture);
            glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
                                ShadowMapSize, ShadowMapSize);
        }
        glCullFace(GL_BACK);
        glShadeModel(GL_SMOOTH);
        glColorMask(1, 1, 1, 1);
    }//use shadows
    //Pass II-------------------------------------
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport (0, 0, Sys.width, Sys.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    glLoadMatrixf(CameraProjectionMatrix);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    float m2[16];
    Vector3D target = Cam3D.pos() + Cam3D.dir();
    MatrixLookAt(Cam3D.pos(), target,  Cam3D.up(), m2);
    glMultMatrixf(m2);
    glPushMatrix();

    if (Sys.hasGLSL)
        GLSLprog[1].use(); //tex'n'color
    glActiveTexture(GL_TEXTURE0);
    Sky.draw(Pics);
    if (Sys.hasGLSL)
        glUseProgram(0);

    //-----------
    glEnable(GL_LIGHTING);
    Arena.setLightning();
    glShadeModel(GL_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, 0);
    //---
    glActiveTexture(GL_TEXTURE0);

    if (Sys.useShadows)
        DrawArenaModels(true);
    else
        DrawArenaModels(false);
    //---
    glPopMatrix();
    if (Sys.useShadows){
        glActiveTexture(GL_TEXTURE1);
        //----Pass III---------------------
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, ShadowMapTexture);
        glEnable(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, 
                        GL_COMPARE_R_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, 
                        GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, 
                        GL_INTENSITY);

        if (!Sys.hasGLSL){

            const double x[] = {1.0, 0.0, 0.0, 0.0};  
            const double y[] = {0.0, 1.0, 0.0, 0.0};  
            const double z[] = {0.0, 0.0, 1.0, 0.0};
            const double w[] = {0.0, 0.0, 0.0, 1.0};

            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_GEN_R);
            glEnable(GL_TEXTURE_GEN_Q);

            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
            glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
            glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

            glTexGendv(GL_S, GL_EYE_PLANE, x);
            glTexGendv(GL_T, GL_EYE_PLANE, y);
            glTexGendv(GL_R, GL_EYE_PLANE, z);
            glTexGendv(GL_Q, GL_EYE_PLANE, w);
        }
        else{

            GLSLprog[3].use();
            int loc = GLSLprog[3].getUniformID("tex");
            glUniform1i(loc, 0);
            loc = GLSLprog[3].getUniformID("ShadowMap");
            glUniform1i(loc, 1);
        }

        glMatrixMode(GL_TEXTURE);
        glLoadMatrixd(biasMatrix);
        glMultMatrixf(LightProjectionMatrix);
        glMultMatrixf(LightViewMatrix);
        glMatrixMode(GL_MODELVIEW);

        glAlphaFunc(GL_GEQUAL, 0.99f);
        glEnable(GL_ALPHA_TEST);
        //--------
        glActiveTexture(GL_TEXTURE0);
        DrawArenaModels();
        //---------
        glActiveTexture(GL_TEXTURE1);

        if (!Sys.hasGLSL){
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_R);
            glDisable(GL_TEXTURE_GEN_Q);
        }
        else{
            glUseProgram(0);
        }

        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
    }//---useShadows
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glPopMatrix();
    //--------------------------

    glPopMatrix();
    glPushMatrix();

    if (outline){
        Arena.draw(0);
        COLOR tmp;
        for (unsigned i = 0; i < Entit.count(); i++)
            DrawFighter(i, tmp, tmp, true);

    }
    glPopMatrix();

    glDisable(GL_LIGHTING);

    if (Combatselect.active()){
        glColor4f(1,0,0,1);
        if (Sys.hasGLSL)
            GLSLprog[0].use();
        Combatselect.draw3DPointer(Pointer3D, 
                                   true); //pointer
    }

    if (Sys.showDebugText){
        DrawCross(LightPos);
        DrawCross(LightTo);
    }

    glEnable(GL_TEXTURE_2D);

    if (Sys.hasGLSL)
        GLSLprog[1].use(); //tex'n'color
    PS.draw(Cam3D); //particles yay

    glPopMatrix();
}
//-------------------------------------------------------------
void DrawMiniMap(){
    float tilesize = 4.0f;

    int x = (Sys.width - map.width()*tilesize) - tilesize;
    int y = (Sys.height - map.height()*tilesize) - tilesize;

    map.drawMiniMap(x, y, Pics, tilesize);
    COLOR c = COLOR(0.0f, 0.0f, 1.0f, 0.8f);
    COLOR r = COLOR(1.0f, 0.0f, 0.0f, 0.8f);
    for (unsigned man = 0; man < npcnation.count(); man++){

        Npc * dud = 0;
        dud = npcnation.getNpc(man);
        Pics.draw(-1,
               (int)(dud->x()/32) * tilesize + x,
               (int)(dud->y()/32) * tilesize + y,
                0,
                false,
                tilesize,
                tilesize, 
                0.0f, c, c);
    }
    
    for (unsigned ent = 0; ent < Events.entries.count(); ent++){
        Pics.draw(-1, 
                  (int)(Events.entries[ent].x) * tilesize + x,
                  (int)(Events.entries[ent].y) * tilesize + y,
                   0,
                   false,
                   tilesize,
                   tilesize,
                   0.0f, r,r);

    }


}


//--------------------------------------------------------------
void Draw2DStuff(){

    if (SYSTEM_filesloaded){

        int fontas  =  Pics.findByName("charai.tga");
        int rodykle =  Pics.findByName("rod.tga");
        int remelis =  Pics.findByName("frm.tga");
        int icons   =  Pics.findByName("icons.tga");



        //draw 2d map
        if ((GameState == WALKING) || (GameState == BEGIN_COMBAT)){

            if (!IsCurrentMap3D)
                DrawMap(mapposx, mapposy);
            else{
                DrawMiniMap();
            }

        }

        //Draw main menu & screen bg
        if ((GameState == TITLEMENU) || (GameState == CREATEPARTY))
            DrawIntroScreen();
        //--
        if ((GameState == TITLEMENU) && (!Mainmenu.selected)
            && (Mainmenu.active()))
            Mainmenu.draw(Pics, remelis, rodykle,fontas);

        if (CreditWindow.active())
            CreditWindow.draw(Pics, remelis, fontas);

        if (NewGameMenu.active())
            NewGameMenu.draw(Pics, remelis, rodykle, fontas);

        if (OptionMenu.active())
            OptionMenu.draw(Pics, remelis, rodykle, fontas);


        //draw party creation stuff---
        if (GameState == CREATEPARTY){

            if (hMaker.dicesRolled)
                CreatureInfo(220, 200, *jparty.member(hMaker.heroIndex),
                             Rases, itemData, klases, ML.data[8],
                             remelis, fontas, Pics);

            if (CreateHeroMenu.active())
                CreateHeroMenu.draw( Pics,remelis,rodykle,fontas);

            if (CreateHeroMenu.selected){

                if (sexsel.active())
                    sexsel.draw(Pics, remelis,rodykle,fontas);

                Creature * dude = jparty.member(hMaker.heroIndex);

                switch(CreateHeroMenu.state){
                    case 0: racesel.draw(Pics, remelis,
                                         rodykle, fontas);
                            break;
                    case 2: classsel.draw(Pics, remelis,
                                          rodykle, fontas);
                            break;
                    case 3: NameEnter.draw(Pics, fontas,
                                               remelis);
                            break;
                    case 5:{
                            unsigned picindex = Pics.findByName(dude->faceTileSet);

                            SelectNewFace.draw(Pics, remelis,
                                               rodykle, fontas, 
                                               picindex);
                    }
                }
            }
            }//CreateParty
            //-----------------------------
            if (GameState != COMBAT){

                if ((GameState != COMBAT_INTRO) 
                    && (GameState != TITLEMENU)){

                    if (Console.active())
                        Console.draw(Pics, remelis,fontas, rodykle);
                }

                if (InnPartyMeniu.active())
                    InnPartyMeniu.draw(Pics, remelis,rodykle,fontas);


                if (ItemCountedit.active())
                    ItemCountedit.draw(Pics,fontas,remelis);

                //---
                //draws hero faces
                if (orderselect.active()){
                    PartyStatus(0, Sys.height - 100, jparty,false,false);

                    if (!orderselect.selected)
                        orderselect.draw(Pics, remelis,rodykle,fontas);

                        else {
                            //rodoma infa apie heroju
                            if (orderselect.state == 3)
                                CreatureInfo(10,80, 
                                             *jparty.member(partyfaces.face),
                                             Rases, itemData, klases,
                                             ML.data[8],
                                             remelis,fontas, Pics);

                            if (orderselect.state == 4)//options meniu
                                optmeniu.draw(Pics, remelis,rodykle,fontas);

                            if (invsel.active())
                                invsel.draw(Pics, remelis, rodykle, fontas);

                            if (EquipBody.active())
                                EquipBody.draw(Pics, remelis,rodykle,fontas);

                        }

                    }
                    //---
                    //piesia komandine eilute
                    if (code_enter)
                        CodeEdit.draw(Pics,fontas,remelis);

                    //rodome conversation langa jei kas nors ka sako
                    if ((conversation.active())&&(GameState == WALKING))  {
                        conversation.draw(Pics,remelis,fontas,rodykle);

                        Talker.poisoned = false;
                        Talker.dead = false;
                        Talker.draw(Pics, 
                                    Pics.findByName("status.tga"),
                                    Pics.findByName("mark.tga"),
                                    fontas, false);
                        char buf[255];
                        sprintf(buf,"%s:", Talker.name);
                        WriteText(conversation.getX()+10,
                                  conversation.getY()+12,
                                  Pics, 
                                  Pics.findByName("charai.tga"), buf);
                    }

                    //-------------------
                    //kai uzkalbini innkeeperi piesia meniu
                    if ((ENCmersh)||(ENCinn)||(ENCsmith)){

                        PartyStatus(0, Sys.height - 100, jparty,false,false);

                        if (InnMenu.active())
                            InnMenu.draw(Pics, remelis, rodykle, fontas);

                        if (merchantmeniu.active())
                            merchantmeniu.draw(Pics, remelis,
                                               rodykle, fontas);
                        if (SmithMeniu.active())
                            SmithMeniu.draw(Pics, remelis, rodykle, fontas);


                if (MerchantItems.active())
                    MerchantItems.draw(Pics, remelis,
                                       rodykle, fontas);

            } //((ENCmersh)||(ENCinn)||(ENCsmith)) --------------------

            if (partyfaces.active()){
                partyfaces.draw(Pics,
                                Pics.findByName("faceselector.tga"));
            }

        } //(GameState != COMBAT)
        else { 

            //TODO: Perkelti šita į atskira funkciją
            FightersStatus(Sys.width - 220, 5, eparty, visible_enemy,
                           Pics,
                           Pics.findByName("frm.tga"),
                           fontas, icons);

            FightersStatus(5, 5, jparty,true,
                           Pics,
                           Pics.findByName("frm.tga"),
                           fontas, icons);

            if (Combatselect.active())
                Combatselect.drawFlatPointer(Pics,
                                             Pics.findByName("handy.tga"));

            //console
            Console.draw(Pics, remelis,fontas, rodykle);

            if (ActionSelect.active())
                ActionSelect.draw(Pics, remelis,rodykle,fontas);

        } //-----------------

        if (StatGain.active())
            StatGain.draw(jparty, Pics, fontas, remelis);

        if (ItemGain.active())
            ItemGain.draw(jparty, Pics, fontas, remelis);

        if (ItemMenu.active()){
            ItemMenu.draw(Pics, remelis, rodykle, fontas, icons);
        }

        if ((SpellMenu.active()))
            SpellMenu.draw(Pics, remelis,rodykle,fontas);

        if (QuitMenu.active())
            QuitMenu.draw(Pics, remelis, rodykle, fontas);

        if (ModMenu.active())
            ModMenu.draw(Pics, remelis, rodykle, fontas);
        if (Savemenu.active())
            Savemenu.draw(Pics, remelis, rodykle, fontas);

        if (Board.active())
            Board.draw(Pics, remelis, rodykle, fontas);

        if (gameover)
            GameOver();

        if (Sys.showDebugText)
            DrawSomeText(fontas);


        ShaderProgram* jc = 0;
        ShaderProgram* tc = 0;
        if (Sys.hasGLSL){
            jc = &GLSLprog[0]; //color
            tc = &GLSLprog[1]; //tex'n'color
        } 

        Pics.draw(icons, 
                  _MouseX,
                  _MouseY,
                  1);


        Pics.drawBatch(jc, tc, 666);


       /* glActiveTexture(GL_TEXTURE0);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, ShadowMapTexture);

        COLOR c = COLOR(1,1,1,1);
        glBegin(GL_QUADS);

        glColor4fv(c.c);
        glTexCoord2f ( 0, 0 );
        glVertex2f ( 0, 0 );

        glColor4fv(c.c);
        glTexCoord2f ( 1, 0 );
        glVertex2f ( 256,0  );

        glColor4fv(c.c);
        glTexCoord2f ( 1, 1 );
        glVertex2f ( 256, 256 );

        glColor4fv(c.c);
        glTexCoord2f ( 0,1 );
        glVertex2f ( 0, 256 );

        glEnd();
        glActiveTexture(GL_TEXTURE0);*/

    }
    else{ // if images not loaded
        //TODO: Draw some "Loading" stuff
    }
}


//--------------------------------------------------------------------------
//  Renders stuff in the screen
void RenderScreen(){

    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    //------------------------
    // 3D mode
    //---------------------
    if ((GameState == COMBAT_INTRO)||(GameState == COMBAT)
        ||(GameState == VICTORY)||(GameState == COMBAT_STATS))

        DrawArena(Sys.drawOutlines);

    if (((GameState == WALKING)||
            (GameState == BEGIN_COMBAT)) && (IsCurrentMap3D))

        DrawMap3D();
    //--------------
    //GO TO 2D mode
    //----------------
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0, Sys.width, Sys.height, 0, -100.0, 100.0);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity ();
    //-----------------------------------
    // 2D mode
    //-----------------------------------
    Draw2DStuff();
    //------------------------------back to 3d mode
    glEnable(GL_DEPTH_TEST);
    //-------------------------------

    glFlush();
    SDL_GL_SwapBuffers( );
}
//-----------------------------------------
void QuitOpenAL(){
    SS.freeData();
    SS.exit();
}
//-----------------------------------
void Quit(){
    GameLog.write("Quiting...");
    if (Sys.useSound){
        GameLog.write("Releasing stream...");
        Stream.release();
        GameLog.write("Quiting OpenAl...");
        QuitOpenAL();
    }

    GameLog.write("Destroying pics...");
    Pics.destroy();
    Tailsetas.destroy();
    Arena.destroy();
    HeroActions.destroy();
    EnemyActions.destroy();
    Entit.destroy();

    EmptyChests.destroy();

    CreditWindow.reset();

    for (unsigned long i = 0; i < ScriptStack.count(); i++ ){

        ScriptStack[i].clear();
        ActiveScript = -1;
    }

    ScriptStack.destroy();

    Kintamieji.destroy();
    map.destroy();
    Events.destroy();
    MonsterGroups.destroy();
    NewGameMenu.destroy();
    npcnation.destroy();
    itemData.destroy();
    Spells.destroy();
    Combatselect.destroy();
    Savemenu.destroy();
    ModMenu.destroy();
    OptionMenu.destroy();

    Rases.destroy();
    klases.destroy();

    GameLog.write("Destroying parties...");

    jparty.destroy();
    GameLog.write("heroes party freed...");
    innparty.destroy();
    eparty.destroy();

    ML.destroy();
    PML.destroy();
    GameLog.write("Destroying models...");

    Pointer3D.destroy();
    DummyFighter.destroy();

    GameLog.write("Destroying menus");

    ActionSelect.destroy();

    InnMenu.destroy();
    SmithMeniu.destroy();
    merchantmeniu.destroy();

    InnPartyMeniu.destroy();
    MerchantItems.destroy();
    orderselect.destroy();
    invsel.destroy();
    EquipBody.destroy();
    optmeniu.destroy();
    SpellMenu.destroy();
    QuitMenu.destroy();
    Mainmenu.destroy();
    CreateHeroMenu.destroy();
    racesel.destroy();
    sexsel.destroy();
    classsel.destroy();
    SelectNewFace.destroy();
    ItemMenu.destroy();
    StatGain.destroy();
    ItemGain.destroy();

    Console.destroy();
    conversation.destroy();
    Funcs.destroy();
    Merch.destroy();

    Tileset3D.destroy();

    for (unsigned i = 0; i < GLSLprog.count(); i++)
        GLSLprog[i].destroy();
    GLSLprog.destroy();

    DestroyShadowMap(&ShadowMapTexture, Sys.hasFBO, &fboId);

    GameLog.write("Bye Bye");

    SDL_FreeSurface(WindowIcon);
    SDL_Quit();
}
//---------------------------------
static void process_events( void ){

    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
        case SDL_KEYDOWN:

            globalKEY = (char)event.key.keysym.unicode;
            switch( event.key.keysym.sym ) {

                case SDLK_PAUSE:
                    Sys.gamePaused =! Sys.gamePaused;
                    if (Sys.gamePaused){
                        SDL_ShowCursor(1);
                        SDL_WM_GrabInput(SDL_GRAB_OFF);
                    }
                    else{
                        SDL_ShowCursor(0);
                        SDL_WM_GrabInput(SDL_GRAB_ON);
                    }
                break;
                case SDLK_F1:{
                    Sys.showDebugText = !Sys.showDebugText;
                } break;
                case SDLK_F2:{
                    ShowEntries = !ShowEntries;
                } break;
                case SDLK_F3:{
                    IsCurrentMap3D = !IsCurrentMap3D;
                    map.alloc3DMap();
                } break;


                default:{}

            }
            break;
            case SDL_QUIT:{
                QuitApp();
            }
        }
    }
}

//-----------------------------------------
static void SetupOpengl( int width, int height ){

    const GLubyte* vendor=glGetString(GL_VENDOR);
    const GLubyte* renderer=glGetString(GL_RENDERER);
    const GLubyte* version=glGetString(GL_VERSION);
    const GLubyte* ext=glGetString(GL_EXTENSIONS);

    GameLog.write((const char*)vendor);
    GameLog.write((const char*)renderer);
    GameLog.write((const char*)version);
    GameLog.write((const char*)ext);

    if ((strstr((const char*)ext, "GL_ARB_shader_objects"))&&
        (strstr((const char*)ext, "GL_ARB_shading_language_100"))&&
        (strstr((const char*)ext, "GL_ARB_vertex_shader"))&&
        (strstr((const char*)ext, "GL_ARB_fragment_shader"))){

        Sys.hasGLSL = true;
        GameLog.write("GLSL shaders available!");
    }
    else{
        GameLog.write("No GLSL available!");
        Sys.hasGLSL = false;
    }

    if (Sys.disableGLSL)
        Sys.hasGLSL = false;
    
    if (strstr((const char*)ext, "GL_EXT_framebuffer_object")){
        Sys.hasFBO = true;
        GameLog.write("FBO available");
    }


    glClearDepth( 1.0 );
    glViewport (0, 0, (GLsizei) Sys.width, (GLsizei) Sys.height);
    glClearColor (0, 0, 0, 1.0);

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    //glPolygonMode (GL_FRONT, GL_FILL);
    glFrontFace(GL_CW);   //front face is clock wise
    glCullFace (GL_BACK); //do not draw back faces
    glEnable (GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


    GLfloat aspect = (GLfloat) Sys.width / Sys.height;


    LightPos = Vector3D(-5.0f, 10.0f , -20.0f);
    LightTo = Vector3D(5.0f, 2.0f, 10.0f);

    float fov = 60.0f;


    FrustumRadius = CalcFrustumSphereRadius(1.0f, 100.0f, fov);

    MatrixPerspective(fov, aspect, 1.0f, 500.0f, CameraProjectionMatrix);
    MatrixPerspective(fov, 1.0, 1.0f, 100.0f, LightProjectionMatrix);

    /*glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, ShadowMapSize, ShadowMapSize, 0, -100.0, 100.0);
    //glOrtho(0, ShadowMapSize, 0, ShadowMapSize, 1.0f, 100);
    glGetFloatv(GL_MODELVIEW_MATRIX, LightProjectionMatrix);

    glPopMatrix();

    LightPos = Vector3D(1,1,-1);
    LightTo = Vector3D(0,0,0);*/

    MatrixLookAt(LightPos, LightTo,  Vector3D(0,1,0), LightViewMatrix);

    if (Sys.useShadows)
        PrepareShadowMap(ShadowMapSize, &ShadowMapTexture,
                         Sys.hasFBO, &fboId);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_RESCALE_NORMAL);

    Cam3D.setpos(Vector3D(0.0f, 6.5f, -11.0f));
    Cam3D.setdir(Vector3D(0.0f, 0.0f, 1.0f));
    Cam3D.setright(Vector3D(1.0f, 0.0f, 0.0f));
    Cam3D.setup(Vector3D(0.0f, 1.0f, 0.0f));
    Cam3D.pitch(-0.500f);
}
//--------------------------------
void InitOpenAL(){

    bool res = false;
    /*if (strlen(Sys.customSoundDevice))
        res = SS.init(Sys.customSoundDevice);
    else*/
        res = SS.init(NULL);

    if (res){
        Vector3D p(0,0,0);
        SS.setupListener(p.v, p.v);

        SS.loadFiles(Sys.basePath, "sfx/list.txt" );
    }
    else
        Sys.useSound = false;
}

//---------------------------------------------
    void CameraFoo(){
        if (keys[8]) Cam3D.strife(0.5f);
        if (keys[10]) Cam3D.strife(-0.5f);
        if (keys[7]) Cam3D.move(0.5f);
        if (keys[9]) Cam3D.move(-0.5f);

        if (MouseX!=0)
            Cam3D.yaw(3.14f/(-1024.0f/MouseX), false);

        if (MouseY!=0)
            Cam3D.pitch(3.14f/(-1024.0f/MouseY));
    }
//-------------------------------------
void    LoadShaders(const char* list){

    FILE * f = 0;
    f = fopen(list, "rt");
    if (!f)
        return;
    while(!feof(f)){
        Shader frag;
        Shader vert;

        ShaderProgram np;
        GLSLprog.add(np);

        GLSLprog[GLSLprog.count() - 1].create();

        char buf[255];

        if (!fscanf(f, "%s\n", buf))
            return;
        vert.load(GL_VERTEX_SHADER, buf);
        if (!fscanf(f, "%s\n", buf))
            return;
        frag.load(GL_FRAGMENT_SHADER, buf);

        GLSLprog[GLSLprog.count() - 1].attach(vert);
        GLSLprog[GLSLprog.count() - 1].attach(frag);
        GLSLprog[GLSLprog.count() - 1].link();

        char error[1024];
        GLSLprog[GLSLprog.count() - 1].getLog(error, 1024);
        GameLog.write(error);
    }

}
//-------------------------------------------
void InitSdlWindow(const char * windowTitle, const char* iconPath){
    const SDL_VideoInfo* info = NULL;

    int bpp = 0;
    // Flags we will pass into SDL_SetVideoMode.
    int flags = 0;

    // First, initialize SDL's video subsystem.
    GameLog.write("Trying SDL_INIT...");
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {

        GameLog.write("Video initialization failed!");
        QuitApp();
    }
    GameLog.write("SDL_INIT succsess!");

    // Let's get some video information.
    info = SDL_GetVideoInfo( );

    if( !info ) {
        GameLog.write("Video query failed !");
        QuitApp();
    }

    bpp = info->vfmt->BitsPerPixel;

    int rSize, gSize, bSize, aSize;
    rSize = gSize = bSize = aSize = 8;
    int buffsize = ((rSize + gSize + bSize + aSize + 7) & 0xf8);

    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, rSize );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, gSize );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, bSize );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, aSize );
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, buffsize );

    flags = SDL_OPENGL|SDL_DOUBLEBUF;

    if ( info->hw_available)
        flags = flags|SDL_HWSURFACE;
    else
        flags = flags|SDL_SWSURFACE;

    if ( info->blit_hw )
        flags = flags |SDL_HWACCEL;

    if (!Sys.isWindowed)
        flags = flags |SDL_FULLSCREEN;

    SDL_WM_SetCaption(windowTitle, 0);
    WindowIcon = SDL_LoadBMP(iconPath);
    if (WindowIcon){
        SDL_SetColorKey(WindowIcon, SDL_SRCCOLORKEY,
                        SDL_MapRGB(WindowIcon->format, 255, 255, 255));
        SDL_WM_SetIcon(WindowIcon, 0);
    }

    if( SDL_SetVideoMode( Sys.width, Sys.height, bpp, flags ) == 0 ) {

        GameLog.write("Video mode set failed!");
        QuitApp();
    }

}

//--------------------------------------------
void Init(){
    char path[214];
    puts("Systems online!");

    srand(time(0));
    MakeFunctionArray();

    GetHomePath(HPath);
    //makes main settings dir
    sprintf(path, "%s.godrpg", HPath);
    MakeDir(path);

    sprintf(path, "%s.godrpg/gamelog.log", HPath);
    GameLog.create(path);

    GameLog.write("Loading config");
    sprintf(path, "%s.godrpg/conf.xml", HPath);
    if (Sys.load(path))
        GameLog.write("done!");
    else{
        GameLog.write("failed. using defaults");
        Sys.write(path);
    }
    Options_Width = Sys.width;
    Options_Heigt = Sys.height;

    //creates dir for savegames
    sprintf(path, "%s.godrpg/%s", HPath, Sys.basePath);
    path[strlen(path)-1] = 0;
    MakeDir(path);
    sprintf(path, "%s.godrpg/%ssaves/", HPath, Sys.basePath);
    path[strlen(path)-1] = 0;
    MakeDir(path);

    sprintf(path, "%sgodrpg.bmp", Sys.basePath);
    char title[255];
    sprintf(title,"Godrpg ver.%s", Version);
    InitSdlWindow(title, path);
    LoadExtensions();

    if (glGetInfoLogARB)
        puts("Extensions OK");
    else
        puts("Extensions FAIL");
    if ( Sys.useSound ){
        GameLog.write("Initializing OpenAL...");
            InitOpenAL();
            GameLog.write("done!");
    }
        
    if (Sys.useGamepad){
        GameLog.write("Init joystick");
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        if(SDL_NumJoysticks() > 0){
            Joy = SDL_JoystickOpen(0);
        }
    }

    SDL_EnableUNICODE(SDL_ENABLE);
    SDL_ShowCursor(0);
    //SDL_WM_GrabInput(SDL_GRAB_ON);
    SetupOpengl(Sys.width, Sys.height );

    Sky.build(36.0f);

    sprintf(path, "%smodels/rodykle.rm2", Sys.basePath);
    if (!Pointer3D.load(path)){
        Pointer3D.load("base/models/rodykle.rm2");
    }

    sprintf(path, "%smodels/minifighter.rm2", Sys.basePath);
    if (!DummyFighter.load(path)){
        DummyFighter.load("base/models/minifighter.rm2");
    }
    //-----------load shaders--------------------
    if (Sys.hasGLSL){
        LoadShaders("base/shaders/list.txt");
    }
    //-----------------------------------
    if ( Sys.useSound ){
        if (OpenMusic(MainMenuMusic))
            Stream.playback();
    }

    GameLog.write("Let's load some stuff");
    OnBegin();
    GameLog.write("Entering game loop");
    Mainmenu.activate();

}


//=============================================
int main( int argc, char* argv[] ){
#ifdef __APPLE__
   CFBundleRef mainBundle = CFBundleGetMainBundle();
   CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
   char path[PATH_MAX];
   if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
    // error!
   }
   CFRelease(resourcesURL);
   chdir(path);
#endif

    while (Sys.reloadApp){
        Sys.reloadApp = false;
        Sys.quitApp = false;
        //initialize everything
        Init();

        unsigned long sumLogicTicks = 0;
        unsigned long countLogicTicks = 0;
        unsigned long sumVideoTicks = 0;
        unsigned long countVideoTicks = 0;

        unsigned timeInterval = 14;

        while(!Sys.quitApp) { //game loop

            if (!Sys.limitFPS){
                unsigned long beforeVideo = SDL_GetTicks();
                RenderScreen();
                unsigned long afterVideo = SDL_GetTicks();
                sumVideoTicks += (afterVideo - beforeVideo);
                countVideoTicks++;
            }

            static unsigned long tick = SDL_GetTicks();

            CheckKeys();

            if (keys[0]) globalKEY = SDLK_UP;
            if (keys[1]) globalKEY = SDLK_DOWN;
            if (keys[2]) globalKEY = SDLK_LEFT;
            if (keys[3]) globalKEY = SDLK_RIGHT;
            if (keys[4]) globalKEY = SDLK_RETURN;
            if (keys[5]) globalKEY = SDLK_ESCAPE;
            if (keys[6]) globalKEY = SDLK_SPACE;

            if ((SDL_GetTicks() > tick)&&(!Sys.gamePaused)){

                unsigned long beforeLogic = SDL_GetTicks();
                if (Sys.showDebugText)
                    CameraFoo();
                OnTimer();
                unsigned long afterLogic = SDL_GetTicks();
                sumLogicTicks += (afterLogic - beforeLogic);
                countLogicTicks++;

                //tries to guess needed time for drawing and logic---------
                unsigned long timeNeeded = 0;
                timeNeeded += (unsigned long)((sumLogicTicks*1.0f)/
                                              (countLogicTicks*1.0f));
                timeNeeded += (unsigned long)((sumVideoTicks*1.0f)/
                                              (countVideoTicks*1.0f));
                if (timeNeeded > timeInterval)
                    timeNeeded = timeInterval;
                //---------------------------------------------------------
                timeNeeded = 0;  //no guessing
                tick = SDL_GetTicks() + (timeInterval - timeNeeded);

                if (Sys.useSound){
                    if (Stream.playing())
                        Stream.update();
                }

                if (Sys.limitFPS){
                    unsigned long beforeVideo = SDL_GetTicks();
                    RenderScreen();
                    unsigned long afterVideo = SDL_GetTicks();
                    sumVideoTicks += (afterVideo - beforeVideo);
                    countVideoTicks++;

                }
            }
            else{
                if (Sys.limitFPS)
                    SDL_Delay(1);
            }

            if (shut_down)  QuitApp();

            process_events();
        }
        Quit();


        printf("Average duration of logic frame: %2.4f\n", 
            (sumLogicTicks*1.0f)/(countLogicTicks*1.0f));
        printf("Average duration of video frame: %2.4f\n", 
            (sumVideoTicks*1.0f)/(countVideoTicks*1.0f));
    } //while reboot
    //visual studio memleak dump
    #ifdef WIN32
        #ifdef _DEBUG
            _CrtDumpMemoryLeaks();
        #endif
    #endif

    return 0;
}
