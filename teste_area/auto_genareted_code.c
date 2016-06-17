#include <stdio.h>
#include <stdlib.h>
#include <NLF.h>

int main(int arg, char *argc[])
{
	NLF_init();

	NLF_screen_add(1, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	NLF_screen_add(1, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	NLF_screen_add(2, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	NLF_screen_add(5, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	NLF_screen_add(5, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	NLF_screen_add(6, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	NLF_screen_add(7, 0, 0, 0, 0, NLF_AlignmentCenter, NLF_AlignmentCenter, NLF_False);
	NLF_screen_remove(6);
	NLF_screen_remove(1);
	NLF_screen_remove(5);
	NLF_screen_remove(5);
	NLF_screen_remove(-1);

	NLF_USInt id = 9, sid = 10, sid2 = 11;
	NLF_Error e;
	e = NLF_animation_add_empty(1, 1, &id);
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

	SDL_Delay(2000);
	NLF_quit();
	SDL_WaitThread(NLF_thread_screen, NULL);

	return 0;
}