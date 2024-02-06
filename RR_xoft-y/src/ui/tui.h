/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-137_air                              */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* tui.h / 0-137_air                                                          */
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
// sContext

    class sContext
    {
    public:
	sContext() { /* nothing */ }

	sInt_32 kb_Hit(void) const;

	void clear_Screen(void) const;
	
	void draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height) const;
	void draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title) const;

	void draw_Text(sInt_32 x, sInt_32 y, const sString &text) const;
    };


/*----------------------------------------------------------------------------*/
// sWindow

    class sWindow
    {
    public:
	sWindow() { /* nothing */ }
	sWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title)
	    : m_context(&context)
	    , m_x(x)
	    , m_y(y)
	    , m_width(width)
	    , m_height(height)
	    , m_title(title)
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

	sStatusWindow(sContext &context, sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title)
	    : sWindow(context, x, y, width, height, title)
	{ /* nothing */ }

	virtual void redraw(void) const;
	
	void set_Text(const sString &text);

    public:
	sString m_text;
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

	virtual void refresh(void);
	virtual void redraw(void);

    public:
	Windows_vector m_Windows;

	sInt_32 m_screen_width, m_screen_height;
    };
    

/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __TUI_H__ */
