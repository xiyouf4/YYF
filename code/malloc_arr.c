//动态申请一个int [2][3]的数组

#include <stdio.h>
#include <stdlib.h>

int main()
{
	int (*a)[3] = (int (*)[3])malloc(sizeof(int)*3*2);
	a[0][0] = 1;
	printf("%d\n", a[0][0]);
	free(a);
}
