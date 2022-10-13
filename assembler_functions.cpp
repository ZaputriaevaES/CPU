#include "assembler_functions.h"

void start_program(const char * nameReadFile, struct Text * original_file, FILE * assembler_input, struct label ** array_of_labels)
{
    original_file->buffer = createBufferWithFread(nameReadFile, &original_file->amountOfElements, assembler_input, &original_file->amountOfStrings);

    fclose(assembler_input);

    original_file->strings  =  (char **)calloc(original_file->amountOfStrings, sizeof(char *));
    assert(original_file->strings != NULL);

    createArrayOfStrings (original_file->amountOfStrings, original_file->buffer, original_file->strings);

    *array_of_labels  =  (struct label *)calloc(MAX_NUMBER_OF_LABELS, sizeof(struct label));
    assert (*array_of_labels != NULL);
    memset (*array_of_labels, -1 , MAX_NUMBER_OF_LABELS * sizeof(struct label));

}

void finish_program(struct Text * original_file, struct label * array_of_labels, FILE * assembler_output)
{
    free(original_file->buffer);
    free(original_file->strings);
    free(array_of_labels);

    fclose(assembler_output);
}

void file_compilation(struct Text * original_file, FILE * assembler_output, struct label * array_of_labels, int compile_number, Elem_t * Regs)
{
    size_t number_of_cmd = 0;

    int * command_array = (int *)calloc(original_file->amountOfStrings*2, sizeof(int));
    assert(command_array != NULL);

    for(int line = 0; line < original_file->amountOfStrings; line++)
    {
        char cmd[20] = "";

        int n = 0;
        sscanf(original_file->strings[line], "%19s%n", cmd, &n);

        if(strncmp(cmd, "push", 4) == 0)
            {

                if(strchr(original_file->strings[line], '[') != NULL)
                {

                    int val = 0;

                    if(sscanf(original_file->strings[line] + n + 2, "%d", &val))
                    {
                        command_array[number_of_cmd] = PUSH_MrI;
                        command_array[number_of_cmd + 1] = val;

                    }
                    else
                    {
                        command_array[number_of_cmd] = PUSH_MrI;
                        char arg[4] = "";
                        sscanf(original_file->strings[line] + n + 2, "%s", arg);

                        if     (strncmp(arg, "rax", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RAX;
                        else if(strncmp(arg, "rbx", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RBX;
                        else if(strncmp(arg, "rcx", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RCX;
                        else if(strncmp(arg, "rdx", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RDX;
                    }

                }
                else
                {

                    int val = 0;

                    if(sscanf(original_file->strings[line] + n + 1, "%d", &val))
                    {
                        command_array[number_of_cmd] = PUSH_mrI;
                        command_array[number_of_cmd + 1] = val;
                    }
                    else
                    {
                        command_array[number_of_cmd] = PUSH_mRi;
                        char arg[4] = "";
                        sscanf(original_file->strings[line] + n + 1, "%s", arg);

                        if     (strncmp(arg, "rax", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RAX;
                        else if(strncmp(arg, "rbx", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RBX;
                        else if(strncmp(arg, "rcx", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RCX;
                        else if(strncmp(arg, "rdx", 3) == 0)
                            command_array[number_of_cmd + 1] = REG_RDX;
                    }
                }

                number_of_cmd += 2;

            }
        else if(strncmp(cmd, "pop", 3) == 0)
            {
                command_array[number_of_cmd] = POP;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "add", 3) == 0)
            {
                command_array[number_of_cmd] = ADD;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "sab", 3) == 0)
            {
                command_array[number_of_cmd] = SAB;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "mul", 3) == 0)
            {
                command_array[number_of_cmd] = MUL;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "div", 3) == 0)
            {
                command_array[number_of_cmd] = DIV;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "out", 3) == 0)
            {
                command_array[number_of_cmd] = OUT;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "hlt", 3) == 0)
            {
                command_array[number_of_cmd] = HLT;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "dump", 4) == 0)
            {
                command_array[number_of_cmd] = DUMP;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "in", 2) == 0)
            {
                command_array[number_of_cmd] = IN;
                number_of_cmd += 1;
            }
        else if(strncmp(cmd, "jmp", 3) == 0)
            {
                command_array[number_of_cmd] = JMP;

                if(strchr(original_file->strings[line] + n + 1, ':') != NULL)
                {
                    int label = 0;
                    sscanf(original_file->strings[line] + n + 2, "%d", &label);

                    command_array[number_of_cmd + 1] = label_search(label, array_of_labels);

                    printf("\n==== %d ===\n", label);
                }

                else
                {
                    int label = 0;
                    sscanf(original_file->strings[line] + n + 1, "%d", &label);

                    command_array[number_of_cmd + 1] = label;
                }

                number_of_cmd += 2;

            }
        else if(strchr(cmd, ':') != NULL)
            {

                int num_label = 0;
                if(sscanf(original_file->strings[line], "%d", &num_label) != 0)
                {
                    add_label_to_array(num_label, number_of_cmd, array_of_labels);

                    if(compile_number == 1)
                    {
                        command_array[number_of_cmd] = num_label;
                        number_of_cmd += 1;
                    }
                }
                else
                {
                    char str_label[MAX_LABEL_LENGTH] = "";
                    sscanf(original_file->strings[line], "%s", str_label);
                }
            }
        else if(strncmp(cmd, "dup", 3) == 0)
            {
                command_array[number_of_cmd] = DUP;
                number_of_cmd += 1;
            }
        else
            {
                printf("no such command exists, number_of_cmd = %d\n", number_of_cmd);
            }
    }

    listing_output(command_array, number_of_cmd);
    label_array_output(array_of_labels);

    if(compile_number == 2)
    {
        fprintf(assembler_output, "%s %d %d ", SIGNATURE, VERSION, number_of_cmd);

        for(size_t i = 0; i < number_of_cmd; i++)
        {
            fprintf(assembler_output, "%d ", command_array[i]);
        }

        free(command_array);
    }
}

void push_check(int push_cmd)
{

}

/*
void add_label_to_array(char * label, int number_of_cmd, struct label * array_of_labels)
{
    //(array_of_labels[label]).literal_mean = number_of_cmd;
}
*/

void add_label_to_array(int label, int number_of_cmd, struct label * array_of_labels)
{

    if(label < MAX_NUMBER_OF_LABELS)
            (array_of_labels[label]).hop_address = number_of_cmd;
    else
        {
            printf("you have exceeded the maximum number of tags");
            abort();
        }

}

int label_search(int label, struct label * array_of_labels)
{

    if(label < MAX_NUMBER_OF_LABELS)
            return (array_of_labels[label]).hop_address;
    else
        {
            printf("you have exceeded the maximum number of tags");
            abort();
        }

}

void listing_output(int * command_array, int number_of_cmd)
{
    for(int cmd = 0; cmd < number_of_cmd; cmd++)
    {
       if(command_array[cmd] == PUSH_mrI)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, PUSH_mrI, command_array[cmd + 1], "PUSH_mrI", command_array[cmd + 1]);
            cmd++;
       }

       else if(command_array[cmd] == PUSH_mRi)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, PUSH_mRi, command_array[cmd + 1], "PUSH_mRi", command_array[cmd + 1]);
            cmd++;
       }

       else if(command_array[cmd] == PUSH_MrI)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, PUSH_MrI, command_array[cmd + 1], "PUSH_MrI", command_array[cmd + 1]);
            cmd++;
       }

       else if(command_array[cmd] == PUSH_MRi)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, PUSH_MRi, command_array[cmd + 1], "PUSH_MRi", command_array[cmd + 1]);
            cmd++;
       }

       else if(command_array[cmd] == JMP)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JMP, command_array[cmd + 1], "JUMP", command_array[cmd + 1]);
            cmd++;
       }

       else
       {
            printf("%04d  %02d     --  %s    \n", cmd, command_array[cmd], command_decoder(command_array[cmd]));
       }
    }
    printf("\n---------------\n");
}

char * command_decoder(int cmd)
{
    switch(cmd)
            {
            case POP:
                {
                    return "pop";
                    break;
                }
            case ADD:
                {
                    return "add";
                    break;
                }
            case SAB:
                {
                    return "sab";
                    break;
                }
            case MUL:
                {
                    return "mul";
                    break;
                }
            case DIV:
                {
                    return "div";
                    break;
                }
            case OUT:
                {
                    return "out";
                    break;
                }
            case HLT:
                {
                    return "hlt";
                    break;
                }
            case DUMP:
                {
                    return "dump";
                    break;
                }
            case IN:
                {
                    return "in";
                    break;
                }
            case DUP:
                {
                    return "dup";
                    break;
                }
            default:
                return "no such command exists";
            }
}

void label_array_output(struct label * array_of_labels)
{
    printf("\n\n-------LABELS--------\n");

    for(int i = 0; i < MAX_NUMBER_OF_LABELS; i++)
    {
        printf("labels[%04d] = %4d\n", i, (array_of_labels[i]).hop_address);
    }

    printf("\n---------------------\n\n");
}

void createArrayOfStrings(const size_t strNum, char * pBUF, char ** textStruct)
{
    assert(pBUF != NULL);
    assert(textStruct != NULL);

    for(size_t i = 0; i < strNum; i++)
    {
        pBUF = takeLineFromBuffer(&textStruct[i], pBUF);
    }
}

char * createBufferWithFread(const char * nameReadFile, size_t * elements, FILE * readFile, size_t * strNum)
{
    assert(readFile != NULL);

    *elements = fileSizeDetection(nameReadFile, readFile);

    char * buffer = (char *)calloc(*elements, sizeof(char));

    assert(buffer != NULL);

    fread(buffer, sizeof(char), *elements, readFile);

    *strNum = countNumberOfRows(buffer, *elements);

    *elements -= *strNum - 1;

    buffer = (char *)realloc(buffer, (*elements) * sizeof(char));

    assert(buffer != NULL);

    return buffer;
}

size_t fileSizeDetection(const char * nameReadFile, FILE * read)
{
    assert(read != NULL);

    struct stat stbuf;

    stat(nameReadFile, &stbuf);

    off_t fsize = stbuf.st_size;

    return fsize / sizeof(char) ;
}

char * takeLineFromBuffer(char ** string, char * pBUF)
{
    assert(string != NULL);
    assert(pBUF   != NULL);

    *string = pBUF;

    int num = 0;

    while(*pBUF != '\n')
    {
        pBUF++;
        num++;
    }

    *pBUF = '\0';
    pBUF++;

    //string->strLen = num;

    return pBUF;
}

size_t countNumberOfRows(char * BUF, size_t elements)
{
    assert(BUF != NULL);

    size_t n = 0; //количество символов новой строки
    for (size_t i = 0; i < elements; i++)
    {
        if (BUF[i] == '\n')
        {
            n++;
        }
    }

    n++;

    return n;
}
