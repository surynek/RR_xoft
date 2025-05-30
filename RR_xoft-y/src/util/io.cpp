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
/* io.cpp / 0-189_air                                                         */
/*----------------------------------------------------------------------------*/
//
// Input/output functions and utilities.
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/times.h>

#include "config.h"
#include "compile.h"

#include "common/types.h"
#include "util/io.h"


using namespace RR_xoft;




/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


    

/*============================================================================*/
// Global functions
/*----------------------------------------------------------------------------*/

    sInt_32 sConsumeUntilChar(FILE *fr, sChar c)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if ((sChar)ch == c)
	    {
		break;
	    }
	}
	return chars_consumed;
    }


    sInt_32 sReadUntilChar(FILE *fr, sChar c, sString &string)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;	    
	    
	    if ((sChar)ch == c)
	    {
		break;
	    }
	    string += ch;
	}
	return chars_consumed;
    }    

    
    sInt_32 sReadUntilString(FILE *fr, const sString &string)
    {
	sInt_32 ch, chars_consumed = 0;
	sString read_string;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;	    

	    if (read_string.length() < string.length())
	    {
		read_string += ch;
	    }
	    else
	    {
		read_string.erase(read_string.begin());
		read_string += ch;
	    }	    
	    if (read_string == string)
	    {
		break;
	    }
	}
	return chars_consumed;
    }


    sInt_32 sReadAlphaString(FILE *fr, sString &alpha_string)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if (!isalpha(ch) && ch != '_')
	    {
		ungetc(ch, fr);
		break;
	    }
	    alpha_string += ch;
	}
	return chars_consumed;
    }


    sInt_32 sReadAlnumString(FILE *fr, sString &alnum_string)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if (!isalnum(ch))
	    {
		ungetc(ch, fr);
		break;
	    }
	    alnum_string += ch;
	}
	return chars_consumed;
    }    


    sInt_32 sReadNumericString(FILE *fr, sString &numeric_string)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if (!isdigit(ch))
	    {
		ungetc(ch, fr);
		break;
	    }
	    numeric_string += ch;
	}
	return chars_consumed;
    }


    sInt_32 sReadFloatalString(FILE *fr, sString &floatal_string)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if (!isdigit(ch) && ch != '.')
	    {
		ungetc(ch, fr);
		break;
	    }
	    floatal_string += ch;
	}
	return chars_consumed;
    }


    sInt_32 sReadDigitalString(FILE *fr, sString &digital_string)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if (!isdigit(ch))
	    {
		ungetc(ch, fr);
		return 0;
	    }
	    digital_string += ch;
	}
	return chars_consumed;
    }    


    sInt_32 sReadPrintableString(FILE *fr, sString &printable_string)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if (!isprint(ch))
	    {
		ungetc(ch, fr);
		return 0;
	    }
	    printable_string += ch;
	}
	return chars_consumed;
    }
    
    
    sInt_32 sConsumeWhiteSpaces(FILE *fr)
    {
	sInt_32 ch, chars_consumed = 0;

	while ((ch = fgetc(fr)) != EOF)
	{
	    ++chars_consumed;
	    
	    if (!isspace(ch))
	    {
		ungetc(ch, fr);
		break;
	    }
	}
	return chars_consumed;
    }                

    
/*----------------------------------------------------------------------------*/

} // namespace RR_xoft
