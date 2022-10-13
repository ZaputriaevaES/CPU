#include "processor_functions.h"

const char *logFileName = "log.txt";
FILE * logFile = fileCtor(logFileName);

int main()
{
    const char * nameReadFile =  "C://Users//zaput//Documents//CPU//PROCESSOR//processor_input.txt";
    const char * nameWriteFile = "C://Users//zaput//Documents//CPU//PROCESSOR//processor_output.txt";

    FILE * processor_input  = fopen(nameReadFile, "r");
    FILE * processor_output = fopen(nameWriteFile, "w+");

    struct Text translated_file = {NULL, 0};

    stack_t stack1 = {};       //общая функция открытия и инициализации всего
    stackCtor (stack1, 6);     //сделать сигнатуру как число
    Elem_t value = 0;

    Elem_t RAM[MAX_RAM_SIZE] = {0};

    Elem_t Regs[4] = {3, 8, 2, 4};

//--------------------------------Создание буфера с помощью fread------------------------------------------------------

    char * buffer = createBufferWithFread(nameReadFile, &translated_file.amount_of_cmd, processor_input);

    fclose(processor_input);

    buffer = input_file_verification(buffer, &translated_file);

    create_array_of_commands(buffer, &translated_file);

    command_execution(&translated_file, processor_output, &stack1, Regs, RAM);

    fclose(processor_output);

    stackDtor (&stack1);
    fileDtor(logFile);

    free(buffer);
    free(translated_file.commands_array);

    return 0;
}
