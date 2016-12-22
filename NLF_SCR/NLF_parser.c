#include "NLF_parser.h"

#define BASE_CHAR_LENGH 20

static Markdowns whichMarkdown(char *s0, char *sf)
{
	char keep = *sf;
	Markdowns ret;

	s0++;
	if(*s0 == '/')
		s0++;
	*sf = '\0';

	if(strcmp(s0, "ROOT") == 0)
		ret = MARK_ROOT;
	else if(strcmp(s0, "ADDTYPE") == 0)
		ret = MARK_ADDTYPE;
	else if(strcmp(s0, "ELEMENT") == 0)
		ret = MARK_ELEMENT;
	else if(strcmp(s0, "NAME") == 0)
		ret = MARK_NAME;
	else if(strcmp(s0, "VALUE") == 0)
		ret = MARK_VALUE;
	else if(strcmp(s0, "TYPE") == 0)
		ret = MARK_TYPE;
	else if(strcmp(s0, "NEED_MIN") == 0)
		ret = MARK_NEED_MIN;
	else if(strcmp(s0, "NEED_MAX") == 0)
		ret = MARK_NEED_MAX;
	else
		ret = MARK_ERROR;

	*sf = keep;

	return ret;
}

static Eltype typeExist(char *tname0, char *tnamef, char **tobeset)
{
	Eltype ret = t_error;
	char keep;
	short int i;

	keep = *tnamef;
	*tnamef = '\0';
	if(*tname0 == '>')
		tname0++;

	if(strcmp(tname0, "int") == 0)
	{
		ret = t_int;
	}else if(strcmp(tname0, "float") == 0)
	{
		ret = t_float;
	}else if(strcmp(tname0, "double") == 0)
	{
		ret = t_double;
	}else if(strcmp(tname0, "string") == 0)
	{
		ret = t_string;
	}else if(ctypes[0] != NULL){
		//check in the custom types
		for(i = 0; ctypes[i] != NULL; i++)
		{
			if(strcmp(tname0, ctypes[i]->name) == 0)
			{
				ret = t_custom;
				break;
			}
		}

		if(ret != t_custom)
			ret = t_error;
		else if(tobeset != NULL)
			*tobeset = ctypes[i]->name;

	}else{
		ret = t_error;
	}

	*tnamef = keep;

	return ret;
}

static void closeByError(int exitnumber, FILE *to_close, char *theLine)
{
	int i, i2;
	if(elements != NULL)
	{
		for(i = 0; elements[i] != NULL; i++)
		{
			free(elements[i]->name);
			free(elements[i]);
		}
		free(elements);
		elements = NULL;
	}
	if(ctypes != NULL)
	{
		for(i = 0; ctypes[i] != NULL; i++)
		{
			free(ctypes[i]->name);
			for(i2 = 0; ctypes[i]->types[i2] != NULL; i2++)
			{
				free(ctypes[i]->types[i2]);
			}
			free(ctypes[i]->types);
			free(ctypes[i]);
		}
		free(ctypes);
		ctypes = NULL;
	}
	if(theLine != NULL)
		free(theLine);
	if(to_close != NULL)
		fclose(to_close);
	printf("quiting, error code: %d\n", exitnumber);
	exit(exitnumber);
}

static int readLine(FILE *rules, char **line, NLF_bool showLine)
{
	char c, *aux;
	static short int linemaxlengh = 0;
	short int linelengh = 0;
	int ret = 0, fret = 0;

	do{
		fret = fscanf(rules, "%c", &c);

		if(fret != EOF)
		{
			ret += fret;
			if(linelengh >= linemaxlengh-1)
			{
				/*ALLOCATING MEMORY TO LINE*/
				linemaxlengh += BASE_CHAR_LENGH;
				aux = (char*) realloc(*line, sizeof(char)*linemaxlengh);
				if(aux == NULL)
				{
					closeByError(4, rules, *line);
				}else{
					*line = aux;
					aux = NULL;
				}
				/***************************/
			}

			/*STORING LINE ON MEMORY*/
			if(c != '\t' && c != '\n')
			{
				(*line)[linelengh] = c;
				linelengh++;
			}
			/************************/
		}else{
			ret = EOF;
		}
	}while(fret != EOF && (c != '\n' || linelengh == 0));

	(*line)[linelengh] = '\0';
	linecounter++;

	if(showLine == NLF_True)
		printf("%dline: %s\n", linecounter, *line);

	return ret;
}

static NLF_bool lookForCommentary(char *line, NLF_bool showLine)
{
	char *aux, *aux2;
	short int i;
	static NLF_bool onComment = NLF_False;

	aux = strstr(line, "<!--");
	if(aux != NULL || onComment == NLF_True)
	{
		if(onComment == NLF_False && aux != NULL)
		{
			*aux = '\0';
			for(i = 0; (aux + i) >= line; i--)
				if(*(aux + i) == ' ')
					*(aux + i) = '\0';
		}

		aux2 = strstr(((onComment == NLF_False)?(aux+4):(line)), "-->");
		if(aux2 != NULL)
		{
			aux2 += 3;
			if(onComment == NLF_True)
			{
				if(*aux2 == '\0')
				{
					linecounter--;
					*line = '\0';
				}else{
					strcpy(line, aux2);
				}
				onComment = NLF_False;
			}
		}else{
			if(onComment == NLF_True)
			{
				*line = '\0';
				linecounter--;
			}
			onComment = NLF_True;
		}
	}


	if(showLine == NLF_True && *line != '\0')
		printf("no-comment-line: %s\n", line);

	return onComment;
}

static short int syntaxChecker(char *line)
{
	char *aux, *conductor, *commentStart;

	commentStart = strstr(line, "<!--");
	/*CHEKING MARKDOWN*/
	do{
		line = strstr(line, "<");
		if(line != NULL)
		{
			//to ignore the comments
			if(commentStart != NULL)
				aux = commentStart;
			else
				aux = strstr(line, ">");

			if(aux != NULL)
			{
				for(conductor = line + 1; conductor != aux; conductor++)
				{
					if(!(
						(*conductor >= '0' && *conductor <= '9' && conductor > line + 1)
						|| (*conductor >= 'A' && *conductor <= 'Z')
						|| (*conductor >= 'a' && *conductor <= 'z')
						|| (*(conductor-1) == '<' && (*conductor == '/' || *conductor == '!'))
						|| *conductor == '_' || *conductor == '-'))
					{
						printf("error on line %d: ", linecounter);
						printf("incorrect character \'%c\'\n", *conductor);
						return 5;
					}
				}
			}else{
				printf("error on line %d: ", linecounter);
				printf("\'<\' with no \'>\', incomplet markdown\n");
				return 6;
			}
		}
		/******************/

		/*CHECKING BETWEEN MARKDOWN*/
		if(aux != NULL && *aux == '>')
		{
			line = aux;
			aux = strstr(aux+1, "<");
			if(aux != NULL)
			{
				for(conductor = line + 1; conductor != aux; conductor++)
				{
					if(*conductor == '&' || *conductor == '<')
					{
						printf("error on line %d: ", linecounter);
						printf("incorret character \'%c\'\n", *conductor);
						return 7;
					}
				}
				line = aux;

			}else{
				line = NULL;
			}
		}
	}while(line != NULL);
	/***************************/

	return 0;
}

static int interpeter(char *line)
{
	char *aux, *aux2, *allocaux, **dcaux;
	short int i;
	Markdowns mark = MARK_ERROR;
	Eltype etype;

	Element **eleaux;
	static Element *inuse = NULL;
	short int elei;

	CustomType **ctypesaux;
	static CustomType *inusetype = NULL;
	static short int ctypesi;
	static NLF_bool addingType = NLF_False;

	/*INTERPRETING*/
	aux = line;
	elei = 0;
	do{
		aux = strstr(aux, "<");
		if(aux != NULL)
		{
			aux2 = strstr(aux, ">");

			mark = whichMarkdown(aux, aux2);

			if(*(aux+1) != '/')
			{
				switch(mark)
				{
					case MARK_ERROR:
						printf("error on line %d: ", linecounter);
						printf("markdown not reconized\n \"%s\"\n", line);
						return 10;
					break;

					//case MARK_ROOT nothing to be done

					case MARK_ADDTYPE:
						if(addingType == NLF_True)
						{
							printf("error on line %d: ", linecounter);
							printf("nested <ADDTYPE> not accepted\n");
							return 21;
						}
						ctypesi = ctypessize-1;
						ctypessize++;
						ctypesaux = (CustomType**) realloc(ctypes, sizeof(CustomType*)*ctypessize);
						if(ctypesaux == NULL)
						{
							printf("could not reallocate element's vector\n");
							return 19;
						}
						ctypes = ctypesaux;
						ctypes[ctypessize-1] = NULL;
						ctypes[ctypesi] = (CustomType*) malloc(sizeof(CustomType));
						if(ctypes[ctypesi] == NULL)
						{
							printf("could not allocate custom type\n");
							return 20;
						}
						inusetype = ctypes[ctypesi];
						inusetype->name = NULL;
						inusetype->types = (char**) malloc(sizeof(char*));
						if(inusetype->types == NULL)
						{
							printf("could not allocate memory\n");
							return 1;
						}
						inusetype->types[0] = NULL;
						inusetype->closed = NLF_False;
						addingType = NLF_True;
					break;

					case MARK_ELEMENT:
						if(addingType == NLF_True)
						{
							printf("error on line %d: ", linecounter);
							printf("This tag is not reconized inside <ADDTYPE>\n");
							return 21;
						}

						elei = elevetsize-1;
						elevetsize++;
						eleaux = (Element**) realloc(elements, sizeof(Element*)*elevetsize);
						if(eleaux == NULL)
						{
							printf("could not reallocate element's vector\n");
							return 14;
						}
						elements = eleaux;
						elements[elevetsize-1] = NULL;
						elements[elei] = (Element*) malloc(sizeof(Element));
						if(elements[elei] == NULL)
						{
							printf("could not allocate element\n");
							return 8;
						}
						inuse = elements[elei];
						inuse->type = t_noTypeYet;
						inuse->customTypeName = NULL;
						inuse->name = NULL;
						inuse->qtdmin = -1;
						inuse->qtdmax = -1;
						inuse->closed = NLF_False;

						for(i = elei-1; i >= 0 && elements[i]->closed == NLF_True; i--);

						if(i >= 0)
							inuse->insideof = elements[i];
						else
							inuse->insideof = NULL;
					break;

					case MARK_NAME:
						aux = strstr(aux, ">");
						aux2 = strstr(aux, "</NAME>");
						if(aux == NULL || aux2 == NULL)
						{
							printf("error on line %d: ", linecounter);
							printf("missing to close \"NAME\" tag\n");
							return 18;
						}
						aux += 1;
						*aux2 = '\0';
						allocaux = (char*) malloc(sizeof(char)*(strlen(aux) + 1));
						if(allocaux == NULL)
						{
							printf("could not allocate char* name\n");
							return 15;
						}

						if(addingType == NLF_True)
						{
							ctypes[ctypesi]->name = allocaux;
							strcpy(ctypes[ctypesi]->name, aux);
						}else{
							inuse->name = allocaux;
							strcpy(inuse->name, aux);
						}
						allocaux = NULL;
						*aux2 = '<';
						aux = aux2+1;
					break;

					case MARK_VALUE:
						if(addingType == NLF_False)
						{
							printf("error on line %d: ", linecounter);
							printf("tag not reconized for this element\n");
							return 21;
						}

						for(i = 0; inusetype->types[i] != NULL; i++);
						i++;
						dcaux = (char**) realloc(inusetype->types, sizeof(char*)*(i+1));
						if(dcaux == NULL)
						{
							printf("could not reallocate type's names's vector\n");
							return 19;
						}
						dcaux[i] = NULL;
						inusetype->types = dcaux;

						aux = strstr(aux, ">");
						aux2 = strstr(aux, "</VALUE>");
						if(aux == NULL || aux2 == NULL)
						{
							printf("error on line %d: ", linecounter);
							printf("missing to close \"VALUE\" tag\n");
							return 18;
						}
						aux += 1;
						*aux2 = '\0';
						allocaux = (char*) malloc(sizeof(char)*(strlen(aux) + 1));
						if(allocaux == NULL)
						{
							printf("could not allocate char* name\n");
							return 15;
						}
						strcpy(allocaux, aux);
						inusetype->types[i-1] = allocaux;
						*aux2 = '<';
						aux = aux2+1;
					break;

					case MARK_TYPE:
						if(addingType == NLF_True)
						{
							printf("error on line %d: ", linecounter);
							printf("This tag is not reconized inside <ADDTYPE>\n");
							return 21;
						}else if(inuse == NULL)
						{
							printf("error on line %d: ", linecounter);
							printf("the \"TYPE\" tag should be in a element\n");
							return 25;
						}else if(inuse->type != t_noTypeYet)
						{
							printf("error on line %d: ", linecounter);
							printf("double type.");
							if(inuse->type == t_elements)
								printf(" An element wichi is a set of others elements already has a defined type.");
							printf("\n");
							return 26;
						}

						aux = strstr(aux, ">");
						aux2 = strstr(aux, "</TYPE>");
						if(aux == NULL || aux2 == NULL)
						{
							printf("error on line %d: ", linecounter);
							printf("missing to close \"TYPE\" tag\n");
							return 18;
						}
						aux++;
						*aux2 = '\0';
						allocaux = (char*) malloc(sizeof(char)*(strlen(aux) + 1));
						if(allocaux == NULL)
						{
							printf("could not allocate char* name\n");
							return 15;
						}
						*aux2 = '<';
						etype = typeExist(aux, aux2, &inuse->customTypeName);
						if(etype == t_error)
						{
							printf("error on line %d: ", linecounter);
							printf("type not reconized\n");
							return 24;
						}
						inuse->type = etype;

						free(allocaux);
						allocaux = 	NULL;
						aux = aux2+1;
					break;

					case MARK_NEED_MAX:
					case MARK_NEED_MIN:
						if(addingType == NLF_True)
						{
							printf("error on line %d: ", linecounter);
							printf("This tag is not reconized inside <ADDTYPE>\n");
							return 21;
						}else if(inuse == NULL)
						{
							printf("error on line %d: ", linecounter);
							printf("<NEED_MIN> and <NEED_MAX> shall be inside an element\n");
							return 28;
						}

						aux = strstr(aux, ">");
						if(mark == MARK_NEED_MIN)
							aux2 = strstr(aux, "</NEED_MIN>");
						else
							aux2 = strstr(aux, "</NEED_MAX>");
						if(aux == NULL || aux2 == NULL)
						{
							printf("error on line %d: ", linecounter);
							printf("missing to close \"NEED_MIN\" or \"NEED_MAX\" tag\n");
							return 18;
						}
						aux++;

						//checking if number
						for(i = 0; aux + i != aux2; i++)
						{
							if(aux[i] < '0' || aux[i] > '9')
							{
								printf("error on line %d: ", linecounter);
								printf("\"NEED_MIN\" and \"NEED_MAX\" tag requires a positive integer number.\n");
								return 27;
							}
						}

						*aux2 = '\0';
						if(mark == MARK_NEED_MIN)
							inuse->qtdmin = atoi(aux);
						else
							inuse->qtdmax = atoi(aux);
						*aux2 = '<';

						if(inuse->qtdmax != -1 && inuse->qtdmin != -1  && inuse->qtdmin > inuse->qtdmax)
						{
							printf("error on line %d: ", linecounter);
							printf("The minimum quantity of the element cannot be bigger than the maximum.\n");
							return 29;
						}
					break;
				}
			}else{
				switch(mark)
				{
					case MARK_ERROR:
						printf("error on line %d: ", linecounter);
						printf("markdown not reconized\n \"%s\"\n", line);
						return 10;
					break;

					//case MARK_ROOT nothing to be done

					case MARK_ADDTYPE:
						for(i = 0; ctypes[i] != NULL && ctypes[i]->closed == NLF_True; i++);

						if(ctypes[i] == NULL && inusetype == ctypes[i])
						{
							printf("error on line %d: ", linecounter);
							printf("no <ADDTYPE> found to be closed\n");
							return 22;
						}else if(ctypes[i]->types[0] == NULL && inusetype == ctypes[i])
						{
							printf("error on line %d: ", linecounter);
							printf("trying to close an <ADDTYPE> without adding any value\n");
							return 23;
						}

						inusetype->closed = NLF_True;
						inusetype = NULL;
						addingType = NLF_False;
					break;

					case MARK_ELEMENT:
						if(inuse != NULL)
						{
							for(i = elevetsize-2; i >= 0 && elements[i]->insideof != inuse; i--);
							
							if(inuse->type == t_noTypeYet && i >= 0)
							{ //there  is one elemenet inside de element in use
								inuse->type = t_elements;
							}else if(inuse->type == t_noTypeYet)
							{
								printf("error on line %d: ", linecounter);
								printf("element nor has a set of elements nor has the tab \"TYPE\" defined\n");
								return 17;
							}

							inuse->closed = NLF_True;

							for(i = elevetsize-2; i >= 0 && elements[i]->closed == NLF_True; i--);

							if(i < 0)
								inuse = NULL;
							else
								inuse = elements[i];
						}else{
							printf("error on line %d: ", linecounter);
							printf("no element to be closed\n");
							return 16;
						}
					break;

					//cases MARK_NAME, MARK_TYPE, MARK_NEED_MIN, MARK_NEED_MAX, MARK_VALUE: nothing to be done
				}
			}
		aux = aux2 + 1;
		}
		if(aux != NULL && *aux == '\0')
			aux = NULL;
	}while(aux != NULL);
	/**************/

	return 0;
}

static NLF_bool finalCheck(NLF_bool showRules)
{
/*
	check if all elements where correctly colsed
	and may also show the rules learned is showRules == NLF_True
*/
	int i, i2;

	if(showRules == NLF_True)
	{
		for(i = 0; ctypes[i] != NULL; i++)
		{
			printf("\n");
			printf("ctypes[i]->name %s\n", ctypes[i]->name);
			if(ctypes[i]->types != NULL)
				for(i2 = 0; ctypes[i]->types[i2] != NULL; i2++)
					printf("ctypes[i]->types[%d] %s\n", i2, ctypes[i]->types[i2]);
			printf("ctypes[i]->closed %d\n", ctypes[i]->closed);
		}
	}

	for(i = 0; elements[i] != NULL; i++)
	{
		if(elements[i]->closed == NLF_False)
		{
			printf("error: element \"%s\" wasn't closed\n", elements[i]->name);
			return NLF_False;
		}
		if(showRules == NLF_True)
		{
			printf("\n");
			printf("elements[i]->type %d\n", elements[i]->type);
			printf("elements[i]->name %s\n", elements[i]->name);
			printf("elements[i]->qtdmin %d\n", elements[i]->qtdmin);
			printf("elements[i]->qtdmax %d\n", elements[i]->qtdmax);
			if(elements[i]->insideof != NULL)
				printf("elements[i]->insideof->name %s\n", elements[i]->insideof->name);
			else
				printf("elements[i]->insideof (NULL)\n");
			printf("elements[i]->closed %d\n", elements[i]->closed);
		}
	}

	if(showRules == NLF_False)
		printf("Rules learned correctly.\n");

	return NLF_True;
}

static int ruleApplier(char *line)
{
	char *aux, *aux2;
	short int i;
	CheckerElement **checkaux;
	short int checkeri = 0;

	aux = line;
	do{
		aux = strstr(aux, "<");
		if(aux != NULL)
		{
			aux++;
			aux2 = strstr(aux, ">");
			if(*aux != '/')
			{
				//search the if the element that needs to be created was defined in rules
				*aux2 = '\0';
				for(i = 0; elements[i] != NULL && strcmp(aux, elements[i]->name) != 0; i++);
				*aux2 = '>';
				if(elements[i] == NULL)
				{
					printf("error on line %d: ", linecounter);
					printf("Element not reconized\n");
					return 30;
				}
				checkeri = checkersize-1;
				checkersize++;
				checkaux = (CheckerElement**) realloc(checkerele, sizeof(CheckerElement*)*checkersize);
				if(checkaux == NULL)
				{
					printf("could not reallocate checker element's vector\n");
					return 31;
				}
				checkerele = checkaux;
				checkerele[checkersize-1] = NULL;
				checkerele[checkeri] = (CheckerElement*) malloc(sizeof(CheckerElement));
				if(checkerele[checkeri] == NULL)
				{
					printf("could not allocate elementc cheker\n");
					return 32;
				}
				checkerele[checkeri]->elrule = elements[i];
				checkerele[checkeri]->qtd = 1;
				checkerele[checkeri]->closed = NLF_False;
			}else{
			}
		}
		if(aux != NULL && *aux == '\0')
			aux = NULL;
	}while(aux != NULL);

	return 0;
}

int NLF_parser_check(char *parser_rules, char *parser_xml)
{
	char *line;
	char *aux, *aux2, *conductor;
	FILE *inputfile;
	short int i, i2;
	NLF_bool onComment = NLF_False;

	ctypes = (CustomType**) malloc(sizeof(CustomType*));
	elements = (Element**) malloc(sizeof(Element*));
	checkerele = (CheckerElement**) malloc(sizeof(CheckerElement*));
	if(elements == NULL || ctypes == NULL)
	{
		printf("could not allocate memory\n");
		exit(1);
	}
	elements[0] = NULL;
	elevetsize = 1;
	ctypes[0] = NULL;
	ctypessize = 1;
	checkerele[0] = NULL;
	checkersize = 1;


	inputfile = fopen(parser_rules, "r");
	if(inputfile == NULL)
	{
		printf("could not open \"%s\"\n", parser_rules);
		exit(2);
	}

	/*LEARNING RULES*/
	line = NULL;
	linecounter = 0;
	while(readLine(inputfile, &line, NLF_False) != EOF)
	{
		onComment = lookForCommentary(line, NLF_False);

		if(onComment == NLF_False || *line != '\0')
		{
			i = syntaxChecker(line);
			if(i != 0)
				closeByError(i, inputfile, line);

			i = interpeter(line);
			if(i != 0)
				closeByError(i, inputfile, line);
		}
	}

	if(finalCheck(NLF_False) == NLF_False)
		closeByError(17, inputfile, line);
	/****************/

	fclose(inputfile);
	inputfile = fopen(parser_xml, "r");
	if(inputfile == NULL)
	{
		printf("could not open \"%s\"\n", parser_xml);
		closeByError(2, inputfile, line);
	}

	/*READING THE FILE AND APPLYING THE RULES*/
	linecounter = 0;
	while(readLine(inputfile, &line, NLF_False) != EOF)
	{
		onComment = lookForCommentary(line, NLF_True);

		i = syntaxChecker(line);
		if(i != 0)
			closeByError(i, inputfile, line);

		i = ruleApplier(line);
		if(i != 0)
			closeByError(i, inputfile, line);
	}
	/*****************************************/

	closeByError(0, inputfile, line);
	return 0;
}