#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
 
int main()
{
	struct termios old,new;
	
	char password[8] = {0};	
	char ch;
	int  i = 0;
	
	tcgetattr(0,&old);
	new = old;
	
	new.c_lflag &= ~(ECHO | ICANON);
	
	printf("请输入密码....\n");
	
	while(1)
	{
		tcsetattr(0,TCSANOW,&new);
		
		scanf("%c",&ch);
		
		tcsetattr(0,TCSANOW,&old);
		
		if(i == 8 || ch == '\n')
		{
			break;
		}
		
		password[i] = ch;
		printf("*");
		
		i++;
	}
	
	return 0;
}
 
