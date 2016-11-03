#include <stdio.h>
#include <locale.h>

int main(){
	
	setlocale(LC_ALL, "C");
	
	for(int i = 0; i < 256; i++){
		printf("%3d\t%d %d\t%c\n", i, i/16, i%16 , i);
	}
	
	getchar();getchar();
}
