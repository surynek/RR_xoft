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
/* control_panel_main.h / 0-098_air                                           */
/*----------------------------------------------------------------------------*/
//
// Control Panel - main program.
//
// Control Panel for the RR1 robotic arm and related robots.
//
/*----------------------------------------------------------------------------*/


#ifndef __CONTROL_PANEL_MAIN_H__
#define __CONTROL_PANEL_MAIN_H__

#include "config.h"
#include "compile.h"
#include "defs.h"
#include "version.h"

using namespace std;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{

    const sInt_32 kbhit_J_S1_steps = 64;
    const sInt_32 kbhit_J_S2_steps = 64;
    const sInt_32 kbhit_J_E1_steps = 64;
    const sInt_32 kbhit_J_E2_steps = 64;
    const sInt_32 kbhit_J_W1_steps = 64;
    const sInt_32 kbhit_J_W2_steps = 64;
    const sInt_32 kbhit_J_G_steps = 64;

    
/*----------------------------------------------------------------------------*/

struct sCommandParameters
{
    sCommandParameters();
    /*--------------------------------*/
    
    sInt_32 m_seed;
};
    

struct JointsState
{    
    JointsState()
	: m_J_S1_state(0)
	, m_J_S2_state(0)
	, m_J_E1_state(0)
	, m_J_E2_state(0)
	, m_J_W1_state(0)
	, m_J_W2_state(0)
	, m_J_G_state(0)
    {
	/* nothing */
    }

    void set_Zero(void)
    {
	m_J_S1_state = 0;
	m_J_S2_state = 0;
	m_J_E1_state = 0;
	m_J_E2_state = 0;
	m_J_W1_state = 0;
	m_J_W2_state = 0;
	m_J_G_state = 0;
    }

    
    inline bool is_Zero(void) const
    {
	return (   m_J_S1_state == 0
		&& m_J_S2_state == 0
		&& m_J_E1_state == 0
		&& m_J_E2_state == 0
		&& m_J_W1_state == 0
		&& m_J_W2_state == 0
		&& m_J_G_state == 0);
    }

    
    sString to_String(void) const
    {
	sString output;

	output += " Shoulder J-S1: " + sInt_32_to_String(m_J_S1_state) + "\n";
	output += "          J-S2: " + sInt_32_to_String(m_J_S2_state) + "\n";
	output += " Elbow    J-E1: " + sInt_32_to_String(m_J_E1_state) + "\n";
	output += "          J-E2: " + sInt_32_to_String(m_J_E2_state) + "\n";
	output += " Wrist    J-W1: " + sInt_32_to_String(m_J_W1_state) + "\n";
	output += "          J-W2: " + sInt_32_to_String(m_J_W2_state) + "\n";
	output += " Gripper  J-G : " + sInt_32_to_String(m_J_G_state);

	return output;
    }
    
    sInt_32 m_J_S1_state;
    sInt_32 m_J_S2_state;
    sInt_32 m_J_E1_state;
    sInt_32 m_J_E2_state;
    sInt_32 m_J_W1_state;
    sInt_32 m_J_W2_state;
    sInt_32 m_J_G_state;   
};
        


/*----------------------------------------------------------------------------*/

    void print_IntroductoryMessage(void);
    void print_ConcludingMessage(void);
    void print_Help(void);
    
    sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &parameters);

    
/*----------------------------------------------------------------------------*/
    
    const char* find_RRMessageHeader(const char *message_buffer, sInt_32 message_buffer_size);
    void parse_JointsStateEncoder(const char *message_buffer, JointsState &joints_state);
    void parse_JointsStateExecute(const char *message_buffer, JointsState &joints_state);
    
    sResult set_InterfaceAttribs(sInt_32 fd, sInt_32 speed, sInt_32 parity);
    sResult set_InterfaceBlocking(sInt_32 fd, bool should_block);

    void refresh_Environment();
    void handle_Winch(sInt_32 sig);
    
    sResult run_RRControlPanelMainLoop(void);    

    
/*----------------------------------------------------------------------------*/

} // namespace RR_xoft


#endif /* __CONTROL_PANEL_MAIN_H__ */
