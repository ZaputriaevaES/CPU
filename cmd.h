DEF_CMD(PUSH, 1, 1,
            {
            Elem_t * code = translated_file->commands_array;

            Elem_t cmd = code[ip++];
            Elem_t arg = 0;

            if     ((cmd & ARG_IMMED)>>5) arg = code[ip++];
            else if((cmd & ARG_REG)>>6)   arg = Regs[code[ip++]];
            else if((cmd & ARG_REM)>>7)   arg =  RAM[arg];
            else   printf("invalid arg in cmd_num = %d\n", ip);

            printf("push(cmd=) %d, IMR = %d, arg = %d\n", code[ip-2], (cmd & ARG_IMMED)>>5, code[ip-1]);

            stackPush(stack, arg);
            ip--;
            break;
            })

DEF_CMD(POP, 2, 0,
{
            Elem_t value = 0;
            stackPop (stack, &value);
            break;
})

DEF_CMD(ADD, 3, 0,
{
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            stackPush (stack, b + a);
})

DEF_CMD(SAB, 4, 0,
{
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            stackPush (stack, b - a);
})

DEF_CMD(MUL, 5, 0,
{
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            stackPush (stack, b * a);
})

DEF_CMD(DIV, 6, 0,
{
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            stackPush (stack, b / a);
})

DEF_CMD(OUT, 7, 0,
            {
            Elem_t value = 0;
            stackPop (stack, &value);
            printf("%d\n", value);
            break;
            })

DEF_CMD(HLT, 8, 0,
            {
            printf("goodbye");
            //exit(0);
            break;
            })

DEF_CMD(DUMP, 9, 0,
            {
            printf("-----------------\nstack:\n");

            for(size_t i = 0; i < stack->size; i++)
            {
                printf("[%d] = %d\n", i, stack->data[i]);
            }
            printf("-----------------\n");
            break;
            })

DEF_CMD(IN, 10, 0,
            {
            Elem_t value = 0;
            printf("in : ");
            scanf("%d", &value);
            stackPush (stack, value);
            break;
            })

DEF_CMD(JMP, 11, 1,
            {
            printf("jjjjmmmppp  to %d \n", translated_file->commands_array[ip + 1]);    
            ip = translated_file->commands_array[ip + 1];
            break;
            })

DEF_CMD(DUP, 12, 0,
            {
            Elem_t value = 0;
            stackPop (stack, &value);
            stackPush (stack, value);
            stackPush (stack, value);
            break;
            })

DEF_CMD(JB, 13, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b < a) ip = translated_file->commands_array[ip + 1];
            break;               
            }) 

DEF_CMD(JBE, 14, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b <= a) ip = translated_file->commands_array[ip + 1];
            break;               
            }) 

DEF_CMD(JA, 15, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b > a) ip = translated_file->commands_array[ip + 1];
            break;               
            }) 

DEF_CMD(JAE, 16, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b >= a) ip = translated_file->commands_array[ip + 1];
            break;               
            }) 

DEF_CMD(JE, 17, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b == a) ip = translated_file->commands_array[ip + 1];
            break;               
            }) 

DEF_CMD(JNE, 18, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b != a) ip = translated_file->commands_array[ip + 1];
            break;               
            })            

DEF_CMD(VERSION, 19, 0)