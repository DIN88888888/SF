/*интерфейс отрисовки*/
#pragma once
#include "Const.h"
#include "Predef.h"


using namespace std;
class TScreen//NOT thread safe!
{
public:
	TScreen();
	~TScreen();	

	void debug_write(string s);
	//записать текст в заданных координатах с переносом строки
	//вторая и последующие строки могут быть сдвинуты от левого края
	COORD write_text(int x, int y, int sx, int sy, string s, int nlshift = 0);
	//установка параметров цвета в прямоугольнике
	void write_chiAttr(int x, int y, int sx, int sy, int attr);
	//установить курсор
	void set_cpos(int x, int y);
	//рисовать
	void draw();
private:
	HANDLE hStdout;
	HANDLE hNewScreenBuffer;

	SMALL_RECT rScreenSize;
	COORD c_ScreenSize{ SCREEN_MAX_X ,SCREEN_MAX_Y };
	COORD c_ScreenPos{ 0,0 };
	CHAR_INFO chiScreen[SCREEN_MAX_X * SCREEN_MAX_Y];
	bool updated;

	SMALL_RECT rDebugWriteRect;
	CHAR_INFO chiDebug[80];
	COORD c_DebugSize{ SCREEN_MAX_X ,1 };
	COORD c_DebugPos{ 0,0 };
};



