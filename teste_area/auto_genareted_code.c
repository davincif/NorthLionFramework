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
	NLF_init();

	NLF_USInt id = 9, sid = 10, sid2 = 11;
	NLF_Error e;
	e = NLF_animation_add_empty(1, &id, 1);
	printf("e = %d\n", e);
	printf("id = %d\n", id);
	e = NLF_animation_add(id, "images/runningcat.png", 2, 4, 8, 0, 1, &sid);
	printf("e = %d\n", e);
	printf("sid = %d\n", sid);
	e = NLF_animation_add(id, "images/gatin.jpg", 3, 4, 10, 0, 1, &sid2);
	printf("e = %d\n", e);
	printf("sid2 = %d\n", sid2);
	e = NLF_animation_toggle_way(id, 0);
	printf("\ne = %d\n", e);
	printf("NLF_animation_start\n");
	e = NLF_animation_start(id, 0);
	printf("NLF_animation_remove\n");
	NLF_animation_remove(id);
	printf("Animation %d removed\n", id);

	NLF_quit();

	return 0;
}