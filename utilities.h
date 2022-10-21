
#define ARG_IMMED 0x20
#define ARG_REG   0x40
#define ARG_REM   0x80

#define DEF_CMD(name, num, arg, ...) name = num,

enum cmd{
    #include "cmd.h"
    };

#undef DEF_CMD

/*
enum commands{
    PUSH      = 1,
    //PUSH_mrI  = ARG_IMMED +             1,
    //PUSH_mRi  = ARG_REG   +             1,
    //PUSH_MrI  = ARG_REM   + ARG_IMMED + 1,
    //PUSH_MRi  = ARG_REM   + ARG_REG   + 1,
    POP       = 2,
    ADD       = 3,
    SAB       = 4,
    MUL       = 5,
    DIV       = 6,
    OUT       = 7,
    HLT       = 8,
    DUMP      = 9,
    IN        = 10,
    JMP       = 11,
    DUP       = 12,
    VERSION   = 14
    };
*/

enum regs{
    REG_RAX = 0,
    REG_RBX = 1,
    REG_RCX = 2,
    REG_RDX = 3
    };

#define SIGNATURE "CP"
//#define VERSION 1
#define MAX_RAM_SIZE 20
#define MAX_NUMBER_OF_LABELS 20
#define MAX_LABEL_LENGTH 20
