#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
	int i, j, num, randtext, temp;
	int arr[20];

	printf("Input number : ");
	scanf("%d",&num);
	srand((unsigned)time(NULL));
	for(i=0; i<20;){
		randtext = rand()%num;
		for(j=0; j<i; ++j){
			if(arr[j] == randtext){
				break;
			}
		}
		if(i==j){
			arr[i] =randtext;
			++i;
		}
	}
printf("Numbers are : ");
	for(i=0; i<20; ++i){
		printf("%d ", arr[i]);
	}
	for(i=0; i<20;++i){
		for(j=0;j<20;++j){
			if(arr[i] < arr[j]){
				temp = arr[i];
				arr[i] = arr[j];
				arr[j]= temp;
			}
		}
	}
	printf("\nSorted : ");
	for(i=0; i<20; ++i){
		printf("%d ", arr[i]);
	}

	printf("\nMaximum number : %d", arr[19]);
	printf("\nMinimum number : %d", arr[0]);


}