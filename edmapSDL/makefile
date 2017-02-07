OBJS=obj/Xml.o obj/Utils.o obj/TileTool.o obj/TextureLoader.o obj/Image.o obj/Worldmap.o obj/Vectors.o obj/rm2.o obj/Extensions.o obj/Matrix.o obj/Entity.o obj/VertexObject.o obj/MapHeaderDlg.o obj/ShaderProgram.o obj/TilesetInfo.o obj/Npc.o obj/Button.o obj/ViewDialog.o obj/Text.o obj/AssetBrowser.o obj/main.o
CC=g++
CFLAGS=-O3 -Wall -c
LFLAGS= -lSDL -lGL -lGLU -lpthread
LWFLAGS= -lmingw32 -lSDLmain -lSDL -lOpenGL32 -lGLU32
all:${OBJS} 
	${CC} ${OBJS} ${LFLAGS} -o edmap
win32:${OBJS} 
	${CC} ${OBJS} ${LWFLAGS} -o edmap.exe
clean:
	rm obj/*.o

obj/AssetBrowser.o: src/AssetBrowser.cpp src/AssetBrowser.h
	${CC} ${CFLAGS} src/AssetBrowser.cpp -o obj/AssetBrowser.o
obj/ViewDialog.o: src/ViewDialog.cpp src/ViewDialog.h
	${CC} ${CFLAGS} src/ViewDialog.cpp -o obj/ViewDialog.o
obj/Text.o: ../src/gui/Text.cpp ../src/gui/Text.h
	${CC} ${CFLAGS} ../src/gui/Text.cpp -o obj/Text.o
obj/TileTool.o: src/TileTool.cpp src/TileTool.h
	${CC} ${CFLAGS} src/TileTool.cpp -o obj/TileTool.o
obj/Button.o: ../src/gui/Button.cpp ../src/gui/Button.h
	${CC} ${CFLAGS} ../src/gui/Button.cpp -o obj/Button.o
obj/TilesetInfo.o: src/TilesetInfo.cpp src/TilesetInfo.h
	${CC} ${CFLAGS} src/TilesetInfo.cpp -o obj/TilesetInfo.o 
obj/VertexObject.o: ../src/VertexObject.cpp ../src/VertexObject.h
	${CC} ${CFLAGS} ../src/VertexObject.cpp -o obj/VertexObject.o
obj/MapHeaderDlg.o: src/MapHeaderDlg.cpp src/MapHeaderDlg.h ../src/Worldmap.h
	${CC} ${CFLAGS} src/MapHeaderDlg.cpp -o obj/MapHeaderDlg.o
obj/Image.o: ../src/Image.cpp ../src/Image.h ../src/Utils.h
	${CC} ${CFLAGS} ../src/Image.cpp -o obj/Image.o 
obj/TextureLoader.o: ../src/TextureLoader.cpp ../src/TextureLoader.h ../src/Image.h ../src/Colors.h
	${CC} ${CFLAGS} ../src/TextureLoader.cpp -o obj/TextureLoader.o 
obj/Entity.o: ../src/Entity.cpp ../src/Entity.h
	${CC} ${CFLAGS} ../src/Entity.cpp -o obj/Entity.o 
obj/Worldmap.o: ../src/Worldmap.cpp ../src/Worldmap.h ../src/TextureLoader.h
	${CC} ${CFLAGS} ../src/Worldmap.cpp -o obj/Worldmap.o 
obj/Vectors.o: ../src/Vectors.cpp ../src/Vectors.h
	${CC} ${CFLAGS} ../src/Vectors.cpp -o obj/Vectors.o
obj/rm2.o: ../src/models/rm2.h ../src/models/rm2.cpp ../src/Vectors.h ../src/VertexObject.h ../src/Extensions.h ../src/Matrix.h
	${CC} ${CFLAGS} ../src/models/rm2.cpp -o obj/rm2.o
obj/Matrix.o: ../src/Matrix.cpp ../src/Matrix.h
	${CC} ${CFLAGS} ../src/Matrix.cpp -o obj/Matrix.o
obj/Extensions.o: ../src/Extensions.cpp ../src/Extensions.h
	${CC} ${CFLAGS} ../src/Extensions.cpp -o obj/Extensions.o
obj/Utils.o: ../src/Utils.cpp ../src/Utils.h
	${CC} ${CFLAGS} ../src/Utils.cpp -o obj/Utils.o
obj/Xml.o: ../src/Xml.cpp ../src/Xml.h ../src/Utils.h
	${CC} ${CFLAGS} ../src/Xml.cpp -o obj/Xml.o
obj/Npc.o: ../src/npc.cpp ../src/npc.h ../src/Worldmap.h ../src/TextureLoader.h ../src/Utils.h ../src/Xml.h
	${CC} ${CFLAGS} ../src/npc.cpp -o obj/Npc.o
obj/ShaderProgram.o: ../src/ShaderProgram.h ../src/ShaderProgram.cpp ../src/Shader.h ../src/Extensions.h
	${CC} ${CFLAGS} ../src/ShaderProgram.cpp -o obj/ShaderProgram.o
obj/main.o: src/main.cpp src/main.h ../src/TextureLoader.h ../src/models/rm2.h ../src/Entity.h src/TileTool.h src/TilesetInfo.h src/AssetBrowser.h
	${CC} ${CFLAGS} src/main.cpp -o obj/main.o
