#include "NLF.h"

void NLF_init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
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

void NLF_game_start(void *user_parameter, void (*user_start_function)(void*))
{
	printf("Starting game...\n");
	user_start_function(user_parameter);
	printf("Game started\n\n");
}