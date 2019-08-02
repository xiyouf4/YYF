#include <stdio.h>
#include <stdlib.h>


int main()
{
	FILE  *fp;
	int i = 1000000;
	int a;
	if((fp = fopen("user_number.txt", "w")) == NULL)
		printf("sssssssssssssssssssssss\n");
	fwrite(&i, sizeof(int), 1, fp);
	fclose(fp);
	if((fp = fopen("user_number.txt", "r")) == NULL)
		printf("aaaaaaaaaaaaaaaaaaa\n");
	fread(&a, sizeof(int), 1, fp);
	printf("%d\n",a);
	if(a == 1000000)
		printf("ajfhgefgqwyfg\n");
	fclose(fp);
}
