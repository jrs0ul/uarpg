import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)

code = [            "src/gui/Text.cpp",
                      "src/DebugProps.cpp",
                      "src/gui/MenuStruct.cpp",
                      "src/Arena.cpp",
                      "src/Combat.cpp",
                      "src/VertexObject.cpp",
                      "src/Strategy.cpp",
                      "src/Creature.cpp",
                      "src/EventArray.cpp",
                      "src/ShadowMap.cpp",
                      "src/Extensions.cpp",
                      "src/Image.cpp",
                      "src/Inventory.cpp",
                      "src/Item.cpp",
                      "src/Klasses.cpp",
                      "src/MonsterGroups.cpp",
                      "src/particles/Particles.cpp",
                      "src/particles/TextParticles.cpp",
                      "src/particles/ParticleSystems.cpp",
                      "src/Party.cpp",
                      "src/PlayerGfxList.cpp",
                      "src/Races.cpp",
                      "src/SpellData.cpp",
                      "src/SystemConfig.cpp",
                      "src/TextureLoader.cpp",
                      "src/Treasure.cpp",
                      "src/Worldmap.cpp",
                      "src/Xml.cpp",
                      "src/interpreter.cpp",
                      "src/npc.cpp",
                      "src/savegame.cpp",
                      "src/Entity.cpp",
                      "src/Matrix.cpp",
                      "src/Utils.cpp",
                      "src/Vectors.cpp",
                      "src/CCode.cpp",
                      "src/variables.cpp",
                      "src/audio/SoundSystem.cpp",
                      "src/audio/OggStream.cpp",
                      "src/gui/BatlleSelect.cpp",
                      "src/gui/Gui.cpp",
                      "src/gui/MenuLoader.cpp",
                      "src/gui/SelectMenu.cpp",
                      "src/gui/CreatureStatus.cpp",
                      "src/gui/TextScreen.cpp",
                      "src/gui/StatGain.cpp",
                      "src/gui/ItemGain.cpp",
                      "src/Merchant.cpp",
                      "src/gui/EditBox.cpp",
                      "src/Skybox.cpp",
                      "src/Log.cpp",
                      "src/gui/OnScreenBoard.cpp",
                      "src/Shader.cpp",
                      "src/ShaderProgram.cpp",
                      "src/models/rm2.cpp",
                      "src/gui/Credits.cpp",
                      "src/gui/Coolframe.cpp",
                      "src/main.cpp"
                      ]


#Windblows
if sys.platform == 'win32':
    Tool('mingw')(env)
    env.Append(LIBS = ['mingw32','SDLmain', 'SDL', 'OpenGL32',
                       'openal32', 'vorbisfile', 'ogg'])
else: #Mac OS X
    if sys.platform == 'darwin':
        env['FRAMEWORKS'] = ['Cocoa', 'SDL', 'OPENGL', 'OpenAL','vorbis', 'Ogg']
        env.Append(LIBS = [ 'iconv', 'pthread'])
        env.Append(CCFLAGS = ['-arch', 'i386'])
        env.Object('src/osx/SDLMain.m', OBJPREFIX = OBJ_DIR,
                   CCFLAGS = ['-arch', 'i386', '-I/Library/Frameworks/SDL.framework/Headers/'])
        code.append('obj/SDLMain.o')
        env.Append(LINKFLAGS = ['-arch', 'i386'])
        
    else: #Linux
        env.Append(LIBS = ['SDL','GL', 'openal', 'vorbisfile',
                           'ogg', 'pthread'])


env.Program(target = "godrpg", source = code)
SConscript('edmapSDL/SConstruct')

