#include <stdio.h>
#include <string.h>

#define MAX 100000

struct strack
{
	char ch[MAX];
	int top; //标记栈顶
};

struct strack bracket;

int kong(int num)
{
	if(num == -1)
		return 0; //返回０为空
	else
		return 1;
}

int man(int num)
{
	if(num == MAX-1)
		return 0; //返回０说明满了
	else
		return 1;
}

//入栈函数
int pop(char a[] , char x)
{
	if(!man(bracket.top))
		return 0; //失败
	else
		a[++bracket.top] = x;
	return 1; //成功入栈
}

//出栈函数
int push(char a[] , char *x)
{
	if(!kong(bracket.top))
		return 0; //失败
	else
		*x = a[bracket.top--];
	return 1; //成功
}

//判断括号是否匹配的函数
int judge(char a , char b)
{
	if((a=='(' && b==')') || (a=='[' && b==']') || (a=='{' && b=='}'))
		return 1;//成功匹配
	else
		return 0; //匹配失败
}

int main()
{
	int i;
	int k;
	int ret;
	char x;
	char str[100000];
	bracket.top = -1; //开始时栈内为空指向-1;
	fgets(str,100000,stdin);
	k = strlen(str);
	for(i=0;i<k;i++)
	{
		if(str[i]=='(' || str[i]=='[' || str[i]=='{')
		{
			if(pop(bracket.ch , str[i]) == 0)
			{
				printf("false\n");
				return 0;
			}
		}
		else if(str[i]==')' || str[i]=='}' || str[i]==']')
		{
			if(push(bracket.ch , &x) == 0)
			{
				printf("false\n");
				return 0;
			}
			else
			{
				if(judge(x,str[i]) == 0)
				{
					printf("false\n");
					return 0;
				}
			}
		}
	}
	if(bracket.top == -1)
		printf("ture\n");
	else
		printf("false\n");
	return 0;
}
