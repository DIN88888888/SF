/*=== модули интерфейса ===*/
/*выполнены в стиле "ретро-футуризм с лёгкими нотками извращений"*/
/*работает только если в него верить!*/
#pragma once
#include "Const.h"
#include "Predef.h"

#include "Exceptions.h"
#include "TEvent.h"

#include "UI_Tab.h"
/*главный обработчик интерфейса - управляет вкладками*/
class TUI_Main
{
public:
	TUI_Main(TMainController* mc, TScreen* screen);
	~TUI_Main();

	TScreen* Screen() { return _screen; }
	TMainController* mc() { return _mc; }
	void Update();//обновить состояние
	void Draw();//нарисовать
	void DispatchKey(TEvent_Key* e, bool& dispatched);//сообщения клавиатуры
	void DispatchEvent(IEvent* e, bool& dispatched);//сообщения приложения
	void PushTAB(IUI_Tab* nTab) { tabs.push(nTab);};//создать веладку
	void PopTAB();//удалить вкладку
	IUI_Tab* ActiveTab() { return _activeTab; };//активная вкладка
	void ReDraw() { tabs.top()->ReDraw(); };//требуется перерисовать экран
private:
	TScreen* _screen;
	TMainController* _mc;
	stack<IUI_Tab*> tabs;//вкладки
	IUI_Tab* _activeTab;
	IUI_Tab* _popTab;//удаляемая вкладка
	void popTab();
	
};

