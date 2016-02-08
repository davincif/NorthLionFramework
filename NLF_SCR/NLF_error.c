#include "NLF_error.h"

/*GLOBAL FUNCTIONS*/
void NLF_error_init()
{
	filesMutex = SDL_CreateMutex();
	flagsMutex = SDL_CreateMutex();
	if(filesMutex == NULL || flagsMutex == NULL)
	{
		NLF_error_make_file_crash_report(NLF_ErrorSDLMutexCreationFail, "Could not initialize the Error Module", SDL_GetError()), NULL;
		exit(NLF_ErrorSDLMutexCreationFail);
	}

	errormsg = (char**) malloc(sizeof(char*));
	if(errormsg == NULL)
	{
		NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "could not allocate memory to errormsg", NULL);
		exit(NLF_ErrorInsufficientMemory);
	}
	*errormsg = NULL;
	fileName = NULL;
	errorFlag = NLF_ErrorNone;
	/*if(filePath == NULL)
		NLF_error_set_path(NULL);*/
}

void NLF_error_quit()
{
	time_t know, timer;
	int status1 = 1, status2 = 1;
	int i;

	//check if anyone is using the mutex and give it a time
	time(&timer);
	do{
		if(status1 != 0)
			status1 = SDL_TryLockMutex(filesMutex);
		if(status2 != 0)
			status2 = SDL_TryLockMutex(flagsMutex);
		time(&know);
	}while((status1 != 0 || status2 != 0) && (difftime(know,timer < 3.5)));

	//after 3.5 seconds destroy the mutex with or without the lock
	SDL_DestroyMutex(filesMutex);
		filesMutex = NULL;
	SDL_DestroyMutex(flagsMutex);
		flagsMutex = NULL;

	if(fileName != NULL)
		free(fileName);
	fileName = NULL;

	if(errormsg != NULL)
	{
		for(i = 0; errormsg[i] != NULL; i++)
			free(errormsg[i]);
		free(errormsg);
	}
	errormsg = NULL;

	if(filePath != NULL)
		free(filePath);
	filePath = NULL;

	errorFlag = NLF_ErrorNone;
}

NLF_bool NLF_error_or_warning(NLF_Error e)
{
/*
	warning == NLF_True
	error == NLF_False
	PS.: is e == NLF_ErrorNone, returns as warning
*/
	if(e > NLF_ErrorToken && e != NLF_ErrorNone)
		return NLF_True;
	else
		return NLF_False;
}

NLF_ErrorOrigin NLF_error_origin(NLF_Error e)
{
/*
	Returns from where the error came from
*/
	NLF_ErrorOrigin ero;
	if(e == NLF_ErrorNone)
		ero = NLF_ErrorNone;
	else if(e  <= NLF_ErrorSDLToken)
		ero = NLF_ErrorOriginSDL;
	else if(e <= NLF_ErrorCToken)
		ero = NLF_ErrorOriginC;
	else if(e <= NLF_ErrorNLFToken)
		ero = NLF_ErrorOriginNLF;
	else if(e == NLF_ErrorUserAppProblem)
		ero = NLF_ErrorUserAppProblem;
	else if(e <= NLF_WarningSDLToken)
		ero = NLF_WarningSDLToken;
	else if(e == NLF_WarningUserApp)
		ero = NLF_WarningUserApp;
	else
		ero = NLF_ErrorNone -1; //this case shall never happen
	//as long as the warning error will be creted this function will need upgrade

	return ero;
}

void NLF_error_make_file_crash_report(NLF_Error e, char *msg, ...)
{
/*
	arguments:
		e - error flag
		msg - a constume message from the game maker to the player
		NULL - This function should always end with NULL
	This fuction will:
		check if the standard directory from crashs files existe, and create it if necessary.
		then will create insed the directory above the file with the name of the standard file path plus the time plus "_CrashReportFile".
		within the file, this funtion will left a message witl the name of the given flag and message.
*/
	char *aux;
	int i;
	FILE *f;
	va_list lst;

	if((filesMutex == NULL || filesMutex == NULL) || (SDL_LockMutex(filesMutex) == 0 && SDL_LockMutex(flagsMutex) == 0))
	{
		//enter here if lock the mutex of if they were not created yeat
		f = create_report_file(filePath, "CrashReportFile");

		fprintf(f, "eh... sorry, i seems like a crash has occurred in your system.\n\n");

		write_error(f, e, NLF_True);

		fprintf(f, "Messages:\n");
		aux = msg;
		va_start(lst, msg);
		while(aux != NULL)
		{
			fprintf(f, "\t- %s\n", aux);
			aux = va_arg(lst, char*);
		}
		va_end(lst);

		if(errorFlag != NLF_ErrorNone)
		{
			fprintf(f, "\n\nAlso there were an error not treated:\n");
			write_error(f, errorFlag, NLF_False);
			fprintf(f, "Messages:\n");
			for(i = 0; errormsg[i] != NULL; i++)
				fprintf(f, "\t- %s\n", errormsg[i]);
		}

		fclose(f);
		SDL_UnlockMutex(filesMutex);
		SDL_UnlockMutex(flagsMutex);
	}else{
		//still do know how to treat that
	}
}
/******************/




/*LOCAL FUNTIONS*/
static void write_error(FILE *f, NLF_Error e, NLF_bool showOStype)
{
/*
	write in the given fale "*f" a line saying what is the error flag given "e" and if it's a error or a warning
	it'll also write the internal error flag, if it exists
	if showOStype if NLF_true it'll also show the OS type and its bits, and will not show otherwhise.
*/
	if(showOStype == NLF_True)
	{
		switch(NLF_OSS_type())
		{
			case NLF_SystemTypeUndefenied:
				fprintf(f, "OS: undefenied %dbits\n", NLF_OSS_type());
			break;

			case NLF_SystemTypeWindows:
				fprintf(f, "OS: windows %dbits\n", NLF_OSS_type());
			break;

			case NLF_SystemTypeUnix:
				fprintf(f, "OS: unix %dbits\n", NLF_OSS_type());
			break;

			case NLF_SystemTypeApple:
				fprintf(f, "OS: apple %dbits\n", NLF_OSS_type());
			break;

			case NLF_SystemTypeIphoneSimulatior:
				fprintf(f, "OS: iphone simulatior %dbits\n", NLF_OSS_type());
			break;

			case NLF_SystemTypeIphone:
				fprintf(f, "OS: iphone %dbits\n", NLF_OSS_type());
			break;

			case NLF_SystemTypeOSX:
				fprintf(f, "OS: OSX %dbits\n", NLF_OSS_type());
			break;

			case NLF_SystemTypeLinux:
				fprintf(f, "OS: linux %dbits\n", NLF_OSS_type());
			break;

			default:
				fprintf(f, "OS: Ehh... that's frustrating but, I have no idea what the hell the system is =| sorry ='(");
		}
	}

	switch(e)
	{
		case NLF_ErrorSDLInitializer:
			fprintf(f, "flag: NLF_ErrorSDLInitializer\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorSDLProblem:
			fprintf(f, "flag: NLF_ErrorSDLProblem\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorSDLImageInitializer:
			fprintf(f, "flag: NLF_ErrorSDLImageInitializer\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorSDLImageLoad:
			fprintf(f, "flag: NLF_ErrorSDLImageLoad\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorSDLMutexCreationFail:
			fprintf(f, "flag: NLF_ErrorSDLMutexCreationFail\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorSDLMutexLockFail:
			fprintf(f, "flag: NLF_ErrorSDLMutexLockFail\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorSDLCreatingFail:
			fprintf(f, "flag: NLF_ErrorSDLCreatingFail\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorFileNotFound:
			fprintf(f, "flag: NLF_ErrorFileNotFound\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorCantCreateFile:
			fprintf(f, "flag: NLF_ErrorCantCreateFile\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorCrashReporCrash:
			fprintf(f, "flag: NLF_ErrorCrashReporCrash\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorInsufficientMemory:
			fprintf(f, "flag: NLF_ErrorInsufficientMemory\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;
		
		case NLF_ErrorCrash:
			fprintf(f, "flag: NLF_ErrorCrash\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorBadArgument:
			fprintf(f, "flag: NLF_ErrorBadArgument\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorMultiCharDef:
			fprintf(f, "flag: NLF_ErrorMultiCharDef\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_ErrorUserAppProblem:
			fprintf(f, "flag: NLF_ErrorUserAppProblem\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_WarningSDLImageLoadProblem:
			fprintf(f, "flag: NLF_WarningSDLImageLoadProblem\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		case NLF_WarningUserApp:
			fprintf(f, "flag: NLF_WarningUserApp\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
		break;

		default:
			fprintf(f, "flag: No flag detected\ntype: %s\n", (NLF_error_or_warning(e) == NLF_False ? "error" : "warning"));
	}

	switch(NLF_error_origin(e))
	{
		case NLF_ErrorOriginSDL:
			fprintf(f,"origin: NLF_ErrorOriginSDL\n");
		break;

		case NLF_ErrorOriginC:
			fprintf(f,"origin: NLF_ErrorOriginC\n");
		break;

		case NLF_ErrorOriginNLF:
			fprintf(f,"origin: NLF_ErrorOriginNLF\n");
		break;

		case NLF_ErrorOriginNome:
			fprintf(f,"origin: NLF_ErrorOriginNome... wait, what? Someting is not right...\n");
		break;

		default:
			fprintf(f,"origin: ehh... i guess someone made something very very wrong here...\n");

		//as long as the warning error will be creted this function will need upgrade just like NLF_error_origin
	}
}

static FILE* create_report_file(const char *pathToFile, const char *fileType)
{
/*
	craete a crash file with int the path given by "pathToFile" with the following pattern:
		.pathToFile/currentetime&&data_fileType
	return a pointer to the file created
*/
	time_t currentTime;
	FILE *f;
	short int iaux, i;
	char *aux;

	/*Cheking directory exists*/
	if(pathToFile != NULL)
	{
		if(NLF_OSS_check_directory_existence(filePath) == NLF_False)
		{
			if(NLF_OSS_create_directory(filePath, NULL) != NLF_ErrorNone)
			{
				//still do not thought how to handle that...
			}
		}
	}

	/*Making the file name*/
	time(&currentTime);
	aux = ctime(&currentTime);
	if(fileName != NULL)
		free(fileName);
	fileName = (char*) malloc(sizeof(char)*((pathToFile != NULL ? strlen(pathToFile) : 0)+strlen(aux)+strlen(fileType)+2));
	if(pathToFile != NULL)
	{
		strcpy(fileName, pathToFile);
		strcat(fileName, aux);
	}else{
		strcpy(fileName, aux);
	}
	iaux = strlen(fileName) - 1;
	if(fileName[iaux] == '\n')
		fileName[iaux] = '\0';
	strcat(fileName, "_");
	strcat(fileName, fileType);

	/*removing spaces*/
	for(i = 0; fileName[i] != '\0'; i++)
		if(fileName[i] == ' ')
			fileName[i] = '_';

	f = fopen(fileName, "w+");
	if(f == NULL)
	{
		//still do not thought how to handle that...
	}

	return f;
}
/***************/