/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                              RR_xoft 0-188_air                             */
/*                                                                            */
/*                  (C) Copyright 2021 - 2025 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* tui.cpp / 0-188_air                                                        */
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
#include <conio/conio.h>
#include <iostream>

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
    draw_Rectangle(x, y, width, height, BLUE);
}


void sContext::draw_PlainRectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height) const
{
    draw_PlainRectangle(x, y, width, height, BLUE);
}    

    
void sContext::draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, const sString &title, bool inverse) const
{
    draw_Rectangle(x, y, width, height, BLUE, title, inverse);
}


void sContext::draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, sInt_32 background) const
{
    textbackground(background);
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

    
void sContext::draw_PlainRectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, sInt_32 background) const
{
    textbackground(background);
    textcolor(LIGHTGRAY);
    
    gotoxy(x, y);
        
    for (int i = 0; i < height; ++i)
    {
	gotoxy(x, y + i);
	for (int j = 0; j < width; ++j)
	{
	    cprintf(" ");
	}
	cprintf("\n");
    }    
}    

    
void sContext::draw_Rectangle(sInt_32 x, sInt_32 y, sInt_32 width, sInt_32 height, sInt_32 background, const sString &title, bool inverse) const
{
    textbackground(background);
    textcolor(LIGHTGRAY);
    
    gotoxy(x, y);
    
    sInt_32 length = title.length();
    sInt_32 half = (width - length) / 2;
    
    cprintf("┌");
    for (int j = 1; j < half - 1; ++j)
    {
	cprintf("─");
    }

    if (inverse)
    {
	textbackground(LIGHTGRAY);
	textcolor(background);
	
	cprintf(" %s ", title.c_str());
	
	textbackground(background);
	textcolor(LIGHTGRAY);	
    }
    else
    {
	cprintf(" %s ", title.c_str());
    }
    
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
    

void sContext::draw_Text(sInt_32 x, sInt_32 y, const sString &text, bool inverse) const
{
    draw_Text(x, y, text, BLUE, inverse);
}

    
void sContext::draw_Text(sInt_32 x, sInt_32 y, const sString &text, sInt_32 background, bool inverse) const
{
    draw_Text(x, y, text, LIGHTGRAY, background, inverse);
}


void sContext::draw_Text(sInt_32 x, sInt_32 y, const sString &text, sInt_32 text_color, sInt_32 background, bool inverse) const
{
    if (inverse)
    {
	textbackground(text_color);
	textcolor(background);	
    }
    else
    {
	textbackground(background);
	textcolor(text_color);
    }

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
    m_context->draw_Rectangle(m_x, m_y, m_width, m_height, m_title, m_focused);
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
// sMultilineWindow class
/*----------------------------------------------------------------------------*/

void sMultilineWindow::redraw(void) const
{
    sWindow::redraw();

    for (sInt_32 i = 0; i < m_lines; ++i)
    {
	m_context->draw_Text(m_x + 1, m_y + i + 1, m_Texts[(m_curr_line + i) % m_lines]);
    }
}


void sMultilineWindow::set_Text(const sString &text)
{
    m_Texts[m_curr_line++] = text;
    m_curr_line %= m_lines;
}


sString sMultilineWindow::read_KeyboardString(void)
{
    sString text;
    cin >> text;

    return text;
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


sString sStatusWindow::read_KeyboardString(void)
{
    sString text;
    cin >> text;

    return text;
}



    
/*============================================================================*/
// sSignalWindow class
/*----------------------------------------------------------------------------*/

void sSignalWindow::redraw(void) const
{
    sWindow::redraw();

    if (m_signal_color >= 0)
    {
	m_context->draw_PlainRectangle(m_x + 1, m_y + 1, m_width - 2, m_height - 2, m_signal_color);
    }
    if (!m_text.empty())
    {
	m_context->draw_Text(m_x + m_width / 2 - m_text.length() / 2, m_y + m_height / 2, m_text, WHITE, m_signal_color >= 0 ? m_signal_color : BLUE, false);
    }
}


void sSignalWindow::set_Signal(sInt_32 signal_color, const sString &text)
{
    m_signal_color = signal_color;
    m_text = text;    
}


void sSignalWindow::set_SignalGreen(const sString &text)
{
    set_Signal(GREEN, text);
}


void sSignalWindow::set_SignalYellow(const sString &text)
{
    set_Signal(YELLOW, text);
}


void sSignalWindow::set_SignalRed(const sString &text)
{
    set_Signal(RED, text);    
}



    
/*============================================================================*/
// sMenuWindow class
/*----------------------------------------------------------------------------*/

void sMenuWindow::redraw(void) const
{
    sWindow::redraw();

    for (sInt_32 i = 0; i < m_menu_Items.size(); ++i)
    {
	if (m_current_item == i)
	{
	    if (m_focused)
	    {
		m_context->draw_Text(m_x + 1, m_y + 1 + i, m_menu_Items[i].m_text, true);
	    }
	    else
	    {
		m_context->draw_Text(m_x + 1, m_y + 1 + i, m_menu_Items[i].m_text, false);		
	    }
	}
	else
	{
	    m_context->draw_Text(m_x + 1, m_y + 1 + i, m_menu_Items[i].m_text, false);
	}
    }
}


/*----------------------------------------------------------------------------*/

sInt_32 sMenuWindow::get_ItemCount(void) const
{
    return m_menu_Items.size();
}


sInt_32 sMenuWindow::get_CurrentItem(void) const
{
    return m_current_item;
}


sMenuWindow::ItemState sMenuWindow::get_ItemState(sInt_32 item) const
{
    return m_menu_Items[item].m_state;
}


sString sMenuWindow::get_ItemText(sInt_32 item) const
{
    return m_menu_Items[item].m_text;    
}


sMenuWindow::ItemState sMenuWindow::get_CurrentItemState(void) const
{
    return m_menu_Items[m_current_item].m_state;
}

    
sString sMenuWindow::get_CurrentItemText(void) const
{
    return m_menu_Items[m_current_item].m_text;    
}

    
void sMenuWindow::add_Item(const sString &item_text, ItemState state)
{
    m_menu_Items.push_back(Item(state, item_text));
}


void sMenuWindow::set_Item(sInt_32 item, ItemState state)
{
    m_menu_Items[item].m_state = state;    
}

    
void sMenuWindow::set_Item(sInt_32 item, const sString &item_text, ItemState state)
{
    m_menu_Items[item].m_state = state;    
    m_menu_Items[item].m_text = item_text;
}    


void sMenuWindow::go_TO(sInt_32 item)
{
    m_current_item = item;
}


void sMenuWindow::go_UP(void)
{
    if (m_current_item > 0)
    {
	--m_current_item;
    }
}

    
void sMenuWindow::go_DOWN(void)
{
    if (m_current_item < m_menu_Items.size() - 1)
    {
	++m_current_item;
    }    
}


void sMenuWindow::go_PAGEUP(void)
{
    m_current_item = 0;
}

    
void sMenuWindow::go_PAGEDOWN(void)
{
    m_current_item = m_menu_Items.size() - 1;
}

    
void sMenuWindow::go_HOME(void)
{
    m_current_item = 0;
}

    
void sMenuWindow::go_END(void)
{
    m_current_item = m_menu_Items.size() - 1;
}
    

sString sMenuWindow::enter_ItemFromKeyboard(void)
{
    sString text, filtered_text;
    
    sString space;
    for (sInt_32 i = 0; i < m_width - 2; ++i)
    {
	space += " ";
    }
    
    m_context->draw_Text(m_x + 1, m_y + 1 + m_current_item, space, true);
    gotoxy(m_x + 1, m_y + 1 + m_current_item);

    std::getline (std::cin, text);

    for (const auto ch: text)
    {
	if (std::isprint(ch))
	{
	    filtered_text += ch;
	}	
    }
    m_menu_Items[m_current_item].m_text = filtered_text;
    m_menu_Items[m_current_item].m_state = ITEM_STATE_OCCUPIED;

    return text;
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


void sEnvironment::rotate_Focus(bool backward)
{
    sInt_32 window;
    
    for (window = 0; window < m_Windows.size(); ++window)
    {
	if (m_Windows[window]->m_focused)
	{
	    break;
	}
    }    

    if (backward)
    {
	for (sInt_32 i = 1; i < m_Windows.size() + 1; ++i)
	{
	    sInt_32 w = (window - i) % m_Windows.size();
	    
	    if (m_Windows[w]->m_focusable)
	    {
		m_Windows[window]->m_focused = false;	    
		m_Windows[w]->m_focused = true;
		
		return;
	    }
	}	
    }
    else
    {	
	for (sInt_32 i = 1; i < m_Windows.size() + 1; ++i)
	{
	    sInt_32 w = (window + i) % m_Windows.size();
	    
	    if (m_Windows[w]->m_focusable)
	    {
		m_Windows[window]->m_focused = false;	    
		m_Windows[w]->m_focused = true;
		
		return;
	    }
	}
    }
}
    
    
/*----------------------------------------------------------------------------*/

} // namespace RR_xoft
    
