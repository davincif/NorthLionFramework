#include "NLF_screen.h"

void NLF_screen_init()
{
	int flags;
	int aux;
	char *str;

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
			printf("\n");
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
		printf("\n\t%d Displays avaliable, piking the number %d as pattern\n", displayInUse + 1);
	}

	if(SDL_GetCurrentDisplayMode(displayInUse, &videoMode) != 0)
	{
		//error case
		printf("\tCould not get display mode for video display #%d: %s", displayInUse, SDL_GetError());
		printf("\tstandard 800x600 will be set\n");
		displayInfoUnknown = NLF_True;
	}else{
		printf("\tcurrent display(%d) mode detected %dx%dpx @ %dhz. \n", 0, videoMode.w, videoMode.h, videoMode.refresh_rate);
		displayInfoUnknown = NLF_False;
	}
	printf("\n");
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

	screens.scene = NULL;
	screens.position = 0;
	screens.next = NULL;

	currentFPS = 0;
	//there's just no need to the FPS be greater then the display refrash rate
	(videoMode.refresh_rate >= 60) ? (idealFPS = 60): (idealFPS = videoMode.refresh_rate);
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
				printf("\tIt was impossibel to create the render.\n");
				str = SDL_GetError();
				printf("\tSDL_ERROR: %s\n", str);
				printf("\tAborting\n");
				NLF_error_make_file_crash_report(NLF_ErrorSDLProblem, "It was impossibel to create the render", "SDL Error: ", str, NULL);
				exit(NLF_ErrorSDLProblem); //I don't know what to do else but quiting... you know, we also need a renderer .-.
			}
		}else{
			if(aux > 0)
				printf("done\n");
		}
	}
	if(aux > 0)
		printf("\n");
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
	if(aux > 0)
		printf("\n");
	///******************************
	/***************************/
}

void NLF_screen_quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(window_rederer);
	IMG_Quit();
}