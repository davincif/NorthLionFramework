#the compiler
CC = gcc

#the standart ompilation flags of the project
CFLAGS = -O3 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-implicit-function-declaration

#path to the folder's root, where the holy not build framework is. Relate to this Makefile
PREPATH = ../

#Path to the SDL, SDL_image and Lua includes and libs
SDL2INCLUDES = -I $(PREPATH)SDL2/include
SDL2LIBS = $(PREPATH)SDL2/lib/libSDL2-2.0.so.0 $(PREPATH)SDL2/lib/libSDL2_test.a $(PREPATH)SDL2/lib/libSDL2-2.0.so.0.2.1 $(PREPATH)SDL2/lib/libSDL2main.a $(PREPATH)SDL2/lib/libSDL2.a $(PREPATH)SDL2/lib/libSDL2.so
SDLIMAGE2INCLUDES = -I $(PREPATH)SDL2/SDL_image
SDLIMAGE2LIBS = $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0 $(PREPATH)SDL2/lib/libSDL2_image.so $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0.0.0 $(PREPATH)SDL2/lib/libSDL2_image.a
#LUAINCLUDES = -I $(PREPATH)lua/
#LUALIB = $(PREPATH)lua/ -llua -lm

#Where to put the compiled program
COMPILEPATH = $(PREPATH)BINARIES/



#Build options

build: NLF.o
	cp ./*.o $(COMPILEPATH)
	$(CC) -shared $(CFLAGS) $(SDL2INCLUDES) $(SDL2LIBS) $(SDLIMAGE2INCLUDES) $(SDLIMAGE2LIBS) $(COMPILEPATH)*.o -o $(COMPILEPATH)libNLF.so

NLF.o: NLF_osservice.o NLF_Error.o NLF_event.o NLF_screen.o NLF.h.gch
	$(CC) $(CFLAGS) $(SDL2INCLUDES) $(SDLIMAGE2INCLUDES) -fPIC -c NLF.c

NLF.h.gch: NLF.h
	$(CC) $(CFLAGS) $(SDL2INCLUDES) $(SDLIMAGE2INCLUDES) NLF.h

NLF_osservice.o: NLF_osservice.h.gch
	$(CC) $(CFLAGS) $(SDL2INCLUDES) -fPIC -c NLF_osservice.c
NLF_osservice.h.gch:
	$(CC) $(CFLAGS) $(SDL2INCLUDES) NLF_osservice.h

NLF_Error.o: NLF_error.h.gch
	$(CC) $(CFLAGS) $(SDL2INCLUDES) -fPIC -c NLF_error.c
NLF_error.h.gch:
	$(CC) $(CFLAGS) $(SDL2INCLUDES) NLF_error.h

NLF_event.o: NLF_event.h.gch
	$(CC) $(CFLAGS) $(SDL2INCLUDES) -fPIC -c NLF_event.c
NLF_event.h.gch:
	$(CC) $(CFLAGS) $(SDL2INCLUDES) NLF_event.h	

NLF_screen.o: NLF_screen.h.gch
	$(CC) $(CFLAGS) $(SDL2INCLUDES) $(SDLIMAGE2INCLUDES) -fPIC -c NLF_screen.c
NLF_screen.h.gch:
	$(CC) $(CFLAGS) $(SDL2INCLUDES) $(SDLIMAGE2INCLUDES) NLF_screen.h



#cleaning options

clean-build:
	rm -f -v $(COMPILEPATH)*.o

clean-hard clean-all:
	rm -f -v ./*.o ./*.h.gch
	rm -f -v $(COMPILEPATH)*.o $(COMPILEPATH)*.so

clean-soft clean-all-but-so:
	rm -f -v ./*.o ./*.h.gch