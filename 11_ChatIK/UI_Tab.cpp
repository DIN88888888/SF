#include "UI_Tab.h"
#include "TUI_Main.h"
#include "TScreen.h"
#include "TClient.h"
#include "TMainController.h"

IUI_Tab::IUI_Tab(TUI_Main* UIM)
	:hintRect(this, 0, 0, 80, 1, BCOLOR_GRAY)
	, hintText(this, 0, 0, 80, 1, "")
{
	_selectedItem = -1;
	_uim = UIM;
	_redraw = true;
}
IUI_Tab::~IUI_Tab() {
	for (auto i : _items) {
		delete i;
	}
}
void IUI_Tab::AddControl(string name, IUI_Item* item) {
	_items.push_back(item);
	if (_items.back()->isSelectable())
		AddSelectableControl(dynamic_cast<IUI_Item_Selectable*>(_items.back()));
	_Nameditems.emplace(name, item);
}
void IUI_Tab::AddSelectableControl(IUI_Item_Selectable* item) {
	_selectableItems.push_back(item);
	if (_selectedItem == -1) {
		_selectedItem = 0;		
		_selectableItems.at(0)->Select();
	}
}
TScreen* IUI_Tab::Screen() {
	return _uim->Screen();
}
void IUI_Tab::Update() {
	TabUpdate();
	for (auto i : _items) {
		i->update();
	}
}
void IUI_Tab::Draw() {
	if (!_redraw) return;
	UIM()->mc()->Screen()->debug_write("draw");
	UIM()->mc()->Screen()->set_cpos(0, 0);
	hintRect.draw();
	hintText.draw();
	for (auto i : _items) {
		i->draw();
	}
	_redraw = false;
}
void IUI_Tab::DispatchKey(TEvent_Key* e, bool& dispatched) {
	if (dispatched) return;
	if (_selectableItems.size() > 0 && _selectedItem>=0) {
		_selectableItems[_selectedItem]->dispatchEvent(e, dispatched);
	}
}
void IUI_Tab::selectControl(int n) {
	int old_selectedItem = _selectedItem;

	if (_selectableItems.size() == 0)return;
	if (_selectedItem < 0)_selectedItem = 0;

	int step = n - _selectedItem;

	if (step > 0) step = step / abs(step);
	if (step == 0) step = 1;

	_selectableItems[_selectedItem]->Deselect();
	_selectedItem = n;

	if (_selectedItem < 0)_selectedItem = _selectableItems.size() - 1;	
	if (_selectedItem >= _selectableItems.size())_selectedItem = 0;

	for (int i = 0;  (step > 0 && i < (int)_selectableItems.size()) || (step < 0 && i > ((int)_selectableItems.size() * -1)); i += step) {
		int pos = (_selectableItems.size() + i + _selectedItem) % _selectableItems.size();
		if (_selectableItems[pos]->Enabled()) {
			_selectableItems[pos]->Select();
			_selectedItem = pos;
			if(old_selectedItem!= _selectedItem) ReDraw();
			return;			
		}
	}
	_selectedItem = -1;
}
IUI_Item* IUI_Tab::GetControl(int n) {
	if (n > 0 && n <= _items.size()) return nullptr;
	return _items[n];
}
IUI_Item* IUI_Tab::GetControl(string name) {
	return _Nameditems[name];
}
void IUI_Tab::setHintText(string txt) {
	hintText.setText(txt);
}

/********************************************************************************/
TUI_Tab_Main::TUI_Tab_Main(TUI_Main* UIM) :IUI_Tab(UIM) {
	
	conncount = -1;
	setHintText("   Arrows = select option;   Enter = execute");
	AddControl("rTitle", new TUI_Item_Rect(this, 10, 8, 35, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tTitle", new TUI_Item_Text(this, 10, 8, 35, 1, "    == ChatIK =="));

	AddControl("rSrv", new TUI_Item_Rect(this, 10, 10, 35, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tSrv", new TUI_Item_Text(this, 10, 10, 35, 1, "Server:"));

	AddControl("srvStat", new TUI_Item_ServerStatus(this, 10, 11, 35, 1));

	AddControl("rCli", new TUI_Item_Rect(this, 10, 13, 35, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tCli", new TUI_Item_Text(this, 10, 13, 35, 1, "Chats:"));

	for (int i = 0; i < CLI_MAX_COUNT; i++) {
		//empty
		AddControl("rcle" + to_string(i), new TUI_Item_Rect(this, 10, 14 + i, 35, 1, TCOLOR_INTENSITY));
		AddControl("tcle" + to_string(i), new TUI_Item_Text(this, 10, 14 + i, 35, 1, "  <empty slot>"));
		//create
		AddControl("bconn" + to_string(i), new TUI_Item_Btn(this, 10, 14 + i, 35, 1, "Connect", [](IUI_Tab* t) {t->UIM()->PushTAB(new TUI_Tab_ClientConnection(t->UIM())); }));
		//client   TUI_Item_ClientStatus
		AddControl("cliStat" + to_string(i), new TUI_Item_ClientStatus(this, 10, 14 + i, 35, 1 ));
		//AddControl("tmpbtn", new TUI_Item_Btn(this, 12, 23, 15, 1, "Btn 1"));
	}

	AddControl("bExit", new TUI_Item_Btn(this, 10, 21, 35, 1, "Exit", [](IUI_Tab* t) {
		t->UIM()->mc()->Exit(); }));
}
void TUI_Tab_Main::TabUpdate() {
	if (conncount != UIM()->mc()->ConnectionCount())
	{
		conncount = UIM()->mc()->ConnectionCount();
		for (int i = 0; i < min(conncount+1, CLI_MAX_COUNT); i++)	{
			GetControl("rcle" + to_string(i))->Disable();
			GetControl("tcle" + to_string(i))->Disable();
			
		}		
		for (int i = 0; i < CLI_MAX_COUNT; i++) {
			GetControl("bconn" + to_string(i))->Disable();
			GetControl("cliStat" + to_string(i))->Disable();
		}
		for (int i = 0; i < conncount; i++) {
			GetControl("cliStat" + to_string(i))->Enable();
			dynamic_cast<TUI_Item_ClientStatus*>(GetControl("cliStat" + to_string(i)))->_clid = _uim->mc()->Client(i)->id();
		}
		if (conncount < CLI_MAX_COUNT) {
			GetControl("bconn" + to_string(conncount))->Enable();
		}
		selectControl(_selectedItem);
	}

}
/********************************************************************************/
TUI_Tab_ServerOptions::TUI_Tab_ServerOptions(TUI_Main* UIM) :IUI_Tab(UIM) {

	setHintText("   Esc = back;   Arrows = select option;   Enter = execute");
	AddControl("rTitle", new TUI_Item_Rect(this, 10, 8, 35, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tTitle", new TUI_Item_Text(this, 10, 8, 35, 1, "    == ChatIK =="));

	AddControl("editName", new TUI_Item_TextEdit(this, 12, 12, 9, 11, 1, "  Name:","ChatIK",10));
	//GetControl("btnStop")->Disable();
	AddControl("rName", new TUI_Item_Rect(this, 12, 12, 20, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tName", new TUI_Item_Text(this, 12, 12, 20, 1, ""));

	AddControl("editPort", new TUI_Item_TextEdit(this, 12, 13, 9, 11, 1, "  Port:",to_string(SRV_DEFAULT_PORT),4, [](char c) {return c >= 48 && c <= 57; }));
	AddControl("rPort", new TUI_Item_Rect(this, 12, 13, 20, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tPort", new TUI_Item_Text(this, 12, 13, 20, 1, ""));
	//GetControl("rPort")->Disable();

	AddControl("btnStart", new TUI_Item_Btn(this, 12, 14, 15, 1, "Start"
		, [](IUI_Tab* t) {
			t->UIM()->mc()->Server()->Start(stoi(dynamic_cast<TUI_Item_TextEdit*>(t->GetControl("editPort"))->GetValue()),
											     dynamic_cast<TUI_Item_TextEdit*>(t->GetControl("editName"))->GetValue());
		}
	));
	/*AddControl("btnStop", new TUI_Item_Btn(this, 12, 14, 15, 1, "Stop"
		, [](IUI_Tab* t) {
			t->UIM()->mc()->Server()->Stop();
		}
	));*/
	//GetControl("btnStop")->Disable();	
	
	AddControl("btnBack", new TUI_Item_Btn(this, 12, 15, 15, 1, "Back", [](IUI_Tab* t) {t->UIM()->PopTAB(); }));
}
void TUI_Tab_ServerOptions::TabUpdate() {
	switch (_uim->mc()->Server()->Status()) {
	case SRV_STATUS_READY:
		GetControl("btnStart")->Enable();
		//GetControl("btnStop")->Disable();
		GetControl("editPort")->Enable();
		GetControl("editName")->Enable();
		GetControl("rName")->Disable();
		GetControl("tName")->Disable();
		GetControl("rPort")->Disable();
		GetControl("tPort")->Disable();
		break;
	case SRV_STATUS_STARTED:
		GetControl("btnStart")->Disable();
		GetControl("editPort")->Disable();
		GetControl("editName")->Disable();
		//GetControl("btnStop")->Enable();
		GetControl("rName")->Enable();		
		GetControl("tName")->Enable();
		dynamic_cast<TUI_Item_Text*>(GetControl("tName"))->setText(string("  Name:") + UIM()->mc()->Server()->Name());
		GetControl("rPort")->Enable();
		GetControl("tPort")->Enable();
		dynamic_cast<TUI_Item_Text*>(GetControl("tPort"))->setText(string("  Port:")+to_string(UIM()->mc()->Server()->Port()));
		break;
	default:
		GetControl("btnStart")->Disable();
		//GetControl("btnStop")->Disable();
		GetControl("editPort")->Disable();
		GetControl("editName")->Disable();
		GetControl("rName")->Disable();
		GetControl("tName")->Disable();
		GetControl("rPort")->Disable();
		GetControl("tPort")->Disable();
		break;
	}
	selectControl(_selectedItem);
	
}
/********************************************************************************/
TUI_Tab_ClientConnection::TUI_Tab_ClientConnection(TUI_Main* UIM) :IUI_Tab(UIM) {
	setHintText("   Esc = back;   Arrows = select option;   Enter = execute");
	AddControl("rTitle", new TUI_Item_Rect(this, 10, 8, 35, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tTitle", new TUI_Item_Text(this, 10, 8, 35, 1, "    New connection:"));

	AddControl("editIP", new TUI_Item_TextEdit(this, 10, 10, 9, 21, 1, "    IP:", "127.0.0.1", 15, [](char c) {return (c >= 48 && c <= 57) || c==46; }));

	AddControl("editPort", new TUI_Item_TextEdit(this, 10, 11, 9, 21, 1, "  Port:", to_string(SRV_DEFAULT_PORT), 4, [](char c) {return c >= 48 && c <= 57; }));

	AddControl("btnStop", new TUI_Item_Btn(this, 10, 13, 15, 1, "Connect", [](IUI_Tab* t) {
			if (t->UIM()->mc()->CreateClientConnection(dynamic_cast<TUI_Item_TextEdit*>(t->GetControl("editIP"))->GetValue(),
													   dynamic_cast<TUI_Item_TextEdit*>(t->GetControl("editPort"))->GetValue())) {
				t->UIM()->PopTAB();
				t->UIM()->PushTAB(new TUI_Tab_ClientAuth(t->UIM(), t->UIM()->mc()->Client()->id()));				
			}
			else {
				dynamic_cast<TUI_Item_Text*>(t->GetControl("tConStat"))->setText(string("  Server: connection error"));
				dynamic_cast<TUI_Item_Rect*>(t->GetControl("rConStat"))->setAttr(TCOLOR_RED);
			}

		})
	);

	AddControl("rConStat", new TUI_Item_Rect(this, 10, 14, 30, 1, 0));
	AddControl("tConStat", new TUI_Item_Text(this, 10, 14, 30, 1, ""));
}
/********************************************************************************/
TUI_Tab_ClientAuth::TUI_Tab_ClientAuth(TUI_Main* UIM, int clid) :IUI_Tab(UIM), clID(clid) {
	setHintText("   Esc = back;   Arrows = select option;   Enter = execute");
	AddControl("rTitle", new TUI_Item_Rect(this, 10, 8, 35, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tTitle", new TUI_Item_Text(this, 10, 8, 35, 1, ""));

	AddControl("rErr", new TUI_Item_Rect(this, 15, 15, 35, 1, 0));
	AddControl("tErr", new TUI_Item_Text(this, 15, 15, 35, 1, ""));

	AddControl("editName", new TUI_Item_TextEdit(this, 10, 10, 11, 19, 1, "    Name:", "", 12));

	AddControl("editPass", new TUI_Item_TextEdit(this, 10, 11, 11, 19, 1, "Password:", "", 4, [](char c) {return c >= 48 && c <= 57; }));
	AddControl("btnLogin", new TUI_Item_Btn(this, 10, 12, 15, 1, "Send", [](IUI_Tab* t) {
			IClient* cl = t->UIM()->mc()->ClientByID(static_cast<TUI_Tab_ClientAuth*>(t)->clID);
			if (cl) {
				cl->SendMsg(
					make_shared<TEvent_net_send_LoginPass>(
						dynamic_cast<TUI_Item_TextEdit*>(t->GetControl("editName"))->GetValue(),
						dynamic_cast<TUI_Item_TextEdit*>(t->GetControl("editPass"))->GetValue()
					)
				);
			}
		})
	);
}
void TUI_Tab_ClientAuth::TabUpdate() {
	IClient* cl = UIM()->mc()->ClientByID(clID);
	if (cl) {
		dynamic_cast<TUI_Item_Text*>(GetControl("tTitle"))->setText(string("  Authorization on ") + cl->SrvName);
	}
	else {
		UIM()->PopTAB();
	}

}
void TUI_Tab_ClientAuth::DispatchEvent(IEvent* e, bool& dispatched) {
	if (dispatched) return;
	if (e->type() == ET_SEND_AUTHSTATUS) {
		if (static_cast<TEvent_net_send_AuthStatus*>(e)->CLID() == clID) {
			dispatched = true;
			if (static_cast<TEvent_net_send_AuthStatus*>(e)->Stat()) {
				UIM()->PopTAB();
				UIM()->PushTAB(new TUI_Tab_Chat(UIM(), clID));
			}
			else
			{
				dynamic_cast<TUI_Item_Text*>(GetControl("tErr"))->setText("login fails");
				dynamic_cast<TUI_Item_Rect*>(GetControl("rErr"))->setAttr(BCOLOR_GRAY | TCOLOR_RED);
				
			}
		}
		
	}
}
/********************************************************************************/
TUI_Tab_Chat::TUI_Tab_Chat(TUI_Main* UIM, int clid) :IUI_Tab(UIM), clID(clid) {
	setHintText("Esc=back;Arrows=select;Enter=send;pgUp/pgDown=scroll;'~Name 'to whisp;F9=exit");
	IClient* cl = UIM->mc()->ClientByID(clID);

	AddControl("rTitle", new TUI_Item_Rect(this, 0, 3, 80, 1, BCOLOR_GRAY | TCOLOR_BLUE));
	AddControl("tTitle", new TUI_Item_Text(this, 0, 3, 80, 1, "            ====================  " + cl->SrvName + "  ===================="));

	AddControl("editMsg", new TUI_Item_ChatEdit(this, 25, 5));

	AddControl("chat", new TUI_Item_Chat(this, 4, 21, clid));	
}
void TUI_Tab_Chat::TabUpdate() {
	IClient* cl = UIM()->mc()->ClientByID(clID);
	if (cl) {
		
	}
	else {
		UIM()->PopTAB();
	}

}
void TUI_Tab_Chat::DispatchEvent(IEvent* e, bool& dispatched) {
	if (dispatched) return;
	if (e->type() == ET_SEND_HISTMESSAGE) {
		if (static_cast<TEvent_net_send_HistMesage*>(e)->CLID() == clID) {
			dispatched = true;
			ReDraw();			
		}
	}
	if (e->type() == ET_SEND_DISCONNECT) {
		if (static_cast<TEvent_net_send_AuthStatus*>(e)->CLID() == clID) {
			dispatched = true;
			UIM()->PopTAB();
		}

	}
}
void TUI_Tab_Chat::DispatchKey(TEvent_Key* e, bool& dispatched) {
	if (dispatched) return;
	if (e->keyType() == KT_SPECIAL && e->keyValue() == KV_F9) {
		dispatched = true;
		IClient* cl = UIM()->mc()->ClientByID(clID);
		if (cl) {
			cl->SendMsg(make_shared<TEvent_net_send_Disconnect>(cl->Name, 0));
		}
		_uim->PopTAB();
	}
	IUI_Tab::DispatchKey(e, dispatched);
}
