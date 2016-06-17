#include "NLF_osservice.h"
#include <sys/types.h>
#include <sys/stat.h>

NLF_bool NLF_OSS_check_directory_existence(const char *pathname)
{
/*
	Check is there's an directory with the name given by pathname
	returning NLF_True in a positive case, or NLF_False otherwise.
*/
	struct stat info;

	if(stat(pathname, &info ) != 0 )
		return NLF_False;
		//the directory given by pathname cannot be accessed
	else if( info.st_mode & S_IFDIR )
		return NLF_True;
		//the directory given by pathname exists
	else
		return NLF_False;
		//there is no directory with the given pathname
}

NLF_Error NLF_OSS_create_directory(const char *directory, const char *permission)
{
/*
	returns:
		NLF_ErrorNone on success
		NLF_ErrorCantCreateFile on fail.
		NLF_ErrorBadArgument if the permission is incorrectly passed
		NLF_ErrorInsufficientMemory if some momery allocation fail
	1º argument: the directory name to be created
		in case of the craetion directory do not be the actual directory it's recommended to pass its name with '/' separating
		the navagation trough the file system, the funtion will change for '\' if the OS is windows.
		Although the funtion will also search for '\' and change for '/' properly with the OS
	2º argument: an string like "xxx" having {x | 1 <= x <= 7} just like chmod command in linux (or just NULL)
		The first number is the permission for owner, the second to the group and the third to the others (just like chmod)
		7 = 4+2+1 (read/write/execute)
		6 = 4+2 (read/write)
		5 = 4+1 (read/execute)
		4 = 4 (read)
		3 = 2+1 (write/execute)
		2 = 2 (write)
		1 = 1 (execute)
	PS.: in windows this argument is just ignored.

		if the Argument is NULL, it will just be created with 777 status
*/
	short int i, auxi;
	char *directoryAux = NULL;
	NLF_bool needChange = NLF_False;

	#ifndef NLF_OS_WINDOWS
		mode_t openMode = 0;

		/*SETTING/CHECKING PERMISSION CONSISTENCY*/
		if(permission != NULL)
		{
			if(permission != NULL)
			if(strlen(permission) != 3)
				return NLF_ErrorBadArgument;

			for(i = 0; i < 4; i++)
			{
				switch(permission[i])
				{
					case '7':
						openMode = openMode | (i == 1 ? S_IRWXU : (i == 2 ? S_IRWXG : S_IRWXO));
					break;

					case '6':
						openMode = openMode | (i == 1 ? S_IRUSR|S_IWUSR : (i == 2 ? S_IRGRP|S_IWGRP : S_IROTH|S_IWOTH));
					break;

					case '5':
						openMode = openMode | (i == 1 ? S_IRUSR|S_IXUSR : (i == 2 ? S_IRGRP|S_IXGRP : S_IROTH|S_IXOTH));
					break;

					case '4':
						openMode = openMode | (i == 1 ? S_IRUSR : (i == 2 ? S_IRGRP : S_IROTH));
					break;

					case '3':
						openMode = openMode | (i == 1 ? S_IWUSR|S_IXUSR : (i == 2 ? S_IWGRP|S_IXGRP : S_IWOTH|S_IXOTH));
					break;

					case '2':
						openMode = openMode | (i == 1 ? S_IWUSR : (i == 2 ? S_IWGRP : S_IWOTH));
					break;

					case '1':
						openMode = openMode | (i == 1 ? S_IXUSR : (i == 2 ? S_IXGRP : S_IXOTH));
					break;

					default:
						return NLF_ErrorBadArgument;
				}
			}
		}else{
			openMode = S_IRWXU|S_IRWXG|S_IRWXO; //"777" permission
		}
	#endif

	/*CHECKING DIRETORY PATH PORTABILITY*/
	auxi = strlen(directory);
	for(i = 0; i < auxi; i++)
	{
		#ifdef NLF_OS_WINDOWS
			if(directory[i] == '/')
		#else
			if(directory[i] == '\\')
		#endif
		{
			needChange = NLF_True;
			break;
		}
	}

	directoryAux = (char*) malloc(sizeof(char)*(auxi + 1));
	if(directoryAux == NULL)
		return NLF_ErrorInsufficientMemory;

	if(needChange == NLF_True)
	{
		for(i = 0; i < auxi; i++)
		{
		#ifdef NLF_OS_WINDOWS
			if(directory[i] == '/')
				directoryAux[i] = '\\';
		#else
			if(directory[i] == '\\')
				directoryAux[i] = '/';
		#endif
			else
				directoryAux[i] = directory[i];
		}
		directoryAux[i] = '\0';
	}else{
		strcpy(directoryAux, directory);
	}

	//now create the directory
	#ifdef NLF_OS_WINDOWS
		if(mkdir(directoryAux) != 0)
			return NLF_ErrorCantCreateFile;
	#else
		if(mkdir(directoryAux, openMode) != 0)
			return NLF_ErrorCantCreateFile;
	#endif

	//job done successfully
	if(directoryAux != NULL)
		free(directoryAux);
	return NLF_ErrorNone;
}

NLF_SystemType NLF_OSS_type()
{
/*
	Return which is the Operating System acording to NLF_SystemType enum
	PS.: the funtion will return the os type as specific as it can, for ex.:
		if it had detected that the os is unix based and linux, is'll return linux.
		if it had detected that the os is unix based but nothing more, will return only unix.
*/
	#if defined(NLF_OS_UNIDENTIFIED)
		return NLF_SystemTypeUndefenied;
	#elif defined(NLF_OS_LINUX)
		return NLF_SystemTypeLinux;
	#elif defined(NLF_OS_FREEBSD)
		return NLF_SystemTypeFreeBSD;
	#elif defined(NLF_OS_OSX)
		return NLF_SystemTypeOSX;
	#elif defined(NLF_OS_IPHONE)
		return NLF_SystemTypeIphone;
	#elif defined(NLF_OS_SIMULATOR)
		return NLF_SystemTypeIphoneSimulatior;
	#elif defined(NLF_OS_APPLE)
		return NLF_SystemTypeApple;
	#elif defined(NLF_OS_UNIX)
		return NLF_SystemTypeUnix;
	#elif defined(NLF_OS_WINDOWS)
		return NLF_SystemTypeWindows;
	#endif
}

unsigned short int NLF_OSS_bits()
{
/*
	Returns how manu the bits of the system or 0(zero) if NLF could not determinate it.
*/
	#if defined(NLF_OS_BITS_UNIDENTIFIED)
		return 0;
	#elif defined(NLF_OS_64BITS)
		return 64;
	#elif defined(NLF_OS_32BITS)
		return 32;
	#endif
}

NLF_bool NLF_OSS_check_file_existence(const char *pathname)
{
/*
	Check is there's an directory with the name given by pathname
	return:
		NLF_True on success
		NLF_False otherwise
*/
	struct stat info;
	NLF_bool ok;

	//if(errno == ENOENT) //path não existe
	if(stat(pathname, &info) < 0)
		ok = NLF_False;
	else
		ok = NLF_True;

	return ok;
}