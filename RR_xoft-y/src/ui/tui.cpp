/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-098_air                              */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* tui.cpp / 0-098_air                                                        */
/*----------------------------------------------------------------------------*/
//
// Text based user interface.
//
/*----------------------------------------------------------------------------*/


#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "conio/conio.h"

#include "defs.h"
#include "result.h"

#include "common/types.h"
#include "core/robot.h"

#include "ui/tui.h"


using namespace RR_xoft;




/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


    

/*============================================================================*/
    
void s_handle_Winch(sInt_32 sUNUSED(sig))
{
    signal(SIGWINCH, SIG_IGN);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf("%d,%d\n", w.ws_col, w.ws_row);    
    signal(SIGWINCH, s_handle_Winch);
}

    
/*============================================================================*/
// sContext class
/*----------------------------------------------------------------------------*/

sInt_32 sContext::kb_Hit(void) const
{
    return kbhit();
}
    
    
void sContext::clear_Screen(void) const
{
    textbackground(BLACK);    
    clrscr();
}

    
void sContext::draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height) const
{
    textbackground(BLUE);
    textcolor(LIGHTGRAY);
    
    gotoxy(x, y);
    
    cprintf("┌");
    for (int j = 1; j < width - 1; ++j)
    {
	cprintf("─");
    }	
    cprintf("┐");	
    
    for (int i = 1; i < height - 1; ++i)
    {
	gotoxy(x, y + i);
	cprintf("│");
	for (int j = 1; j < width - 1; ++j)
	{
	    cprintf(" ");
	}
	cprintf("│\n");
    }
    
    gotoxy(x, y + height - 1);
    cprintf("└");
    for (int j = 1; j < width - 1; ++j)
    {
	cprintf("─");
    }
    cprintf("┘\n");	
}

    
void sContext::draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title) const
{	
    textbackground(BLUE);
    textcolor(LIGHTGRAY);
    
    gotoxy(x, y);
    
    sInt_32 length = title.length();
    sInt_32 half = (width - length) / 2;
    
    cprintf("┌");
    for (int j = 1; j < half - 1; ++j)
    {
	cprintf("─");
    }
    cprintf(" %s ", title.c_str());
    
    for (int j = half + length + 1; j < width - 1; ++j)
    {
	cprintf("─");
    }	
	
    cprintf("┐");	
    
    for (int i = 1; i < height - 1; ++i)
    {
	gotoxy(x, y + i);
	cprintf("│");
	for (int j = 1; j < width - 1; ++j)
	{
	    cprintf(" ");
	}
	cprintf("│\n");
    }
    
    gotoxy(x, y + height - 1);
    cprintf("└");
    for (int j = 1; j < width - 1; ++j)
    {
	cprintf("─");
    }
    cprintf("┘\n");	
}


void sContext::draw_Text(sInt_32 x, sInt_32 y, const sString &text) const
{
    textbackground(BLUE);
    textcolor(LIGHTGRAY);

    gotoxy(x, y);
    
    for (sInt_32 i = 0; i < text.length(); ++i)
    {
	if (text[i] == '\n')
	{
	    ++y;
	    gotoxy(x, y);
	    continue;
	}
	else
	{
	    putchar(text[i]);
	}
    }
    putchar('\n');
}



    
/*============================================================================*/
// sWindow class
/*----------------------------------------------------------------------------*/

void sWindow::redraw(void) const
{
    m_context->draw_Rectangle(m_x, m_y, m_width, m_height, m_title);
}

    
void sWindow::resize(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;

    redraw();
}



    
/*============================================================================*/
// sStatusWindow class
/*----------------------------------------------------------------------------*/

void sStatusWindow::redraw(void) const
{
    sWindow::redraw();
    m_context->draw_Text(m_x + 1, m_y + 1, m_text);
}


void sStatusWindow::set_Text(const sString &text)
{
    m_text = text;
}



    
/*============================================================================*/
// sEnvironment class
/*----------------------------------------------------------------------------*/

void sEnvironment::refresh(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    m_screen_width = w.ws_col;
    m_screen_height = w.ws_row;
}


void sEnvironment::redraw(void)
{
    for (Windows_vector::const_iterator window = m_Windows.begin(); window != m_Windows.end(); ++window)
    {
	(*window)->redraw();
    }    
}    
    
    
/*----------------------------------------------------------------------------*/

} // namespace RR_xoft
    
