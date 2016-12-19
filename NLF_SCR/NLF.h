#ifndef NLF_H
#define NLF_H

/*Standard Includes*/
#include <stdio.h>
#include <omp.h>
/*******************/

/*EXTERNAL LIBRARIES*/
#include "SDL.h"
/*********************/

/*NLF Internal Includes*/
#include "NLF_error.h"
#include "NLF_osservice.h"
#include "NLF_screen.h"
/***********************/

/*ENUM AND TYPES*/
typedef enum NLF_API
{
	NLF_APIError, NLF_APIC, NLF_APIlua, NLF_APIToken
} NLF_API;
/****************/

/*GLOBAL VARIABLES*/
static NLF_API api;
//THREADS
SDL_Thread *NLF_thread_screen;
SDL_Thread *NLF_thread_physics;
SDL_Thread *NLF_thread_event_watcher;
SDL_Thread *NLF_thread_sound_player;
/******************/

/*GLOBAL FUNCTIONS*/
void NLF_init();
void NLF_quit();
void NLF_pause();
void NLF_continue();
void NLF_game_start();
NLF_bool NLF_set_api(NLF_API eipiai);
/******************/

/*LOCAL FUNTIONS*/
/****************/

#endif
