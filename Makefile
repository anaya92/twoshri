architecture = x86_64# only 64 bit possible for now
cc = gcc# soon add compiler options which allow for 32/64 bit compilation

libRoot = ./libs

requireSDL2 = -I$(libRoot)/$(architecture)/SDL2/include -L$(libRoot)/$(architecture)/SDL2/lib -lSDL2

.PHONY: Gl46 TwoshriDemo
all: Gl46 TwoshriDemo
	echo built all

# GL STUFF
glSrc = ./src/Dll/R/Gl46
glLib = ./src/Dll/R/Gl46/libs
glFiles = $(glSrc)/dllmain.c $(glSrc)/renderer.c $(glSrc)/2d.c $(glSrc)/3d.c $(glSrc)/_utl/*.c $(glLib)/glad/src/*.c
glCFlags = -O2 -shared -o Ts.Gl46 -Wint-to-pointer-cast -I$(libRoot)/SingleHeader/ -I./src/R/ -I$(glLib)/glad/include $(requireSDL2) -lopengl32

Gl46:
	$(cc) $(glFiles) $(glCFlags) 
	mv Ts.Gl46 Ts.Gl46.dll

#primary stuff
primarySrc = ./src
primaryLib = ./libs
primaryFiles = $(primarySrc)/primary_main.c $(primarySrc)/R/*.c $(primarySrc)/Sys/*.c
primaryCFlags = -O2 -o TwoshriDemo -I$(libRoot)/glad/include -I$(libRoot)/SingleHeader $(requireSDL2) -lSDL2main -lopengl32

TwoshriDemo:
	$(cc) $(primaryFiles) $(primaryCFlags) 
