#ifndef NLF_ERROR_H
#define NLF_ERROR_H

/*CLIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
/************/

/*EXTERNAL LIBRARIES*/
#include "SDL_mutex.h"
/*********************/

/*INTERNAL LIBRARIES*/
#include "NLF_osservice.h"
#include "NLF_error_types.h"
/********************/

/*ENUM AND TYPES*/
/****************/

/*GLOBAL VARIABLES*/

//for internal funcionality only
static SDL_mutex *filesMutex; //lock to use fileName or filePath
static SDL_mutex *flagsMutex; //lock to use the flags of error
static char *fileName;
static char *filePath;
static NLF_Error errorFlag;
static char **errormsg;
/******************/

/*GLOBAL FUNCTIONS*/
void NLF_error_init();
void NLF_error_quit();
void NLF_error_make_file_crash_report(NLF_Error e, char *msg, ...);
NLF_ErrorOrigin NLF_error_origin(NLF_Error e);
NLF_bool NLF_error_or_warning(NLF_Error e);
NLF_Error NLF_error_set_path(const char *stdErrorPath);
NLF_bool NLF_error_set_flag(NLF_Error e, int args, char *msg, ...);
void NLF_error_clean_flag();
NLF_Error NLF_error_get_flag();
/******************/

/*LOCAL FUNTIONS*/
static void write_error(FILE *f, NLF_Error e, NLF_bool showOStype);
static FILE* create_report_file(const char *pathToFile, const char *fileType);
/****************/

#endif