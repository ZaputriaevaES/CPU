#include "assembler_functions.h"

void start_program(const char * nameReadFile, struct Text * original_file, FILE * assembler_input, struct assemb * assembler)
{
    original_file->buffer = createBufferWithFread(nameReadFile, &original_file->amountOfElements, assembler_input, &original_file->amountOfStrings);

    fclose(assembler_input);

    original_file->strings  =  (char **)calloc(original_file->amountOfStrings, sizeof(char *));
    assert(original_file->strings != NULL);

    createArrayOfStrings (original_file->amountOfStrings, original_file->buffer, original_file->strings);

    assembler->array_of_labels  =  (struct label *)calloc(MAX_NUMBER_OF_LABELS, sizeof(struct label));
    assert (assembler->array_of_labels != NULL);
    memset (assembler->array_of_labels, -1 , MAX_NUMBER_OF_LABELS * sizeof(struct label));

}

void finish_program(struct Text * original_file, struct label * array_of_labels, FILE * assembler_output)
{
    free(original_file->buffer);
    free(original_file->strings);
    free(array_of_labels);

    fclose(assembler_output);
}


#define DEF_CMD(name, num, arg, ...) \
        else if(stricmp(cmd, #name) == 0)                                                                                                         \
            {                                                                                                                                     \
                if(num == RDMP) printf("RDMP:\n");                                                                                                \
                assembler->command_array[number_of_cmd++] = num;                                                                                  \
                if (arg) get_args(original_file->strings[line] + n, &number_of_cmd, num, assembler);  \
            }    


void file_compilation(struct Text * original_file, FILE * assembler_output, int compile_number, struct assemb * assembler)
{
    size_t number_of_cmd = 0;

    assembler->command_array = (int *)calloc(original_file->amountOfStrings*2, sizeof(int));
    assert(assembler->command_array != NULL);

    int number_of_string_labels = 0;

    for(int line = 0; line < original_file->amountOfStrings; line++)
    {
        char cmd[20] = "";

        int n = 0;
        sscanf(original_file->strings[line], "%19s%n", cmd, &n);
        
        
        //printf("%d - %s\n", line, cmd);

        if(strchr(cmd, ':') != NULL)
            {

                int num_label = 0;
                if(sscanf(original_file->strings[line], "%d", &num_label) != 0)
                {
                    //printf("###  label : %d ###\n",num_label);

                    add_label_to_array(num_label, number_of_cmd, assembler->array_of_labels);
                }
                else if(sscanf(original_file->strings[line], "%s", assembler->array_of_labels[number_of_string_labels].literal_mean))
                {
                    int len = strlen(assembler->array_of_labels[number_of_string_labels].literal_mean);
                    (assembler->array_of_labels[number_of_string_labels].literal_mean)[len-1] = '\0';

                    add_label_to_array(number_of_string_labels, number_of_cmd, assembler->array_of_labels);
                    //add_str_label_to_array(number_of_string_labels, number_of_cmd, assembler->array_of_labels);
                    //printf("%d_th find text label = %s\n", number_of_string_labels, assembler->array_of_labels[number_of_string_labels].literal_mean);
                    number_of_string_labels++; 
                    //printf("==== nosl = %d ===\n", number_of_string_labels);
                }
                else
                {
                    printf("invalid label format\n");
                }
            }            
        
        #include "C:\Users\zaput\Documents\CPU\cmd.h"

        #undef DEF_CMD

        else
            {
                printf("no such command exists, number_of_cmd = %d cmd = %s \n", number_of_cmd, cmd);
            }
        
    }
    /*
    for(int i = 0; i < number_of_cmd; i++)
    {
         printf("%5d", assembler->command_array[i]);
    }
    printf("\n");
    */
    label_array_output(assembler->array_of_labels);
    listing_output(assembler->command_array, number_of_cmd);

    //printf("text_label = %s\n", assembler->array_of_labels[0].literal_mean);

    if(compile_number == 2)
    {
        fprintf(assembler_output, "%s %d %d ", SIGNATURE, VERSION, number_of_cmd);

        for(size_t i = 0; i < number_of_cmd; i++)
        {
        /*
       else if(command_array[cmd] == JMP || command_array[cmd] == JB || command_array[cmd] == JBE || command_array[cmd] == JA || command_array[cmd] == JAE || command_array[cmd] == JN || command_array[cmd] == JNE )
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JMP, command_array[cmd + 1], "JUMP", command_array[cmd + 1]); 
            cmd++;

            fprintf(assembler_output, "%d ", assembler->command_array[i]);
       }

       else if(command_array[cmd] == CALL)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, CALL, command_array[cmd + 1], "CALL", command_array[cmd + 1]); 
            cmd++;
       }               
        */    
            fprintf(assembler_output, "%d ", assembler->command_array[i]);
        }

        free(assembler->command_array);
    }
}

int check_what_not_adress(int cmd, struct assemb * assembler)
{
    for(int i = 0; i < MAX_NUMBER_OF_LABELS; i++)
    {
        if((assembler->array_of_labels[i]).hop_address != -1)
        {
            printf("lab = %d, h.a. = %d\n", (assembler->array_of_labels[i]).hop_address);
        }
    }
    printf("\n--------\n");
    return 1;
}

void get_args(char * line, size_t * number_of_cmd, int cmd_name, struct assemb * assembler)
{
    (*number_of_cmd)--;
    //printf("=== line: %s cmd_name = %d ==\n", line, cmd_name);

    if(cmd_name == PUSH)
    {
         if(strchr(line, '[') != NULL)
        {
            int val = 0;

            if(sscanf(line + 2, "%d", &val)) //push[10]
            {
                printf("in cmd = %d, PUSH_MrI[%d]\n", *number_of_cmd, val);
                assembler->command_array[*number_of_cmd] = PUSH_MrI;
                assembler->command_array[*number_of_cmd + 1] = val;
            }
            else   //push[rax]
            {
                assembler->command_array[*number_of_cmd] = PUSH_MRi;
                char arg[4] = "";
                sscanf(line + 2, "%s", arg);

                if     (strncmp(arg, "rax", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RAX;
                else if(strncmp(arg, "rbx", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RBX;
                else if(strncmp(arg, "rcx", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RCX;
                else if(strncmp(arg, "rdx", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RDX;
            }

        }
        else
        {

            int val = 0;

            if(sscanf(line + 1, "%d", &val))  //push 10
            {
                assembler->command_array[*number_of_cmd] = PUSH_mrI;
                assembler->command_array[*number_of_cmd + 1] = val;
            }
            else  //push rax
            {
                assembler->command_array[*number_of_cmd] = PUSH_mRi;
                char arg[4] = "";
                sscanf(line + 1, "%s", arg);

                if     (strncmp(arg, "rax", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RAX;
                else if(strncmp(arg, "rbx", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RBX;
                else if(strncmp(arg, "rcx", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RCX;
                else if(strncmp(arg, "rdx", 3) == 0)
                    assembler->command_array[*number_of_cmd + 1] = REG_RDX;
            }
        }


    }
    else if(cmd_name == JMP || cmd_name == JB || cmd_name == JBE || cmd_name == JA || cmd_name == JAE || cmd_name == JE || cmd_name == JNE || cmd_name == CALL)
    {
        if(strchr(line + 1, ':') != NULL)
        {
            int label = 0;
            if(sscanf(line + 2, "%d", &label))
            {
                assembler->command_array[*number_of_cmd + 1] = label_search(label, assembler->array_of_labels);
            }
            else
            {
                char text_lebel[MAX_LABEL_LENGTH];
                if(sscanf(line + 2, "%s", text_lebel))
                {
                    assembler->command_array[*number_of_cmd + 1] = text_label_search(text_lebel, assembler->array_of_labels);
                }
            }
            //printf("\n==== label = %d , adress = %d ===\n", label, label_search(label, assembler->array_of_labels));
        }

        else
        {
            int label = 0;
            sscanf(line + 1, "%d", &label);

            assembler->command_array[*number_of_cmd + 1] = label;
        }

    }
    else if(cmd_name == POP)       
    {
    if(strchr(line, '[') != NULL)  //pop [10]
        {
            int val = 0;

            sscanf(line + 2, "%d", &val);
            assembler->command_array[*number_of_cmd] = POP_MrI; //
            assembler->command_array[*number_of_cmd + 1] = val;
        }
    else  //pop rax
    {
        assembler->command_array[*number_of_cmd] = POP_mRi; //
        char arg[4] = "";
        sscanf(line + 1, "%s", arg);

        if     (strncmp(arg, "rax", 3) == 0)
            assembler->command_array[*number_of_cmd + 1] = REG_RAX;
        else if(strncmp(arg, "rbx", 3) == 0)
            assembler->command_array[*number_of_cmd + 1] = REG_RBX;
        else if(strncmp(arg, "rcx", 3) == 0)
            assembler->command_array[*number_of_cmd + 1] = REG_RCX;
        else if(strncmp(arg, "rdx", 3) == 0)
            assembler->command_array[*number_of_cmd + 1] = REG_RDX;
    }

    }
    
    else
    {printf("ERROR CMD");}

        (*number_of_cmd) += 2;
}

/*
void push_check(int push_cmd)
{

}
*/
/*
void add_label_to_array(char * label, int number_of_cmd, struct label * array_of_labels)
{
    //(array_of_labels[label]).literal_mean = number_of_cmd;
}
*/

void add_label_to_array(int label, int number_of_cmd, struct label * array_of_labels)
{
    //printf("lb in funk = %d, numb_of_cmd = %d\n", label, number_of_cmd);

    if(label < MAX_NUMBER_OF_LABELS)
    {
        (array_of_labels[label]).hop_address = number_of_cmd;
        //printf("hop_adresss = %d\n", (array_of_labels[label]).hop_address);
    }    
    else
    {
        printf("you have exceeded the maximum number of tags");
        abort();
    }

}
/*
void add_str_label_to_array(size_t number_of_string_labels, int number_of_cmd, struct label * array_of_labels)
{
    if(number_of_string_labels < MAX_NUMBER_OF_LABELS)
    {
        (array_of_labels[number_of_string_labels]).hop_address = number_of_cmd;
        //printf("hop_adresss = %d\n", (array_of_labels[label]).hop_address);
    }    
    else
    {
        printf("you have exceeded the maximum number of tags");
        abort();
    }
}
*/
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

int text_label_search(char * text_lebel, struct label * array_of_labels)
{
    for(int i = 0; i <= MAX_NUMBER_OF_LABELS; i++)
    {
        //printf("%d_th t_l = %s ~~ %s\n", i, array_of_labels[i].literal_mean, text_lebel);
        if(strncmp(text_lebel, array_of_labels[i].literal_mean, MAX_LABEL_LENGTH) == NULL)
        {
            //printf("%d_th mas text label = %s hop_adress = %d\n", i, array_of_labels[i].literal_mean, array_of_labels[i].hop_address);
            return array_of_labels[i].hop_address; 
        }
    } 
    printf("!!! text_label not found !!!\n");
    return 0;   
}

void listing_output(int * command_array, int number_of_cmd)
{
    for(int cmd = 0; cmd < number_of_cmd; cmd++)
    {
        //printf("cmd = %7d  ", command_array[cmd]);
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

       else if(command_array[cmd] == JB)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JB, command_array[cmd + 1], "JB", command_array[cmd + 1]); 
            cmd++;
       }

       else if(command_array[cmd] == JBE)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JBE, command_array[cmd + 1], "JBE", command_array[cmd + 1]); 
            cmd++;
       }

       else if(command_array[cmd] == JA)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JA, command_array[cmd + 1], "JA", command_array[cmd + 1]); 
            cmd++;
       }
       
       else if(command_array[cmd] == JAE)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JAE, command_array[cmd + 1], "JAE", command_array[cmd + 1]); 
            cmd++;
       }

       else if(command_array[cmd] == JE)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JE, command_array[cmd + 1], "JE", command_array[cmd + 1]); 
            cmd++;
       }
       
       else if(command_array[cmd] == JNE)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, JNE, command_array[cmd + 1], "JNE", command_array[cmd + 1]); 
            cmd++;
       }

       else if(command_array[cmd] == CALL)
       {
            printf("%04d  %02d %2d  --  %s %d\n", cmd, CALL, command_array[cmd + 1], "CALL", command_array[cmd + 1]); 
            cmd++;
       }       

       else if(command_array[cmd] == POP_MrI)
       {
           printf("%04d  %02d %2d  --  %s %d\n", cmd, POP_MrI, command_array[cmd + 1], "POP_MrI", command_array[cmd + 1]);
            cmd++;
       }

       else if(command_array[cmd] == POP_mRi)
       {
           printf("%04d  %02d %2d  --  %s %d\n", cmd, POP_mRi, command_array[cmd + 1], "POP_mRi", command_array[cmd + 1]);
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
            case RET:
                {
                    return "ret";
                    break;  
                }    
            case SQRT:
                {
                    return "sqrt";
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
