/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                              RR_xoft 0-189_air                             */
/*                                                                            */
/*                  (C) Copyright 2021 - 2025 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* tui.h / 0-189_air                                                          */
/*----------------------------------------------------------------------------*/
//
// Text based user interface.
//
/*----------------------------------------------------------------------------*/


#ifndef __TUI_H__
#define __TUI_H__

#include <unistd.h>
#include <sys/ioctl.h>

#include <vector>

#include "common/types.h"


using namespace std;
using namespace RR_xoft;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


    
    
/*----------------------------------------------------------------------------*/
    
void s_handle_Winch(sInt_32 sig);

    
/*----------------------------------------------------------------------------*/

typedef std::vector<sString> Strings_vector;

    
/*----------------------------------------------------------------------------*/
// sContext

    class sContext
    {
    public:
	sContext() { /* nothing */ }

	sInt_32 kb_Hit(void) const;

	void clear_Screen(void) const;
	
	void draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height) const;
	void draw_PlainRectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height) const;	
	void draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, bool inverse = false) const;

	void draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, sInt_32 background) const;
	void draw_PlainRectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, sInt_32 background) const;
	void draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, sInt_32 background, const sString &title, bool inverse = false) const;	

	void draw_Text(sInt_32 x, sInt_32 y, const sString &text, bool inverse = false) const;
	void draw_Text(sInt_32 x, sInt_32 y, const sString &text, sInt_32 background, bool inverse = false) const;
	void draw_Text(sInt_32 x, sInt_32 y, const sString &text, sInt_32 text_color, sInt_32 background, bool inverse) const;	
    };


/*----------------------------------------------------------------------------*/
// sWindow

    class sWindow
    {
    public:
	sWindow() { /* nothing */ }
        sWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, bool focusable = false)
	    : m_context(&context)
	    , m_x(x)
	    , m_y(y)
	    , m_width(width)
	    , m_height(height)
	    , m_title(title)
	    , m_focusable(focusable)
	    , m_focused(false)
	{
	    // nothing
	}

	virtual void redraw(void) const;
	virtual void resize(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height);	    

    public:
	sContext *m_context;
	
	sInt_32 m_x, m_y;
	sInt_32 m_width, m_height;

	sString m_title;
	
	bool m_focusable;
	bool m_focused;
    };
    

/*----------------------------------------------------------------------------*/
// sMultilineWindow

    class sMultilineWindow
	: public sWindow
    {
    public:
        sMultilineWindow()
	    : sWindow()
	{ /* nothing */ }

        sMultilineWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, bool focusable = false)
	: sWindow(context, x, y, width, height, title, focusable)
	, m_lines(height - 2)
	, m_curr_line(0)
	{
	    for (sInt_32 i = 0; i < m_lines; ++i)
	    {
		m_Texts.push_back("");
	    }
	}

	virtual void redraw(void) const;	
	
	void set_Text(const sString &text);
	sString read_KeyboardString(void);	

    public:
	sInt_32 m_lines;
	sInt_32 m_curr_line;
	
	Strings_vector m_Texts;
    };

    
/*----------------------------------------------------------------------------*/
// sStatusWindow

    class sStatusWindow
	: public sWindow
    {
    public:
        sStatusWindow()
	    : sWindow()
	{ /* nothing */ }

        sStatusWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, bool focusable = false)
	: sWindow(context, x, y, width, height, title, focusable)
	{ /* nothing */ }

	virtual void redraw(void) const;	
	
	void set_Text(const sString &text);
	sString read_KeyboardString(void);	

    public:
	sString m_text;
    };


/*----------------------------------------------------------------------------*/
// sSignalWindow

    class sSignalWindow
	: public sWindow
    {
    public:
        sSignalWindow()
	    : sWindow()
	{ /* nothing */ }

        sSignalWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, bool focusable = false)
	: sWindow(context, x, y, width, height, title, focusable)
	, m_signal_color(-1)
	{ /* nothing */ }

	virtual void redraw(void) const;	

	void set_Signal(sInt_32 signal_color, const sString &text);
	
	void set_SignalGreen(const sString &text);	
	void set_SignalYellow(const sString &text);
	void set_SignalRed(const sString &text);	

    public:
	sInt_32 m_signal_color;
	sString m_text;
    };

    
/*----------------------------------------------------------------------------*/
// sMenuWindow

    class sMenuWindow
	: public sWindow
    {
    public:
	const sInt_32 MESSAGE_COUNT = 16;

	enum ItemState
	{
	    ITEM_STATE_UNDEFINED,
	    ITEM_STATE_EMPTY,
	    ITEM_STATE_OCCUPIED
	};
	
	struct Item
	{
	    Item() { /* nothing */ }
	    Item(ItemState state, const sString &text)
		: m_state(state)
		, m_text(text)
	    { /* nothing */ }
	    
	    ItemState m_state;
	    sString m_text;	    
	};
	
	typedef std::vector<Item> Items_vector;
	
    public:
        sMenuWindow()
	    : sWindow()
	{ /* nothing */ }

        sMenuWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, bool focusable = true)
	: sWindow(context, x, y, width, height, title, focusable)
	, m_current_item(0)
	{
	    /* nothing */
	}
	
        sMenuWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, sInt_32 item_count, bool focusable = true)
	: sWindow(context, x, y, width, height, title, focusable)
	, m_current_item(0)
	{
	    m_menu_Items.resize(item_count);
	}

	virtual void redraw(void) const;

	sInt_32 get_ItemCount(void) const;

	ItemState get_ItemState(sInt_32 item) const;
	sString get_ItemText(sInt_32 item) const;		
	
	sInt_32 get_CurrentItem(void) const;	
	ItemState get_CurrentItemState(void) const;
	sString get_CurrentItemText(void) const;	

	void add_Item(const sString &item_text, ItemState state = ITEM_STATE_OCCUPIED);

	void set_Item(sInt_32 item, ItemState state);
	void set_Item(sInt_32 item, const sString &item_text, ItemState state = ITEM_STATE_OCCUPIED);

	void go_TO(sInt_32 item);
	void go_UP(void);
	void go_DOWN(void);
	void go_PAGEUP(void);
	void go_PAGEDOWN(void);
	void go_HOME(void);
	void go_END(void);

	sString enter_ItemFromKeyboard(void);

    public:
	sInt_32 m_current_item;

	Items_vector m_menu_Items;
    };    


/*----------------------------------------------------------------------------*/
// sEnvironment

    class sEnvironment
    {
    public:
	typedef std::vector<sWindow*> Windows_vector;
	
    public:
	sEnvironment()
	{
	    struct winsize w;
	    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	    
	    m_screen_width = w.ws_col;
	    m_screen_height = w.ws_row;	    
	}

	void refresh(void);
	void redraw(void);

	void rotate_Focus(bool backward = false);

    public:
	Windows_vector m_Windows;

	sInt_32 m_screen_width, m_screen_height;
    };
    

/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __TUI_H__ */
