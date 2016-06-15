#include "NLF_animation.h"


/*GLOBAL FUNCTIONS*/
void NLF_animation_init()
{
	secCounter = 0;

	actorsAniMutex = SDL_CreateMutex();
	if (actorsAniMutex == NULL)
	{
		printf("Could nOt create Animation mutex\n");
		printf("\tAborting\n");
		NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not create Animation mutex", SDL_GetError(), NULL);
		return NLF_ErrorInsufficientMemory;
	}

	NLF_actorsAni = (struct AniVector*) malloc(sizeof(struct AniVector));
	if(NLF_actorsAni == NULL)
	{
		printf("\tCould not allocate animation vector\n");
		printf("\tAborting\n");
		NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate animation vector", NULL);
		exit(NLF_ErrorInsufficientMemory);
	}
	NLF_actorsAni->next = NULL;
	NLF_actorsAni->fristEmpty = 0;

	NLF_actorsAni->anivect = (NLF_Animation**) calloc(ANIVECTORSIZE, sizeof(NLF_Animation*));
	if(NLF_actorsAni->anivect == NULL)
	{
		printf("\tCould not allocate animation vector\n");
		printf("\tAborting\n");
		NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate animation vector", NULL);
		exit(NLF_ErrorInsufficientMemory);
	}
}

void NLF_animation_quit()
{
	NLF_USInt i, i2;
	struct AniVector *auxvect;

	SDL_DestroyMutex(actorsAniMutex);
	actorsAniMutex = NULL;

	while(NLF_actorsAni != NULL)
	{
		auxvect = NLF_actorsAni;
		for(i = 0; i < ANIVECTORSIZE; i++)
		{
			if(auxvect->anivect[i] != NULL)
			{
				for(i2 = 0; i2 < auxvect->anivect[i]->aniAmnt; i2++)
				{
					if(auxvect->anivect[i]->animations[i2] != NULL)
					{
						SDL_DestroyTexture(auxvect->anivect[i]->animations[i2]->sheet);
						free(auxvect->anivect[i]->animations[i2]->frames);
						free(auxvect->anivect[i]->animations[i2]);
					}
				}
				free(auxvect->anivect[i]->animations);
			}
		}
		NLF_actorsAni = auxvect->next;
		free(auxvect);
	}
}

NLF_Error NLF_animation_add_empty(NLF_USInt animationAmnt, NLF_USInt screenPosition, NLF_USInt *ID)
{
/*
arguments:
		animationAmnt - how many sprite-sheet animation there are in this animation.
		screenPosition - what is the screen where this animation shall be printed.
		ID - a integer to be filled with the new animation ID.
	This fuction will:
		Alloc a new Animation and it's sprites. But leave all things setted to 0.
		To maintain the correctness this function will also treat the interlocked list, updating pointer and counters.
*/
	NLF_Error e = NLF_ErrorNone;
	struct AniVector *auxvect;
	NLF_Animation *aniaux;
	NLF_USInt i;

	if(SDL_LockMutex(actorsAniMutex) == 0)
	{
		auxvect = NLF_actorsAni;
		while(auxvect->fristEmpty == ANIVECTORSIZE)
		{
			if(auxvect->next == NULL)
			{
				auxvect->next = (struct AniVector*) malloc(sizeof(struct AniVector));
				if(auxvect->next == NULL)
				{
					printf("Could not allocate new animation");
					printf("\tAborting\n");
					NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate new animation", NULL);
					SDL_UnlockMutex(actorsAniMutex);
					return NLF_ErrorInsufficientMemory;
				}
				auxvect = auxvect->next;

				auxvect->fristEmpty = 0;
				auxvect->next = NULL;
				auxvect->anivect = (NLF_Animation**) calloc(ANIVECTORSIZE, sizeof(NLF_Animation*));
				if(auxvect->anivect == NULL)
				{
					printf("\tCould not allocate animation vector\n");
					printf("\tAborting\n");
					NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate animation vector", NULL);
					SDL_UnlockMutex(actorsAniMutex);
					return NLF_ErrorInsufficientMemory;
				}
			}else{
				auxvect = auxvect->next;
			}
		}

		//allocate the new NLF_Animaiton
		aniaux = (NLF_Animation*) malloc(sizeof(NLF_Animation));
		if(aniaux == NULL)
		{
			printf("Could not allocate new animation");
			printf("\tAborting\n");
			NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate new animation", NULL);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorInsufficientMemory;
		}
		*ID = auxvect->fristEmpty;
		aniaux->aniAmnt = animationAmnt;
		aniaux->currentAni = NULL;
		aniaux->screenID = screenPosition;
		auxvect->anivect[auxvect->fristEmpty] = aniaux;

		//actualize auxvect->fristEmpty
		for(auxvect->fristEmpty++; auxvect->anivect[auxvect->fristEmpty] == NULL && auxvect->fristEmpty < ANIVECTORSIZE; auxvect->fristEmpty++);
		
		//allocing animaitons
		aniaux->animations = (struct spriteSheet**) calloc(aniaux->aniAmnt, sizeof(struct spriteSheet*));
		if(aniaux->animations == NULL)
		{
			printf("Could not allocate new sprites");
			printf("\tAborting\n");
			NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate new sprites", NULL);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorInsufficientMemory;
		}

		SDL_UnlockMutex(actorsAniMutex);
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
	}

	return e;
}

NLF_Error NLF_animation_add(NLF_USInt ID, const char *spriteAdress, NLF_USInt columns, NLF_USInt lines, NLF_USInt frameAmnt, NLF_USInt initialFrame, NLF_USInt way, NLF_USInt *aniID)
{
/*
	arguments:
		ID - The ID of the animation where the new sprite belongs.
		spriteAdress - the relative adress of the image to be loaded. eg.: myFold/images/mysprite.png
		columns - how many columns sprite there are in the animation image.
		lines - how many lines sprite there are in the animation image.
		frameAmnt - the total amount of frame in the animation image.
		way - +1 means the animation runs normal -default-, -1 that it runs backward. if any other value of 'way' will be interpreted as default.
		aniID - an int pointer to be filled with the ID of the new animation.
	This fuction will:
		create a new sprite sheet inside the given animation. And:
		returns NLF_ErrorNone - if everything goes right;
		returns NLF_ErrorAnimatinNotFound + an error flag if the animation the de 'ID' given doesn't exist;
		returns NLF_ErrorFileNotFound if the file 'spriteAdress' doesn't exist;
		returns NLF_ErrorSDLImageLoad + an error flag if the IMG_Load() function fail;
		returns NLF_ErrorSDLCreatingFail + an error flag if the NLF_OSS_check_file_existence() function fail.
		returns NLF_ErrorMultiDef + an error if the animation required is aready full of sprites
*/
	NLF_Animation *aniaux;
	char str[100];
	SDL_Surface *loadedimg;
	SDL_Texture *tempTexture;
	NLF_Rect *frames;
	NLF_USInt putIn, i;
	int w, h;

	//check if the animation with de given ID existe
	aniaux = NLF_animation_search(ID);
	if(aniaux == NULL)
	{
		sprintf(str, "Animation ID=%d not found\n", ID);
		printf("%s", str);
		NLF_error_set_flag(NLF_ErrorAnimatinNotFound, 1, str, NULL);
		SDL_UnlockMutex(actorsAniMutex);
		return NLF_ErrorAnimatinNotFound;
	}

	//check if the image file given exists
	if(NLF_OSS_check_file_existence(spriteAdress) == NLF_False)
	{
		printf("File \"%s\" not found\n", spriteAdress);
		SDL_UnlockMutex(actorsAniMutex);
		return NLF_ErrorFileNotFound;
	}

	if(SDL_LockMutex(actorsAniMutex) == 0)
	{
		//checking animaiton sprite capacity
		for(putIn = 0; putIn < aniaux->aniAmnt && aniaux->animations[putIn] != NULL; putIn++);
		if(putIn == aniaux->aniAmnt)
		{
			sprintf(str, "Animation ID=%d can't hold another sprite, it's already full: %d\n", ID, aniaux->aniAmnt);
			printf("%s", str);
			NLF_error_set_flag(NLF_ErrorMultiDef, 1, str, NULL);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorMultiDef;
		}

		//load the image file
		loadedimg = IMG_Load(spriteAdress);
		if( loadedimg == NULL )
		{
			printf("Couldn't load image \"%s\"", spriteAdress);
			sprintf(str, IMG_GetError());
			str[99] = '\0';
			printf("SDL_image Error: %s\n", str);
			NLF_error_set_flag(NLF_ErrorSDLImageLoad, 4, "Couldn't load image ", spriteAdress, "SDL_image Error: ", str, NULL);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorSDLImageLoad;
		}

		//transforme surface in texture
		tempTexture = SDL_CreateTextureFromSurface(window_rederer, loadedimg);
		if (tempTexture == NULL)
		{
			sprintf(str, IMG_GetError());
			str[99] = '\0';
			printf("Couldn't load create texture: %s\n", str);
			NLF_error_set_flag(NLF_ErrorSDLCreatingFail, 2, "Couldn't load create texture: ", str, NULL);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorSDLCreatingFail;
		}
		free(loadedimg);

		//allocate the frames
		frames = (NLF_Rect*) malloc(sizeof(NLF_Rect)*frameAmnt);
		if(frames == NULL)
		{
			printf("Could not allocate new frames");
			printf("\tAborting\n");
			NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate new frames", NULL);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorInsufficientMemory;
		}
		SDL_QueryTexture(tempTexture, NULL, NULL, &w, &h);
		w = w / columns;
		h = h / lines;
		for(i = 0; i < frameAmnt; i++)
		{
			frames[i].x = w * i;
			frames[i].y = h * (i % columns);
			frames[i].w = w;
			frames[i].h = h;
		}

		//setting the new animation fields
		aniaux->animations[putIn] = (struct spriteSheet*) malloc(sizeof(struct spriteSheet));
		if(aniaux->animations[putIn] == NULL)
		{
			printf("Could not allocate new sprites");
			printf("\tAborting\n");
			NLF_error_make_file_crash_report(NLF_ErrorInsufficientMemory, "Could not allocate new sprites", NULL);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorInsufficientMemory;
		}
		*aniID = putIn;
		aniaux->animations[putIn]->sheet = tempTexture;
		aniaux->animations[putIn]->frames = frames;
		aniaux->animations[putIn]->framesAmnt = frameAmnt;
		aniaux->animations[putIn]->currentFrame = NULL;
		aniaux->animations[putIn]->way = way;
		aniaux->animations[putIn]->refreshTime = (abs(((int)(frameAmnt/1000))*1000 - 1000) <= abs(((int)(frameAmnt/1000)+1)*1000 - 1000))? (frameAmnt/1000) : ((frameAmnt/1000)+1);

		SDL_UnlockMutex(actorsAniMutex);
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
	}

	return NLF_ErrorNone;
}

NLF_Error NLF_animation_set_way(NLF_USInt ID, NLF_USInt aniID, short int way)
{
/*
	arguments:
		ID - The ID of the animation wantede.
		aniID - the ID of the sheet wanted in the animation.
		way - way must be: -1, 0 or 1.
			if -1 or 1 - this function sets the 'way' status of the sheet aniID in animation ID to the given way.
			if 0 - this function toggle the 'way' status of the sheet aniID in animation ID to the given way.
	This fuction will:
		toggle the direction of the animation execution, and:
		returns NLF_ErrorNone - if everything goes right;
		returns NLF_ErrorAnimatinNotFound if the animation the de 'ID' given doesn't exist;
		returns NLF_ErrorBadArgument if the sheet 'aniID' doesn't exist in the animation;
		returns NLF_ErrorBadArgument if 'way' is not >=-1 and <=1
*/
	NLF_Animation *aniaux;

	if(way < -1 || way > 1)
	{
		printf("way argument must be >=-1 and <=1, but it's %d\n", way);
		return NLF_ErrorBadArgument;
	}

	aniaux = NLF_animation_search(ID);
	if(aniaux == NULL)
	{
		printf("Animation ID=%d not found\n", ID);
		return NLF_ErrorAnimatinNotFound;
	}

	if(SDL_LockMutex(actorsAniMutex) == 0)
	{
		if(aniID < 0 || aniID >= aniaux->aniAmnt)
		{
			printf("In animation ID=%d there's not sheet with aniID=%d\n", ID, aniID);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorBadArgument;
		}

		if(way == 0)
			aniaux->animations[aniID]->way *= -1;
		else
			aniaux->animations[aniID]->way *= way;

		SDL_UnlockMutex(actorsAniMutex);
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
	}

	return NLF_ErrorNone;
}

NLF_Animation* _NLF_animation_search_n_dell(NLF_USInt ID, NLF_bool dell)
{
/*
	arguments:
		ID - The ID of the animation who must be found
		dell - A flag, NLF_True means: delete this animation after finding.
	This fuction will:
		Return a pointer tho the animation found.
		Returns NULL if animation if not found.
		If the dell flag is true, delete the animation if it existe, or do nothing. Then return NULL.
*/
	struct AniVector *ant = NULL, *auxvect;
	NLF_USInt listID = (NLF_USInt) (ID / ANIVECTORSIZE);
	NLF_USInt i;
	NLF_Animation *ret;
	char str[50];

	ID = ID % ANIVECTORSIZE;

	if(SDL_LockMutex(actorsAniMutex) == 0)
	{
		auxvect = NLF_actorsAni;
		for(; listID > 0; listID++)
		{
			ant = auxvect;
			auxvect = auxvect->next;
			if(auxvect == NULL)
				break;
		}

		if(auxvect != NULL)
			ret = auxvect->anivect[ID];
		else
			ret = NULL;

		if(dell == NLF_True && ret != NULL)
		{
			ret = NULL;

			//freeing animation
			for(i = 0; i < auxvect->anivect[ID]->aniAmnt; i++)
			{
				if(auxvect->anivect[ID]->animations[i] != NULL)
				{
					SDL_DestroyTexture(auxvect->anivect[ID]->animations[i]->sheet);
					free(auxvect->anivect[ID]->animations[i]->frames);
					free(auxvect->anivect[ID]->animations[i]);
				}
			}
			free(auxvect->anivect[ID]->animations);
			free(auxvect->anivect[ID]);
			auxvect->anivect[ID] = NULL;

			if(ant != NULL)
			{
				ant->next = auxvect->next;
				free(auxvect->anivect);
				free(auxvect);
			}
			if(auxvect->fristEmpty > ID)
				auxvect->fristEmpty = ID;
		}

		SDL_UnlockMutex(actorsAniMutex);
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
	}

	return ret;
}

NLF_Error _NLF_animation_stop_start(NLF_USInt ID, NLF_USInt aniID, NLF_bool sos)
{
/*
	arguments:
		ID - The ID of the animation who must be found
		sos "stop or start" - if NLF_False, stop the animation. If NLF_True start it.
	This fuction will:
		stop or start running the animation, according to 'sos' parameter, and:
		returns NLF_ErrorNone - if everything goes right;
		returns NLF_ErrorActorNotFound - if the animation with the id 'ID' were not found.
		returns NLF_ErrorAnimatinNotFound - if the sprite 'aniID' in animation 'ID' is not found.
*/
	NLF_Animation *aniaux;
	NLF_USInt i;

	aniaux = NLF_animation_search(ID);
	if(aniaux == NULL)
	{
		printf("Animation ID=%d not found\n", ID);
		return NLF_ErrorActorNotFound;
	}

	if(SDL_LockMutex(actorsAniMutex) == 0)
	{
		if(aniID >= aniaux->aniAmnt)
		{
			printf("In animation %d sprite %d not found\n", ID, aniID);
			SDL_UnlockMutex(actorsAniMutex);
			return NLF_ErrorAnimatinNotFound;
		}

		if(sos == NLF_True)
		{
			//start animation
			aniaux->currentAni = aniaux->animations[aniID];
			if(aniaux->currentAni->way == 1)
				aniaux->currentAni = aniaux->currentAni->framesAmnt; //LEMBRAR DE CHECAR A TIPAGEM DESTA LINHA!!!
			else
				aniaux->currentAni = (aniaux->currentAni->frames + aniaux->currentAni->framesAmnt-1);
		}else{
			//stop animation
			aniaux->currentAni = NULL;
		}

		SDL_UnlockMutex(actorsAniMutex);
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
	}

	return NLF_ErrorNone;
}

NLF_Error NLF_animation_change_screen(NLF_USInt ID, NLF_USInt screenPosition)
{
/*
	arguments:
		ID - The ID of the animation who must be found
		screenPosition - what is the screen where this animation shall be printed.
	This fuction will:
		change in wich screen the given animation will be printed, and:
		returns NLF_ErrorNone - if everything goes right;
		returns NLF_ErrorBadArgument - if 'screenPosition' < 0. NOTE.: this fucntion will not notice if you pass a screen ID too big that does not existe in the game and this animation will be running but not printed
		returns NLF_ErrorActorNotFound - if the animation with the id 'ID' were not found.
*/
	NLF_Animation *aniaux;
	NLF_USInt i;

	if(screenPosition < 0)
	{
		printf("screenPosition value invalid\n", ID);
		return NLF_ErrorBadArgument;
	}

	aniaux = NLF_animation_search(ID);
	if(aniaux == NULL)
	{
		printf("Animation ID=%d not found\n", ID);
		return NLF_ErrorActorNotFound;
	}

	if(SDL_LockMutex(actorsAniMutex) == 0)
	{
		aniaux->screenID = screenPosition;

		SDL_UnlockMutex(actorsAniMutex);
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
	}

	return NLF_ErrorNone;
}

void NLF_animation_update()
{
/*
	This fuction will:
		update the state of all current running animations
*/
	secCounter += screen_deltaTicks;
	struct AniVector *auxvect;
	NLF_USInt i, aux;

	if(SDL_LockMutex(actorsAniMutex) == 0)
	{
		for(auxvect = NLF_actorsAni; auxvect != NULL; auxvect = auxvect->next)
		{
			for(i = 0; i < ANIVECTORSIZE; i++)
			{
				if(auxvect->anivect[i] != NULL && auxvect->anivect[i]->currentAni != NULL)
				{
					aux = secCounter % auxvect->anivect[i]->currentAni->refreshTime;
					if(&auxvect->anivect[i]->currentAni->frames[aux] != auxvect->anivect[i]->currentAni->currentFrame)
					{
						if(auxvect->anivect[i]->currentAni->way == 1)
							auxvect->anivect[i]->currentAni->currentFrame = &auxvect->anivect[i]->currentAni->frames[aux];
						else
							auxvect->anivect[i]->currentAni->currentFrame = &auxvect->anivect[i]->currentAni->frames[auxvect->anivect[i]->currentAni->framesAmnt-1 - aux];
						//update on screen!
					}
				}
			}
		}

		SDL_UnlockMutex(actorsAniMutex);
	} else {
		fprintf(stderr, "Couldn't lock mutex\n");
	}
}
/******************/



/*LOCAL FUNTIONS*/
/****************/