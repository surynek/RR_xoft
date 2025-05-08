/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                              RR_xoft 0-186_air                             */
/*                                                                            */
/*                  (C) Copyright 2021 - 2025 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* motion_plan_main.cpp / 0-186_air                                           */
/*----------------------------------------------------------------------------*/
//
// Robot Motion Planning - main program.
//
// Motion planning for the RR1 robotic arm and related robots.
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

#include "config.h"
#include "compile.h"
#include "defs.h"
#include "version.h"

#include "core/robot.h"
#include "util/statistics.h"

#include "main/motion_plan_main.h"


using namespace std;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


/*----------------------------------------------------------------------------*/

  sCommandParameters::sCommandParameters()
      : m_seed(0)
  {
      // nothing
  }


/*----------------------------------------------------------------------------*/

    void print_IntroductoryMessage(void)
    {
	printf("----------------------------------------------------------------\n");
	printf("%s : Path Instance Generator\n", sPRODUCT);
	printf("%s\n", sCOPYRIGHT);
	printf("================================================================\n");	
    }


    void print_ConcludingMessage(void)
    {
	printf("----------------------------------------------------------------\n");
    }


    void print_Help(void)
    {
	printf("Usage:\n");
	printf("motion_plan_RR\n");
	printf("             [--seed=<int>\n");
	printf("\n");
	printf("Examples:\n");
	printf("motion_plan_RR\n");
	printf("             --seed=12345\n");
	printf("\n");
	printf("Defaults:\n");
	printf("          --seed=0\n");
	printf("\n");
    }


    sResult plan_Motion(const sCommandParameters &parameters)
    {
	//sResult result;
	srand(parameters.m_seed);
	
	#ifdef sSTATISTICS
	{
	  s_GlobalStatistics.to_Screen();
	}
	#endif

	return sRESULT_SUCCESS;
    }


    sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &command_parameters)
    {
	if (parameter.find("--seed=") == 0)
	{
	    command_parameters.m_seed = sInt_32_from_String(parameter.substr(7, parameter.size()));
	}
	else
	{
	    return sMOTION_PLAN_PROGRAM_UNRECOGNIZED_PARAMETER_ERROR;
	}
	return sRESULT_SUCCESS;
    }


/*----------------------------------------------------------------------------*/

} // namespace RR_xoft


/*----------------------------------------------------------------------------*/
// main program

int main(int argc, char **argv)
{
    sResult result;
    sCommandParameters command_parameters;

    print_IntroductoryMessage();

    if (argc >= 2 && argc <= 11)
    {
	for (int i = 1; i < argc; ++i)
	{
	    result = parse_CommandLineParameter(argv[i], command_parameters);
	    if (sFAILED(result))
	    {
		printf("Error: Cannot parse command line parameters (code = %d).\n", result);
		print_Help();

		return result;
	    }
	}
	result = plan_Motion(command_parameters);
	if (sFAILED(result))
	{
	    return result;
	}
    }
    else
    {
	print_Help();
    }
    return sRESULT_SUCCESS;
}
