#include "processor_functions.h"

void start_program(struct Text * my_cpu, stack_t * stack1, const char * nameReadFile, FILE * processor_input)
{
    stack_t * call_stack = &(my_cpu->call_stack);

    stackCtor (*stack1, 6);     //сделать сигнатуру как число
    stackCtor (*call_stack, 6);    

    char * buffer = createBufferWithFread(nameReadFile, &(my_cpu->amount_of_cmd), processor_input);

    fclose(processor_input);

    buffer = input_file_verification(buffer, my_cpu);

    printf("%s\n", buffer);

    create_array_of_commands(buffer, my_cpu);

    for(int i = 0; i < MAX_RAM_SIZE; i++)
    {
        my_cpu->RAM[i] = i+1;
    }

    printf("\n------------RAM------------------\n");
    for(int i = 0; i < MAX_RAM_SIZE; i++)
    {
        printf("[%2d] = %20d; ", i, my_cpu->RAM[i]);
    }
    printf("\n---------------------------------\n"); 

    printf("\n------------REGS-----------------\n");
    for(int i = 0; i < 4; i++)
    {
        printf("[%2d] = %20d; ", i, my_cpu->Regs[i]);
    }
    printf("\n---------------------------------\n");    
}

#define DEF_CMD(name, num, arg, ...)         \
    case num:                                \
        {                                    \   
            if(num == HLT) printf("HLT\n");  \
            __VA_ARGS__                      \   
            break;                           \       
        }  

void command_execution(struct Text * my_cpu, FILE * processor_output, stack_t * stack)
{
    //for(int i = 0; i < my_cpu->amount_of_cmd; i++) printf("%d ", my_cpu->commands_array[i]);
    //("\n");

        stack_t call_stack = {};//&my_cpu->call_stack;
        stackCtor (call_stack, 16);    
    for(size_t ip = 0; ip < my_cpu->amount_of_cmd; ip++)
    {
        switch((my_cpu->commands_array[ip]) & 31)
        {
            #include "C:\Users\zaput\Documents\CPU\cmd.h"    
        /*    
        case PUSH:
            {
            //stackPush (stack, translated_file->commands_array[ip+1]);
            //ip += 1;

            Elem_t * code = translated_file->commands_array;

            Elem_t cmd = code[ip++];
            Elem_t arg = 0;

            if     (cmd & ARG_IMMED) arg = code[ip++];
            else if(cmd & ARG_REG)   arg = Regs[code[ip++]];
            else if(cmd & ARG_REM)   arg =  RAM[arg];
            else   printf("invalid arg\n");

            //Elem_t arg = push_cmd_check(translated_file->commands_array, &cmd);

            stackPush(stack, arg);

            break;
            }

        case POP:
            {
            Elem_t value = 0;
            stackPop (stack, &value);
            break;
            }
        case ADD:
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            stackPush (stack, b + a);
            break;
            }
        case SAB:
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            stackPush (stack, b - a);
            break;
            }
        case MUL:
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            stackPush (stack, b * a);
            break;
            }
        case DIV:
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(a != 0)
            {
                stackPush (stack, b / a);
            }
            break;
            }
        case OUT:
            {
            Elem_t value = 0;
            stackPop (stack, &value);
            printf("%d\n", value);
            break;
            }
        case HLT:
            {
            printf("goodbye");
            exit(0);
            break;
            }
        case DUMP:
            {
            printf("-----------------\nstack:\n");
            //printf("dump: %d\n", stack->data[stack->size - 1]);
            for(size_t i = 0; i < stack->size; i++)
            {
                printf("[%d] = %d\n", i, stack->data[i]);
            }
            printf("-----------------\n");
            break;
            }
        case IN:
            {
            Elem_t value = 0;
            printf("in : ");
            scanf("%d", &value);
            stackPush (stack, value);
            //printf("%d\n", stack->data[stack->size - 1]);
            break;
            }
        case JMP:
            {
            ip = translated_file->commands_array[ip + 1];
            break;
            }
        case DUP:
            {
            Elem_t value = 0;
            stackPop (stack, &value);
            stackPush (stack, value);
            stackPush (stack, value);
            break;
            }
        default:
            printf("no such command exists");
        
        */
        default:
            { 
            printf("no such command\n");
            printf("in command = %d (full : %d)\n", (my_cpu->commands_array[ip]) & 31, my_cpu->commands_array[ip]);
            }
        }
        #undef DEF_CMD

    }
}
/*
Elem_t push_check(Elem_t * code, size_t * ip)
{
    Elem_t cmd = code[(*ip)++];
    int arg = 0;

         if(cmd & 0x20) arg = code[(*ip)++];
    else if(cmd & 0x40) arg = Regs[code[(*ip)++]];
    else if(cmd & 0x80) arg =  RAM[arg];

    stackPush(code, arg);
}
*/
char * input_file_verification(char * buffer, struct Text * my_cpu)
{
    char sign[5] = "";
    int n = 0;
    sscanf(buffer, "%5s%n", sign, &n);

    if(strncmp(sign, SIGNATURE, 5) != 0)
    {
        printf("invalid format file");
        abort();
    }

    buffer += n + 1;

    int version = 0;
    sscanf(buffer, "%d", &version);

    if(version != VERSION)
    {
        printf("your version is outdated");
        abort();
    }

    buffer += 2;

    int k = 0;
    sscanf(buffer, "%d%n", &my_cpu->amount_of_cmd, &k);

    buffer += k + 1;

    //printf("%s\n", buffer);

    return buffer;
}

void create_array_of_commands(char * buffer, struct Text * my_cpu)
{
    my_cpu->commands_array  =  (int *)calloc(my_cpu->amount_of_cmd, sizeof(int));
    assert(my_cpu->commands_array != NULL);

    for(size_t i = 0; i < my_cpu->amount_of_cmd; i++)
    {
        int n = -1;
        sscanf(buffer, "%d%n", &my_cpu->commands_array[i], &n);

        printf("%d ", my_cpu->commands_array[i]);

        buffer += n;
    }   

    free(buffer);
}

char * createBufferWithFread(const char * nameReadFile, size_t * elements, FILE * readFile)
{
    assert(readFile != NULL);

    *elements = fileSizeDetection(nameReadFile, readFile);

    char * buffer = (char *)calloc(*elements, sizeof(char));

    assert(buffer != NULL);

    fread(buffer, sizeof(char), *elements, readFile);

    buffer = (char *)realloc(buffer, (*elements) * sizeof(char));

    assert(buffer != NULL);
    //printf("%s\n", buffer);
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

void finish_program(FILE * processor_output, stack_t * stack1, FILE * logFile, struct Text * my_cpu)
{
    fclose(processor_output);

    stackDtor (stack1);
    fileDtor(logFile);

    free(my_cpu->commands_array);
}