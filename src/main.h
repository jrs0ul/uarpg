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
    #ifdef _MSC_VER
        #include <SDL.h>
        #include <SDL_opengl.h>
        #include <alc.h>
    #else
        #include <SDL/SDL.h>
        #include <AL/alc.h>
        #include <SDL/SDL_opengl.h>
    #endif
#else
  #include <SDL/SDL.h>
  #ifdef __APPLE__
      #include <OpenAL/alc.h>
  #else
      #include <AL/alc.h>
  #endif
  #include <SDL/SDL_opengl.h>
  #include <dirent.h>
  #include <sys/stat.h>
  #include <sys/types.h>
#endif



#include "Extensions.h"

#include <cstdio>
#include <ctime>


#include "Shader.h"
#include "ShaderProgram.h"

#include "Image.h"
#include "TextureLoader.h"

#include "Utils.h"

#include "audio/SoundSystem.h"

#include "audio/OggStream.h"
#include "Colors.h"


#include "interpreter.h"
#include "Klasses.h"
#include "Races.h"
#include "Creature.h"
#include "Worldmap.h"
#include "Inventory.h"
#include "SpellData.h"
#include "Party.h"
#include "npc.h"
#include "savegame.h"
#include "Combat.h"
#include "variables.h"
#include "MonsterGroups.h"
#include "CCode.h"
#include "models/ModelCollection.h"
#include "Camera.h"
#include "particles/ParticleSystems.h"


#include "gui/MenuLoader.h"
#include "gui/Gui.h"
#include "gui/Coolframe.h"
#include "gui/Text.h"
#include "gui/BatlleSelect.h"
#include "gui/TextScreen.h"
#include "gui/CreatureStatus.h"
#include "gui/SelectMenu.h"
#include "gui/StatGain.h"
#include "gui/ItemGain.h"
#include "gui/EditBox.h"
#include "gui/OnScreenBoard.h"
#include "gui/Credits.h"
#include "SystemConfig.h"
#include "EventArray.h"
#include "PlayerGfxList.h"
#include "HeroMaker.h"
#include "Skybox.h"
#include "Arena.h"
#include "Merchant.h"
#include "Log.h"
#include "ShadowMap.h"

#include "DebugProps.h"



#ifdef WIN32
    #ifdef _DEBUG
    #include <crtdbg.h>//for mem leak detection in MS VC
    #endif
#endif



//======================================
    struct Camera{
        int x;
        int y;
        Camera(){
            x=0; y=0;
        }
    };


    enum GameStates {TITLEMENU,
                     CREATEPARTY,
                     WALKING,
                     COMBAT,
                     BEGIN_COMBAT,
                     COMBAT_INTRO,
                     VICTORY,
                     COMBAT_STATS};

    typedef void (*ScriptFuncPtr)(Arguments &);

    struct ScriptFunction{
        ScriptFuncPtr p;
        char name[50];

        void set(ScriptFuncPtr f, const char * n){
             p = f;
             strncpy(name, n, 50);
        }
    };

    struct Functions{
        DArray<ScriptFunction> func;
        void add(ScriptFuncPtr f, const char * n){
            ScriptFunction t;
            t.set(f, n);
            func.add(t);
        }
        void destroy(){func.destroy();}
    };


//===================================GLOBALS
//makes array of script funtions
void    MakeFunctionArray();
void    ResetGlobals();                               //resets global variables
//focuses cam on point x,y in map grid
void    Focus3DCam(float x, float y, 
                   float height = 20.0f, //above ground
                   float distance = 20.0f, //from object
                   float pitch = -0.77f,
                   float tilsize = 8.0f
                   );
void    MakeNextEncounterSteps();                     //make number of steps 'till next enemy encounter
void    QuitApp();                                    //quits app
void    Interpret(const char* line);                  //Interprets script line
void    MoveMap(int x, int y, int charx, int chary);  //TODO: write something
void    CenterMap(int x,int y);                       //Centers map in the screen acording x and y
void    DrawMap3D();
void    DrawMap(float x, float y);                    //Draws whole map; X,Y is map's higher left corner coords
bool    LoadObjects(const char* s);                   //Load some usefull info, like races, items, etc.
void    PartyStatus(int x, int y,                     //*Draws party faces: x,y - coordinates
                    Party& partis,                    //*Party class
                    bool enemy, bool vertical);       //*is enemy ?, draw verticaly ?
void    EnemyGenerate( Party& part,                   // Generates enemy party
                       MonsterGroup& galimipriesai);
int     Attack( unsigned myNumber,                    //Fighter atacks another figher
                unsigned oponentNumber,               //mytype = 1  ---> hero party
                unsigned myType, unsigned oponentType);
void    PoisonEffect( Party& partis, unsigned index); //Poison effect on selected member
void    ScanCreature(Creature& crtr);                 //Scan creature with scan spell
void    End_combat();                                 //Ends combat
void    CastSpell(int spellindex,                     //Casts spell
               Party& casterparty,
               int casterindex,
               Party& oponentparty,
               int oponentindex);
void    UseItem( Creature& crtr,                     //Uses item
                 unsigned itemID,
                 Party& partis, unsigned index);
//Enemy fighter sends message to his comrades
void    SendCall(unsigned itemID,
                 const char* textas, unsigned kas);  
//Enemy fighter trys to help sender
void    HelpCaller(unsigned z, unsigned name,
                   unsigned & caller);
int     PickVictim(bool hp);                         //Enemy picks victim to atack
void    EnemyAI(unsigned index);                        //Enemy AI
void    CreateItemsMenu(unsigned x, unsigned y,      //Creates characters inventory menu
                          SelectMenu& ItemsMeniu,
                          Creature* dude);
void    CreateSpellMenu( SelectMenu& selectmeniu,    //Creates spell menu
                         Creature* dude,
                         bool combat);
void    CreateActionMenu(unsigned kas,               //Creates combat action menu for hero kas
                         SelectMenu& selMeniu);
void CreateSaveStateMenu();
void CreateModMenu();
//Handles hero action menu in combat
void    HeroAction(unsigned kas, bool& baigta);
void    S_Atck(unsigned kas, bool& baigta);
void    S_Cast(unsigned char kas, bool& baigta);
void    S_Use(unsigned char kas, bool& baigta);
//'Run' selected
void    S_Escape(unsigned kas, bool& over);
//Equips weapon or arrmor
void    EquipItem(unsigned sav,
                  unsigned itemmeniuindex,
                  ItemSlots slot);

void    M_UnequipItems();
void    M_Options_Ingame();
void    M_Save();
void    M_NewGame();
void    M_MakeHero();
void    M_UseDefaultHero();
void    M_Onward();
void    M_Options();
void    M_Mods();
void    M_Credits();
//Unequips weapon or arrmor
void    UnequipItem(unsigned who, ItemSlots slot);
//Handles inventory menu while adventuring
void    HandleInventory();
void    Inventory_Use();
void    Inventory_Give();
void    Inventory_Throw();

void    ExitYesNo();
void    TakeLoot();                                 //Takes all stuff from defeated enemies
void    OnVictory();
void    SharedItems(Creature& crtr);                //Generated creature gets basic items
void    ApplyOpenedChests();                        //chenges map tiles acording where opened boxes are
//Load combat arena
bool LoadArena(const char * path);
//Loads map 'name', if it's different than CurrentMap
bool    LoadCurrentMap(float startx, float starty,
                       const char* name, bool loadNpcs = true);           
//Loads saved game
void    Onward(const char * path);                                  
void    BuyItem (unsigned kind,  bool& stop,        //if kind  2 - weapon, if 1 - item
                 SelectMenu& parent);
void    SellItem(bool& stop, SelectMenu& parent);   //sells items
void    BuildInnPartyMeniu();                       //Creates menu of heroes names who are in the inn
void    TheInn();                                   //Handles inn menu input
void    SmithShop();
void    Merchantshop();
//Party meets some enemies, index - monster group index
void    Encounter(unsigned index, const char* combatTune);
void    LookForEnemy();
void    CheckVictory();

void    CMD_MVCAM(Arguments& args);
void    CMD_PUSHCAM(Arguments& args);
void    CMD_MVNPC(Arguments& args);
void    CMD_PSNPC(Arguments& args);
void    CMD_DRAINHP(Arguments& args);              //DrainHP(caster,oponent,power)
void    CMD_RESTOREHP(Arguments& args);
void    CMD_RESTOREMP(Arguments& args);
void    CMD_SCAN(Arguments& args);
void    CMD_RESURECT(Arguments& args);
void    CMD_ELEMENTAL(Arguments& args);
void    CMD_ANTIDOTE(Arguments& args);
void    CMD_POISON(Arguments& args);
void    CMD_SAY(Arguments& args);
//party escapes from combat
void    CMD_ESCAPE(Arguments& args);
void    CMD_INN(Arguments& args);
void    CMD_SHOP(Arguments& args);
//activates blacksmith menu
void    CMD_SMITH(Arguments& args);
void    CMD_IF(const char * cmdline,
               unsigned char cmdlen);
void    CMD_EXE(Arguments& args);
void    CMD_GOTO(Arguments& args);
void    CMD_GIVE(Arguments& args);
void    CMD_Inject(Arguments& args);
void    CMD_LoadMap(Arguments& args);
void    CMD_ADDNPC(Arguments& args);
void    CMD_REMOVENPC(Arguments& args);
void    CMD_NPCFRAME(Arguments& args);              //changes basic npc frame
void    CMD_NOJOY(Arguments& args);                                //turns on/off player control
void    CMD_DESERT(Arguments& args);
void    CMD_INTERNET(Arguments& args);
void    CMD_WRITE(Arguments& args);
void    CMD_FIGHT(Arguments& args);
void    CMD_LET(Arguments& args);
void    CMD_ADDPMEMB(Arguments& args);              //Adds party member
//OpenBox(keyID, itemID, count, tile)
void    CMD_OPENBOX(Arguments& args);
//Exits game
void    CMD_EXITP(Arguments& args);
void    Interpret(const char* line);                //Interprets script line
void    CommandLine();
void    RunScript();
unsigned char GetDir(unsigned int key);
void    SetRandomCamPos();
void    NextActiveFighter();
void    FighterActionSelection();                   
//animates atack
void    AttackAnimation(Fighter * dude, int percent,
                        PartyActions * action);
//animates casting a spell
void    CastAnimation(Fighter * dude, int percent, 
                      PartyActions * action);
//animates item usage
void    UseAnimation(Fighter * dude, int percent, 
                      PartyActions * action);
//animates oponent dodging
void    DodgeAnimation(Fighter * dude, int percent, 
                      PartyActions * action);

//handles action selection in combat
void    CombatAnimationHandling();
void    OnCombat();
int     OnEntry(int x, int y);                      //Player just about to step on active sector
void    Walking();                                  //Party walking
void    HeroInfo();
//if 'cast' selected in adventure mode
void    CastOrder();
void    NpcAI();
void    Talking();
void    Game();
void    DrawIntroScreen();                          //Draws title background, menu, etc.
void    SelectRace (unsigned hero, unsigned key);
void    SelectSex (unsigned hero,
                   unsigned short key);
void    SelectClass (unsigned hero,
                     unsigned key);
void    WriteName (unsigned char hero,              //Enter hero name
                   unsigned char key);
void    RollDice (unsigned hero);
void    SelectFace(unsigned hero, unsigned key);
//Enables spells, transfers heroes names to script,
//and starts begining script
void    StartNewGame();
void    CreateParty();                               //Main heroes creation subroutine
void    GameOver();
void    LoadGraphics();
//prints text from buffer to game console
void    ConsleRoutine();
void    OnTimer();
int     FPS ();                                      //Get frames per second
void    CheckKeys();                                 //Reads input devices
void    DrawSomeText(int fontas);                    //Debug text
//Draws a fighter in the arena
void    DrawFighter(unsigned i, 
                    COLOR normal,
                    COLOR poison, 
                    bool outline = true);
//Draws 3D arena and fighters
void    DrawArena(bool outline = true);
void    RenderScreen();                              //Draw stuff on screen
void    QuitOpenAL();
void    Quit();                                      //destroys stuff
static void process_events( void );
void    LoadShaders(const char* list);
static void SetupOpengl( int width, int height );
void    InitOpenAL();
//Tries to load ogg file to stream, if fails tries from base/ folder
bool    OpenMusic(const char * name);
//plays stream if sound is enabled
void PlayMusic(const char* name);

