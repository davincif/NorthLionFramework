#!/bin/bash

DS="$" #Dolar Sign

printf "#the compiler
CC = gcc

#the standart ompilation flags of the project
CFLAGS = -O3 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-implicit-function-declaration

#path to the folder's root, where the holy not build framework is. Relate to this Makefile
PREPATH = ..\n" > Makefile

printf "
#Path to the SDL, SDL_image and Lua includes and libs
SDL2INCLUDES = -I $DS(PREPATH)/SDL2/include
SDL2LIBS =" >> Makefile

for f in ../SDL2/lib/*.a
do
	if ! [[ $f == *"_image"* ]]
	then
		printf " $f" >> Makefile
	fi
done
for f in ../SDL2/lib/*.so.*
do
	if ! [[ $f == *"_image"* ]]
	then
		printf " $f" >> Makefile
	fi
done

printf "
SDLIMAGE2INCLUDES = -I $DS(PREPATH)/SDL2/SDL_image
SDLIMAGE2LIBS =" >> Makefile

for f in ../SDL2/lib/*.a
do
	if [[ $f == *"_image"* ]]
	then
		printf " $f" >> Makefile
	fi
done
for f in ../SDL2/lib/*.so.*
do
	if [[ $f == *"_image"* ]]
	then
		printf " $f" >> Makefile
	fi
done

printf "
#LUAINCLUDES = -I $DS(PREPATH)/lua/
#LUALIB = $DS(PREPATH)/lua/ -llua -lm" >> Makefile

printf "\n
#Where to put the compiled program
COMPILEPATH = $DS(PREPATH)/BINARIES/\n" >> Makefile

cat make_config >> Makefile