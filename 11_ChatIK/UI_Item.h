/*элементы интерфейса*/
#pragma once
#include "Const.h"
#include "Predef.h"

#include "TEvent.h"

using namespace std;
/********************************************************************************/
/*******************  abstract  *************************************************/
/********************************************************************************/
class IUI_Item//основа
{
public:
	IUI_Item(IUI_Tab* parent, bool isSelectable)
		:_parent(parent), _isSelectable(isSelectable), _enabled(true)
	{}
	virtual ~IUI_Item() = 0;

	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void dispatchEvent(IEvent* e, bool& dispatched) =0;

	bool isSelectable() { return _isSelectable; }		
	bool Enabled() { return _enabled; };
	void Disable() { _enabled = false; };
	void Enable() { _enabled = true; };
protected:
	IUI_Tab* _parent;
	bool _isSelectable;
	bool _enabled;
};
/********************************************************************************/
class IUI_Item_base/*что-то имеющее размеры*/
	:public virtual IUI_Item
{
public:
	IUI_Item_base(int x, int y, int w, int h )
		:IUI_Item(nullptr,false), _x(x), _y(y), _w(w), _h(h)
	{};
	~IUI_Item_base()=0;
	int X() { return _x; };
	int Y() { return _y; };
	int W() { return _w; };
	int H() { return _h; };

protected:
	int _x;
	int _y;
	int _w;
	int _h;
	
};
/********************************************************************************/
class IUI_Item_Selectable//выделяемое
	:public virtual IUI_Item
{
public:
	IUI_Item_Selectable() 
		: IUI_Item(nullptr, true), _state(false)
	{};
	~IUI_Item_Selectable() = 0;
	void Select();
	void Deselect();	
	void dispatchEvent(IEvent* e, bool& dispatched) override;

protected:
	bool _state;
};
/********************************************************************************/
class IUI_Item_Action//можно активировать для выполнения действия
	:public virtual IUI_Item
{
public:
	IUI_Item_Action(void (*action)(IUI_Tab* t) = nullptr)
		:IUI_Item(nullptr, false), _action(action)
	{};
	~IUI_Item_Action() = 0;
	void dispatchEvent(IEvent* e, bool& dispatched) override;
	virtual void DoAction ();

protected:
	void(*_action)(IUI_Tab* t);
};
/********************************************************************************/
class IUI_Item_Btn// интерфейс кнопки
	: public virtual IUI_Item_base
	, public virtual IUI_Item_Selectable
	, public virtual IUI_Item_Action
{
public:
	IUI_Item_Btn()
		: IUI_Item(nullptr, true)
		, IUI_Item_base(0, 0, 0, 0)
		, IUI_Item_Selectable()
		, IUI_Item_Action(nullptr)
	{}
	~IUI_Item_Btn() = 0;
	void dispatchEvent(IEvent* e, bool& dispatched);
private:
};
/********************************************************************************/
class IUI_Item_TextEdit//ввод текста
	: public virtual IUI_Item_base
	, public virtual IUI_Item_Selectable	
{
public://1-8,11,12,14-26,28-254
	IUI_Item_TextEdit(string v,int MaxLen = -1, bool (validate)(char c) = [](char c) {return c >= 28 && c <= 254; })
		: IUI_Item(nullptr, true)
		, IUI_Item_base(0, 0, 0, 0)
		, IUI_Item_Selectable()		
		, _value(v)
		, _validate(validate)
		, _lenlim(MaxLen)
	{}
	~IUI_Item_TextEdit() = 0;
	void dispatchEvent(IEvent* e, bool& dispatched);
	string GetValue() { return _value; };
	void SetValue(string v) { _value = v; };
protected:
	string _value;	
	bool (*_validate)(char c);
	int _lenlim;
};
/********************************************************************************/
/*******************  real  *****************************************************/
/********************************************************************************/
class TUI_Item_Text final ://рисует текст в заданных рамках
	public IUI_Item_base
{
public:
	TUI_Item_Text(IUI_Tab* parent, int x, int y, int w, int h, string text)
		: IUI_Item(parent, false)
		, IUI_Item_base(x, y, w, h)
		, _t(text)
	{};

	void setText(string text);

	void draw() override;
	void update() override {};
	void dispatchEvent(IEvent* e, bool& dispatched) override {};
	COORD LastCHPos;
private:
	string _t;
};
/********************************************************************************/
class TUI_Item_Rect final ://окрашивает прямоугольник
	public IUI_Item_base
{
public:
	TUI_Item_Rect(IUI_Tab* parent, int x, int y, int w, int h, int attr = TCOLOR_WHITE)
		: IUI_Item(parent, false)
		, IUI_Item_base(x, y, w, h)
		, _attr(attr)
	{};

	void setAttr(int attr);

	void draw() override;
	void update() override {};
	void dispatchEvent(IEvent* e, bool& dispatched) override {};
private:
	int _attr;
};
/********************************************************************************/
class TUI_Item_Btn final//кнопка
	: public IUI_Item_Btn
{
public:
	TUI_Item_Btn(IUI_Tab* parent, int x, int y, int w, int h
		, string text
		, void (*action)(IUI_Tab* t) = nullptr
		, string prefOFF = "  "
		, string prefON = ">>"
		, int attr_OFF = BCOLOR_GRAY
		, int attr_ON = BCOLOR_WHITE
	)
		: IUI_Item(parent, true)
		, IUI_Item_base(x, y, w, h)
		, IUI_Item_Selectable()
		, IUI_Item_Action(action)
		, IUI_Item_Btn()
		, _txt(parent, x + prefOFF.length(), y, w - prefOFF.length(), h, text)
		, _preOFF(parent, x                , y, prefOFF.length(), h, prefOFF)
		, _preON(parent, x, y, prefON.length(), h, prefON)
		, _bg (parent, x, y, w, h, _attrOFF)
		, _attrOFF(attr_OFF)
		, _attrON(attr_ON)
	{};

	void draw() override;
	void update() override {};

protected:
	TUI_Item_Text _txt;
	TUI_Item_Text _preON;
	TUI_Item_Text _preOFF;
	TUI_Item_Rect _bg;
	
	int _attrOFF;
	int _attrON;
};
/********************************************************************************/
class TUI_Item_ServerStatus final//кнопка-сервер, отображает его статус
	: public IUI_Item_Btn
{
public:
	TUI_Item_ServerStatus(IUI_Tab* parent, int x, int y, int w, int h		// 7 chars for "OFF/ON line" width 
		, string prefOFF = "  "
		, string prefON = ">>"
		, int bgattr_OFF = BCOLOR_GRAY
		, int bgattr_ON = BCOLOR_WHITE
		, int tattr_OFF = 0 //black
		, int tattr_ON = 0
	)
		: IUI_Item(parent, true)
		, IUI_Item_base(x, y, w, h)
		, IUI_Item_Selectable()
		, IUI_Item_Action(nullptr)
		, IUI_Item_Btn()
		, _txtText(parent, x + prefOFF.length(), y, w - prefOFF.length() - 7, h, "Server")
		, _txtStatus(parent, x + w - 7, y, 7, h, "OFFLINE")
		, _preOFF(parent, x, y, prefOFF.length(), h, prefOFF)
		, _preON(parent, x, y, prefON.length(), h, prefON)
		, _bgText(parent, x, y, w - 7, h, bgattr_OFF)
		, _bgStatus(parent, x + w - 7, y, 7, h, TCOLOR_RED)
		, _attrOFFb(bgattr_OFF)
		, _attrONb(bgattr_ON)
		, _attrOFFt(tattr_OFF)
		, _attrONt(tattr_ON)
		, _attrSRVt(0)
	{};

	void draw() override;
	void update() override {};

	virtual void DoAction() override;

protected:
	TUI_Item_Text _txtText;
	TUI_Item_Rect _bgText;

	TUI_Item_Text _txtStatus;
	TUI_Item_Rect _bgStatus;

	TUI_Item_Text _preON;
	TUI_Item_Text _preOFF;	

	int _attrOFFb;
	int _attrONb;
	int _attrOFFt;
	int _attrONt;
	int _attrSRVt;
};
/********************************************************************************/
class TUI_Item_TextEdit//ввод текста
	:public IUI_Item_TextEdit
{
public:
	TUI_Item_TextEdit(IUI_Tab* parent, int x, int y, int w_cap, int w_txt, int h		// 7 chars for "OFF/ON line" width 
		, string caption
		, string starttext = ""
		, int editLenLim = -1
		, bool (validate)(char c) = [](char c) {return c >= 28 && c <= 254; }
		, string prefOFF = "  "
		, string prefON = ">>"
		, int attr_caption_on = BCOLOR_WHITE
		, int attr_caption_off = BCOLOR_GRAY
		, int attr_text_on = BCOLOR_WHITE
		, int attr_text_off = BCOLOR_GRAY

		, int tattr_OFF = 0 //black
		, int tattr_ON = 0
	)
		: IUI_Item(parent, true)
		, IUI_Item_base(x, y, w_cap + w_txt, h)
		, IUI_Item_Selectable()
		, IUI_Item_TextEdit(starttext, editLenLim, validate)
		, _preOFF(parent, x, y, prefOFF.length(), h, prefOFF)
		, _preON(parent, x, y, prefON.length(), h, prefON)
		, _capt(parent, x + prefOFF.length(), y, w_cap - prefOFF.length(), h, caption)
		, _capb(parent, x, y, w_cap, h, attr_caption_off)
		, _txtt(parent, x  + w_cap, y, w_txt, h, starttext)
		, _txtb(parent, x  + w_cap, y, w_txt, h, attr_text_off)
		, attr_c_on(attr_caption_on)
		, attr_c_off(attr_caption_off)
		, attr_t_on(attr_text_on)
		, attr_t_off(attr_text_off)
	{};

	void draw() override;
	void update() override {};
private:	
	TUI_Item_Text _capt;
	TUI_Item_Rect _capb;
	int attr_c_on;
	int attr_c_off;
	
	TUI_Item_Text _txtt;
	TUI_Item_Rect _txtb;
	int attr_t_on;
	int attr_t_off;

	TUI_Item_Text _preON;
	TUI_Item_Text _preOFF;
};
/********************************************************************************/
class TUI_Item_ClientStatus final//кнопка - статус клиента
	: public IUI_Item_Btn
{
public:
	TUI_Item_ClientStatus(IUI_Tab* parent, int x, int y, int w, int h		// 7 chars for "OFF/ON line" width 
		, string prefOFF = "  "
		, string prefON = ">>"
		, int bgattr_OFF = BCOLOR_GRAY
		, int bgattr_ON = BCOLOR_WHITE
		, int tattr_OFF = 0 //black
		, int tattr_ON = 0
	)
		: IUI_Item(parent, true)
		, IUI_Item_base(x, y, w, h)
		, IUI_Item_Selectable()
		, IUI_Item_Action(nullptr)
		, IUI_Item_Btn()
		, _srvNamet(parent, x + prefOFF.length(), y, w - prefOFF.length() - 12, h, "Server")
		, _cliNamet(parent, x + w - 12, y, 12, h, "")
		, _preOFF(parent, x, y, prefOFF.length(), h, prefOFF)
		, _preON(parent, x, y, prefON.length(), h, prefON)
		, _srvNamer(parent, x, y, w - 7, h, bgattr_OFF)
		, _clinamer(parent, x + w - 12, y, 12, h, TCOLOR_RED)
		, _attrOFFb(bgattr_OFF)
		, _attrONb(bgattr_ON)
		, _attrOFFt(tattr_OFF)
		, _attrONt(tattr_ON)
		, _attrSRVt(0)
	{};

	void draw() override;
	void update() override {};

	virtual void DoAction() override;
	int _clid;
protected:
	TUI_Item_Text _srvNamet;
	TUI_Item_Rect _srvNamer;

	TUI_Item_Text _cliNamet;
	TUI_Item_Rect _clinamer;

	TUI_Item_Text _preON;
	TUI_Item_Text _preOFF;

	int _attrOFFb;
	int _attrONb;
	int _attrOFFt;
	int _attrONt;
	int _attrSRVt;

	
};
/********************************************************************************/
class TUI_Item_ChatEdit// ввод текста чата
	: public IUI_Item_Action
	, public IUI_Item_TextEdit
		
	
{
public:
	TUI_Item_ChatEdit(IUI_Tab* parent, int y, int h
		, bool (validate)(char c) = [](char c) {return c >= 28 && c <= 254; }
		, int attr = BCOLOR_GRAY
	)
		: IUI_Item(parent, true)
		, IUI_Item_base(0, y, 80, h)
		, IUI_Item_Selectable()
		, IUI_Item_TextEdit("", 220, validate)
		, IUI_Item_Action()
		, _attr(attr)
		, _box(parent,0,y,80,h,attr)
		, _topt(parent, 0, y, 80, 1, "=======================    your message:      ")
		, _bott(parent, 0, y+h, 80, 1, "=================================================================")
		, _msgt(parent, 0, y+1, 80, h-2, "")
	{};

	void draw() override;
	void dispatchEvent(IEvent* e, bool& dispatched) override;
	void update() override {};
	void DoAction() override;
private:
	TUI_Item_Rect _box;
	TUI_Item_Text _topt;
	TUI_Item_Text _bott;
	TUI_Item_Text _msgt;
	int _attr;	
};
/********************************************************************************/
class TUI_Item_Chat final :// отображение сообщений чата
	public IUI_Item_base
{
public:
	TUI_Item_Chat(IUI_Tab* parent, int y, int h, int clid)
		: IUI_Item(parent, false)
		, IUI_Item_base(0, y, 80, h)
		,_clid(clid)
		,_msgshift(0)
		, _box(parent,0,y,80,h,TCOLOR_WHITE)
	{};	

	void draw() override;
	void update() override {};
	void dispatchEvent(IEvent* e, bool& dispatched) override {};
	
private:
	TUI_Item_Rect _box;
	int _clid;
	int _msgshift;

};