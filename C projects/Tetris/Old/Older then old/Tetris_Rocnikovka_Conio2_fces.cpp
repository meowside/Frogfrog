/*
	Name: Tetris 0.0.0.1
	Copyright: David Knieradl
	Author: David Knieradl
	Date: 21/12/15 16:21
	Description: rocnikova prace
*/



/*
        TO DO

	Rotation

*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define DELAY 10

void Game();
void Options();
void Info();
int Quit();
void Mid(const char TEXT[]);
void EpicText(const char TEXT[]);
void EpicTextMid(char TEXT[]);
void CurBlock(int sel);
void gotoxy(int x, int y);

int pos_x = 0, pos_y = 0, width = 0, height = 0, temp;

int brd_x = 5, brd_y = 1;

bool board[23][18] = {false};
bool board2[23][18] = {false};

int main(){
	srand(time(NULL));
	
	int position = 2;
	bool down = false;
	
	PlaySound(TEXT("test.wav"), NULL, SND_ASYNC);
	
	int end = 1;
	int select = 0;
	char key;
	
	while(true){
		
		bool render = true;
		
		if(position == 6){
			down = true;
		}
		else if(position == 2){
			down = false;
		}
		
		if(!down){
			position++;
		}
		else if(down){
			position--;
		}
				
		for(int i = 0; i < 32; i++){
			
			Sleep(DELAY);
			
			if(_kbhit()){
				key = _getch();
				if(key == 'w' && select > 0)select--;
				else if(key == 's' && select < 3)select++;
				else if(key == 13){
					if(select == 0) Game();
					else if(select == 1) Options();
					else if(select == 2) Info();
					else if(select == 3){
						end = Quit();
						if(end == 0) return 1;
					} 
				}
				
				render = true;
			}
			
			if(render){
				system("cls");
				for(int j = 0; j < position; j++)printf("\n");
				EpicText("TETRIS");
					
				for(int j = 0; j < 10-position; j++) printf("\n"); 
				//Mid("\b\b\b");
				Mid(select == 0 ? "> Play <" : "  Play  ");
				printf("\n");
				Mid(select == 1 ? "> Settings <" : "  Settings  ");	  
				printf("\n");
				Mid(select == 2 ? "> Info <" : "  Info  ");
				printf("\n\n");
				Mid(select == 3 ? "> Exit <" : "  Exit  ");
				render = false;
			}
			
			
		}
	}
	
	return 0;
}


void Game(){
	system("cls");
	
	bool collided = false;
	bool end = false;
	char key;

	int max_pos = 17;
	int min_pos = 0;
	
	bool render = false;
	
	PlaySound(0,0,0);
	
	while(!end){
		
		CurBlock(rand() % 7 + 1);
		
		while(!end){
			
			//system("pause");
			render = true;
			
			for(int i = 0; i < 16; i++){
				
				Sleep(DELAY);
				
				if(_kbhit()){
					
					key = _getch();
					
					render = true;
					
					
					//
					// MOVE TO LEFT
					//
					
					if(key == 'a' && pos_x > 0){
						for(int i = 0; i < width; i++){
							for(int j = 0; j < height; j++){
								board2[pos_y - j][pos_x + i - 1] = board2[pos_y - j][pos_x + i];
								board2[pos_y - j][pos_x + i] = false;
							}
						}
						pos_x--;
					}
					
					//
					// MOVE TO RIGHT
					//
					
					else if(key == 'd' && pos_x + width < 16){
						for(int i = 0; i < width; i++){
							for(int j = 0; j < height; j++){
								board2[pos_y - j][pos_x - i + width] = board2[pos_y - j][pos_x - i + width - 1]; 
								board2[pos_y - j][pos_x - i + width - 1] = false;
							}
						}
						pos_x++;
					}
					
					//   01234567
					// 0 ...x....
					// 1 ...xxx..
					// 2 ........
					//
					
					//   01234567
					// 0 ....x...
					// 1 ....x...
					// 2 ...xx...
					//
					
					//
					// ROTATION TO LEFT
					//
					
					else if(key == 'q'){						
						bool mezipole[width][height] = {false};
						
						for(int a = 0; a < height; a++){
							for(int b = 0; b < width; b++){
								mezipole[width - 1 - b][a] = board2[pos_y - height + 1 + a][pos_x + b];
								board2[pos_y - height + 1 + a][pos_x + b] = false;
							}
						}
						
						temp = height;
						height = width;
						width = temp;
						
						if (pos_x == 0) pos_x++;
						else if (pos_x == 16) pos_x--;
						
						
						for(int i = 0; i < height; i++){
							for(int j = 0; j < width; j++){
								board2[pos_y - height + 1 + i][pos_x + j] = mezipole[i][j];
							}
						}
					}
					
					//   01234567
					// 0 ...x....
					// 1 ...xxx..
					// 2 ........
					//
					
					//   01234567
					// 0 ....xx..
					// 1 ....x...
					// 2 ....x...
					//
					
					//
					// ROTATION TO RIGHT
					//
					
					else if(key == 'e'){
						bool mezipole[width][height] = {false};
						
						for(int a = 0; a < height; a++){
							for(int b = 0; b < width; b++){
								mezipole[b][height - 1 - a] = board2[pos_y - height + 1 + a][pos_x + b];
								board2[pos_y - height + 1 + a][pos_x + b] = false;
							}
						}
						
						temp = height;
						height = width;
						width = temp;
						
						if (pos_x == 0) pos_x++;
						else if (pos_x > 15) pos_x--;
						
						for(int i = 0; i < height; i++){
							for(int j = 0; j < width; j++){
								board2[pos_y - height + 1 + i][pos_x + j] = mezipole[i][j];
							}
						}
					}
					
					else if(key == 27){
						Info();
					}
				}
			
				if(render){
					system("cls");
					
					for(int row = 1; row < 23; row++){
						for(int column = 0; column < 16; column++){
							printf(board2[row][column] ? "#" : (board[row][column] ? "x" : "." ));
						}
						printf("\n");
					}
				
					render = false;
				}
			
			}
			
			bool brk = false;
			
			for(int row = 0; row < height; row++){
				for(int column = 0; column < width; column++){
					if(board2[pos_y - row][pos_x + column] && board[pos_y - row + 1][pos_x + column]){
						collided = true;
						brk = true;
						break;
					}
				}
				if(brk) break;
			}
			
			//
			//	Kolize na konci desky
			//
			
			if(!collided && pos_y >= 22){
					for(int i = 0; i < 16; i++){
						if(board2[22][i]){
							collided = true;
							break;
						}
					}
				}
			
			//
			//	Zapis do desky
			//
			
			if(collided){

				for(int row = 0; row < height; row++){
					for(int column = 0; column < width; column++){
						if(board2[pos_y - row][pos_x + column]){
							board[pos_y - row][pos_x + column] = board2[pos_y - row][pos_x + column];
						}
						
						board2[pos_y - row][pos_x + column] = false;
					}
				}
				collided = false;
				break;
			}
					
			//
			//	Posun Dolu
			//
			
			else if(!collided){
			
				pos_y++;
				
				for(int row = 0; row < height; row++){
					for(int column = 0; column < width; column++){
						board2[pos_y - row][pos_x + column] = board2[pos_y - row - 1][pos_x + column];
						board2[pos_y - row - 1][pos_x + column] = false; 
					}
				}
					
			}
	
		}
		
	}	
}

void CurBlock(int sel){
	
	
	pos_x = rand()%13 + 1;
	
	if(sel == 1){
		// xxxx
		
		width = 4;
		height = 1;
		pos_y = 1;
		
		board2[0][pos_x] = true;
		board2[0][pos_x + 1] = true;
		board2[0][pos_x + 2] = true;
		board2[0][pos_x + 3] = true;
	}
	
	else if(sel == 2){
		//  x
		// xxx
		
		width = 3;
		height = 2;
		pos_y = 1;
		
		board2[0][pos_x + 1] = true;
		board2[1][pos_x] = true;
		board2[1][pos_x + 1] = true;
		board2[1][pos_x + 2] = true;
	}
	
	else if(sel == 3){
		// xx
		// xx
		
		width = 2;
		height = 2;
		pos_y = 1;
		
		board2[0][pos_x] = true;
		board2[0][pos_x + 1] = true;
		board2[1][pos_x] = true;
		board2[1][pos_x + 1] = true;
	}
	
	else if(sel == 4){
		// x
		// xxx
		
		width = 3;
		height = 2;
		pos_y = 1;
		
		board2[0][pos_x] = true;
		board2[1][pos_x] = true;
		board2[1][pos_x + 1] = true;
		board2[1][pos_x + 2] = true;
	}
	
	else if(sel == 5){
		//   x
		// xxx
		
		width = 3;
		height = 2;
		pos_y = 1;
		
		board2[0][pos_x + 2] = true;
		board2[1][pos_x] = true;
		board2[1][pos_x + 1] = true;
		board2[1][pos_x + 2] = true;
	}
	
	else if(sel == 6){
		// xx
		//  xx
		
		width = 3;
		height = 2;
		pos_y = 1;
		
		board2[0][pos_x] = true;
		board2[0][pos_x + 1] = true;
		board2[1][pos_x + 1] = true;
		board2[1][pos_x + 2] = true;
	}
	
	else if(sel == 7){
		//  xx
		// xx
		
		width = 3;
		height = 2;
		pos_y = 1;
		
		board2[0][pos_x + 1] = true;
		board2[0][pos_x + 2] = true;
		board2[1][pos_x] = true;
		board2[1][pos_x + 1] = true;
	}
}

void Options(){
	
	system("cls");
	printf("options\n");
	system("pause");
	
	return;
}

void Info(){
	system("cls");
	printf("Info\n");
	system("pause");
	
	return;
}

int Quit(){
	int exit = 1;
	
	while(true){
		system("cls");
		for(int i = 0; i < 10; i++)printf("\n");
		Mid("Are you sure you want to quit this game?");
		printf("\n\n");
		Mid(exit == 0 ? "> YES <            No  " : "  YES            > No <");
		
		char key = _getch();
		
		if(key == 'a' && exit > 0){
			exit--;
		}
		
		if(key == 'd' && exit < 1){
			exit++;
		}
		
		if(key == 13){
			if(exit == 0) return exit;
			else if(exit == 1) return exit;
		}
	}
	
	
}

void Mid(const char TEXT[]){
	for(int i = 0; i < (80 - strlen(TEXT))/2; i++) printf(" ");
	printf("%s", TEXT);
}

void EpicText(const char TEXT[]){

	int chr = 0;
	char text[strlen(TEXT)];
	
	for(int i = 0; i < strlen(TEXT); i++) text[i] = TEXT[i];
	
	for(int row = 0; row < 6; row++){
		EpicTextMid(text);
		for(chr = 0; chr < strlen(TEXT); chr++){
			
			
			if(text[chr] == 'a'){
				
				if(row == 0 || row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 3 || row == 4)printf("\xB2      \xB2");
			}
			
			else if(text[chr] == 'b'){
				
				if(row == 0 || row == 2 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ");
				else if(row == 1 || row == 3)printf("\xB2      \xB2");
			}
			
			else if(text[chr] == 'c'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 2 || row == 3)printf("\xB2       ");
			}
			
			else if(text[chr] == 'd'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ");
				else if(row == 1 || row == 2 || row == 3)printf("\xB2      \xB2");
			}
			
			else if(text[chr] == 'e'){
				
				if(row == 0 || row == 2 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 3)printf("\xB2       ");
			}
			
			else if(text[chr] == 'f'){
				
				if(row == 0 || row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 3 || row == 4)printf("\xB2       ");
			}
			
			else if(text[chr] == 'g'){
				
				if(row == 0 ||row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1)printf("\xB2       ");
				else if(row == 2)printf("\xB2    \xB2\xB2\xB2");
				else if(row == 3)printf("\xB2      \xB2");
			}
			
			else if(text[chr] == 'h'){
				
				if(row == 0 || row == 1 || row == 3 || row == 4)printf("\xB2      \xB2");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
			}
			
			else if(text[chr] == 'i'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 2 || row == 3)printf("  \xB2  ");
			}
			
			else if(text[chr] == 'j'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3)printf("     \xB2");
				else if(row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
			}
			
			else if(text[chr] == 'k'){
				
				if(row == 0)printf("\xB2     \xB2");
				else if(row == 1)printf("\xB2    \xB2 ");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 3 || row == 4)printf("\xB2     \xB2");
			}
			
			
			else if(text[chr] == 'l'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3)printf("\xB2       ");
				else if(row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
			}
			
			else if(text[chr] == 'm'){
				
				if(row == 0)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 2 || row == 3 || row == 4)printf("\xB2   \xB2   \xB2");
			}
			
			else if(text[chr] == 'n'){
				
				if(row == 0)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ");
				else if(row == 1 || row == 2 || row == 3 || row == 4)printf("\xB2      \xB2");
			}
			
			else if(text[chr] == 'o'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 2 || row == 3)printf("\xB2      \xB2");
			}
			
			else if(text[chr] == 'p'){
				
				if(row == 0)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ");
				else if(row == 1 || row == 3 || row == 4)printf("\xB2      \xB2");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
			}
			
			else if(text[chr] == 'q'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 2)printf("\xB2      \xB2");
				else if(row == 3)printf("\xB2    \xB2 \xB2");
			}
			
			else if(text[chr] == 'r'){
				
				if(row == 0)printf("\xB2\xB2\xB2\xB2\xB2\xB2 ");
				else if(row == 1 || row == 3 || row == 4)printf("\xB2     \xB2");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2 ");
			}
			
			else if(text[chr] == 's'){
				
				if(row == 0 || row == 2 || row == 4)printf(" \xB2\xB2\xB2\xB2\xB2\xB2\xB2 ");
				else if(row == 1)printf("\xB2        ");
				else if(row == 3)printf("        \xB2");
			}
			
			else if(text[chr] == 't'){
				
				if(row == 0)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 2 || row == 3 || row == 4)printf("   \xB2   ");
			}
			
			else if(text[chr] == 'u'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3)printf("\xB2      \xB2");
				else if(row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ");
			}
			
			else if(text[chr] == 'v'){
				
				if(row == 0)printf("\xB2      \xB2");
				else if(row == 1)printf("\xB2     \xB2 ");
				else if(row == 2)printf("\xB2    \xB2  ");
				else if(row == 3)printf("\xB2   \xB2   ");
				else if(row == 4)printf("\xB2\xB2\xB2\xB2    ");
			}
			
			else if(text[chr] == 'w'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3)printf("\xB2   \xB2   \xB2");
				else if(row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ");
			}
			
			else if(text[chr] == 'x'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2     \xB2\xB2");
				else if(row == 1 || row == 3)printf("  \xB2\xB2 \xB2\xB2  ");
				else if(row == 2)printf("    \xB2    ");
			}
			
			else if(text[chr] == 'y'){
				
				if(row == 0 || row == 1)printf("\xB2     \xB2");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 3 || row == 4)printf("   \xB2   ");
			}
			
			else if(text[chr] == 'z'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1)printf("     \xB2\xB2 ");
				else if(row == 2)printf("   \xB2\xB2   ");
				else if(row == 3)printf(" \xB2\xB2     ");
			}
			
			else if(text[chr] == '.'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3)printf(" ");
				else if(row == 4)printf("\xB2");
			}
			
			else if(text[chr] == ','){
				
				if(row == 0 || row == 1 || row == 2)printf(" ");
				else if(row == 3 || row == 4)printf("\xB2");
			}
			
			else if(text[chr] == '!'){
				
				if(row == 0 || row == 1 || row == 2 || row == 4)printf("\xB2");
				else if(row == 3)printf(" ");
			}
			
			else if(text[chr] == '?'){
				
				if(row == 0)printf(" \xB2\xB2\xB2 ");
				else if(row == 1)printf("\xB2   \xB2");
				else if(row == 2)printf("   \xB2 ");
				else if(row == 3)printf("     ");
				else if(row == 4)printf("  \xB2  ");
			}
			
			else if(text[chr] == '#'){
				
				if(row == 0 || row == 2 || row == 4)printf(" \xB2  \xB2 ");
				else if(row == 1 || row == 3)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
			}
			
			else if(text[chr] == '%'){
				
				if(row == 0)printf("\xB2\xB2\xB2  \xB2 ");
				else if(row == 1)printf("\xB2 \xB2 \xB2  ");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 3)printf("  \xB2 \xB2 \xB2");
				else if(row == 4)printf(" \xB2  \xB2\xB2\xB2");
			}
			
			else if(text[chr] == '('){
				
				if(row == 0 || row == 4)printf(" \xB2");
				else if(row == 1 || row == 2 || row == 3)printf("\xB2 ");
			}
			
			else if(text[chr] == ')'){
				
				if(row == 0 || row == 4)printf("\xB2 ");
				else if(row == 1 || row == 2 || row == 3)printf(" \xB2");
			}
			
			else if(text[chr] == '*'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2 \xB2 \xB2\xB2");
				else if(row == 1 || row == 3)printf("  \xB2\xB2\xB2  ");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
			}
			
			else if(text[chr] == '-'){
				
				if(row == 0 || row == 1 || row == 3 || row == 4)printf("    ");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2");
			}
			
			else if(text[chr] == '/'){
				
				if(row == 0)printf("    \xB2");
				else if(row == 1)printf("   \xB2 ");
				else if(row == 2)printf("  \xB2  ");
				else if(row == 3)printf(" \xB2   ");
				else if(row == 4)printf("\xB2    ");
			}
			
			else if(text[chr] == ' '){
				
				printf("   ");
			}
			else if(text[chr] == '0'){
				
				if(row == 0 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1)printf("\xB2   \xB2\xB2");
				else if(row == 2)printf("\xB2 \xB2\xB2 \xB2");
				else if(row == 3)printf("\xB2\xB2   \xB2");
			}
			else if(text[chr] == '1'){
				
				if(row == 0)printf("\xB2\xB2\xB2  ");
				else if(row == 1 || row == 2 || row == 3)printf("  \xB2  ");
				else if(row == 4)printf("\xB2\xB2\xB2\xB2\xB2");
			}
			else if(text[chr] == '2'){
				
				if(row == 0 || row == 2 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1)printf("     \xB2");
				else if(row == 3)printf("\xB2     ");
			}
			else if(text[chr] == '3'){
				
				if(row == 0 || row == 2 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 3)printf("     \xB2");
			}
			else if(text[chr] == '4'){
				
				if(row == 0 || row == 1)printf("\xB2    \xB2");
				else if(row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 3 || row == 4)printf("     \xB2");
			}
			else if(text[chr] == '5'){
				
				if(row == 0 || row == 2 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1)printf("\xB2     ");
				else if(row == 3)printf("     \xB2");
			}
			else if(text[chr] == '6'){
				
				if(row == 0 || row == 1)printf("\xB2     ");
				else if(row == 2 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 3)printf("\xB2    \xB2");
			}
			else if(text[chr] == '7'){
				
				if(row == 0)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 2 || row == 3 || row == 4)printf("     \xB2");
			}
			else if(text[chr] == '8'){
				
				if(row == 0 || row == 2 || row == 4)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1 || row == 3)printf("\xB2    \xB2");
			}
			else if(text[chr] == '9'){
				
				if(row == 0 || row == 2)printf("\xB2\xB2\xB2\xB2\xB2\xB2");
				else if(row == 1)printf("\xB2    \xB2");
				else if(row == 3 || row == 4)printf("     \xB2");
			}
			
			else printf("CHYBA");
			
			if(chr < strlen(TEXT)-1)printf(" ");
		}
		
		printf("\n");
	}
}

void EpicTextMid(char TEXT[]){
	
/*
 ********  *******   ********  *******   ********  xxxxxxxx  xxxxxxxx  x      x  xxxxx       x  x     x   x         xxxxxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxx   xxxxxxxx xxxxxxx    xxxxxxx  xxxxxxxxx  x      x  x      x  x   x   x  xx     xx  x     x  xxxxxxxx   xxxxxx  xxx   xxxxxx  xxxxxx  x    x  xxxxxx  x       xxxxxx  xxxxxx  xxxxxx        x   xxx    x  x   xxx  x   x   x  x    xx x xx            x                            
 *      *  *      *  *         *      *  *         x         x         x      x    x         x  x    x    x         x   x   x  x      x  x      x  x      x  x      x x      x  x             x      x      x  x     x   x   x   x    xx xx    x     x       xx    x   xx    x        x       x  x    x  x       x            x  x    x  x    x        x  x   x  xxxxxx  x x x    x  x    x     xxx             x
 ********  ********  *         *      *  ********  xxxxxxxx  x    xxx  xxxxxxxx    x         x  xxxxxxxx  x         x   x   x  x      x  x      x  xxxxxxxx  x      x xxxxxxxx  xxxxxxxx      x      x      x  x    x    x   x   x      x      xxxxxxx     xx      x xx x    x   xxxxxx  xxxxxx  xxxxxx  xxxxxx  xxxxxx       x  xxxxxx  xxxxxx        x     x    x  x   xxxxxxx     x    x   xxxxxxx  xxxx    x                            
 *      *  *      *  *         *      *  *         x         x      x  x      x    x         x  x      x  x         x   x   x  x      x  x      x  x         x   x  x x      x         x      x      x      x  x   x     x   x   x    xx xx       x      xx        xx   x    x   x            x       x       x  x    x       x  x    x       x     x            xxxxxx    x x x     x    x     xxx           x
 *      *  ********  ********  ********  ********  x         xxxxxxxx  x      x  xxxxx  xxxxxx  x      x  xxxxxxxx  x   x   x  x      x  xxxxxxxx  x         xxxxxxxx x      x  xxxxxxxx      x      xxxxxxxx  xxxx      xxxxxxxxx  xx     xx     x     xxxxxxxx   xxxxxx  xxxxx xxxxxx  xxxxxx       x  xxxxxx  xxxxxx       x  xxxxxx       x  x  x  x    x     x  x    x  xxx      x  x    xx x xx        x
                                                                                                                                                                 
*/
	
	int string_length = 0;
	//              a b c d e f g h i j k l m n o p q r s t u v w x y z . , ! ? # % ( ) * - / _ 0 1 2 3 4 5 6 7 8 9 
	int length[] = {8,8,8,8,8,8,8,8,5,6,6,9,8,8,8,8,8,7,9,8,8,9,9,7,8,8,1,1,1,5,6,7,2,2,7,4,5,3,6,5,6,6,6,6,6,6,6,6};
	char chrs[] = {"abcdefghijklmnopqrstuvwxyz.,!?#%()*-/ 0123456789"};
	
	for(int i = 0; i < strlen(TEXT); i++){
		for(int j = 0; j < sizeof(length)/sizeof(int); j++){
			if(TEXT[i] >= 65 && TEXT[i] <=90) TEXT[i] = TEXT[i] + 32;
			
			if(TEXT[i] == chrs[j]) string_length += length[j];
		}
		
		if(i < strlen(TEXT)-1) string_length += 1;	
	}
		
	for(int i = 0; i < (80 - string_length)/2; i++) printf(" ");
}

void gotoxy(int x, int y){
  COORD c ;
  c.X = x - 1;
  c.Y = y - 1;
  SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

