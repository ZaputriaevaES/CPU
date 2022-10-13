#ifndef PROCESSOS_FUNCTIONS_H_INCLUDED
#define PROCESSOS_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <malloc.h>

#include "C://Users//zaput//Documents//CPU//STACK//error_functions.h"
#include "C://Users//zaput//Documents//CPU//STACK//stack_functions.h"

#include "C:\Users\zaput\Documents\CPU\utilities.h"

struct Text{
    Elem_t * commands_array;
    size_t amount_of_cmd;
    };

/*
enum commands{
    PUSH = 1,
    //PUSH_mrI  = 33,
    //PUSH_mRi  = 65,
    //PUSH_MrI  = 161,
    //PUSH_MRi  = 193,
    POP = 2,
    ADD = 3,
    SAB = 4,
    MUL = 5,
    DIV = 6,
    OUT = 7,
    HLT = 8,
    DUMP = 9,
    IN = 10,
    JMP = 11,
    DUP = 12,
    VERSION = 14
    };

enum regs{
    REG_RAX = 1,
    REG_RBX = 2,
    REG_RCX = 3,
    REG_RDX = 4
    };

#define SIGNATURE "CP"
//#define VERSION 1
#define MAX_RAM_SIZE 100

#define ARG_IMMED 0x20
#define ARG_REG   0x40
#define ARG_REM   0x80
//#define
*/

void push_check(Elem_t * code, size_t * ip);
void command_execution(struct Text * translated_file, FILE * processor_output, stack_t * stack, Elem_t * Regs, Elem_t * RAM);
void create_array_of_commands(char * buffer, struct Text * translated_file);
char * input_file_verification(char * buffer, struct Text * translated_file);
char * createBufferWithFread(const char * nameReadFile, size_t * elements, FILE * readFile);
size_t fileSizeDetection(const char * nameReadFile, FILE * read);

#endif // PROCESSOS_FUNCTIONS_H_INCLUDED
