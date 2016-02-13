#include <stdio.h>
#include <stdlib.h>
#include <NLF.h>

void startGame()
{
	printf("peraê vissi, tamo arrumando ar bagaça aqui, rlx ;)\n");
	printf("Pronto, funfando\n\n");
}

int main(int arg, char *argc[])
{
	unsigned short int r;
	int xd = 0;

	NLF_init();

	NLF_game_start(NULL, startGame);

	r = NLF_screen_add(1, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	r = NLF_screen_add(1, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	r = NLF_screen_add(2, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	r = NLF_screen_add(5, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	r = NLF_screen_add(5, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	r = NLF_screen_add(6, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	r = NLF_screen_add(7, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	//TESTE_RUN_LIST();
	//TESTE_RUN_LIST - was a funtion that only run the list and print the positions, only to test the code

	NLF_screen_remove(6);
	//TESTE_RUN_LIST();
	NLF_screen_remove(1);
	//TESTE_RUN_LIST();
	NLF_screen_remove(5);
	//TESTE_RUN_LIST();
	NLF_screen_remove(5);
	//TESTE_RUN_LIST();
	NLF_screen_remove(-1);
	//TESTE_RUN_LIST();

	NLF_screen_print();

	NLF_quit();

	return 0;
}