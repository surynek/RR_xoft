/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-173_air                             */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                  */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* control_panel_main.h / 0-173_air                                           */
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
    const sInt_32 kbhit_J_G_steps  = 64;

    const sInt_32 kbhit_J_max_steps = 512;

    typedef enum
    {
	INTERACTIVE_STEPPER_SAFETY_UNDEFINED = 0,
	INTERACTIVE_STEPPER_SAFETY_HIGH = 1,
	INTERACTIVE_STEPPER_SAFETY_LOW = 2
    } InteractiveStepperSafety;

    
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

    
    void negate(void)
    {
	m_J_S1_state *= -1;
	m_J_S2_state *= -1;
	m_J_E1_state *= -1;
	m_J_E2_state *= -1;
	m_J_W1_state *= -1;
	m_J_W2_state *= -1;
	m_J_G_state *= -1;
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

    sString to_String_linear(void) const
    {
	sString output;

	output += "J-S1: " + sInt_32_to_String(m_J_S1_state) + "  ";
	output += "J-S2: " + sInt_32_to_String(m_J_S2_state) + "  ";
	output += "J-E1: " + sInt_32_to_String(m_J_E1_state) + "  ";
	output += "J-E2: " + sInt_32_to_String(m_J_E2_state) + "  ";
	output += "J-W1: " + sInt_32_to_String(m_J_W1_state) + "  ";
	output += "J-W2: " + sInt_32_to_String(m_J_W2_state) + "  ";
	output += "J-G: " + sInt_32_to_String(m_J_G_state);

	return output;
    }    

    JointsState& operator+=(const JointsState &joints_state)
    {
	m_J_S1_state += joints_state.m_J_S1_state;
	m_J_S2_state += joints_state.m_J_S2_state;
	m_J_E1_state += joints_state.m_J_E1_state;
	m_J_E2_state += joints_state.m_J_E2_state;
	m_J_W1_state += joints_state.m_J_W1_state;
	m_J_W2_state += joints_state.m_J_W2_state;
	m_J_G_state  += joints_state.m_J_G_state;
	
	return *this;
    }

    JointsState& operator-=(const JointsState &joints_state)
    {
	m_J_S1_state -= joints_state.m_J_S1_state;
	m_J_S2_state -= joints_state.m_J_S2_state;
	m_J_E1_state -= joints_state.m_J_E1_state;
	m_J_E2_state -= joints_state.m_J_E2_state;
	m_J_W1_state -= joints_state.m_J_W1_state;
	m_J_W2_state -= joints_state.m_J_W2_state;
	m_J_G_state  -= joints_state.m_J_G_state;
	
	return *this;
    }   
    
    sInt_32 m_J_S1_state;
    sInt_32 m_J_S2_state;
    sInt_32 m_J_E1_state;
    sInt_32 m_J_E2_state;
    sInt_32 m_J_W1_state;
    sInt_32 m_J_W2_state;
    sInt_32 m_J_G_state;   
};

    
typedef std::vector<JointsState> JointsStates_vector;
typedef std::vector<JointsState*> JointsStates_pvector;    

    
/*----------------------------------------------------------------------------*/

typedef std::vector<sString> String_vector;
    
    
/*----------------------------------------------------------------------------*/

void handle_Winch(sInt_32 sig);

    
/*----------------------------------------------------------------------------*/

struct sRRControlPanel
{
    virtual ~sRRControlPanel();
    
    static void print_IntroductoryMessage(void);
    static void print_ConcludingMessage(void);
    static void print_Help(void);
    
    static sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &parameters);

    static const char* find_RRMessageHeader(const char *message_buffer, sInt_32 message_buffer_size, sString &serial_number);

    static void parse_JointsLimitersState(const char *message_buffer, sUInt_32 &limiters_state);
    static void parse_JointsStateEncoder(const char *message_buffer, JointsState &joints_state);
    static void parse_JointsStateExecute(const char *message_buffer, JointsState &joints_state);
    static void parse_JointsStateCummulative(const char *message_buffer, JointsState &joints_state);    

    static sInt_32 serialize_JointsStateExecute(const JointsState &joints_state, InteractiveStepperSafety interactive_stepper_safety, char *message_buffer);    

    static sResult set_InterfaceAttribs(sInt_32 fd, sInt_32 speed, sInt_32 parity);
    static sResult set_InterfaceBlocking(sInt_32 fd, bool should_block);

    static sString limiters_to_String(sUInt_32 limiters_state);    
    static sString configurations_to_String(const JointsStates_pvector &joints_configurations);

    static sInt_32 check_KeyboardHit();
    /*--------------------------------*/

    sResult save_ConfigurationFilenames(void);
    sResult load_ConfigurationFilenames(void);
    /*--------------------------------*/        

    void refresh_Environment();
    
    void switch_EnvironmentEchoON(void);
    void switch_EnvironmentEchoOFF(void);    

    sResult initialize_RRControlPanel(void);
    void destroy_RRControlPanel(void);
    
    sResult run_RRControlPanelMainLoop(void);
    /*--------------------------------*/
    
    sContext Context;
    sEnvironment Environment;

    JointsState joints_stepper_cummulative;
    JointsStates_pvector joints_Configurations;
    sUInt_32 joints_limiters_state;
    
    sStatusWindow *title_Window = NULL;
    sStatusWindow *joints_status_encoder_Window = NULL;
    sStatusWindow *joints_status_execute_Window = NULL;
    sStatusWindow *joints_status_cummulative_Window = NULL;
    sStatusWindow *joints_configurations_Window = NULL;
    sStatusWindow *joints_limiters_status_Window = NULL;
    
    sMenuWindow *saved_configurations_Window = NULL;
    sStatusWindow *serial_connection_Window = NULL;
    
    sUInt_32 general_robot_state;
    sUInt_32 limiters_state_change;

    String_vector joint_configuration_Filenames;
};

    
/*----------------------------------------------------------------------------*/

} // namespace RR_xoft


#endif /* __CONTROL_PANEL_MAIN_H__ */
