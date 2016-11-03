
/// 	Tetris 0.12 Beta
/// 	Copyright: David Knieradl
/// 	Author: David Knieradl
/// 	Description: rocnikova prace



/////////////////////////////////////////////////////
///												  ///
///		Pridat -lwinmm do linkeru kompilátoru	  ///
///												  ///
/////////////////////////////////////////////////////


/*
    -------- TO DO ---------

	*	   EVERYTHING	   *
	
	IMPLEMENT MODIFICATORS
	ANIMATIONS INFO
	BUGS CORRECTION
*/

#pragma comment(lib,"libwinmm.a")

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

enum COLORS {
	BLACK,			// 0
	BLUE,			// 1
	GREEN,			// 2
	CYAN,			// 3
	RED,			// 4
	MAGENTA,		// 5
	BROWN,			// 6
	LIGHTGRAY,		// 7
    DARKGRAY,		// 8
	LIGHTBLUE,		// 9
	LIGHTGREEN,		// 10
	LIGHTCYAN,		// 11
	LIGHTRED,		// 12
	LIGHTMAGENTA,	// 13
	YELLOW,			// 14
	WHITE			// 15
};

enum BOARDFULL{
	NONE,			// 0
	QUAD,			// 1
	HALF,			// 2
	THREEQUAD		// 3
};

enum CURBL{
	I,
	T,
	O,
	J,
	L,
	Z,
	S
};

typedef struct{
	float TIMEFACTOR;		// done
	bool CHAINREACTION;		// done
	bool UPSIDEDOWN;		// done
	short BOARDFULL;		// done
	bool HARDDROPONLY;		// done
	short TIMERUN;			// 
	bool REBUILD;			//
	int BOARDSIZE;			// done
	bool GHOSTBLOCKS;		//
	bool BUILDER;			// done
} MODS;

typedef struct{
	int UP;
	int DOWN;
	int LEFT;
	int RIGHT;
	int ROTATE_L;
	int ROTATE_R;
	int DROP;
	int NEXTSONG;
	int PAUSE;
	int PRESSED;
	
	bool SAMEKEYEXCEPTION;
} KEYS;

typedef struct{
	int I;
	int J;
	int L;
	int O;
	int S;
	int T;
	int Z;
}BLOCKTYPE;

typedef struct{
	COORD POS;
	short WIDTH;
	short HEIGHT;
	short CUR;
	short NEXT;
	short CURBLOCK_SCORE;
	
	BLOCKTYPE STATS;
} BLOCKPLUS;

typedef struct{
	COORD GAMETYPE;
	COORD MUTATOR;
	
	COORD BOARD;
	COORD CURBLOCK;
	COORD PAUSE;
	
	COORD BRDSIZE;
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
	COORD KBINDSIZE;
	
	int TETRIS;
} RENDER;

typedef struct{
	bool MAINMENU;
	bool GAME;
	bool GAMETYPE;
	bool PAUSE;
	bool PAUSERETURN;
	bool SETTINGS;
	bool KEYS;
	bool INFO;
	bool EXIT;
	bool GAMELOADED;
	bool MUTATOR;
	bool HELP;
} PAGE;

typedef struct{
	int MAINMENU;
	int PAUSE;
	int SETTINGS;
	int GAMETYPE;
	int KEYS;
	int INFO;
	int EXIT;
	int MUTATOR;
	int HELP;
	int HELPPAGE;
} SELECT;

//
//	Pages
//

void GameType();
void Game();
void Options();
void Info();
bool Quit();
bool Pause();

//
//	Functions
//

void OnLoad();
void Defaults();
bool KeyBindingChecker();
void GetKey();
char * Keys(int key);
void EpicText(const char TEXT[], int y, int color);

void Mid(const char TEXT[], int x, int y, int color);
void setcursortype(int type);
void crmove(int x, int y);
void textattr(int _attr);
void textcolor(int _color);
void clrscr();
void SetWindow(int Width, int Height);
void SetFont(int size_x, int size_y);
void WindowSize();
int Digits(int number);
float BoardScoreRatio(int size);

//
//	Threads
//

DWORD WINAPI WindowResizedEvent(void* data);
DWORD WINAPI RenderScreen(void* data);
DWORD WINAPI MusicPlay(void* data);
DWORD WINAPI Timer(void* data);

//
//	Game Mechanics
//

void CurBlock(int sel);
void MoveLeft();
void MoveRight();
void RotateLeft();
void RotateRight();
void CollisionCheck();
void MoveDown();
void BoardWrite();
void FullRowDetection();
void FullRowDetectionChain();

//
//	Help Animations
//

void CurBlockHelp(int sel, int Y);
void MoveLeftHelp();
void MoveRightHelp();
void RotateLeftHelp();
void RotateRightHelp();
void CollisionCheckHelp();
void MoveDownHelp();
void BoardWriteHelp();
void FullRowDetectionHelp();
void FullRowDetectionChainHelp();

#define DEBUG 1

#define DEF DARKGRAY
#define TR true
#define FL false

#define UP_K 296				//up arrow key code
#define DOWN_K 304				//down arrow key code
#define LEFT_K 299				//left arrow key code
#define RIGHT_K 301				//right arrow key code
#define SPACE_K 32				//space key code
#define ESC 27					//esc key code
#define F1 -59					//F+ key code
#define F2 -60					//F2 key code
#define F3 -61					//F3 key code
#define F4 -62					//F4 key code
#define F5 -63					//F5 key code
#define F6 -64					//F6 key code
#define F7 -65					//F7 key code
#define F8 -66					//F8 key code
#define F9 -67					//F9 key code
#define F10 -68					//F10 key code
#define F11 357					//F11 key code
#define F12 358					//F12 key code
#define TAB 9					//TAB key code
#define DEL 307					//DELETE key code


#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2

#define SMALL_W 14
#define MEDIUM_W 18
#define LARGE_W 22

#define SMALL_H  18
#define MEDIUM_H 22
#define LARGE_H 26

#define SMALLSCORE 0.5
#define NORMALSCORE 1
#define LARGESCORE 2

#define BLOCKSCORE 10
#define ROWSCORE 40

#define BASECLEAR 10


//
//	VARIABLES
//


RENDER screen;
BLOCKPLUS block, blockhelp;
KEYS keys;
PAGE page;
SELECT selection;
MODS mods;

bool help[SMALL_H + 1][SMALL_W] = {false};
bool help2[SMALL_H + 1][SMALL_W] = {false};

unsigned char help_color[SMALL_H + 1][SMALL_W] = {0};
unsigned char help2_color[SMALL_H + 1][SMALL_W] = {0};

bool board[LARGE_H + 1][LARGE_W] = {false};
bool board2[LARGE_H + 1][LARGE_W] = {false};

unsigned char board_color[LARGE_H + 1][LARGE_W] = {0};
unsigned char board2_color[LARGE_H + 1][LARGE_W] = {0};

bool sound = true, changed = false, collided = false, qt_select = false, collidedhelp = false;

int temp = 0, last_width = 0, last_height = 0;
int row = 0, column = 0;
int row_th = 0, column_th = 0;

#if !DEBUG
	bool loading = true;
#elif DEBUG
	bool loading = false;
#endif

bool exit_app = false;

int score = 0;
int level = 1;
int thislevelclear = 0;

int delay = 0;
int timeleft = 0;

//
// strings
//

char name[] = "Tetris 0.13 Beta - by David Knieradl";
const char *song[] = {
	"Music/Disease_-_Beautiful_insanity.wav",
	"Music/Fearofdark_-_Hopeless_romantic.wav",
	"Music/Lluvia_-_Chanel_n.wav",
	"Music/Lukas_Eriksson_-_Space_delivery.wav",
	"Music/Shirobon_-_Under_the_moonlight.wav",
	"Music/Shirobon_and_Radix_-_Cyber_party.wav",
	"Music/Tetris.wav",
	"Music/Wiklund_and_Joule_-_Feline_areola.wav",
	"Music/Xemogasa_-_Pulsating_oratorio.wav",
	"Music/Zalza_-_The_Lonesome_Droid.wav"
};

int song_l[] = {
	121,
	167,
	210,
	153,
	208,
	170,
	207,
	128,
	81,
	220
};

const char *help_label[] = {
	"Moving",
	"Scoring",
	"Normal",
	"Chainreaction",
	"Builed",
	"Sharpshooter",
	"Half way",
	"Mutators"
};

const char *help_txt[][8] = {
	{	"Move block by pressing",
		"A - left",
		"D - right",
		"S - down",
		"Q - rotate left",
		"E - rotate right",
		"SPACE - drop"},
		
	{	"Scoring is calculated",
		"with simple formula",
		"Level * BoardSize * TimeFactor"},
		
	{	"Blocks have basic physics",
		"Timefactor - 1x",
		"Boardsize - Normal",
		"Time - none",
		"Boardfill - none"},
	
	{	"Chainreaction physics enabled",
		"Timefactor - 1x",
		"Boardsize - Normal",
		"Time - none",
		"Boardfill - none"},
	
	{	"Normal physics",
		"New rows are generated with time",
		"Boardsize - Normal",
		"Time - none",
		"Boardfill - none"},
	
	{	"Board set to upsidedown",
		"Blocks don't fall down",
		"Can be only droped with SPACE",
		"New rows are generated with time",
		"Boardsize - Normal"},
		
	{	"Half of board filled by default",
		"Blocks have basic physics",
		"Timefactor - 1x",
		"Boardsize - Normal",
		"Time - none"},
	
	{	"Mutators are mods used for",
		"creating your own game",
		"Every property can be set",
		"with your own preferences",
		"No Limits"}
};

int help_txt_rows[] = {
	7,
	3,
	5,
	5,
	5,
	5,
	5,
	5
};

int cur_song = 5;
int max_song = sizeof(song_l) / sizeof(int);

int main(){
	
	OnLoad();
	
	last_height = screen.MMSIZE.Y;
	last_width = screen.MMSIZE.X;
	
	while(loading){
		Sleep(100);
	}
	
	while(_kbhit()) getch();
	
	while(true){
		
		GetKey();
		
		if((keys.PRESSED == keys.UP || keys.PRESSED == 'w') && selection.MAINMENU > 0){
			selection.MAINMENU--;	
			keys.PRESSED = 0;
		}
		
		else if((keys.PRESSED == keys.DOWN || keys.PRESSED == 's') && selection.MAINMENU < 3){
			selection.MAINMENU++;
			keys.PRESSED = 0;
		}
		
		else if(keys.PRESSED == 13){
			keys.PRESSED = 0;
			
			page.MAINMENU = false;
			
			if(selection.MAINMENU == 0) GameType();
			else if(selection.MAINMENU == 1) Options();
			else if(selection.MAINMENU == 2) Info();
			else if(selection.MAINMENU == 3){
				int end = Quit();
				if(end) return 1;
			}
			
			last_height = screen.MMSIZE.Y;
			last_width = screen.MMSIZE.X;
			
			page.MAINMENU = true;
			
		}
	}
}

void GameType(){
	
	page.GAMETYPE = true;

	while(true){
		
		GetKey();
		
		if((keys.PRESSED == keys.UP || keys.PRESSED == 'w') && selection.GAMETYPE > 1){
			selection.GAMETYPE--;
		}
		
		else if((keys.PRESSED == keys.DOWN || keys.PRESSED == 's') && selection.GAMETYPE < 7){
			selection.GAMETYPE++;
		}
		
		else if(keys.PRESSED == 13){
					
			if(selection.GAMETYPE != 7){
				
				mods.BOARDFULL = 0;
				mods.BUILDER = false;
				mods.CHAINREACTION = false;
				mods.BOARDSIZE = 1;
				mods.GHOSTBLOCKS = false;
				mods.HARDDROPONLY = false;
				mods.REBUILD = false;
				mods.TIMEFACTOR = 1;
				mods.TIMERUN = false;
				mods.UPSIDEDOWN = false;
				
				if(selection.GAMETYPE == 1){ page.GAMETYPE = false; Game(); }
				
				else if(selection.GAMETYPE == 2){
					page.GAMETYPE = false;
					mods.CHAINREACTION = true;
					Game();
					
				}
				else if(selection.GAMETYPE == 3){
					page.GAMETYPE = false;
					mods.BUILDER = true;
					Game();
				}
				else if(selection.GAMETYPE == 4){
					page.GAMETYPE = false;
					mods.HARDDROPONLY = true;
					mods.BUILDER = true;
					mods.UPSIDEDOWN = true;
					Game();
				}
				
				else if(selection.GAMETYPE == 5){
					page.GAMETYPE = false;
					mods.BUILDER = true;
					mods.BOARDFULL = HALF;
					Game();
				}
				else if(selection.GAMETYPE == 6){
					page.MUTATOR = true;
					
					while(page.MUTATOR){
						
						GetKey();
						
						if((keys.PRESSED == keys.UP || keys.PRESSED == 'w') && selection.MUTATOR > 0){ selection.MUTATOR--; }
						else if((keys.PRESSED == keys.DOWN || keys.PRESSED == 's') && selection.MUTATOR < 11){ selection.MUTATOR++; }
						
						else if(keys.PRESSED == keys.LEFT || keys.PRESSED == 'a'){
							if(selection.MUTATOR == 0){ mods.TIMEFACTOR /= 2; if(mods.TIMEFACTOR < 0.25) mods.TIMEFACTOR = 4; }							
							else if(selection.MUTATOR == 1){ mods.CHAINREACTION = false; }							
							else if(selection.MUTATOR == 2){ mods.UPSIDEDOWN = false; }							
							else if(selection.MUTATOR == 3){ mods.BOARDFULL -= 1; if(mods.BOARDFULL < 0) mods.BOARDFULL = 3; }							
							else if(selection.MUTATOR == 4){ mods.HARDDROPONLY = false; }							
							else if(selection.MUTATOR == 5){ mods.TIMERUN -= 1; if(mods.TIMERUN < -1) mods.TIMERUN = 5; }							
							//else if(selection.MUTATOR == 6){ mods.TIMERUN = -= 1; mods }							
							else if(selection.MUTATOR == 6){ mods.REBUILD = false; }							
							else if(selection.MUTATOR == 7){ mods.BOARDSIZE -= 1; if(mods.BOARDSIZE < 0) mods.BOARDSIZE = 2;}
							else if(selection.MUTATOR == 8){ mods.GHOSTBLOCKS = false; }							
							else if(selection.MUTATOR == 9){ mods.BUILDER = false; }
						}
						
						else if(keys.PRESSED == keys.RIGHT || keys.PRESSED == 'd'){
							if(selection.MUTATOR == 0){ mods.TIMEFACTOR *= 2; if(mods.TIMEFACTOR > 4) mods.TIMEFACTOR = 0.25; }							
							else if(selection.MUTATOR == 1){ mods.CHAINREACTION = true; }							
							else if(selection.MUTATOR == 2){ mods.UPSIDEDOWN = true; }							
							else if(selection.MUTATOR == 3){ mods.BOARDFULL += 1; if(mods.BOARDFULL > 3) mods.BOARDFULL = 0; }							
							else if(selection.MUTATOR == 4){ mods.HARDDROPONLY = true; }							
							else if(selection.MUTATOR == 5){ mods.TIMERUN += 1; if(mods.TIMERUN > 5) mods.TIMERUN = -1; }							
							//else if(selection.MUTATOR == 6){ mods.TIMERUN = true; }							
							else if(selection.MUTATOR == 6){ mods.REBUILD = true; }							
							else if(selection.MUTATOR == 7){ mods.BOARDSIZE += 1; if(mods.BOARDSIZE > 2) mods.BOARDSIZE = 0;}							
							else if(selection.MUTATOR == 8){ mods.GHOSTBLOCKS = true; }							
							else if(selection.MUTATOR == 9){ mods.BUILDER = true; }
						}
						
						else if(keys.PRESSED == 13){
							if(selection.MUTATOR == 10){
								page.MUTATOR = false;
								page.GAMETYPE = false;
								Game();
							}
							else if(selection.MUTATOR == 11){
								page.MUTATOR = false;
							}
						}
					}
				}
			}
				
			else{
				page.GAMETYPE = false;
				return;	
			}
			
			last_height = screen.MMSIZE.Y;
			last_width = screen.MMSIZE.X;
			
			page.GAMETYPE = true;
		}
	}
}

void Game(){
	
	page.GAME = true;
	
	last_width = screen.GAMESIZE.X;
	last_height = screen.GAMESIZE.Y;
	
	bool end = false;
	bool down_brk = false;
	
	short build = 0;
	
	score = 0;
	level = 1;
	thislevelclear = BASECLEAR;
	
	screen.GAME.BRDSIZE.X = (mods.BOARDSIZE == 0 ? SMALL_W : (mods.BOARDSIZE == 1 ? MEDIUM_W : LARGE_W));
	screen.GAME.BRDSIZE.Y = (mods.BOARDSIZE == 0 ? SMALL_H : (mods.BOARDSIZE == 1 ? MEDIUM_H : LARGE_H));
	
	memset(board, false, sizeof(board[0][0]) * LARGE_H * LARGE_W);
	memset(board2, false, sizeof(board2[0][0]) * LARGE_H * LARGE_W);
	
	memset(board_color, DARKGRAY, sizeof(board_color[0][0]) * LARGE_H * LARGE_W);
	memset(board2_color, DARKGRAY, sizeof(board2_color[0][0]) * LARGE_H * LARGE_W);
	
	delay = 50 / mods.TIMEFACTOR;
	
	if(mods.BOARDFULL != 0){
		for(row = screen.GAME.BRDSIZE.Y - (screen.GAME.BRDSIZE.Y / 4) * mods.BOARDFULL; row < screen.GAME.BRDSIZE.Y; row++){
			for(column = 0; column < screen.GAME.BRDSIZE.X - 2; column++){
				int blockis = rand() % 16;
				while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis = rand() % 16;
				
				if(rand() % 2 == 0){
					board[row][column + 1] = true;
					board_color[row][column + 1] = blockis;
				}
			}
		}
	}
	
	block.NEXT = rand() % 7 + 1;
	
	while(!end){
		
		if(mods.BUILDER){
			if(build >= 20 + 5 * level){
				build = 0;
				
				for(row = 1; row < screen.GAME.BRDSIZE.Y; row++){
					for(column = 0; column < screen.GAME.BRDSIZE.X; column++){
						board[row - 1][column + 1] = board[row][column + 1];
						board[row][column + 1] = false;
						
						board_color[row - 1][column + 1] = board_color[row][column + 1];
						board_color[row][column + 1] = false;
					}
				}
				
				for(column = 0; column < screen.GAME.BRDSIZE.X - 2; column++){
					int blockis = rand() % 16;
					while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis = rand() % 16;
					
					if(rand() % 2 == 0){
						board[screen.GAME.BRDSIZE.Y - 1][column + 1] = true;
						board_color[screen.GAME.BRDSIZE.Y - 1][column + 1] = blockis;
					}
					else board_color[screen.GAME.BRDSIZE.Y - 1][column + 1] = DARKGRAY;
				}
			}
		}
		
		
		CurBlock(block.CUR = block.NEXT);
		block.NEXT = rand() % 7 + 1;
		page.GAMELOADED = true;
		
		if(delay > 2)delay = (50 - 2 * level) / mods.TIMEFACTOR;
		
		
		
		collided = false;
		
		while(!collided){
			for(int i = 0; i < 30 && !collided; i++){
				
				Sleep(delay);
				
				if(_kbhit()){
					
					GetKey();
					
					//
					// MOVE TO LEFT
					//
					
					if((keys.PRESSED == keys.LEFT || keys.PRESSED == 'a') && block.POS.X > 1){
						
						MoveLeft();
						
						bool canmove = true;
						
						for(row = block.POS.Y; row > block.POS.Y - block.HEIGHT && canmove; row--){
							for(column = block.POS.X; column < block.POS.X + block.WIDTH && canmove; column++){
								if(board[row][column] && board2[row][column]){
									canmove = false;
								}
							}
						}
						
						if(!canmove) MoveRight();
					}
					
					//
					// MOVE TO RIGHT
					//
					
					else if((keys.PRESSED == keys.RIGHT || keys.PRESSED == 'd') && block.POS.X + block.WIDTH - 1 < screen.GAME.BRDSIZE.X - 2){
						
						MoveRight();
						
						bool canmove = true;
						
						for(row = block.POS.Y; row > block.POS.Y - block.HEIGHT && canmove; row--){
							for(column = block.POS.X; column < block.POS.X + block.WIDTH && canmove; column++){
								if(board[row][column] && board2[row][column]){
									canmove = false;
								}
							}
						}
						
						if(!canmove) MoveLeft();
					}
					
					//
					// ROTATE LEFT
					//
					
					else if(keys.PRESSED == keys.ROTATE_L || keys.PRESSED == 'q'){
						
						while(block.POS.Y - block.WIDTH + 1 < 0){ MoveDown(); }
						
						bool canrotate = true;
						
						RotateLeft();
						
						for(row = block.POS.Y; row > block.POS.Y - block.HEIGHT && canrotate; row--){
							for(column = block.POS.X; column < block.POS.X + block.WIDTH && canrotate; column++){
								if(board[row][column] && board2[row][column]){
									canrotate = false;
								}
							}
						}
						
						if(block.POS.X + block.WIDTH > screen.GAME.BRDSIZE.X) canrotate = false;
						
						if(!canrotate) RotateRight();
							
						/*while(block.POS.X < 1){ MoveRight(); }
						while(block.POS.X + block.WIDTH > screen.GAME.BRDSIZE.X - 1){ MoveLeft(); }*/
					}
					
					//
					// ROTATE RIGHT
					//
					
					else if(keys.PRESSED == keys.ROTATE_R || keys.PRESSED == 'e'){
						
						while(block.POS.Y - block.WIDTH + 1 < 0){ MoveDown(); }
						
						bool canrotate = true;
						
						RotateRight();
						
						for(row = block.POS.Y; row > block.POS.Y - block.HEIGHT && canrotate; row--){
							for(column = block.POS.X; column < block.POS.X + block.WIDTH && canrotate; column++){
								if(board[row][column] && board2[row][column]){
									canrotate = false;
								}
							}
						}
						
						if(block.POS.X + block.WIDTH > screen.GAME.BRDSIZE.X) canrotate = false;
						
						if(!canrotate) RotateLeft();
						
						/*while(block.POS.X < 1){ MoveRight(); }
						while(){ MoveLeft(); }*/
					}
					
					//
					// DROP
					//
					
					else if((keys.PRESSED == keys.DOWN || keys.PRESSED == 's') && !mods.HARDDROPONLY){ 
						CollisionCheck();
						if(!collided){
							MoveDown();
							i = 0;
						}
						else BoardWrite();
					}
					
					else if(keys.PRESSED == keys.DROP || keys.PRESSED == 32){
						
						while(!collided){
							CollisionCheck();
							if(!collided) MoveDown();
							Sleep(delay / 2);
						}
						BoardWrite();
						
						build++;
						
						score += level * BLOCKSCORE * mods.TIMEFACTOR;
					}
					
					else if(keys.PRESSED == ESC){
						
						bool mainmenu = false;
						
						page.GAME = false;
						
						mainmenu = Pause();
						qt_select = false;
						if(mainmenu) return;
						
						page.GAMELOADED = true;
						page.GAME = true;
					}
				}
				
				
						
			}
			
			if(!collided){
				CollisionCheck();
			
				if(collided || block.POS.Y >= screen.GAME.BRDSIZE.Y - 1){
					BoardWrite();
					score += BLOCKSCORE;
					
					build++;
				}
				
				else{
					if(!mods.HARDDROPONLY) MoveDown();
					else build++;
				}
			}
			
		}
		
	}
}


///
///		xxxxxxxxx     x     xxx     xxx xxxxxxxxx
///		x            x x    x  xx xx  x x
///		x    xxxx   xxxxx   x    x    x xxxxxxx
///		x       x  x     x  x         x x    
///		xxxxxxxxx x       x x         x xxxxxxxxx
///



void CurBlock(int sel){
	
	block.POS.X = screen.GAME.BRDSIZE.X / 2 - 2;
	
	if(sel == 1){
		// xxxx
		
		block.WIDTH = 4;
		block.HEIGHT = 1;
		block.POS.Y = 1;
		
		board2[1][block.POS.X] = true;
		board2[1][block.POS.X + 1] = true;
		board2[1][block.POS.X + 2] = true;
		board2[1][block.POS.X + 3] = true;
		
		//
		//	COLOR
		//
		
		board2_color[1][block.POS.X] = BLUE;
		board2_color[1][block.POS.X + 1] = BLUE;
		board2_color[1][block.POS.X + 2] = BLUE;
		board2_color[1][block.POS.X + 3] = BLUE;
		
		block.STATS.I++;
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
		
		//
		//	COLOR
		//
		
		board2_color[0][block.POS.X + 1] = GREEN;
		board2_color[1][block.POS.X] = GREEN;
		board2_color[1][block.POS.X + 1] = GREEN;
		board2_color[1][block.POS.X + 2] = GREEN;
		
		block.STATS.T++;
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
		
		//
		//	COLOR
		//
		
		board2_color[0][block.POS.X] = CYAN;
		board2_color[0][block.POS.X + 1] = CYAN;
		board2_color[1][block.POS.X] = CYAN;
		board2_color[1][block.POS.X + 1] = CYAN;
		
		block.STATS.O++;
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
		
		//
		//	COLOR
		//
		
		board2_color[0][block.POS.X] = RED;
		board2_color[1][block.POS.X] = RED;
		board2_color[1][block.POS.X + 1] = RED;
		board2_color[1][block.POS.X + 2] = RED;
		
		block.STATS.J++;
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
		
		//
		//	COLOR
		//
		
		board2_color[0][block.POS.X + 2] = MAGENTA;
		board2_color[1][block.POS.X] = MAGENTA;
		board2_color[1][block.POS.X + 1] = MAGENTA;
		board2_color[1][block.POS.X + 2] = MAGENTA;
		
		block.STATS.L++;
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
		
		//
		//	COLOR
		//
		
		board2_color[0][block.POS.X] = LIGHTBLUE;
		board2_color[0][block.POS.X + 1] = LIGHTBLUE;
		board2_color[1][block.POS.X + 1] = LIGHTBLUE;
		board2_color[1][block.POS.X + 2] = LIGHTBLUE;
		
		block.STATS.Z++;
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
		
		//
		//	COLOR
		//
		
		board2_color[0][block.POS.X + 1] = LIGHTGREEN;
		board2_color[0][block.POS.X + 2] = LIGHTGREEN;
		board2_color[1][block.POS.X] = LIGHTGREEN;
		board2_color[1][block.POS.X + 1] = LIGHTGREEN;
		
		block.STATS.S++;
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
			
			//
			//	COLOR MOVE
			//
			
			board2_color[block.POS.Y - column][block.POS.X + row - 1] = board2_color[block.POS.Y - column][block.POS.X + row];
			board2_color[block.POS.Y - column][block.POS.X + row] = DARKGRAY;
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
			
			//
			//	COLOR MOVE
			//
			
			board2_color[block.POS.Y - column][block.POS.X - row + block.WIDTH] = board2_color[block.POS.Y - column][block.POS.X - row + block.WIDTH - 1]; 
			board2_color[block.POS.Y - column][block.POS.X - row + block.WIDTH - 1] = DARKGRAY;
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
	unsigned char mezipole2[block.WIDTH][block.HEIGHT] = {0};
	
	// BLOCK ROTATE
						
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			mezipole[block.WIDTH - 1 - column][row] = board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column];
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = false;
			
			mezipole2[block.WIDTH - 1 - column][row] = board2_color[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column];
			board2_color[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = DARKGRAY;
		}
	}
						
	temp = block.HEIGHT;
	block.HEIGHT = block.WIDTH;
	block.WIDTH = temp;
	
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = mezipole[row][column];
			
			board2_color[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = mezipole2[row][column];
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
	unsigned char mezipole2[block.WIDTH][block.HEIGHT] = {0};
						
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			mezipole[column][block.HEIGHT - 1 - row] = board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column];
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = false;
			
			mezipole2[column][block.HEIGHT - 1 - row] = board2_color[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column];
			board2_color[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = DARKGRAY;
		}
	}
						
	temp = block.HEIGHT;
	block.HEIGHT = block.WIDTH;
	block.WIDTH = temp;
	
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			board2[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = mezipole[row][column];
			
			board2_color[block.POS.Y - block.HEIGHT + 1 + row][block.POS.X + column] = mezipole2[row][column];
		}
	}
}

void CollisionCheck(){
	
	if(block.POS.Y >= screen.GAME.BRDSIZE.Y - 1){
		collided = true;
		return;
	}
	
	for(row = 0; row < block.HEIGHT && !collided; row++){
		for(column = 0; column < block.WIDTH && !collided; column++){
			if(board2[block.POS.Y - row][block.POS.X + column] && board[block.POS.Y - row + 1][block.POS.X + column]){
				collided = true;
				return;
			}
		}
	}
}

void MoveDown(){
	block.POS.Y++;
				
	for(row = 0; row < block.HEIGHT + 1; row++){
		for(column = 0; column < block.WIDTH; column++){
			board2[block.POS.Y - row][block.POS.X + column] = board2[block.POS.Y - row - 1][block.POS.X + column];
			board2[block.POS.Y - row - 1][block.POS.X + column] = false;
			
			board2_color[block.POS.Y - row][block.POS.X + column] = board2_color[block.POS.Y - row - 1][block.POS.X + column];
			board2_color[block.POS.Y - row - 1][block.POS.X + column] = DARKGRAY;
		}
	}
}

void BoardWrite(){
	for(row = 0; row < block.HEIGHT; row++){
		for(column = 0; column < block.WIDTH; column++){
			if(board2[block.POS.Y - row][block.POS.X + column]){
				board[block.POS.Y - row][block.POS.X + column] = board2[block.POS.Y - row][block.POS.X + column];
				board_color[block.POS.Y - row][block.POS.X + column] = board2_color[block.POS.Y - row][block.POS.X + column];
			}
			
			board2[block.POS.Y - row][block.POS.X + column] = false;
			board2_color[block.POS.Y - row][block.POS.X + column] = DARKGRAY;
		}
	}
	
	if (!mods.CHAINREACTION) FullRowDetection();
	else if(mods.CHAINREACTION) FullRowDetectionChain();
}

void FullRowDetection(){
	
	int rowscleared = 0;
	
	for(row = 0; row <= block.POS.Y; row++){
		
		int get = 0;
		
		for(int i = 1; i < screen.GAME.BRDSIZE.X - 1; i++){
			if(board[row][i]) get++;
		}
		
		if(get == screen.GAME.BRDSIZE.X - 2){
			for(column = 0; column < screen.GAME.BRDSIZE.X; column++) board[row][column] = false;
			
			rowscleared++;
			
			for(int i = row; i > 0; i--){
				for(int j = 0; j < screen.GAME.BRDSIZE.X; j++){
					board[i][j] = board[i - 1][j];
					board[i - 1][j] = false;
					
					board_color[i][j] = board_color[i - 1][j];
					board_color[i - 1][j] = DARKGRAY;
				}
			}
		}
	}
	
	if(rowscleared != 0){
		
		score += (rowscleared == 1 ? 40 : (rowscleared == 2 ? 100 : (rowscleared == 3 ? 300 : 1200))) * level * mods.TIMEFACTOR * BoardScoreRatio(mods.BOARDSIZE);
		
		thislevelclear -= rowscleared;
		
		if(thislevelclear <= 0){
			level++;
			thislevelclear += BASECLEAR + level;
		}
		
		rowscleared = 0;
	}
}

void FullRowDetectionChain(){
	
	bool done = true;
	int rowscleared = 0;
	
	do{
		bool doit = false;
		done = true;
		
		for(row = 0; row < screen.GAME.BRDSIZE.Y; row++){
			
			int get = 0;
		
			for(int i = 1; i < screen.GAME.BRDSIZE.X - 1; i++){
				if(board[row][i]) get++;
			}
			
			if(get == screen.GAME.BRDSIZE.X - 2){
				for(column = 0; column < screen.GAME.BRDSIZE.X; column++) board[row][column] = false;
				
				rowscleared++;
				
				doit = true;
				
				for(int i = row; i > 0; i--){
					for(int j = 0; j < screen.GAME.BRDSIZE.X; j++){
						board[i][j] = board[i - 1][j];
						board[i - 1][j] = false;
						
						board_color[i][j] = board_color[i - 1][j];
						board_color[i - 1][j] = DARKGRAY;
					}
				}
				
				
			}
		}
		
		
		if(doit){
			for(row = screen.GAME.BRDSIZE.Y - 1 - 1; row >= 0; row--){
				for(column = 0; column < screen.GAME.BRDSIZE.X - 2; column++){
					while(board[row][column + 1] && !board[row + 1][column + 1] && row < screen.GAME.BRDSIZE.Y){
						board[row + 1][column + 1] = board[row][column + 1];
						board[row][column + 1] = false;
						
						board_color[row + 1][column + 1] = board_color[row][column + 1];
						board_color[row][column + 1] = DARKGRAY;
						
						row++;
					}
				}
				Sleep(10);
			}
		}
		
		
		
		
		for(row = 0; row < screen.GAME.BRDSIZE.Y - 1; row++){
			
			int get = 0;
		
			for(int i = 1; i < screen.GAME.BRDSIZE.X - 1; i++){
				if(board[row][i]) get++;
			}
			
			if(get == screen.GAME.BRDSIZE.X - 2){
				done = false;
			}
		}
	}while(!done);
	
	
	if(rowscleared != 0){
		
		score += (rowscleared == 1 ? 40 : (rowscleared == 2 ? 100 : (rowscleared == 3 ? 300 : 1200))) * level * mods.TIMEFACTOR * BoardScoreRatio(mods.BOARDSIZE);
		
		thislevelclear -= rowscleared;
		
		if(thislevelclear <= 0){
			level++;
			thislevelclear += BASECLEAR + level;
		}
		
		rowscleared = 0;
	}
	
}


///		
///		x       x xxxxxxxxxx x        xxxxxxxxx
///		x       x x          x        x        x
///		xxxxxxxxx xxxxxxx    x        xxxxxxxxxx
///		x       x x          x        x
///		x       x xxxxxxxxxx xxxxxxxx x
///


void CurBlockHelp(int sel, int Y){
	
	blockhelp.POS.X = SMALL_H / 2 - 2;
	
	if(sel == 1){
		// xxxx
		
		blockhelp.WIDTH = 4;
		blockhelp.HEIGHT = 1;
		blockhelp.POS.Y = Y;
		
		help2[blockhelp.POS.Y][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 2] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 3] = true;
		
		//
		//	COLOR
		//
		
		help2_color[blockhelp.POS.Y][blockhelp.POS.X] = BLUE;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 1] = BLUE;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 2] = BLUE;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 3] = BLUE;
		
		blockhelp.STATS.I++;
	}
	
	else if(sel == 2){
		//  x
		// xxx
		
		blockhelp.WIDTH = 3;
		blockhelp.HEIGHT = 2;
		blockhelp.POS.Y = Y;
		
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 2] = true;
		
		//
		//	COLOR
		//
		
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = GREEN;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X] = GREEN;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 1] = GREEN;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 2] = GREEN;
		
		blockhelp.STATS.T++;
	}
	
	else if(sel == 3){
		// xx
		// xx
		
		blockhelp.WIDTH = 2;
		blockhelp.HEIGHT = 2;
		blockhelp.POS.Y = Y;
		
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 1] = true;
		
		//
		//	COLOR
		//
		
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X] = CYAN;
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = CYAN;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X] = CYAN;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 1] = CYAN;
		
		blockhelp.STATS.O++;
	}
	
	else if(sel == 4){
		// x
		// xxx
		
		blockhelp.WIDTH = 3;
		blockhelp.HEIGHT = 2;
		blockhelp.POS.Y = Y;
		
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 2] = true;
		
		//
		//	COLOR
		//
		
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X] = RED;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X] = RED;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 1] = RED;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 2] = RED;
		
		blockhelp.STATS.J++;
	}
	
	else if(sel == 5){
		//   x
		// xxx
		
		blockhelp.WIDTH = 3;
		blockhelp.HEIGHT = 2;
		blockhelp.POS.Y = Y;
		
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X + 2] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 2] = true;
		
		//
		//	COLOR
		//
		
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X + 2] = MAGENTA;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X] = MAGENTA;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 1] = MAGENTA;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 2] = MAGENTA;
		
		blockhelp.STATS.L++;
	}
	
	else if(sel == 6){
		// xx
		//  xx
		
		blockhelp.WIDTH = 3;
		blockhelp.HEIGHT = 2;
		blockhelp.POS.Y = Y;
		
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 2] = true;
		
		//
		//	COLOR
		//
		
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X] = LIGHTBLUE;
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = LIGHTBLUE;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 1] = LIGHTBLUE;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 2] = LIGHTBLUE;
		
		blockhelp.STATS.Z++;
	}
	
	else if(sel == 7){
		//  xx
		// xx
		
		blockhelp.WIDTH = 3;
		blockhelp.HEIGHT = 2;
		blockhelp.POS.Y = Y;
		
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = true;
		help2[blockhelp.POS.Y - 1][blockhelp.POS.X + 2] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X] = true;
		help2[blockhelp.POS.Y][blockhelp.POS.X + 1] = true;
		
		//
		//	COLOR
		//
		
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X + 1] = LIGHTGREEN;
		help2_color[blockhelp.POS.Y - 1][blockhelp.POS.X + 2] = LIGHTGREEN;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X] = LIGHTGREEN;
		help2_color[blockhelp.POS.Y][blockhelp.POS.X + 1] = LIGHTGREEN;
		
		blockhelp.STATS.S++;
	}
}

void MoveLeftHelp(){
	
	if(blockhelp.POS.X <= 0) return;
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			if(help[row][column] && help2[row][column]) return;
		}
	}
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			help2[blockhelp.POS.Y - column][blockhelp.POS.X + row - 1] = help2[blockhelp.POS.Y - column][blockhelp.POS.X + row];
			help2[blockhelp.POS.Y - column][blockhelp.POS.X + row] = false;
			
			//
			//	COLOR MOVE
			//
			
			help2_color[blockhelp.POS.Y - column][blockhelp.POS.X + row - 1] = help2_color[blockhelp.POS.Y - column][blockhelp.POS.X + row];
			help2_color[blockhelp.POS.Y - column][blockhelp.POS.X + row] = DARKGRAY;
		}
	}
	
	blockhelp.POS.X--;
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			if(help[blockhelp.POS.Y - row][blockhelp.POS.X + column] && help2[blockhelp.POS.Y - row][blockhelp.POS.X + column]){
				MoveRightHelp();
				return;
			}
		}
	}
}

	//
	// MOVE TO RIGHT
	//

void MoveRightHelp(){
	
	if(blockhelp.POS.X + blockhelp.WIDTH - 1 >= 17) return;
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			if(help[row][column] && help2[row][column]) return;
		}
	}
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			help2[blockhelp.POS.Y - column][blockhelp.POS.X - row + blockhelp.WIDTH] = help2[blockhelp.POS.Y - column][blockhelp.POS.X - row + blockhelp.WIDTH - 1]; 
			help2[blockhelp.POS.Y - column][blockhelp.POS.X - row + blockhelp.WIDTH - 1] = false;
			
			//
			//	COLOR MOVE
			//
			
			help2_color[blockhelp.POS.Y - column][blockhelp.POS.X - row + blockhelp.WIDTH] = help2_color[blockhelp.POS.Y - column][blockhelp.POS.X - row + blockhelp.WIDTH - 1]; 
			help2_color[blockhelp.POS.Y - column][blockhelp.POS.X - row + blockhelp.WIDTH - 1] = DARKGRAY;
		}
	}
	
	blockhelp.POS.X++;
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			if(help[blockhelp.POS.Y - row][blockhelp.POS.X + column] && help2[blockhelp.POS.Y - row][blockhelp.POS.X + column]){
				MoveLeftHelp();
				return;
			}
		}
	}
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

void RotateLeftHelp(){
	bool mezipole[blockhelp.WIDTH][blockhelp.HEIGHT] = {false};
	unsigned char mezipole2[blockhelp.WIDTH][blockhelp.HEIGHT] = {0};
	
	if(blockhelp.POS.X + blockhelp.WIDTH - 1 > 17) return;
	
	// BLOCK ROTATE
						
	for(row = 0; row < blockhelp.HEIGHT; row++){
		for(column = 0; column < blockhelp.WIDTH; column++){
			mezipole[blockhelp.WIDTH - 1 - column][row] = help2[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column];
			help2[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = false;
			
			mezipole2[blockhelp.WIDTH - 1 - column][row] = help2_color[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column];
			help2_color[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = DARKGRAY;
		}
	}
						
	temp = blockhelp.HEIGHT;
	blockhelp.HEIGHT = blockhelp.WIDTH;
	blockhelp.WIDTH = temp;
	
	for(row = 0; row < blockhelp.HEIGHT; row++){
		for(column = 0; column < blockhelp.WIDTH; column++){
			help2[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = mezipole[row][column];
			
			help2_color[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = mezipole2[row][column];
		}
	}
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			if(help[blockhelp.POS.Y - row][blockhelp.POS.X + column] && help2[blockhelp.POS.Y - row][blockhelp.POS.X + column]){
				RotateRightHelp();
				return;
			}
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

void RotateRightHelp(){
	bool mezipole[blockhelp.WIDTH][blockhelp.HEIGHT] = {false};
	unsigned char mezipole2[blockhelp.WIDTH][blockhelp.HEIGHT] = {0};
	
	if(blockhelp.POS.X + blockhelp.WIDTH - 1 > 17) return;
	
	for(row = 0; row < blockhelp.HEIGHT; row++){
		for(column = 0; column < blockhelp.WIDTH; column++){
			mezipole[column][blockhelp.HEIGHT - 1 - row] = help2[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column];
			help2[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = false;
			
			mezipole2[column][blockhelp.HEIGHT - 1 - row] = help2_color[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column];
			help2_color[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = DARKGRAY;
		}
	}
						
	temp = blockhelp.HEIGHT;
	blockhelp.HEIGHT = blockhelp.WIDTH;
	blockhelp.WIDTH = temp;
	
	for(row = 0; row < blockhelp.HEIGHT; row++){
		for(column = 0; column < blockhelp.WIDTH; column++){
			help2[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = mezipole[row][column];
			
			help2_color[blockhelp.POS.Y - blockhelp.HEIGHT + 1 + row][blockhelp.POS.X + column] = mezipole2[row][column];
		}
	}
	
	
	for(row = 0; row < blockhelp.WIDTH; row++){
		for(column = 0; column < blockhelp.HEIGHT; column++){
			if(help[blockhelp.POS.Y - row][blockhelp.POS.X + column] && help2[blockhelp.POS.Y - row][blockhelp.POS.X + column]){
				RotateLeftHelp();
				return;
			}
		}
	}
}

void CollisionCheckHelp(){
	
	if(blockhelp.POS.Y >= SMALL_H - 1){
		collidedhelp = true;
		BoardWriteHelp();
		return;
	}
	
	for(row = 0; row < blockhelp.HEIGHT && !collidedhelp; row++){
		for(column = 0; column < blockhelp.WIDTH && !collidedhelp; column++){
			if(help2[blockhelp.POS.Y - row][blockhelp.POS.X + column] && help[blockhelp.POS.Y - row + 1][blockhelp.POS.X + column]){
				collidedhelp = true;
				BoardWriteHelp();
				return;
			}
		}
	}
}

void MoveDownHelp(){
	blockhelp.POS.Y++;
				
	for(row = 0; row < blockhelp.HEIGHT + 1; row++){
		for(column = 0; column < blockhelp.WIDTH; column++){
			help2[blockhelp.POS.Y - row][blockhelp.POS.X + column] = help2[blockhelp.POS.Y - row - 1][blockhelp.POS.X + column];
			help2[blockhelp.POS.Y - row - 1][blockhelp.POS.X + column] = false;
			
			help2_color[blockhelp.POS.Y - row][blockhelp.POS.X + column] = help2_color[blockhelp.POS.Y - row - 1][blockhelp.POS.X + column];
			help2_color[blockhelp.POS.Y - row - 1][blockhelp.POS.X + column] = DARKGRAY;
		}
	}
}

void BoardWriteHelp(){
	for(row = 0; row < blockhelp.HEIGHT; row++){
		for(column = 0; column < blockhelp.WIDTH; column++){
			if(help2[blockhelp.POS.Y - row][blockhelp.POS.X + column]){
				help[blockhelp.POS.Y - row][blockhelp.POS.X + column] = help2[blockhelp.POS.Y - row][blockhelp.POS.X + column];
				help_color[blockhelp.POS.Y - row][blockhelp.POS.X + column] = help2_color[blockhelp.POS.Y - row][blockhelp.POS.X + column];
			}
			
			help2[blockhelp.POS.Y - row][blockhelp.POS.X + column] = false;
			help2_color[blockhelp.POS.Y - row][blockhelp.POS.X + column] = DARKGRAY;
		}
	}
	
	if (!mods.CHAINREACTION) FullRowDetectionHelp();
	else if(mods.CHAINREACTION) FullRowDetectionChainHelp();
}

void FullRowDetectionHelp(){
	
	int rowscleared = 0;
	
	for(row = 0; row <= blockhelp.POS.Y; row++){
		
		int get = 0;
		
		for(int i = 1; i < screen.GAME.BRDSIZE.X - 1; i++){
			if(help[row][i]) get++;
		}
		
		if(get == screen.GAME.BRDSIZE.X - 2){
			for(column = 0; column < screen.GAME.BRDSIZE.X; column++) help[row][column] = false;
			
			rowscleared++;
			
			for(int i = row; i > 0; i--){
				for(int j = 0; j < screen.GAME.BRDSIZE.X; j++){
					help[i][j] = help[i - 1][j];
					help[i - 1][j] = false;
					
					help_color[i][j] = help_color[i - 1][j];
					help_color[i - 1][j] = DARKGRAY;
				}
			}
		}
	}
}

void FullRowDetectionChainHelp(){
	
	bool done = true;
	int rowscleared = 0;
	
	do{
		bool doit = false;
		done = true;
		
		for(row = 0; row < screen.GAME.BRDSIZE.Y; row++){
			
			int get = 0;
		
			for(int i = 1; i < screen.GAME.BRDSIZE.X - 1; i++){
				if(help[row][i]) get++;
			}
			
			if(get == screen.GAME.BRDSIZE.X - 2){
				for(column = 0; column < screen.GAME.BRDSIZE.X; column++) help[row][column] = false;
				
				rowscleared++;
				
				doit = true;
				
				for(int i = row; i > 0; i--){
					for(int j = 0; j < screen.GAME.BRDSIZE.X; j++){
						help[i][j] = help[i - 1][j];
						help[i - 1][j] = false;
						
						help_color[i][j] = help_color[i - 1][j];
						help_color[i - 1][j] = DARKGRAY;
					}
				}
				
				
			}
		}
		
		
		if(doit){
			for(row = screen.GAME.BRDSIZE.Y - 1 - 1; row >= 0; row--){
				for(column = 0; column < screen.GAME.BRDSIZE.X - 2; column++){
					while(help[row][column + 1] && !help[row + 1][column + 1] && row < screen.GAME.BRDSIZE.Y){
						help[row + 1][column + 1] = help[row][column + 1];
						help[row][column + 1] = false;
						
						help_color[row + 1][column + 1] = help_color[row][column + 1];
						help_color[row][column + 1] = DARKGRAY;
						
						row++;
					}
				}
				Sleep(10);
			}
		}
		
		
		
		
		for(row = 0; row < screen.GAME.BRDSIZE.Y - 1; row++){
			
			int get = 0;
		
			for(int i = 1; i < screen.GAME.BRDSIZE.X - 1; i++){
				if(help[row][i]) get++;
			}
			
			if(get == screen.GAME.BRDSIZE.X - 2){
				done = false;
			}
		}
	} while(!done);
}


int Digits(int n){
	if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    /*      2147483647 is 2^31-1 - add more ifs as needed
       and adjust this final return as well. */
    return 10;
}

float BoardScoreRatio(int size){
	switch(size){
		case 0:
			return SMALLSCORE;
			
		case 1:
			return NORMALSCORE;
			
		case 2:
			return LARGESCORE;
			
		default:
			return 0;
	}
}

void OnLoad(){
	srand(time(NULL));
	
	HANDLE thread1 = CreateThread(NULL, 0, WindowResizedEvent, NULL, 0, NULL);
	HANDLE thread2 = CreateThread(NULL, 0, RenderScreen, NULL, 0, NULL);
	
	if(sound) HANDLE thread3 = CreateThread(NULL, 0, MusicPlay, NULL, 0, NULL);
	
	setcursortype(_NOCURSOR);
	SetConsoleTitle(name);
	SetWindow(80,26);
	SetFont(12, 16);
	
	screen.MAINMENU.Y = 17;
	screen.SETTINGS.Y = 13;
	screen.INFO.Y = 13;
	screen.GAME.BOARD.Y = 2;
	screen.GAME.BOARD.X = 10;
	screen.GAME.CURBLOCK.Y = 3;
	screen.GAME.CURBLOCK.X = 55;
	screen.GAME.PAUSE.Y = 16;
	screen.GAME.PAUSE.X = 55;
	screen.EXIT.Y = 10;
	screen.GAME.GAMETYPE.Y = 14;
	screen.GAME.MUTATOR.Y = 10;
	
	screen.EXITSIZE.X = 80;
	screen.EXITSIZE.Y = 26;
	screen.GAMESIZE.X = 80;
	screen.GAMESIZE.Y = 34;
	screen.INFOSIZE.X = 80;
	screen.INFOSIZE.Y = 30;
	screen.MMSIZE.X = 80;
	screen.MMSIZE.Y = 26;
	screen.SETTSIZE.X = 80;
	screen.SETTSIZE.Y = 26;
	screen.KBINDSIZE.X = 80;
	screen.KBINDSIZE.Y = 35;
	
	
	selection.EXIT = 0;
	selection.INFO = 0;
	selection.KEYS = 9;
	selection.MAINMENU = 0;
	selection.PAUSE = 0;
	selection.SETTINGS = 0;
	selection.GAMETYPE = 1;
	
	screen.TETRIS = 3;
	
	page.MAINMENU = true;
	
	keys.UP = 'w';
	keys.DOWN = 's';
	keys.LEFT = 'a';
	keys.RIGHT = 'd';
	keys.ROTATE_L = 'q';
	keys.ROTATE_R = 'e';
	keys.NEXTSONG = 'n';
	keys.DROP = SPACE_K;
	keys.PAUSE = ESC;
	
	keys.PRESSED = NULL;
}

void Options(){
	
	page.SETTINGS = true;
	
	last_width = screen.SETTSIZE.X;
	last_height = screen.SETTSIZE.Y;
	
	
	while(true){
		
		GetKey();
		
		if((keys.PRESSED == keys.UP || keys.PRESSED == 'w') && selection.SETTINGS > 0){
			selection.SETTINGS--;
		}
		
		else if((keys.PRESSED == keys.DOWN || keys.PRESSED == 's') && selection.SETTINGS < 4){
			selection.SETTINGS++;
		}
		
		else if(keys.PRESSED == 13){
			
			

			if(selection.SETTINGS == 0){
				
				last_width = screen.KBINDSIZE.X;
				last_height = screen.KBINDSIZE.Y;
				
				page.KEYS = true;
				
				clrscr();
				
				while(page.KEYS){
					
					GetKey();
					
					if((keys.PRESSED == 'w' || keys.PRESSED == keys.UP) && selection.KEYS > 0){
						selection.KEYS--;
					}
					
					else if((keys.PRESSED == 's' || keys.PRESSED == keys.DOWN) && selection.KEYS < 9){
						selection.KEYS++;
					}
					
					else if(keys.PRESSED == 13){
						if(selection.KEYS != 9){
							
							GetKey();
							
							keys.SAMEKEYEXCEPTION = false;
							
							if(keys.PRESSED != DEL && !KeyBindingChecker()){
								if(selection.KEYS == 0) keys.UP = keys.PRESSED;
								else if(selection.KEYS == 1) keys.DOWN = keys.PRESSED;
								else if(selection.KEYS == 2) keys.LEFT = keys.PRESSED;
								else if(selection.KEYS == 3) keys.RIGHT = keys.PRESSED;
								else if(selection.KEYS == 4) keys.ROTATE_L = keys.PRESSED;
								else if(selection.KEYS == 5) keys.ROTATE_R = keys.PRESSED;
								else if(selection.KEYS == 6) keys.DROP = keys.PRESSED;
								else if(selection.KEYS == 7) keys.PAUSE = keys.PRESSED;
								else if(selection.KEYS == 8) keys.NEXTSONG = keys.PRESSED;
							}
							else if(KeyBindingChecker()){
								keys.SAMEKEYEXCEPTION = true;
							}
							
						}
						
						else if(selection.KEYS == 9){
							
							page.KEYS = false;
							
						}
					}
				}
			}
			
			else if(selection.SETTINGS == 1){
				sound = !sound;
				
				if(sound) HANDLE thread3 = CreateThread(NULL, 0, MusicPlay, NULL, 0, NULL);
			}
			else if(selection.SETTINGS == 2){
				
			}
			else if(selection.SETTINGS == 3){
				Defaults();
			}
			else if(selection.SETTINGS == 4){
				page.SETTINGS = false;
				return;
			}
			
			last_width = screen.SETTSIZE.X;
			last_height = screen.SETTSIZE.Y;
		}
	}
}

void Info(){
	
	page.INFO = true;
	
	last_width = screen.INFOSIZE.X;
	last_height = screen.INFOSIZE.Y;
	
	int timer = 0, time = 0;
	
	while(true){
		
		GetKey();
	
		if((keys.PRESSED == keys.UP || keys.PRESSED == 'w') && selection.INFO > 0){
			selection.INFO--;
		}
		
		else if((keys.PRESSED == keys.DOWN || keys.PRESSED == 's') && selection.INFO < 2){
			selection.INFO++;
		}
		
		else if(keys.PRESSED == 13){
			if(selection.INFO == 0){
				
				page.HELP = true;
				
				while(page.HELP){
					
					if(timer == 0){
						memset(help, false, sizeof(board[0][0]) * SMALL_H * SMALL_H);
						memset(help2, false, sizeof(board2[0][0]) * SMALL_H * SMALL_H);
						
						memset(help_color, DARKGRAY, sizeof(board_color[0][0]) * SMALL_H * SMALL_H);
						memset(help2_color, DARKGRAY, sizeof(board2_color[0][0]) * SMALL_H * SMALL_H);
					}
					
					if(selection.HELPPAGE == 0 || selection.HELPPAGE == 1){
						if(timer == 0){
							CurBlockHelp(rand() % 7 + 1, 6);
						}
						
						else if(timer % 100 == 20){ RotateLeftHelp(); }
						else if(timer % 100 == 40){ MoveLeftHelp(); }
						else if(timer % 100 == 60){ RotateRightHelp(); }
						else if(timer % 100 == 80){ MoveRightHelp(); }
					}
					
					else if(selection.HELPPAGE == 2 || selection.HELPPAGE == 3 || selection.HELPPAGE == 4){
						
						if(timer == 0){
							bool brd_preset[][SMALL_W] = {
								{ TR, FL, TR, FL, FL, FL, FL, FL, FL, FL, TR, FL, FL, TR },
								{ TR, TR, TR, FL, FL, TR, TR, TR, FL, FL, TR, FL, TR, TR },
								{ TR, TR, TR, TR, TR, TR, TR, TR, FL, FL, FL, FL, TR, FL },
								{ TR, FL ,FL ,FL ,TR, TR, TR, FL, FL, FL, TR, TR, TR, FL },
								{ TR, TR, TR, TR, TR, TR, TR, TR, FL, TR, TR, TR, TR, TR }
							};
							
							for(row = 0; row < 5; row++){
								for(column = 0; column < SMALL_W; column++){
									
									help[13 + row][column] = brd_preset[row][column];
									
									if(brd_preset[row][column]){
										int blockis = rand() % 16;
										while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis = rand() % 16;
										
										help_color[13 + row][column] = blockis;
									}
								}
							}
							
							CurBlockHelp(I + 1, 2);
						}
						
						else if(timer == 20){ MoveRightHelp(); }
						else if(timer == 25){ MoveDownHelp(); }
						else if(timer == 30){ MoveDownHelp(); }
						else if(timer == 40){ RotateLeftHelp(); }
						else if(timer == 45){
							
						}
						
					}
					
					timer++;
					
					if(_kbhit()){
						
						GetKey();
						
						if((keys.PRESSED == 'w' || keys.PRESSED == keys.UP) && selection.HELP > 0){
							selection.HELP--;
						}
						
						else if((keys.PRESSED == 's' || keys.PRESSED == keys.DOWN) && selection.HELP < 1){
							selection.HELP++;
						}
						
						else if((keys.PRESSED == 'a' || keys.PRESSED == keys.LEFT) && selection.HELP == 0){
							selection.HELPPAGE--;
							if(selection.HELPPAGE < 0) selection.HELPPAGE = 7;
							timer = 0;
						}
						
						else if((keys.PRESSED == 'd' || keys.PRESSED == keys.RIGHT) && selection.HELP == 0){
							selection.HELPPAGE++;
							if(selection.HELPPAGE > 7) selection.HELPPAGE = 0;
							timer = 0;
						}
						
						else if(selection.HELP == 1 && keys.PRESSED == 13){
							page.HELP = false;
						}
					}
					
					Sleep(50);
				}
			}
			
			else if(selection.INFO == 1){
				ShellExecute(NULL, "open", "https://www.youtube.com/playlist?list=PL2b_6zoPWVIqE_Xozc4-mSrq75fQu56pw", NULL, NULL, SW_SHOWNORMAL);
			}
			
			else if(selection.INFO == 2){
				page.INFO = false;
				return;
			}
			
		}
		
		
		
	}
	
}

bool Quit(){
	
	page.EXIT = true;
	
	last_width = screen.EXITSIZE.X;
	last_height = screen.EXITSIZE.Y;
	
	while(true){
		
		GetKey();
		
		if(keys.PRESSED == keys.LEFT || keys.PRESSED == 'a'){
			exit_app = true;
		}
		
		if(keys.PRESSED == keys.RIGHT || keys.PRESSED == 'd'){
			exit_app = false;
		}
		
		if(keys.PRESSED == 13){
			page.EXIT = false;
			
			return exit_app;
		}
	}
		
}

bool Pause(){
	
	page.PAUSE = true;
	page.GAMELOADED = true;
	
	while(true){
		
		GetKey();
		
		if((keys.PRESSED == 'w' || keys.PRESSED == keys.UP) && selection.PAUSE > 0){
			selection.PAUSE--;
		}
		
		else if((keys.PRESSED == 's' || keys.PRESSED == keys.DOWN) && selection.PAUSE < 4){
			selection.PAUSE++;
		}
		
		else if(keys.PRESSED == 13){
			
			if(selection.PAUSE == 0){
				page.PAUSE = false;
				return false;
			} 
			//else if(selection == 1) 
			else if(selection.PAUSE == 2){ page.PAUSE = false; Options(); }
			else if(selection.PAUSE == 3){ page.PAUSE = false; Info(); } 
			else if(selection.PAUSE == 4) {
				
				page.PAUSERETURN = true;
				
				while(page.PAUSERETURN){

					keys.PRESSED = _getch();
					
					if(keys.PRESSED == 0 || keys.PRESSED == 224){
						keys.PRESSED += _getch();
					}
					
					if(keys.PRESSED == 'a' || keys.PRESSED == keys.LEFT) qt_select = true;
					else if(keys.PRESSED == 'd' || keys.PRESSED == keys.RIGHT) qt_select = false;
					
					else if(keys.PRESSED == 13){
						
						page.PAUSERETURN = false;
						
						if(qt_select){
							page.PAUSE = false;
							
							return qt_select;
						}
					}
				}
				
			}
			
			page.PAUSE = true;
			page.GAMELOADED = true;
			
			last_width = screen.GAMESIZE.X;
			last_height = screen.GAMESIZE.Y;
			
			
		}
	}
	
}

void GetKey(){
	keys.PRESSED = _getch();
		
	if(keys.PRESSED == 224){
		keys.PRESSED += _getch();
	}
	
	if(keys.PRESSED == 0){
		keys.PRESSED -= _getch();
	}
	
	while(_kbhit()) _getch();
}

bool KeyBindingChecker(){
	if(keys.PRESSED == keys.DOWN || keys.PRESSED == keys.DROP || keys.PRESSED == keys.LEFT || keys.PRESSED == keys.NEXTSONG || keys.PRESSED == keys.PAUSE || keys.PRESSED == keys.RIGHT || keys.PRESSED == keys.ROTATE_L || keys.PRESSED == keys.ROTATE_R || keys.PRESSED == keys.UP || keys.PRESSED == 'w' || keys.PRESSED == 's' || keys.PRESSED == 'a' || keys.PRESSED == 'd' || keys.PRESSED == 'q' || keys.PRESSED == 'e' || keys.PRESSED == SPACE_K || keys.PRESSED == ESC || keys.PRESSED == 'n'){
		
		if(selection.KEYS == 0 && keys.PRESSED == 'w') return false;
		else if(selection.KEYS == 1 && keys.PRESSED == 's') return false;
		else if(selection.KEYS == 2 && keys.PRESSED == 'a') return false;
		else if(selection.KEYS == 3 && keys.PRESSED == 'd') return false;
		else if(selection.KEYS == 4 && keys.PRESSED == 'q') return false;
		else if(selection.KEYS == 5 && keys.PRESSED == 'e') return false;
		else if(selection.KEYS == 6 && keys.PRESSED == SPACE_K) return false;
		else if(selection.KEYS == 7 && keys.PRESSED == ESC) return false;
		else if(selection.KEYS == 8 && keys.PRESSED == 'n') return false;
		
		return true;
	}
	
	return false;
}



char * Keys(int key){
	
	char * txt = (char *) malloc(sizeof(char) * 6);
	
	if(key == UP_K) strcpy(txt, "\x18    ");
	else if(key == DOWN_K) strcpy(txt, "\x19    ");
	else if(key == LEFT_K) strcpy(txt, "\x1B    ");
	else if(key == RIGHT_K) strcpy(txt, "\x1A    ");
	else if(key == ESC) strcpy(txt, "ESC  ");
	else if(key == SPACE_K) strcpy(txt, "space");
	else if(key == F1) strcpy(txt, "F1   ");
	else if(key == F2) strcpy(txt, "F2   ");
	else if(key == F3) strcpy(txt, "F3   ");
	else if(key == F4) strcpy(txt, "F4   ");
	else if(key == F5) strcpy(txt, "F5   ");
	else if(key == F6) strcpy(txt, "F6   ");
	else if(key == F7) strcpy(txt, "F7   ");
	else if(key == F8) strcpy(txt, "F8   ");
	else if(key == F9) strcpy(txt, "F9   ");
	else if(key == F10) strcpy(txt, "F10  ");
	else if(key == F11) strcpy(txt, "F11  ");
	else if(key == F12) strcpy(txt, "F12  ");
	
	else{
		txt[0] = key;
		txt[1] = ' ';
		txt[2] = ' ';
		txt[3] = ' ';
		txt[4] = ' ';
		txt[5] = '\0';
	}
	
	return txt;
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
				else if(row == 1) fputs("\xDB      \xDB", stdout);
				else if(row == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row == 3 || row == 4) fputs("\xDB       ", stdout);
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
				else if(row == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
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

void setcursortype(int type){
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
		
		Sleep(5);
	}
	
	
}

void LoadingScreenAnim(){
	
	Sleep(100);
	
	for(int x = 0; x < 6; x++){
		
		Mid("|", 60, 20, WHITE);
		Mid("|", 19, 20, WHITE);
		if(x != 0) Mid("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2", 20 + (x - 1) * 8 , 20, DARKGRAY);
		
		
		crmove(38, 22);
		textcolor(WHITE);
		printf("%3d%%", x * 20);
		
		for(int i = -1; i < 6; i++){
			EpicText("tetris",10, YELLOW);
			for(int j = 0; j < 5; j++){
				if(j != i)Mid("                                                     ", (80 - strlen("                                                     ")) / 2, 10 + j, BLACK);
			}

			Sleep(200);
		}
		
	}
	
	for(int i = 0; i < 6; i++){
		EpicText("tetris",10, YELLOW);
		for(int j = i; j < 6; j++){
			if(j != i)Mid("                                                     ", (80 - strlen("                                                     ")) / 2, 10 + j, BLACK);
		}
		Sleep(200);
	}
	
	for(int i = 0; i < 5; i++){
		EpicText("tetris", 10, YELLOW);
		Sleep(150);
		clrscr();
		Sleep(200);
	}
	
	for(int i = 10; i > 3; i--){
		EpicText("tetris", i, YELLOW);
		Sleep(250);
	}
	
	
	
}

void Render(){
	
	int xd = 0;
	
	if(loading)LoadingScreenAnim();
	
	loading = false;
	
	bool anim_d = false;
	
	while(true){
		
		clrscr();
	
		while(page.MAINMENU){
			
			if(screen.TETRIS == 8){
				anim_d = true;
			}
			else if(screen.TETRIS == 3){
				anim_d = false;
			}
			
			if(!anim_d){
				screen.TETRIS++;
			}
			else if(anim_d){
				screen.TETRIS--;
			}
			
			EpicText("tetris", screen.TETRIS, YELLOW);
			
			for(int i = 0; i < 50 && page.MAINMENU ; i++){
				Mid( (selection.MAINMENU == 0 ? "> Play <" : "  Play  ") , (80 - strlen("  Play  ")) / 2, 17, (selection.MAINMENU == 0 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 1 ? "> Settings <" : "  Settings  ") , (80 - strlen("  Settings  ")) / 2, 18, (selection.MAINMENU == 1 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 2 ? "> Info <" : "  Info  ") , (80 - strlen("  Info  ")) / 2, 19, (selection.MAINMENU == 2 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 3 ? "> Exit <" : "  Exit  ") , (80 - strlen("  Exit  ")) / 2, 21, (selection.MAINMENU == 3 ? WHITE : DARKGRAY));
				
				#if DEBUG
					crmove(0,0);
					textcolor(DARKGRAY);
					printf("sel %d | pressed %3d %c | keys.UP %3d | keys.DOWN %3d | pos_t %d |", selection.MAINMENU, keys.PRESSED, keys.PRESSED, keys.UP, keys.DOWN, screen.TETRIS);
					crmove(0,1);
					printf("width %3d | height %2d | sound %d", last_width, last_height, sound);
					Mid("DEBUG MODE",0,25,WHITE);
				#endif
				
				Sleep(25);
			}
			
		}
		
		clrscr();
		
		while(page.GAMETYPE){
			
			EpicText("game type",3, WHITE);
			
			Mid(selection.GAMETYPE == 1 ? "> Normal <" : "  Normal  ", (80 - strlen("> Normal <")) / 2, screen.GAME.GAMETYPE.Y, selection.GAMETYPE == 1 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 2 ? "> Chainreaction <" : "  Chainreaction  ", (80 - strlen("> Chainreaction <")) / 2, screen.GAME.GAMETYPE.Y + 1, selection.GAMETYPE == 2 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 3 ? "> Builder <" : "  Builder  ", (80 - strlen("> Builder <")) / 2, screen.GAME.GAMETYPE.Y + 2, selection.GAMETYPE == 3 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 4 ? "> Sharp shooter <" : "  Sharp shooter  ", (80 - strlen("> Sharp shooter <")) / 2, screen.GAME.GAMETYPE.Y + 3, selection.GAMETYPE == 4 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 5 ? "> Half way <" : "  Half way  ", (80 - strlen("> Half way <")) / 2, screen.GAME.GAMETYPE.Y + 4, selection.GAMETYPE == 5 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 6 ? "> Mutators <" : "  Mutators  ", (80 - strlen("> Mutators <")) / 2, screen.GAME.GAMETYPE.Y + 6, selection.GAMETYPE == 6 ? WHITE : DARKGRAY);
			
			Mid(selection.GAMETYPE == 7 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.GAME.GAMETYPE.Y + 9, selection.GAMETYPE == 7 ? WHITE : DARKGRAY);

			if(page.MUTATOR){
				clrscr();
				
				EpicText("mutators", 1, WHITE);
				
				while(page.MUTATOR){
					Mid("Timefactor", 20, screen.GAME.MUTATOR.Y, selection.MUTATOR == 0 ? WHITE : DARKGRAY);
					crmove(50, screen.GAME.MUTATOR.Y);
					textcolor(selection.MUTATOR == 0 ? WHITE : DARKGRAY);
					printf(mods.TIMEFACTOR >= 1 ? "%4.0fx" : (mods.TIMEFACTOR >= 0.5 ? "%4.1fx" : "%1.2fx") ,mods.TIMEFACTOR);
					
					Mid("Physics", 20, screen.GAME.MUTATOR.Y + 1, selection.MUTATOR == 1 ? WHITE : DARKGRAY);
					Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 1, BLACK);
					Mid("Normal", mods.CHAINREACTION ? 54 - strlen("Chainreaction") / 2 - strlen("Normal") - 2 : 54 - strlen("Normal") / 2, screen.GAME.MUTATOR.Y + 1, selection.MUTATOR == 1 ? (!mods.CHAINREACTION ? WHITE : DARKGRAY) : (!mods.CHAINREACTION ? DARKGRAY : BLACK));
					Mid("Chainreaction", mods.CHAINREACTION ? 54 - strlen("Chainreaction") / 2 : 54 + 2 + strlen("Normal") / 2, screen.GAME.MUTATOR.Y + 1, selection.MUTATOR == 1 ? (mods.CHAINREACTION ? WHITE : DARKGRAY) : (mods.CHAINREACTION ? DARKGRAY : BLACK));						
					
					Mid("Upsidedown", 20 , screen.GAME.MUTATOR.Y + 2, selection.MUTATOR == 2 ? WHITE : DARKGRAY);
					Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 2, BLACK);
					Mid("Disabled", mods.UPSIDEDOWN ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 2, selection.MUTATOR == 2 ? (!mods.UPSIDEDOWN ? WHITE : DARKGRAY) : (!mods.UPSIDEDOWN ? DARKGRAY : BLACK));						
					Mid("Enabled", mods.UPSIDEDOWN ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 2, selection.MUTATOR == 2 ? (mods.UPSIDEDOWN ? WHITE : DARKGRAY) : (mods.UPSIDEDOWN ? DARKGRAY : BLACK));
					
					Mid("Board Fill", 20, screen.GAME.MUTATOR.Y + 3, selection.MUTATOR == 3 ? WHITE : DARKGRAY);
					crmove(51,screen.GAME.MUTATOR.Y + 3);
					textcolor(selection.MUTATOR == 3 ? WHITE : DARKGRAY);
					printf("%3d%%",mods.BOARDFULL * 25);
					
					Mid("Harddrop only", 20, screen.GAME.MUTATOR.Y + 4, selection.MUTATOR == 4 ? WHITE : DARKGRAY);
					Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 4, BLACK);
					Mid("Disabled", mods.HARDDROPONLY ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 4, selection.MUTATOR == 4 ? (!mods.HARDDROPONLY ? WHITE : DARKGRAY) : (!mods.HARDDROPONLY ? DARKGRAY : BLACK));						
					Mid("Enabled", mods.HARDDROPONLY ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 4, selection.MUTATOR == 4 ? (mods.HARDDROPONLY ? WHITE : DARKGRAY) : (mods.HARDDROPONLY ? DARKGRAY : BLACK));
					
					Mid("Time", 20, screen.GAME.MUTATOR.Y + 5, selection.MUTATOR == 5 ? WHITE : DARKGRAY);
					Mid("                  ", 50 - strlen("Disabled") / 2, screen.GAME.MUTATOR.Y + 5, BLACK);
					crmove(mods.TIMERUN == -1 ? 54-strlen("Endless")/2 : (mods.TIMERUN == 0 ? 54 - strlen("Disabled")/2 : 52), screen.GAME.MUTATOR.Y + 5);
					textcolor(selection.MUTATOR == 5 ? WHITE : DARKGRAY);
					if(mods.TIMERUN <= 0){ printf(mods.TIMERUN == -1 ? "Endless" : "Disabled"); }
					else printf("%dmin",mods.TIMERUN);
				    
					Mid("Random rebuild", 20, screen.GAME.MUTATOR.Y + 6, selection.MUTATOR == 6 ? WHITE : DARKGRAY);
					Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 6, BLACK);
					Mid("Disabled", mods.REBUILD ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 6, selection.MUTATOR == 6 ? (!mods.REBUILD ? WHITE : DARKGRAY) : (!mods.REBUILD ? DARKGRAY : BLACK));						
					Mid("Enabled", mods.REBUILD ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 6, selection.MUTATOR == 6 ? (mods.REBUILD ? WHITE : DARKGRAY) : (mods.REBUILD ? DARKGRAY : BLACK));
					
					Mid("Board size", 20, screen.GAME.MUTATOR.Y + 7, selection.MUTATOR == 7 ? WHITE : DARKGRAY);
					Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 7, BLACK);
					Mid(mods.BOARDSIZE == 0 ? "Small" : (mods.BOARDSIZE == 1 ? "Medium" : "HUGE"), 54 - (mods.BOARDSIZE == 0 ? strlen("Small") : (mods.BOARDSIZE == 1 ? strlen("Medium") : strlen("HUGE"))) / 2, screen.GAME.MUTATOR.Y + 7, selection.MUTATOR == 7 ? WHITE : DARKGRAY);
					
					Mid("Ghostblocks", 20, screen.GAME.MUTATOR.Y + 8, selection.MUTATOR == 8 ? WHITE : DARKGRAY);
					Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 8, BLACK);
					Mid("Disabled", mods.GHOSTBLOCKS ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 8, selection.MUTATOR == 8 ? (!mods.GHOSTBLOCKS ? WHITE : DARKGRAY) : (!mods.GHOSTBLOCKS ? DARKGRAY : BLACK));						
					Mid("Enabled", mods.GHOSTBLOCKS ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 8, selection.MUTATOR == 8 ? (mods.GHOSTBLOCKS ? WHITE : DARKGRAY) : (mods.GHOSTBLOCKS ? DARKGRAY : BLACK));
					
					Mid("Rows builder", 20, screen.GAME.MUTATOR.Y + 9, selection.MUTATOR == 9 ? WHITE : DARKGRAY);
					Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 9, BLACK);
					Mid("Disabled", mods.BUILDER ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 9, selection.MUTATOR == 9 ? (!mods.BUILDER ? WHITE : DARKGRAY) : (!mods.BUILDER ? DARKGRAY : BLACK));						
					Mid("Enabled", mods.BUILDER ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 9, selection.MUTATOR == 9 ? (mods.BUILDER ? WHITE : DARKGRAY) : (mods.BUILDER ? DARKGRAY : BLACK));
					
					
					Mid(selection.MUTATOR == 10 ? "> Start game <" : "  Start game  ", (80 - strlen("  Start game  ")) / 2, screen.GAME.MUTATOR.Y + 12, selection.MUTATOR == 10 ? WHITE : DARKGRAY);
					Mid(selection.MUTATOR == 11 ? "> Back <" : "  Back  ", (80 - strlen("  Back  ")) / 2, screen.GAME.MUTATOR.Y + 14, selection.MUTATOR == 11 ? WHITE : DARKGRAY);
							
					Sleep(50);
				}
				
				clrscr();
				
			}
			
			Sleep(50);
		}
		
		
		clrscr();
		
		while(page.GAME){
			
			if(page.GAMELOADED){
				switch(block.NEXT){
				
					case 1:
						Mid("####", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid("    ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 2:			
						Mid("  # ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ###", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 3:
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
					
					case 4:
						Mid(" #  ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ###", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 5:
						Mid("   #", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ###", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 6:
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid("  ##", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 7:
						Mid("  ##", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
				}
				
				for(row_th = 0; row_th < screen.GAME.BRDSIZE.Y + 2; row_th++){
					
					#if DEBUG
						textcolor(DARKGRAY);
						crmove(0,screen.GAME.BOARD.Y + row_th);
						printf("%2d",row_th);
					#endif
					
					if(row_th < screen.GAME.BRDSIZE.Y){
						Mid("<|", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						//for(int x = 0; x < screen.GAME.BRDSIZE.X - 2; x++) Mid(" ", screen.GAME.BOARD.X, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						Mid("|>", screen.GAME.BOARD.X + screen.GAME.BRDSIZE.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					else if(row_th == screen.GAME.BRDSIZE.Y){
						Mid("<", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						for(int x = 0; x < screen.GAME.BRDSIZE.X; x++) Mid("=", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						Mid(">", screen.GAME.BOARD.X + screen.GAME.BRDSIZE.X - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					else{
						for(int x = 0; x < screen.GAME.BRDSIZE.X; x++) Mid("v", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					
					#if DEBUG
						Mid("  123456789ABCDEFG  ", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + screen.GAME.BRDSIZE.Y + 2, DARKGRAY);
					#endif
				}
				
				Mid("LEVEL", screen.GAME.CURBLOCK.X - strlen("LEVEL"), screen.GAME.CURBLOCK.Y, WHITE);
				
				Mid("SCORE", screen.GAME.CURBLOCK.X - strlen("SCORE"), screen.GAME.CURBLOCK.Y + 2, WHITE);
				Mid("000000", screen.GAME.CURBLOCK.X + 2, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				
				Mid("Next Block", screen.GAME.CURBLOCK.X - strlen("Next block")/2, screen.GAME.CURBLOCK.Y + 5, WHITE);
				
				Mid("Stats", screen.GAME.CURBLOCK.X - strlen("Stats")/2, screen.GAME.CURBLOCK.Y + 11, WHITE);
				Mid("I-Type", screen.GAME.CURBLOCK.X - strlen("Next block") + 2, screen.GAME.CURBLOCK.Y + 12, WHITE);
				Mid("L-Type", screen.GAME.CURBLOCK.X - strlen("Next block") + 2, screen.GAME.CURBLOCK.Y + 13, WHITE);
				Mid("J-Type", screen.GAME.CURBLOCK.X - strlen("Next block") + 2, screen.GAME.CURBLOCK.Y + 14, WHITE);
				Mid("O-Type", screen.GAME.CURBLOCK.X - strlen("Next block") + 2, screen.GAME.CURBLOCK.Y + 15, WHITE);
				Mid("T-Type", screen.GAME.CURBLOCK.X - strlen("Next block") + 2, screen.GAME.CURBLOCK.Y + 16, WHITE);
				Mid("Z-Type", screen.GAME.CURBLOCK.X - strlen("Next block") + 2, screen.GAME.CURBLOCK.Y + 17, WHITE);
				Mid("S-Type", screen.GAME.CURBLOCK.X - strlen("Next block") + 2, screen.GAME.CURBLOCK.Y + 18, WHITE);
				
				Mid("0000", screen.GAME.CURBLOCK.X + 4, screen.GAME.CURBLOCK.Y + 12, DARKGRAY);
				Mid("0000", screen.GAME.CURBLOCK.X + 4, screen.GAME.CURBLOCK.Y + 13, DARKGRAY);
				Mid("0000", screen.GAME.CURBLOCK.X + 4, screen.GAME.CURBLOCK.Y + 14, DARKGRAY);
				Mid("0000", screen.GAME.CURBLOCK.X + 4, screen.GAME.CURBLOCK.Y + 15, DARKGRAY);
				Mid("0000", screen.GAME.CURBLOCK.X + 4, screen.GAME.CURBLOCK.Y + 16, DARKGRAY);
				Mid("0000", screen.GAME.CURBLOCK.X + 4, screen.GAME.CURBLOCK.Y + 17, DARKGRAY);
				Mid("0000", screen.GAME.CURBLOCK.X + 4, screen.GAME.CURBLOCK.Y + 18, DARKGRAY);
				
				crmove(screen.GAME.CURBLOCK.X + 3 - Digits(level), screen.GAME.CURBLOCK.Y);
				textcolor(WHITE);
				printf("%d", level);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(score), screen.GAME.CURBLOCK.Y + 2);
				textcolor(WHITE);
				printf("%d", score);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.I), screen.GAME.CURBLOCK.Y + 12);
				textcolor(WHITE);
				printf("%d", block.STATS.I);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.L), screen.GAME.CURBLOCK.Y + 13);
				textcolor(WHITE);
				printf("%d", block.STATS.L);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.J), screen.GAME.CURBLOCK.Y + 14);
				textcolor(WHITE);
				printf("%d", block.STATS.J);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.O), screen.GAME.CURBLOCK.Y + 15);
				textcolor(WHITE);
				printf("%d", block.STATS.O);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.T), screen.GAME.CURBLOCK.Y + 16);
				textcolor(WHITE);
				printf("%d", block.STATS.T);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.Z), screen.GAME.CURBLOCK.Y + 17);
				textcolor(WHITE);
				printf("%d", block.STATS.Z);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.S), screen.GAME.CURBLOCK.Y + 18);
				textcolor(WHITE);
				printf("%d", block.STATS.S);
				
				page.GAMELOADED = false;
			}
			
			if(!mods.UPSIDEDOWN){
				for(row_th = 0; row_th < screen.GAME.BRDSIZE.Y; row_th++){
					for(column_th = 0; column_th < screen.GAME.BRDSIZE.X - 2; column_th++){
						Mid(board2[row_th][column_th + 1] ? "#" : (board[row_th][column_th + 1] ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, board2[row_th][column_th + 1] ? board2_color[row_th][column_th + 1] : board_color[row_th][column_th + 1]);
					}
				}
			}
			
			else if(mods.UPSIDEDOWN){
				for(row_th = 0; row_th < screen.GAME.BRDSIZE.Y; row_th++){
					for(column_th = 0; column_th < screen.GAME.BRDSIZE.X - 2; column_th++){
						Mid(board2[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] ? "#" : (board[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, board2[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] ? board2_color[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] : board_color[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1]);
					}
				}
			}
			
			

			
			#if DEBUG
				crmove(0,0);
				textcolor(DARKGRAY);
				printf("delay %2d | level %2d | rowtoclear %2d",delay, level, thislevelclear);
			#endif	
			
			Sleep(delay / 2);
		}
		
		clrscr();
		
		while(page.SETTINGS){
			
			EpicText("settings", 2, WHITE);
			
			Mid(selection.SETTINGS == 0 ? "> Key Binding <" : "  Key Binding  ", (80 - strlen("> Key Binding <")) / 2, screen.SETTINGS.Y, selection.SETTINGS == 0 ? WHITE : DARKGRAY);
			Mid(selection.SETTINGS == 1 ? (sound ? "> Sound ON < " : "> Sound OFF <") : (sound ? "  Sound ON   " : "  Sound OFF  "), (80 - strlen("> Sound OFF <")) / 2, screen.SETTINGS.Y + 1, selection.SETTINGS == 1? WHITE : DARKGRAY);
			Mid(selection.SETTINGS == 2 ? "> Reset HighScores <" : "  Reset HighScores  ", (80 - strlen("> Reset HighScores <")) / 2, screen.SETTINGS.Y + 4, selection.SETTINGS == 2 ? WHITE : DARKGRAY);
			Mid(selection.SETTINGS == 3 ? "> Reset Defaults <" : "  Reset Defaults  ", (80 - strlen("> Reset Defaults <")) / 2, screen.SETTINGS.Y + 5, selection.SETTINGS == 3 ? WHITE : DARKGRAY);
	
			Mid(selection.SETTINGS == 4 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.SETTINGS.Y + 8, selection.SETTINGS == 4 ? WHITE : DARKGRAY);
			
			if(page.KEYS){
				
				clrscr();
				
				EpicText("settings", 2, WHITE);
				
				while(page.KEYS){
				
						
					Mid("> Key Binding <", 13 - strlen("> Key Binding <") / 2, 13, DARKGRAY);
					Mid(sound ? "  Sound ON   " : "  Sound OFF  ", 14 - strlen("> Key Binding <") / 2, 14, DARKGRAY);
					Mid("  Reset HighScores  ", 14 - strlen("> Reset HighScores <") / 2, 17, DARKGRAY);
					Mid("  Reset Defaults  ", 14 - strlen("> Reset defaults <") / 2, 18, DARKGRAY);
					Mid("  Back  ", 14 - strlen("> Back <") / 2, 21, DARKGRAY);
					
					Mid("Keys", (80 - strlen("Keys")) / 2, 11, WHITE);
					Mid("Primary", 50, 11, WHITE);
					Mid("Secondary*", 60, 11, WHITE);
					
					Mid(selection.KEYS == 0 ? "> Up" : "  Up", (80 - strlen("Keys")) / 2 - 1, 14, selection.KEYS == 0 ? WHITE : DARKGRAY);
					crmove(53, 14);
					printf("%s", Keys(keys.UP));
					crmove(63, 14);
					printf("W");
					
					Mid(selection.KEYS == 1 ? "> Down" : "  Down", (80 - strlen("Keys")) / 2 - 1, 15, selection.KEYS == 1 ? WHITE : DARKGRAY);
					crmove(53, 15);
					printf("%s", Keys(keys.DOWN));
					crmove(63, 15);
					printf("S");
					
					Mid(selection.KEYS == 2 ? "> Left" : "  Left", (80 - strlen("Keys")) / 2 - 1, 16, selection.KEYS == 2 ? WHITE : DARKGRAY);
					crmove(53, 16);
					printf("%s", Keys(keys.LEFT));
					crmove(63, 16);
					printf("A");
					
					Mid(selection.KEYS == 3 ? "> Right" : "  Right", (80 - strlen("Keys")) / 2 - 1, 17, selection.KEYS == 3 ? WHITE : DARKGRAY);
					crmove(53, 17);
					printf("%s", Keys(keys.RIGHT));
					crmove(63, 17);
					printf("D");
					
					Mid(selection.KEYS == 4 ? "> Rotate left" : "  Rotate left", (80 - strlen("Keys")) / 2 - 1, 18, selection.KEYS == 4 ? WHITE : DARKGRAY);
					crmove(53, 18);
					printf("%s", Keys(keys.ROTATE_L));
					crmove(63, 18);
					printf("Q");
					
					Mid(selection.KEYS == 5 ? "> Rotate right" : "  Rotate right", (80 - strlen("Keys")) / 2 - 1, 19, selection.KEYS == 5 ? WHITE : DARKGRAY);
					crmove(53, 19);
					printf("%s", Keys(keys.ROTATE_R));
					crmove(63, 19);
					printf("E");
					
					Mid(selection.KEYS == 6 ? "> Drop" : "  Drop", (80 - strlen("Keys")) / 2 - 1, 20, selection.KEYS == 6 ? WHITE : DARKGRAY);
					crmove(53, 20);
					printf("%s", Keys(keys.DROP));
					crmove(63, 20);
					printf("SPACE");
					
					Mid(selection.KEYS == 7 ? "> Pause" : "  Pause", (80 - strlen("Keys")) / 2 - 1, 21, selection.KEYS == 7 ? WHITE : DARKGRAY);
					crmove(53, 21);
					printf("%s", Keys(keys.PAUSE));
					crmove(63, 21);
					printf("ESC");
					
					Mid(selection.KEYS == 8 ? "> Next song" : "  Next song", (80 - strlen("Keys")) / 2 - 1, 22, selection.KEYS == 8 ? WHITE : DARKGRAY);
					crmove(53, 22);
					printf("%s", Keys(keys.NEXTSONG));
					crmove(63, 22);
					printf("N");
					
					Mid(selection.KEYS == 9 ? "> Back <" : "  Back  " , (80 - strlen("> Back <")) / 2, 30, selection.KEYS == 9 ? WHITE : DARKGRAY);
					
					Mid("*can't be changed - backup keys", 80 - strlen("*can't be changed - backup keys") - 1, 34, DARKGRAY);
					
					if(keys.PRESSED == 13){
						
						if(selection.KEYS != 9){
							Mid("Press any key, DELETE for cancel", (80 - strlen("Press any key, DELETE for cancel")) / 2, 9, WHITE);
						}
					}
					
					else if(keys.SAMEKEYEXCEPTION){
						Mid(" Can't bind, key already binded!", (80 - strlen(" Can't bind, key already binded!")) / 2, 9, RED);
					}
					
					else Mid("                                ", (80 - strlen("Press any key, DELETE for cancel")) / 2, 9, WHITE);
					
					Sleep(50);
				}
				
				clrscr();
			}
			
			if(keys.PRESSED == 13){
				if(selection.SETTINGS == 3){
					Mid("DEFAULTS RESTORED!", (80 - strlen("DEFAULTS RESTORED!")) / 2, 10, LIGHTRED);
					xd = 0;
				}
			}
			
			else if(xd < 21) xd++; 
			
			if(xd == 20) Mid("                  ", (80 - strlen("DEFAULTS RESTORED!")) / 2, 10, LIGHTRED);
			
			
			Sleep(50);
		}
		
		clrscr();
		
		while(page.INFO){
			
			EpicText("tetris", 2, WHITE);
			
			if(page.HELP){
				clrscr();
				
				EpicText("tetris", 2, WHITE);
				
				while(page.HELP){
					
					for(row_th = 0; row_th < SMALL_H; row_th++){
						for(column_th = 0; column_th < SMALL_W - 1; column_th++){
							Mid(help2[row_th][column_th + 1] ? "#" : (help[row_th][column_th + 1] ? "x" : "."), 10 + column_th, 10 + row_th, help2[row_th][column_th + 1] ? help2_color[row_th][column_th + 1] : help_color[row_th][column_th + 1]);
						}
					}
					
					crmove(60 - strlen("                             ") / 2, 10);
					textcolor(BLACK);
					printf("                             ");
					
					crmove(60 - (strlen(help_label[selection.HELPPAGE]) + 4) / 2, 10);
					textcolor(selection.HELP == 0 ? WHITE : DARKGRAY);
					printf(selection.HELP == 0 ? "> %s <" : "  %s  ",help_label[selection.HELPPAGE]);
					
					for(row_th = 0; row_th < 8; row_th++){
						Mid("                                         ", 45, 13 + row_th, BLACK);
					}
					
					for(row_th = 0; row_th < help_txt_rows[selection.HELPPAGE]; row_th++){
						Mid(help_txt[selection.HELPPAGE][row_th], 45, 13 + row_th, selection.HELP == 0 ? WHITE : DARKGRAY);
					}
					
					Mid(selection.HELP == 1 ? "> Back <" : "  Back  ", 60 - strlen("  Back  ") / 2, 25, selection.HELP == 1 ? WHITE : DARKGRAY);
					
					Sleep(50);
				}
				
				clrscr();
				
			}
			
			Mid("(c) 2016 David Knieradl",(80 - strlen("(c) 2016 David Knieradl")) / 2, screen.INFO.Y, WHITE);
			Mid("Song: Leonell Cassio - TiME", (80 - strlen("Song: Leonell Cassio - TiME")) / 2, screen.INFO.Y + 1, WHITE);
			
			
			Mid(selection.INFO == 0 ? "> Help <" : "  Help  ", (80 - strlen("  Help  ")) / 2, screen.INFO.Y + 4, selection.INFO == 0 ? WHITE : DARKGRAY);
			Mid(selection.INFO == 1 ? "> Playlist <" : "  Playlist  ", (80 - strlen("  Playlist  ")) / 2, screen.INFO.Y + 5, selection.INFO == 1 ? WHITE : DARKGRAY);
			Mid(selection.INFO == 2 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.INFO.Y + 8,selection.INFO == 2 ? WHITE : DARKGRAY);
			
			Sleep(50);
		}
		
		while(page.EXIT){
			Mid("Are you sure you want to quit this game?", (80 - strlen("Are you sure you want to quit this game?")) / 2, screen.EXIT.Y, WHITE);
		
			Mid(exit_app ? "> Yes <" : "  Yes  ", 20, screen.EXIT.Y + 3, exit_app ? WHITE : DARKGRAY);
			Mid(!exit_app ? "> No <" : "  No  ", 54, screen.EXIT.Y + 3, !exit_app ? WHITE : DARKGRAY);
			
			Sleep(50);
		}
		
		clrscr();
		
		while(page.PAUSE){
			
			if(page.GAMELOADED){
				switch(block.NEXT){
				
					case 1:
						Mid("####", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid("    ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 2:			
						Mid("  # ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ###", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 3:
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
					
					case 4:
						Mid(" #  ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ###", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 5:
						Mid("   #", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ###", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 6:
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid("  ##", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
					case 7:
						Mid("  ##", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 7, DARKGRAY);
						Mid(" ## ", screen.GAME.CURBLOCK.X - 2, screen.GAME.CURBLOCK.Y + 8, DARKGRAY);
						break;
						
				}
				
				for(row_th = 0; row_th < screen.GAME.BRDSIZE.Y + 2; row_th++){
					
					#if DEBUG
						textcolor(DARKGRAY);
						crmove(0,screen.GAME.BOARD.Y + row_th);
						printf("%2d",row_th);
					#endif
					
					if(row_th < screen.GAME.BRDSIZE.Y){
						Mid("<|", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						for(int x = 0; x < screen.GAME.BRDSIZE.X - 2; x++) Mid(" ", screen.GAME.BOARD.X, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						Mid("|>", screen.GAME.BOARD.X + screen.GAME.BRDSIZE.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					else if(row_th == screen.GAME.BRDSIZE.Y){
						Mid("<", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						for(int x = 0; x < screen.GAME.BRDSIZE.X; x++) Mid("=", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						Mid(">", screen.GAME.BOARD.X + screen.GAME.BRDSIZE.X - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					else{
						for(int x = 0; x < screen.GAME.BRDSIZE.X; x++) Mid("v", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					
					#if DEBUG
						Mid("  123456789ABCDEFG  ", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + screen.GAME.BRDSIZE.Y + 2, DARKGRAY);
					#endif
				}
				
				Mid("LEVEL", screen.GAME.CURBLOCK.X - strlen("LEVEL"), screen.GAME.CURBLOCK.Y, WHITE);
				
				Mid("SCORE", screen.GAME.CURBLOCK.X - strlen("SCORE"), screen.GAME.CURBLOCK.Y + 2, WHITE);
				Mid("000000", screen.GAME.CURBLOCK.X + 2, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				
				Mid("Next Block", screen.GAME.CURBLOCK.X - strlen("Next block")/2, screen.GAME.CURBLOCK.Y + 5, WHITE);
				
				
				
				if(mods.UPSIDEDOWN){
					for(row_th = 0; row_th < screen.GAME.BRDSIZE.Y; row_th++){
						for(column_th = 0; column_th < screen.GAME.BRDSIZE.X - 2; column_th++){
							Mid(board2[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] ? "#" : (board[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, board2[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] ? board2_color[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1] : board_color[screen.GAME.BRDSIZE.Y - 1 - row_th][column_th + 1]);
						}
					}
				}
				
				else if(!mods.UPSIDEDOWN){
					for(row_th = 0; row_th < screen.GAME.BRDSIZE.Y; row_th++){
						for(column_th = 0; column_th < screen.GAME.BRDSIZE.X - 2; column_th++){
							Mid(board2[row_th][column_th + 1] ? "#" : (board[row_th][column_th + 1] ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						}
					}
				}
				
				page.GAMELOADED = false;
			}
			
			
			Mid("PAUSED", screen.GAME.PAUSE.X - strlen("PAUSED") / 2, screen.GAME.PAUSE.Y, WHITE);
			Mid(selection.PAUSE == 0 ? "> Resume <" : "  Resume  ", screen.GAME.PAUSE.X - strlen("> Resume <") / 2, screen.GAME.PAUSE.Y + 2, selection.PAUSE == 0 ? WHITE : DARKGRAY);
			Mid(selection.PAUSE == 1 ? "> Restart game <" : "  Restart game  ", screen.GAME.PAUSE.X - strlen("> Restart game <") / 2, screen.GAME.PAUSE.Y + 3, selection.PAUSE == 1 ? WHITE : DARKGRAY);
			Mid(selection.PAUSE == 2 ? "> Settings <" : "  Settings  ", screen.GAME.PAUSE.X - strlen("> Settings <") / 2, screen.GAME.PAUSE.Y + 4, selection.PAUSE == 2 ? WHITE : DARKGRAY);
			Mid(selection.PAUSE == 3 ? "> Info <" : "  Info  ", screen.GAME.PAUSE.X - strlen("> Info <") / 2, screen.GAME.PAUSE.Y + 5, selection.PAUSE == 3 ? WHITE : DARKGRAY);
			Mid(selection.PAUSE == 4 ? "> Return to MainMenu <" : "  Return to MainMenu  ", screen.GAME.PAUSE.X - strlen("> Return to MainMenu <") / 2, screen.GAME.PAUSE.Y + 8, selection.PAUSE == 4 ? WHITE : DARKGRAY);
			
			if(page.PAUSERETURN){
				
				clrscr();
				
				while(page.PAUSERETURN){
					Mid("Are you sure you want to go to MainMenu?", (screen.EXITSIZE.X - strlen("Are you sure you want to go to MainMenu?")) / 2, screen.EXIT.Y, WHITE);
					Mid("Progress will be lost!", (screen.EXITSIZE.X - strlen("Progress will be lost!")) / 2, screen.EXIT.Y + 1, WHITE);
					Mid(qt_select ? "> Yes <" : "  Yes  ", 28, screen.EXIT.Y + 4, qt_select ? WHITE : DARKGRAY);
					Mid(!qt_select ? "> No <" : "  No  ", 52, screen.EXIT.Y + 4, !qt_select ? WHITE : DARKGRAY);
					Sleep(50);
				}
				
				clrscr();
			}
			
			Sleep(50);
		}
		
		
		Sleep(50);
	}
}

void Music(){
	
	srand(time(NULL));

	while(sound){
		
		PlaySound(song[cur_song], NULL, SND_ASYNC);
		
		for(int i = 0; i < song_l[cur_song] * 10 && sound; i++){
			
			if(!page.KEYS && (keys.PRESSED == 'n' || keys.PRESSED == keys.NEXTSONG)) break;
			
			Sleep(100);
		}
		
		keys.PRESSED = NULL;
		
		int temp = cur_song;
		while(temp == cur_song) cur_song = rand() % max_song;
	}
	
	PlaySound(0,0,0);
	return;
}

DWORD WINAPI WindowResizedEvent(void* data){
	WindowSize();
}

DWORD WINAPI RenderScreen(void* data){
	Render();
}

DWORD WINAPI MusicPlay(void* data){
	Music();
}

DWORD WINAPI Timer(void* data){
	
	/*timeleft = mods.TIMERUN;
	
	while(page.GAME){
		Sleep(100);
		
		while(page.)
	}*/
}


