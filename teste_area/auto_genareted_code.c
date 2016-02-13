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

	printf("criando %d\n", xd++);
	r = NLF_screen_add(1, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	printf("r = %d\n", r);

	printf("criando %d\n", xd++);
	r = NLF_screen_add(1, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	printf("r = %d\n", r);

	printf("criando %d\n", xd++);
	r = NLF_screen_add(2, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	printf("r = %d\n", r);

	printf("criando %d\n", xd++);
	r = NLF_screen_add(4, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	printf("r = %d\n", r);
	

	NLF_quit();

	return 0;
}