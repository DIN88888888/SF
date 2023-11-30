#pragma once

#define DEBUG_MSG_DELAY 10
//статус основного цикла
#define MC_AS_STOPED 0
#define MC_AS_WORKING 1
//размер экрана в знаках
#define SCREEN_MAX_X 80
#define SCREEN_MAX_Y 30
//коды цвета для буфера символов консоли
#define TCOLOR_BLUE      0x0001 // text color contains blue.
#define TCOLOR_GREEN     0x0002 // text color contains green.
#define TCOLOR_RED       0x0004 // text color contains red.
#define TCOLOR_INTENSITY 0x0008 // text color is intensified.
#define TCOLOR_GRAY TCOLOR_BLUE | TCOLOR_GREEN | TCOLOR_RED 
#define TCOLOR_WHITE TCOLOR_BLUE | TCOLOR_GREEN | TCOLOR_RED | TCOLOR_INTENSITY

#define BCOLOR_BLUE      0x0010 // background color contains blue.
#define BCOLOR_GREEN     0x0020 // background color contains green.
#define BCOLOR_RED       0x0040 // background color contains red.
#define BCOLOR_INTENSITY 0x0080 // background color is intensified.
#define BCOLOR_GRAY BCOLOR_BLUE | BCOLOR_GREEN | BCOLOR_RED
#define BCOLOR_WHITE BCOLOR_BLUE | BCOLOR_GREEN | BCOLOR_RED |BCOLOR_INTENSITY

#define SOPT_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define SOPT_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define SOPT_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define SOPT_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define SOPT_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define SOPT_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define SOPT_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.

//коды сообщений
#define ET_NONE 0
#define ET_KEY 1
#define ET_ERRMSG 2
#define ET_DEBUGTEXT 3
#define ET_SEND_SRVNAME 4
#define ET_SEND_LOGINPASS 5
#define ET_SEND_AUTHSTATUS 6
#define ET_SEND_HISTMESSAGE 7
#define ET_SEND_MESSAGE 8
#define ET_SEND_DISCONNECT 9
#define ET_SEND_EMPTY 10

//коды клавиатуры
#define KT_KEY 0
#define KT_ARROW 1
#define KT_SPECIAL 2

#define KV_ESC 27
#define KV_ENTER 13
#define KV_F9 67

#define KV_ARROW_TOP 72
#define KV_ARROW_DOWN 80
#define KV_ARROW_LEFT 75
#define KV_ARROW_RIGHT 77

//параметры сервера по умолчанию
#define SRV_IP "127.0.0.1"
#define SRV_DEFAULT_PORT 8888

//статус цикла сервера
#define SRV_STATUS_READY 1
#define SRV_STATUS_STARTED 2
#define SRV_STATUS_STOPING 3
#define SRV_STATUS_FAIL -1

//статус цикла клиента и максимум клиентов на одно приложение
#define CLI_MAX_COUNT 5
#define CLI_STATUS_CONNECTED 1
#define CLI_STATUS_STOP 2
#define CLI_STATUS_FAIL -1

//размер буфера обмена по сети
#define DEFAULT_BUFLEN 4096
//коды разделители сообщений
#define MSG_SPLIT (char)1
#define MSG_ARG_SPLIT (char)2

//путь к паролям
#define DEFAULT_USER_FN "UserList.txt"

