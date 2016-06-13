/*BIG ISSUE!
The animation shall be thread safe! but it'll adds a very big overhead, look:
if NLF_animation_update() is running, and for some reason the user call a function to delete an animation
this 2 things will very probably be running on different threads, and my lead the update() function to a segmentation fault!
Think how to solve it...*/
#ifndef NLF_ANIMATION_H
#define NLF_ANIMATION_H
#define ANIVECTORSIZE 500

/*CLIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/************/

/*EXTERNAL LIBRARIES*/
#include "SDL.h"
/*********************/

/*INTERNAL LIBRARIES*/
#include "NLF_error.h"
#include "NLF_screen_essential.h"
/********************/

/*ENUM AND TYPES*/
typedef struct NLF_Animation
{
	struct spriteSheet **animations; //a type of actor may have lots of animations
	NLF_USInt aniAmnt;
	struct spriteSheet *currentAni; //each actor's type may have a lot of instances (the same reason why ID is **ID)
	NLF_USInt screenID; //the screen in which this animation is on
} NLF_Animation;


//for internal funcionality only
struct spriteSheet
{
	SDL_Texture *sheet;
	NLF_Rect *frames;
	NLF_USInt framesAmnt;
	NLF_Rect *currentFrame;
	int refreshTime;
	NLF_USInt way; //+1 means the animation runs normal -default-, -1 that it runs backward.
};

struct AniVector
{
	NLF_Animation **anivect;
	NLF_USInt fristEmpty;
	struct AniVector *next;
};
/****************/

/*GLOBAL VARIABLES*/

//for internal funcionality only
static struct AniVector *NLF_actorsAni;
static int secCounter;
/******************/

/*GLOBAL FUNCTIONS*/
void NLF_animation_init();
void NLF_animation_quit();
void NLF_animation_update();
#define NLF_animation_search(A) _NLF_animation_search_n_dell(A, NLF_False)
#define NLF_animation_remove(A) _NLF_animation_search_n_dell(A, NLF_True)
NLF_Animation* _NLF_animation_search_n_dell(NLF_USInt ID, NLF_bool dell);
NLF_Error NLF_animation_add_empty(NLF_USInt animationAmnt, NLF_USInt screenPosition, NLF_USInt *ID);
NLF_Error NLF_animation_add(NLF_USInt ID, const char *spriteAdress, NLF_USInt columns, NLF_USInt lines, NLF_USInt frameAmnt, NLF_USInt initialFrame, NLF_USInt way, NLF_USInt *aniID);
NLF_Error NLF_animation_toggle(NLF_USInt ID, NLF_USInt toAniID); //this function will very probably require many things not planned up to now, so it's not gonna be implemented now.
#define NLF_animation_toggle_way(A, B) NLF_animation_set_way(A, B, 0)
NLF_Error NLF_animation_set_way(NLF_USInt ID, NLF_USInt aniID, short int way);
#define NLF_animation_stop(A, B) _NLF_animation_stop_start(A, B, NLF_False)
#define NLF_animation_start(A, B) _NLF_animation_stop_start(A, B, NLF_True)
NLF_Error _NLF_animation_stop_start(NLF_USInt ID, NLF_USInt aniID, NLF_bool sos);
NLF_Error NLF_animation_change_screen(NLF_USInt ID, NLF_USInt screenPosition);
/******************/

/*LOCAL FUNTIONS*/
/****************/

#endif