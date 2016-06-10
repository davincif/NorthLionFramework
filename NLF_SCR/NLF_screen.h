#ifndef NLF_SCREEN_H
#define NLF_SCREEN_H

/*CLIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
/************/

/*EXTERNAL LIBRARIES*/
#include "SDL.h"
#include "SDL_image.h"
/*********************/

/*INTERNAL LIBRARIES*/
#include "NLF_error.h"
#include "NLF_animation.h"
#include "NLF_screen_essential.h"
/********************/

/*GLOBAL VARIABLES*/

//for internal funcionality only
static NLF_Camera camera; //defines the global coodinate system, wich the (0,0), width and height are the same of game window
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
static NLF_USInt currentFPS; //the FPS the system is operating just now
static NLF_USInt idealFPS; //the FPS that the system should be operating
static NLF_USInt estimatedFPS; //the FPS the the system estimante to reach.
/******************/

/*GLOBAL FUNCTIONS*/
void NLF_screen_init();
void NLF_screen_quit();
void NLF_scree_run();
NLF_USInt NLF_screen_add(NLF_USInt sugestPosition, NLF_USInt x, NLF_USInt y, NLF_USInt w, NLF_USInt h, NLF_Alignment vAlign, NLF_Alignment hAlign, NLF_bool isStatic);
void NLF_screen_remove(short int position);
void NLF_screen_print();
void NLF_camera_move(int plusx, int plusy);
void NLF_camera_resize(int x, int y, int w, int h);
void NLF_camera_add_size(int plusw, int plush);
void NLF_camera_setPosition(int newx, int newy);
void NLF_camera_realign(NLF_Alignment vAlignment, NLF_Alignment hAlignment);
void NLF_camera_reset();
/******************/

/*LOCAL FUNTIONS*/
static void NLF_screen_destroy();
/****************/

#endif