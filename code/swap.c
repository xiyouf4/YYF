//交换函数任意类型的参数都可以交换

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *a, void *b, int size)
{
	void *temp = malloc(size);
	memcpy(temp, a, size);
	memcpy(a, b, size);
	memcpy(b, temp, size);
}

int main()
{
	int a, b;
	a = 5;
	b = 4;
	printf("%d %d\n", a, b);

	swap(&a, &b, sizeof(int));
	printf("%d %d\n", a, b);
}
