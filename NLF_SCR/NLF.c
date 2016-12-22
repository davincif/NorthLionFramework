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

	printf("NLF Initialized!\n\n");
}


void NLF_quit()
{
/*
	This fuction will:
		give the signal to quit the game.
*/
	NLF_signal_quit = NLF_True;

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

void NLF_game_start()
{
	printf("\t\tMax threads to parallel region %d\n", omp_get_max_threads());
	printf("\t\tMaximum threads available %d\n", omp_get_thread_limit());
	printf("\t\tNumber of processors %d\n", omp_get_num_procs());

	#pragma omp parallel num_threads(4)
	{
		#pragma omp single
			NLF_screen_run();
		//physics
		//watcher
		//sound
	}
}
/******************/