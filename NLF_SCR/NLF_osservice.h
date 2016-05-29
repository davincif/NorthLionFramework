#ifndef NLF_OSSERVICE_H
#define NLF_OSSERVICE_H

/*CLIBRARIES*/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
/************/

/*MARCOS*/
#if defined(__WIN64__) || defined(__WIN64) || defined(WIN64)
	#define NLF_OS_WINDOWS
	#define NLF_OS_64BITS
#elif defined(__WIN32__) || defined(__WIN32) || defined(WIN32)
	#define NLF_OS_WINDOWS
	#define NLF_OS_32BITS
#elif defined(unix) || defined(__unix__) || defined(__unix)
	#define NLF_OS_UNIX
	#if defined(__APPLE__)
		#define NLF_OS_APPLE
		#define NLF_OS_BITS_UNIDENTIFIED
		#include <TargetConditionals.h>
		#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
			#define NLF_OS_SIMULATOR
		#elif TARGET_OS_IPHONE
			#define NLF_OS_IPHONE
		#else
			#define NLF_OS_OSX
		#endif
	#elif defined(__linux__) || defined(__linux) || defined(linux)
		#define NLF_OS_LINUX
		#if defined(i386) || defined(__i386) || defined(__i386__)
			#define NLF_OS_32BITS
		#elif defined(amd64)  || defined(__amd64) || defined(__amd64__)
			#define NLF_OS_64BITS
		#endif
	#elif defined(__FreeBSD__) || defined(__FreeBSD) || defined(FreeBSD)
		#define NLF_OS_FREEBSD
		#if defined(i386) || defined(__i386) || defined(__i386__)
			#define NLF_OS_32BITS
		#elif defined(amd64)  || defined(__amd64) || defined(__amd64__)
			#define NLF_OS_64BITS
		#endif
	#endif
#else
	#define NLF_OS_UNIDENTIFIED
	#define NLF_OS_BITS_UNIDENTIFIED
#endif
/********/

/*EXTERNAL LIBRARIES*/
#include "SDL.h"
/*********************/

/*INTERNAL LIBRARIES*/
#include "NLF_error_types.h"
/********************/

/*ENUM AND TYPES*/
typedef SDL_Rect NLF_Rect;
typedef SDL_Point NLF_Point;

typedef enum NLF_bool
{
	NLF_False = 0, NLF_True = 1
} NLF_bool;

typedef enum NLF_SystemType
{
/*
	this enum identify wall kind of system that NLF can handle
*/
	NLF_SystemTypeUndefenied, NLF_SystemTypeWindows, NLF_SystemTypeUnix, NLF_SystemTypeApple, NLF_SystemTypeIphoneSimulatior,
	NLF_SystemTypeIphone, NLF_SystemTypeOSX, NLF_SystemTypeLinux, NLF_SystemTypeFreeBSD
} NLF_SystemType;
/****************/

/*EXTRUTURED TYPES*/
/******************/

/*GLOBAL VARIABLES*/
/******************/

/*GLOBAL FUNCTIONS*/
NLF_bool NLF_OSS_check_directory_existence(const char *pathname);
NLF_Error NLF_OSS_create_directory(const char *directory, const char *permission);
NLF_SystemType NLF_OSS_type();
unsigned short int NLF_OSS_bits();
NLF_bool NLF_OSS_check_file_existence(const char *pathname);
/******************/


#endif