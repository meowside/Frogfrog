
/// 	Tetris
/// 	Copyright: 2016 David Knieradl
/// 	Author: David Knieradl
/// 	Description: rocnikova prace

const char *author	=	"David Knieradl";
const char *version	=	"1.4";
const char *day		=	"5";
const char *month	=	"05";
const char *year	=	"2016";
char name[128];

#define DEBUG 1

///////////////////////////////////////////////////////////////////////////////////////////////////
///																								///
///									Pokud není otevøen projekt			  						///
///					Pridat "-lwinmm" (bez uvozovek) do pøíkazù pro kompilátor  					///
///												  												///
///											JINAK												///
///																								///
///							Pokud 64bit OS, tak pridat tuto cestu								///
///								vcetne uvozovek do tohoto pole:									///
///																								///
///		pole: Projekt -> Možnosti projektu -> Parametry -> Linkery								///
///		cesta: "C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32/libwinmm.a"		///
///																								///
///						Pokud 32bit OS, tak pridat tuto cestu do stejneho pole:					///
///																								///
///		cesta: "C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32/libwinmm.a"			///
///												  												///
///																								///
///							Podporován pouze Windows Vista a výš								///
///						Lze kompilovat pouze pomocí Dev-C++ ver.5 a výš							///
///																								///
///////////////////////////////////////////////////////////////////////////////////////////////////


/*
    -------- TO DO ---------

	*	   EVERYTHING	   *
	
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

#define TR true
#define FL false

#define UP_K 296				//up arrow key code
#define DOWN_K 304				//down arrow key code
#define LEFT_K 299				//left arrow key code
#define RIGHT_K 301				//right arrow key code
#define SPACE_K 32				//space key code
#define ESC 27					//esc key code
#define F1 -59					//F1 key code
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

#define BLOCKS 7

#define DELAY 40

#define Random(x,y) rand() % (y - x + 1) + x

#if !DEBUG
	bool loading	=	true;
#elif DEBUG
	bool loading	=	false;
#endif

enum HELP{
	MOVING,
	SCORING,
	NORMAL,
	CHAINREACTION,
	BUILDER,
	SHARPSHOOTER,
	HALFWAY,
	MUTATORS
};

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

enum BLOCKTYPES{
	I,
	T,
	O,
	J,
	L,
	Z,
	S
};

enum eKEY{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ROTATE_L,
	ROTATE_R,
	DROP,
	PAUSE,
	NEXTSONG,
	NUMBEROFTYPES
};

typedef struct VISUAL{
	bool IsBlock;
	unsigned char Color;
} VISUAL;

typedef struct BLOCK{
	short Width;
	short Height;
	short Left;
	
	bool IsActive;
	
	VISUAL Layout[2][4];
} BLOCK;

typedef struct BOARD{
	short WIDTH;
	short HEIGHT;
	
	VISUAL Layers[LARGE_H + 1][LARGE_W][3];
} BOARD;

typedef struct MODS{
	float TIMEFACTOR;		// done
	
	short BOARDSIZE;			// done
	short BOARDFULL;		// done
	short TIMERUN;			// done
	
	bool CHAINREACTION;		// done
	bool UPSIDEDOWN;		// done
	bool HARDDROPONLY;		// done
	bool GHOSTBLOCKS;		// done
	bool BUILDER;			// done
} MODS;

typedef struct KEYS{
	short KEY[NUMBEROFTYPES];
	short PRESSED;
	
	bool SAMEKEYEXCEPTION;
} KEYS;

typedef struct BLOCKTYPE{
	short I;
	short J;
	short L;
	short O;
	short S;
	short T;
	short Z;
} BLOCKTYPE;

typedef struct BLOCKPLUS{
	COORD POS;
	short WIDTH;
	short HEIGHT;
	short CUR;
	short NEXT;
	short CURBLOCK_SCORE;
	
	BLOCKTYPE STATS;
} BLOCKPLUS;

typedef struct GAMESCR{
	COORD GAMETYPE;
	COORD MUTATOR;
	
	COORD BOARD;
	COORD CURBLOCK;
	COORD PAUSE;
} GAMESCR;

typedef struct RENDER{
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
	
	short TETRIS;
} RENDER;

typedef struct PAGE{
	bool MAINMENU;
	bool GAME;
	bool GAMELOAD;
	bool GAMETYPE;
	bool GAMEOVER;
	bool GONAME;
	bool PAUSE;
	bool PAUSERETURN;
	bool SETTINGS;
	bool KEYS;
	bool INFO;
	bool EXIT;
	bool MUTATOR;
	bool HELP;
	bool PAGELOAD;
	bool HIGHSCORE;
	bool RESETGAME;
	bool BLOCKCREATION;
	bool CHANGELOG;
} PAGE;

typedef struct SELECT{
	short MAINMENU;
	short PAUSE;
	short SETTINGS;
	short GAMETYPE;
	short KEYS;
	short INFO;
	short EXIT;
	short MUTATOR;
	short HELP;
	short HELPPAGE;
	short BLOCKSEL;
	short BLOCKCREATEY;
	short BLOCKCREATEX;
	short CHANGELOG;
} SELECT;

typedef struct SCOREBOARD{
	char players[10][4];
	int scores[10];
	short index;
} SCOREBOARD;

typedef struct SETTINGS{
	bool sound;
	bool drophelp;
} SETTINGS;

//
//	Pages
//

int main();
void GameType();
void Game();
void Options();
void HighScore();
void Info();
bool Quit();
bool Pause();
void GameOver();
void ResetGame();
void BlockCreation();
void ChangeLog();

//
//	Functions
//

void GetBlockSizes();
void SoundPlayer();
void OnLoad();
void Defaults();
void HighScoreReset();
bool KeyBindingChecker();
void LoadSettings();
void LoadHighscores();
void LoadSounds();
void LoadKeys();
void LoadBlocks();
void SaveSettings();
void SaveKeys();
void SaveHighscore();
void SaveBlocks();
void ReadmeCreate();
void GetKey();
char * Keys(int key);
void EpicText(const char TEXT[], int y, int color);

void Mid(const char TEXT[], int x, int y, int color);
void Mid(const char TEXT[], int x, int y, int foreground, int background);
void setcursortype(int type);
void crmove(int x, int y);
void textattr(int _attr);
void textcolor(int _fg, int _bg);
void textforeground(int _color);
void textbackground(int _color);
void clrscr();
void SetWindow(int Width, int Height);
void SetFont(int size_x, int size_y);
void WindowSize();
int Digits(int number);
float BoardScoreRatio(int size);
void Box(int _height, int _width, int X, int Y, int _fg, int _bg);

//
//	Threads
//

DWORD WINAPI WindowResizedEvent(void* data);
DWORD WINAPI RenderScreen(void* data);
DWORD WINAPI MusicPlay(void* data);
DWORD WINAPI Timer(void* data);
DWORD WINAPI TimerGB(void* data);

//
//	Game Mechanics
//


void GenerateHelper(BOARD *brd, BLOCKPLUS *src, BLOCKPLUS *dest);
bool EndGame(BOARD *brd);
void AddRow(BOARD *brd);
void Endless(BOARD *brd);
void FillBoard(BOARD *brd);
void CurBlock(int sel, int y, BOARD *brd, BLOCKPLUS *blck);
void MoveLeft(BOARD *brd, BLOCKPLUS *blck);
void MoveRight(BOARD *brd, BLOCKPLUS *blck);
void RotateLeft(BOARD *brd, BLOCKPLUS *blck);
void RotateRight(BOARD *brd, BLOCKPLUS *blck);
bool CollisionCheck(BOARD *brd, BLOCKPLUS *blck);
void MoveDown(BOARD *brd, BLOCKPLUS *blck);
void BoardWrite(BOARD *brd, BLOCKPLUS *blck, bool _physicschain);
void FullRowDetection(BOARD *brd, BLOCKPLUS *blck);
void FullRowDetectionChain(BOARD *brd, BLOCKPLUS *blck);
void BuildGame(BOARD *brd);


//
//	VARIABLES
//


RENDER screen;

BLOCKPLUS block;
BLOCKPLUS blockhelp;
BLOCKPLUS helper;

const BLOCKTYPE reset_stats	=	{ 0, 0, 0, 0, 0, 0, 0 };
const COORD reset_pos	=	{ 0, 0};
const BLOCKPLUS reset_blck	=	{ reset_pos, 0, 0, 0, 0, 0, reset_stats };

BLOCKPLUS reset_block;

KEYS keys;
PAGE page;
SELECT selection;
MODS mods;
SCOREBOARD highscore;

BOARD board;
BOARD help;

SETTINGS settings;

BLOCK customblocks[3];


int temp		=	0;
int last_width	=	0;
int last_height	=	0;
int row			=	0;
int column		=	0;
int row_th		=	0;
int column_th	=	0;
int namecount	=	0;

bool collided		=	false;
bool qt_select		=	false;
bool collidedhelp	=	false;
bool soundavailable	=	true;
bool gameover		=	false;
bool exit_app		=	false;
bool keychanging	=	false;
bool defaults		=	false;
bool highreset		=	false;
bool reset			=	false;

int score			=	0;
int level			=	1;
int thislevelclear	=	0;

int delay		=	0;
int timeleft	=	0;
int rnd			=	0;

char player[3];
char info_txt[3][128];

//
// strings
//

const char *loadstr[]	=	{
	"Putting trolls into machines",
	"Obtaining randomizers",
	"Mixing pixels",
	"Formatting all disks",
	"Downloading illegal music",
	"Clearing boards",
	"Choosing hardcore gamemode",
	"No cool features available",
	"Destroying worlds",
	"Stealing princess",
	"Spawning dragons",
	"Waiting for love",
	"Here should be some random text",
	"Eating chocolate",
	"BSoD incoming",
	"Programming AI to kill all humans",
	"Deleting legendary blocks",
	"Uploading virus T.E.T.R.I.S.",
	"Using your computer to mine bitcoins",
	"Allocating 1024 Terabytes of RAM just for me!",
	"MUHEHEHEHEHE",
	"Connecting to DarkNET",
	"Working on 4 dimensional transporter",
	"Generating cheats"
};

#define load_scr_str 24 - 1

const char *changelog[]	=	{
	"Version 1.4",
	" - Added custom blocks",
	" - Added custom blocks into file",
	" - Added changelog"
	" - Bug fixes",
	" ",
	"Version 1.3",
	" - Updated loading screen",
	" - Bug fixes",
	" ",
	"Version 1.2",
	" - Added highscores",
	" - Added drop helper",
	" - Minor bug fixes",
	" Mutators",
	"   - Added time factor",
	"   - Added ghost blocks",
	"   - Added time",
	"   - Added boardfill",
	"   - Added rows build",
	"   - Added board sizes",
	"   - Added physics",
	" ",
	"Version 1.1",
	" - Added Multithreading",
	" - Optimization",
	" - Added Chainreaction mode",
	" - Added Sharpshooter mode",
	" - Added Halfway mode",
	" - Added Builder mode",
	" - Added Mutators",
	" - Added colors",
	" ",
	"Version 1.0",
	" - Released",
	" - Bugs correction"
};

#define changelog_rows 36 - 1

const char *img[]	=	{
	"#                   #",
	"##                 ##",
	"####             ####",
	"######         ######",
	"##  ####     ####  ##",
	"##   ####   ####   ##",
	"##     ### ###     ##",
	" ##    ### ###    ## ",
	"  #################  ",
	"         ###         ",
	"         ###         ",
	"                     ",
	"    #############    ",
	"  ##             ##  "
};


const char *song[]	=	{
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



int song_l[]	=	{
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

const char *help_label[]	=	{
	"Moving",
	"Scoring",
	"Normal",
	"Chainreaction",
	"Builed",
	"Sharp shooter",
	"Half way",
	"Mutators"
};

const char *help_txt[][8]	=	{
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

int help_txt_rows[]	=	{
	7,
	3,
	5,
	5,
	5,
	5,
	5,
	5
};

int cur_song;

#define max_song sizeof(song_l) / sizeof(int)

bool available_songs[max_song]	=	{false};

///		  xxxx  xxxxxx  xxxxxx
///		 x    x x     x x     x
///		 xxxxxx xxxxxxx xxxxxxx
///		 x    x x       x
///		 x    x x       x


int main(){
	
	OnLoad();
	
	page.MAINMENU	=	true;
	
	last_height	=	screen.MMSIZE.Y;
	last_width	=	screen.MMSIZE.X;
	
	while(loading){
		Sleep(100);
	}
	
	while(_kbhit()) _getch();
	
	while(true){
		
		GetKey();
		
		if((keys.PRESSED == keys.KEY[UP] || keys.PRESSED == 'w') && selection.MAINMENU > 0){
			selection.MAINMENU--;	
			keys.PRESSED	=	0;
		}
		
		else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && selection.MAINMENU < 5){
			selection.MAINMENU++;
			keys.PRESSED	=	0;
		}
		
		else if(keys.PRESSED == 13){
			keys.PRESSED	=	0;
			
			page.MAINMENU	=	false;
			
			if(selection.MAINMENU == 0) GameType();
			else if(selection.MAINMENU == 1) HighScore();
			else if(selection.MAINMENU == 2) BlockCreation();
			else if(selection.MAINMENU == 3) Options();
			else if(selection.MAINMENU == 4) Info();
			else if(selection.MAINMENU == 5){
				int end	=	Quit();
				if(end) return EXIT_SUCCESS;
			}
			
			if(gameover){
				GameOver();
			}
			
			last_height	=	screen.MMSIZE.Y;
			last_width	=	screen.MMSIZE.X;
			
			page.MAINMENU	=	true;
			
		}
	}
	
	return EXIT_FAILURE;
}

void GameType(){
	
	page.GAMETYPE	=	true;
	page.PAGELOAD	=	true;

	selection.GAMETYPE	=	1;

	while(true){
		
		GetKey();
		
		if((keys.PRESSED == keys.KEY[UP] || keys.PRESSED == 'w') && selection.GAMETYPE > 1){
			selection.GAMETYPE--;
			keys.PRESSED	=	0;
		}
		
		else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && selection.GAMETYPE < 7){
			selection.GAMETYPE++;
			keys.PRESSED	=	0;
		}
		
		else if(keys.PRESSED == 13){
			
			keys.PRESSED	=	0;
			
			if(selection.GAMETYPE != 7){
				
				mods.BOARDFULL	=	0;
				mods.BUILDER	=	false;
				mods.CHAINREACTION	=	false;
				mods.BOARDSIZE	=	1;
				mods.GHOSTBLOCKS	=	false;
				mods.HARDDROPONLY	=	false;
				mods.TIMEFACTOR	=	1;
				mods.TIMERUN	=	false;
				mods.UPSIDEDOWN	=	false;
				
				if(selection.GAMETYPE == 1){ page.GAMETYPE	=	false; Game(); }
				
				else if(selection.GAMETYPE == 2){
					page.GAMETYPE	=	false;
					mods.CHAINREACTION	=	true;
					Game();
					
				}
				else if(selection.GAMETYPE == 3){
					page.GAMETYPE	=	false;
					mods.BUILDER	=	true;
					Game();
				}
				else if(selection.GAMETYPE == 4){
					page.GAMETYPE		=	false;
					mods.HARDDROPONLY	=	true;
					mods.BUILDER		=	true;
					mods.UPSIDEDOWN		=	true;
					Game();
				}
				
				else if(selection.GAMETYPE == 5){
					page.GAMETYPE	=	false;
					mods.BUILDER	=	true;
					mods.BOARDFULL	=	HALF;
					Game();
				}
				else if(selection.GAMETYPE == 6){
					page.MUTATOR	=	true;
					page.PAGELOAD	=	true;
					
					selection.MUTATOR == 0;
					
					while(page.MUTATOR){
						
						GetKey();
						
						if((keys.PRESSED == keys.KEY[UP] || keys.PRESSED == 'w') && selection.MUTATOR > 0){ selection.MUTATOR--; }
						else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && selection.MUTATOR < 10){ selection.MUTATOR++; }
						
						else if(keys.PRESSED == keys.KEY[LEFT] || keys.PRESSED == 'a'){
							if(selection.MUTATOR == 0){ mods.TIMEFACTOR /= 2; if(mods.TIMEFACTOR < 0.25) mods.TIMEFACTOR	=	4; }							
							else if(selection.MUTATOR == 1){ mods.CHAINREACTION	=	false; }							
							else if(selection.MUTATOR == 2){ mods.UPSIDEDOWN	=	false; }							
							else if(selection.MUTATOR == 3){ mods.BOARDFULL -= 1; if(mods.BOARDFULL < 0) mods.BOARDFULL	=	3; }							
							else if(selection.MUTATOR == 4){ mods.HARDDROPONLY	=	false; }							
							else if(selection.MUTATOR == 5){ mods.TIMERUN -= 1; if(mods.TIMERUN < -1) mods.TIMERUN	=	-1; }						
							else if(selection.MUTATOR == 6){ mods.BOARDSIZE -= 1; if(mods.BOARDSIZE < 0) mods.BOARDSIZE	=	2;}
							else if(selection.MUTATOR == 7){ mods.GHOSTBLOCKS	=	false; }							
							else if(selection.MUTATOR == 8){ mods.BUILDER		=	false; }
							
							keys.PRESSED == 0;
						}
						
						else if(keys.PRESSED == keys.KEY[RIGHT] || keys.PRESSED == 'd'){
							if(selection.MUTATOR == 0){ mods.TIMEFACTOR *= 2; if(mods.TIMEFACTOR > 4) mods.TIMEFACTOR	=	0.25; }							
							else if(selection.MUTATOR == 1){ mods.CHAINREACTION	=	true; }							
							else if(selection.MUTATOR == 2){ mods.UPSIDEDOWN	=	true; }							
							else if(selection.MUTATOR == 3){ mods.BOARDFULL += 1; if(mods.BOARDFULL > 3) mods.BOARDFULL	=	0; }							
							else if(selection.MUTATOR == 4){ mods.HARDDROPONLY	=	true; }							
							else if(selection.MUTATOR == 5){ mods.TIMERUN += 1; if(mods.TIMERUN > 60) mods.TIMERUN	=	-1;}			
							else if(selection.MUTATOR == 6){ mods.BOARDSIZE += 1; if(mods.BOARDSIZE > 2) mods.BOARDSIZE	=	0;}							
							else if(selection.MUTATOR == 7){ mods.GHOSTBLOCKS	=	true; }							
							else if(selection.MUTATOR == 8){ mods.BUILDER		=	true; }
							
							keys.PRESSED == 0;
						}
						
						else if(keys.PRESSED == 13){
							
							keys.PRESSED == 0;
							
							if(selection.MUTATOR == 9){
								page.GAMETYPE	=	false;
								page.MUTATOR	=	false;
								
								Game();
							}
							else if(selection.MUTATOR == 10){
								page.MUTATOR	=	false;
							}
						}
					}
				}
			}
			
			else{
				page.GAMETYPE	=	false;
				return;	
			}
			
			
			
			last_height	=	screen.MMSIZE.Y;
			last_width	=	screen.MMSIZE.X;
			
			page.PAGELOAD	=	true;
			page.GAMETYPE	=	true;
		}
		
		if(gameover){
			page.GAMETYPE	=	false;
			return;
		}
	}
}

void Game(){
	
	last_width	=	screen.GAMESIZE.X;
	last_height	=	screen.GAMESIZE.Y;
	
	page.GAMELOAD	=	true;
	page.GAME		=	true;
	
	bool end		=	false;
	bool down_brk	=	false;
	
	short build		=	0;
	
	board.WIDTH		=	(mods.BOARDSIZE == 0 ? SMALL_W : (mods.BOARDSIZE == 1 ? MEDIUM_W : LARGE_W));
	board.HEIGHT	=	(mods.BOARDSIZE == 0 ? SMALL_H : (mods.BOARDSIZE == 1 ? MEDIUM_H : LARGE_H));
	
	BuildGame(&board);
	
	while(!end){
		
		if(gameover){
		
			page.GAME	=	false;
			
			return;
		}
		
		if(mods.BUILDER){
			if(build >= 20){
				build	=	0;
				
				AddRow(&board);
			}
		}
		
		
		CurBlock(block.CUR	=	block.NEXT, 1, &board, &block);
		block.NEXT	=	Random(1, BLOCKS);
		page.GAMELOAD	=	true;
		
		if(settings.drophelp) GenerateHelper(&board, &block, &helper);
		
		if(delay > 2) delay	=	(DELAY - 2 * level) / mods.TIMEFACTOR;
		
		collided	=	false;
		
		while(!collided){
			for(int i	=	0; i < 30 && !collided; i++){
				
				Sleep(delay);
				
				if(timeleft == 0 && mods.TIMERUN > 0){
					gameover	=	true;
					page.GAME	=	false;
					
					return;
				}
				
				if(_kbhit()){
					
					GetKey();
					
					//
					// MOVE TO LEFT
					//
					
					if((keys.PRESSED == keys.KEY[LEFT] || keys.PRESSED == 'a')){
						
						keys.PRESSED == 0;
						
						MoveLeft(&board, &block);
						
						if(settings.drophelp) GenerateHelper(&board, &block, &helper);
					}
					
					//
					// MOVE TO RIGHT
					//
					
					else if((keys.PRESSED == keys.KEY[RIGHT] || keys.PRESSED == 'd')){
						
						keys.PRESSED == 0;
						
						MoveRight(&board, &block);
						
						if(settings.drophelp) GenerateHelper(&board, &block, &helper);
					}
					
					//
					// ROTATE LEFT
					//
					
					else if(keys.PRESSED == keys.KEY[ROTATE_L] || keys.PRESSED == 'q'){
						
						keys.PRESSED == 0;
						
						RotateLeft(&board, &block);
						
						if(settings.drophelp) GenerateHelper(&board, &block, &helper);
					}
					
					//
					// ROTATE RIGHT
					//
					
					else if(keys.PRESSED == keys.KEY[ROTATE_R] || keys.PRESSED == 'e'){
						
						keys.PRESSED == 0;
						
						RotateRight(&board, &block);
						
						if(settings.drophelp) GenerateHelper(&board, &block, &helper);
					}
					
					//
					// DROP
					//
					
					else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && !mods.HARDDROPONLY){ 
						
						keys.PRESSED == 0;
						
						collided	=	CollisionCheck(&board, &block);
						if(!collided){
							MoveDown(&board, &block);
							i	=	0;
						}
						else BoardWrite(&board, &block, mods.CHAINREACTION);
					}
					
					else if(keys.PRESSED == keys.KEY[DROP] || keys.PRESSED == 32){
						
						keys.PRESSED == 0;
						
						while(!collided){
							collided	=	CollisionCheck(&board, &block);
							if(!collided) MoveDown(&board, &block);
							Sleep(delay / 2);
						}
						BoardWrite(&board, &block, mods.CHAINREACTION);
						
						build++;
						
						score += level * BLOCKSCORE * mods.TIMEFACTOR;
					}
					
					else if(keys.PRESSED == ESC){
						
						keys.PRESSED == 0;
						
						bool mainmenu	=	false;
						
						page.GAME	=	false;
						
						mainmenu	=	Pause();
						
						qt_select	=	false;
						if(mainmenu) return;
						
						page.GAMELOAD	=	true;
						page.GAME	=	true;
						
						if(mods.TIMEFACTOR > 0) HANDLE thread4	=	CreateThread(NULL, 0, Timer, NULL, 0, NULL);
						if(mods.GHOSTBLOCKS) HANDLE thread5	=	CreateThread(NULL, 0, TimerGB, NULL, 0, NULL);
						
						if(reset){
							
							BuildGame(&board);
							
							CurBlock(block.CUR	=	block.NEXT, 1, &board, &block);
							block.NEXT	=	Random(1, BLOCKS);
							
							i	=	0;
						}
						
						if(settings.drophelp) GenerateHelper(&board, &block, &helper);
						else{
							for(row	=	0; row < LARGE_H + 1; row++){
								for(column	=	0; column < LARGE_W + 1; column++){
									board.Layers[row][column][2].IsBlock	=	false;
								}
							}
						}
					}
				}
				
				
						
			}
			
			if(!collided){
				collided	=	CollisionCheck(&board, &block);
			
				if(collided || block.POS.Y >= board.HEIGHT - 1){
					BoardWrite(&board, &block, mods.CHAINREACTION);
					score += BLOCKSCORE;
					
					build++;
				}
				
				else{
					if(!mods.HARDDROPONLY) MoveDown(&board, &block);
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


void GenerateHelper(BOARD *brd, BLOCKPLUS *src, BLOCKPLUS *dest){
	
	for(row	=	0; row < LARGE_H + 1; row++){
		for(column	=	0; column < LARGE_W; column++){
			brd -> Layers[row][column][2]	=	brd -> Layers[row][column][1];
		}
	}
	
	*dest	=	*src;
	
	bool coll	=	false;
	
	while(!coll){
		
		if(dest -> POS.Y >= brd -> HEIGHT - 1) coll	=	true;
		
		for(row	=	0; row < dest -> HEIGHT && !coll; row++){
			for(column	=	0; column < dest -> WIDTH && !coll; column++){
				if(brd -> Layers[dest -> POS.Y - row][dest -> POS.X + column][2].IsBlock && brd -> Layers[dest -> POS.Y - row + 1][dest -> POS.X + column][0].IsBlock) coll	=	true;
			}
		}
		
		if(!coll){
			
			dest -> POS.Y++;
			
			for(row	=	0; row < dest -> HEIGHT + 1; row++){
				for(column	=	0; column < dest -> WIDTH; column++){
					brd -> Layers[dest -> POS.Y - row][dest -> POS.X + column][2].IsBlock	=	brd -> Layers[dest -> POS.Y - row - 1][dest -> POS.X + column][2].IsBlock;
					brd -> Layers[dest -> POS.Y - row - 1][dest -> POS.X + column][2].IsBlock	=	false;
				}
			}
		}
		
	}
}

bool EndGame(BOARD *brd){
	
	int temp	=	0;
	
	for(column	=	brd -> WIDTH / 2 - 3; column < brd -> WIDTH / 2 + 3; column++){
		if(brd -> Layers[1][column][0].IsBlock) return true;
	}
	
	return false;
}

void AddRow(BOARD *brd){
	for(row	=	1; row < brd -> HEIGHT; row++){
		for(column	=	0; column < brd -> WIDTH; column++){
			brd -> Layers[row - 1][column + 1][0].IsBlock	=	brd -> Layers[row][column + 1][0].IsBlock;
			brd -> Layers[row][column + 1][0].IsBlock	=	false;
			
			brd -> Layers[row - 1][column + 1][0].Color	=	brd -> Layers[row][column + 1][0].Color;
			brd -> Layers[row][column + 1][0].Color	=	DARKGRAY;
		}
	}
	
	for(column	=	0; column < brd -> WIDTH - 2; column++){
		int blockis	=	Random(0, 15);
		while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis	=	Random(0, 15);
		
		if(Random(0, 1) == 0){
			brd -> Layers[brd -> HEIGHT - 1][column + 1][0].IsBlock	=	true;
			brd -> Layers[brd -> HEIGHT - 1][column + 1][0].Color	=	blockis;
		}
		else brd -> Layers[brd -> HEIGHT - 1][column + 1][0].Color	=	DARKGRAY;
	}
}

void Endless(BOARD *brd){
	bool temp	=	false;
	
	for(row	=	brd -> HEIGHT / 2; row > brd -> HEIGHT / 2 - 4; row--){
		for(column	=	0; column < brd -> WIDTH; column++){
			if(brd -> Layers[row][column][0].IsBlock) temp	=	true;
		}
	}
	
	while(temp){
		
		for(column	=	0; column < brd -> WIDTH; column++){
			brd -> Layers[board.HEIGHT - 1][column][0].IsBlock	=	false;
			brd -> Layers[board.HEIGHT - 1][column][0].Color	=	DARKGRAY;
		}
		
		for(int i	=	brd -> HEIGHT - 1; i > 1; i--){
			for(int j	=	0; j < brd -> WIDTH; j++){
				brd -> Layers[i][j][0].IsBlock	=	brd -> Layers[i - 1][j][0].IsBlock;
				brd -> Layers[i - 1][j][0].IsBlock	=	false;
				
				brd -> Layers[i][j][0].Color	=	brd -> Layers[i - 1][j][0].Color;
				brd -> Layers[i - 1][j][0].Color	=	DARKGRAY;
			}
		}
		
		temp	=	false;
		
		for(row	=	brd -> HEIGHT / 2; row > brd -> HEIGHT / 2 - 4; row--){
			for(column	=	0; column < brd -> WIDTH; column++){
				if(brd -> Layers[row][column][0].IsBlock) temp	=	true;
			}
		}
	}
	
}

void FillBoard(BOARD *brd){
	for(row	=	brd -> HEIGHT - (brd -> HEIGHT / 4) * mods.BOARDFULL; row < brd -> HEIGHT; row++){
		for(column	=	0; column < brd -> WIDTH - 2; column++){
			int blockis	=	Random(0, 15);
			while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis	=	Random(0, 15);
			
			if(Random(0, 1) == 0){
				brd -> Layers[row][column + 1][0].IsBlock	=	true;
				brd -> Layers[row][column + 1][0].Color	=	blockis;
			}
		}
	}
}

void CurBlock(int sel, int y, BOARD *brd, BLOCKPLUS *blck){
	
	//dfak	=	&blck -> POS.Y;
	
	blck -> POS.X	=	brd -> WIDTH / 2 - 2;
	
	
	if(sel == 1){
		// xxxx
		
		blck -> WIDTH	=	4;
		blck -> HEIGHT	=	1;
		blck -> POS.Y	=	y;
		
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 3][1].IsBlock	=	true;
		
		//
		//	COLOR
		//
		
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].Color	=	BLUE;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].Color	=	BLUE;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].Color	=	BLUE;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 3][1].Color	=	BLUE;
		
		blck -> STATS.I++;
	}
	
	else if(sel == 2){
		//  x
		// xxx
		
		blck -> WIDTH	=	3;
		blck -> HEIGHT	=	2;
		blck -> POS.Y	=	y;
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].IsBlock	=	true;
		
		//
		//	COLOR
		//
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].Color	=	GREEN;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].Color	=	GREEN;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].Color	=	GREEN;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].Color	=	GREEN;
		
		blck -> STATS.T++;
	}
	
	else if(sel == 3){
		// xx
		// xx
		
		blck -> WIDTH	=	2;
		blck -> HEIGHT	=	2;
		blck -> POS.Y	=	y;
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].IsBlock	=	true;
		
		//
		//	COLOR
		//
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X][1].Color	=	CYAN;
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].Color	=	CYAN;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].Color	=	CYAN;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].Color	=	CYAN;
		
		blck -> STATS.O++;
	}
	
	else if(sel == 4){
		// x
		// xxx
		
		blck -> WIDTH	=	3;
		blck -> HEIGHT	=	2;
		blck -> POS.Y	=	y;
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].IsBlock	=	true;
		
		//
		//	COLOR
		//
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X][1].Color	=	RED;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].Color	=	RED;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].Color	=	RED;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].Color	=	RED;
		
		blck -> STATS.J++;
	}
	
	else if(sel == 5){
		//   x
		// xxx
		
		blck -> WIDTH	=	3;
		blck -> HEIGHT	=	2;
		blck -> POS.Y	=	y;
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 2][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].IsBlock	=	true;
		
		//
		//	COLOR
		//
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 2][1].Color	=	MAGENTA;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].Color	=	MAGENTA;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].Color	=	MAGENTA;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].Color	=	MAGENTA;
		
		blck -> STATS.L++;
	}
	
	else if(sel == 6){
		// xx
		//  xx
		
		blck -> WIDTH	=	3;
		blck -> HEIGHT	=	2;
		blck -> POS.Y	=	y;
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].IsBlock	=	true;
		
		//
		//	COLOR
		//
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X][1].Color	=	LIGHTBLUE;
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].Color	=	LIGHTBLUE;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].Color	=	LIGHTBLUE;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 2][1].Color	=	LIGHTBLUE;
		
		blck -> STATS.Z++;
	}
	
	else if(sel == 7){
		//  xx
		// xx
		
		blck -> WIDTH	=	3;
		blck -> HEIGHT	=	2;
		blck -> POS.Y	=	y;
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 2][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].IsBlock	=	true;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].IsBlock	=	true;
		
		//
		//	COLOR
		//
		
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 1][1].Color	=	LIGHTGREEN;
		brd -> Layers[blck -> POS.Y - 1][blck -> POS.X + 2][1].Color	=	LIGHTGREEN;
		brd -> Layers[blck -> POS.Y][blck -> POS.X][1].Color	=	LIGHTGREEN;
		brd -> Layers[blck -> POS.Y][blck -> POS.X + 1][1].Color	=	LIGHTGREEN;
		
		blck -> STATS.S++;
	}
	
	else if(sel == 8 && customblocks[0].IsActive){
		///
		///	Custom block #1
		///
		
		blck -> WIDTH = customblocks[0].Width;
		blck -> HEIGHT = customblocks[0].Height;
		blck -> POS.Y	=	y;
		
		for(row = 0; row < 2; row++){
			for(column = 0; column < 4; column++){
				brd -> Layers[blck -> POS.Y + row - 1][column][1].IsBlock = customblocks[0].Layout[row][column].IsBlock;
				brd -> Layers[blck -> POS.Y + row - 1][column][1].Color = customblocks[0].Layout[row][column].Color;
			}
		}
	}
	
	else if(sel == 9 && customblocks[1].IsActive){
		///
		///	Custom block #1
		///
		
		blck -> WIDTH = customblocks[1].Width;
		blck -> HEIGHT = customblocks[1].Height;
		blck -> POS.Y	=	y;
		
		for(row = 0; row < 2; row++){
			for(column = 0; column < 4; column++){
				brd -> Layers[blck -> POS.Y + row - 1][column][1].IsBlock = customblocks[1].Layout[row][column].IsBlock;
				brd -> Layers[blck -> POS.Y + row - 1][column][1].Color = customblocks[1].Layout[row][column].Color;
			}
		}
	}
	
	else if(sel == 10 && customblocks[2].IsActive){
		///
		///	Custom block #1
		///
		
		blck -> WIDTH = customblocks[2].Width;
		blck -> HEIGHT = customblocks[2].Height;
		blck -> POS.Y	=	y;
		
		for(row = 0; row < 2; row++){
			for(column = 0; column < 4; column++){
				brd -> Layers[blck -> POS.Y + row - 1][column][1].IsBlock = customblocks[2].Layout[row][column].IsBlock;
				brd -> Layers[blck -> POS.Y + row - 1][column][1].Color = customblocks[2].Layout[row][column].Color;
			}
		}
	}
}

	//
	// MOVE TO LEFT
	//

void MoveLeft(BOARD *brd, BLOCKPLUS *blck){
	
	if(blck -> POS.X <= 1) return;
	
	for(row	=	0; row < blck -> WIDTH; row++){
		for(column	=	0; column < blck -> HEIGHT; column++){
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X + row - 1][1].IsBlock	=	brd -> Layers[blck -> POS.Y - column][blck -> POS.X + row][1].IsBlock;
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X + row][1].IsBlock	=	false;
			
			//
			//	COLOR MOVE
			//
			
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X + row - 1][1].Color	=	brd -> Layers[blck -> POS.Y - column][blck -> POS.X + row][1].Color;
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X + row][1].Color	=	DARKGRAY;
		}
	}
	
	blck -> POS.X--;
	
	bool canmove	=	true;
	
	for(row	=	blck -> POS.Y; row > blck -> POS.Y - blck -> HEIGHT && canmove; row--){
		for(column	=	blck -> POS.X; column < blck -> POS.X + blck -> WIDTH && canmove; column++){
			if(brd -> Layers[row][column][0].IsBlock && brd -> Layers[row][column][1].IsBlock){
				canmove	=	false;
			}
		}
	}
	
	if(!canmove) MoveRight(brd, blck);
}

	//
	// MOVE TO RIGHT
	//

void MoveRight(BOARD *brd, BLOCKPLUS *blck){
	
	if(blck -> POS.X + blck -> WIDTH - 1 >= brd -> WIDTH - 2) return;
	
	for(row	=	0; row < blck -> WIDTH; row++){
		for(column	=	0; column < blck -> HEIGHT; column++){
			
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X - row + blck -> WIDTH][1].IsBlock	=	brd -> Layers[blck -> POS.Y - column][blck -> POS.X - row + blck -> WIDTH - 1][1].IsBlock; 
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X - row + blck -> WIDTH - 1][1].IsBlock	=	false;
			
			//
			//	COLOR MOVE
			//
			
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X - row + blck -> WIDTH][1].Color	=	brd -> Layers[blck -> POS.Y - column][blck -> POS.X - row + blck -> WIDTH - 1][1].Color; 
			brd -> Layers[blck -> POS.Y - column][blck -> POS.X - row + blck -> WIDTH - 1][1].Color	=	DARKGRAY;
		}
	}
	
	blck -> POS.X++;
	
	bool canmove	=	true;
	
	for(row	=	blck -> POS.Y; row > blck -> POS.Y - blck -> HEIGHT && canmove; row--){
		for(column	=	blck -> POS.X; column < blck -> POS.X + blck -> WIDTH && canmove; column++){
			if(brd -> Layers[row][column][0].IsBlock && brd -> Layers[row][column][1].IsBlock){
				canmove	=	false;
			}
		}
	}
	
	if(!canmove) MoveLeft(brd, blck);
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

void RotateLeft(BOARD *brd, BLOCKPLUS *blck){
	
	if(blck -> POS.X + blck -> HEIGHT > brd -> WIDTH - 1) return;
	if(blck -> POS.Y - blck -> WIDTH + 1 < 0) return;
	
	bool mezipole[blck -> WIDTH][blck -> HEIGHT]	=	{false};
	unsigned char mezipole2[blck -> WIDTH][blck -> HEIGHT]	=	{0};
	
	// BLOCK ROTATE
						
	for(row	=	0; row < blck -> HEIGHT; row++){
		for(column	=	0; column < blck -> WIDTH; column++){
			mezipole[blck -> WIDTH - 1 - column][row]	=	brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].IsBlock;
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].IsBlock	=	false;
			
			mezipole2[blck -> WIDTH - 1 - column][row]	=	brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].Color;
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].Color	=	DARKGRAY;
		}
	}
						
	temp	=	blck -> HEIGHT;
	blck -> HEIGHT	=	blck -> WIDTH;
	blck -> WIDTH	=	temp;
	
	for(row	=	0; row < blck -> HEIGHT; row++){
		for(column	=	0; column < blck -> WIDTH; column++){
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].IsBlock	=	mezipole[row][column];
			
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].Color	=	mezipole2[row][column];
		}
	}
	
	bool canrotate	=	true;
	
	for(row	=	blck -> POS.Y; row > blck -> POS.Y - blck -> HEIGHT && canrotate; row--){
		for(column	=	blck -> POS.X; column < blck -> POS.X + blck -> WIDTH && canrotate; column++){
			if(brd -> Layers[row][column][0].IsBlock && brd -> Layers[row][column][1].IsBlock){
				canrotate	=	false;
			}
		}
	}
	
	if(!canrotate) RotateRight(brd, blck);
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

void RotateRight(BOARD *brd, BLOCKPLUS *blck){
	
	if(blck -> POS.X + blck -> HEIGHT > brd -> WIDTH - 1) return;
	if(blck -> POS.Y - blck -> WIDTH + 1 < 0) return;
	
	bool mezipole[blck -> WIDTH][blck -> HEIGHT]	=	{false};
	unsigned char mezipole2[blck -> WIDTH][blck -> HEIGHT]	=	{0};
						
	for(row	=	0; row < blck -> HEIGHT; row++){
		for(column	=	0; column < blck -> WIDTH; column++){
			mezipole[column][blck -> HEIGHT - 1 - row]	=	brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].IsBlock;
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].IsBlock	=	false;
			
			mezipole2[column][blck -> HEIGHT - 1 - row]	=	brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].Color;
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].Color	=	DARKGRAY;
		}
	}
						
	temp	=	blck -> HEIGHT;
	blck -> HEIGHT	=	blck -> WIDTH;
	blck -> WIDTH	=	temp;
	
	for(row	=	0; row < blck -> HEIGHT; row++){
		for(column	=	0; column < blck -> WIDTH; column++){
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].IsBlock	=	mezipole[row][column];
			
			brd -> Layers[blck -> POS.Y - blck -> HEIGHT + 1 + row][blck -> POS.X + column][1].Color	=	mezipole2[row][column];
		}
	}
	
	bool canrotate	=	true;
	
	for(row	=	blck -> POS.Y; row > blck -> POS.Y - blck -> HEIGHT && canrotate; row--){
		for(column	=	blck -> POS.X; column < blck -> POS.X + blck -> WIDTH && canrotate; column++){
			if(brd -> Layers[row][column][0].IsBlock && brd -> Layers[row][column][1].IsBlock){
				canrotate	=	false;
			}
		}
	}
	
	if(!canrotate) RotateLeft(brd, blck);
}

bool CollisionCheck(BOARD *brd, BLOCKPLUS *blck){
	
	if(blck -> POS.Y >= brd -> HEIGHT - 1){
		return true;
	}
	
	for(row	=	0; row < blck -> HEIGHT && !collided; row++){
		for(column	=	0; column < blck -> WIDTH && !collided; column++){
			if(brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].IsBlock && brd -> Layers[blck -> POS.Y - row + 1][blck -> POS.X + column][0].IsBlock){
				return true;
			}
		}
	}
	
	return false;
}

void MoveDown(BOARD *brd, BLOCKPLUS *blck){
	blck -> POS.Y++;
				
	for(row	=	0; row < blck -> HEIGHT; row++){
		for(column	=	0; column < blck -> WIDTH; column++){
			brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].IsBlock	=	brd -> Layers[blck -> POS.Y - row - 1][blck -> POS.X + column][1].IsBlock;
			brd -> Layers[blck -> POS.Y - row - 1][blck -> POS.X + column][1].IsBlock	=	false;
			
			brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].Color	=	brd -> Layers[blck -> POS.Y - row - 1][blck -> POS.X + column][1].Color;
			brd -> Layers[blck -> POS.Y - row - 1][blck -> POS.X + column][1].Color	=	DARKGRAY;
		}
	}
}

void BoardWrite(BOARD *brd, BLOCKPLUS *blck, bool _physicschain){
	for(row	=	0; row < blck -> HEIGHT; row++){
		for(column	=	0; column < blck -> WIDTH; column++){
			if(brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].IsBlock){
				brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][0].IsBlock	=	brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].IsBlock;
				brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][0].Color	=	brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].Color;
			}
			
			brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].IsBlock	=	false;
			brd -> Layers[blck -> POS.Y - row][blck -> POS.X + column][1].Color	=	DARKGRAY;
		}
	}
	
	if (!_physicschain) FullRowDetection(brd, blck);
	else if(_physicschain) FullRowDetectionChain(brd, blck);
	
	if(page.GAME){
		if(mods.TIMERUN == -1) Endless(brd);
		else gameover	=	EndGame(brd);
	}
	
}

void FullRowDetection(BOARD *brd, BLOCKPLUS *blck){
	
	int rowscleared	=	0;
	
	for(row	=	0; row <= blck -> POS.Y; row++){
		
		int get	=	0;
		
		for(int i	=	1; i < brd -> WIDTH - 1; i++){
			if(brd -> Layers[row][i][0].IsBlock) get++;
		}
		
		if(get == brd -> WIDTH - 2){
			for(column	=	0; column < brd -> WIDTH; column++) brd -> Layers[row][column][0].IsBlock	=	false;
			
			rowscleared++;
			
			for(int i	=	row; i > 0; i--){
				for(int j	=	0; j < brd -> WIDTH; j++){
					brd -> Layers[i][j][0].IsBlock	=	brd -> Layers[i - 1][j][0].IsBlock;
					brd -> Layers[i - 1][j][0].IsBlock	=	false;
					
					brd -> Layers[i][j][0].Color	=	brd -> Layers[i - 1][j][0].Color;
					brd -> Layers[i - 1][j][0].Color	=	DARKGRAY;
				}
			}
		}
	}
	
	if(rowscleared != 0 && page.GAME){
		
		score += (rowscleared == 1 ? 40 : (rowscleared == 2 ? 100 : (rowscleared == 3 ? 300 : 1200))) * level * mods.TIMEFACTOR * BoardScoreRatio(mods.BOARDSIZE);
		
		thislevelclear -= rowscleared;
		
		if(thislevelclear <= 0){
			level++;
			thislevelclear += BASECLEAR + level;
		}
		
		rowscleared	=	0;
	}
}

void FullRowDetectionChain(BOARD *brd, BLOCKPLUS *blck){
	
	bool done	=	true;
	int rowscleared	=	0;
	
	do{
		bool doit	=	false;
		done	=	true;
		
		for(row	=	0; row < brd -> HEIGHT; row++){
			
			int get	=	0;
		
			for(int i	=	1; i < brd -> WIDTH - 1; i++){
				if(brd -> Layers[row][i][0].IsBlock) get++;
			}
			
			if(get == brd -> WIDTH - 2){
				for(column	=	0; column < brd -> WIDTH; column++) brd -> Layers[row][column][0].IsBlock	=	false;
				
				rowscleared++;
				
				doit	=	true;
				
				for(int i	=	row; i > 0; i--){
					for(int j	=	0; j < brd -> WIDTH; j++){
						brd -> Layers[i][j][0].IsBlock	=	brd -> Layers[i - 1][j][0].IsBlock;
						brd -> Layers[i - 1][j][0].IsBlock	=	false;
						
						brd -> Layers[i][j][0].Color	=	brd -> Layers[i - 1][j][0].Color;
						brd -> Layers[i - 1][j][0].Color	=	DARKGRAY;
					}
				}
				
				
			}
		}
		
		
		if(doit){
			for(row	=	brd -> HEIGHT - 1; row >= 0; row--){
				for(column	=	0; column < brd -> WIDTH - 2; column++){
					while(brd -> Layers[row][column + 1][0].IsBlock && !brd -> Layers[row + 1][column + 1][0].IsBlock && row < brd -> HEIGHT){
						brd -> Layers[row + 1][column + 1][0].IsBlock	=	brd -> Layers[row][column + 1][0].IsBlock;
						brd -> Layers[row][column + 1][0].IsBlock	=	false;
						
						brd -> Layers[row + 1][column + 1][0].Color	=	brd -> Layers[row][column + 1][0].Color;
						brd -> Layers[row][column + 1][0].Color	=	DARKGRAY;
						
						row++;
					}
				}
				Sleep(10);
			}
		}
		
		
		
		
		for(row	=	0; row < brd -> HEIGHT; row++){
			
			int get	=	0;
		
			for(int i	=	1; i < brd -> WIDTH - 1; i++){
				if(brd -> Layers[row][i][0].IsBlock) get++;
			}
			
			if(get == brd -> WIDTH - 2){
				done	=	false;
			}
		}
		
	}while(!done);
	
	
	if(rowscleared != 0 && page.GAME){
		
		score += (rowscleared == 1 ? 40 : (rowscleared == 2 ? 100 : (rowscleared == 3 ? 300 : 1200))) * level * mods.TIMEFACTOR * BoardScoreRatio(mods.BOARDSIZE);
		
		thislevelclear -= rowscleared;
		
		if(thislevelclear <= 0){
			level++;
			thislevelclear += BASECLEAR + level;
		}
		
		rowscleared	=	0;
	}
	
}

void BuildGame(BOARD *brd){
	
	for(row	=	0; row < LARGE_H + 1; row++){
		for(column	=	0; column < LARGE_W + 1; column++){
			board.Layers[row][column][0].IsBlock	=	false;
			board.Layers[row][column][1].IsBlock	=	false;
			board.Layers[row][column][2].IsBlock	=	false;
			
			board.Layers[row][column][0].Color	=	DARKGRAY;
			board.Layers[row][column][1].Color	=	DARKGRAY;
			board.Layers[row][column][2].Color	=	DARKGRAY;
		}
	}
	
	delay	=	50 / mods.TIMEFACTOR;
	
	if(mods.TIMEFACTOR > 0) HANDLE thread4	=	CreateThread(NULL, 0, Timer, NULL, 0, NULL);
	if(mods.GHOSTBLOCKS) HANDLE thread5	=	CreateThread(NULL, 0, TimerGB, NULL, 0, NULL);
	
	if(mods.BOARDFULL != 0){
		FillBoard(&board);
	}
	
	block.NEXT	=	Random(1, BLOCKS);
	
	score	=	0;
	level	=	1;
	thislevelclear	=	BASECLEAR;
}

int Digits(int n){
	if (n < 0) n	=	(n == INT_MIN) ? INT_MAX : -n;
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

void Box(int _height, int _width, int X, int Y, int _fg, int _bg){
	for(row	=	0; row < _height; row++){
		for(column	=	0; column < _width; column++){
			crmove(X + column, Y + row);
			textcolor(_fg, _bg);
			if(row == 0 && column == 0) puts("\xC9");
			else if(row == _height - 1 && column == 0) puts("\xC8");
			else if(row == 0 && column == _width - 1) puts("\xBB");
			else if(row == _height - 1 && column == _width - 1) puts("\xBC");
			else if((row == 0 || row == _height - 1) && (column != 0 || column != _width - 1)) puts("\xCD");
			else if((row != 0 || row != _height - 1) && (column == 0 || column == _width - 1)) puts("\xBA");
			else puts(" ");
			
		}
	}
}

void GetBlockSizes(){
	
	int start = 0, stop = 0;
	
	for(row = 0; row < 3; row++){
		
		for(int row_2 = 0; row_2 < 2; row_2++){
			if(customblocks[row].Layout[row_2][0].IsBlock || customblocks[row].Layout[row_2][1].IsBlock || customblocks[row].Layout[row_2][2].IsBlock || customblocks[row].Layout[row_2][3].IsBlock){
				if(start == 0) start = column;
				else stop = column;
			}
		}
		
		customblocks[row].Height = stop - start + 1;
		
		start = 0;
		stop = 0;
		
		for(column = 0; column < 4; column++){
			if(customblocks[row].Layout[0][column].IsBlock || customblocks[row].Layout[1][column].IsBlock){
				if(start == 0) start = column;
				else stop = column;
			}
		}
		
		customblocks[row].Width = stop - start + 1;
		
	}
	
}

void SoundPlayer(){
	HANDLE thread3	=	CreateThread(NULL, 0, MusicPlay, NULL, 0, NULL);
}

void OnLoad(){
	srand(time(NULL));
	
	LoadSettings();
	LoadKeys();
	LoadHighscores();
	LoadSounds();
	LoadBlocks();
	
	sprintf(name, "Tetris v%s by %s", version, author);
	sprintf(info_txt[0], "Copyright (c) %s %s", year, author);
	sprintf(info_txt[1], "Version %s", version);
	sprintf(info_txt[2], "%s/%s/%s", month, day, year);
	
	HANDLE thread1	=	CreateThread(NULL, 0, WindowResizedEvent, NULL, 0, NULL);
	HANDLE thread2	=	CreateThread(NULL, 0, RenderScreen, NULL, 0, NULL);
	
	if(settings.sound) SoundPlayer();
	
	setcursortype(_NOCURSOR);
	SetConsoleTitle(name);
	SetWindow(80,26);
	SetFont(12, 16);
	
	screen.MAINMENU.Y	=	17;
	screen.SETTINGS.Y	=	13;
	screen.INFO.Y	=	13;
	screen.GAME.BOARD.Y	=	2;
	screen.GAME.BOARD.X	=	10;
	screen.GAME.CURBLOCK.Y	=	3;
	screen.GAME.CURBLOCK.X	=	55;
	screen.GAME.PAUSE.Y	=	16;
	screen.GAME.PAUSE.X	=	55;
	screen.EXIT.Y	=	10;
	screen.GAME.GAMETYPE.Y	=	14;
	screen.GAME.MUTATOR.Y	=	10;
	
	screen.EXITSIZE.X	=	80;
	screen.EXITSIZE.Y	=	26;
	screen.GAMESIZE.X	=	80;
	screen.GAMESIZE.Y	=	34;
	screen.INFOSIZE.X	=	80;
	screen.INFOSIZE.Y	=	30;
	screen.MMSIZE.X	=	80;
	screen.MMSIZE.Y	=	26;
	screen.SETTSIZE.X	=	80;
	screen.SETTSIZE.Y	=	26;
	screen.KBINDSIZE.X	=	80;
	screen.KBINDSIZE.Y	=	35;
	
	
	selection.EXIT	=	0;
	selection.INFO	=	0;
	selection.KEYS	=	9;
	selection.MAINMENU	=	0;
	selection.PAUSE	=	0;
	selection.SETTINGS	=	0;
	selection.GAMETYPE	=	1;
	
	screen.TETRIS	=	3;
	
	help.HEIGHT	=	SMALL_H;
	help.WIDTH	=	SMALL_W;
	
	keys.PRESSED	=	NULL;
}

void HighScore(){
	
	last_width	=	screen.MMSIZE.X;;
	last_height	=	screen.MMSIZE.Y;
	
	page.HIGHSCORE	=	true;
	page.PAGELOAD	=	true;
	
	while(true){
		
		GetKey();
		
		if(keys.PRESSED == 13){
			page.HIGHSCORE	=	false;
			keys.PRESSED	=	0;
			
			return;
		}
	}
}

void Options(){
	
	page.SETTINGS	=	true;
	page.PAGELOAD	=	true;
	
	last_width	=	screen.SETTSIZE.X;
	last_height	=	screen.SETTSIZE.Y;
	
	selection.SETTINGS	=	0;
	
	while(true){
		
		GetKey();
		
		if((keys.PRESSED == keys.KEY[UP] || keys.PRESSED == 'w') && selection.SETTINGS > 0){
			selection.SETTINGS--;
			keys.PRESSED	=	0;
			
			if(selection.SETTINGS == 1 && !soundavailable) selection.SETTINGS--;
		}
		
		else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && selection.SETTINGS < 5){
			selection.SETTINGS++;
			keys.PRESSED	=	0;
			
			if(selection.SETTINGS == 1 && !soundavailable) selection.SETTINGS++;
		}
		
		else if(keys.PRESSED == 13){
			
			keys.PRESSED	=	0;

			if(selection.SETTINGS == 0){
				
				last_width	=	screen.KBINDSIZE.X;
				last_height	=	screen.KBINDSIZE.Y;
				
				page.KEYS	=	true;
				page.PAGELOAD	=	true;
				
				selection.KEYS	=	9;
				
				while(page.KEYS){
					
					GetKey();
					
					if((keys.PRESSED == 'w' || keys.PRESSED == keys.KEY[UP]) && selection.KEYS > 0){
						selection.KEYS--;
						keys.PRESSED	=	0;
					}
					
					else if((keys.PRESSED == 's' || keys.PRESSED == keys.KEY[DOWN]) && selection.KEYS < 9){
						selection.KEYS++;
						keys.PRESSED	=	0;
					}
					
					else if(keys.PRESSED == 13){
						
						keys.PRESSED	=	0;
						
						if(selection.KEYS != 9){
							
							keychanging	=	true;
							
							GetKey();
							
							keys.SAMEKEYEXCEPTION	=	false;
							
							if(keys.PRESSED != DEL && !KeyBindingChecker()){
								if(selection.KEYS == 0) keys.KEY[UP]	=	keys.PRESSED;
								else if(selection.KEYS == 1) keys.KEY[DOWN]	=	keys.PRESSED;
								else if(selection.KEYS == 2) keys.KEY[LEFT]	=	keys.PRESSED;
								else if(selection.KEYS == 3) keys.KEY[RIGHT]	=	keys.PRESSED;
								else if(selection.KEYS == 4) keys.KEY[ROTATE_L]	=	keys.PRESSED;
								else if(selection.KEYS == 5) keys.KEY[ROTATE_R]	=	keys.PRESSED;
								else if(selection.KEYS == 6) keys.KEY[DROP]	=	keys.PRESSED;
								else if(selection.KEYS == 7) keys.KEY[PAUSE]	=	keys.PRESSED;
								else if(selection.KEYS == 8) keys.KEY[NEXTSONG]	=	keys.PRESSED;
								
								SaveKeys();
							}
							else if(KeyBindingChecker()){
								keys.SAMEKEYEXCEPTION	=	true;
							}
							
							keychanging	=	false;
						}
						
						else if(selection.KEYS == 9){
							
							page.KEYS	=	false;
							
						}
					}
				}
				
				last_width	=	screen.SETTSIZE.X;
				last_height	=	screen.SETTSIZE.Y;
				
				page.PAGELOAD	=	true;
			}
			
			else if(selection.SETTINGS == 1 && soundavailable){
				settings.sound	=	!settings.sound;
				
				if(settings.sound) SoundPlayer();
				
				SaveSettings();
			}
			else if(selection.SETTINGS == 2){
				settings.drophelp	=	!settings.drophelp;
				SaveSettings();
			}
			else if(selection.SETTINGS == 3){
				HighScoreReset();
			}
			else if(selection.SETTINGS == 4){
				Defaults();
			}
			else if(selection.SETTINGS == 5){
				page.SETTINGS	=	false;
				return;
			}
			
			
		}
	}
}

void Info(){
	
	page.INFO	=	true;
	page.PAGELOAD	=	true;
	
	last_width	=	screen.INFOSIZE.X;
	last_height	=	screen.INFOSIZE.Y;
	
	selection.INFO	=	0;
	
	int timer	=	0;
	
	while(true){
		
		GetKey();
	
		if((keys.PRESSED == keys.KEY[UP] || keys.PRESSED == 'w') && selection.INFO > 0){
			selection.INFO--;
			keys.PRESSED	=	0;
		}
		
		else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && selection.INFO < 3){
			selection.INFO++;
			keys.PRESSED	=	0;
		}
		
		else if(keys.PRESSED == 13){
			
			keys.PRESSED	=	0;
			
			if(selection.INFO == 0){
				
				page.HELP	=	true;
				page.PAGELOAD	=	true;
				
				selection.HELPPAGE	=	0;
				
				while(page.HELP){
					
					if(timer == 0){
						
						for(row	=	0; row < SMALL_H; row++){
							for(column	=	0; column < SMALL_W; column++){
								help.Layers[row][column][0].IsBlock	=	false;
								help.Layers[row][column][1].IsBlock	=	false;
								
								help.Layers[row][column][0].Color	=	DARKGRAY;
								help.Layers[row][column][1].Color	=	DARKGRAY;
							}
						}
					}
					
					if(selection.HELPPAGE == 0 || selection.HELPPAGE == 1){
						if(timer == 0){
							CurBlock(Random(1, BLOCKS), 6, &help, &blockhelp);
						}
						
						else if(timer % 100 == 20){ RotateLeft(&help, &blockhelp); }
						else if(timer % 100 == 40){ MoveLeft(&help, &blockhelp); }
						else if(timer % 100 == 60){ RotateRight(&help, &blockhelp); }
						else if(timer % 100 == 80){ MoveRight(&help, &blockhelp); }
					}
					
					else if(selection.HELPPAGE == 2 || selection.HELPPAGE == 3 || selection.HELPPAGE == 4){
						
						if(timer == 0){
							bool brd_preset[][SMALL_W]	=	{
								{ TR, FL, TR, FL, FL, FL, FL, FL, FL, FL, TR, FL, FL, TR },
								{ TR, TR, TR, FL, FL, TR, TR, TR, FL, FL, TR, FL, TR, TR },
								{ TR, TR, TR, TR, TR, TR, TR, TR, FL, FL, FL, FL, TR, FL },
								{ TR, FL ,FL ,FL ,TR, TR, TR, FL, FL, FL, TR, TR, TR, FL },
								{ TR, TR, TR, TR, TR, TR, TR, TR, FL, TR, TR, TR, TR, TR }
							};
							
							for(row	=	0; row < 5; row++){
								for(column	=	0; column < SMALL_W; column++){
									
									help.Layers[13 + row][column][0].IsBlock	=	brd_preset[row][column];
									
									if(brd_preset[row][column]){
										int blockis	=	Random(0, 15);
										while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis	=	Random(0, 15);
										
										help.Layers[13 + row][column][0].Color	=	blockis;
									}
								}
							}
							
							CurBlock(I + 1, 2, &help, &blockhelp);
						}
						
						else if(timer == 20){ MoveRight(&help, &blockhelp); }
						else if(timer == 25){ MoveRight(&help, &blockhelp); }
						else if(timer == 30){ MoveDown(&help, &blockhelp); }
						else if(timer == 35){ MoveDown(&help, &blockhelp); }
						else if(timer == 40){ MoveRight(&help, &blockhelp); }
						else if(timer == 45){ RotateLeft(&help, &blockhelp); }
						else if(timer == 50 && selection.HELPPAGE == 4) { AddRow(&help); }
						else if(timer == 55){
							while(!collidedhelp){
								collidedhelp	=	CollisionCheck(&help, &blockhelp);
								if(!collidedhelp) MoveDown(&help, &blockhelp);
								Sleep(20);
							}
							collidedhelp	=	false;
							BoardWrite(&help, &blockhelp, selection.HELPPAGE == 3 ? true : false);
						}
						
						else if(timer == 80) timer	=	-1;
						
					}
					
					else if(selection.HELPPAGE == 5){
						if(timer == 0){
							bool brd_preset[][SMALL_W]	=	{
								{ TR, FL, TR, FL, FL, FL, FL, FL, FL, FL, TR, FL, FL, TR },
								{ TR, TR, TR, FL, FL, TR, TR, TR, FL, FL, TR, FL, TR, TR },
								{ TR, TR, TR, TR, TR, TR, TR, TR, FL, FL, FL, FL, TR, FL },
								{ TR, FL ,FL ,FL ,TR, TR, TR, FL, FL, FL, TR, TR, TR, FL },
								{ TR, TR, TR, TR, TR, TR, TR, TR, FL, TR, TR, TR, TR, TR }
							};
							
							for(row	=	0; row < 5; row++){
								for(column	=	0; column < SMALL_W; column++){
									
									help.Layers[13 + row][column][0].IsBlock	=	brd_preset[row][column];
									
									if(brd_preset[row][column]){
										int blockis	=	Random(0, 15);
										while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis	=	Random(0, 15);
										
										help.Layers[13 + row][column][0].Color	=	blockis;
									}
								}
							}
							
							CurBlock(T + 1, 2, &help, &blockhelp);
						}
						
						else if(timer == 20){ MoveRight(&help, &blockhelp); }
						else if(timer == 25){ MoveRight(&help, &blockhelp); }
						else if(timer == 30){ MoveRight(&help, &blockhelp); }
						else if(timer == 40){ RotateRight(&help, &blockhelp); }
						
						else if(timer == 55){
							while(!collidedhelp){
								collidedhelp	=	CollisionCheck(&help, &blockhelp);
								if(!collidedhelp) MoveDown(&help, &blockhelp);
								Sleep(20);
							}
							collidedhelp	=	false;
						}
						else if(timer == 60){ BoardWrite(&help, &blockhelp, false); }
						
						else if(timer == 90) timer	=	-1;
					}
					
					else if(selection.HELPPAGE == 6){
						if(timer == 0){
							for(row	=	0; row < 8; row++){
								for(column	=	0; column < SMALL_W; column++){
									
									help.Layers[SMALL_H - 1 - row][column][0].IsBlock	=	Random(0, 1) == 0 ? false : true;
									
									if(help.Layers[SMALL_H - 1 - row][column][0].IsBlock){
										int blockis	=	Random(0, 15);
										while(blockis != BLUE && blockis != GREEN && blockis != CYAN && blockis != RED && blockis != MAGENTA && blockis != LIGHTBLUE && blockis != LIGHTGREEN) blockis	=	Random(0, 15);
										
										help.Layers[SMALL_H - 1 - row][column][0].Color	=	blockis;
									}
								}
							}
							
							CurBlock(Random(1, BLOCKS), 3, &help, &blockhelp);
						}
						
						else if(timer % 100 == 20){ RotateLeft(&help, &blockhelp); }
						else if(timer % 100 == 40){ MoveLeft(&help, &blockhelp); }
						else if(timer % 100 == 60){ RotateRight(&help, &blockhelp); }
						else if(timer % 100 == 80){ MoveRight(&help, &blockhelp); }
						
					}
					
					timer++;
					
					if(_kbhit()){
						
						GetKey();
						
						if((keys.PRESSED == 'w' || keys.PRESSED == keys.KEY[UP]) && selection.HELP > 0){
							selection.HELP--;
							keys.PRESSED	=	0;
							page.PAGELOAD	=	true;
						}
						
						else if((keys.PRESSED == 's' || keys.PRESSED == keys.KEY[DOWN]) && selection.HELP < 1){
							selection.HELP++;
							keys.PRESSED	=	0;
							page.PAGELOAD	=	true;
						}
						
						else if((keys.PRESSED == 'a' || keys.PRESSED == keys.KEY[LEFT]) && selection.HELP == 0){
							selection.HELPPAGE--;
							if(selection.HELPPAGE < 0) selection.HELPPAGE	=	7;
							timer	=	0;
							
							keys.PRESSED	=	0;
							page.PAGELOAD	=	true;
						}
						
						else if((keys.PRESSED == 'd' || keys.PRESSED == keys.KEY[RIGHT]) && selection.HELP == 0){
							selection.HELPPAGE++;
							if(selection.HELPPAGE > 7) selection.HELPPAGE	=	0;
							timer	=	0;
							
							keys.PRESSED	=	0;
							page.PAGELOAD	=	true;
						}
						
						else if(selection.HELP == 1 && keys.PRESSED == 13){
							page.HELP	=	false;
							keys.PRESSED	=	0;
						}
						
					}
					
					Sleep(50);
				}
				
				page.PAGELOAD	=	true;
			}
			
			else if(selection.INFO == 1){
				ShellExecute(NULL, "open", "https://www.youtube.com/playlist?list=PL2b_6zoPWVIqE_Xozc4-mSrq75fQu56pw", NULL, NULL, SW_SHOWNORMAL);
			}
			
			else if(selection.INFO == 2){
				ChangeLog();
				page.PAGELOAD = true;
			}
			
			else if(selection.INFO == 3){
				page.INFO	=	false;
				return;
			}
			
		}
	}
}

bool Quit(){
	
	page.EXIT	=	true;
	page.PAGELOAD	=	true;
	
	last_width	=	screen.EXITSIZE.X;
	last_height	=	screen.EXITSIZE.Y;
	
	exit_app	=	false;
	
	while(true){
		
		GetKey();
		
		if(keys.PRESSED == keys.KEY[LEFT] || keys.PRESSED == 'a'){
			exit_app	=	true;
			keys.PRESSED	=	0;
		}
		
		if(keys.PRESSED == keys.KEY[RIGHT] || keys.PRESSED == 'd'){
			exit_app	=	false;
			keys.PRESSED	=	0;
		}
		
		if(keys.PRESSED == 13){
			page.EXIT	=	false;
			keys.PRESSED	=	0;
			
			return exit_app;
		}
	}
		
}

bool Pause(){
	
	page.PAUSE	=	true;
	page.GAMELOAD	=	true;
	
	selection.PAUSE	=	0;
	
	while(true){
		
		GetKey();
		
		if((keys.PRESSED == 'w' || keys.PRESSED == keys.KEY[UP]) && selection.PAUSE > 0){
			selection.PAUSE--;
			keys.PRESSED	=	0;
		}
		
		else if((keys.PRESSED == 's' || keys.PRESSED == keys.KEY[DOWN]) && selection.PAUSE < 4){
			selection.PAUSE++;
			keys.PRESSED	=	0;
		}
		
		else if(keys.PRESSED == 13){
			
			if(selection.PAUSE == 0){
				page.PAUSE	=	false;
				keys.PRESSED	=	0;
				
				return false;
			} 
			else if(selection.PAUSE == 1) {
				ResetGame();
				
				if(reset){
					page.PAUSE	=	false;
					
					return false;
				}
			}
			else if(selection.PAUSE == 2) { page.PAUSE	=	false; Options(); }
			else if(selection.PAUSE == 3) { page.PAUSE	=	false; Info(); } 
			else if(selection.PAUSE == 4) {
				
				page.PAUSERETURN	=	true;
				
				qt_select	=	false;
				
				while(page.PAUSERETURN){

					GetKey();
					
					if(keys.PRESSED == 'a' || keys.PRESSED == keys.KEY[LEFT]){
						keys.PRESSED	=	0;
						qt_select	=	true;
					} 
					else if(keys.PRESSED == 'd' || keys.PRESSED == keys.KEY[RIGHT]){
						keys.PRESSED	=	0;
						qt_select	=	false;
					} 
					
					else if(keys.PRESSED == 13){
						
						page.PAUSERETURN	=	false;
						keys.PRESSED	=	0;
						
						if(qt_select){
							page.PAUSE	=	false;
							
							return qt_select;
						}
					}
				}
				
			}
			
			last_width	=	screen.GAMESIZE.X;
			last_height	=	screen.GAMESIZE.Y;
			
			page.PAUSE	=	true;
			page.GAMELOAD	=	true;
			
		}
	}
	
}

void GameOver(){
	
	namecount	=	0;
		
	page.GAMEOVER	=	true;
	page.GONAME	=	true;
	page.PAGELOAD	=	true;
	
	do{
		GetKey();
		
		bool ischar	=	false;
		
		switch(keys.PRESSED){
			case 'a' ... 'z':
			case 'A' ... 'Z':
			case '0' ... '9':
				ischar	=	true;
				break;
		}
		
		if(ischar && namecount < 3){
			player[namecount++]	=	keys.PRESSED;
			keys.PRESSED == 0;
		}
		
		else if(keys.PRESSED == 8 && namecount > 0){
			keys.PRESSED	=	0;
			namecount--;
		} 
		else if(keys.PRESSED == 13 && namecount == 3){
			keys.PRESSED	=	0;
			break;
		}
		
	} while(true);
	
	if(highscore.index < 10){
		highscore.scores[highscore.index]	=	score;
		strcpy(highscore.players[highscore.index], player);
		
		highscore.index++;
	}
	
	else{
		if(score > highscore.scores[9]){
			highscore.scores[9]	=	score;
			strcpy(highscore.players[9], player);
		}
	}
	
	
	for(column	=	0; column < highscore.index; column++){
		for(row	=	0; row < highscore.index; row++){
			if(highscore.scores[row] < highscore.scores[row + 1]){
				int temp	=	highscore.scores[row + 1];
				highscore.scores[row + 1]	=	highscore.scores[row];
				highscore.scores[row]	=	temp;
				
				char tempchr[3];
				strcpy(tempchr, highscore.players[row + 1]);
				strcpy(highscore.players[row + 1], highscore.players[row]);
				strcpy(highscore.players[row], tempchr);
			}
		}
	}
	
	SaveHighscore();
	
	page.GONAME	=	false;
	page.PAGELOAD	=	true;
	
	do{
		GetKey();
		
		if(keys.PRESSED == 13){
			keys.PRESSED	=	0;
			page.GAMEOVER	=	false;
			gameover	=	false;
			
			return;
		}
		
	} while(true);
}

void ResetGame(){
	
	page.RESETGAME	=	true;
	
	reset	=	false;
	
	while(true){
		GetKey();
		
		
		if(keys.PRESSED == keys.KEY[LEFT] || keys.PRESSED == 'a'){
			keys.PRESSED	=	0;
			reset	=	true;
		}
		
		else if(keys.PRESSED == keys.KEY[RIGHT] || keys.PRESSED == 'd'){
			keys.PRESSED	=	0;
			reset	=	true;
		}
		
		else if(keys.PRESSED == 13){
			if(reset){
				
				for(row	=	0; row < LARGE_H; row++){
					for(column	=	0; column < LARGE_W; column++){
						board.Layers[row][column][0].IsBlock	=	false;
						board.Layers[row][column][0].Color	=	DARKGRAY;
						
						board.Layers[row][column][1].IsBlock	=	false;
						board.Layers[row][column][1].Color	=	DARKGRAY;
						
						board.Layers[row][column][2].IsBlock	=	false;
						board.Layers[row][column][2].Color	=	DARKGRAY;
						
					}
				}
				
				block	=	reset_block;
				
				score	=	0;
				timeleft	=	0;
				
				page.RESETGAME	=	false;
				
				return;
			}
			else if(!reset){
				
				page.RESETGAME	=	false;
				
				return;
			}
		}
		
	}
	
}

void BlockCreation(){
	
	page.BLOCKCREATION	=	true;
	page.PAGELOAD	=	true;
	
	last_width	=	screen.MMSIZE.X;
	last_height	=	screen.MMSIZE.Y;
	
	selection.BLOCKSEL	=	0;
	selection.BLOCKCREATEX	=	5;
	selection.BLOCKCREATEY	=	0;
	
	
	while(true){
		
		GetKey();
		
		if(keys.PRESSED == keys.KEY[UP] || keys.PRESSED == 'w'){
			keys.PRESSED	=	0;
			if(selection.BLOCKCREATEX > 3 && selection.BLOCKSEL > 0)selection.BLOCKSEL--;
			else if(selection.BLOCKCREATEX < 4 && selection.BLOCKCREATEY > 0)selection.BLOCKCREATEY--;
			
			page.PAGELOAD	=	true;
		}
		
		else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && selection.BLOCKSEL < 3){
			keys.PRESSED	=	0;
			if(selection.BLOCKCREATEX == 5 && selection.BLOCKSEL < 3)selection.BLOCKSEL++;
			else if(selection.BLOCKCREATEX < 4 && selection.BLOCKCREATEY < 1)selection.BLOCKCREATEY++;
			
			page.PAGELOAD	=	true;
		}
		
		else if((keys.PRESSED == keys.KEY[LEFT] || keys.PRESSED == 'a') && selection.BLOCKCREATEX > 0){
			keys.PRESSED	=	0;
			if(selection.BLOCKSEL != 3)selection.BLOCKCREATEX--;
			
			page.PAGELOAD	=	true;
		}
		
		else if((keys.PRESSED == keys.KEY[RIGHT] || keys.PRESSED == 'd') && selection.BLOCKCREATEX < 5){
			keys.PRESSED	=	0;
			selection.BLOCKCREATEX++;
			
			page.PAGELOAD	=	true;
		}
		
		else if(keys.PRESSED == 13){
			
			if(selection.BLOCKCREATEX < 4 && selection.BLOCKSEL < 3){
				
				if(customblocks[selection.BLOCKSEL].Left > 0 || customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].IsBlock){
					customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].IsBlock = !customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].IsBlock;
					
					if(customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].IsBlock){
						customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].Color = WHITE;
						customblocks[selection.BLOCKSEL].Left--;
					}
					else customblocks[selection.BLOCKSEL].Left++;
				} 
				
			}
			else if(selection.BLOCKCREATEX == 4 && selection.BLOCKSEL < 3){
				
				bool IsAvailable = false;
				
				for(row = 0; row < 2 && !IsAvailable; row++){
					for(column = 0; column < 4 && !IsAvailable; column++){
						if(customblocks[selection.BLOCKSEL].Layout[row][column].IsBlock) IsAvailable = true;
					}
				}
				
				if(IsAvailable) customblocks[selection.BLOCKSEL].IsActive = true;
				else customblocks[selection.BLOCKSEL].IsActive = false;
			}
			else if(selection.BLOCKCREATEX == 5 && selection.BLOCKSEL == 3){
				SaveBlocks();
				GetBlockSizes();
				
				page.BLOCKCREATION	=	false;
				
				return;
			}
			
			page.PAGELOAD	=	true;
			
		}
		
		else if(keys.PRESSED == SPACE_K){
			if(selection.BLOCKCREATEX < 4){
				customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].Color++;
				if(customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].Color > WHITE) customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].Color	=	BLACK + 1;
				if(customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].Color == LIGHTGRAY) customblocks[selection.BLOCKSEL].Layout[selection.BLOCKCREATEY][selection.BLOCKCREATEX].Color += 2;
			}
			
			page.PAGELOAD	=	true;
		}
	}
}

void ChangeLog(){
	
	page.CHANGELOG = true;
	page.PAGELOAD = true;
	
	while(true){
		
		GetKey();
		
		if((keys.PRESSED == keys.KEY[UP] || keys.PRESSED == 'w') && selection.CHANGELOG > 0){
			selection.CHANGELOG--;
			page.PAGELOAD = true;
		}
		
		else if((keys.PRESSED == keys.KEY[DOWN] || keys.PRESSED == 's') && selection.CHANGELOG + 20 < changelog_rows){
			selection.CHANGELOG++;
			page.PAGELOAD = true;
		}
		
		else if(keys.PRESSED == 13){
			page.CHANGELOG = false;
			return;
		}
	}
}

void GetKey(){
	keys.PRESSED	=	_getch();
		
	if(keys.PRESSED == 224){
		keys.PRESSED += _getch();
	}
	
	if(keys.PRESSED == 0){
		keys.PRESSED -= _getch();
	}
	
	while(_kbhit()) _getch();
}

bool KeyBindingChecker(){
	
	int temp	=	0;
	
	for(int i	=	0; i < NUMBEROFTYPES; i++) if(keys.PRESSED == keys.KEY[i]) temp++;
	
	if(temp == 0) return false;
	else if(keys.PRESSED == keys.KEY[selection.KEYS]) return false;
	else if(selection.KEYS == 0 && keys.PRESSED == 'w') return false;
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

void LoadKeys(){
	
	FILE *fp;
	
	fp	=	fopen("keys.wat","r");
	
	if(fp == NULL){
		
		keys.KEY[UP]	=	'w';
		keys.KEY[DOWN]	=	's';
		keys.KEY[LEFT]	=	'a';
		keys.KEY[RIGHT]	=	'd';
		keys.KEY[ROTATE_L]	=	'q';
		keys.KEY[ROTATE_R]	=	'e';
		keys.KEY[NEXTSONG]	=	'n';
		keys.KEY[DROP]	=	SPACE_K;
		keys.KEY[PAUSE]	=	ESC;
		
		fp	=	fopen("keys.wat","w");
		fwrite(&keys, sizeof(keys), 1, fp);
		
		fclose(fp);
	}
	
	else{
		fread(&keys, sizeof(keys), 1, fp);
		fclose(fp);
	}
	
}

void LoadBlocks(){
	FILE *fp;
	
	fp	=	fopen("customblocks.wat","r");
	
	if(fp == NULL){
		
		for(row	=	0; row < 3; row++) customblocks[row].Left	=	4;
		
		fp	=	fopen("customblocks.wat","w");
		fwrite(&customblocks, sizeof(customblocks), 1, fp);
		
		fclose(fp);
	}
	
	else{
		fread(&customblocks, sizeof(customblocks), 1, fp);
		fclose(fp);
	}
}

void LoadHighscores(){
	FILE *fp;
	
	fp	=	fopen("highscore.wat","r");
	
	if(fp == NULL){
		
		highscore.index	=	0;
	
		for(row	=	0; row < 10; row++) strcpy(highscore.players[row], "AAA");
		memset(highscore.scores, 0, sizeof(highscore.scores[0]) * 10);
		
		fp	=	fopen("highscore.wat","w");
		fwrite(&highscore, sizeof(highscore), 1, fp);
		
		fclose(fp);
	}
	
	else{
		fread(&highscore, sizeof(highscore), 1, fp);
		fclose(fp);
	}
}

void LoadSounds(){
	
	FILE *fp;
	int temp	=	0;
	
	for(row	=	0; row < max_song; row++){
		fp	=	fopen(song[row], "r");
		
		if(fp != NULL){
			available_songs[row]	=	true;
			temp++;
		}
		fclose(fp);
	}
	
	if(temp == 0) soundavailable	=	false; 
}

void LoadSettings(){
	FILE *fp;
	
	fp	=	fopen("settings.wat","r");
	
	if(fp == NULL){
		
		settings.sound	=	true;
		settings.drophelp	=	true;
		
		fp	=	fopen("settings.wat","w");
		fwrite(&settings, sizeof(settings), 1, fp);
		
		fclose(fp);
	}
	
	else{
		fread(&settings, sizeof(settings), 1, fp);
		fclose(fp);
	}
}

void SaveKeys(){
	FILE *fp;
	
	fp	=	fopen("keys.wat","w+");
	
	fwrite(&keys, sizeof(keys), 1, fp);
	fclose(fp);
}

void SaveSettings(){
	FILE *fp;
	
	fp	=	fopen("settings.wat","w+");
	
	fwrite(&settings, sizeof(settings), 1, fp);
	fclose(fp);
}

void SaveHighscore(){
	FILE *fp;
	
	fp	=	fopen("highscore.wat","w+");
	
	fwrite(&highscore, sizeof(highscore), 1, fp);
	fclose(fp);
}

void SaveBlocks(){
	FILE *fp;
	
	fp	=	fopen("customblocks.wat","w+");
	
	fwrite(&customblocks, sizeof(customblocks), 1, fp);
	fclose(fp);
}

void ReadmeCreate(){
	FILE *fp;
	
	fp	=	fopen("highscore.wat","r");
	
	if(fp == NULL){
		
		fp	=	fopen("highscore.wat","w");
		fwrite(&highscore, sizeof(highscore), 1, fp);
		
		fclose(fp);
	}
	
	else{
		fread(&highscore, sizeof(highscore), 1, fp);
		fclose(fp);
	}
}

char * Keys(int key){
	
	char * txt	=	(char *) malloc(sizeof(char) * 6);
	
	if(key == UP_K) strcpy(txt, "\x18    ");
	else if(key == DOWN_K) strcpy(txt, "\x19    ");
	else if(key == LEFT_K) strcpy(txt, "\x1B    ");
	else if(key == RIGHT_K) strcpy(txt, "\x1A    ");
	else if(key == ESC) strcpy(txt, "ESC  ");
	else if(key == SPACE_K) strcpy(txt, "SPACE");
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
	
	else sprintf(txt, "%c    ", key);
	
	return txt;
}

void EpicText(const char TEXT[], int y, int color){

	int chr	=	0;
	char text[strlen(TEXT)];
	
	strcpy(text, TEXT);
	
	/*
		 xxxxxxxx  xxxxxxx   xxxxxxxx  xxxxxxx   xxxxxxxx  xxxxxxxx  xxxxxxxx  x      x  xxxxx       x  x    x   x        xxxxxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxx   xxxxxxxx xxxxxxx    xxxxxxx  xxxxxxxxx  x      x  x      x  x   x   x  xx     xx  x     x  xxxxxxxx   xxxxxx  xxx   xxxxxx  xxxxxx  x    x  xxxxxx  x       xxxxxx  xxxxxx  xxxxxx        x   xxx    x  x   xxx  x   x   x  x    xx x xx            x                            
		 x      x  x      x  x         x      x  x         x         x         x      x    x         x  x   x    x        x   x   x  x      x  x      x  x      x  x      x x      x  x             x      x      x  x     x   x   x   x    xx xx    x     x       xx    x   xx    x        x       x  x    x  x       x            x  x    x  x    x        x  x   x  xxxxxx  x x x    x  x    x     xxx             x
		 xxxxxxxx  xxxxxxxx  x         x      x  xxxxxxxx  xxxxxxxx  x    xxx  xxxxxxxx    x         x  xxxxxxx  x        x   x   x  x      x  x      x  xxxxxxxx  x      x xxxxxxxx  xxxxxxxx      x      x      x  x    x    x   x   x      x      xxxxxxx     xx      x xx x    x   xxxxxx  xxxxxx  xxxxxx  xxxxxx  xxxxxx       x  xxxxxx  xxxxxx        x     x    x  x   xxxxxxx     x    x   xxxxxxx  xxxx    x                            
		 x      x  x      x  x         x      x  x         x         x      x  x      x    x         x  x     x  x        x   x   x  x      x  x      x  x         x   x  x x      x         x      x      x      x  x   x     x   x   x    xx xx       x      xx        xx   x    x   x            x       x       x  x    x       x  x    x       x     x            xxxxxx    x x x     x    x     xxx           x
		 x      x  xxxxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxxx  x         xxxxxxxx  x      x  xxxxx  xxxxxx  x     x  xxxxxxx  x   x   x  x      x  xxxxxxxx  x         xxxxxxxx x      x  xxxxxxxx      x      xxxxxxxx  xxxx      xxxxxxxxx  xx     xx     x     xxxxxxxx   xxxxxx  xxxxx xxxxxx  xxxxxx       x  xxxxxx  xxxxxx       x  xxxxxx       x  x  x  x    x     x  x    x  xxx      x  x    xx x xx        x                                                                                                                                                           
	*/
	
	int string_length	=	0;
	//              a b c d e f g h i j k l m n o p q r s t u v w x y z . , ! ? # % ( ) * - / _ 0 1 2 3 4 5 6 7 8 9 
	int length[]	=	{8,8,8,8,8,8,8,8,5,6,7,7,9,8,8,8,8,7,8,7,8,9,9,7,8,8,1,1,1,5,6,7,2,2,7,4,5,3,6,5,6,6,6,6,6,6,6,6};
	char chrs[]	=	{"abcdefghijklmnopqrstuvwxyz.,!?#%()*-/ 0123456789"};
	
	for(row	=	0; row < strlen(TEXT); row++){
		for(column	=	0; column < sizeof(length)/sizeof(int); column++){
			if(text[row] >= 65 && text[row] <=90) text[row]	=	text[row] + 32;	
			if(text[row] == chrs[column]) string_length += length[column];
		}
		if(row < strlen(text)-1) string_length += 1;	
	}
	
	textcolor(color, BLACK);
	
	for(int row_x	=	-1; row_x < 6; row_x++){
			
		crmove((80 - string_length) / 2, y + row_x);
		
		for(chr	=	0; chr < strlen(text); chr++){
			
			if(row_x == -1 || row_x == 5){
				for(int row_x	=	0; row_x < string_length; row_x++) printf(" ");
				break;
			}
			
			if(text[chr] == 'a'){
				
				if(row_x == 0 || row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 3 || row_x == 4) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'b'){
				
				if(row_x == 0 || row_x == 2 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row_x == 1 || row_x == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'c'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3) fputs("\xDB       ", stdout);
			}
			else if(text[chr] == 'd'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'e'){
				
				if(row_x == 0 || row_x == 2 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 3) fputs("\xDB       ", stdout);
			}
			else if(text[chr] == 'f'){
				
				if(row_x == 0 || row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 3 || row_x == 4) fputs("\xDB       ", stdout);
			}
			else if(text[chr] == 'g'){
				
				if(row_x == 0 ||row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1) fputs("\xDB       ", stdout);
				else if(row_x == 2) fputs("\xDB    \xDB\xDB\xDB", stdout);
				else if(row_x == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'h'){
				
				if(row_x == 0 || row_x == 1 || row_x == 3 || row_x == 4) fputs("\xDB      \xDB", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'i'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3) fputs("  \xDB  ", stdout);
			}
			else if(text[chr] == 'j'){
				
				if(row_x == 0 || row_x == 1 || row_x == 2 || row_x == 3) fputs("     \xDB", stdout);
				else if(row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'k'){
				
				if(row_x == 0) fputs("\xDB     \xDB", stdout);
				else if(row_x == 1) fputs("\xDB    \xDB ", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 3 || row_x == 4) fputs("\xDB     \xDB", stdout);
			}			
			else if(text[chr] == 'l'){
				
				if(row_x == 0 || row_x == 1 || row_x == 2 || row_x == 3) fputs("\xDB      ", stdout);
				else if(row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'm'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3 || row_x == 4) fputs("\xDB   \xDB   \xDB", stdout);
			}
			else if(text[chr] == 'n'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3 || row_x == 4) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'o'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3) fputs("\xDB      \xDB", stdout);
			}
			else if(text[chr] == 'p'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row_x == 1) fputs("\xDB      \xDB", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 3 || row_x == 4) fputs("\xDB       ", stdout);
			}
			else if(text[chr] == 'q'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 2) fputs("\xDB      \xDB", stdout);
				else if(row_x == 3) fputs("\xDB    \xDB \xDB", stdout);
			}
			else if(text[chr] == 'r'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
				else if(row_x == 1 || row_x == 3 || row_x == 4) fputs("\xDB     \xDB", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
			}
			else if(text[chr] == 's'){
				
				if (row_x == 0)  fputs(" \xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 0 || row_x == 2 || row_x == 4)  fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1) fputs("\xDB       ", stdout);
				else if(row_x == 3) fputs("       \xDB", stdout);
			}
			else if(text[chr] == 't'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3 || row_x == 4) fputs("   \xDB   ", stdout);
			}
			else if(text[chr] == 'u'){
				
				if(row_x == 0 || row_x == 1 || row_x == 2 || row_x == 3) fputs("\xDB      \xDB", stdout);
				else if(row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == 'v'){
				
				if(row_x == 0) fputs("\xDB      \xDB", stdout);
				else if(row_x == 1) fputs("\xDB     \xDB ", stdout);
				else if(row_x == 2) fputs("\xDB    \xDB  ", stdout);
				else if(row_x == 3) fputs("\xDB   \xDB   ", stdout);
				else if(row_x == 4) fputs("\xDB\xDB\xDB\xDB    ", stdout);
			}
			else if(text[chr] == 'w'){
				
				if(row_x == 0 || row_x == 1 || row_x == 2 || row_x == 3) fputs("\xDB   \xDB   \xDB", stdout);
				else if(row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB ", stdout);
			}
			else if(text[chr] == 'x'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB     \xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 3) fputs("  \xDB\xDB \xDB\xDB  ", stdout);
				else if(row_x == 2) fputs("    \xDB    ", stdout);
			}
			else if(text[chr] == 'y'){
				
				if(row_x == 0 || row_x == 1) fputs("\xDB     \xDB", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 3 || row_x == 4) fputs("   \xDB   ", stdout);
			}
			else if(text[chr] == 'z'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1)  fputs("     \xDB\xDB ", stdout);
				else if(row_x == 2) fputs("   \xDB\xDB   ", stdout);
				else if(row_x == 3) fputs(" \xDB\xDB     ", stdout);
			}
			else if(text[chr] == '.'){
				
				if(row_x == 0 || row_x == 1 || row_x == 2 || row_x == 3) fputs(" ", stdout);
				else if(row_x == 4) fputs("\xDB", stdout);
			}
			else if(text[chr] == ','){
				
				if(row_x == 0 || row_x == 1 || row_x == 2) fputs(" ", stdout);
				else if(row_x == 3 || row_x == 4) fputs("\xDB", stdout);
			}
			else if(text[chr] == '!'){
				
				if(row_x == 0 || row_x == 1 || row_x == 2 || row_x == 4) fputs("\xDB", stdout);
				else if(row_x == 3) fputs(" ", stdout);
			}
			else if(text[chr] == '?'){
				
				if(row_x == 0) fputs(" \xDB\xDB\xDB ", stdout);
				else if(row_x == 1) fputs("\xDB   \xDB", stdout);
				else if(row_x == 2) fputs("   \xDB ", stdout);
				else if(row_x == 3) fputs("     ", stdout);
				else if(row_x == 4) fputs("  \xDB  ", stdout);
			}
			else if(text[chr] == '#'){
				
				if(row_x == 0 || row_x == 2 || row_x == 4) fputs(" \xDB  \xDB ", stdout);
				else if(row_x == 1 || row_x == 3) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == '%'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB  \xDB ", stdout);
				else if(row_x == 1) fputs("\xDB \xDB \xDB  ", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 3) fputs("  \xDB \xDB \xDB", stdout);
				else if(row_x == 4) fputs(" \xDB  \xDB\xDB\xDB", stdout);
			}			
			else if(text[chr] == '('){
				
				if(row_x == 0 || row_x == 4) fputs(" \xDB", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3) fputs("\xDB ", stdout);
			}			
			else if(text[chr] == ')'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB ", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3) fputs(" \xDB", stdout);
			}			
			else if(text[chr] == '*'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB \xDB \xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 3) fputs("  \xDB\xDB\xDB  ", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
			}			
			else if(text[chr] == '-'){
				
				if(row_x == 0 || row_x == 1 || row_x == 3 || row_x == 4) fputs("    ", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB", stdout);
			}			
			else if(text[chr] == '/'){
				
				if(row_x == 0) fputs("    \xDB", stdout);
				else if(row_x == 1) fputs("   \xDB ", stdout);
				else if(row_x == 2) fputs("  \xDB  ", stdout);
				else if(row_x == 3) fputs(" \xDB   ", stdout);
				else if(row_x == 4) fputs("\xDB    ", stdout);
			}		
			else if(text[chr] == ' '){
				
				 fputs("   ", stdout);
			}
			else if(text[chr] == '0'){
				
				if(row_x == 0 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1) fputs("\xDB   \xDB\xDB", stdout);
				else if(row_x == 2) fputs("\xDB \xDB\xDB \xDB", stdout);
				else if(row_x == 3) fputs("\xDB\xDB   \xDB", stdout);
			}
			else if(text[chr] == '1'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB  ", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3) fputs("  \xDB  ", stdout);
				else if(row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB", stdout);
			}
			else if(text[chr] == '2'){
				
				if(row_x == 0 || row_x == 2 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1) fputs("     \xDB", stdout);
				else if(row_x == 3) fputs("\xDB     ", stdout);
			}
			else if(text[chr] == '3'){
				
				if(row_x == 0 || row_x == 2 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 3) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '4'){
				
				if(row_x == 0 || row_x == 1) fputs("\xDB    \xDB", stdout);
				else if(row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 3 || row_x == 4) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '5'){
				
				if(row_x == 0 || row_x == 2 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1) fputs("\xDB     ", stdout);
				else if(row_x == 3) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '6'){
				
				if(row_x == 0 || row_x == 1) fputs("\xDB     ", stdout);
				else if(row_x == 2 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 3) fputs("\xDB    \xDB", stdout);
			}
			else if(text[chr] == '7'){
				
				if(row_x == 0) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 2 || row_x == 3 || row_x == 4) fputs("     \xDB", stdout);
			}
			else if(text[chr] == '8'){
				
				if(row_x == 0 || row_x == 2 || row_x == 4) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1 || row_x == 3) fputs("\xDB    \xDB", stdout);
			}
			else if(text[chr] == '9'){
				
				if(row_x == 0 || row_x == 2) fputs("\xDB\xDB\xDB\xDB\xDB\xDB", stdout);
				else if(row_x == 1) fputs("\xDB    \xDB", stdout);
				else if(row_x == 3 || row_x == 4) fputs("     \xDB", stdout);
			}
			
			else  fputs("CHYBA", stdout);
			
			if(chr < strlen(TEXT)-1)  fputs(" ", stdout);
		}

	}
}

void Defaults(){
	
	if(!settings.sound){
		settings.sound	=	true;
		SoundPlayer();
	}
	
	settings.drophelp	=	true;
	
	block.CUR	=	0;
	block.NEXT	=	0;
	block.HEIGHT	=	0;
	block.WIDTH	=	0;
	block.POS.X	=	0;
	block.POS.Y	=	0;
	
	keys.KEY[UP]	=	'w';
	keys.KEY[DOWN]	=	's';
	keys.KEY[LEFT]	=	'a';
	keys.KEY[RIGHT]	=	'd';
	keys.KEY[ROTATE_L]	=	'q';
	keys.KEY[ROTATE_R]	=	'e';
	keys.KEY[DROP]	=	SPACE_K;
	keys.KEY[PAUSE]	=	ESC;
	keys.KEY[NEXTSONG]	=	'n';

	SaveSettings();
	SaveKeys();

	defaults	=	true;
	highreset	=	false;
}

void HighScoreReset(){
	for(row	=	0; row < 10; row++) strcpy(highscore.players[row], "AAA");
	memset(highscore.scores, 0, sizeof(highscore.scores[0]) * 10);
	
	highreset	=	true;
	defaults	=	false;
	
	SaveHighscore();
}

void Mid(const char TEXT[], int x, int y, int color){
	textcolor(color, BLACK);
	crmove(x, y);
	
	fputs(TEXT, stdout);
}

void Mid(const char TEXT[], int x, int y, int foreground, int background){
	textcolor(foreground, background);
	crmove(x, y);
	
	fputs(TEXT, stdout);
}

void textattr(int _attr) {
	SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), _attr);
}

void textcolor(int _fg, int _bg) {
	textforeground(_fg);
	textbackground(_bg);
}

void textforeground(int _color){
	CONSOLE_SCREEN_BUFFER_INFO Info ;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info) ;
	textattr((Info.wAttributes & 0xF0) | _color);
}

void textbackground(int _color) {
	CONSOLE_SCREEN_BUFFER_INFO Info ;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info) ;
	textattr((Info.wAttributes & 0x0F) | (_color<<4));
}

void crmove(int x, int y) {
	COORD c ;
	c.X	=	x;
	c.Y	=	y;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clrscr(){
	COORD coord	=	{ 0, 0 } ;
	DWORD dwWritten, dwSize ;
	HANDLE hScreen=GetStdHandle(STD_OUTPUT_HANDLE) ;
	CONSOLE_SCREEN_BUFFER_INFO Info;
	
	if(GetConsoleScreenBufferInfo(hScreen, &Info)){
		dwSize	=	Info.dwSize.X * Info.dwSize.Y ;
		FillConsoleOutputCharacter (hScreen, ' ', dwSize, coord, &dwWritten);
		FillConsoleOutputAttribute (hScreen, Info.wAttributes, dwSize, coord, &dwWritten);
		crmove(0, 0);
	}
}

void setcursortype(int type){
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible	=	TRUE;
	
	if (type == _NOCURSOR){
		Info.dwSize	=	100;
		Info.bVisible	=	FALSE;
	} 
	else if (type == _SOLIDCURSOR) Info.dwSize	=	90 ;
	else if (type == _NORMALCURSOR) Info.dwSize	=	1;
	
	SetConsoleCursorInfo (GetStdHandle (STD_OUTPUT_HANDLE), &Info);
}

void SetWindow(int x, int y) { 
    HANDLE h	=	GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(h, &bufferInfo);
    SMALL_RECT& winInfo	=	bufferInfo.srWindow;
    COORD windowSize	=	{ winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1};

    if (windowSize.X > x || windowSize.Y > y)
    {
    	///
        /// window size needs to be adjusted before the buffer size can be reduced.
        ///
        
		SMALL_RECT info	=	
        { 
            0, 
            0, 
            x < windowSize.X ? x-1 : windowSize.X-1, 
            y < windowSize.Y ? y-1 : windowSize.Y-1 
        };

		SetConsoleWindowInfo(h, TRUE, &info);
    }

    COORD size	=	{ x, y };
    SetConsoleScreenBufferSize(h, size);

    SMALL_RECT info	=	{ 0, 0, x - 1, y - 1 };
    SetConsoleWindowInfo(h, TRUE, &info);
}

void SetFont(int size_x, int size_y){
	CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize	=	sizeof cfi;
    cfi.nFont	=	0;
    cfi.dwFontSize.X	=	size_x;
    cfi.dwFontSize.Y	=	size_y;
    cfi.FontFamily	=	FF_DONTCARE;
    cfi.FontWeight	=	FW_NORMAL;
    
    wcscpy(cfi.FaceName, L"Terminal");
    
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void WindowSize(){
	while(true){
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		
		int Width	=	csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int Height	=	csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		
		if(last_height != Height || last_width != Width){
			clrscr();
			SetWindow(last_width, last_height);
			
			page.PAGELOAD	=	true;
			page.GAMELOAD	=	true;
		}
		
		Sleep(10);
	}
	
	
}

void LoadingScreenAnim(){
	
	int cur_str	=	0, percent	=	0, t	=	0, t2	=	0, speed	=	0;
	
	while(percent < 1000){
		
		int temp	=	cur_str;
		
		while(temp == cur_str) cur_str	=	Random(0, load_scr_str);
		
		if(t == 0){
			speed	=	Random(2, 10);
			t	=	Random(20, 100);
			
			Mid("                                                ", (80 - strlen("                                                ")) / 2, 20, WHITE);
			Mid(loadstr[cur_str], (80 - strlen(loadstr[cur_str])) / 2, 20, WHITE);
		} 
		
		t--;
		t2++;
		
		if(t % speed == 0) percent += 10;
		if(percent > 1000) percent	=	1000;
		
		crmove(38, 22);
		textcolor(WHITE, BLACK);
		printf("%3d%%", percent / 10);
		
		if(t2 % 20 == 1){
			EpicText("tetris",10, YELLOW);
			for(row_th	=	0; row_th < 5; row_th++){
				if((t2 % 100) / 20 != row_th) Mid("                                                     ", (80 - strlen("                                                     ")) / 2, 10 + row_th, BLACK);
			}
		}
		
		Sleep(30);
	}
	
	for(int i	=	0; i < 6; i++){
		EpicText("tetris",10, YELLOW);
		for(int j	=	i; j < 6; j++){
			if(j > i)Mid("                                                     ", (80 - strlen("                                                     ")) / 2, 10 + j, BLACK);
		}
		Sleep(200);
	}
	
	for(int i	=	0; i < 5; i++){
		EpicText("tetris", 10, YELLOW);
		Sleep(150);
		clrscr();
		Sleep(200);
	}
	
	for(int i	=	10; i > 3; i--){
		EpicText("tetris", i, YELLOW);
		Sleep(250);
	}
	
	
	
}

void Render(){
	
	srand(time(NULL));
	
	int xd	=	0;
	
	if(loading)LoadingScreenAnim();
	
	loading	=	false;
	
	bool anim_d	=	false;
	
	while(true){
		
		clrscr();
	
		while(page.MAINMENU){
			
			if(screen.TETRIS == 8){
				anim_d	=	true;
			}
			else if(screen.TETRIS == 3){
				anim_d	=	false;
			}
			
			if(!anim_d){
				screen.TETRIS++;
			}
			else if(anim_d){
				screen.TETRIS--;
			}
			
			EpicText("tetris", screen.TETRIS, YELLOW);
			
			for(int i	=	0; i < 50 && page.MAINMENU ; i++){
				Mid( (selection.MAINMENU == 0 ? "> Play <" : "  Play  ") , (80 - strlen("  Play  ")) / 2, 16, (selection.MAINMENU == 0 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 1 ? "> HighScores <" : "  HighScores  ") , (80 - strlen("  HighScores  ")) / 2, 17, (selection.MAINMENU == 1 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 2 ? "> Block Creation <" : "  Block Creation  ") , (80 - strlen("  Block Creation  ")) / 2, 18, (selection.MAINMENU == 2 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 3 ? "> Settings <" : "  Settings  ") , (80 - strlen("  Settings  ")) / 2, 19, (selection.MAINMENU == 3 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 4 ? "> Info <" : "  Info  ") , (80 - strlen("  Info  ")) / 2, 20, (selection.MAINMENU == 4 ? WHITE : DARKGRAY));
				Mid( (selection.MAINMENU == 5 ? "> Exit <" : "  Exit  ") , (80 - strlen("  Exit  ")) / 2, 22, (selection.MAINMENU == 5 ? WHITE : DARKGRAY));
				
				#if DEBUG
					crmove(0,0);
					textcolor(DARKGRAY, BLACK);
					printf("sel %d | pressed %3d %c | pos_t %d |", selection.MAINMENU, keys.PRESSED, keys.PRESSED, screen.TETRIS);
					crmove(0,1);
					printf("width %3d | height %2d | sound %d", last_width, last_height, settings.sound);
					Mid("DEBUG MODE",0,25,WHITE);
				#endif
				
				Sleep(25);
			}
			
		}
		
		clrscr();
		
		while(page.HIGHSCORE){
			
			if(page.PAGELOAD){
				
				clrscr();
				
				EpicText("highscore", 2, GREEN);
				page.PAGELOAD	=	false;
			}
			
			textcolor(WHITE, BLACK);
			
			for(row	=	0; row < 10; row++){
				crmove(25, 10 + row);
				puts(highscore.players[row]);
				
				crmove(55 - Digits(highscore.scores[row]), 10 + row);
				printf("%d", highscore.scores[row]);
			}
			
			Mid("> Back <", (80 - strlen("> Back <")) / 2, 22, WHITE);
			
			Sleep(50);
		}
		
		clrscr();
		
		while(page.GAMEOVER){
			
			while(page.GONAME){
				if(page.PAGELOAD){
					clrscr();
					
					EpicText("game over", 2, LIGHTRED);
					page.PAGELOAD	=	false;
				}
				Mid("SELECT NAME", (80 - strlen("SELECT NAME")) / 2, 12, WHITE);
				
				for(int i	=	0; i < 3; i++){
					Mid(" ", 39 + i, 15, BLACK);
				}
				
				for(int i	=	0; i < 3; i++){
					crmove(39 + i, 15);
					textcolor(WHITE, BLACK);
					printf("%c", i < namecount ? player[i] : '_');
				}
				
				Mid(namecount == 3 ? "> Continue <" : "  Continue  ", (80 - strlen("  continue  ")) / 2, 22, namecount == 3 ? WHITE : DARKGRAY);
				
				Sleep(50);
			}
			
			if(page.PAGELOAD){
				clrscr();
				
				EpicText("score", 2, LIGHTGREEN);
				page.PAGELOAD	=	false;
			}
			
			Mid("Your Score:", 30, 12, WHITE);
			crmove(55 - Digits(score), 12);
			printf("%d", score);
			
			Mid("Level:", 30, 13, WHITE);
			crmove(55 - Digits(level), 13);
			printf("%d", level);
			
			Mid("Blocks droped: ", 30, 14, WHITE);
			
			int temp	=	block.STATS.I + block.STATS.J + block.STATS.L + block.STATS.O + block.STATS.S + block.STATS.T + block.STATS.Z;
			
			crmove(55 - Digits(temp), 14);
			printf("%d", temp);
			
			Mid("> Continue <", (80 - strlen("  continue  ")) / 2, 22, WHITE);
			
			Sleep(50);
		}
		
		clrscr();
		
		while(page.GAMETYPE){
			
			while(page.MUTATOR){
				
				if(page.PAGELOAD){
					clrscr();
					
					EpicText("mutators", 1, WHITE);
					page.PAGELOAD	=	false;
				}
				
				Mid("Timefactor", 20, screen.GAME.MUTATOR.Y, selection.MUTATOR == 0 ? WHITE : DARKGRAY);
				crmove(50, screen.GAME.MUTATOR.Y);
				textcolor(selection.MUTATOR == 0 ? WHITE : DARKGRAY, BLACK);
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
				textcolor(selection.MUTATOR == 3 ? WHITE : DARKGRAY, BLACK);
				printf("%3d%%",mods.BOARDFULL * 25);
				
				Mid("Harddrop only", 20, screen.GAME.MUTATOR.Y + 4, selection.MUTATOR == 4 ? WHITE : DARKGRAY);
				Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 4, BLACK);
				Mid("Disabled", mods.HARDDROPONLY ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 4, selection.MUTATOR == 4 ? (!mods.HARDDROPONLY ? WHITE : DARKGRAY) : (!mods.HARDDROPONLY ? DARKGRAY : BLACK));						
				Mid("Enabled", mods.HARDDROPONLY ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 4, selection.MUTATOR == 4 ? (mods.HARDDROPONLY ? WHITE : DARKGRAY) : (mods.HARDDROPONLY ? DARKGRAY : BLACK));
				
				Mid("Time", 20, screen.GAME.MUTATOR.Y + 5, selection.MUTATOR == 5 ? WHITE : DARKGRAY);
				Mid("                  ", 50 - strlen("Disabled") / 2, screen.GAME.MUTATOR.Y + 5, BLACK);
				crmove(mods.TIMERUN == -1 ? 54-strlen("Endless")/2 : (mods.TIMERUN == 0 ? 54 - strlen("Disabled")/2 : 52), screen.GAME.MUTATOR.Y + 5);
				textcolor(selection.MUTATOR == 5 ? WHITE : DARKGRAY, BLACK);
				if(mods.TIMERUN <= 0){ printf(mods.TIMERUN == -1 ? "Endless" : "Disabled"); }
				else printf("%dmin",mods.TIMERUN);
			    
				Mid("Board size", 20, screen.GAME.MUTATOR.Y + 6, selection.MUTATOR == 6 ? WHITE : DARKGRAY);
				Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 6, BLACK);
				Mid(mods.BOARDSIZE == 0 ? "Small" : (mods.BOARDSIZE == 1 ? "Medium" : "HUGE"), 54 - (mods.BOARDSIZE == 0 ? strlen("Small") : (mods.BOARDSIZE == 1 ? strlen("Medium") : strlen("HUGE"))) / 2, screen.GAME.MUTATOR.Y + 6, selection.MUTATOR == 6 ? WHITE : DARKGRAY);
				
				Mid("Ghostblocks", 20, screen.GAME.MUTATOR.Y + 7, selection.MUTATOR == 7 ? WHITE : DARKGRAY);
				Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 7, BLACK);
				Mid("Disabled", mods.GHOSTBLOCKS ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 7, selection.MUTATOR == 7 ? (!mods.GHOSTBLOCKS ? WHITE : DARKGRAY) : (!mods.GHOSTBLOCKS ? DARKGRAY : BLACK));						
				Mid("Enabled", mods.GHOSTBLOCKS ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 7, selection.MUTATOR == 7 ? (mods.GHOSTBLOCKS ? WHITE : DARKGRAY) : (mods.GHOSTBLOCKS ? DARKGRAY : BLACK));
				
				Mid("Rows builder", 20, screen.GAME.MUTATOR.Y + 8, selection.MUTATOR == 8 ? WHITE : DARKGRAY);
				Mid("                                ", 50 - strlen("Normal") - 4, screen.GAME.MUTATOR.Y + 8, BLACK);
				Mid("Disabled", mods.BUILDER ? 54 - strlen("Enabled") / 2 - 2 - strlen("Disabled") : 54 - strlen("Disabled") / 2,screen.GAME.MUTATOR.Y + 8, selection.MUTATOR == 8 ? (!mods.BUILDER ? WHITE : DARKGRAY) : (!mods.BUILDER ? DARKGRAY : BLACK));						
				Mid("Enabled", mods.BUILDER ? 54 - strlen("Enabled") / 2 : 54 + strlen("Disabled") / 2 + 2,screen.GAME.MUTATOR.Y + 8, selection.MUTATOR == 8 ? (mods.BUILDER ? WHITE : DARKGRAY) : (mods.BUILDER ? DARKGRAY : BLACK));
				
				
				Mid(selection.MUTATOR == 9 ? "> Start game <" : "  Start game  ", (80 - strlen("  Start game  ")) / 2, screen.GAME.MUTATOR.Y + 12, selection.MUTATOR == 9 ? WHITE : DARKGRAY);
				Mid(selection.MUTATOR == 10 ? "> Back <" : "  Back  ", (80 - strlen("  Back  ")) / 2, screen.GAME.MUTATOR.Y + 14, selection.MUTATOR == 10 ? WHITE : DARKGRAY);
						
				Sleep(50);
			}
			
			if(page.PAGELOAD){
				clrscr();
				
				EpicText("game type", 2, WHITE);
				page.PAGELOAD	=	false;
			}
			
			Mid(selection.GAMETYPE == 1 ? "> Normal <" : "  Normal  ", (80 - strlen("> Normal <")) / 2, screen.GAME.GAMETYPE.Y, selection.GAMETYPE == 1 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 2 ? "> Chainreaction <" : "  Chainreaction  ", (80 - strlen("> Chainreaction <")) / 2, screen.GAME.GAMETYPE.Y + 1, selection.GAMETYPE == 2 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 3 ? "> Builder <" : "  Builder  ", (80 - strlen("> Builder <")) / 2, screen.GAME.GAMETYPE.Y + 2, selection.GAMETYPE == 3 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 4 ? "> Sharp shooter <" : "  Sharp shooter  ", (80 - strlen("> Sharp shooter <")) / 2, screen.GAME.GAMETYPE.Y + 3, selection.GAMETYPE == 4 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 5 ? "> Half way <" : "  Half way  ", (80 - strlen("> Half way <")) / 2, screen.GAME.GAMETYPE.Y + 4, selection.GAMETYPE == 5 ? WHITE : DARKGRAY);
			Mid(selection.GAMETYPE == 6 ? "> Mutators <" : "  Mutators  ", (80 - strlen("> Mutators <")) / 2, screen.GAME.GAMETYPE.Y + 6, selection.GAMETYPE == 6 ? WHITE : DARKGRAY);
			
			Mid(selection.GAMETYPE == 7 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.GAME.GAMETYPE.Y + 9, selection.GAMETYPE == 7 ? WHITE : DARKGRAY);
			Sleep(50);
		}
		
		clrscr();
		
		while(page.GAME){
			
			if(page.GAMELOAD){
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
				
				for(row_th	=	0; row_th < board.HEIGHT + 2; row_th++){
					
					#if DEBUG
						textcolor(DARKGRAY, BLACK);
						crmove(0,screen.GAME.BOARD.Y + row_th);
						printf("%2d",row_th);
					#endif
					
					if(row_th < board.HEIGHT){
						Mid("<|", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, WHITE);
						Mid("|>", screen.GAME.BOARD.X + board.WIDTH - 2, screen.GAME.BOARD.Y + row_th, WHITE);
					}
					else if(row_th == board.HEIGHT){
						Mid("<", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, WHITE);
						for(int x	=	0; x < board.WIDTH; x++) Mid("=", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, WHITE);
						Mid(">", screen.GAME.BOARD.X + board.WIDTH - 1, screen.GAME.BOARD.Y + row_th, WHITE);
					}
					else{
						for(int x	=	0; x < board.WIDTH; x++) Mid("v", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, WHITE);
					}
					
					#if DEBUG
						Mid("  123456789ABCDEFG  ", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + board.HEIGHT + 2, DARKGRAY);
					#endif
				}
				
				Mid("LEVEL", screen.GAME.CURBLOCK.X - strlen("LEVEL"), screen.GAME.CURBLOCK.Y, WHITE);
				
				Mid("SCORE", screen.GAME.CURBLOCK.X - strlen("SCORE"), screen.GAME.CURBLOCK.Y + 2, WHITE);
				Mid("000000000", screen.GAME.CURBLOCK.X + 2, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				
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
				textcolor(WHITE, BLACK);
				printf("%d", level);
				
				crmove(screen.GAME.CURBLOCK.X + 11 - Digits(score), screen.GAME.CURBLOCK.Y + 2);
				textcolor(WHITE, BLACK);
				printf("%d", score);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.I), screen.GAME.CURBLOCK.Y + 12);
				textcolor(WHITE, BLACK);
				printf("%d", block.STATS.I);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.L), screen.GAME.CURBLOCK.Y + 13);
				textcolor(WHITE, BLACK);
				printf("%d", block.STATS.L);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.J), screen.GAME.CURBLOCK.Y + 14);
				textcolor(WHITE, BLACK);
				printf("%d", block.STATS.J);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.O), screen.GAME.CURBLOCK.Y + 15);
				textcolor(WHITE, BLACK);
				printf("%d", block.STATS.O);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.T), screen.GAME.CURBLOCK.Y + 16);
				textcolor(WHITE, BLACK);
				printf("%d", block.STATS.T);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.Z), screen.GAME.CURBLOCK.Y + 17);
				textcolor(WHITE, BLACK);
				printf("%d", block.STATS.Z);
				
				crmove(screen.GAME.CURBLOCK.X + 8 - Digits(block.STATS.S), screen.GAME.CURBLOCK.Y + 18);
				textcolor(WHITE, BLACK);
				printf("%d", block.STATS.S);
				
				page.GAMELOAD	=	false;
			}
			
			if(!mods.UPSIDEDOWN){
				for(row_th	=	0; row_th < board.HEIGHT; row_th++){
					for(column_th	=	0; column_th < board.WIDTH - 2; column_th++){
						Mid(board.Layers[row_th][column_th + 1][1].IsBlock || board.Layers[row_th][column_th + 1][2].IsBlock  ? "#" : (board.Layers[row_th][column_th + 1][0].IsBlock  ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, board.Layers[row_th][column_th + 1][1].IsBlock ? board.Layers[row_th][column_th + 1][1].Color : (board.Layers[row_th][column_th + 1][2].IsBlock ? LIGHTGRAY : board.Layers[row_th][column_th + 1][0].Color));
					}
				}
			}
			
			else if(mods.UPSIDEDOWN){
				for(row_th	=	0; row_th < board.HEIGHT; row_th++){
					for(column_th	=	0; column_th < board.WIDTH - 2; column_th++){
						Mid(board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][1].IsBlock || board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][2].IsBlock ? "#" : (board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][0].IsBlock ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][1].IsBlock ? board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][1].Color : (board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][2].IsBlock ? LIGHTGRAY : board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][0].Color));
					}
				}
			}
			
			if(mods.TIMERUN > 0){
				crmove( screen.GAME.CURBLOCK.X - strlen("Time Left: xx:xx")/2, screen.GAME.CURBLOCK.Y + 22);
				textcolor(timeleft > 20 ? WHITE : LIGHTRED, BLACK);
				printf("Time Left: %02d:%02d", timeleft / 60, timeleft % 60);
			}
			
			
			#if DEBUG
				crmove(0,0);
				textcolor(DARKGRAY, BLACK);
				printf("delay %2d | level %2d | rowtoclear %2d | h-X %2d | h-Y %2d | %d pause",delay, level, thislevelclear, helper.POS.X, helper.POS.Y, page.PAUSERETURN);
			#endif	
			
			Sleep(delay / 3 * 2);
		}
		
		clrscr();
		
		while(page.SETTINGS){
			
			while(page.KEYS){
				
				if(page.PAGELOAD){
					clrscr();
					
					EpicText("settings", 2, WHITE);
					page.PAGELOAD	=	false;
				}
				
				Mid("> Key Binding <", 13 - strlen("> Key Binding <") / 2, 13, DARKGRAY);
				Mid(settings.sound ? "  Sound ON   " : "  Sound OFF  ", 14 - strlen("> Key Binding <") / 2, 14, DARKGRAY);
				Mid("  Reset HighScores  ", 14 - strlen("> Reset HighScores <") / 2, 17, DARKGRAY);
				Mid("  Reset Defaults  ", 14 - strlen("> Reset defaults <") / 2, 18, DARKGRAY);
				Mid("  Back  ", 14 - strlen("> Back <") / 2, 21, DARKGRAY);
				
				Mid("Keys", (80 - strlen("Keys")) / 2, 11, WHITE);
				Mid("Primary", 50, 11, WHITE);
				Mid("Secondary*", 60, 11, WHITE);
				
				Mid(selection.KEYS == 0 ? "> Up" : "  Up", (80 - strlen("Keys")) / 2 - 1, 14, selection.KEYS == 0 ? WHITE : DARKGRAY);
				crmove(53, 14);
				printf("%s", Keys(keys.KEY[UP]));
				crmove(63, 14);
				printf("W");
				
				Mid(selection.KEYS == 1 ? "> Down" : "  Down", (80 - strlen("Keys")) / 2 - 1, 15, selection.KEYS == 1 ? WHITE : DARKGRAY);
				crmove(53, 15);
				printf("%s", Keys(keys.KEY[DOWN]));
				crmove(63, 15);
				printf("S");
				
				Mid(selection.KEYS == 2 ? "> Left" : "  Left", (80 - strlen("Keys")) / 2 - 1, 16, selection.KEYS == 2 ? WHITE : DARKGRAY);
				crmove(53, 16);
				printf("%s", Keys(keys.KEY[LEFT]));
				crmove(63, 16);
				printf("A");
				
				Mid(selection.KEYS == 3 ? "> Right" : "  Right", (80 - strlen("Keys")) / 2 - 1, 17, selection.KEYS == 3 ? WHITE : DARKGRAY);
				crmove(53, 17);
				printf("%s", Keys(keys.KEY[RIGHT]));
				crmove(63, 17);
				printf("D");
				
				Mid(selection.KEYS == 4 ? "> Rotate left" : "  Rotate left", (80 - strlen("Keys")) / 2 - 1, 18, selection.KEYS == 4 ? WHITE : DARKGRAY);
				crmove(53, 18);
				printf("%s", Keys(keys.KEY[ROTATE_L]));
				crmove(63, 18);
				printf("Q");
				
				Mid(selection.KEYS == 5 ? "> Rotate right" : "  Rotate right", (80 - strlen("Keys")) / 2 - 1, 19, selection.KEYS == 5 ? WHITE : DARKGRAY);
				crmove(53, 19);
				printf("%s", Keys(keys.KEY[ROTATE_R]));
				crmove(63, 19);
				printf("E");
				
				Mid(selection.KEYS == 6 ? "> Drop" : "  Drop", (80 - strlen("Keys")) / 2 - 1, 20, selection.KEYS == 6 ? WHITE : DARKGRAY);
				crmove(53, 20);
				printf("%s", Keys(keys.KEY[DROP]));
				crmove(63, 20);
				printf("SPACE");
				
				Mid(selection.KEYS == 7 ? "> Pause" : "  Pause", (80 - strlen("Keys")) / 2 - 1, 21, selection.KEYS == 7 ? WHITE : DARKGRAY);
				crmove(53, 21);
				printf("%s", Keys(keys.KEY[PAUSE]));
				crmove(63, 21);
				printf("ESC");
				
				Mid(selection.KEYS == 8 ? "> Next song" : "  Next song", (80 - strlen("Keys")) / 2 - 1, 22, selection.KEYS == 8 ? WHITE : DARKGRAY);
				crmove(53, 22);
				printf("%s", Keys(keys.KEY[NEXTSONG]));
				crmove(63, 22);
				printf("N");
				
				Mid(selection.KEYS == 9 ? "> Back <" : "  Back  " , (80 - strlen("> Back <")) / 2, 30, selection.KEYS == 9 ? WHITE : DARKGRAY);
				
				Mid("*can't be changed - backup keys", 80 - strlen("*can't be changed - backup keys") - 1, 34, DARKGRAY);
				
				if(keychanging){
					Mid("Press any key, DELETE for cancel", (80 - strlen("Press any key, DELETE for cancel")) / 2, 9, WHITE);
				}
				
				else if(keys.SAMEKEYEXCEPTION){
					Mid(" Can't bind, key already binded!", (80 - strlen(" Can't bind, key already binded!")) / 2, 9, RED);
				}
				
				else Mid("                                ", (80 - strlen("Press any key, DELETE for cancel")) / 2, 9, WHITE);
				
				Sleep(50);
			}
			
			if(page.PAGELOAD){
				clrscr();
				
				EpicText("settings", 2, WHITE);
				page.PAGELOAD	=	false;
			}
			
			
			Mid(selection.SETTINGS == 0 ? "> Key Binding <" : "  Key Binding  ", (80 - strlen("> Key Binding <")) / 2, screen.SETTINGS.Y, selection.SETTINGS == 0 ? WHITE : DARKGRAY);
			Mid(soundavailable ? (selection.SETTINGS == 1 ? (settings.sound ? "> Sound ON < " : "> Sound OFF <") : (settings.sound ? "  Sound ON   " : "  Sound OFF  ")) : "Sound Not Available", soundavailable ? (80 - strlen("> Sound OFF <")) / 2 :  (80 - strlen("Sound Not Available")) / 2 , screen.SETTINGS.Y + 1, soundavailable ? (selection.SETTINGS == 1 ? WHITE : DARKGRAY) : DARKGRAY);
			Mid(selection.SETTINGS == 2 ? (settings.drophelp ? "> Drop help ON < " : "> Drop help OFF <") : (settings.drophelp ? "  Drop help ON   " : "  Drop help OFF  "), (80 - strlen(">  Drop help OFF  ")) / 2, screen.SETTINGS.Y + 2, selection.SETTINGS == 2 ? WHITE : DARKGRAY);
			Mid(selection.SETTINGS == 3 ? "> Reset HighScores <" : "  Reset HighScores  ", (80 - strlen("> Reset HighScores <")) / 2, screen.SETTINGS.Y + 5, selection.SETTINGS == 3 ? WHITE : DARKGRAY);
			Mid(selection.SETTINGS == 4 ? "> Reset Defaults <" : "  Reset Defaults  ", (80 - strlen("> Reset Defaults <")) / 2, screen.SETTINGS.Y + 6, selection.SETTINGS == 4 ? WHITE : DARKGRAY);
	
			Mid(selection.SETTINGS == 5 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.SETTINGS.Y + 9, selection.SETTINGS == 5 ? WHITE : DARKGRAY);
			
			if(defaults && xd == 0){
				Mid("DEFAULTS RESTORED!", (80 - strlen("DEFAULTS RESTORED!")) / 2, 10, LIGHTRED);
			}
			
			if(highreset && xd == 0){
				Mid("SCOREBOARD WAS RESET!", (80 - strlen("SCOREBOARD WAS RESET!")) / 2, 10, LIGHTRED);
			}
			
			if(defaults || highreset) xd++; 
			
			if(xd == 40){
				Mid("                         ", (80 - strlen("                         ")) / 2, 10, LIGHTRED);
				defaults	=	false;
				highreset	=	false;
				xd	=	0;
			} 
			
			Sleep(50);
		}
		
		clrscr();
		
		while(page.INFO){
			
			while(page.HELP){
				
				if(page.PAGELOAD){
					
					clrscr();
					
					EpicText("tetris", 2, WHITE);
					
					crmove(60 - strlen("                             ") / 2, 10);
					textcolor(BLACK, BLACK);
					printf("                             ");
					
					crmove(60 - (strlen(help_label[selection.HELPPAGE]) + 4) / 2, 10);
					textcolor(selection.HELP == 0 ? WHITE : DARKGRAY, BLACK);
					printf(selection.HELP == 0 ? "> %s <" : "  %s  ",help_label[selection.HELPPAGE]);
					
					for(row_th	=	0; row_th < help_txt_rows[selection.HELPPAGE]; row_th++){
						Mid(help_txt[selection.HELPPAGE][row_th], 45, 13 + row_th, selection.HELP == 0 ? WHITE : DARKGRAY);
					}
					
					Mid(selection.HELP == 1 ? "> Back <" : "  Back  ", 60 - strlen("  Back  ") / 2, 25, selection.HELP == 1 ? WHITE : DARKGRAY);
					
					if(selection.HELPPAGE == MUTATORS){ for(row_th	=	0; row_th < 14; row_th++) Mid(img[row_th], 10, 10 + row_th, LIGHTRED); }
					else{
						for(row_th	=	0; row_th < SMALL_H + 2; row_th++){
							if(row_th < SMALL_H) Mid("<|            |>", 8, 9 + row_th, WHITE);
							else if(row_th == SMALL_H) Mid("<-------------->", 8, 9 + row_th, WHITE);
							else Mid("vvvvvvvvvvvvvv", 9, 9 + row_th, WHITE);
						}
					}
					
					page.PAGELOAD	=	false;
				}
				
				if(selection.HELPPAGE != MUTATORS){
					if(selection.HELPPAGE != SHARPSHOOTER){
						for(row_th	=	0; row_th < SMALL_H; row_th++){
							for(column_th	=	0; column_th < SMALL_W - 2; column_th++){ 
								Mid(help.Layers[row_th][column_th + 1][1].IsBlock ? "#" : (help.Layers[row_th][column_th + 1][0].IsBlock ? "x" : "."), 10 + column_th, 9 + row_th, help.Layers[row_th][column_th + 1][1].IsBlock ? help.Layers[row_th][column_th + 1][1].Color : help.Layers[row_th][column_th + 1][0].Color);
							}
						}
					}
					
					else{
						for(row_th	=	0; row_th < SMALL_H; row_th++){
							for(column_th	=	0; column_th < SMALL_W - 2; column_th++){
								Mid(help.Layers[row_th][column_th + 1][1].IsBlock ? "#" : (help.Layers[row_th][column_th + 1][0].IsBlock ? "x" : "."), 10 + column_th, 8 + SMALL_H - row_th, help.Layers[row_th][column_th + 1][1].IsBlock ? help.Layers[row_th][column_th + 1][1].Color : help.Layers[row_th][column_th + 1][0].Color);
							}
						}
					}
				}
				
				#if DEBUG
					crmove(0,0);
					textcolor(DARKGRAY, BLACK);
					printf("%d", &blockhelp.POS.Y);
					crmove(0,1);
					textcolor(DARKGRAY, BLACK);
					printf("%d", &block.POS.Y);
				#endif
				
				Sleep(50);
			}
			
			while(page.CHANGELOG){
				if(page.PAGELOAD){
					
					clrscr();
					
					for(row_th = 0; row_th < 20; row_th++){
						Mid(changelog[selection.CHANGELOG + row_th], 25, 3 + row_th, LIGHTGRAY);
					}
					
					Mid("> Back <", 40 - strlen("> Back <") / 2, 27, WHITE);
					
					page.PAGELOAD = false;
				}
				Sleep(50);
			}
			
			if(page.PAGELOAD){
				clrscr();
				
				EpicText("tetris", 2, WHITE);
				page.PAGELOAD	=	false;
			}
			
			for(row_th	=	0; row_th < 3; row_th++){
				Mid(info_txt[row_th], (80 - strlen(info_txt[row_th])) / 2, screen.INFO.Y + row_th, WHITE);
			}
			
			
			Mid(selection.INFO == 0 ? "> Help <" : "  Help  ", (80 - strlen("  Help  ")) / 2, screen.INFO.Y + 7, selection.INFO == 0 ? WHITE : DARKGRAY);
			Mid(selection.INFO == 1 ? "> Playlist <" : "  Playlist  ", (80 - strlen("  Playlist  ")) / 2, screen.INFO.Y + 8, selection.INFO == 1 ? WHITE : DARKGRAY);
			Mid(selection.INFO == 2 ? "> Changelog <" : "  Changelog  ", (80 - strlen("> Changelog <")) / 2, screen.INFO.Y + 9,selection.INFO == 2 ? WHITE : DARKGRAY);
			Mid(selection.INFO == 3 ? "> Back <" : "  Back  ", (80 - strlen("> Back <")) / 2, screen.INFO.Y + 12,selection.INFO == 3 ? WHITE : DARKGRAY);
			
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
			
			if(page.GAMELOAD){
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
				
				for(row_th	=	0; row_th < board.HEIGHT + 2; row_th++){
					
					#if DEBUG
						textcolor(DARKGRAY, BLACK);
						crmove(0,screen.GAME.BOARD.Y + row_th);
						printf("%2d",row_th);
					#endif
					
					if(row_th < board.HEIGHT){
						Mid("<|", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						for(int x	=	0; x < board.WIDTH - 2; x++) Mid(" ", screen.GAME.BOARD.X, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						Mid("|>", screen.GAME.BOARD.X + board.WIDTH - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					else if(row_th == board.HEIGHT){
						Mid("<", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						for(int x	=	0; x < board.WIDTH; x++) Mid("=", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						Mid(">", screen.GAME.BOARD.X + board.WIDTH - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					else{
						for(int x	=	0; x < board.WIDTH; x++) Mid("v", screen.GAME.BOARD.X + x - 1, screen.GAME.BOARD.Y + row_th, DARKGRAY);
					}
					
					#if DEBUG
						Mid("  123456789ABCDEFG  ", screen.GAME.BOARD.X - 2, screen.GAME.BOARD.Y + board.HEIGHT + 2, DARKGRAY);
					#endif
				}
				
				Mid("LEVEL", screen.GAME.CURBLOCK.X - strlen("LEVEL"), screen.GAME.CURBLOCK.Y, DARKGRAY);
				
				Mid("SCORE", screen.GAME.CURBLOCK.X - strlen("SCORE"), screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				Mid("000000000", screen.GAME.CURBLOCK.X + 2, screen.GAME.CURBLOCK.Y + 2, DARKGRAY);
				
				Mid("Next Block", screen.GAME.CURBLOCK.X - strlen("Next block")/2, screen.GAME.CURBLOCK.Y + 5, DARKGRAY);
				
				crmove(screen.GAME.CURBLOCK.X + 3 - Digits(level), screen.GAME.CURBLOCK.Y);
				textcolor(DARKGRAY, BLACK);
				printf("%d", level);
				
				crmove(screen.GAME.CURBLOCK.X + 11 - Digits(score), screen.GAME.CURBLOCK.Y + 2);
				textcolor(DARKGRAY, BLACK);
				printf("%d", score);
				
				if(!mods.UPSIDEDOWN){
					for(row_th	=	0; row_th < board.HEIGHT; row_th++){
						for(column_th	=	0; column_th < board.WIDTH - 2; column_th++){
							Mid(board.Layers[row_th][column_th + 1][1].IsBlock ? "#" : (board.Layers[row_th][column_th + 1][0].IsBlock  ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						}
					}
				}
				
				else if(mods.UPSIDEDOWN){
					for(row_th	=	0; row_th < board.HEIGHT; row_th++){
						for(column_th	=	0; column_th < board.WIDTH - 2; column_th++){
							Mid(board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][1].IsBlock ? "#" : (board.Layers[board.HEIGHT - 1 - row_th][column_th + 1][0].IsBlock ? "x" : "."), screen.GAME.BOARD.X + column_th, screen.GAME.BOARD.Y + row_th, DARKGRAY);
						}
					}
				}
				
				page.GAMELOAD	=	false;
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
					Mid(qt_select ? "> Yes <" : "  Yes  ", 24, screen.EXIT.Y + 4, qt_select ? WHITE : DARKGRAY);
					Mid(!qt_select ? "> No <" : "  No  ", 48, screen.EXIT.Y + 4, !qt_select ? WHITE : DARKGRAY);
					Sleep(50);
				}
				
				clrscr();
			}
			
			if(page.RESETGAME){
				
				clrscr();
				
				while(page.RESETGAME){
					Mid("Are you sure you want to reset this game?", (screen.EXITSIZE.X - strlen("Are you sure you want to reset this game?")) / 2, screen.EXIT.Y, WHITE);
					Mid("Progress will be lost!", (screen.EXITSIZE.X - strlen("Progress will be lost!")) / 2, screen.EXIT.Y + 1, WHITE);
					Mid(reset ? "> Yes <" : "  Yes  ", 24, screen.EXIT.Y + 4, reset ? WHITE : DARKGRAY);
					Mid(!reset ? "> No <" : "  No  ", 48, screen.EXIT.Y + 4, !reset ? WHITE : DARKGRAY);
					Sleep(50);
				}
				
				clrscr();
			}
			
			Sleep(50);
		}
		
		while(page.BLOCKCREATION){
			if(page.PAGELOAD){
				
				EpicText("creation", 2, MAGENTA);
				
				Box(8, 12, 18, 12, WHITE, BLACK);
				
				for(row_th	=	0; row_th < 4; row_th++){
					for(column_th	=	0; column_th < 8; column_th++){
						Mid(customblocks[selection.BLOCKSEL < 3 ? selection.BLOCKSEL : 2].Layout[row_th / 2][column_th / 2].IsBlock ? "\xB2" : ".", 20 + column_th, 14 + row_th, customblocks[selection.BLOCKSEL < 3 ? selection.BLOCKSEL : 2].Layout[row_th / 2][column_th / 2].IsBlock ? customblocks[selection.BLOCKSEL < 3 ? selection.BLOCKSEL : 2].Layout[row_th / 2][column_th / 2].Color : LIGHTGRAY, row_th / 2 == selection.BLOCKCREATEY && column_th / 2 == selection.BLOCKCREATEX ? LIGHTBLUE : BLACK);
					}
				}
				
				Mid(selection.BLOCKCREATEX == 4 ? "> Is Enabled <" : "  Is Enabled  ", 33, 14, selection.BLOCKCREATEX == 4 ? WHITE : DARKGRAY);
				Mid(customblocks[selection.BLOCKSEL < 3 ? selection.BLOCKSEL : 2].IsActive ? "True " : "False", 38, 16, selection.BLOCKCREATEX == 4 ? WHITE : DARKGRAY);
				
				crmove(18, 22);
				textcolor(LIGHTGRAY, BLACK);
				printf("Blocks left: %1d", customblocks[selection.BLOCKSEL < 3 ? selection.BLOCKSEL : 2].Left);
				
				Mid("Enter - enable/disable position      Space - Change color of position", (80 - strlen("Enter - enable/disable position      Space - Change color of position")) / 2, 25, DARKGRAY);
				
				Mid(selection.BLOCKSEL == 0 ? "> Block #1 <" : "  Block #1  ", 55 - strlen("> Block #1 <") / 2, 14, selection.BLOCKCREATEX == 5 ? (selection.BLOCKSEL == 0 ? WHITE : DARKGRAY) : DARKGRAY);
				Mid(selection.BLOCKSEL == 1 ? "> Block #2 <" : "  Block #2  ", 55 - strlen("> Block #1 <") / 2, 15, selection.BLOCKCREATEX == 5 ? (selection.BLOCKSEL == 1 ? WHITE : DARKGRAY) : DARKGRAY);
				Mid(selection.BLOCKSEL == 2 ? "> Block #3 <" : "  Block #3  ", 55 - strlen("> Block #1 <") / 2, 16, selection.BLOCKCREATEX == 5 ? (selection.BLOCKSEL == 2 ? WHITE : DARKGRAY) : DARKGRAY);
				
				Mid(selection.BLOCKSEL == 3 ? "> Back <" : "  Back  ", 55 - strlen("  Back  ") / 2, 18, selection.BLOCKSEL == 3 ? WHITE : DARKGRAY);
				
				page.PAGELOAD	=	false;
			}
			
			Sleep(50);
		}
		
		
		Sleep(50);
	}
}

void Music(){
	
	srand(time(NULL));
	if(soundavailable){
		cur_song	=	5;
		
		while(!available_songs[cur_song]) cur_song	=	Random(0, max_song);
	
		while(settings.sound){
			
			PlaySound(song[cur_song], NULL, SND_ASYNC);
			
			for(int i	=	0; i < song_l[cur_song] * 10 && settings.sound; i++){
				
				if(!page.KEYS && (keys.PRESSED == 'n' || keys.PRESSED == keys.KEY[NEXTSONG])) break;
				
				Sleep(100);
			}
			
			keys.PRESSED	=	NULL;
			
			int temp	=	cur_song;
			while(temp == cur_song || !available_songs[cur_song]) cur_song	=	Random(0, max_song);
		}
		
		PlaySound(0,0,0);
	}
	
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
	if(timeleft <= 0) timeleft	=	mods.TIMERUN * 600;
	
	while(timeleft > 0 && page.GAME){
		Sleep(100);
		
		timeleft--;
	}
}

DWORD WINAPI TimerGB(void* data){
	while(page.GAME){
		rnd	=	Random(100, 300);
		
		
		while(rnd > 0  && page.GAME){
			Sleep(100);
			
			rnd--;
		}
		
		int X	=	Random(0, board.WIDTH);
		int Y	=	Random(0, board.HEIGHT - 1);
		
		board.Layers[Y][X][0].IsBlock	=	false;
		board.Layers[Y][X][0].Color	=	DARKGRAY;
	}
}


