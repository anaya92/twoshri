cc = gcc

.PHONY: Gl46 TwoshriDemo
all: Gl46 TwoshriDemo
	echo built all

# GL STUFF
glSrc = ./src/Dll/R/Gl46
glLib = ./src/Dll/R/Gl46/libs
glFiles = $(glSrc)/dllmain.c $(glSrc)/renderer.c $(glSrc)/2d.c $(glSrc)/3d.c $(glSrc)/_utl/*.c $(glLib)/glad/src/*.c
glCFlags = -O2 -shared -o Ts.Gl46 -I./libs/SingleHeader/ -I./src/R/ -I$(glLib)/glad/include -lSDL2 -lopengl32

Gl46:
	$(cc) $(glFiles) $(glCFlags) 
	mv Ts.Gl46 Ts.Gl46.dll

#primary stuff
primarySrc = ./src
primaryLib = ./libs
primaryFiles = $(primarySrc)/main.c $(primarySrc)/R/*.c $(primarySrc)/Sys/*.c
primaryCFlags = -O2 -o TwoshriDemo -I$(primaryLib)/glad/include -I$(primaryLib)/SingleHeader -lSDL2 -lSDL2main -lopengl32

TwoshriDemo:
	$(cc) $(primaryFiles) $(primaryCFlags) 
