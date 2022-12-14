DEF_CMD(PUSH, 1, 1,
            {
            Elem_t * code = my_cpu->commands_array;

            Elem_t cmd = code[ip++];
            Elem_t arg = 0;

            //printf("\n push_cmp = %d, I = %d, R = %d, M = %d\n", cmd, cmd & ARG_IMMED, cmd & ARG_REG, cmd & ARG_REM);

            if((cmd & ARG_IMMED)>>5) arg += code[ip];
            if((cmd & ARG_REG)>>6)   arg += my_cpu->Regs[code[ip]];
            if((cmd & ARG_REM)>>7)   arg  = my_cpu->RAM[arg];
            //else   printf("invalid arg in cmd_num = %d\n", ip);

            //printf("push(cmd=) %d, I = %d, R = %d, M = %d, arg = %d / %d\n", code[ip-1], (cmd & ARG_IMMED)>>5, (cmd & ARG_REG)>>6, (cmd & ARG_REM)>>7, code[ip], arg);

            stackPush(stack, arg);
            //ip--;
            break;
            })

DEF_CMD(POP, 2, 1,
{
            Elem_t value = 0;
            stackPop (stack, &value);
            
            Elem_t * code = my_cpu->commands_array;

            Elem_t cmd = code[ip++];

            printf("\n pop_cmp = %d, I = %d, R = %d, M = %d\n", cmd, cmd & ARG_IMMED, cmd & ARG_REG, cmd & ARG_REM);

            if ((cmd & ARG_REM)>>7)   //pop [10]  -> pop 10                           
            {                                                     
                int ram_ind = code[ip];     
                my_cpu->RAM[ram_ind] = value;                  
            }                                                     
            else          //pop rax  -> pop 1                                      
            {                                                     
                int reg = code[ip];
                printf("old_reg = %d, reg = %d; ", my_cpu->Regs[reg], code[ip]);                                     
                my_cpu->Regs[reg] = value; 
                printf("new_reg = %d, value = %d\n", my_cpu->Regs[reg], value);
            }

            printf("pop(cmd=) %d, I = %d, R = %d, M = %d, arg = %d\n", code[ip-1], (cmd & ARG_IMMED)>>5, (cmd & ARG_REG)>>6, (cmd & ARG_REM)>>7, code[ip]);

            //ip--;
            
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
            printf("MUL: %d * %d = %d\n", b, a , b * a);
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
            printf("DUP %d\n", value);
            break;
            })

DEF_CMD(JB, 13, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            printf("JB? %d < %d, jb to %d\n", b, a, my_cpu->commands_array[ip + 1] - 1);            
            if(b < a) ip = my_cpu->commands_array[ip + 1] - 1;
            else ip++;
            
            break;               
            }) 

DEF_CMD(JBE, 14, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b <= a) ip = my_cpu->commands_array[ip + 1] - 1;
            else ip++;
            break;               
            }) 

DEF_CMD(JA, 15, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b > a) ip = my_cpu->commands_array[ip + 1] - 1;
            else ip++;
            break;               
            }) 

DEF_CMD(JAE, 16, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b >= a) ip = my_cpu->commands_array[ip + 1] - 1;
            else ip++;
            break;               
            }) 

DEF_CMD(JE, 17, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            printf("JE? %d == %d, je to %d\n", b, a, my_cpu->commands_array[ip + 1] - 1);
            if(b == a) ip = my_cpu->commands_array[ip + 1] - 1;
            else ip++;
            break;               
            }) 

DEF_CMD(JNE, 18, 1,
            {
            Elem_t a = 0;
            Elem_t b = 0;
            stackPop (stack, &a);
            stackPop (stack, &b);
            if(b != a) ip = my_cpu->commands_array[ip + 1] - 1;
            else ip++;
            break;               
            })

DEF_CMD(CALL, 19, 1,
            {
            stackPush (&call_stack, ip);

            printf("CALL: %d \n",my_cpu->commands_array[ip + 1]);
            
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
                printf("SQRT: old_val = %d, new_val = %g\n", value, sqrt(value));                
                stackPush (stack, sqrt(value));
            }) 

DEF_CMD(RDMP, 22, 0,
            {
                printf("\n------------REGS-----------------\n");
                for(int i = 0; i < 4; i++)
                {
                    printf("[%2d] = %20d; ", i, my_cpu->Regs[i]);
                }
                printf("\n---------------------------------\n");                
            }) 


DEF_CMD(VERSION, 23, 0)