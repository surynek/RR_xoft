/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-178_air                             */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                  */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* io.h / 0-178_air                                                           */
/*----------------------------------------------------------------------------*/
//
// Input/output functions and utilities.
//
/*----------------------------------------------------------------------------*/


#ifndef __IO_H__
#define __IO_H__

#include "result.h"


using namespace RR_xoft;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{




/*----------------------------------------------------------------------------*/
// Global functions

    sInt_32 sConsumeUntilChar(FILE *fr, sChar c);
    sInt_32 sConsumeUntilString(FILE *fr, const sString &string);

    sInt_32 sReadUntilChar(FILE *fr, sChar c, sString &string);    
    
    sInt_32 sReadAlphaString(FILE *fr, sString &alpha_string);
    sInt_32 sReadAlnumString(FILE *fr, sString &alnum_string);    
    sInt_32 sReadNumericString(FILE *fr, sString &numeric_string);
    sInt_32 sReadFloatalString(FILE *fr, sString &floatal_string);
    sInt_32 sReadDigitalString(FILE *fr, sString &digital_string);
    sInt_32 sReadPrintableString(FILE *fr, sString &printable_string);        

    sInt_32 sConsumeWhiteSpaces(FILE *fr);
    

/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __IO_H__ */
