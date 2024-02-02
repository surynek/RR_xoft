/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-125_air                              */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* serial_test.h / 0-125_air                                                  */
/*----------------------------------------------------------------------------*/
//
// Serial port test.
//
/*----------------------------------------------------------------------------*/


#ifndef __SERIAL_TEST_H__
#define __SERIAL_TEST_H__

#include "defs.h"
#include "result.h"


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{

void print_Introduction(void);
sResult test_serial_1(void);


/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __SERIAL_TEST_H__ */

