#ifndef NLF_SCREEN_H
#define NLF_SCREEN_H

/*CLIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
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
	unsigned short int x;
	unsigned short int y;
	unsigned short int w;
	unsigned short int h;
	unsigned short int position;
	NLF_Alignment vAlign;
	NLF_Alignment hAlign;
	struct NLF_Screen *next;
} NLF_Screen;
/****************/

/*GLOBAL VARIABLES*/
NLF_Camera camera;

//for internal funcionality only
static SDL_Renderer *window_rederer;
static SDL_Window *window;
static SDL_RendererInfo rendererInfo;
static NLF_bool rendererInfoUnknown; //in case it was impossible to get information about renderer, set to NLF_True
static SDL_RendererInfo rendererDriverInfo;
static NLF_bool rendererDriverInfoUnkown; //in case it was impossible to get information about renderer driver, set to NLF_True
static NLF_Screen *screens;
static NLF_bool jpgFlag; //if image format was loaded and can be used
static NLF_bool pngFlag;
static NLF_bool tifFlag;
static int qtdDisplay; //the amount of displays in the system
static int displayInUse; //the number of the display we're using now
static SDL_DisplayMode videoMode; //the video mode of the display we're using now
static NLF_bool displayInfoUnknown; //in case it was impossible to define the display mode, set to NLF_True
static unsigned short int currentFPS; //the FPS the system is operating just now
static unsigned short int idealFPS; //the FPS that the system should be operating
/******************/

/*GLOBAL FUNCTIONS*/
void NLF_screen_init();
void NLF_screen_quit();
unsigned short int NLF_screen_add(unsigned short int sugestPosition, unsigned short int x, unsigned short int y, unsigned short int w, unsigned short int h, NLF_Alignment vAlign, NLF_Alignment hAlign, NLF_bool isStatic);
void NLF_screen_remove(short int position);
/******************/

/*LOCAL FUNTIONS*/
static void NLF_screen_destroy();
/****************/

#endif