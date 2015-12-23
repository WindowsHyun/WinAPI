#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
	int onebae[4][3];
	int twobae[3][4];
	int plusbae[4][3];
	int i, j;

	srand((unsigned)time(NULL));

	for(i=0; i<4; ++i){
		for(j=0; j<3; ++j){
			onebae[i][j] = rand()%50+1;
		}
	}
	printf("1¹ø ¹è¿­ : \n");
	for(i=0; i<4; ++i){
		for(j=0; j<3; ++j){
			printf("%4d", onebae[i][j]);
		}
		printf("\n");
	}

	for(i=0; i<3; ++i){
		for(j=0; j<4; ++j){
			twobae[i][j] = rand()%50+1;
		}
	}
	printf("2¹ø ¹è¿­ : \n");
	for(i=0; i<3; ++i){
		for(j=0; j<4; ++j){
			printf("%4d", twobae[i][j]);
		}
		printf("\n");
	}
	printf("°á°ú-µ¡¼À : \n");
	for(i=0; i<4; ++i){
		for(j=0; j<3; ++j){
			plusbae[i][j] = onebae[i][j] + twobae[j][i];
		}
	}
	for(i=0; i<4; ++i){
		for(j=0; j<3; ++j){
			printf("%4d", plusbae[i][j]);
		}
		printf("\n");
	}
	printf("°á°ú-»¬¼À : \n");
	for(i=0; i<4; ++i){
		for(j=0; j<3; ++j){
			plusbae[i][j] = onebae[i][j] - twobae[j][i];
		}
	}
	for(i=0; i<4; ++i){
		for(j=0; j<3; ++j){
			printf("%4d", plusbae[i][j]);
		}
		printf("\n");
	}
}