CC = gcc
CFLAGS = -O3
PREPATH = ../

SDL2INCLUDES = -I $(PREPATH)SDL2/include
SDL2LIBS = -L $(PREPATH)SDL2/lib/ -lSDL2 -lSDL2main -lSDL2_test
SDLIMAGE2INCLUDES = -I $(PREPATH)SDL2/SDL_image
SDLIMAGE2LIBS = $(PREPATH)SDL2/lib/libSDL2_image.a $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0 $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0.0.1
SDLIMAGE2INCLUDES = -I $(PREPATH)SDL2/SDL_image
#LUAINCLUDES = -I $(PREPATH)lua/
#LUALIB = -L $(PREPATH)lua/ -llua -lm

NLFINCLUDES = -I $(PREPATH)NLF_SCR/
NLFLIB = ../BINARIES/libNLF.so


build:
	$(CC) $(CFLAGS) $(NLFINCLUDES) $(SDL2INCLUDES) $(SDLIMAGE2INCLUDES) auto_genareted_code.c $(NLFLIB) $(SDL2LIBS) $(SDLIMAGE2LIBS) -o auto_genareted_code

clean:
	rm auto_genareted_code ./*.o ./*.h.gch
