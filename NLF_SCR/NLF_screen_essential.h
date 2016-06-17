#ifndef NLF_SCREEN_ESSENTIAL_H
#define NLF_SCREEN_ESSENTIAL_H

/*CLIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/************/

/*EXTERNAL LIBRARIES*/
#include "SDL.h"
#include "SDL_image.h"
/*********************/

/*INTERNAL LIBRARIES*/
#include "NLF_error.h"
/********************/

/*ENUM AND TYPES*/
typedef enum NLF_Alignment
{
	NLF_AlignmentNone, NLF_AlignmentCenter, NLF_AlignmentRight, NLF_AlignmentLeft, NLF_AlignmentUp, NLF_AlignmentDown, NLF_AlignmentToken
}NLF_Alignment;

typedef NLF_Rect NLF_Camera;

typedef struct NLF_Screen
{
	SDL_Texture *scene;
	NLF_Rect dimetions; //an screen may the larger then the window
	unsigned short int position; //position of the windows between the other screens
	NLF_Alignment vAlign;
	NLF_Alignment hAlign;
	struct NLF_Screen *next;
} NLF_Screen;
/****************/

/*GLOBAL VARIABLES*/
SDL_Renderer *window_rederer; //the windows renderer
int screen_deltaTicks; //time since last update
/******************/

#endif
