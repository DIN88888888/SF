#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include "TTree.h"

//выдернул из своего чата, многовато, зато красиво
HANDLE hStdout;
HANDLE hNewScreenBuffer;
SMALL_RECT rScreenSize;

#define SCREEN_MAX_X 80
#define SCREEN_MAX_Y 30

CHAR_INFO chiScreen[SCREEN_MAX_X * SCREEN_MAX_Y];
COORD c_ScreenSize{ SCREEN_MAX_X ,SCREEN_MAX_Y };
COORD c_ScreenPos{ 0,0 };

#define TCOLOR_BLUE      0x0001 
#define TCOLOR_GREEN     0x0002 
#define TCOLOR_RED       0x0004 
#define TCOLOR_INTENSITY 0x0008 
#define TCOLOR_GRAY TCOLOR_BLUE | TCOLOR_GREEN | TCOLOR_RED 
#define TCOLOR_WHITE TCOLOR_BLUE | TCOLOR_GREEN | TCOLOR_RED | TCOLOR_INTENSITY

#define BCOLOR_BLUE      0x0010 
#define BCOLOR_GREEN     0x0020 
#define BCOLOR_RED       0x0040 
#define BCOLOR_INTENSITY 0x0080 
#define BCOLOR_GRAY BCOLOR_BLUE | BCOLOR_GREEN | BCOLOR_RED
#define BCOLOR_WHITE BCOLOR_BLUE | BCOLOR_GREEN | BCOLOR_RED |BCOLOR_INTENSITY

using std::string;

TTree Dict;// справочник команд

void prepareScreen() {//подготовка консоли к издевательству.
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hNewScreenBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ |
        GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);
    if (hStdout == INVALID_HANDLE_VALUE || hNewScreenBuffer == INVALID_HANDLE_VALUE) {
        //todo CreateConsoleScreenBuffer failed GetLastError()        
        return;
    }
    if (!SetConsoleActiveScreenBuffer(hNewScreenBuffer)) {
        //todo SetConsoleActiveScreenBuffer failed GetLastError();
        return;
    }

    rScreenSize.Top = 0;
    rScreenSize.Left = 0;
    rScreenSize.Bottom = SCREEN_MAX_Y - 1;
    rScreenSize.Right = SCREEN_MAX_X - 1;    

    for (int i = 0; i < SCREEN_MAX_X; i++) {
        for (int j = 0; j < SCREEN_MAX_Y; j++) {
            chiScreen[i+j* SCREEN_MAX_X].Attributes = TCOLOR_WHITE;
        }
    }
    for (int i = 0; i < SCREEN_MAX_X; i++) {
        chiScreen[i + (SCREEN_MAX_X) * (SCREEN_MAX_Y - 1)].Attributes = BCOLOR_GRAY | TCOLOR_WHITE;
    }
}
void writenewline(const string& str) {//вывод команд на экран
    memcpy(chiScreen, chiScreen + SCREEN_MAX_X, sizeof(CHAR_INFO) * SCREEN_MAX_X * (SCREEN_MAX_Y - 2));
    for (int i = 0; i < SCREEN_MAX_X; i++) {
        chiScreen[i + SCREEN_MAX_X * (SCREEN_MAX_Y - 2)].Char.AsciiChar = ' ';
        chiScreen[i + SCREEN_MAX_X * (SCREEN_MAX_Y - 2)].Attributes = TCOLOR_WHITE;
    }
    for (int i = 0; i < str.length() && i < SCREEN_MAX_X; i++) {
        chiScreen[i + SCREEN_MAX_X * (SCREEN_MAX_Y - 2)].Char.AsciiChar = str[i];        
    }
}
void draw() {//обновление экрана
    WriteConsoleOutput(
        hNewScreenBuffer, // screen buffer to write to
        chiScreen,        // buffer to copy from
        c_ScreenSize,     // col-row size of chiBuffer
        c_ScreenPos,      // top left src cell in chiBuffer
        &rScreenSize);  // dest. screen buffer rectangle
}
void writeWord(const string &str) {//обновление состояния строки ввода
    string ending = Dict.GetNearestEnding(str);
    int tcolor;
    if (Dict.IsValid(str))
        tcolor = TCOLOR_GREEN;
    else
        tcolor = TCOLOR_RED;
    for (int i = 0; i < SCREEN_MAX_X; i++) {
        chiScreen[i + SCREEN_MAX_X * (SCREEN_MAX_Y - 1)].Char.AsciiChar = ' ';
        chiScreen[i + SCREEN_MAX_X * (SCREEN_MAX_Y - 1)].Attributes = BCOLOR_GRAY | TCOLOR_WHITE;
    }
    for (int i = 0; i < str.length() && i< SCREEN_MAX_X; i++) {
        chiScreen[i + SCREEN_MAX_X * (SCREEN_MAX_Y - 1)].Char.AsciiChar = str[i];
        chiScreen[i + SCREEN_MAX_X * (SCREEN_MAX_Y - 1)].Attributes = BCOLOR_GRAY | tcolor;
    }    
    for (int i = 0; i < ending.length() && i< SCREEN_MAX_X-str.length(); i++) {
        chiScreen[str.length()+i + SCREEN_MAX_X * (SCREEN_MAX_Y - 1)].Char.AsciiChar = ending[i];
        chiScreen[str.length()+i + SCREEN_MAX_X * (SCREEN_MAX_Y - 1)].Attributes = BCOLOR_GRAY | TCOLOR_BLUE;
    }    
    COORD pos = { str.length(),SCREEN_MAX_Y - 1 };
    SetConsoleCursorPosition(hNewScreenBuffer, pos);
}



int main()
{    
    //фантазии нема, что смог.
    Dict.Add("exit");
    Dict.Add("com1");
    Dict.Add("command");
    Dict.Add("commandos");
    Dict.Add("format \\\\C:");
    Dict.Add("format \\\\D:");
    Dict.Add("format \\\\E:");
    Dict.Add("format ALL");

    prepareScreen();
    bool doit = true;
    string str;
    

    draw();
    writeWord("");

    while (doit) {
        int k = _getch();
        if (k == 224) {
            k = _getch();
            //arrow - ignore
        }
        else if (k == 0) {
            k = _getch();
            //special - ignore
        }
        else {//скорее всего сюда могут попасть спец символы, можно дополнительно отфильтровать, но потом, если надо... мыж не на прод накатываем? :)
            if (k == 8 && str.length() > 0) {//удаление последнего символа
                str.erase(str.length()-1,1);
            }
            else if (k == 13) {//энтер
                string s = Dict.GetNearestWord(str);
                if (s.length() > 0) {
                    writenewline(s);
                    str = "";
                    if (s == "exit") doit = false;
                }
                else
                {
                    writenewline("Wrong Command!");
                }
            }
            else {//любой символ кроме бэкспейса и энтра прилепляем к строке.
                str = str + (char)k;                
            }
            writeWord(str);
        }
        draw();
    }
    SetConsoleActiveScreenBuffer(hStdout);
}
