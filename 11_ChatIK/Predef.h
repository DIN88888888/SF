#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <memory>
#include <chrono>
#include <conio.h>

#include <thread>
#include <mutex>

#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <stack>

#include <algorithm>

class TMainController;
class IUI_Tab;
class IUI_Item;
class TUI_Main;
class TScreen;
class TServer;
class IClient;

time_t gettime();

#pragma warning( disable : 4267)
