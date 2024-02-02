/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-127_air                              */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* p.cpp / 0-127_air                                                          */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"


int main(int argc, char **argv)
{    
    printf("Real Control Panel\n");

    int xp, yp;
    
    __CONIO_H output;    
    output.wherexy(xp, yp);

    while (true)
    {
	for (int i = 0; i < 256; ++i)
	{
	    textcolor(i);
	    gotoxy(xp + 10, yp + 10);
	    printf("Value: %d\n", i);
	}
    }
    
    return 0;
}
