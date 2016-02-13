#include "NLF_screen.h"

/*GLOBAL FUNCTIONS*/
void NLF_screen_init()
{
	int flags;
	int aux;
	char *str;
	int sdlRet;
	NLF_bool saydone;

	/*INITIALIZING SDL_IMAGE*/
	flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if((IMG_Init(flags) & flags) != flags)
	{
		flags = IMG_INIT_TIF;
		if((IMG_Init(flags) & flags) != flags)
		{
			printf("\tCan't initialize SDL_image\nSDL_IMAGE ERROR: %s\n", IMG_GetError());
			printf("\ttraing for TIF format... ");
			flags = IMG_INIT_TIF;
			if((IMG_Init(flags) & flags) != flags)
			{
				printf("fail\n\tSDL_IMAGE ERROR: %s\n", IMG_GetError());
				tifFlag = NLF_True;
			}else{
				printf("done\n");
				tifFlag = NLF_False;
			}

			printf("\ttraing for PNG format... \n");
			flags = IMG_INIT_PNG;
			if((IMG_Init(flags) & flags) != flags)
			{
				printf("fail\n\tSDL_IMAGE ERROR: %s\n", IMG_GetError());
				pngFlag = NLF_True;
			}else{
				printf("done\n");
				pngFlag = NLF_False;
			}

			printf("\ttraing for JPG format... \n");
			flags = IMG_INIT_JPG;
			if((IMG_Init(flags) & flags) != flags)
			{
				printf("fail\n\tSDL_IMAGE ERROR: %s\n", IMG_GetError());
				jpgFlag = NLF_True;
			}else{
				printf("done\n");
				jpgFlag = NLF_False;
			}

			if(jpgFlag == NLF_False && pngFlag == NLF_False && tifFlag == NLF_False)
			{
				printf("\tScreen module initialization has failed.\n");
				str = SDL_GetError();
				printf("\tSDL_ERROR: %s\n", str);
				printf("\tAborting\n");
				NLF_error_make_file_crash_report(NLF_ErrorSDLImageInitializer, "Screen module initialization has failed", "SDL Error: ", str, NULL);
				exit(NLF_ErrorSDLImageInitializer);
			}
		}
	}else{
		jpgFlag = NLF_True;
		pngFlag = NLF_True;
		tifFlag = NLF_True;
	}
	/************************/

	/*SETTING GLOBALS VARIABLES*/
	///GETTING DISPLAY INFORMATIONS
	aux = SDL_GetNumVideoDisplays();
	if(aux < 0)
	{
		//error case
		printf("Fail trying to get video display number.\n");
		str = SDL_GetError();
		printf("\tSDL_ERROR: %s\n", str);
		printf("\tAborting\n");
		NLF_error_make_file_crash_report(NLF_ErrorSDLProblem, "Fail trying to get video display number", "SDL Error: ", str, NULL);
		exit(aux);
	}else{
		displayInUse = 0;
		printf("\n\t%d Displays avaliable, piking the number %d as pattern\n", aux, displayInUse + 1);
	}

	displayInfoUnknown = NLF_False;
	for(aux = 0; aux < 3; aux++)
	{
		saydone = NLF_False;
		if(aux == 0)
			sdlRet = SDL_GetCurrentDisplayMode(displayInUse, &videoMode);
		else if(aux == 1)
			sdlRet = SDL_GetDisplayMode(displayInUse, displayInUse, &videoMode);
		else if(aux == 2)
			sdlRet = SDL_GetWindowDisplayMode(displayInUse, &videoMode);

		if(sdlRet != 0)
		{
			if(aux == 2)
			{
				//totally unsuccessful case
				printf("\tSorry, impossible to get display #%d mode:\n\tSDL_ERROR: %s\n", displayInUse + 1, SDL_GetError());
				printf("\tstandard 800x600 will be set\n");
				displayInfoUnknown = NLF_True;
			}else{
				printf("\tSorry, could not get display #%d mode:\n\tSDL_ERROR: %s\n", displayInUse + 1, SDL_GetError());
				printf("\ttrying another way...\n");
				saydone = NLF_True;
			}
		}else{
			printf("\tcurrent display(%d) mode detected %dx%dpx @ %dhz. \n", displayInUse + 1, videoMode.w, videoMode.h, videoMode.refresh_rate);
			if(videoMode.w == 0 || videoMode.h == 0 || videoMode.refresh_rate == 0)
			{
				printf("\tSorry, could not get all display #%d mode information\n", displayInUse + 1);
				printf("\ttrying another way...\n");
				saydone = NLF_True;
			}else{
				printf("\tdone\n");
				aux = 3;
			}
		}
	}
	///****************************

	/*SETTING SOME GLOBAL VARIABLES*/
	camera.x = 0;
	camera.y = 0;
	if(displayInfoUnknown == NLF_False)
	{
		camera.w = 800;
		camera.h = 600;
	}else{
		camera.w = videoMode.w;
		camera.h = videoMode.h;
	}

	screens = NULL;
	currentFPS = 0;
	//there's just no need to the FPS be greater then the display refresh rate
	(videoMode.refresh_rate >= 60 || videoMode.refresh_rate == 0) ? (idealFPS = 60): (idealFPS = videoMode.refresh_rate);
	/*******************************/

	///CREATING WINDOW SET
	window = SDL_CreateWindow("NorthLionFramework Game", 0, 0, camera.w, camera.h, SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED);
	if(window == NULL)
	{
		printf("\tCould not craete the window\n");
		str = SDL_GetError();
		printf("\tSDL_ERROR: %s\n", str);
		printf("\tAborting\n");
		NLF_error_make_file_crash_report(NLF_ErrorSDLProblem, "Could not craete the window", "SDL Error: ", str, NULL);
		exit(NLF_ErrorSDLProblem); //I don't know what to do else but quiting... you know, we need a window .-.
	}

	//this loop will try to initialize the render the best way possible
	for(aux = 0; aux < 4 && window_rederer == NULL; aux++)
	{
		switch(aux)
		{
			case 0:
				flags = SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
				break;
			case 1:
				flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
				break;
			case 2:
				flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
				break;
			case 3:
				flags = SDL_RENDERER_ACCELERATED;
				break;
		}
		if(aux > 0)
			printf("\ttrying flags(%d)... ", flags);

		window_rederer = SDL_CreateRenderer(window, -1, flags);
		if(window_rederer == NULL)
		{
			if(aux > 0)
				printf("fail\n");
			if(aux != 3)
			{
				printf("\tCould not create render with all flags(%d)\n", flags);
				printf("\tSDL_ERROR: %s\n", SDL_GetError());
			}else{
				printf("\tIt was impossible to create the render.\n");
				str = SDL_GetError();
				printf("\tSDL_ERROR: %s\n", str);
				printf("\tAborting\n");
				NLF_error_make_file_crash_report(NLF_ErrorSDLProblem, "It was impossible to create the render", "SDL Error: ", str, NULL);
				exit(NLF_ErrorSDLProblem); //I don't know what to do else but quiting... you know, we also need a renderer .-.
			}
		}else{
			if(aux > 0)
				printf("done\n");
		}
	}
	if(aux > 0)
	///*******************

	///GETTING VIDEO CARD INFORMATION
	aux = 0;
	if(SDL_GetRendererInfo(window_rederer, &rendererInfo) < 0)
	{
		printf("\tCould not get information about the renderer.\n");
		printf("\tSDL_ERROR: %s\n", SDL_GetError());
		rendererInfoUnknown = NLF_True;
	}else{
		aux = 1;
		rendererInfoUnknown = NLF_False;
	}

	if(SDL_GetRenderDriverInfo(displayInUse, &rendererDriverInfo) < 0)
	{
		printf("\tCould not get information about the renderer driver.\n");
		printf("\tSDL_ERROR: %s\n", SDL_GetError());
		rendererDriverInfoUnkown = NLF_True;
	}else{
		aux = 1;
		rendererDriverInfoUnkown = NLF_False;
	}
	///******************************
	/***************************/
}

void NLF_screen_quit()
{
	NLF_screen_destroy();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(window_rederer);
	IMG_Quit();
}

unsigned short int NLF_screen_add(unsigned short int sugestPosition, unsigned short int x, unsigned short int y, unsigned short int w, unsigned short int h, NLF_Alignment vAlign, NLF_Alignment hAlign, NLF_bool isStatic)
{
/*
	arguments:
		sugestPosition - the order, between the other screen, where to insert the new screen.
		x,y - the position where where this screen shall be prited in the APPLICATION WINDOW, understand, not the device screen.
		w,h - the screen's widh height.
		verticalAlign,verticalAlign - the alignment this screen will have retated to the appliaction window.
		isStatic - NLF_True if this screen changes rarely, NLF_False if it is.
		Note.: leave w or h 0 and the screen will have the size of the Windows, x and y will also be set to 0 and the alignment will be ignored.
		Note².: leave the alignment 0 or NLF_AlignmentNone to use no alignment.
		Note³.: any alignment but 0 lead the function to ignore x and y values.
	This fuction will:
		Create a new screen and try to put it at the position sugested
		returns the position where the screen was inserted, or 0 in error case
		the only way this function can fail is if any allocation errorc ocurrur, then it'll sets an error flag and msg
*/
	unsigned short int aux, ret;
	NLF_Screen *ps, *psant, *stemp;
	int ww, hw;

	if(sugestPosition <= 0)
	{
		NLF_error_set_flag(NLF_ErrorCantCreateFile, 1, "in NLF_screen_add 1st argument must be > 0");
		return 0;
	}

	stemp = (NLF_Screen*) malloc(sizeof(NLF_Screen));
	if(stemp == NULL)
	{
		printf("Could not craete NLF_Screen\n");
		printf("Out of memory\n");
		NLF_error_set_flag(NLF_ErrorInsufficientMemory, 1, "Out of memory when creating new screen");
		ret = 0;
	}else{
		if(screens != NULL)
		{
			//fiding where to insert the screen
			ps = screens;
			psant = screens;
			for(aux = 1; aux < sugestPosition && ps != NULL; aux++)
			{
				psant = ps;
				ps = ps->next;
			}
			stemp->position = aux;

			//fixing the pointers
			if(aux == sugestPosition)
				stemp->next = ps;
			else
				stemp->next = NULL;

			if(ps != psant)
				psant->next = stemp;
			else
				screens = stemp;

			//return set
			ret = aux;

			//fixign the positions
			for(ps = stemp->next; ps != NULL; ps = ps->next)
			{
				ps->position++;
			}
		}else{
			stemp->position = 1;
			stemp->next = NULL;
			screens = stemp;

			ret = 1;
		}

		//calculatting position in the windows
		SDL_GetWindowSize(window, &ww, &hw);
		if(w == 0 || h == 0)
		{
			stemp->dimetions.x = 0;
			stemp->dimetions.y = 0;
			stemp->vAlign = NLF_AlignmentNone;
			stemp->hAlign = NLF_AlignmentNone;
			stemp->dimetions.w = ww;
			stemp->dimetions.h = hw;
		}else{
			stemp->dimetions.w = w;
			stemp->dimetions.h = h;
			if(vAlign == 0)
			{
				stemp->dimetions.x = x;
				stemp->vAlign = NLF_AlignmentNone;
			}else{
				stemp->vAlign = vAlign;
				switch(vAlign)
				{
					case NLF_AlignmentCenter:
						stemp->dimetions.y = (hw - stemp->dimetions.h) / 2;
						break;

					case NLF_AlignmentUp:
						stemp->dimetions.y = 0;
						break;

					case  NLF_AlignmentDown:
						stemp->dimetions.y = hw - stemp->dimetions.h;
						break;

					//Right and Left are not valid alignment for height
					default:
						stemp->vAlign = NLF_AlignmentNone;
				}
			}
			if(hAlign == 0)
			{
				stemp->dimetions.y = y;
				stemp->hAlign = NLF_AlignmentNone;
			}else{
				stemp->hAlign = hAlign;
				switch(hAlign)
				{
					case NLF_AlignmentCenter:
						stemp->dimetions.x = (ww - stemp->dimetions.w) / 2;
						break;

					case NLF_AlignmentRight:
						stemp->dimetions.x = ww - stemp->dimetions.w;
						break;

					case  NLF_AlignmentLeft:
						stemp->dimetions.x = 0;
						break;

					//Up and Down are not valid alignment for width
					default:
						stemp->vAlign = NLF_AlignmentNone;
				}
			}
			stemp->dimetions.w = w;
			stemp->dimetions.h = h;
		}

		//creating the SDL_Texutre
		stemp->scene = SDL_CreateTexture(window_rederer, SDL_PIXELFORMAT_UNKNOWN, (isStatic == NLF_True) ? SDL_TEXTUREACCESS_STATIC : SDL_TEXTUREACCESS_STREAMING, stemp->dimetions.w, stemp->dimetions.h);
		if(stemp->scene == NULL)
		{
			printf("Could not craete screen's texture\n");
			printf("Out of memory\n");
			NLF_error_set_flag(NLF_ErrorInsufficientMemory, 1, "Out of memory when creating new screen's texture");
			NLF_screen_remove(stemp->position);
			ret = 0;
		}else{

		}
	}

	return ret;
}

void NLF_screen_remove(short int position)
{
/*
	arguments:
		position - the place of the screen to remove
		NOTE.: pass -1 as 'position' and the last screen will be removed
		NOTE².: if there is not a 'position' screen of if position is 0, this function won't do nothing but cosuming process
*/
	NLF_Screen *ps, *psant;
	short int aux;

	ps = screens;
	psant = screens;
	if(position > 0)
	{
		//fiding the given positon
		for(aux = 1; aux < position && ps != NULL; aux++)
		{
			psant = ps;
			ps = ps->next;
		}
	}else if(position == -1)
	{
		//fiding the last position
		while(ps->next != NULL)
		{
			psant = ps;
			ps = ps->next;
		}
		aux = position;
	}

	if(position != 0)
	{
		//removing the found position
		if(aux == position && ps != NULL)
		{
			if(psant == screens)
				screens = psant->next;
			else
				psant->next = ps->next;
			SDL_DestroyTexture(ps->scene);
			free(ps);
		}

		//fixing the positions
		for(ps = psant->next; ps != NULL; ps = ps->next)
		{
			ps->position--;
		}
	}
}

void NLF_screen_print()
{
/*
	this function will:
		print all screens in the display
*/
	NLF_Screen *ps;
	SDL_RenderClear(window_rederer);
	for(ps = screens; ps != NULL; ps = ps->next)
	{
		SDL_RenderCopy(window_rederer, ps->scene, (const SDL_Rect*) &camera, (const SDL_Rect*) &ps->dimetions);
	}
	SDL_RenderPresent(window_rederer);
}

void NLF_camera_move(int plusx, int plusy)
{
/*
	arguments:
		plusx - the number to be added (or subtracted) from the camera's x position
		plusy - the number to be added (or subtracted) from the camera's y position
	This fuction will:
		just move the camera. Related to the windows OF COURSE
*/
	camera.x += plusx;
	camera.y += plusy;
}

void NLF_camera_resize(int x, int y, int w, int h)
{
/*
	arguments:
		x,y,w,h - the new x, y, width and height of the camera
*/
		camera.x = x;
		camera.y = y;
		camera.w = w;
		camera.h = h;
}

void NLF_camera_add_size(int plusw, int plush)
{
/*
	arguments:
		plusx - the number to be added (or subtracted) from the camera's width size
		plusy - the number to be added (or subtracted) from the camera's height size
	This fuction will:
		just change the camera size
*/
}

void NLF_camera_setPosition(int newx, int newy)
{
/*
	arguments:
		newx, newy - the new x and y of the camera
	This fuction will:
		just reposition the camera
*/
		camera.x = newx;
		camera.y = newy;
}

void NLF_camera_realign(NLF_Alignment vAlignment, NLF_Alignment hAlignment)
{
/*
	arguments:
		vAlignment - the new camera's vertical alignment
		hAlignment - the new camera's horizontal alignment
		NOTE.: both related to the windows, OF COURSE
		NOTE².: NLF_AlignmentRight and NLF_AlignmentLeft are ignored for vAligment, analogously, NLF_AlignmentUp and NLF_AlignmentDown are for hAlignment
	This fuction will:
		realing the camera
*/
		int ww, hw;

		SDL_GetWindowSize(window, &ww, &hw);

		switch(vAlignment)
		{
			case NLF_AlignmentCenter:
				camera.y = (hw - camera.h) / 2;
				break;

			case NLF_AlignmentUp:
				camera.y = 0;
				break;

			case NLF_AlignmentDown:
				camera.y = hw - camera.h;
				break;
		}

		switch(hAlignment)
		{
			case NLF_AlignmentCenter:
				camera.x = (ww - camera.w) / 2;
				break;

			case NLF_AlignmentRight:
				camera.x = ww - camera.w;
				break;

			case NLF_AlignmentLeft:
				camera.x = 0;
				break;
		}
}

void NLF_camera_reset()
{
/*
	arguments:
	This fuction will:
*/
	int ww, wh;

	camera.x = 0;
	camera.y = 0;
	SDL_GetWindowSize(window, &camera.w, &camera.h);
}

/******************/

/*LOCAL FUNTIONS*/
static void NLF_screen_destroy()
{
	NLF_Screen *ps;
	for(ps = screens; ps != NULL;)
	{
		screens = ps->next;
		SDL_DestroyTexture(ps->scene);
		free(ps);
		ps = screens;
	}
}
/****************/