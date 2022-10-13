#include "processor_functions.h"

#define DEF_CMD(name, num, arg, ...) \
    case num:                        \
        {                            \   
            __VA_ARGS__              \   
            break;                   \       
        }  

void command_execution(struct Text * translated_file, FILE * processor_output, stack_t * stack, Elem_t * Regs, Elem_t * RAM)
{
    for(size_t ip = 0; ip < translated_file->amount_of_cmd; ip++)
    {
        switch((translated_file->commands_array[ip]) & 31)
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
        }
        
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
char * input_file_verification(char * buffer, struct Text * translated_file)
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

    sscanf(buffer, "%d", &translated_file->amount_of_cmd);

    buffer += 2;

    return buffer;
}

void create_array_of_commands(char * buffer, struct Text * translated_file)
{
    translated_file->commands_array  =  (int *)calloc(translated_file->amount_of_cmd, sizeof(int));
    assert(translated_file->commands_array != NULL);

    for(size_t i = 0; i < translated_file->amount_of_cmd; i++)
    {
        int n = 0;
        sscanf(buffer, "%d%n", &translated_file->commands_array[i], &n);

        //printf("%d ", translated_file->commands_array[i]);

        buffer += n;
    }
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
