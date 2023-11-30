/*описание всех видов вкладок и размещение на них элементов интерфейса*/
#pragma once

#include "Const.h"
#include "Predef.h"

#include "UI_Item.h"

using namespace std;

class IUI_Tab
{
public:
	IUI_Tab(TUI_Main* UIM);
	virtual ~IUI_Tab() = 0;

	void AddControl(string name, IUI_Item* item);
	void AddSelectableControl(IUI_Item_Selectable* item);
	void SelectNextControl() {	selectControl( _selectedItem - 1); };
	void SelectPrevControl() {	selectControl( _selectedItem + 1); };
	void selectControl(int n);

	IUI_Item* GetControl(int n);
	IUI_Item* GetControl(string name);

	TScreen* Screen();

	void Update();	
	void Draw();

	void ReDraw() { _redraw = true; };
	virtual void DispatchKey(TEvent_Key* e, bool& dispatched);
	virtual void DispatchEvent(IEvent* e, bool& dispatched) {};

	void setHintText(string txt);
	TUI_Main* UIM() { return _uim; };
protected:
	virtual void TabUpdate() {};

	TUI_Main* _uim;
	vector < IUI_Item* > _items;
	map <string, IUI_Item* > _Nameditems;

	vector <IUI_Item_Selectable* > _selectableItems;
	int _selectedItem;	

	bool _redraw;

	TUI_Item_Rect hintRect;
	TUI_Item_Text hintText;
};
/********************************************************************************/
class TUI_Tab_Main
	:public IUI_Tab
{
public:
	TUI_Tab_Main(TUI_Main* UIM);	
private:
	void TabUpdate() override;
	int conncount;

};
/********************************************************************************/
class TUI_Tab_ServerOptions
	:public IUI_Tab
{
public:
	TUI_Tab_ServerOptions(TUI_Main* UIM);	
private:
	void TabUpdate() override;
};
/********************************************************************************/
class TUI_Tab_ClientConnection
	:public IUI_Tab
{
public:
	TUI_Tab_ClientConnection(TUI_Main* UIM);
private:
	

};
/********************************************************************************/
class TUI_Tab_ClientAuth
	:public IUI_Tab
{
public:
	TUI_Tab_ClientAuth(TUI_Main* UIM, int clid);
	void DispatchEvent(IEvent* e, bool& dispatched) override;
private:
	void TabUpdate() override;
	int clID;

};
/********************************************************************************/
class TUI_Tab_Chat
	:public IUI_Tab
{
public:
	TUI_Tab_Chat(TUI_Main* UIM, int clid);
	void DispatchKey(TEvent_Key* e, bool& dispatched) override;
	void DispatchEvent(IEvent* e, bool& dispatched) override;
	auto CLID() { return clID; };
private:
	void TabUpdate() override;
	int clID;

};

