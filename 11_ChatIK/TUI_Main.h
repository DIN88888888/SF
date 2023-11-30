/*=== ������ ���������� ===*/
/*��������� � ����� "�����-�������� � ������ ������� ����������"*/
/*�������� ������ ���� � ���� ������!*/
#pragma once
#include "Const.h"
#include "Predef.h"

#include "Exceptions.h"
#include "TEvent.h"

#include "UI_Tab.h"
/*������� ���������� ���������� - ��������� ���������*/
class TUI_Main
{
public:
	TUI_Main(TMainController* mc, TScreen* screen);
	~TUI_Main();

	TScreen* Screen() { return _screen; }
	TMainController* mc() { return _mc; }
	void Update();//�������� ���������
	void Draw();//����������
	void DispatchKey(TEvent_Key* e, bool& dispatched);//��������� ����������
	void DispatchEvent(IEvent* e, bool& dispatched);//��������� ����������
	void PushTAB(IUI_Tab* nTab) { tabs.push(nTab);};//������� �������
	void PopTAB();//������� �������
	IUI_Tab* ActiveTab() { return _activeTab; };//�������� �������
	void ReDraw() { tabs.top()->ReDraw(); };//��������� ������������ �����
private:
	TScreen* _screen;
	TMainController* _mc;
	stack<IUI_Tab*> tabs;//�������
	IUI_Tab* _activeTab;
	IUI_Tab* _popTab;//��������� �������
	void popTab();
	
};

