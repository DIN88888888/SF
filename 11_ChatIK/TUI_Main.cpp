#include "TUI_Main.h"
#include "TScreen.h"
#include "TMainController.h"

TUI_Main::TUI_Main(TMainController* mc, TScreen* screen){
	_screen = screen;
	_mc = mc;
	_popTab = nullptr;

	PushTAB(new TUI_Tab_Main(this));
}
TUI_Main::~TUI_Main(){
	while (!tabs.empty()) {
		delete tabs.top();
		tabs.pop();
	}
}
void TUI_Main::Update() {
	_activeTab = tabs.top();
	_activeTab->Update();
	if (_popTab){		
		popTab();
	}
}
void TUI_Main::Draw() {
	_activeTab->Draw();
}
void TUI_Main::DispatchKey(TEvent_Key* e, bool& dispatched) {
	if (dispatched)return;
	if (e->keyType() == KT_KEY && e->keyValue() == KV_ESC) {
		dispatched = true;
		PopTAB();
	}
	else
	{
		_activeTab->DispatchKey(e,dispatched);
	}
}
void TUI_Main::DispatchEvent(IEvent* e, bool& dispatched) {
		if (dispatched)return;
		tabs.top()->DispatchEvent(e, dispatched);
	}
void TUI_Main::PopTAB() {	
	if (tabs.size() > 1) {
		_popTab = tabs.top();
		tabs.pop();
	}
}
void TUI_Main::popTab() {	
	delete _popTab;
	_popTab = nullptr;
	tabs.top()->ReDraw();	
}