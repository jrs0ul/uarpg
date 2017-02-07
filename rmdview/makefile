CC=g++
all: 
	${CC} src/gtkrmd.cpp  ../src/Extensions.cpp ../src/TextureLoader.cpp ../src/Image.cpp ../src/models/RatModel.cpp ../src/models/ModelCollection.cpp ../src/models/rm2.cpp ../src/Shader.cpp ../src/ShaderProgram.cpp ../src/Matrix.cpp ../src/Vectors.cpp ../src/Entity.cpp ../src/Utils.cpp -o gtkrmd -Wall `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs gtkglext-1.0` `sdl-config --cflags --libs`  
