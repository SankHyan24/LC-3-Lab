#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXN 65536
#define NONE 30583
#define OS_SP 12288	   //ϵͳջָ��x3000(12288)
#define USER_PSR 32770 //x8002
#define USER_PC 12288  //x3000
const int BIT[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
int Mem[MAXN]; //�ڴ��ַ,x0000~xFFFF(65535),��ʼ��Ϊx7777
int R[8];	   //�Ĵ���R0~R7,��ʼ��Ϊx7777(30583)
int PC, PSR;
int MAR, MDR, N, Z, P;
int Save_SSP, Save_USP;
char IR[1000];
int ORIG(char[]);	//�������ʼ��ַ
void Input(char[]); //�������ָ��ת����int����Mem��
int value(int, int);

int Decode(int); //����ָ��,���������IR��,����ָ������
void setcc(int); //����NZP��ֵ
void displayer()
{
	// system("cls");
	for (int i = 0; i < 8; i++)
	{
		printf("    R[%d] is %04hX", i, R[i]);
		if (i % 4 == 3)
			printf("\n");
	}
	printf("    IR is x%s   PC is x%04hx\n", IR, PC);
	// system("pause");
}
void Init();
void Work();
void ADD();	 //0001(2)=1 (10)
void AND();	 //0101(2)=5 (10)
void BR();	 //0000(2)=0 (10)
void JMP();	 //1100(2)=12(10)
void JSR();	 //0100(2)=4 (10)
void LD();	 //0010(2)=2 (10)
void LDI();	 //1010(2)=10(10)
void LDR();	 //0110(2)=6 (10)
void LEA();	 //1110(2)=14(10)
void NOT();	 //1001(2)=9 (10)
void ST();	 //0011(2)=3 (10)
void STI();	 //1011(2)=11(10)
void STR();	 //0111(2)=7 (10)
void HALT(); //1111(2)=15(10)

int main()
{
	freopen("ins.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	Init();
	Work();
	return 0;
}
void Init()
{
	int i = 0, Begin;
	for (i = 0; i <= 7; i++)
		R[i] = NONE;
	for (i = 0; i < MAXN; i++)
		Mem[i] = NONE;

	char tmp[1000];
	while (1)
	{
		gets(tmp);
		Begin = ORIG(tmp);
		if (Begin == -1)
			continue;
		break;
	}
	while (gets(tmp))
		Input(tmp);
	PC = Begin;
}
void Work()
{
	while (1)
	{
		MAR = PC;
		PC = PC + 1;
		MDR = Mem[MAR];
		int Now = Decode(MDR);

		if (Now == 1)
			ADD();
		else if (Now == 5)
			AND();
		else if (Now == 0)
			BR();
		else if (Now == 12)
			JMP();
		else if (Now == 4)
			JSR();
		else if (Now == 2)
			LD();
		else if (Now == 10)
			LDI();
		else if (Now == 6)
			LDR();
		else if (Now == 14)
			LEA();
		else if (Now == 9)
			NOT();
		else if (Now == 3)
			ST();
		else if (Now == 11)
			STI();
		else if (Now == 7)
			STR();
		else if (Now == 15)
		{
			HALT();
			break;
		}
		displayer();
	}
}
int ORIG(char tmp[])
{ //�������ʼ��ַ
	int len = strlen(tmp), flag = 0;
	int i = 0, ans = 0;
	for (i = 0; i < len; i++)
	{
		if (tmp[i] == ' ')
			continue;
		if (tmp[i] == ';')
			break;
		ans = ans * 2 + (tmp[i] - '0');
		flag++;
	}
	if (flag != 16)
		return -1;
	PC = ans;
	return ans;
}
void Input(char tmp[])
{ //�������ָ��ת����int����Mem��
	int len = strlen(tmp), flag = 0;
	int i = 0, ans = 0;
	for (i = 0; i < len; i++)
	{
		if (tmp[i] == ' ')
			continue;
		if (tmp[i] == ';')
			break;
		ans = ans * 2 + (tmp[i] - '0');
		flag++;
	}
	if (flag != 16)
		return;
	Mem[PC] = ans;
	PC++;
}
int value(int R, int L)
{ //��R~L�����ֺ�������ֵ,��;:����Rx�Լ�����λ��չ
	char tmp[16];
	int ans = 0, i, flag = 1;
	for (i = 0; i <= 15; i++)
		tmp[i] = IR[i];
	if (tmp[R] == 1 && R - L + 1 > 3)
	{							 //�������λΪ����,��ô����������,���ȡ��
		for (i = R; i >= L; i--) //ȡ��
			if (tmp[i] == 1)
				tmp[i] = 0;
			else
				tmp[i] = 1;
		for (i = L; i <= R; i++) //��1
			if (tmp[i] == 1)
				tmp[i] = 0;
			else
			{
				tmp[i] = 1;
				break;
			}

		for (i = R; i >= L; i--)
			ans = ans * 2 + tmp[i];
		ans = -ans;
	}
	else
	{
		for (i = R; i >= L; i--)
			ans = ans * 2 + tmp[i];
	}
	return ans;
}

int Decode(int tmp)
{ //����ָ��,���������IR��,����ָ������
	int i = 0, type = 0;
	for (i = 15; i >= 0; i--)
	{
		if (tmp & BIT[i])
			IR[i] = 1;
		else
			IR[i] = 0;
	}
	for (i = 15; i >= 12; i--)
		type = type * 2 + IR[i];
	return type;
}
void setcc(int tmp)
{ //����NZP��ֵ
	if (tmp & BIT[15])
	{
		N = 1, Z = 0, P = 0;
		return;
	}

	int flag = 0, i;
	for (i = 14; i >= 0; i--)
		if (tmp & BIT[i])
			flag = 1;

	if (flag)
		N = 0, Z = 0, P = 1;
	else
		N = 0, Z = 1, P = 0;
}
void ADD()
{ //0001(2)=1 (10)
	int DR = 0, SR1 = 0, SR2 = 0, Imm5 = 0, i;
	DR = value(11, 9);
	SR1 = value(8, 6);
	SR2 = value(2, 0);
	Imm5 = value(4, 0);

	if (IR[5] == 0)
		R[DR] = R[SR1] + R[SR2];
	else
		R[DR] = R[SR1] + Imm5;
	R[DR] %= 65536;
	setcc(R[DR]);
}
void AND()
{ //0101(2)=5 (10)
	int DR = 0, SR1 = 0, SR2 = 0, Imm5 = 0, i;
	DR = value(11, 9);
	SR1 = value(8, 6);
	SR2 = value(2, 0);
	Imm5 = value(4, 0);

	if (IR[5] == 0)
		R[DR] = R[SR1] & R[SR2];
	else
		R[DR] = R[SR1] & Imm5;

	setcc(R[DR]);
}
void BR()
{ //0000(2)=0 (10)
	int n = IR[11], z = IR[10], p = IR[9];
	int PC_offset9 = 0, i;
	PC_offset9 = value(8, 0);

	if ((n & N) || (z & Z) || (p & P))
		PC = PC + PC_offset9;
}
void JMP()
{ //1100(2)=12(10)
	int BaseR = 0, i;
	BaseR = value(8, 6);

	PC = R[BaseR];
}
void JSR()
{ //0100(2)=4 (10)
	int BaseR = 0, PC_offset11 = 0, i;
	BaseR = value(8, 6);
	PC_offset11 = value(10, 0);

	int temp = PC;
	if (IR[11] == 0)
		PC = R[BaseR];
	else
		PC = PC + PC_offset11;
	R[7] = temp;
}
void LD()
{ //0010(2)=2 (10)
	int DR = 0, PC_offset9 = 0, i;
	DR = value(11, 9);
	PC_offset9 = value(8, 0);

	R[DR] = Mem[PC + PC_offset9];
	setcc(R[DR]);
}
void LDI()
{ //1010(2)=10(10)
	int DR = 0, PC_offset9 = 0, i;
	DR = value(11, 9);
	PC_offset9 = value(8, 0);

	R[DR] = Mem[Mem[PC + PC_offset9]];
	setcc(R[DR]);
}
void LDR()
{ //0110(2)=6 (10)
	int DR = 0, BaseR = 0, offset6 = 0, i;
	DR = value(11, 9);
	BaseR = value(8, 6);
	offset6 = value(5, 0);

	R[DR] = Mem[R[BaseR] + offset6];
	setcc(R[DR]);
}
void LEA()
{ //1110(2)=14(10)
	int DR = 0, PC_offset9 = 0, i;
	DR = value(11, 9);
	PC_offset9 = value(8, 0);

	R[DR] = PC + PC_offset9;
}
void NOT()
{ //1001(2)=9 (10)
	int DR = 0, SR1 = 0, i;
	DR = value(11, 9);
	SR1 = value(8, 6);

	R[DR] = ~R[SR1];
	setcc(R[DR]);
}
void ST()
{ //0011(2)=3 (10)
	int SR = 0, PC_offset9 = 0, i;
	SR = value(11, 9);
	PC_offset9 = value(8, 0);

	Mem[PC + PC_offset9] = R[SR];
}
void STI()
{ //1011(2)=11(10)
	int SR = 0, PC_offset9 = 0, i;
	SR = value(11, 9);
	PC_offset9 = value(8, 0);

	Mem[Mem[PC + PC_offset9]] = R[SR];
}
void STR()
{ //0111(2)=7 (10)
	int SR = 0, BaseR = 0, offset6 = 0, i;
	SR = value(11, 9);
	BaseR = value(8, 6);
	offset6 = value(5, 0);

	Mem[R[BaseR] + offset6] = R[SR];
}
void HALT()
{ //1111(2)=15(10)
	int i = 0;
	for (i = 0; i <= 7; i++)
		printf("R%d = x%04hX\n", i, R[i]);
}
