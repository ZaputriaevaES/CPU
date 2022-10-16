#include "assembler_functions.h"

int main()
{
    const char * nameReadFile =  "C://Users//zaput//Documents//CPU//ASSEMBLER//assembler_input.txt";
    const char * nameWriteFile = "C://Users//zaput//Documents//CPU//ASSEMBLER//assembler_output.txt";

    FILE * assembler_input  = fopen(nameReadFile, "r");
    FILE * assembler_output = fopen(nameWriteFile, "wb+");

    //Elem_t Regs[4] = {3, 8, 2, 5};

    struct Text original_file = {NULL, NULL, 0, 0};

    //struct label * array_of_labels = NULL;

    struct assemb assembler = {};

    for(int i = 0; i < 4; i++)
    {
        printf("-- %d -- \n", assembler.Regs[i]);
    }

    start_program(nameReadFile, &original_file, assembler_input,  &assembler);

    file_compilation(&original_file, assembler_output, 1, &assembler);
    file_compilation(&original_file, assembler_output, 2, &assembler);

    finish_program(&original_file, assembler.array_of_labels, assembler_output);
}
