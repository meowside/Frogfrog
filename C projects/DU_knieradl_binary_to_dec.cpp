#include <stdio.h>
#include <windows.h>
#include <conio.h>
#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2

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


void Box(int _height, int _width, int X, int Y, int _fg, int _bg);
void textattr(int _attr);
void textcolor(int _fg, int _bg);
void textforeground(int _color);
void textbackground(int _color);
void crmove(int x, int y);
void clrscr();
void setcursortype(int type);
void Mid(const char *tx, short _hor, short _vert, const char *vert, const char *hori, int _fg, int _bg);
void Mid(char TEXT, int x, int y, int color);

char * Input(char *_tx);
long long Conversion(char *_tx);
void Print(long long vys, char *_tx);

void OnLoad(){
	clrscr();
	setcursortype(_NOCURSOR);
}

int main(){
	OnLoad();
	char txt[65];
	Print(Conversion(Input(txt)), txt);
}

char * Input(char *_tx){
	
	int selection = 0;
	bool IsZeroOne = false;
	bool wronginput = false;
	char *tmp = (char*)malloc(sizeof(char) * 30);
	
	while(!IsZeroOne){
		
		selection = 0;
		IsZeroOne = true;
		
		if(wronginput) Mid("Spatne zadani!", 0, 3, "top", "center", LIGHTRED, BLACK);
		
		wronginput = false;
		
		for(int column = 0; column < 64; ){
			
			Box(3, 42, 19, 11, selection == 0 ? WHITE : DARKGRAY, BLACK);
			Mid("Zadej retezec", 0, -3, "center", "center", WHITE, BLACK);
			
			for(int i = column - 40 > 0 ? column - 40 : 0; i < column; i++){
				Mid(_tx[i], 20 + i - (column - 40 > 0 ? column - 40 : 0), 12, DARKGRAY);
			}
			
			sprintf(tmp, "Zbyvajici znaky: %2d", 63 - column);
			Mid(tmp, 0, 5, "center", "center", DARKGRAY, BLACK);
			
			Mid(selection == 0 ? "  Continue  " : "> Continue <", 0, -5, "bottom", "center",selection == 0 ? DARKGRAY : WHITE, BLACK);
			
			int key = _getch();
			
			if(key == 224) key += _getch();
			if(key == 0) key -= _getch();
			
			while(_kbhit()) _getch();
			
			if(key == 296){
				if(selection > 0) selection--;
			}
			
			else if(key == 304){
				if(selection < 1) selection++;
			}
			
			else if(key == 13){
				if(selection == 0){
					selection++;
				}
				
				else{
					_tx[column] = '\0';
					break;
				}
			}
			else if(key == 8 && selection == 0){
				if(column > 0){
					column--;
					_tx[column] = ' ';	
				}
			}
			else if(column < 63  && selection == 0){
				_tx[column] = key;
				column++;
				if(column == 63 && selection == 0){
					_tx[column] = '\0';
					selection = 1;
				}
			}
		}
		
		if(strlen(_tx) == 0){
			IsZeroOne = false;
			wronginput = true;
		}
		
		for(int column = 0; column < strlen(_tx) && IsZeroOne; column++){
			if(_tx[column] != '0' && _tx[column] != '1'){
				IsZeroOne = false;
				wronginput = true;
			}
		}
	}
	
	return _tx;
}


long long Conversion(char *_tx){
	long long vys = 0;
	int temp = 0;
	long long count = 1;
	
	for(int column = strlen(_tx) - 1; column >= 0; column--){
		temp = (_tx[column] == '0' ? 0 : 1);
		vys += temp * count;
		count *= 2;
	}
	
	return vys;
}

void Print(long long vys, char *_tx){
	clrscr();
	Box(10, (strlen(_tx) < 38 ? 40 : strlen(_tx) + 4), (strlen(_tx) < 38 ? 20 : (80 - strlen(_tx) - 3) / 2), 6, WHITE, BLACK);
	crmove(40 - (strlen("Vysledek: ") + 20) / 2, 8);
	printf("Vysledek: %20lld", vys);
	crmove(40 - strlen(_tx) / 2, 12);
	printf("%s", _tx);
	crmove(0, 23);
	system("pause");
	
}


void Box(int _height, int _width, int X, int Y, int _fg, int _bg){
	for(int row	=	0; row < _height; row++){
		for(int column	=	0; column < _width; column++){
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

void Mid(const char *tx, short _hor, short _vert, const char *vert, const char *hori, int _fg, int _bg){
	HANDLE h	=	GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(h, &bufferInfo);
    SMALL_RECT& winInfo	=	bufferInfo.srWindow;
    COORD windowSize	=	{ winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1};
    
    int horizontal = 0, vertical = 0;
    
    if(strcmp("left", hori) == 0) horizontal = (0 + _hor < 0 ? 0 : (0 + _hor > windowSize.X ? windowSize.X - strlen(tx) : 0 + _hor));
    else if(strcmp("right", hori) == 0) horizontal = (windowSize.X - strlen(tx) + _hor < 0 ? 0 : (windowSize.X + strlen(tx) + _hor > windowSize.X ? windowSize.X - strlen(tx) : windowSize.X + _hor));
    else if(strcmp("center", hori) == 0) horizontal = (windowSize.X / 2 + _hor < 0 ? 0 : (windowSize.X / 2 + _hor + strlen(tx) > windowSize.X ? windowSize.X - strlen(tx) : ( windowSize.X - strlen(tx)) / 2 + _hor));
    
    if(strcmp("top", vert) == 0) vertical = (0 + _vert < 0 ? 0 : (0 + _vert > windowSize.Y ? windowSize.Y : 0 + _vert));
    else if(strcmp("bottom", vert) == 0) vertical = (windowSize.Y + _vert > windowSize.Y ? windowSize.Y : (windowSize.Y + _vert < 0 ? 0 : windowSize.Y + _vert));
    else if(strcmp("center", vert) == 0) vertical = (windowSize.Y / 2 + _vert < 0 ? 0 : (windowSize.Y / 2 + _vert > windowSize.Y ? windowSize.Y : windowSize.Y / 2 + _vert));
    
    crmove(horizontal , vertical);
    textcolor(_fg, _bg);
    puts(tx);
}

void Mid(char TEXT, int x, int y, int color){
	textcolor(color, BLACK);
	crmove(x, y);
	
	putchar(TEXT);
}

