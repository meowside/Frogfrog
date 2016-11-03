#include <conio.h>
#include <stdio.h>

int main(){
	int i = 0;
	while(true){
		i = getch();
		
		printf("%d\n",i);
		
		if(i == 224 || i == 0){
			i += getch();
			printf("%d\n",i);
		}
		
		//printf("%d\n", i++);
	}
}
