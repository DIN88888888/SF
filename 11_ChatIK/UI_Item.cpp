#include "UI_Item.h"
#include "UI_Tab.h"
#include "TScreen.h"
#include "TMainController.h"
#include "TServer.h"

#define DEBUG(MSG) _parent->Screen()->debug_write(MSG)

/********************************************************************************/
/*******************  abstract  *************************************************/
/********************************************************************************/
IUI_Item::~IUI_Item() {};
/********************************************************************************/
IUI_Item_base::~IUI_Item_base() {};
/********************************************************************************/
IUI_Item_Selectable::~IUI_Item_Selectable() {};
void IUI_Item_Selectable::Select() {
	if (!_enabled) return;
	_state = true;
	//_parent->ReDraw();
}
void IUI_Item_Selectable::Deselect() {
	_state = false;
	//_parent->ReDraw();
}
void IUI_Item_Selectable::dispatchEvent(IEvent* e, bool& dispatched){
	if (dispatched) return;
	if (!_enabled) return;
	if (e->type() == ET_KEY) {
		if (static_cast<TEvent_Key*>(e)->keyType() == KT_ARROW && static_cast<TEvent_Key*>(e)->keyValue() == KV_ARROW_TOP) {
			dispatched = true;
			_parent->SelectNextControl();
			return;
		}
		if (static_cast<TEvent_Key*>(e)->keyType() == KT_ARROW && static_cast<TEvent_Key*>(e)->keyValue() == KV_ARROW_DOWN) {
			dispatched = true;
			_parent->SelectPrevControl();
			return;
		}
	}
}
/********************************************************************************/
IUI_Item_Action::~IUI_Item_Action() {};
void IUI_Item_Action::dispatchEvent(IEvent* e, bool& dispatched) {
	if (dispatched) return;
	if (!_enabled) return;
	if (e->type() == ET_KEY) {
		if (static_cast<TEvent_Key*>(e)->keyType() == KT_KEY && static_cast<TEvent_Key*>(e)->keyValue() == KV_ENTER) {
			dispatched = true;			
			DoAction();
			_parent->ReDraw();			
			return;
		}
	}
}
void IUI_Item_Action::DoAction() {
	if (_action && _enabled)
		_action(_parent);
}
/********************************************************************************/
IUI_Item_Btn::~IUI_Item_Btn() {};
void IUI_Item_Btn::dispatchEvent(IEvent* e, bool& dispatched) {
	this->IUI_Item_Selectable::dispatchEvent(e, dispatched);
	this->IUI_Item_Action::dispatchEvent(e, dispatched);
}
/********************************************************************************/
IUI_Item_TextEdit::~IUI_Item_TextEdit() {}
void IUI_Item_TextEdit::dispatchEvent(IEvent* e, bool& dispatched) {
	IUI_Item_Selectable::dispatchEvent(e, dispatched);
	if (dispatched) return;
	if (!_enabled) return;
	if (e->type() == ET_KEY) {
		if (static_cast<TEvent_Key*>(e)->keyType() == KT_KEY) {			
			char c = static_cast<TEvent_Key*>(e)->keyValue();
			if (_validate && _validate(c))
			{
				dispatched = true;
				if (_lenlim < 0 || _value.length() < _lenlim) {
					_value = _value + c;
					_parent->ReDraw();
					return;
				}
			}
			else if(c==8 && _value.length()>0) {
				dispatched = true;
				_value.erase(_value.length() - 1, 1);
				_parent->ReDraw();
				return;
				
			}
		}
	}
}
/********************************************************************************/
/*******************  real  *****************************************************/
/********************************************************************************/
void TUI_Item_Text::setText(string text) {
	if (_t != text) {
		_t = text;
		_parent->ReDraw();
	}
};
void TUI_Item_Text::draw() {
	if (!_enabled)return;
	LastCHPos = _parent->Screen()->write_text(_x, _y, _w, _h, _t);
}
/********************************************************************************/
void TUI_Item_Rect::setAttr(int attr) {
	if (_attr != attr) {
		_attr = attr;
		_parent->ReDraw();
	}
}
void TUI_Item_Rect::draw() {
	if (!_enabled)return;
	_parent->Screen()->write_chiAttr(_x, _y, _w, _h, _attr);
}
/********************************************************************************/
void TUI_Item_Btn::draw() {
	if (!_enabled)return;
	_txt.draw();
	if (_state) {
		_preON.draw();
		_bg.setAttr(_attrON);
	}
	else {
		_preOFF.draw();
		_bg.setAttr(_attrOFF);
	}
	_bg.draw();
}
/********************************************************************************/
void TUI_Item_ServerStatus::draw() {
	if (!_enabled)return;
		
	switch (_parent->UIM()->mc()->Server()->Status()) {
	case SRV_STATUS_FAIL:
		_txtStatus.setText("DISABLE");
		_txtText.setText("----------");
		_attrSRVt = TCOLOR_GRAY;
		break;
	case SRV_STATUS_READY:
		_txtStatus.setText("OFFLINE");
		_txtText.setText("----------");
		_attrSRVt = TCOLOR_RED;
		break;
	case SRV_STATUS_STARTED:
		_txtStatus.setText(" ONLINE");
		_txtText.setText(_parent->UIM()->mc()->Server()->Name());
		_attrSRVt = TCOLOR_GREEN;
		break;
	default:
		break;
	}
	_txtText.draw();
	_txtStatus.draw();
	if (_state) {
		_preON.draw();
		_bgText.setAttr(_attrONb | _attrONt);
		_bgStatus.setAttr(_attrONb | _attrSRVt);
	}
	else {
		_preOFF.draw();
		_bgText.setAttr(_attrOFFb | _attrOFFt);
		_bgStatus.setAttr(_attrOFFb | _attrSRVt);
	}
	_bgText.draw();
	_bgStatus.draw();
}
void TUI_Item_ServerStatus::DoAction() {
	if (!_enabled)return;
	_parent->UIM()->PushTAB(new TUI_Tab_ServerOptions(_parent->UIM()));

}
/********************************************************************************/
void TUI_Item_ClientStatus::draw() {
	if (!_enabled)return;
	IClient* cl = _parent->UIM()->mc()->ClientByID(_clid);
	if (!cl) return;
	switch (cl->Status()) {
	case CLI_STATUS_FAIL:
		_cliNamet.setText("<Error>");
		_srvNamet.setText("----------");
		_attrSRVt = TCOLOR_RED;
		break;
	case CLI_STATUS_CONNECTED:
		_cliNamet.setText(cl->Name);
		_srvNamet.setText(cl->SrvName);
		_attrSRVt = TCOLOR_GREEN;
		break;		
	default:
		break;
	}
	_cliNamet.draw();
	_srvNamet.draw();
	if (_state) {
		_preON.draw();
		_srvNamer.setAttr(_attrONb | _attrONt);
		_clinamer.setAttr(_attrONb | _attrSRVt);
	}
	else {
		_preOFF.draw();
		_srvNamer.setAttr(_attrOFFb | _attrOFFt);
		_clinamer.setAttr(_attrOFFb | _attrSRVt);
	}
	_srvNamer.draw();
	_clinamer.draw();
}
void TUI_Item_ClientStatus::DoAction() {
	if (!_enabled)return;
	IClient* cl = _parent->UIM()->mc()->ClientByID(_clid);
	if (!cl) return;
	if (cl->authorised) {
		_parent->UIM()->PushTAB(new TUI_Tab_Chat(_parent->UIM(),_clid));
	}
	else {
		_parent->UIM()->PushTAB(new TUI_Tab_ClientAuth(_parent->UIM(), _clid));
	}

}
/********************************************************************************/
void TUI_Item_TextEdit::draw() {
	if (!_enabled)return;
	_txtt.setText(_value);
	if (_state) {		
		_preON.draw();
		_capb.setAttr(attr_c_on);
		_txtb.setAttr(attr_t_on);		
	}
	else {
		_preOFF.draw();
		_capb.setAttr(attr_c_off);
		_txtb.setAttr(attr_t_off);
	}
	_capt.draw();
	_capb.draw();
	_txtt.draw();
	_txtb.draw();
	if (_state)
		_parent->UIM()->mc()->Screen()->set_cpos(_txtt.LastCHPos.X, _txtt.LastCHPos.Y);
}
/********************************************************************************/
void TUI_Item_ChatEdit::draw() {
	if (!_enabled)return;
	_msgt.setText(_value);

	_box.draw();
	_topt.draw();
	_bott.draw();
	_msgt.draw();
	if (_state)
		_parent->UIM()->mc()->Screen()->set_cpos(_msgt.LastCHPos.X, _msgt.LastCHPos.Y);
}
void TUI_Item_ChatEdit::dispatchEvent(IEvent* e, bool& dispatched) {
	IUI_Item_TextEdit::dispatchEvent(e,dispatched);
	IUI_Item_Action::dispatchEvent(e, dispatched);
}
void TUI_Item_ChatEdit::DoAction() {
	IClient* cl = _parent->UIM()->mc()->ClientByID(dynamic_cast<TUI_Tab_Chat*>(_parent)->CLID());
	if (cl) {
		string msg = _value;
		string to = "ALL";
		if (_value[0] == '~') {
			int pos = 0;
			for (pos = 0; pos < msg.length() && msg[pos] != ' '; pos++);
			to = msg.substr(1, pos -1);
			msg = msg.substr(pos+1);
		}		
		cl->SendMsg(make_shared<TEvent_net_send_Mesage>(to,msg));
		_value = "";
	}
}
/********************************************************************************/
void TUI_Item_Chat::draw() {	
	IClient* cl = _parent->UIM()->mc()->ClientByID(_clid);
	if (cl) {
		_box.draw();
		int ln = _y+_h;
		int msgcount = cl->data.MsgCount();
		int curmsg = msgcount - _msgshift -1;
		while (curmsg>=0) {
			string msg = cl->data.Msg(curmsg)->GetMessageText();
			int chs = msg.length();			
			chs = (chs+(72-(chs - 80) % 72)-80) / 72 +1;
			ln = ln - ( (chs>0)?chs:0 + 1);
			if (ln >= _y) {
				_parent->Screen()->write_text(0, ln, 80, 5, msg, 8);
				//_parent->Screen()->write_chiAttr(0, ln, 80, 5, TCOLOR_GREEN | BCOLOR_BLUE);
			}
			else {
				break;
			}
			curmsg--;
		}
	}
}