CC = gcc
CFLAGS = -O3
PREPATH = ../

SDL2INCLUDES = -I $(PREPATH)SDL2/include
SDL2LIBS = -L $(PREPATH)SDL2/lib/libSDL2-2.0.so.0 -L $(PREPATH)SDL2/lib/libSDL2_test.a -L $(PREPATH)SDL2/lib/libSDL2-2.0.so.0.2.1 -L $(PREPATH)SDL2/lib/libSDL2main.a -L $(PREPATH)SDL2/lib/libSDL2.a -L $(PREPATH)SDL2/lib/libSDL2.so
SDLIMAGE2INCLUDES = -I $(PREPATH)SDL2/SDL_image
SDLIMAGE2LIBS = -L $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0 -L $(PREPATH)SDL2/lib/libSDL2_image.so -L $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0.0.0 -L $(PREPATH)SDL2/lib/libSDL2_image.a
LUAINCLUDES = -I $(PREPATH)lua/
LUALIB = -L $(PREPATH)lua/ -llua -lm

NLFINCLUDES = -I $(PREPATH)NLF_SCR/
NLFLIB = -L ./libNFL.so


build:
	$(CC) $(CFLAGS) $(NLFINCLUDES) $(SDL2INCLUDES) auto_genareted_code.c -o auto_genareted_code $(NLFLIB) $(SDL2LIBS)


#CC = gcc
#CFLAGS = -O3
#PREPATH = ../
#
#SDL2INCLUDES = -I $(PREPATH)SDL2/include
#SDL2LIBS = -L $(PREPATH)SDL2/lib/libSDL2-2.0.so.0 -L $(PREPATH)SDL2/lib/libSDL2_test.a -L $(PREPATH)SDL2/lib/libSDL2-2.0.so.0.2.1 -L $(PREPATH)SDL2/lib/libSDL2main.a -L $(PREPATH)SDL2/lib/libSDL2.a -L $(PREPATH)SDL2/lib/libSDL2.so
#SDLIMAGE2INCLUDES = -I $(PREPATH)SDL2/SDL_image
#SDLIMAGE2LIBS = -L $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0 -L $(PREPATH)SDL2/lib/libSDL2_image.so -L $(PREPATH)SDL2/lib/libSDL2_image-2.0.so.0.0.0 -L $(PREPATH)SDL2/lib/libSDL2_image.a
#LUAINCLUDES = -I $(PREPATH)lua/
#LUALIB = -L $(PREPATH)lua/ -llua -lm
#
#NLFINCLUDES = -I $(PREPATH)NLF_SCR/
#NLFLIB = -L $(PREPATH)BINARIES/libNFL.so
#
#
#build:
#	#$(CC) $(CFLAGS) auto_genareted_code.c $(NLFLIB)  $(SDLIMAGE2LIBS) $(LUALIB) $(LUAINCLUDES) $(SDL2INCLUDES) $(SDLIMAGE2INCLUDES) $(NLFINCLUDES) -o auto_genareted_code