CC = gcc
CFLAGS = -O3
PREPATH = ../

SDL2INCLUDES = -I $(PREPATH)SDL2/include
SDL2LIBS = $(PREPATH)SDL2/lib/libSDL2-2.0.so.0 $(PREPATH)SDL2/lib/libSDL2_test.a $(PREPATH)SDL2/lib/libSDL2-2.0.so.0.2.1 $(PREPATH)SDL2/lib/libSDL2main.a $(PREPATH)SDL2/lib/libSDL2.a $(PREPATH)SDL2/lib/libSDL2.so
SDLIMAGE2INCLUDES = -I $(PREPATH)SDL2/SDL_image
SDLIMAGE2LIBS = $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0 $(PREPATH)SDL2/lib/libSDL2_image.so $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0.0.0 $(PREPATH)SDL2/lib/libSDL2_image.a
#LUAINCLUDES = -I $(PREPATH)lua/
#LUALIB = -L $(PREPATH)lua/ -llua -lm

NLFINCLUDES = -I $(PREPATH)NLF_SCR/
NLFLIB = ../BINARIES/libNLF.so


build:
	$(CC) $(CFLAGS) auto_genareted_code.c $(NLFINCLUDES) $(SDL2INCLUDES) -o auto_genareted_code $(NLFLIB) $(SDL2LIBS)

clean:
	rm auto_genareted_code ./*.o ./*.h.gch