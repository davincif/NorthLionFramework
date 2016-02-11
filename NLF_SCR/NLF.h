#ifndef NLF_H
#define NLF_H

/*Standard Includes*/
#include <stdio.h>
/*******************/

/*EXTERNAL LIBRARIES*/
#include "SDL.h"
/*********************/

/*NLF Internal Includes*/
#include "NLF_error.h"
#include "NLF_osservice.h"
#include "NLF_screen.h"
/***********************/

/*GLOBAL FUNCTIONS*/
void NLF_init();
void NLF_quit();
void NLF_game_start(void *user_parameter, void (*user_start_function)(void*));
/******************/

#endif
