DEF_CMD(PUSH, 1, 1,
            {
            Elem_t * code = my_cpu->commands_array;

            Elem_t cmd = code[ip++];
            Elem_t arg = 0;

            if     ((cmd & ARG_IMMED)>>5) arg = code[ip++];
            else if((cmd & ARG_REG)>>6)   arg = my_cpu->Regs[code[ip++]];
            else if((cmd & ARG_REM)>>7)   arg = my_cpu->RAM[arg];
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
/*
            if ((cmd & ARG_REM)>>7)                                
            {                                                     
                int ram_ind = ;     
                my_cpu->RAM[ram_ind] = value;                  
            }                                                     
            else                                                  
            {                                                     
                int reg = my_cpu->code[ip++];                     
                my_cpu->Regs[reg] = value; 
*/
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
            exit(0);
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
            ip = my_cpu->commands_array[ip + 1] - 1;
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
            if(b < a) ip = my_cpu->commands_array[ip + 1] - 1;
            
            break;               
            }) 

DEF_CMD(JBE, 14, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b <= a) ip = my_cpu->commands_array[ip + 1] - 1;
            break;               
            }) 

DEF_CMD(JA, 15, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b > a) ip = my_cpu->commands_array[ip + 1] - 1;
            break;               
            }) 

DEF_CMD(JAE, 16, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b >= a) ip = my_cpu->commands_array[ip + 1] - 1;
            break;               
            }) 

DEF_CMD(JE, 17, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b == a) ip = my_cpu->commands_array[ip + 1] - 1 ;
            break;               
            }) 

DEF_CMD(JNE, 18, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b != a) ip = my_cpu->commands_array[ip + 1] - 1;
            break;               
            })

DEF_CMD(CALL, 19, 1,
            {
            stackPush (&call_stack, ip);
            
            ip = my_cpu->commands_array[ip + 1] - 1;

            printf("-----------------\ncall_stack:\n");

            for(size_t i = 0; i < call_stack.size; i++)
            {
                printf("[%d] = %d\n", i, call_stack.data[i]);
            }
            printf("-----------------\n"); 

            break;               
            })  

DEF_CMD(RET, 20, 0,
            {
            printf("-----------------\ncall_stack:\n");

            for(size_t i = 0; i < call_stack.size; i++)
            {
                printf("[%d] = %d\n", i, call_stack.data[i]);
            }
            printf("-----------------\n");

            int value = 0;    
            stackPop (&call_stack, &value); 
            ip = value + 1;

            break;               
            })  

DEF_CMD(SQRT, 21, 0,
            {
                int value = 0;
                stackPop  (stack, &value);
                stackPush (stack, sqrt(value));
            }) 


DEF_CMD(VERSION, 23, 0)