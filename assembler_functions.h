#ifndef ASSEMBLER_FUNCTIONS_H_INCLUDED
#define ASSEMBLER_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <malloc.h>

#include "C:\Users\zaput\Documents\CPU\utilities.h"

typedef int Elem_t;

/*
#define ARG_IMMED 0x20
#define ARG_REG   0x40
#define ARG_REM   0x80
*/

struct Text{
    char *  buffer;
    char ** strings;
    size_t  amountOfElements;
    size_t  amountOfStrings;
    };

struct label{
    int  hop_address;
    char literal_mean[MAX_LABEL_LENGTH];
    };

struct assemb{
    struct label * array_of_labels;
    int *  command_array;
    Elem_t Regs[4] = {17,18,19,20};
};    
   

#define    PUSH_mrI  ARG_IMMED +             1
#define    PUSH_mRi  ARG_REG   +             1
#define    PUSH_MrI  ARG_REM   + ARG_IMMED + 1
#define    PUSH_MRi  ARG_REM   + ARG_REG   + 1

#define    POP_MrI   ARG_REM   + ARG_IMMED + 2
#define    POP_mRi   ARG_REG   +             2

/*
enum commands{
    PUSH = 1,
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
*/

/*
#define DEF_CMD(name, num, arg, ...) name = num,

enum cmd{
    #include "C:\Users\zaput\Documents\CPU\cmd.h"
    };

#undef DEF_CMD
*/

/*
enum regs{
    REG_RAX = 1,
    REG_RBX = 2,
    REG_RCX = 3,
    REG_RDX = 4
    };

#define SIGNATURE "CP"
//#define VERSION 2
#define MAX_NUMBER_OF_LABELS 20
#define MAX_LABEL_LENGTH 20
*/


int    text_label_search    (char * text_lebel, struct label * array_of_labels);
int    check_what_not_adress(int cmd, struct assemb * assembler);
void   get_args             (char * line, size_t * number_of_cmd, int cmd_name,  struct assemb * assembler);
void   finish_program       (struct Text * original_file, struct label * array_of_labels, FILE * assembler_output);
void   start_program        (const char * nameReadFile, struct Text * original_file, FILE * assembler_input, struct assemb * assembler);
void   label_array_output   (struct label * array_of_labels);
char   * command_decoder    (int cmd);
void   listing_output       (int * command_array, int number_of_cmd);
int    label_search         (int label, struct label * array_of_labels);
void   add_label_to_array   (int label, int number_of_cmd, struct label * array_of_labels);
void   file_compilation     (struct Text * original_file, FILE * assembler_output, int compile_number, struct assemb * assembler);
void   createArrayOfStrings (const size_t strNum, char * pBUF, char ** textStruct);
char * createBufferWithFread(const char * nameReadFile, size_t * elements, FILE * readFile, size_t * strNum);
size_t fileSizeDetection    (const char * nameReadFile, FILE * read);
char * takeLineFromBuffer   (char ** string, char * pBUF);
size_t countNumberOfRows    (char * BUF, size_t elements);

#endif // ASSEMBLER_FUNCTIONS_H_INCLUDED
