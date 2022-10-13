#include "assembler_functions.h"

int main()
{

    const char * nameReadFile =  "C://Users//zaput//Documents//CPU//ASSEMBLER//assembler_input.txt";
    const char * nameWriteFile = "C://Users//zaput//Documents//CPU//ASSEMBLER//assembler_output.txt";

    FILE * assembler_input  = fopen(nameReadFile, "r");
    FILE * assembler_output = fopen(nameWriteFile, "wb+");

    Elem_t Regs[4]           = {3, 8, 2, 5};

    struct Text original_file = {NULL, NULL, 0, 0};

    struct label * array_of_labels  = NULL;

    start_program(nameReadFile, &original_file, assembler_input, &array_of_labels);

    file_compilation(&original_file, assembler_output, array_of_labels, 1, Regs);
    file_compilation(&original_file, assembler_output, array_of_labels, 2, Regs);

    finish_program(&original_file, array_of_labels, assembler_output);
}
