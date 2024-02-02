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
/* ui_test.h / 0-125_air                                                      */
/*----------------------------------------------------------------------------*/
//
// User interface test.
//
/*----------------------------------------------------------------------------*/


#ifndef __UI_TEST_H__
#define __UI_TEST_H__

#include "defs.h"
#include "result.h"


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{

void print_Introduction(void);

sResult test_ui_1(void);
sResult test_ui_2(void);


/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __UI_TEST_H__ */
