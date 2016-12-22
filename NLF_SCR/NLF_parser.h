#ifndef NLF_PARSER_H
#define NLF_PARSER_H

/*CLIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
/************/

/*EXTERNAL LIBRARIES*/
/*********************/

/*INTERNAL LIBRARIES*/
//#include "NLF_error.h"
/********************/

/*ENUM AND TYPES*/
typedef enum NLF_bool
{
	NLF_False = 0, NLF_True = 1
} NLF_bool;

typedef enum Eltype
{
	t_error = -1, t_noTypeYet, t_root, t_int, t_float, t_double, t_string, t_elements, t_custom
} Eltype;

typedef enum Markdowns
{
	MARK_ERROR = -1, MARK_ROOT, MARK_ADDTYPE, MARK_ELEMENT, MARK_NAME, MARK_VALUE, MARK_TYPE, MARK_NEED_MIN, MARK_NEED_MAX
} Markdowns;

typedef struct Element
{
	Eltype type;
	char *customTypeName; //if the element type isn't a custom one, this'll be NULL
	char *name;
	int qtdmin;
	int qtdmax;
	struct Element *insideof;
	NLF_bool closed;
} Element;

typedef struct CustomType
{
	char *name;
	char **types;
	NLF_bool closed;
} CustomType;

typedef struct CheckerElement
{
	Element *elrule;
	int qtd;
	NLF_bool closed;
} CheckerElement;
/****************/

/*GLOBAL VARIABLES*/
/******************/

/*LOCAL GLOBAL VARIABLES*/
static Element **elements;
static short int elevetsize;
static CustomType **ctypes;
static short int ctypessize;
static CheckerElement **checkerele;
static short int checkersize;
static short int linecounter;
/************************/

/*GLOBAL FUNCTIONS*/
int NLF_parser_check(char *parser_rules, char *parser_xml);
/******************/

/*LOCAL FUNTIONS*/
static Markdowns whichMarkdown(char *s0, char *sf);
static Eltype typeExist(char *tname0, char *tnamef, char **tobeset);
static void closeByError(int exitnumber, FILE *to_close, char *theLine);
static int readLine(FILE *rules, char **line, NLF_bool showLine);
static NLF_bool lookForCommentary(char *line, NLF_bool showLine);
static short int syntaxChecker(char *line);
static int interpeter(char *line);
static NLF_bool finalCheck(NLF_bool showRules);
static int ruleApplier(char *line);
/****************/

#endif