#include "TScreen.h"

TScreen::TScreen(){
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

    rDebugWriteRect.Top = SCREEN_MAX_Y - 1;
    rDebugWriteRect.Left = 0;
    rDebugWriteRect.Bottom = SCREEN_MAX_Y - 1;
    rDebugWriteRect.Right = SCREEN_MAX_X -1;

    for (int i = 0; i < SCREEN_MAX_X; i++) {
        chiDebug[i].Attributes = BCOLOR_RED | BCOLOR_GREEN | TCOLOR_WHITE;
    }
    updated = false;

   



}
TScreen::~TScreen() {
	SetConsoleActiveScreenBuffer(hStdout);
}

void TScreen::debug_write(string s) { 
    for(auto i=0;i<s.length();i++){
        chiDebug[i].Char.AsciiChar = s[i];
    }
    for (auto i = s.length(); i < SCREEN_MAX_X; i++) {
        chiDebug[i].Char.AsciiChar = '\0';
    }
    WriteConsoleOutput(
        hNewScreenBuffer, // screen buffer to write to
        chiDebug,        // buffer to copy from
        c_DebugSize,     // col-row size of chiBuffer
        c_DebugPos,      // top left src cell in chiBuffer
        &rDebugWriteRect);  // dest. screen buffer rectangle
    this_thread::sleep_for(chrono::milliseconds(DEBUG_MSG_DELAY));
}
COORD TScreen::write_text(int x, int y, int sx, int sy, string s, int nlshift){
    COORD p{ 0, 0};
    if (x >= SCREEN_MAX_X || x < 0) return p;
    if (y >= SCREEN_MAX_Y || y < 0) return p;
    if (x + sx > SCREEN_MAX_X) sx = SCREEN_MAX_X - x;
    if (y + sy > SCREEN_MAX_Y) sy = SCREEN_MAX_Y - y;
    if (sy <= 0 || sx <= 0) return p;

    size_t px = x;
    size_t py = y;
    for (auto i = 0; i < s.length(); i++) {
        chiScreen[px + py * SCREEN_MAX_X].Char.AsciiChar = s[i];
        px += 1;
        if (px >= x + sx) {
            px = x + nlshift;
            if (px >= x + sx) break;
            py += 1;            
        }
        if (py >= y + sy) {
            break;
        }
    }
    updated = true;
    COORD pos{ px, py };
    return pos;
}
void TScreen::write_chiAttr(int x, int y, int sx, int sy, int attr) {   
    if (x >= SCREEN_MAX_X || x < 0) return;
    if (y >= SCREEN_MAX_Y || y < 0) return;
    if (x + sx > SCREEN_MAX_X) sx = SCREEN_MAX_X - x;
    if (y + sy > SCREEN_MAX_Y) sy = SCREEN_MAX_Y - y;
    if (sy <= 0 || sx <= 0) return;

    for (int px = 0; px < sx; px++)
        for (int py = 0; py < sy; py++) {
            chiScreen[(x + px) + (y + py) * SCREEN_MAX_X].Attributes = attr;
        }
    updated = true;
}
void TScreen::set_cpos(int x, int y) {
        COORD pos{ x,y };
        SetConsoleCursorPosition(hNewScreenBuffer, pos );
}
void TScreen::draw() {
    if (!updated) return;    
    WriteConsoleOutput(
        hNewScreenBuffer, // screen buffer to write to
        chiScreen,        // buffer to copy from
        c_ScreenSize,     // col-row size of chiBuffer
        c_ScreenPos,      // top left src cell in chiBuffer
        &rScreenSize);  // dest. screen buffer rectangle
    updated = false;
    memset(chiScreen, 0, sizeof(CHAR_INFO) * SCREEN_MAX_X * SCREEN_MAX_Y);
}


