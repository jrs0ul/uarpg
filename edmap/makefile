OBJS=obj/Xml.o obj/Utils.o obj/TileTool.o obj/gtkUtils.o obj/TextureLoader.o obj/Image.o obj/Worldmap.o obj/Vectors.o obj/rm2.o obj/Extensions.o obj/Matrix.o obj/Entity.o obj/MapHeaderDlg.o obj/ShaderProgram.o obj/TilesetInfo.o obj/Npc.o obj/VertexObject.o obj/mapper.o
CC=g++
CFLAGS=-O3 -Wall -c
LFLAGS=`pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs gtkglext-1.0` `sdl-config --cflags --libs`
all:${OBJS} 
	${CC} ${OBJS} ${LFLAGS} -o edmap
clean:
	rm obj/*.o

obj/VertexObject.o: ../src/VertexObject.cpp ../src/VertexObject.h
	${CC} ${CFLAGS} ../src/VertexObject.cpp -o obj/VertexObject.o
obj/TileTool.o: src/TileTool.cpp src/TileTool.h
	${CC} ${CFLAGS} src/TileTool.cpp -o obj/TileTool.o ${LFLAGS}
obj/TilesetInfo.o: src/TilesetInfo.cpp src/TilesetInfo.h
	${CC} ${CFLAGS} src/TilesetInfo.cpp -o obj/TilesetInfo.o 
obj/gtkUtils.o: src/gtkUtils.cpp src/gtkUtils.h
	${CC} ${CFLAGS} src/gtkUtils.cpp -o obj/gtkUtils.o ${LFLAGS}
obj/MapHeaderDlg.o: src/MapHeaderDlg.cpp src/MapHeaderDlg.h ../src/Worldmap.h
	${CC} ${CFLAGS} src/MapHeaderDlg.cpp -o obj/MapHeaderDlg.o ${LFLAGS}
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
obj/rm2.o: ../src/models/rm2.h ../src/models/rm2.cpp ../src/Vectors.h ../src/Extensions.h ../src/Matrix.h
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
obj/mapper.o: src/mapper.cpp src/mapper.h ../src/TextureLoader.h ../src/Entity.h src/gtkUtils.h src/TileTool.h src/TilesetInfo.h
	${CC} ${CFLAGS} src/mapper.cpp -o obj/mapper.o ${LFLAGS}
