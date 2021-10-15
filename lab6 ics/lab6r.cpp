#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TransAddrToNum(x) ((int)x - 0x3000)
#define GetLastN(x) (IR & ((1 << x) - 1))
#define setcc SetCC(r[dr])
typedef unsigned short ist;
bool power = 1;
ist r[8];
ist PC, IR;
int cc;
ist *memp;
ist *mem;
int counter = 0;
// void printf_bin(ist num)
// {
//     int i, j, k;
//     unsigned char *p = (unsigned char *)&num + 1; //p先指向num后面第3个字节的地址，即num的最高位字节地址
//     for (i = 0; i < 2; i++)                       //依次处理4个字节(32位）
//     {
//         j = *(p - i);            //取每个字节的首地址，从高位字节到低位字节，即p p-1 p-2 p-3地址处
//         for (k = 7; k >= 0; k--) //处理每个字节的8个位，注意字节内部的二进制数是按照人的习惯存储！
//             if (j & (1 << k))    //1左移k位，与单前的字节内容j进行或运算，如k=7时，00000000&10000000=0 ->该字节的最高位为0
//                 printf("1");
//             else
//                 printf("0");
//         printf(" "); //每8位加个空格，方便查看
//     }
// }
ist BinaryCodeGenerator(char *string)
{
    ist instruction = 0;
    for (int i = 0; i < 16; i++)
        if (string[15 - i] == '1')
            instruction = instruction | (1 << i);
    return instruction;
}
int ExtractN(ist src, int from, int to)
{
    ist mask = (1 << from) - (1 << (to - 1));
    return (mask & src) >> (to - 1);
}
ist SexN(ist from, int degree)
{
    ist stdnegone = (1 << degree) - 1;
    // printf("standard negative one is x%04hx\nfrom is x%04hx", stdnegone, from);
    ist pre = from & stdnegone, mask = 0b1111111111111111;
    // printf("After Clearified is x%04hx\n", pre);
    if (ExtractN(pre, degree, degree) == 1)
        pre = pre | (mask ^ stdnegone);
    // printf("Extended is x%04hx\n", pre);
    // system("pause");
    return pre;
}
void SetCC(ist src)
{
    if (ExtractN(src, 16, 16) == 1)
    {
        cc = -1;
        // printf("    .CC is set to -1\n");
    }
    else if (src == 0)
    {
        cc = 0;
        // printf("    .CC is set to 0\n");
    }
    else
    {
        cc = 1;
        // printf("    .CC is set to 1\n");
    }
}
void InstructionLoader()
{
    FILE *fp;
    char *wkcp;
    // if ((fp = fopen("./ins.txt", "r")) == NULL)
    // {
    //     printf("    >>Input any letter to exit.\n    >>");
    //     getchar();
    //     exit(-1);
    // }
    fp = stdin;
    char instruction[18];
    for (int i = 0; !feof(fp); i++, memp++)
    {
        fscanf(fp, "%s", instruction);
        if ((wkcp = strstr(instruction, "\n")) != NULL)
            *wkcp = '\0';
        if (i == 0)
        {
            memp = memp + TransAddrToNum(BinaryCodeGenerator(instruction));
            PC = BinaryCodeGenerator(instruction);
            // printf("The Begining Address is set as 0x%04hx\n", PC);
            memp--;
        }
        else
        {
            // printf("The string is %s. The Binary code of the instruction is ", instruction);
            *memp = BinaryCodeGenerator(instruction);
            // printf_bin(*memp);
            // printf("Address is %04hx\n", memp - mem + 0x3000);
        }
    }
}
void InstructionExecutor()
{
    counter++;
    IR = mem[(int)PC - 0x3000];
    // printf("\nNo.%d           IR: x%04hx , PC: x%04hx\n", counter, IR, PC);
    PC++;
    switch (IR >> 12)
    {
    case 0: //*BR
    {
        // printf("Executing BR instruction.\n      cc is %d\n    Condition is", cc);
        ist position = SexN(IR, 9) + PC;
        bool n = ExtractN(IR, 12, 12), z = ExtractN(IR, 11, 11), p = ExtractN(IR, 10, 10);
        // if (n)
        //     putchar('n');
        // if (z)
        //     putchar('z');
        // if (p)
        //     putchar('p');
        // putchar('\n');
        if ((n && cc == -1) || (z && cc == 0) || (p && cc == 1))
        {
            PC = position;
            // printf("    Postion is x%04hx. Jump Sucessfully.\n", position);
        }
        break;
    }
    case 1:
    {
        // printf("Executing ADD instruction.\n");
        int dr = ExtractN(IR, 12, 10);
        int sr = ExtractN(IR, 9, 7);
        if (ExtractN(IR, 6, 6) == 0)
        {
            // printf("%04hx(R[%d]) + %04hx(R[%d]) = %04hx(R[%d])\n", r[sr], sr, r[ExtractN(IR, 3, 1)], ExtractN(IR, 3, 1), r[sr] + r[ExtractN(IR, 3, 1)], dr);
            r[dr] = r[sr] + r[ExtractN(IR, 3, 1)];
        }
        else
        {
            // printf("%04hx(R[%d]) + %04hx(imm5) = %04hx(R[%d])\n", r[sr], sr, SexN(IR, 5), r[sr] + SexN(IR, 5), dr);
            r[dr] = r[sr] + SexN(IR, 5);
        }
        setcc;
        break;
    }
    case 2:
    {
        // printf("Executing LD instruction.\n");
        int dr = ExtractN(IR, 12, 10);
        // printf("DR is %d\n", dr);
        // printf("offset is x%04hx\n", SexN(IR, 9));
        r[dr] = mem[(int)(SexN(IR, 9) + PC) - 0x3000];
        setcc;
        break;
    }
    case 3:
    {
        // printf("Executing ST instruction.\n");
        int sr = ExtractN(IR, 12, 10);
        // printf("DR is %d\n", sr);
        // printf("offset is x%04hx\n", SexN(IR, 9));
        mem[(int)(SexN(IR, 9) + PC) - 0x3000] = r[sr];
    }
    case 4:
    {
        // printf("Executing JSR(R) instruction.\n");
        r[7] = PC;
        if (ExtractN(IR, 12, 12) == 1)
        {
            // printf("offset is x%04hx\n", SexN(IR, 11));
            PC += SexN(IR, 11);
        }
        else
            PC = r[ExtractN(IR, 9, 7)];
        break;
    }
    case 5:
    {
        // printf("Executing AND instruction.\n");
        int dr = ExtractN(IR, 12, 10);
        int sr = ExtractN(IR, 9, 7);
        if (ExtractN(IR, 6, 6) == 0)
            r[dr] = r[sr] & (ist)ExtractN(IR, 3, 1);
        else
            r[dr] = r[sr] & SexN(IR, 5);
        setcc;
        break;
    }
    case 6:
    {
        // printf("Executing LDR instruction.\n");
        int dr = ExtractN(IR, 12, 10), br = ExtractN(IR, 9, 7);
        r[dr] = mem[(int)(r[br] + SexN(IR, 6)) - 0x3000];
        setcc;
        break;
    }
    case 7:
    {
        // printf("Executing LDR instruction.\n");
        int dr = ExtractN(IR, 12, 10), br = ExtractN(IR, 9, 7);
        mem[(int)(r[br] + SexN(IR, 6)) - 0x3000] = r[dr];
        break;
    }
    case 8:
    {
        // printf("Executing RTI instruction.\n");
        break;
    }
    case 9:
    {
        // printf("Executing NOT instruction.\n");
        int dr = ExtractN(IR, 12, 10), sr = ExtractN(IR, 9, 7);
        r[dr] = (ist)~r[sr];
        setcc;
        break;
    }
    case 10:
    {
        // printf("Executing LDI instruction.\n");
        int dr = ExtractN(IR, 12, 10);
        r[dr] = mem[mem[(int)(PC + SexN(IR, 9)) - 0x3000]];
        setcc;
        break;
    }
    case 11:
    {
        // printf("Executing STI instruction.\n");
        int sr = ExtractN(IR, 12, 10);
        mem[mem[(int)(PC + SexN(IR, 9)) - 0x3000]] = r[sr];
        break;
    }
    case 12:
    {
        // printf("Executing JMP instruction.\n");
        PC = r[ExtractN(IR, 9, 7)];
        break;
    }
    case 13:
    {
        // printf("No instruction.\n");
        break;
    }
    case 14:
    {
        // printf("Executing LEA instruction.\n");
        int dr = ExtractN(IR, 12, 10);
        r[dr] = PC + SexN(IR, 9);
        break;
    }
    case 15:
    {
        // printf("Executing TRAP instruction.\n");
        if (IR = 0xF025)
            power = 0;
        break;
    }
    default:
        // printf("\nError\n");
        // system("pause");
        break;
    }
}
void displayer()
{
    // system("cls");
    for (int i = 0; i < 8; i++)
    {
        printf("R%d = x%04hX", i, r[i]);
        if (i != 10)
            printf("\n");
    }
    // printf("\ncc is %d  IR is x%04hx   PC is x%04hx", cc, IR, PC);
    // system("pause");
}
int main()
{
    memp = (ist *)malloc((0xFE00 - 0x3000) * sizeof(ist));
    memset(memp, 0x7777, (0xFE00 - 0x3000) * sizeof(ist));
    mem = memp;
    // mem[0x3100 - 0x3000] = 0b1110101010101010;
    InstructionLoader();
    for (int i = 0; i < 8; i++)
        r[i] = 0x7777;
    power = 1;
    while (power)
        InstructionExecutor();
    displayer();
    return 0;
}
