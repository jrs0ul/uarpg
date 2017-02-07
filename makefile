OBJS=obj/Xml.o obj/Arena.o obj/Combat.o obj/Creature.o obj/Strategy.o obj/EventArray.o obj/Extensions.o obj/Image.o obj/VertexObject.o obj/Inventory.o obj/Item.o obj/Klasses.o obj/MonsterGroups.o obj/ParticleSystems.o obj/Particles.o obj/Party.o obj/PlayerGfxList.o obj/Races.o obj/SpellData.o obj/SystemConfig.o obj/TextureLoader.o obj/Treasure.o obj/Worldmap.o obj/interpreter.o obj/npc.o obj/savegame.o obj/Entity.o obj/Matrix.o obj/Utils.o obj/Vectors.o obj/CCode.o obj/variables.o obj/SoundSystem.o obj/OggStream.o obj/BatlleSelect.o obj/Gui.o obj/MenuLoader.o obj/SelectMenu.o obj/CreatureStatus.o obj/TextScreen.o obj/MenuStruct.o obj/TextParticles.o obj/StatGain.o obj/ItemGain.o obj/Merchant.o obj/EditBox.o obj/Skybox.o obj/Log.o obj/OnScreenBoard.o obj/Shader.o obj/ShaderProgram.o obj/rm2.o obj/DebugProps.o obj/Credits.o obj/ShadowMap.o obj/Text.o obj/main.o
CC=g++
CFLAGS=-O3 -Wall -c -isysroot /Developer/SDKs/MacOSX10.5.sdk
LFLAGS=-lSDL -lGL -lopenal -lvorbisfile -logg -lpthread
WINLFLAGS= -lmingw32 -lSDLmain -lSDL -lOPENGL32 -lopenal32 -lvorbisfile -logg 
OSXLFLAGS= -framework Cocoa -lSDLmain -framework SDL -framework OPENGL -framework OpenAL -framework vorbis -framework Ogg -liconv -lpthread

all:${OBJS}
	${CC} ${OBJS} ${LFLAGS} -o godrpg
win32:${OBJS}
	${CC} ${OBJS} ${WINLFLAGS} -mwindows -o godrpg
osx:${OBJS}
	${CC} ${OBJS} ${OSXLFLAGS} -o godrpg

obj/Text.o: src/gui/Text.h
	${CC} ${CFLAGS} src/gui/Text.cpp -o obj/Text.o
obj/DebugProps.o: src/DebugProps.h src/DebugProps.cpp src/Vectors.h
	${CC} ${CFLAGS} src/DebugProps.cpp -o obj/DebugProps.o
obj/MenuStruct.o: src/gui/MenuStruct.cpp src/gui/MenuStruct.h
	${CC} ${CFLAGS} src/gui/MenuStruct.cpp -o obj/MenuStruct.o
obj/Arena.o: src/Arena.cpp src/Arena.h src/TextureLoader.h src/models/rm2.h src/Xml.h
	${CC} ${CFLAGS} src/Arena.cpp -o obj/Arena.o
obj/Combat.o: src/Combat.cpp src/Combat.h
	${CC} ${CFLAGS} src/Combat.cpp -o obj/Combat.o
obj/VertexObject.o: src/VertexObject.cpp src/VertexObject.h
	${CC} ${CFLAGS} src/VertexObject.cpp -o obj/VertexObject.o
obj/Strategy.o: src/Strategy.cpp src/Strategy.h
	${CC} ${CFLAGS} src/Strategy.cpp -o obj/Strategy.o
obj/Creature.o: src/Creature.cpp src/Creature.h src/SpellData.h src/Strategy.h src/Inventory.h src/Xml.h src/Utils.h src/Klasses.h src/Races.h
	${CC} ${CFLAGS} src/Creature.cpp -o obj/Creature.o
obj/EventArray.o: src/EventArray.cpp src/EventArray.h src/Xml.h src/Utils.h
	${CC} ${CFLAGS} src/EventArray.cpp -o obj/EventArray.o
obj/ShadowMap.o: src/ShadowMap.cpp 
	${CC} ${CFLAGS} src/ShadowMap.cpp -o obj/ShadowMap.o
obj/Extensions.o: src/Extensions.cpp src/Extensions.h
	${CC} ${CFLAGS} src/Extensions.cpp -o obj/Extensions.o
obj/Image.o: src/Image.cpp src/Image.h src/Xml.h src/Utils.h
	${CC} ${CFLAGS} src/Image.cpp -o obj/Image.o
obj/Inventory.o: src/Inventory.cpp src/Inventory.h
	${CC} ${CFLAGS} src/Inventory.cpp -o obj/Inventory.o
obj/Item.o: src/Item.cpp src/Item.h src/BasicItem.h src/DArray.h src/Utils.h
	${CC} ${CFLAGS} src/Item.cpp -o obj/Item.o
obj/Klasses.o: src/Klasses.cpp src/Klasses.h src/DArray.h src/Utils.h src/Xml.h
	${CC} ${CFLAGS} src/Klasses.cpp -o obj/Klasses.o
obj/MonsterGroups.o: src/MonsterGroups.cpp src/MonsterGroups.h src/Strategy.h src/DArray.h src/Xml.h src/Utils.h
	${CC} ${CFLAGS} src/MonsterGroups.cpp -o obj/MonsterGroups.o
obj/ParticleSystems.o: src/particles/ParticleSystems.cpp src/particles/ParticleSystems.h
	${CC} ${CFLAGS} src/particles/ParticleSystems.cpp -o obj/ParticleSystems.o
obj/Particles.o: src/particles/Particles.cpp src/particles/Particles.h src/DArray.h src/Camera.h src/Vectors.h src/Colors.h src/VertexObject.h
	${CC} ${CFLAGS} src/particles/Particles.cpp -o obj/Particles.o
obj/TextParticles.o: src/particles/TextParticles.cpp src/particles/TextParticles.h src/particles/Particles.h src/Camera.h src/Vectors.h src/Colors.h src/DArray.h
	${CC} ${CFLAGS} src/particles/TextParticles.cpp -o obj/TextParticles.o
obj/Party.o: src/Party.cpp src/Party.h src/TextureLoader.h src/Creature.h src/npc.h src/models/rm2.h src/Inventory.h
	${CC} ${CFLAGS} src/Party.cpp -o obj/Party.o
obj/PlayerGfxList.o: src/PlayerGfxList.cpp src/PlayerGfxList.h src/Xml.h src/DArray.h src/Klasses.h src/Races.h
	${CC} ${CFLAGS} src/PlayerGfxList.cpp -o obj/PlayerGfxList.o
obj/Races.o: src/Races.cpp src/Races.h src/DArray.h src/Utils.h src/Xml.h
	${CC} ${CFLAGS} src/Races.cpp -o obj/Races.o
obj/SpellData.o: src/SpellData.cpp src/SpellData.h src/BasicItem.h src/DArray.h src/Xml.h
	${CC} ${CFLAGS} src/SpellData.cpp -o obj/SpellData.o
obj/SystemConfig.o: src/SystemConfig.cpp src/SystemConfig.h src/Xml.h
	${CC} ${CFLAGS} src/SystemConfig.cpp -o obj/SystemConfig.o
obj/TextureLoader.o: src/TextureLoader.cpp src/TextureLoader.h src/Image.h src/Colors.h
	${CC} ${CFLAGS} src/TextureLoader.cpp -o obj/TextureLoader.o
obj/Treasure.o: src/Treasure.cpp src/Treasure.h
	${CC} ${CFLAGS} src/Treasure.cpp -o obj/Treasure.o
obj/Worldmap.o: src/Worldmap.cpp src/Worldmap.h src/TextureLoader.h src/models/rm2.h src/VertexObject.h
	${CC} ${CFLAGS} src/Worldmap.cpp -o obj/Worldmap.o
obj/Xml.o: src/Xml.cpp src/Xml.h src/Utils.h
	${CC} ${CFLAGS} src/Xml.cpp -o obj/Xml.o
obj/interpreter.o: src/interpreter.cpp src/interpreter.h
	${CC} ${CFLAGS} src/interpreter.cpp -o obj/interpreter.o
obj/npc.o: src/npc.cpp src/npc.h src/Worldmap.h src/TextureLoader.h src/Utils.h src/Xml.h
	${CC} ${CFLAGS} src/npc.cpp -o obj/npc.o
obj/savegame.o: src/savegame.cpp src/savegame.h src/npc.h src/Creature.h src/Party.h src/Treasure.h
	${CC} ${CFLAGS} src/savegame.cpp -o obj/savegame.o
obj/Entity.o: src/Entity.cpp src/Entity.h
	${CC} ${CFLAGS} src/Entity.cpp -o obj/Entity.o
obj/Matrix.o: src/Matrix.cpp src/Matrix.h
	${CC} ${CFLAGS} src/Matrix.cpp -o obj/Matrix.o
obj/Utils.o: src/Utils.cpp src/Utils.h
	${CC} ${CFLAGS} src/Utils.cpp -o obj/Utils.o
obj/Vectors.o: src/Vectors.cpp src/Vectors.h
	${CC} ${CFLAGS} src/Vectors.cpp -o obj/Vectors.o
obj/CCode.o: src/CCode.cpp src/CCode.h src/DArray.h src/Utils.h
	${CC} ${CFLAGS} src/CCode.cpp -o obj/CCode.o
obj/variables.o: src/variables.cpp src/variables.h
	${CC} ${CFLAGS} src/variables.cpp -o obj/variables.o
obj/SoundSystem.o: src/audio/SoundSystem.cpp src/audio/SoundSystem.h
	${CC} ${CFLAGS} src/audio/SoundSystem.cpp -o obj/SoundSystem.o
obj/OggStream.o: src/audio/OggStream.cpp src/audio/OggStream.h
	${CC} ${CFLAGS} src/audio/OggStream.cpp -o obj/OggStream.o
obj/BatlleSelect.o: src/gui/BatlleSelect.cpp src/gui/BatlleSelect.h src/models/rm2.h src/Combat.h
	${CC} ${CFLAGS} src/gui/BatlleSelect.cpp -o obj/BatlleSelect.o
obj/Gui.o: src/gui/Gui.cpp src/gui/Gui.h src/Utils.h src/gui/BasicUiControl.h src/Creature.h src/Party.h src/TextureLoader.h src/gui/MenuStruct.h src/Item.h
	${CC} ${CFLAGS} src/gui/Gui.cpp -o obj/Gui.o
obj/MenuLoader.o: src/gui/MenuLoader.cpp src/gui/MenuLoader.h src/gui/MenuStruct.h src/Xml.h src/Utils.h
	${CC} ${CFLAGS} src/gui/MenuLoader.cpp -o obj/MenuLoader.o
obj/SelectMenu.o: src/gui/SelectMenu.cpp src/gui/SelectMenu.h src/gui/MenuStruct.h src/gui/BasicUiControl.h src/audio/SoundSystem.h src/TextureLoader.h src/gui/Gui.h
	${CC} ${CFLAGS} src/gui/SelectMenu.cpp -o obj/SelectMenu.o
obj/CreatureStatus.o: src/gui/CreatureStatus.cpp src/gui/CreatureStatus.h src/gui/Gui.h
	${CC} ${CFLAGS} src/gui/CreatureStatus.cpp -o obj/CreatureStatus.o
obj/TextScreen.o: src/gui/TextScreen.cpp src/gui/TextScreen.h src/gui/Gui.h
	${CC} ${CFLAGS} src/gui/TextScreen.cpp -o obj/TextScreen.o
obj/StatGain.o: src/gui/StatGain.cpp src/gui/StatGain.h src/gui/BasicUiControl.h src/Party.h src/gui/Gui.h src/TextureLoader.h
	${CC} ${CFLAGS} src/gui/StatGain.cpp -o obj/StatGain.o
obj/ItemGain.o: src/gui/ItemGain.cpp src/gui/BasicUiControl.h src/Party.h src/gui/Gui.h src/TextureLoader.h
	${CC} ${CFLAGS} src/gui/ItemGain.cpp -o obj/ItemGain.o
obj/Merchant.o: src/Merchant.cpp src/Merchant.h src/DArray.h src/Xml.h src/gui/SelectMenu.h src/Item.h
	${CC} ${CFLAGS} src/Merchant.cpp -o obj/Merchant.o
obj/EditBox.o: src/gui/EditBox.h src/gui/EditBox.cpp src/gui/BasicUiControl.h src/TextureLoader.h
	${CC} ${CFLAGS} src/gui/EditBox.cpp -o obj/EditBox.o
obj/Skybox.o: src/Skybox.h src/Skybox.cpp src/TextureLoader.h
	${CC} ${CFLAGS} src/Skybox.cpp -o obj/Skybox.o
obj/Log.o: src/Log.h src/Log.cpp
	${CC} ${CFLAGS} src/Log.cpp -o obj/Log.o
obj/OnScreenBoard.o: src/gui/OnScreenBoard.h src/gui/OnScreenBoard.cpp src/TextureLoader.h src/gui/BasicUiControl.h
	${CC} ${CFLAGS} src/gui/OnScreenBoard.cpp -o obj/OnScreenBoard.o
obj/Shader.o: src/Shader.h src/Shader.cpp src/Utils.h src/Extensions.h
	${CC} ${CFLAGS} src/Shader.cpp -o obj/Shader.o
obj/ShaderProgram.o: src/ShaderProgram.h src/ShaderProgram.cpp src/Shader.h src/Extensions.h
	${CC} ${CFLAGS} src/ShaderProgram.cpp -o obj/ShaderProgram.o
obj/rm2.o: src/models/rm2.h src/models/rm2.cpp  src/VertexObject.h src/Vectors.h src/Extensions.h src/Matrix.h
	${CC} ${CFLAGS} src/models/rm2.cpp -o obj/rm2.o
obj/Credits.o: src/gui/Credits.h src/gui/Credits.cpp src/gui/BasicUiControl.h src/TextureLoader.h
	${CC} ${CFLAGS} src/gui/Credits.cpp -o obj/Credits.o
obj/main.o: src/main.cpp src/main.h src/gui/SelectMenu.h src/Combat.h src/Arena.h src/Skybox.h src/HeroMaker.h src/PlayerGfxList.h src/EventArray.h src/SystemConfig.h src/gui/CreatureStatus.h src/Strategy.h src/gui/TextScreen.h src/Worldmap.h src/Party.h src/gui/StatGain.h src/gui/ItemGain.h src/Merchant.h src/savegame.h src/npc.h src/TextureLoader.h src/gui/Gui.h src/gui/BatlleSelect.h src/Utils.h src/gui/EditBox.h src/CCode.h src/MonsterGroups.h src/Log.h src/Creature.h src/Shader.h src/ShaderProgram.h src/gui/OnScreenBoard.h src/models/rm2.h src/DebugProps.h src/ShadowMap.h
	${CC} ${CFLAGS} src/main.cpp -o obj/main.o

clean:
	rm obj/*.o

install:
	mkdir /usr/share/godrpg
	cp -r base* /usr/share/godrpg
	cp godrpg /usr/share/godrpg
	cp run /usr/share/godrpg
	cp -r rmdview /usr/share/godrpg
	cp -r edmap /usr/share/godrpg
	ln -s /usr/share/godrpg/run /usr/bin/godrpg


