#include <stdio.h>

int main(){
	int i, j, num=0;
	for(i=1; i<100; ++i){
		if(i%2!=0 && i%3!=0){

			for(j=2; i>j; ++j){
				if(i%j!=0){
					num++;
				}
			}
			if(num == i-2){
				printf("%d ",i);
			}
			num = 0;

		}
	}

}

