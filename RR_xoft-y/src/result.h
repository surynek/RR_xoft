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
/* result.h / 0-098_air                                                       */
/*----------------------------------------------------------------------------*/

#ifndef __RESULT_H__
#define __RESULT_H__

#include <limits.h>

#include "config.h"
#include "compile.h"


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{

typedef int sResult;


/*----------------------------------------------------------------------------*/
// Common results

enum sStandard_Result
{
    sRESULT_SUCCESS    =  0,
    sRESULT_INFO       =  1,
    sRESULT_ERROR      = -1,
    sRESULT_UNDEFINED  = INT_MAX
};


enum sType_Result
{
    sRESULT_TYPE__INFO               =  10000,
    sRESULT_TYPE__ERROR              = -10000,
    sRESULT_TYPE__INT_8_FPRN_ERROR   = (sRESULT_TYPE__ERROR -  1),
    sRESULT_TYPE__UINT_8_FPRN_ERROR  = (sRESULT_TYPE__ERROR -  2),
    sRESULT_TYPE__INT_16_FPRN_ERROR  = (sRESULT_TYPE__ERROR -  3),
    sRESULT_TYPE__UINT_16_FPRN_ERROR = (sRESULT_TYPE__ERROR -  4),
    sRESULT_TYPE__INT_32_FPRN_ERROR  = (sRESULT_TYPE__ERROR -  5),
    sRESULT_TYPE__UINT_32_FPRN_ERROR = (sRESULT_TYPE__ERROR -  6),
    sRESULT_TYPE__INT_64_FPRN_ERROR  = (sRESULT_TYPE__ERROR -  7),
    sRESULT_TYPE__UINT_64_FPRN_ERROR = (sRESULT_TYPE__ERROR -  8),
    sRESULT_TYPE__BOOL_FPRN_ERROR    = (sRESULT_TYPE__ERROR -  9),
    sRESULT_TYPE__CHAR_FPRN_ERROR    = (sRESULT_TYPE__ERROR - 10),
    sRESULT_TYPE__WCHAR_FPRN_ERROR   = (sRESULT_TYPE__ERROR - 11),
    sRESULT_TYPE__STRING_FPRN_ERROR  = (sRESULT_TYPE__ERROR - 12),
    sRESULT_TYPE__PHRASE_FPRN_ERROR  = (sRESULT_TYPE__ERROR - 13),
    sRESULT_TYPE__FLOAT_FPRN_ERROR   = (sRESULT_TYPE__ERROR - 14),
    sRESULT_TYPE__DOUBLE_FPRN_ERROR  = (sRESULT_TYPE__ERROR - 15),
    sRESULT_TYPE__POINTER_FPRN_ERROR = (sRESULT_TYPE__ERROR - 16),
    sRESULT_TYPE__BYTES_FPRN_ERROR   = (sRESULT_TYPE__ERROR - 17),
    sRESULT_TYPE__INT_8_SCAN_ERROR   = (sRESULT_TYPE__ERROR - 18),
    sRESULT_TYPE__UINT_8_SCAN_ERROR  = (sRESULT_TYPE__ERROR - 19),
    sRESULT_TYPE__INT_16_SCAN_ERROR  = (sRESULT_TYPE__ERROR - 20),
    sRESULT_TYPE__UINT_16_SCAN_ERROR = (sRESULT_TYPE__ERROR - 21),
    sRESULT_TYPE__INT_32_SCAN_ERROR  = (sRESULT_TYPE__ERROR - 22),
    sRESULT_TYPE__UINT_32_SCAN_ERROR = (sRESULT_TYPE__ERROR - 23),
    sRESULT_TYPE__INT_64_SCAN_ERROR  = (sRESULT_TYPE__ERROR - 24),
    sRESULT_TYPE__UINT_64_SCAN_ERROR = (sRESULT_TYPE__ERROR - 25),
    sRESULT_TYPE__BOOL_SCAN_ERROR    = (sRESULT_TYPE__ERROR - 26),
    sRESULT_TYPE__CHAR_SCAN_ERROR    = (sRESULT_TYPE__ERROR - 27),
    sRESULT_TYPE__WCHAR_SCAN_ERROR   = (sRESULT_TYPE__ERROR - 28),
    sRESULT_TYPE__STRING_SCAN_ERROR  = (sRESULT_TYPE__ERROR - 29),
    sRESULT_TYPE__PHRASE_SCAN_ERROR  = (sRESULT_TYPE__ERROR - 30),
    sRESULT_TYPE__FLOAT_SCAN_ERROR   = (sRESULT_TYPE__ERROR - 31),
    sRESULT_TYPE__DOUBLE_SCAN_ERROR  = (sRESULT_TYPE__ERROR - 32)
};


enum sUndirectedGraph_Result
{
    sUNDIRECTED_GRAPH_INFO            =  11000,
    sUNDIRECTED_GRAPH_ERROR           = -11000,
    sUNDIRECTED_GRAPH_OPEN_ERROR      = (sUNDIRECTED_GRAPH_ERROR - 1),
    sUNDIRECTED_GRAPH_MHPF_OPEN_ERROR = (sUNDIRECTED_GRAPH_ERROR - 2)    
};


enum sAgentConfiguration_Result
{
    sAGENT_CONFIGURATION_INFO       =  12000,
    sAGENT_CONFIGURATION_ERROR      = -12000,
    sAGENT_CONFIGURATION_OPEN_ERROR = (sAGENT_CONFIGURATION_ERROR - 1),
    sAGENT_CONFIGURATION_SEEK_ERROR = (sAGENT_CONFIGURATION_ERROR - 2)
};


enum sAgentCommitment_Result
{
    sAGENT_COMMITMENT_INFO       =  13000,
    sAGENT_COMMITMENT_ERROR      = -13000,
    sAGENT_COMMITMENT_OPEN_ERROR = (sAGENT_COMMITMENT_ERROR - 1)
};


enum sAgentSolution_Result
{
    sAGENT_SOLUTION_INFO       =  14000,
    sAGENT_SOLUTION_ERROR      = -14000,    
    sAGENT_SOLUTION_OPEN_ERROR = (sAGENT_SOLUTION_ERROR - 1)
};


enum sAgentInstance_Result
{
    sAGENT_INSTANCE_INFO                =  15000,
    sAGENT_INSTANCE_ERROR               = -15000,
    sAGENT_INSTANCE_OPEN_ERROR          = (sAGENT_INSTANCE_ERROR -  1),
    sAGENT_INSTANCE_PDDL_OPEN_ERROR     = (sAGENT_INSTANCE_ERROR -  2),
    sAGENT_INSTANCE_CPF_OPEN_ERROR      = (sAGENT_INSTANCE_ERROR -  3),
    sAGENT_INSTANCE_CCPF_OPEN_ERROR     = (sAGENT_INSTANCE_ERROR -  4),
    sAGENT_INSTANCE_MPF_OPEN_ERROR      = (sAGENT_INSTANCE_ERROR -  5),
    sAGENT_INSTANCE_BGU_OPEN_ERROR      = (sAGENT_INSTANCE_ERROR -  6),
    sAGENT_INSTANCE_USC_MAP_OPEN_ERROR  = (sAGENT_INSTANCE_ERROR -  7),
    sAGENT_INSTANCE_USC_AGNT_OPEN_ERROR = (sAGENT_INSTANCE_ERROR -  8),    
    sAGENT_INSTANCE_DIBOX_OPEN_ERROR    = (sAGENT_INSTANCE_ERROR -  9),    
    sAGENT_INSTANCE_CNF_OPEN_ERROR      = (sAGENT_INSTANCE_ERROR - 10),
    sAGENT_INSTANCE_SEEK_ERROR          = (sAGENT_INSTANCE_ERROR - 11),
    sAGENT_INSTANCE_MOVISCEN_OPEN_ERROR = (sAGENT_INSTANCE_ERROR - 12)    
};


enum sAgentMission_Result
{
    sAGENT_MISSION_INFO                =  15000,
    sAGENT_MISSION_ERROR               = -15000,
    sAGENT_MISSION_OPEN_ERROR          = (sAGENT_MISSION_ERROR -  1),
    sAGENT_MISSION_MHPF_OPEN_ERROR     = (sAGENT_MISSION_ERROR -  2)
};


enum s2DMap_Result
{
    s2D_MAP_INFO                              =  16000,
    s2D_MAP_ERROR                             = -16000,
    s2D_MAP_OPEN_ERROR                        = (s2D_MAP_ERROR - 1),
    s2D_MAP_UNRECOGNIZED_XML_FORMATTING_ERROR = (s2D_MAP_ERROR - 2)    
};


enum sRealConjunction_Result
{
    sREAL_CONJUNCTION_INFO                              =  17000,
    sREAL_CONJUNCTION_ERROR                             = -17000,
    sREAL_CONJUNCTION_OPEN_ERROR                        = (sREAL_CONJUNCTION_ERROR - 1),
    sREAL_CONJUNCTION_XML_OPEN_ERROR                    = (sREAL_CONJUNCTION_ERROR - 2),    
    sREAL_CONJUNCTION_UNRECOGNIZED_XML_FORMATTING_ERROR = (sREAL_CONJUNCTION_ERROR - 3)
};


enum sRealInstance_Result
{
    sREAL_INSTANCE_INFO                =  18000,
    sREAL_INSTANCE_ERROR               = -18000,
    sREAL_INSTANCE_OPEN_ERROR          = (sREAL_INSTANCE_ERROR - 1),
    sREAL_INSTANCE_MOVISCEN_OPEN_ERROR = (sREAL_INSTANCE_ERROR - 2)
};


enum sRealSolution_Result
{
    sREAL_SOLUTION_INFO           =  19000,
    sREAL_SOLUTION_ERROR          = -19000,    
    sREAL_SOLUTION_COLLISION_INFO = (sREAL_SOLUTION_INFO + 1)
};


enum sRealCBS_Result
{
    sREAL_CBS_INFO       =  20000,
    sREAL_CBS_ERROR      = -20000,
    sREAL_CBS_OPEN_ERROR = (sREAL_CBS_ERROR - 1),
};


enum sMotionPlanProgram_Result
{
    sMOTION_PLAN_PROGRAM_INFO                         =  100000,
    sMOTION_PLAN_PROGRAM_ERROR                        = -100000,
    sMOTION_PLAN_PROGRAM_UNRECOGNIZED_PARAMETER_ERROR = (sMOTION_PLAN_PROGRAM_ERROR - 1)
};


enum sControlPanelProgram_Result
{
    sCONTROL_PANEL_PROGRAM_INFO                         =  200000,
    sCONTROL_PANEL_PROGRAM_ERROR                        = -200000,
    sCONTROL_PANEL_PROGRAM_UNRECOGNIZED_PARAMETER_ERROR = (sCONTROL_PANEL_PROGRAM_ERROR - 1),
    sCONTROL_PANEL_PROGRAM_GET_ATTR_ERROR               = (sCONTROL_PANEL_PROGRAM_ERROR - 2),
    sCONTROL_PANEL_PROGRAM_SET_ATTR_ERROR               = (sCONTROL_PANEL_PROGRAM_ERROR - 3),
    sCONTROL_PANEL_PROGRAM_JOINTS_STATE_READ_ERROR      = (sCONTROL_PANEL_PROGRAM_ERROR - 4),
    sCONTROL_PANEL_PROGRAM_SERIAL_PORT_OPEN_ERROR       = (sCONTROL_PANEL_PROGRAM_ERROR - 5)    
};



/*----------------------------------------------------------------------------*/

#define sFAILED(result)    ((result) < sRESULT_SUCCESS)
#define sSUCCEEDED(result) ((result) >= sRESULT_SUCCESS)
#define sINFORMED(result)  ((result) > sRESULT_SUCCESS) 

#define sCHECK_RESULT(command)                                                           \
    {                                                                                    \
        sResult result;                                                                  \
        if (sFAILED(result = (command)))				                 \
        {                                                                                \
            return result;                                                               \
        }                                                                                \
    }


#define sCHECK_INT(command, result)                                                      \
    {                                                                                    \
	if ((command) < 0)                                                               \
	{                                                                                \
	    return result;                                                               \
	}                                                                                \
    }


/*----------------------------------------------------------------------------*/

#ifdef sDEBUG
  #define sASSERT(condition)                                                             \
    {                                                                                    \
      if (!(condition))							                 \
      {                                                                                  \
        printf("sASSERT: assertion failed (file: %s, line:%d).\n", __FILE__, __LINE__);  \
	fflush(NULL);                                                                    \
	exit(-1);						   	                 \
      }                                                                                  \
    }
#else
  #define sASSERT(condition)
#endif /* DEBUG */


#ifdef sDEBUG
  #define sASSERT_MESSAGE(condition, message)					         \
    {                                                                                    \
      if (!(condition))							                 \
      {                                                                                  \
        printf("sASSERT: assertion failed (file: %s, line:%d).\n", __FILE__, __LINE__);  \
	printf("Assertion message: %s\n", (message));			                 \
	fflush(NULL);                                                                    \
	exit(-1);						   	                 \
      }                                                                                  \
    }
#else
  #define sASSERT_MESSAGE(condition, message)
#endif /* DEBUG */


/*----------------------------------------------------------------------------*/

#ifdef sCONSISTENCY
  #define sTEST(condition)				                                                          \
    {                                                                                                             \
      sResult result;							                                          \
      if ((result = (condition)) != sRESULT_SUCCESS)			                                          \
      {                                                                                                           \
	printf("sTEST: consistency test failed (file: %s, line:%d, result: %d).\n", __FILE__, __LINE__, result);  \
	fflush(NULL);                                                                                             \
	exit(-1);						   	                                          \
      }                                                                                                           \
    }
#else
  #define sTEST_MESSAGE(condition, message)
#endif /* CONSISTENCY */


#ifdef sCONSISTENCY
  #define sTEST_MESSAGE(condition, message)				                                          \
    {                                                                                                             \
      sResult result;							                                          \
      if ((result = (condition)) != sRESULT_SUCCESS)		       	                                          \
      {                                                                                                           \
	printf("sTEST: consistency test failed (file: %s, line:%d, result: %d).\n", __FILE__, __LINE__, result);  \
	printf("Test message: %s\n", (message));			                                          \
	fflush(NULL);                                                                                             \
	exit(-1);						   	                                          \
      }                                                                                                           \
    }
#else
  #define sTEST_MESSAGE(condition, message)
#endif /* CONSISTENCY */


/*----------------------------------------------------------------------------*/

#ifdef sTHOROUGH_CONSISTENCY
  #define sTHOROUGH_TEST(condition)				                                                                    \
    {                                                                                                                               \
      sResult result;							                                                            \
      if ((result = (condition)) != sRESULT_SUCCESS)			                                                            \
      {                                                                                                                             \
	printf("sTHOROUGH_TEST: thorough consistency test failed (file: %s, line:%d, result: %d).\n", __FILE__, __LINE__, result);  \
	fflush(NULL);                                                                                                               \
	exit(-1);						   	                                                            \
      }                                                                                                                             \
    }
#else
  #define sTHOROUGH_TEST_MESSAGE(condition, message)
#endif /* THOROUGH_CONSISTENCY */


#ifdef sTHOROUGH_CONSISTENCY
  #define sTHOROUGH_TEST_MESSAGE(condition, message)				                                                    \
    {                                                                                                                               \
      sResult result;							                                                            \
      if ((result = (condition)) != sRESULT_SUCCESS)		       	                                                            \
      {                                                                                                                             \
	printf("sTHOROUGH_TEST: thorough consistency test failed (file: %s, line:%d, result: %d).\n", __FILE__, __LINE__, result);  \
	printf("Test message: %s\n", (message));			                                                            \
	fflush(NULL);                                                                                                               \
	exit(-1);						   	                                                            \
      }                                                                                                                             \
    }
#else
  #define sTHOROUGH_TEST_MESSAGE(condition, message)
#endif /* THOROUGH_CONSISTENCY */


/*----------------------------------------------------------------------------*/

#ifdef sDEEP_CONSISTENCY
  #define sDEEP_TEST(condition)				                                                                    \
    {                                                                                                                       \
      sResult result;							                                                    \
      if ((result = (condition)) != sRESULT_SUCCESS)			                                                    \
      {                                                                                                                     \
	printf("sDEEP_TEST: deep consistency test failed (file: %s, line:%d, result: %d).\n", __FILE__, __LINE__, result);  \
	fflush(NULL);                                                                                                       \
	exit(-1);						   	                                                    \
      }                                                                                                                     \
    }
#else
  #define sDEEP_TEST_MESSAGE(condition, message)
#endif /* DEEP_CONSISTENCY */


#ifdef sDEEP_CONSISTENCY
  #define sDEEP_TEST_MESSAGE(condition, message)				                                            \
    {                                                                                                                       \
      sResult result;							                                                    \
      if ((result = (condition)) != sRESULT_SUCCESS)		       	                                                    \
      {                                                                                                                     \
	printf("sDEEP_TEST: deep consistency test failed (file: %s, line:%d, result: %d).\n", __FILE__, __LINE__, result);  \
	printf("Test message: %s\n", (message));			                                                    \
	fflush(NULL);                                                                                                       \
	exit(-1);						   	                                                    \
      }                                                                                                                     \
    }
#else
  #define sDEEP_TEST_MESSAGE(condition, message)
#endif /* DEEP_CONSISTENCY */


/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __RESULT_H__ */

