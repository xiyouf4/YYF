#include <stdio.h>

int main()
{
    FILE *fp;
    int i = 2000000;
    fp = fopen("group_number.txt", "w");
    fwrite(&i, sizeof(int), 1, fp);
    fclose(fp);
}
