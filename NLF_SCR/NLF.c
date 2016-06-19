#include "NLF.h"

/*GLOBAL FUNCTIONS*/
void NLF_init()
{
	char *str;

	printf("Initing SDL...");
	SDL_Init(SDL_INIT_EVERYTHING);
	printf("done\n");

	printf("Initing NLF...\n");

	api = NLF_APIError;

	printf("\tError module... ");
	NLF_error_init();
	printf("done\n");

	printf("\tScreen module... ");
	NLF_screen_init();
	printf("done\n");

	printf("\tAnimation module... ");
	NLF_animation_init();
	printf("done\n");

	printf("\tCreating Threads...");
	NLF_thread_screen = SDL_CreateThread(NLF_screen_run, "Screen", (void*) NULL);
	if(NLF_thread_screen == NULL)
	{
		str = SDL_GetError();
		printf("Could not create screen thread NLF_thread_screen");
		printf("SDL Error: %s", str);
		NLF_error_make_file_crash_report(NLF_ErrorSDLThreadCreationFail, "Could not create screen thread NLF_thread_screen", "SDL Error: ", str, NULL);
		exit(NLF_ErrorSDLThreadCreationFail);
	}
	NLF_thread_physics = NULL;
	NLF_thread_event_watcher = NULL;
	NLF_thread_sound_player = NULL;
	printf("done\n");

	printf("NLF Initialized!\n\n");
}


void NLF_quit()
{
/*
	This fuction will:
		give the signal to quit the game.
*/
	NLF_signal_quit = NLF_True;
	//Need to find a better to do this. eg.: if the thread do not end, kill it.
	SDL_WaitThread(NLF_thread_screen, NULL);
	SDL_WaitThread(NLF_thread_physics, NULL);
	SDL_WaitThread(NLF_thread_event_watcher, NULL);
	SDL_WaitThread(NLF_thread_sound_player, NULL);
	NLF_thread_screen = NULL;
	NLF_thread_physics = NULL;
	NLF_thread_event_watcher = NULL;
	NLF_thread_sound_player = NULL;

	printf("Quiting NLF...\n");

	printf("\tError module... ");
	NLF_error_quit();
	printf("done\n");

	printf("\tScreen module... ");
	NLF_screen_quit();
	printf("done\n");

	printf("\tAnimation module... ");
	NLF_animation_quit();
	printf("done\n");
	
	printf("NLF finished!\n\n");
	SDL_Quit();
}

void NLF_pause()
{
/*
	This fuction will:
	give the signal to pause the game.
*/
	NLF_signal_pause = NLF_True;
}

void NLF_continue()
{
/*
	This fuction will:
		give the signal to unpause the game.
*/
	NLF_signal_pause = NLF_False;
}

NLF_bool NLF_set_api(NLF_API eipiai)
{
	NLF_Error e;

	if(api != NLF_APIError)
	{
		printf("Error: the API can be set only once!\n");
		e = NLF_False;
	}else if(eipiai >= NLF_APIToken || eipiai <= NLF_APIError)
	{
		printf("Error: API not recognized!\n");
		e = NLF_False;
	}else{
		printf("API set\n");
		api = eipiai;
		e = NLF_True;
	}

	return e;
}
/******************/