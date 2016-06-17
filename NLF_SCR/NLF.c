#include "NLF.h"

/*GLOBAL FUNCTIONS*/
void NLF_init()
{
	printf("Initing SDL...");
	SDL_Init(SDL_INIT_EVERYTHING);
	printf("done\n");
	char *str;

	printf("Initing NLF...\n");

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
/******************/

/*LOCAL FUNTIONS*/
static void _NLF_quit()
{
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
/****************/