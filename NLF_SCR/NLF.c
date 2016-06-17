#include "NLF.h"

void NLF_init()
{
	printf("Initing SDL...");
	SDL_Init(SDL_INIT_EVERYTHING);
	printf("done\n");

	printf("Initing NLF...\n");
	NLF_thread_screen = NULL;
	NLF_thread_physics = NULL;
	NLF_thread_event_watcher = NULL;
	NLF_thread_sound_player = NULL;

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

void NLF_game_start(void *user_parameter)
{
/*
*/
	printf("Starting game...\n");
	printf("Game started\n\n");
}