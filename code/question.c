#include <stdio.h>

int main()
{
	char *s;
	char ch;
	ch = 'a';
	s = &ch;
	printf("%c\n",*s);
}
