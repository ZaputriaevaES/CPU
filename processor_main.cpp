#include "processor_functions.h"

const char *logFileName = "log.txt";
FILE * logFile = fileCtor(logFileName);

int main()
{
    //const char * nameReadFile =  "C://Users//zaput//Documents//CPU//PROCESSOR//processor_input.txt";
    const char * nameWriteFile = "C://Users//zaput//Documents//CPU//PROCESSOR//processor_output.txt";
    const char * nameReadFile =  "C://Users//zaput//Documents//CPU////ASSEMBLER//assembler_output.txt";

    FILE * processor_input  = fopen(nameReadFile, "r");
    FILE * processor_output = fopen(nameWriteFile, "w+");

    struct Text my_cpu = {};//{NULL, 0, 0, 0, 0};

    stack_t stack1 = {};   
/* 
    stackCtor (stack1, 6);     //сделать сигнатуру как число
    stackCtor (my_cpu.call_stack, 6);    

    //Elem_t value = 0;
    //Elem_t RAM[MAX_RAM_SIZE] = {0};
    //Elem_t Regs[4] = {3, 8, 2, 4};

//--------------------------------Создание буфера с помощью fread------------------------------------------------------

    char * buffer = createBufferWithFread(nameReadFile, &my_cpu.amount_of_cmd, processor_input);

    fclose(processor_input);

    buffer = input_file_verification(buffer, &my_cpu);

    create_array_of_commands(buffer, &my_cpu);
*/
    start_program(&my_cpu, &stack1, nameReadFile, processor_input);

    command_execution(&my_cpu, processor_output, &stack1);

    printf("\n------------RAM(main)------------------\n");
    for(int i = 0; i < MAX_RAM_SIZE; i++)
    {
        printf("[%2d] = %20d; ", i, my_cpu.RAM[i]);
    }
    printf("\n---------------------------------------\n"); 

    printf("\n------------REGS(main)-----------------\n");
    for(int i = 0; i < 4; i++)
    {
        printf("[%2d] = %20d; ", i, my_cpu.Regs[i]);
    }
    printf("\n---------------------------------------\n"); 

    finish_program(processor_output, &stack1, logFile, &my_cpu);
    return 0;
}
