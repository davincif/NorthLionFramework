#include <stdio.h>
#include <stdlib.h>
#include <NLF.h>

void startGame()
{
	printf("peraê vissi, tamo arrumando ar bagaça aqui, rlx ;)\n");
}

int main(int arg, char *argc[])
{

	NLF_init();

	NLF_game_start(NULL, startGame);

	NLF_quit();
}