#ifndef NLF_ERROR_TYPES_H
#define NLF_ERROR_TYPES_H

typedef enum NLF_ErrorOrigin
{
	NLF_ErrorOriginNome, NLF_ErrorOriginSDL, NLF_ErrorOriginC, NLF_ErrorOriginNLF
} NLF_ErrorOrigin;

typedef enum NLF_Error
{
/*
	This enum is from error.h but can not be there 'cause it'll cause a circular dependecy with NLF_OSservice.h
*/
	//errors
	NLF_ErrorNone,
		//SDL Problems
	NLF_ErrorSDLInitializer, NLF_ErrorSDLProblem, NLF_ErrorSDLImageInitializer, NLF_ErrorSDLImageLoad,
	NLF_ErrorSDLMutexCreationFail, NLF_ErrorSDLMutexLockFail, NLF_ErrorSDLCreatingFail, NLF_ErrorSDLToken,
		//C Problems
	NLF_ErrorFileNotFound, NLF_ErrorCantCreateFile, NLF_ErrorCrashReporCrash, NLF_ErrorInsufficientMemory, NLF_ErrorCToken,
		//NLF Problems
	NLF_ErrorCrash, NLF_ErrorBadArgument, NLF_ErrorMultiCharDef, NLF_ErrorNLFToken,
		//User Application Problems
	NLF_ErrorUserAppProblem,
		//error token
	NLF_ErrorToken,

	//warnings
		//SDL Warnings
	NLF_WarningSDLImageLoadProblem, NLF_WarningSDLToken,
		//User Application Warning
	NLF_WarningUserApp,
		//warning token
	NLF_WarningToken
} NLF_Error;

#endif