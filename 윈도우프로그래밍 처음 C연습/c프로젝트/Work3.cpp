#include <stdio.h>
#include <string.h>

int main(){
	int i, j, numlen;
	char inputxt[81];

	printf("Enter sentence : ");
	gets(inputxt);

	printf("You entered : %s",inputxt);
	numlen = strlen(inputxt);
	printf("\nLen : %d\n", numlen);
	printf("Reverse : ");
	for(i=numlen; i>=0; --i)
		printf("%c",inputxt[i]);



}