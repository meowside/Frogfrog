#include <stdio.h>
#include <windows.h>

int main(){
	bool test[3][3] = {false};
	int height = 3, width = 2;
	
	test[0][0] = true;
	test[0][1] = true;
	test[1][1] = true;
	test[2][1] = true;
	
	while(true){
		bool wat[width][height] = {false};
		int temp;
		
		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				wat[j][height - 1 - i] = test[i][j];
				printf(test[i][j] ? "x" : ".");
			}
			printf("\n");
		}
		
		temp = width;
		width = height;
		height = temp;
				
		printf("\n\n");
				
		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				test[i][j] = wat[i][j];
				printf(test[i][j] ? "x" : ".");
			}
			printf("\n");
		}
		
		Sleep(2000);
	}
}
