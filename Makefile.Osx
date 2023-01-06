cc = gcc

srcDirectory = ./src
libDirectory = ./libs
srcFiles = $(srcDirectory)/main.c $(srcDirectory)/R/*.c $(srcDirectory)/R/_utl/*.c $(srcDirectory)/Sys/*.c $(libDirectory)/glad/src/gl.c

cFlags = -o macchapuchare -I$(libDirectory)/glad/include -I$(libDirectory)/SingleHeader -lSDL2 -framework OpenGL

build:
	$(cc) $(cFlags) $(srcFiles)