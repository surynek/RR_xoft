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
/* defs.h / 0-189_air                                                         */
/*----------------------------------------------------------------------------*/

#ifndef __DEFS_H__
#define __DEFS_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "compile.h"


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


/*----------------------------------------------------------------------------*/
// Auxiliary macros

#define sUNUSED(x)

#ifdef sDEBUG
  #define sUNUSED_(x) x
#else
  #define sUNUSED_(x)
#endif

#ifdef sDEEP_CONSISTENCY
  #define sUSED_(x) x
#else
  #define sUSED_(x)
#endif


/*----------------------------------------------------------------------------*/
// Maximum, minimum macros

#define sMIN(x,y) (((x) < (y)) ? (x) : (y))
#define sMAX(x,y) (((x) > (y)) ? (x) : (y))
#define sDFR(x,y) (((x) < (y)) ? ((y) - (x)) : ((x) - (y)))
#define sABS(x) (((x) < 0) ? -(x) : (x))
#define sSGN(x) (((x) < 0) ? -(-1) : ((x) > 0) ? 1 : 0)


/*----------------------------------------------------------------------------*/
// Auxiliary macros

#define sSWAP(x, y, T)      \
    {                       \
        T z;                \
	z = x;              \
	x = y;              \
	y = z;              \
    }


/*----------------------------------------------------------------------------*/
// C++ Language extensions

#define SEMICONST


/*----------------------------------------------------------------------------*/
// Size definitions

#define sKILO(x) (1024 * (x))
#define sMEGA(x) (1024 * 1024 * (x))
#define sGIGA(x) (1024 * 1024 * 1024 * (x))


/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __DEFS_H__ */
