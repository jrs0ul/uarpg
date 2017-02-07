CC=g++
CFLAGS=-O3 -Wall -pedantic
CFLAGSNOPED=-O3 -Wall
all:
	${CC} TestMatrix.cpp ../src/Vectors.cpp ../src/Matrix.cpp -lpthread ${CFLAGS} -o testMatrix
	${CC} TestRaces.cpp ../src/Races.cpp ../src/Vectors.cpp ../src/Xml.cpp ../src/Utils.cpp -lpthread ${CFLAGS} -o testraces
	${CC} PlayerGfxListTest.cpp ../src/Vectors.cpp ../src/PlayerGfxList.cpp ../src/Utils.cpp ../src/Xml.cpp ../src/Races.cpp  ../src/Klasses.cpp -lpthread -O3 ${CFLAGS} -o testPlayerGfxList
	${CC} TestClasses.cpp ../src/Utils.cpp ../src/Vectors.cpp ../src/Xml.cpp ../src/Races.cpp -lpthread ${CFLAGS} -o testClasses
	${CC} testspells.cpp ../src/Utils.cpp ../src/Xml.cpp ../src/Vectors.cpp ../src/SpellData.cpp -lpthread ${CFLAGS} -o testspells
	${CC} TestXml.cpp ../src/Utils.cpp ../src/Vectors.cpp ../src/Xml.cpp -lpthread ${CFLAGS} -o testXml
	${CC} TestNpc.cpp ../src/Worldmap.cpp ../src/Extensions.cpp ../src/Vectors.cpp ../src/VertexObject.cpp ../src/Matrix.cpp ../src/models/rm2.cpp ../src/ShaderProgram.cpp ../src/Image.cpp ../src/TextureLoader.cpp ../src/npc.cpp ../src/Utils.cpp ../src/Xml.cpp -lSDL -lGL -lpthread ${CFLAGSNOPED} -o testNpc
	${CC} TestModels.cpp ../src/Extensions.cpp ../src/Vectors.cpp ../src/Matrix.cpp ../src/VertexObject.cpp ../src/Utils.cpp ../src/models/rm2.cpp -lSDL -lGL -lpthread ${CFLAGSNOPED} -o testModels
