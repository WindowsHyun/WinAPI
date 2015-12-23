#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
	int i, j, randnum, temp, temp2;
	int num[45];
	int cho[45];

	for(i=0; i<45; ++i){
		num[i] = i+1;
		cho[i] = 0;
	}
	srand((unsigned)time(NULL));
	for(i=0; i<10000; ++i){
		//while(1){
		randnum = rand()%45;
		cho[randnum]++;
	}

	for(i=0; i<45; ++i){
		for(j=0; j<45; ++j){
			if(cho[i] <cho[j]){
				temp =cho[i];
				temp2 = num[i];
				cho[i] = cho[j];
				num[i] = num[j];
				cho[j] = temp;
				num[j] = temp2;
			}
		}
	}
	printf("Number : ");
		for(i=39; i<45; ++i){
			printf("%d ",num[i]);
		}
		printf("\n");

}