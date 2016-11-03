/*
	Name: Tetris 0.5
	Copyright: David Knieradl
	Author: David Knieradl
	Description: rocnikova prace
*/

/*
    -------- TO DO ---------

	*	   EVERYTHING	   *
	
	DROP BLOCK ON BOTTOM
	COLORED BLOCKS
	AWAY SCREEN
	ANIMATED LOADING
	THREADS

*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define DELAY 10

#define UP_K 296				//up arrow key code
#define DOWN_K 304				//down arrow key code
#define LEFT_K 299				//left arrow key code
#define RIGHT_K 301				//right arrow key code
#define SPACE_K 32				//space key code

#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2

#define DEBUG 1

enum COLORS {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
    DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};


void Game();
void OnLoad();
void EpicText(const char TEXT[], int y, int color);
void Options();
void Info();
bool Quit();
void CurBlock(int sel);
void Render(int GAME);
bool Pause();
void Defaults();

char Keys(int key);

void Mid(const char TEXT[], int x, int y, int color);
void _setcursortype(int type);
void crmove(int x, int y);
void textattr(int _attr);
void textcolor(int _color);
void clrscr();
void SetWindow(int Width, int Height);
void SetFont(int size_x, int size_y);
void WindowSize();
DWORD WINAPI WindowResizedEvent(void* data);

void MoveLeft();
void MoveRight();
void RotateLeft();
void RotateRight();


//
//	VARIABLES
//



typedef struct{
	int UP;
	int DOWN;
	int LEFT;
	int RIGHT;
	int ROTATE_L;
	int ROTATE_R;
	int DROP;
} KEYS;

typedef struct{
	COORD POS;
	int WIDTH;
	int HEIGHT;
	int CUR;
	int NEXT;
} BLOCKPLUS;

typedef struct{
	COORD BOARD;
	COORD CURBLOCK;
	COORD PAUSE;
} GAMESCR;

typedef struct{
	GAMESCR GAME;
	COORD BLOCK;
	COORD MAINMENU;
	COORD SETTINGS;
	COORD INFO;
	COORD EXIT;
	
	COORD MMSIZE;
	COORD GAMESIZE;
	COORD SETTSIZE;
	COORD INFOSIZE;
	COORD EXITSIZE;
} RENDER;


RENDER screen;
BLOCKPLUS block;
KEYS keys;

int key;

bool sound = true, changed = false;

int temp = 0, last_width = 0, last_height = 0, row = 0, column = 0;

bool board[23][18] = {false};
bool board2[23][18] = {false};

// strings

char name[] = "Tetris 0.5 - by David Knieradl";
char song[] = "Music/Tetris_01.wav";

int main(){
	
	OnLoad();
	
	last_height = screen.MMSIZE.Y;
	last_width = screen.MMSIZE.X;
	
	int position = 3;
	bool anim_d = false;
	
	int select = 0;
	
	while(true){
		
		if(position == 8){
			anim_d = true;
		}
		else if(position == 3){
			anim_d = false;
		}
		
		if(!anim_d){
			position++;
		}
		else if(anim_d){
			position--;
		}
		
		EpicText("tetris", position, YELLOW);
		
		Mid( (select == 0 ? "> Play <" : "  Play  ") , (80 - strlen("  Play  ")) / 2, 17, (select == 0 ? WHITE : DARKGRAY));
		Mid( (select == 1 ? "> Settings <" : "  Settings  ") , (80 - strlen("  Settings  ")) / 2, 18, (select == 1 ? WHITE : DARKGRAY));
		Mid( (select == 2 ? "> Info <" : "  Info  ") , (80 - strlen("  Info  ")) / 2, 19, (select == 2 ? WHITE : DARKGRAY));
		Mid( (select == 3 ? "> Exit <" : "  Exit  ") , (80 - strlen("  Exit  ")) / 2, 21, (select == 3 ? WHITE : DARKGRAY));
		
		for(int row = 0; row < 48; row++){
			
			#if DEBUG
				crmove(0,0);
				textcolor(DARKGRAY);
				printf("sel %d | pressed %3d | keys.UP %3d | keys.DOWN %3d | pos_t %d |", select, key, keys.UP, keys.DOWN, position);
				crmove(0,1);
				printf("width %3d | height %2d |", last_width, last_height);
				Mid("DEBUG MODE",0,25,WHITE);
			#endif
			
			if(_kbhit()){
				
				
				key = _getch();
					
				if(key == 0 || key == 224){
					key += _getch();
				}
				
				if((key == keys.UP || key == 'w') && select > 0){
					select--;	
				}
				
				else if((key == keys.DOWN || key == 's') && select < 3){
					select++;
					
				}
				
				else if(key == 13){
					if(select == 0) Game();
					else if(select == 1) Options();
					else if(select == 2) Info();
					else if(select == 3){
						int end = Quit();
						if(end) return 1;
					}
					
					last_height = screen.MMSIZE.Y;
					last_width = screen.MMSIZE.X;
					
					break;
				}
				
				
				
				Mid( (select == 0 ? "> Play <" : "  Play  ") , (80 - strlen("  Play  ")) / 2, screen.MAINMENU.Y, (select == 0 ? WHITE : DARKGRAY));
				Mid( (select == 1 ? "> Settings <" : "  Settings  ") , (80 - strlen("  Settings  ")) / 2, screen.MAINMENU.Y + 1, (select == 1 ? WHITE : DARKGRAY));
				Mid( (select == 2 ? "> Info <" : "  Info  ") , (80 - strlen("  Info  ")) / 2, screen.MAINMENU.Y + 2, (select == 2 ? WHITE : DARKGRAY));
				Mid( (select == 3 ? "> Exit <" : "  Exit  ") , (80 - strlen("  Exit  ")) / 2, screen.MAINMENU.Y + 4, (select == 3 ? WHITE : DARKGRAY));
				
			}
			
			Sleep(DELAY);
		}
	}
}

void Game(){
	
	last_width = screen.GAMESIZE.X;
	last_height = screen.GAMESIZE.Y;
	
	clrscr();
	
	bool collided = false;
	bool end = false;
	char key;

	int max_pos = 1;
	int min_pos = 0;
	
	bool down_brk = false;
	bool render = false;
	
	block.NEXT = rand() % 7 + 1;
	
	PlaySound(0,0,0);
	
	Render(WHITE);
	
	while(!end){
		
		
		CurBlock(block.CUR = block.NEXT);
		block.NEXT = rand() % 7 + 1;
		
		changed = true;
		
		Render(WHITE);
		
		while(!end){
			
			for(row = 0; row < 16; row++){
				
				Sleep(DELAY);
				
				#if DEBUG
					crmove(0,0);
					textcolor(DARKGRAY);
					printf("row %2d | key %d | block");
				#endif
				
				if(_kbhit()){
					
					key = _getch();
					
					//
					// MOVE TO LEFT
					//
					
					if((key == keys.LEFT || key == 'a') && block.POS.X > 1){ MoveLeft(); changed = true; }
					
					//
					// MOVE TO RIGHT
					//
					
					else if((key == keys.RIGHT || key == 'd') && block.POS.X + block.WIDTH - 1 < 16){ MoveRight(); changed = true; }
					
					//
					// ROTATE LEFT
					//
					
					else if(key == keys.ROTATE_L || key == 'q'){						
						
						changed = true;
						
						RotateLeft();
						
						while(block.POS.X < 1){
							MoveRight();
						}
						while(block.POS.X + block.WIDTH > 17){
							MoveLeft();
						}
					}
					
					//
					// ROTATE RIGHT
					//
					
					else if(key == keys.ROTATE_R || key == 'e'){
						
						changed = true;
						
						RotateRight();
						
						while(block.POS.X < 1){
							MoveRight();	
						}
						
						while(block.POS.X + block.WIDTH > 17){
							MoveLeft();	
						}
					}
					
					//
					// DROP
					//
					
					else if(key == keys.DOWN || key == 's'){
						down_brk = true;
						changed = true;
					}
					
					else if(key == 27){
						changed = true;
						Pause();
					}
					
					//else system("pause");
					
					Render(WHITE);
				}
				
				if(down_brk) break;
			
			}
			
			down_brk = false;
			
			bool brk = false;
			
			for(row = 0; row < block.HEIGHT; row++){
				for(column = 0; column < block.WIDTH; column++){
					if(board2[block.POS.Y - row][block.POS.X + column] && board[block.POS.Y - row + 1][block.POS.X + column]){
						collided = true;
						brk = true;
						break;
					}
				}
				if(brk) break;
			}
			
			
			//
			//	Zapis do desky
			//
			
			if(collided || block.POS.Y >= 22){

				for(row = 0; row < block.HEIGHT; row++){
					for(column = 0; column < block.WIDTH; column++){
						if(board2[block.POS.Y - row][block.POS.X + column]){
							board[block.POS.Y - row][block.POS.X + column] = board2[block.POS.Y - row][block.POS.X + column];
						}
						
						board2[block.POS.Y - row][block.POS.X + column] = false;
					}
				}
				
				collided = false;
				break;
			}
					
			//
			//	Posun Dolu
			//
			
			else if(!collided){
			
				block.POS.Y++;
				
				for(row = 0; row < block.HEIGHT + 1; row++){
					for(column = 0; column < block.WIDTH; column++){
						board2[block.POS.Y - row][block.POS.X + column] = board2[block.POS.Y - row - 1][block.POS.X + column];
						board2[block.POS.Y - row - 1][block.POS.X + column] = false;
					}
				}
					
			}
			
			Render(WHITE);
			
			for(row = 0; row < 23; row++){
				if(board[row][1] && board[row][2] && board[row][3] && board[row][4] && board[row][5] && board[row][6] && board[row][7] && board[row][8] && board[row][9] && board[row][10] && board[row][11] && board[row][12] && board[row][13] && board[row][14] && board[row][15] && board[row][16]){
					for(column = 0; column < 18; column++) board[row][column] = false;
					
					for(int i = row; i > 0; i--){
						for(int j = 0; j < 18; j++){
							board[i][j] = board[i - 1][j];
							board[i - 1][j] = false;
						}
					}
				}
			}
	
		}
		
	}	
}

void CurBlock(int sel){
	
	block.POS.X = 7;
	
	if(sel == 1){
		// xxxx
		
		block.WIDTH = 4;
		block.HEIGHT = 1;
		block.POS.Y = 1;
		
		board2[1][block.POS.X] = true;
		board2[1][block.POS.X + 1] = true;
		board2[1][block.POS.X + 2] = true;
		board2[1][block.POS.X + 3] = true;
		
	}
	
	else if(sel == 2){
		//  x
		// xxx
		
		block.WIDTH = 3;
		block.HEIGHT = 2;
		block.POS.Y = 1;
		
		board2[0][block.POS.X + 1] = true;
		board2[1][block.POS.X] = true;
		board2[1][block.POS.X + 1] = true;
		board2[1][block.POS.X + 2] = true;
	}
	
	else if(sel == 3){
		// xx
		// xx
		
		block.WIDTH = 2;
		block.HEIGHT = 2;
		block.POS.Y = 1;
		
		board2[0][block.POS.X] = true;
		board2[0][block.POS.X + 1] = true;
		board2[1][block.POS.X] = true;
		board2[1][block.POS.X + 1] = true;
	}
	
	else if(sel == 4){
		// x
		// xxx
		
		block.WIDTH = 3;
		block.HEIGHT = 2;
		block.POS.Y = 1;
		
		board2[0][block.POS.X] = true;
		board2[1][block.POS.X] = true;
		board2[1][block.POS.X + 1] = true;
		board2[1][block.POS.X + 2] = true;
	}
	
	else if(sel == 5){
		//   x
		// xxx
		
		block.WIDTH = 3;
		block.HEIGHT = 2;
		block.POS.Y = 1;
		
		board2[0][block.POS.X + 2] = true;
		board2[1][block.POS.X] = true;
		board2[1][block.POS.X + 1] = true;
		board2[1][block.POS.X + 2] = true;
	}
	
	else if(sel == 6){
		// xx
		//  xx
		
		block.WIDTH = 3;
		block.HEIGHT = 2;
		block.POS.Y = 1;
		
		board2[0][block.POS.X] = true;
		board2[0][block.POS.X + 1] = true;
		board2[1][block.POS.X + 1] = true;
		board2[1][block.POS.X + 2] = true;
	}
	
	else if(sel == 7){
		//  xx
		// xx
		
		block.WIDTH = 3;
		block.HEIGHT = 2;
		block.POS.Y = 1;
		
		board2[0][block.POS.X + 1] = true;
		board2[0][block.POS.X + 2] = true;
		board2[1][block.POS.X] = true;
		board2[1][block.POS.X + 1] = true;
	}
}

void Render(int GAME){
	
	if(changed){
		
		for(row = 0; row < 25; row++){
			if(row < 23) Mid("<|                |>", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row, GAME);
			else if(row == 23) Mid("<==================>", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row, GAME);
			else Mid(" vvvvvvvvvvvvvvvvvv ", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row, GAME);
		}
		
		Mid("Next Block", screen.GAME.CURBLOCK.X, screen.GAME.CURBLOCK.Y, GAME);
		
		switch(block.NEXT){
			
			case 1:
				Mid("####", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid("    ", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 3, DARKGRAY);
				break;
				
			case 2:			
				Mid("  # ", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid(" ###", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 3, DARKGRAY);
				break;
				
			case 3:
				Mid(" ## ", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid(" ## ", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 3, DARKGRAY);
				break;
			
			case 4:
				Mid(" #  ", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid(" ###", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 3, DARKGRAY);
				break;
				
			case 5:
				Mid("   #", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid(" ###", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 3, DARKGRAY);
				break;
				
			case 6:
				Mid(" ## ", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid("  ##", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 3, DARKGRAY);
				break;
				
			case 7:
				Mid("  ##", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid(" ## ", screen.GAME.CURBLOCK.X + 3, screen.GAME.CURBLOCK.Y + 3, DARKGRAY);
				break;
				
		}
		
		changed = false;
	}
	
	for(row = 0; row < 23; row++){
		for(column = 0; column < 16; column++){
			Mid(board2[row][column + 1] ? "#" : (board[row][column + 1] ? "x" : "."), screen.GAME.BOARD.X + column, screen.GAME.BOARD.Y + row, GAME);
		}
	}
	
}


	//
	// MOVE TO LEFT
	//

void MoveLeft(){
	for(row = 0; row < block.WIDTH; row++){
		for(column = 0; column < block.HEIGHT; column++){
			board2[block.POS.Y - column][block.POS.X + row - 1] = board2[block.POS.Y - column][block.POS.X + row];
			board2[block.POS.Y - column][block.POS.X + row] = false;
		}
	}
	
	block.POS.X--;
}

	//
	// MOVE TO RIGHT
	//

void MoveRight(){
	for(row = 0; row < block.WIDTH; row++){
		for(column = 0; column < block.HEIGHT; column++){
			board2[block.POS.Y - column][block.POS.X - row + block.WIDTH] = board2[block.POS.Y - column][block.POS.X - row + block.WIDTH - 1]; 
			board2[block.POS.Y - column][block.POS.X - row + block.WIDTH - 1] = false;
		}
	}
	
	block.POS.X++;
}

	//   0123456
	// 0 ..x....
	// 1 ..xxx..
	// 2 .......
	//
					
	//   0123456
	// 0 ...x...
	// 1 ...x...
	// 2 ..xx...
	//
				
	//
	// ROTATION TO LEFT
	//

void RotateLeft(){
	bool mezipole[block.WIDTH][block.HEIGHT] = {false};
						
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			mezipole[block.WIDTH - 1 - column][row] = board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column];
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = false;
		}
	}
						
	temp = block.HEIGHT;
	block.HEIGHT = block.WIDTH;
	block.WIDTH = temp;
	
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = mezipole[row][column];
		}
	}
}

	//   0123456
	// 0 ..x....
	// 1 ..xxx..
	// 2 .......
	//
	
	//   0123456
	// 0 ...xx..
	// 1 ...x...
	// 2 ...x...
	//
					
	//
	// ROTATION TO RIGHT
	//

void RotateRight(){
	bool mezipole[block.WIDTH][block.HEIGHT] = {false};
						
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			mezipole[column][block.HEIGHT - 1 - row] = board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column];
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = false;
		}
	}
						
	temp = block.HEIGHT;
	block.HEIGHT = block.WIDTH;
	block.WIDTH = temp;
	
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = mezipole[row][column];
		}
	}
}

void OnLoad(){
	srand(time(NULL));
	
	HANDLE thread = CreateThread(NULL, 0, WindowResizedEvent, NULL, 0, NULL);
	
	last_width = 80;
	last_height = 26;
	
	_setcursortype(_NOCURSOR);
	SetConsoleTitle(name);
	SetWindow(80,26);
	SetFont(12, 16);
	
	if(sound) PlaySound(song, NULL, SND_LOOP | SND_ASYNC);
	
	screen.MAINMENU.Y = 17;
	screen.SETTINGS.Y = 13;
	screen.INFO.Y = 13;
	screen.GAME.BOARD.Y = 2;
	screen.GAME.BOARD.X = 10;
	screen.GAME.CURBLOCK.Y = 5;
	screen.GAME.CURBLOCK.X = 52;
	screen.GAME.PAUSE.Y = 14;
	screen.GAME.PAUSE.X = 57;
	screen.EXIT.Y = 10;
	
	screen.EXITSIZE.X = 80;
	screen.EXITSIZE.Y = 26;
	screen.GAMESIZE.X = 80;
	screen.GAMESIZE.Y = 30;
	screen.INFOSIZE.X = 80;
	screen.INFOSIZE.Y = 30;
	screen.MMSIZE.X = 80;
	screen.MMSIZE.Y = 26;
	screen.SETTSIZE.X = 80;
	screen.SETTSIZE.Y = 35;
	
	keys.UP = 'w';
	keys.DOWN = 's';
	keys.LEFT = 'a';
	keys.RIGHT = 'd';
	keys.ROTATE_L = 'q';
	keys.ROTATE_R = 'e';
	
	
	clrscr();
}

void Options(){
	
	int select = 0;
	
	last_width = screen.SETTSIZE.X;
	last_height = screen.SETTSIZE.Y;
	
	clrscr();
	
	EpicText("settings", 2, WHITE);
	
	while(true){
		
		Mid(select == 0 ? "> Key Binding <" : "  Key Binding  ", (80 - strlen("> Key Binding <")) / 2, screen.SETTINGS.Y, select == 0 ? WHITE : DARKGRAY);
		Mid(select == 1 ? (sound ? "> Sound ON < " : "> Sound OFF <") : (sound ? "  Sound ON   " : "  Sound OFF  "), (80 - strlen("> Sound OFF <")) / 2, screen.SETTINGS.Y + 1, select == 1? WHITE : DARKGRAY);
		Mid(select == 2 ? "> Reset HighScores <" : "  Reset HighScores  ", (80 - strlen("> Reset HighScores <")) / 2, screen.SETTINGS.Y + 4, select == 2 ? WHITE : DARKGRAY);
		Mid(select == 3 ? "> Reset Defaults <" : "  Reset Defaults  ", (80 - strlen("> Reset Defaults <")) / 2, screen.SETTINGS.Y + 5, select == 3 ? WHITE : DARKGRAY);
	
		Mid(select == 4 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.SETTINGS.Y + 8, select == 4 ? WHITE : DARKGRAY);
		
		key = _getch();

		if(key == 0 || key == 224){
			key += _getch();
		}
		
		if((key == keys.UP || key == 'w') && select > 0){
			select--;
		}
		
		else if((key == keys.DOWN || key == 's') && select < 4){
			select++;
		}
		
		else if(key == 13){
			
			

			if(select == 0){
				
				clrscr();
					
				EpicText("settings", 2, WHITE);
					
				Mid("> Key Binding <", 13 - strlen("> Key Binding <") / 2, 13, DARKGRAY);
				Mid(sound ? "  Sound ON   " : "  Sound OFF  ", 14 - strlen("> Key Binding <") / 2, 14, DARKGRAY);
				Mid("  Reset HighScores  ", 14 - strlen("> Reset HighScores <") / 2, 17, DARKGRAY);
				Mid("  Reset Defaults  ", 14 - strlen("> Reset defaults <") / 2, 18, DARKGRAY);
				Mid("  Back  ", 14 - strlen("> Back <") / 2, 21, DARKGRAY);
				
				int sel = 0;
				
				while(true){
					
					Mid("Keys", (80 - strlen("Keys")) / 2, 11, WHITE);
					Mid("Primary", 50, 11, WHITE);
					Mid("Secondary*", 60, 11, WHITE);
					
					Mid(sel == 0 ? "> Up" : "  Up", (80 - strlen("Keys")) / 2 - 1, 14, sel == 0 ? WHITE : DARKGRAY);
					crmove(53, 14);
					printf("%c", Keys(keys.UP));
					crmove(63, 14);
					printf("W");
					
					Mid(sel == 1 ? "> Down" : "  Down", (80 - strlen("Keys")) / 2 - 1, 15, sel == 1 ? WHITE : DARKGRAY);
					crmove(53, 15);
					printf("%c", Keys(keys.DOWN));
					crmove(63, 15);
					printf("S");
					
					Mid(sel == 2 ? "> Left" : "  Left", (80 - strlen("Keys")) / 2 - 1, 16, sel == 2 ? WHITE : DARKGRAY);
					crmove(53, 16);
					printf("%c", Keys(keys.LEFT));
					crmove(63, 16);
					printf("A");
					
					Mid(sel == 3 ? "> Right" : "  Right", (80 - strlen("Keys")) / 2 - 1, 17, sel == 3 ? WHITE : DARKGRAY);
					crmove(53, 17);
					printf("%c", Keys(keys.RIGHT));
					crmove(63, 17);
					printf("D");
					
					Mid(sel == 4 ? "> Rotate left" : "  Rotate left", (80 - strlen("Keys")) / 2 - 1, 18, sel == 4 ? WHITE : DARKGRAY);
					crmove(53, 18);
					printf("%c", Keys(keys.ROTATE_L));
					crmove(63, 18);
					printf("Q");
					
					Mid(sel == 5 ? "> Rotate right" : "  Rotate right", (80 - strlen("Keys")) / 2 - 1, 19, sel == 5 ? WHITE : DARKGRAY);
					crmove(53, 19);
					printf("%c", Keys(keys.ROTATE_R));
					crmove(63, 19);
					printf("E");
					
					Mid(sel == 6 ? "> Back <" : "  Back  " , (80 - strlen("> Back <")) / 2, 21, sel == 6 ? WHITE : DARKGRAY);
					
					Mid("*can't be changed - backup keys", 80 - strlen("*can't be changed - backup keys") - 1, 25, DARKGRAY);
					
					key = _getch();
					
					if(key == 0 || key == 224){
						key += _getch();
					}
					
					if((key == 'w' || key == keys.UP) && sel > 0){
						sel--;
					}
					
					else if((key == 's' || key == keys.DOWN) && sel < 6){
						sel++;
					}
					
					else if(key == 13){
						if(sel != 6){
							Mid("Press any key, ESC for cancer", (80 - strlen("Press any key, ESC for cancer")) / 2, 9, WHITE);
							
							key = _getch();
							
							if(key == 0 || key == 224){
								key += _getch();
							}
							
							if(key != 27){
								if(sel == 0) keys.UP = key;
								else if(sel == 1) keys.DOWN = key;
								else if(sel == 2) keys.LEFT = key;
								else if(sel == 3) keys.RIGHT = key;
								else if(sel == 4) keys.ROTATE_L = key;
								else if(sel == 5) keys.ROTATE_R = key;
							}
							
							
							Mid("                                     ", (80 - strlen("                                     ")) / 2, 9, WHITE);
						}
						
						else if(sel == 6){
							for(int row = 9; row < 25; row++){
			
								crmove(0, row);
								
								for(int column = 0; column < 80; column++){
									printf(" ");
								}
							}
							
							break;
						}
					}
				}
			}
			
			else if(select == 1){
				sound = !sound;
				if(!sound) PlaySound(0,0,0);
				else PlaySound(song, NULL, SND_LOOP | SND_ASYNC);
			}
			else if(select == 2){
				
			}
			else if(select == 3){
				
			}
			else if(select == 4){
				clrscr();
				return;
			}
			
			last_width = screen.SETTSIZE.X;
			last_height = screen.SETTSIZE.Y;
		}
	}
}

void Info(){
	
	int select = 2;
	
	last_width = screen.INFOSIZE.X;
	last_height = screen.INFOSIZE.Y;
	
	clrscr();
	
	EpicText("tetris", 2, WHITE);
	
	Mid("(c) 2016 David Knieradl",(80 - strlen("(c) 2016 David Knieradl")) / 2, screen.INFO.Y, WHITE);
	Mid("Song: Leonell Cassio - TiME", (80 - strlen("Song: Leonell Cassio - TiME")) / 2, screen.INFO.Y + 1, WHITE);
	
	
	//printf("Info\n");
	while(true){
		Mid(select == 1 ? "> Leonell's SC <" : "  Leonell's SC  ", (80 - strlen("  Leonell's SC  ")) / 2, screen.INFO.Y + 5, select == 1 ? WHITE : DARKGRAY);
		Mid(select == 2 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.INFO.Y + 8,select == 2 ? WHITE : DARKGRAY);
		
		key = _getch();
					
		if(key == 0 || key == 224){
			key += _getch();
		}
		
		if((key == keys.UP || key == 'w') && select > 0){
			select--;
		}
		
		else if((key == keys.DOWN || key == 's') && select < 2){
			select++;
		}
		
		else if(key == 13){
			if(select == 0){
				
			}
			
			else if(select == 1){
				ShellExecute(NULL, "open", "https://soundcloud.com/leonellcassio", NULL, NULL, SW_SHOWNORMAL);
			}
			
			else if(select == 2){
				clrscr();
				return;
			}
			
			last_width = screen.INFOSIZE.X;
			last_height = screen.INFOSIZE.Y;
		}
		
	}
	
}

bool Quit(){
	
	bool exit = false;
	
	last_width = screen.EXITSIZE.X;
	last_height = screen.EXITSIZE.Y;
	
	while(true){
		
		clrscr();
		
		
		
		Mid("Are you sure you want to quit this game?", (80 - strlen("Are you sure you want to quit this game?")) / 2, screen.EXIT.Y, WHITE);
		
		Mid(exit ? "> Yes <" : "  Yes  ", 20, screen.EXIT.Y + 3, exit ? WHITE : DARKGRAY);
		Mid(!exit ? "> No <" : "  No  ", 54, screen.EXIT.Y + 3, !exit ? WHITE : DARKGRAY);
		
		key = _getch();
					
		if(key == 0 || key == 224){
			key += _getch();
		}
		
		if(key == keys.LEFT || key == 'a'){
			exit = true;
		}
		
		if(key == keys.RIGHT || key == 'd'){
			exit = false;
		}
		
		if(key == 13){
			clrscr();
			return exit;
		}
	}
		
}

bool Pause(){
	
	int select = 0;
	
	while(true){
		
		Render(DARKGRAY);
		
		Mid("PAUSED", screen.GAME.PAUSE.X - strlen("PAUSED") / 2, screen.GAME.PAUSE.Y, WHITE);
		Mid(select == 0 ? "> Resume <" : "  Resume  ", screen.GAME.PAUSE.X - strlen("> Resume <") / 2, screen.GAME.PAUSE.Y + 2, select == 0 ? WHITE : DARKGRAY);
		Mid(select == 1 ? "> Restart <" : "  Restart  ", screen.GAME.PAUSE.X - strlen("> Restart <") / 2, screen.GAME.PAUSE.Y + 3, select == 1 ? WHITE : DARKGRAY);
		Mid(select == 2 ? "> Settings <" : "  Settings  ", screen.GAME.PAUSE.X - strlen("> Settings <") / 2, screen.GAME.PAUSE.Y + 4, select == 2 ? WHITE : DARKGRAY);
		Mid(select == 3 ? "> Info <" : "  Info  ", screen.GAME.PAUSE.X - strlen("> Info <") / 2, screen.GAME.PAUSE.Y + 5, select == 3 ? WHITE : DARKGRAY);
		Mid(select == 4 ? "> Return to MainMenu <" : "  Return to MainMenu  ", screen.GAME.PAUSE.X - strlen("> Return to MainMenu <") / 2, screen.GAME.PAUSE.Y + 8, select == 4 ? WHITE : DARKGRAY);
		
		key =_getch();
		
		if(key == 0 || key == 224){
			key += _getch();
		}
		
		if((key == 'w' || key == keys.UP) && select > 0){
			select--;
		}
		
		else if((key == 's' || key == keys.DOWN) && select < 4){
			select++;
		}
		
		else if(key == 13){
			if(select == 0) return false;
			//else if(select == 1) 
			else if(select == 2) Options();
			else if(select == 3) Info();
			else if(select == 4) {
				clrscr();
				
				bool qt_select = false;
				
				Mid("Are you sure you want to go to MainMenu?", last_width - strlen("Are you sure you want to go to MainMenu?") / 2, screen.EXIT.Y, WHITE);
				Mid("Progress will be lost!", last_width - strlen("Progress will be lost!") / 2 + 1, screen.EXIT.Y, WHITE);
				Mid(qt_select ? "> Yes <" : "  Yes  ", 28, screen.EXIT.Y + 4, qt_select ? WHITE : DARKGRAY);
				Mid(qt_select ? "> No <" : "  No  ", 52, screen.EXIT.Y + 4, qt_select ? WHITE : DARKGRAY);
				
				key = _getch();
				
				if(key == 'a' || key == keys.LEFT) qt_select = true;
				if(key == 'd' || key == keys.RIGHT) qt_select = false;
			}
			
			last_width = screen.GAMESIZE.X;
			last_height = screen.GAMESIZE.Y;
			
			changed= true;
		}
		
		
	}
	
	
}

char Keys(int key){
	
	if(key == UP_K) return 24;
	else if(key == DOWN_K) return 25;
	else if(key == LEFT_K) return 27;
	else if(key == RIGHT_K) return 26;
	else return key;
}

void EpicText(const char TEXT[], int y, int color){

	int chr = 0;
	char text[strlen(TEXT)];
	
	strcpy(text, TEXT);
	
	/*
		 ********  *******   ********  *******   ********  xxxxxxxx  xxxxxxxx  x      x  xxxxx       x  x    x   x        xxxxxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxx   xxxxxxxx xxxxxxx    xxxxxxx  xxxxxxxxx  x      x  x      x  x   x   x  xx     xx  x     x  xxxxxxxx   xxxxxx  xxx   xxxxxx  xxxxxx  x    x  xxxxxx  x       xxxxxx  xxxxxx  xxxxxx        x   xxx    x  x   xxx  x   x   x  x    xx x xx            x                            
		 *      *  *      *  *         *      *  *         x         x         x      x    x         x  x   x    x        x   x   x  x      x  x      x  x      x  x      x x      x  x             x      x      x  x     x   x   x   x    xx xx    x     x       xx    x   xx    x        x       x  x    x  x       x            x  x    x  x    x        x  x   x  xxxxxx  x x x    x  x    x     xxx             x
		 ********  ********  *         *      *  ********  xxxxxxxx  x    xxx  xxxxxxxx    x         x  xxxxxxx  x        x   x   x  x      x  x      x  xxxxxxxx  x      x xxxxxxxx  xxxxxxxx      x      x      x  x    x    x   x   x      x      xxxxxxx     xx      x xx x    x   xxxxxx  xxxxxx  xxxxxx  xxxxxx  xxxxxx       x  xxxxxx  xxxxxx        x     x    x  x   xxxxxxx     x    x   xxxxxxx  xxxx    x                            
		 *      *  *      *  *         *      *  *         x         x      x  x      x    x         x  x     x  x        x   x   x  x      x  x      x  x         x   x  x x      x         x      x      x      x  x   x     x   x   x    xx xx       x      xx        xx   x    x   x            x       x       x  x    x       x  x    x       x     x            xxxxxx    x x x     x    x     xxx           x
		 *      *  ********  ********  ********  ********  x         xxxxxxxx  x      x  xxxxx  xxxxxx  x     x  xxxxxxx  x   x   x  x      x  xxxxxxxx  x         xxxxxxxx x      x  xxxxxxxx      x      xxxxxxxx  xxxx      xxxxxxxxx  xx     xx     x     xxxxxxxx   xxxxxx  xxxxx xxxxxx  xxxxxx       x  xxxxxx  xxxxxx       x  xxxxxx       x  x  x  x    x     x  x    x  xxx      x  x    xx x xx        x                                                                                                                                                           
	*/
	
	int string_length = 0;
	//              a b c d e f g h i j k l m n o p q r s t u v w x y z . , ! ? # % ( ) * - / _ 0 1 2 3 4 5 6 7 8 9 
	int length[] = {8,8,8,8,8,8,8,8,5,6,7,7,9,8,8,8,8,7,8,7,8,9,9,7,8,8,1,1,1,5,6,7,2,2,7,4,5,3,6,5,6,6,6,6,6,6,6,6};
	char chrs[] = {"abcdefghijklmnopqrstuvwxyz.,!?#%()*-/ 0123456789"};
	
	for(row = 0; row < strlen(TEXT); row++){
		for(column = 0; column < sizeof(length)/sizeof(int); column++){
			if(text[row] >= 65 && text[row] <=90) text[row] = text[row] + 32;	
			if(text[row] == chrs[column]) string_length += length[column];
		}
		if(row < strlen(text)-1) string_length += 1;	
	}
	
	
	for(row = -1; row < 6; row++){
			
		crmove((80 - string_length) / 2, y + row);
		
		//for(int row = 0; row < (80 - string_length)/2; row++) fputs(" ");
		
		for(chr = 0; chr < strlen(text); chr++){
			
			if(row == -1 || row == 5){
				textcolor(BLACK);
				for(int row = 0; row < string_length; row++) printf("\xDB");
				break;
			}
			
			textcolor(color);
			
			if(text[chr] == 'a'){
				
				if(row == 0 || row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 3 || row == 4) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'b'){
				
				if(row == 0 || row == 2 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row == 1 || row == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'c'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 2 || row == 3) fputs("\xDB       ", stdout);
			}
			else if(text[chr] == 'd'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row == 1 || row == 2 || row == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'e'){
				
				if(row == 0 || row == 2 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 3) fputs("\xDB       ", stdout);
			}
			else if(text[chr] == 'f'){
				
				if(row == 0 || row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 3 || row == 4) fputs("\xDB       ", stdout);
			}
			else if(text[chr] == 'g'){
				
				if(row == 0 ||row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1) fputs("\xDB       ", stdout);
				else if(row == 2) fputs("\xDB    \xDB\xDB\xDB", stdout);
				else if(row == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'h'){
				
				if(row == 0 || row == 1 || row == 3 || row == 4) fputs("\xDB      \xDB", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'i'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 2 || row == 3) fputs("  \xDB  ", stdout);
			}
			else if(text[chr] == 'j'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3) fputs("     \xDB", stdout);
				else if(row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'k'){
				
				if(row == 0) fputs("\xDB     \xDB", stdout);
				else if(row == 1) fputs("\xDB    \xDB ", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 3 || row == 4) fputs("\xDB     \xDB", stdout);
			}			
			else if(text[chr] == 'l'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3) fputs("\xDB      ", stdout);
				else if(row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'm'){
				
				if(row == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 2 || row == 3 || row == 4) fputs("\xDB   \xDB   \xDB", stdout);
			}
			else if(text[chr] == 'n'){
				
				if(row == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row == 1 || row == 2 || row == 3 || row == 4) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'o'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 2 || row == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'p'){
				
				if(row == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row == 1 || row == 3 || row == 4) fputs("\xDB      \xDB", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'q'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 2) fputs("\xDB      \xDB", stdout);
				else if(row == 3) fputs("\xDB    \xDB \xDB", stdout);
			}
			else if(text[chr] == 'r'){
				
				if(row == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row == 1 || row == 3 || row == 4) fputs("\xDB     \xDB", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
			}
			else if(text[chr] == 's'){
				
				if (row == 0)  fputs(" \xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 0 || row == 2 || row == 4)  fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1) fputs("\xDB       ", stdout);
				else if(row == 3) fputs("       \xDB", stdout);
			}
			else if(text[chr] == 't'){
				
				if(row == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 2 || row == 3 || row == 4) fputs("   \xDB   ", stdout);
			}
			else if(text[chr] == 'u'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3) fputs("\xDB      \xDB", stdout);
				else if(row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
			}
			else if(text[chr] == 'v'){
				
				if(row == 0) fputs("\xDB      \xDB", stdout);
				else if(row == 1) fputs("\xDB     \xDB ", stdout);
				else if(row == 2) fputs("\xDB    \xDB  ", stdout);
				else if(row == 3) fputs("\xDB   \xDB   ", stdout);
				else if(row == 4) fputs("\xDB\xDB\xDB\xDB    ", stdout);
			}
			else if(text[chr] == 'w'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3) fputs("\xDB   \xDB   \xDB", stdout);
				else if(row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
			}
			else if(text[chr] == 'x'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB     \xDB\xDB", stdout);
				else if(row == 1 || row == 3) fputs("  \xDB\xDB \xDB\xDB  ", stdout);
				else if(row == 2) fputs("    \xDB    ", stdout);
			}
			else if(text[chr] == 'y'){
				
				if(row == 0 || row == 1) fputs("\xDB     \xDB", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 3 || row == 4) fputs("   \xDB   ", stdout);
			}
			else if(text[chr] == 'z'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1)  fputs("     \xDB\xDB ", stdout);
				else if(row == 2) fputs("   \xDB\xDB   ", stdout);
				else if(row == 3) fputs(" \xDB\xDB     ", stdout);
			}
			else if(text[chr] == '.'){
				
				if(row == 0 || row == 1 || row == 2 || row == 3) fputs(" ", stdout);
				else if(row == 4) fputs("\xDB", stdout);
			}
			else if(text[chr] == ','){
				
				if(row == 0 || row == 1 || row == 2) fputs(" ", stdout);
				else if(row == 3 || row == 4) fputs("\xDB", stdout);
			}
			else if(text[chr] == '!'){
				
				if(row == 0 || row == 1 || row == 2 || row == 4) fputs("\xDB", stdout);
				else if(row == 3) fputs(" ", stdout);
			}
			else if(text[chr] == '?'){
				
				if(row == 0) fputs(" \xDB\xDB\xDB ", stdout);
				else if(row == 1) fputs("\xDB   \xDB", stdout);
				else if(row == 2) fputs("   \xDB ", stdout);
				else if(row == 3) fputs("     ", stdout);
				else if(row == 4) fputs("  \xDB  ", stdout);
			}
			else if(text[chr] == '#'){
				
				if(row == 0 || row == 2 || row == 4) fputs(" \xDB  \xDB ", stdout);
				else if(row == 1 || row == 3) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == '%'){
				
				if(row == 0) fputs("\xDB\xDB\xDB  \xDB ", stdout);
				else if(row == 1) fputs("\xDB \xDB \xDB  ", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 3) fputs("  \xDB \xDB \xDB", stdout);
				else if(row == 4) fputs(" \xDB  \xDB\xDB\xDB", stdout);
			}			
			else if(text[chr] == '('){
				
				if(row == 0 || row == 4) fputs(" \xDB", stdout);
				else if(row == 1 || row == 2 || row == 3) fputs("\xDB ", stdout);
			}			
			else if(text[chr] == ')'){
				
				if(row == 0 || row == 4) fputs("\xDB ", stdout);
				else if(row == 1 || row == 2 || row == 3) fputs(" \xDB", stdout);
			}			
			else if(text[chr] == '*'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB \xDB \xDB\xDB", stdout);
				else if(row == 1 || row == 3) fputs("  \xDB\xDB\xDB  ", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}			
			else if(text[chr] == '-'){
				
				if(row == 0 || row == 1 || row == 3 || row == 4) fputs("    ", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB", stdout);
			}			
			else if(text[chr] == '/'){
				
				if(row == 0) fputs("    \xDB", stdout);
				else if(row == 1) fputs("   \xDB ", stdout);
				else if(row == 2) fputs("  \xDB  ", stdout);
				else if(row == 3) fputs(" \xDB   ", stdout);
				else if(row == 4) fputs("\xDB    ", stdout);
			}		
			else if(text[chr] == ' '){
				
				 fputs("   ", stdout);
			}
			else if(text[chr] == '0'){
				
				if(row == 0 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1) fputs("\xDB   \xDB\xDB", stdout);
				else if(row == 2) fputs("\xDB \xDB\xDB \xDB", stdout);
				else if(row == 3) fputs("\xDB\xDB   \xDB", stdout);
			}
			else if(text[chr] == '1'){
				
				if(row == 0) fputs("\xDB\xDB\xDB  ", stdout);
				else if(row == 1 || row == 2 || row == 3) fputs("  \xDB  ", stdout);
				else if(row == 4) fputs("\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == '2'){
				
				if(row == 0 || row == 2 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1) fputs("     \xDB", stdout);
				else if(row == 3) fputs("\xDB     ", stdout);
			}
			else if(text[chr] == '3'){
				
				if(row == 0 || row == 2 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 3) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '4'){
				
				if(row == 0 || row == 1) fputs("\xDB    \xDB", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 3 || row == 4) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '5'){
				
				if(row == 0 || row == 2 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1) fputs("\xDB     ", stdout);
				else if(row == 3) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '6'){
				
				if(row == 0 || row == 1) fputs("\xDB     ", stdout);
				else if(row == 2 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 3) fputs("\xDB    \xDB", stdout);
			}
			else if(text[chr] == '7'){
				
				if(row == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 2 || row == 3 || row == 4) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '8'){
				
				if(row == 0 || row == 2 || row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1 || row == 3) fputs("\xDB    \xDB", stdout);
			}
			else if(text[chr] == '9'){
				
				if(row == 0 || row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 1) fputs("\xDB    \xDB", stdout);
				else if(row == 3 || row == 4) fputs("     \xDB", stdout);
			}
			
			else  fputs("CHYBA", stdout);
			
			if(chr < strlen(TEXT)-1)  fputs(" ", stdout);
		}

	}
}

void Defaults(){
	
	block.CUR = 0;
	block.NEXT = 0;
	block.HEIGHT = 0;
	block.WIDTH = 0;
	block.POS.X = 0;
	block.POS.Y = 0;
	
	keys.UP = 'w';
	keys.DOWN = 's';
	keys.LEFT = 'a';
	keys.RIGHT = 'd';
	keys.ROTATE_L = 'q';
	keys.ROTATE_R = 'e';
	keys.DROP = SPACE_K;

	sound = true;
	changed = false;

	temp = 0;
	last_width = 80;
	last_height = 26;
	row = 0;
	column = 0;
}

void Mid(const char TEXT[], int x, int y, int color){
	textcolor(color);
	crmove(x, y);
	
	fputs(TEXT, stdout);
}

void textattr(int _attr) {
	SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), _attr);
}

void textcolor(int _color) {
	CONSOLE_SCREEN_BUFFER_INFO Info ;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info) ;
	textattr((Info.wAttributes & 0xF0) | _color);
}

void crmove(int x, int y) {
	COORD c ;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clrscr(){
	COORD coord = { 0, 0 } ;
	DWORD dwWritten, dwSize ;
	HANDLE hScreen=GetStdHandle(STD_OUTPUT_HANDLE) ;
	CONSOLE_SCREEN_BUFFER_INFO Info;
	
	if(GetConsoleScreenBufferInfo(hScreen, &Info)){
		dwSize = Info.dwSize.X * Info.dwSize.Y ;
		FillConsoleOutputCharacter (hScreen, ' ', dwSize, coord, &dwWritten);
		FillConsoleOutputAttribute (hScreen, Info.wAttributes, dwSize, coord, &dwWritten);
		crmove(0, 0);
	}
}

void _setcursortype(int type){
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = TRUE;
	
	if (type == _NOCURSOR){
		Info.dwSize = 100;
		Info.bVisible = FALSE;
	} 
	else if (type == _SOLIDCURSOR) Info.dwSize = 90 ;
	else if (type == _NORMALCURSOR) Info.dwSize = 1;
	
	SetConsoleCursorInfo (GetStdHandle (STD_OUTPUT_HANDLE), &Info);
}

void SetWindow(int x, int y) { 
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(h, &bufferInfo);
    SMALL_RECT& winInfo = bufferInfo.srWindow;
    COORD windowSize = { winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1};

    if (windowSize.X > x || windowSize.Y > y)
    {
        // window size needs to be adjusted before the buffer size can be reduced.
        SMALL_RECT info = 
        { 
            0, 
            0, 
            x < windowSize.X ? x-1 : windowSize.X-1, 
            y < windowSize.Y ? y-1 : windowSize.Y-1 
        };

		SetConsoleWindowInfo(h, TRUE, &info);
    }

    COORD size = { x, y };
    SetConsoleScreenBufferSize(h, size);

    SMALL_RECT info = { 0, 0, x - 1, y - 1 };
    SetConsoleWindowInfo(h, TRUE, &info);
}

void SetFont(int size_x, int size_y){
	CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = size_x;
    cfi.dwFontSize.Y = size_y;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    
    wcscpy(cfi.FaceName, L"Terminal");
    
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void WindowSize(){
	while(true){
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		
		int Width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int Height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		
		if(last_height != Height || last_width != Width){
			clrscr();
			SetWindow(last_width, last_height);
		}
		
		Sleep(1);
	}
	
	
}

DWORD WINAPI WindowResizedEvent(void* data){
	WindowSize();
}
