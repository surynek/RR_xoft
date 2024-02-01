/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-121_air                              */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* ui_test.cpp / 0-121_air                                                    */
/*----------------------------------------------------------------------------*/
//
// User interface test.
//
/*----------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <termios.h>

#include <sys/select.h>
#include <sys/ioctl.h>

#include "defs.h"
#include "compile.h"
#include "result.h"
#include "version.h"

#include "common/types.h"
#include "core/robot.h"
#include "util/geometry.h"
#include "ui/tui.h"

#include "test/ui_test.h"


using namespace std;
using namespace RR_xoft;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


    

int test_basic_1(void)
{
    printf("Testing ui 1 ...\n");   
    printf("Testing ui 1 ... finished\n");
    
    return sRESULT_SUCCESS;
}    


/*----------------------------------------------------------------------------*/

void print_Introduction(void)
{
    printf("----------------------------------------------------------------\n");    
    printf("%s : User Interface Test Program\n", sPRODUCT);
    printf("%s\n", sCOPYRIGHT);
    printf("================================================================\n");
}



sResult test_ui_1(void)
{    
    printf("Testing ui 1 ...\n");

    sContext context;
    
    context.clear_Screen();    
    context.draw_Rectangle(5, 5, 20, 20);

    context.draw_Rectangle(26, 5, 20, 20, "Window");
    signal(SIGWINCH, s_handle_Winch);    

    printf("Testing ui 1 ... finished\n");
    
    return sRESULT_SUCCESS;
}

    
int _kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
    

sResult test_ui_2(void)
{    
    printf("Testing ui 2 ...\n");

    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(0, TCSANOW, &t);    

    sContext context;
    
    while (true)
    {
	char ch = getchar();
	printf("%d\n", ch);
    }

    printf("Testing ui 2 ... finished\n");
    
    return sRESULT_SUCCESS;
}    

    
sContext Context_1;    
sEnvironment Environment_1;    

sWindow *window_A;
sWindow *window_B;
    
void test_refresh_Environment()
{
    Context_1.clear_Screen();
    
    window_A->m_x = 1;
    window_A->m_y = 1;
    
    window_A->m_width = Environment_1.m_screen_width - 4;    
    window_A->m_height = Environment_1.m_screen_height - 4;

    Environment_1.refresh();
}


void test_handle_Winch(sInt_32 sUNUSED(sig))
{
    signal(SIGWINCH, SIG_IGN);
    test_refresh_Environment();

    signal(SIGWINCH, test_handle_Winch);
}    

    
sResult test_ui_3(void)
{
    window_A = new sStatusWindow(Context_1, 10, 10, 20, 20, "Window A");
    window_B = new sStatusWindow(Context_1, 24, 10, 20, 20, "Window B");
	
    Environment_1.m_Windows.push_back(window_A);
    Environment_1.m_Windows.push_back(window_B);

    test_refresh_Environment();
    signal(SIGWINCH, test_handle_Winch);    

    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(0, TCSANOW, &t);

    while (true)
    {
	char ch = getchar();
	printf("%d\n", ch);
    }    

    return sRESULT_SUCCESS;
}
    
   
   

}  // namespace RR_xoft

/*----------------------------------------------------------------------------*/


int main(int sUNUSED(argc), const char **sUNUSED(argv))
{
    sResult result;

    print_Introduction();

    /*
    if (sFAILED(result = test_ui_1()))
    {
	printf("Test ui 1 failed (error:%d).\n", result);
	return result;
    }

    if (sFAILED(result = test_ui_2()))
    {
	printf("Test ui 2 failed (error:%d).\n", result);
	return result;
    } 
    */

    if (sFAILED(result = test_ui_3()))
    {
	printf("Test ui 3 failed (error:%d).\n", result);
	return result;
    }     
    
    return 0;
}
