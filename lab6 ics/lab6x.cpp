#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TransAddrToNum(x) ((int)x - 0x3000)
#define SetPCas(x) PC = x
#define ValueofAddr(x) mem[(int)x - 0x3000]
#define GetLastN(x) (IR & ((1 << x) - 1))
#define SignExt(x, len) (((short)(x << (16 - len)) < 0) ? (short)(x | 0xFE00) : (short)((x << (16 - len)) >> (16 - len)))
#define DecodeDR (int)((IR << 4) >> 13)
#define DecodeSR1BR (int)((IR << 7) >> 10)
#define DecodeSR2 (int)(IR & 7)
#define SetCCBy(x) ((x < 0) ? (cc = 4) : ((x == 0) ? (cc = 2) : (cc = 1)))

typedef unsigned short ist;
bool power = 1;
ist R[8];
ist MCR;
ist PC, IR;
ist cc;
ist *memp;
ist *mem;
void InsADD();
void InsLD();
void InsST();
void InsJSR();
void InsAND();
void InsLDR();
void InsSTR();
void InsRTI(); //
void InsNOT();
void InsLDI();
void InsSTI();
void InsJMP();
void Ins1101();
void InsLEA();
void InsTRAP();
void lc3_smi()
{
    system("cls");
}
void printf_bin(ist num)
{
    int i, j, k;
    unsigned char *p = (unsigned char *)&num + 1; //p先指向num后面第3个字节的地址，即num的最高位字节地址

    for (i = 0; i < 2; i++) //依次处理4个字节(32位）
    {
        j = *(p - i);                //取每个字节的首地址，从高位字节到低位字节，即p p-1 p-2 p-3地址处
        for (int k = 7; k >= 0; k--) //处理每个字节的8个位，注意字节内部的二进制数是按照人的习惯存储！
        {
            if (j & (1 << k)) //1左移k位，与单前的字节内容j进行或运算，如k=7时，00000000&10000000=0 ->该字节的最高位为0
                printf("1");
            else
                printf("0");
        }
        printf(" "); //每8位加个空格，方便查看
    }
    printf("\r");
}
ist BinaryCodeGenerator(char *string);
void InstructionLoader()
{
    char instruction[18];
    gets(instruction);
    memp = memp + TransAddrToNum(BinaryCodeGenerator(instruction));
    SetPCas(BinaryCodeGenerator(instruction));
    // printf("The Begining Address is set as 0x%04hx\n", PC);
    gets(instruction);
    // scanf("%s", instruction);
    for (; instruction[0] != '#'; memp++)
    {
        *memp = BinaryCodeGenerator(instruction);
        // printf("The string is %s. The Binary code of the instruction is ", instruction);
        // printf_bin(BinaryCodeGenerator(instruction));
        // printf("\n");
        gets(instruction);
    }
}
void InstructionExecutor()
{

    IR = ValueofAddr(PC);
    PC++;
    ist instruction_selector = IR >> 12;
    printf("%d ", (int)(IR >> 12));
    switch ((int)(IR >> 12))
    {
    case 0:
    {
        printf("Executing BR instruction.\n");
        printf("    cc is %hd\n", cc);
        ist PCoffset9 = SignExt(GetLastN(9), 9);
        ist nzp = (IR << 4) >> 13;
        printf("    Your code is %hd\n", nzp);
        if (nzp & cc != 0)
        {
            printf("Jump to:x%04hx\n", PCoffset9);
            PC += PCoffset9;
        }
        break;
    }
    case 1:
    {
        printf("Executing ADD instruction.\n");
        printf("    DR is R%d:%04hx\n", DecodeDR, R[DecodeDR]);
        if (IR & 32 != 0)
        {
            printf("    SR1 is R%d:%04hx\n", DecodeSR1BR, R[DecodeSR1BR]);
            printf("    imm5 is %d\n", SignExt(GetLastN(5), 5));
            R[DecodeDR] = R[DecodeSR1BR] + SignExt(GetLastN(5), 5);
        }
        else
        {
            printf("    SR1 is R%d:%04hx\n", DecodeSR1BR, R[DecodeSR1BR]);
            printf("    SR2 is R%d:%04hx\n", DecodeSR2, R[DecodeSR2]);
            R[DecodeDR] = R[DecodeSR1BR] + R[DecodeSR2];
        }
        SetCCBy(R[DecodeDR]);
        printf("Set cc as x%04hx\n", R[DecodeDR]);
        break;
    }
    case 2:
        InsLD();
        break;
    case 3:
        InsST();
        break;
    case 4:
        InsJSR();
        break;
    case 5:
    {
        printf("Executing AND instruction.\n");
        printf("    DR is R%d:%04hx\n", DecodeDR, R[DecodeDR]);
        if (IR & 32 != 0)
        {
            printf("    SR1 is R%d:%04hx\n", DecodeSR1BR, R[DecodeSR1BR]);
            printf("    imm5 is %d\n", SignExt(GetLastN(5), 5));
            R[DecodeDR] = R[DecodeSR1BR] & SignExt(GetLastN(5), 5);
        }
        else
        {
            printf("    SR1 is R%d:%04hx\n", DecodeSR1BR, R[DecodeSR1BR]);
            printf("    SR2 is R%d:%04hx\n", DecodeSR2, R[DecodeSR2]);
            R[DecodeDR] = R[DecodeSR1BR] & R[DecodeSR2];
        }
        SetCCBy(R[DecodeDR]);
        printf("Set cc as x%04hx\n", R[DecodeDR]);
        break;
    }
    case 6:
        InsLDR();
        break;
    case 7:
        InsSTR();
        break;
    case 8:
        InsRTI();
        break;
    case 9:
        InsNOT();
        break;
    case 10:
        InsLDI();
        break;
    case 11:
        InsSTI();
        break;
    case 12:
        InsJMP();
        break;
    case 13:
        Ins1101();
        break;
    case 14:
        InsLEA();
        break;
    case 15:
        printf("IR is %04hx \n", IR);
        if (IR == 0b1111000000100101)
            for (int i = 0; i < 8; i++)
            {
                printf("R%d = x%04hx\n", i, R[i]);
            }
        printf("Over\n");
        power = 0;
        break;
    default:
        printf("Wrong Input as %d", (int)(IR >> 12));
        exit(1);
        break;
    }
    printf("PC=x%04hx\n", PC);
}
int main()
{
    memp = (ist *)malloc((0xFE00 - 0x3000) * sizeof(ist));
    mem = memp;
    InstructionLoader();
    while (power)
        InstructionExecutor();
}
ist BinaryCodeGenerator(char *string)
{
    ist instruction = 0;
    for (int i = 0; i < 16; i++)
        if (string[15 - i] == '1')
            instruction = instruction | (1 << i);
    return instruction;
}
// void InsBR()
// {
//     printf("Executing BR instruction.\n");
//     ist PCoffset9 = SignExt(GetLastN(9), 9);
//     ist nzp = (IR << 4) >> 13;
//     if (nzp & cc != 0)
//         PC += PCoffset9;
// }
// void InsADD()
// {
//     printf("Executing ADD instruction.\n");
//     if (IR & 32 != 0)
//         R[DecodeDR] = R[DecodeSR1BR] + SignExt(GetLastN(5), 5);
//     else
//         R[DecodeDR] = R[DecodeSR1BR] + R[DecodeSR2];
//     SetCCBy(R[DecodeDR]);
// }
void InsLD()
{
    printf("Executing LD instruction.\n");
    R[DecodeDR] = ValueofAddr(PC + SignExt(GetLastN(9), 9));
    SetCCBy(R[DecodeDR]);
}
void InsST()
{
    printf("Executing ST instruction.\n");
    ValueofAddr(PC + SignExt(GetLastN(9), 9)) = R[DecodeDR];
}
void InsJSR()
{
    printf("Executing JSR instruction.\n");
    R[7] = PC;
    if ((IR & 1 << 11) != 0)
        PC += SignExt(GetLastN(9), 9);
    else
        PC = DecodeSR1BR;
}
// void InsAND()
// {
//     printf("Executing AND instruction.\n");
//     if (IR & 32 != 0)
//         R[DecodeDR] = R[DecodeSR1BR] & SignExt(GetLastN(5), 5);
//     else
//         R[DecodeDR] = R[DecodeSR1BR] & R[DecodeSR2];
//     SetCCBy(R[DecodeDR]);
// }
void InsLDR()
{
    printf("Executing LDR instruction.\n");
    R[DecodeDR] = ValueofAddr(R[DecodeSR1BR] + SignExt(GetLastN(6), 6));
    SetCCBy(R[DecodeDR]);
}
void InsSTR()
{
    printf("Executing STR instruction.\n");
    ValueofAddr(R[DecodeSR1BR] + SignExt(GetLastN(6), 6)) = R[DecodeDR];
}
void InsRTI()
{
    printf("Executing RTI instruction.\n");
    exit(-1);
} //
void InsNOT()
{
    printf("Executing NOT instruction.\n");
    R[DecodeDR] = ~R[DecodeSR1BR];
    SetCCBy(R[DecodeDR]);
}
void InsLDI()
{
    printf("Executing LDI instruction.\n");
    R[DecodeDR] = ValueofAddr(PC + SignExt(GetLastN(9), 9));
    SetCCBy(R[DecodeDR]);
}
void InsSTI()
{
    printf("Executing STI instruction.\n");
    ValueofAddr(PC + SignExt(GetLastN(9), 9)) = R[DecodeDR];
}
void InsJMP()
{
    printf("Executing JMP instruction.\n");
    PC = R[DecodeSR1BR];
}
void Ins1101() { exit(-1); }
void InsLEA()
{
    printf("Executing LEA instruction.\n");
    R[DecodeSR1BR] = PC + SignExt(GetLastN(9), 9);
}
void InsTRAP()
{
    printf("Executing TRAP instruction.\n");
    printf("IR is %04hx \n", IR);
    // if (IR == 0b1111000000100101)
    for (int i = 0; i < 8; i++)
    {
        printf("R%d = x%04hx\n", i, R[i]);
    }
    printf("Over\n");
    power = 0;
}