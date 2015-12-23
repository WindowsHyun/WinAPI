#include <stdio.h>
#include <string.h>

#define isShowNumber 5

int main(){
	int a, b, c, d, e;
	int isWhatType=0, isC=0;
	int isTempData, temp=0;
	printf("숫자 입력 : ");
	scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);

	if( (b-a) == (c-b) ){
		//1 3 5 7 9 증가할경우
		isWhatType=1, isC = b-a;
	}else if( ((a+1)*10+a == b) && ((a+2)*100+(a+1)*10+a == c)){
		// 1 21 321 순으로 증가할경우
		isWhatType=2;
	}else if( (a*10+1 == b-a) && ( a*100+11 == c-b)){
		// 1 12 123 순으로 증가할경우
		isWhatType=3, isC = e-d;
	}else if( (b == (a*100)+((a+1)*10)+a) && (c == (a*10000)+((a+1)*1000)+((a+2)*100)+((a+1)*10)+a) ){
		// 1 121 12321 순으로 증가할경우
		isWhatType=4;
	}

	if(0 == isWhatType){
		printf("규칙을 확인할수 없습니다..!");
	}else if(1 == isWhatType){
		printf("숫자 예측 : %d %d %d %d %d ", a,b,c,d,e);
		isTempData = e;
		for(int i=0; i<isShowNumber; ++i){
			isTempData+=isC;
			printf("%d ", isTempData);
		}
	}else if(2 == isWhatType){
		isTempData = (e/10000)+1;
		printf("숫자 예측 : %d %d %d %d %d ", a,b,c,d,e);
		for(int i=0; i<isShowNumber; ++i){
			for(int i=isTempData; i>=1; --i){
				printf("%d",i);
			}
			isTempData+=1;
			printf(" ");
		}
	}else if(3 == isWhatType){
		printf("숫자 예측 : %d %d %d %d %d ", a,b,c,d,e);
		isC = isC*10+1, isTempData = e;
		for(int i=0; i<isShowNumber; ++i){
			isTempData = isTempData+isC;
			printf("%d ", isTempData);
			isC = isC*10+1;
		}
	}else if(4 == isWhatType){
		printf("숫자 예측 : %d %d %d %d %d ", a,b,c,d,e);
		isC = a+5;
		while(temp <=isShowNumber){
			for(int i=a; i<=isC; ++i){
				printf("%d",i);
			}
			for(int i=isC-1; i>=a; --i){
				printf("%d",i);
			}
			printf(" ");
			isC+=1;
			temp+=1;
		}
	}


	printf("\n");
}