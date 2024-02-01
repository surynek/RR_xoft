/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-121_air                              */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* control_panel_main.cpp / 0-121_air                                         */
/*----------------------------------------------------------------------------*/
//
// Control Panel - main program.
//
// Control Panel for the RR1 robotic arm and related robots.
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <sys/times.h>
#include <sys/select.h>
#include <sys/ioctl.h>


#include "config.h"
#include "compile.h"
#include "defs.h"
#include "version.h"

#include "core/robot.h"
#include "ui/tui.h"
#include "util/statistics.h"

#include "main/control_panel_main.h"


using namespace std;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


const char RR_serial_port[] = "/dev/ttyACM0";
const sInt_32 RR_configurations_count = 10;


    
    
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
    printf("%s : Conrol Panel\n", sPRODUCT);
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
    printf("control_panel_RR\n");
    printf("             [--seed=<int>\n");
    printf("\n");
    printf("Examples:\n");
    printf("control_panel_RR\n");
    printf("             --seed=12345\n");
    printf("\n");
    printf("Defaults:\n");
    printf("          --seed=0\n");
    printf("\n");
}    


sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &command_parameters)
{
    if (parameter.find("--seed=") == 0)
    {
	command_parameters.m_seed = sInt_32_from_String(parameter.substr(7, parameter.size()));
    }
    else
    {
	return sCONTROL_PANEL_PROGRAM_UNRECOGNIZED_PARAMETER_ERROR;
    }
    return sRESULT_SUCCESS;
}


const char sRR_message_header[] = "RR1-rev.2-robot";
const char sRR_serial_number[] = "0000";
    
const char* find_RRMessageHeader(const char *message_buffer, sInt_32 message_buffer_size, sString &serial_number)
{
    sInt_32 message_header_size = sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2;
    //printf("%d,%d,%d\n", sizeof(sRR_message_header), sizeof(sRR_serial_number), message_header_size);
    
    for (int i = 0; i < message_buffer_size - message_header_size; ++i)
    {
	bool match = true;
	
	for (int j = 0; j < sizeof(sRR_message_header) - 1; ++j)
	{
	    if (message_buffer[i + j] != sRR_message_header[j])
	    {
		match = false;
		break;
	    }	   
	}
	if (match)
	{
	    /*
	    printf("head:%d\n", message_header_size);    
	    for (int j = i; j < i + message_header_size; ++j)
	    {
		printf("%d (%c)\n", message_buffer[j], message_buffer[j]);
	    }
	    printf("data:%d\n", message_buffer_size);
	    for (int j = i + message_header_size; j < message_buffer_size; ++j)
	    {
		printf("%d: %d (%c)\n", j - i - message_header_size, message_buffer[j], message_buffer[j]);
	    }    
	    getchar();
	    
	    printf("Match:%c%c%c%c\n",
		   message_buffer[i + (sizeof(sRR_message_header) - 1) + 0],
		   message_buffer[i + (sizeof(sRR_message_header) - 1) + 1],
		   message_buffer[i + (sizeof(sRR_message_header) - 1) + 2],
		   message_buffer[i + (sizeof(sRR_message_header) - 1) + 3]);	    
	    */
	    serial_number =  sString("") + message_buffer[i + (sizeof(sRR_message_header) - 1) + 0]
		            + message_buffer[i + (sizeof(sRR_message_header) - 1) + 1]
		            + message_buffer[i + (sizeof(sRR_message_header) - 1) + 2]
		            + message_buffer[i + (sizeof(sRR_message_header) - 1) + 3];
	    
	    return message_buffer + i;
	}
    }
    return NULL;
}


void parse_JointsStateEncoder(const char *message_buffer, JointsState &joints_state)
{
    const char *joints_state_buffer = message_buffer + sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2;

    joints_state.m_J_S1_state = *((sInt_32*)(joints_state_buffer + 0 * sizeof(sInt_32)));
    joints_state.m_J_S2_state = *((sInt_32*)(joints_state_buffer + 1 * sizeof(sInt_32)));
    joints_state.m_J_E1_state = *((sInt_32*)(joints_state_buffer + 2 * sizeof(sInt_32)));
    joints_state.m_J_E2_state = *((sInt_32*)(joints_state_buffer + 3 * sizeof(sInt_32)));
    joints_state.m_J_W1_state = *((sInt_32*)(joints_state_buffer + 4 * sizeof(sInt_32)));
    joints_state.m_J_W2_state = *((sInt_32*)(joints_state_buffer + 5 * sizeof(sInt_32)));
    joints_state.m_J_G_state  = *((sInt_32*)(joints_state_buffer + 6 * sizeof(sInt_32)));
}


void parse_JointsStateExecute(const char *message_buffer, JointsState &joints_state)
{
    const char *joints_state_buffer = message_buffer + sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2 + 7 * sizeof(sInt_32);

    joints_state.m_J_S1_state = *((sInt_32*)(joints_state_buffer + 0 * sizeof(sInt_32)));
    joints_state.m_J_S2_state = *((sInt_32*)(joints_state_buffer + 1 * sizeof(sInt_32)));
    joints_state.m_J_E1_state = *((sInt_32*)(joints_state_buffer + 2 * sizeof(sInt_32)));
    joints_state.m_J_E2_state = *((sInt_32*)(joints_state_buffer + 3 * sizeof(sInt_32)));
    joints_state.m_J_W1_state = *((sInt_32*)(joints_state_buffer + 4 * sizeof(sInt_32)));
    joints_state.m_J_W2_state = *((sInt_32*)(joints_state_buffer + 5 * sizeof(sInt_32)));
    joints_state.m_J_G_state  = *((sInt_32*)(joints_state_buffer + 6 * sizeof(sInt_32)));    
}    


sInt_32 serialize_JointsStateExecute(const JointsState &joints_state, InteractiveStepperSafety interactive_stepper_safety, char *message_buffer)
{
    char *joints_state_buffer = message_buffer + sizeof(sRR_message_header) - 1;
    strncpy(message_buffer, sRR_message_header, sizeof(sRR_message_header) - 1);

    *((sInt_32*)(joints_state_buffer + 0 * sizeof(sInt_32))) = interactive_stepper_safety;
    
    *((sInt_32*)(joints_state_buffer + 1 * sizeof(sInt_32))) = joints_state.m_J_S1_state;    
    *((sInt_32*)(joints_state_buffer + 2 * sizeof(sInt_32))) = joints_state.m_J_S2_state;
    *((sInt_32*)(joints_state_buffer + 3 * sizeof(sInt_32))) = joints_state.m_J_E1_state;
    *((sInt_32*)(joints_state_buffer + 4 * sizeof(sInt_32))) = joints_state.m_J_E2_state;
    *((sInt_32*)(joints_state_buffer + 5 * sizeof(sInt_32))) = joints_state.m_J_W1_state;
    *((sInt_32*)(joints_state_buffer + 6 * sizeof(sInt_32))) = joints_state.m_J_W2_state;
    *((sInt_32*)(joints_state_buffer + 7 * sizeof(sInt_32))) = joints_state.m_J_G_state;

    return (sizeof(sRR_message_header) - 1 + 8 * sizeof(sInt_32));
}


sResult set_InterfaceAttribs(sInt_32 fd, sInt_32 speed, sInt_32 parity)
{
    struct termios tty;
    if (tcgetattr (fd, &tty) != 0)
    {
	printf("Error: tcgetattr failed (errno = %d).\n", errno);
	return sCONTROL_PANEL_PROGRAM_GET_ATTR_ERROR;
    }
    
    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);
    
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
    
    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
	printf("Error: tcsetattr failed (errno = %d).\n", errno);
	return sCONTROL_PANEL_PROGRAM_SET_ATTR_ERROR;
    }
    return sRESULT_SUCCESS;
}


sResult set_InterfaceBlocking(sInt_32 fd, bool should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
	printf("Error: tggetattr failed (errno = %d).\n", errno);
	return sCONTROL_PANEL_PROGRAM_GET_ATTR_ERROR;;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
	printf("Error: setting term attributes failed (errno = %d).\n", errno);
	return sCONTROL_PANEL_PROGRAM_SET_ATTR_ERROR;;
    }
    return sRESULT_SUCCESS;
}


sInt_32 check_KeyboardHit()
{
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized)
    {
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}



sContext Context;
sEnvironment Environment;

JointsState joints_stepper_cummulative;
JointsStates_pvector joints_Configurations;

sStatusWindow *title_Window;
sStatusWindow *joints_status_encoder_Window;
sStatusWindow *joints_status_execute_Window;
sStatusWindow *joints_status_cummulative_Window;
sStatusWindow *joints_configurations_Window;
sStatusWindow *serial_connection_Window;

void refresh_Environment()
{
    Context.clear_Screen();

    title_Window->m_x = 1;
    title_Window->m_y = 1;    
    
    joints_status_encoder_Window->m_x = 1;
    joints_status_encoder_Window->m_y = 5;

    joints_status_execute_Window->m_x = 2 + Environment.m_screen_width / 3;
    joints_status_execute_Window->m_y = 5;

    joints_status_cummulative_Window->m_x = 1;
    joints_status_cummulative_Window->m_y = 15;

    joints_configurations_Window->m_x = 1;
    joints_configurations_Window->m_y = 25;

    serial_connection_Window->m_x = 1;
    serial_connection_Window->m_y = 38;

    Environment.refresh();    
    
    title_Window->m_width = Environment.m_screen_width;
    
    joints_status_encoder_Window->m_width = Environment.m_screen_width / 3;
    joints_status_execute_Window->m_width = Environment.m_screen_width / 3;
    joints_status_cummulative_Window->m_width = Environment.m_screen_width / 3;

    joints_configurations_Window->m_width = Environment.m_screen_width;    
    serial_connection_Window->m_width = Environment.m_screen_width;

    Environment.redraw();    
}


void handle_Winch(sInt_32 sUNUSED(sig))
{
    signal(SIGWINCH, SIG_IGN);
    refresh_Environment();

    signal(SIGWINCH, handle_Winch);
}    


sString configurations_to_String(const JointsStates_pvector &joints_configurations)
{
    sASSERT(joints_configurations.size() == RR_configurations_count);
    sString output;
    
    for (sInt_32 i = 0; i < RR_configurations_count; ++i)
    {
	if (joints_configurations[i] != NULL)
	{
	    output += sString(" ") + "[" + sInt_32_to_String(i) + "] " + joints_configurations[i]->to_String_linear() + "\n";
	}
	else
	{
	    output += sString(" ") + "[" + sInt_32_to_String(i) + "]\n";
	}
    }

    return output;
}


sResult initialize_RRControlPanel(void)
{
    joints_Configurations.resize(RR_configurations_count, NULL);
    
    title_Window = new sStatusWindow(Context, 0, 0, 20, 3, "RR1: Real Robot One - Control Panel");
    joints_status_encoder_Window = new sStatusWindow(Context, 0, 0, 20, 9, "Joints State");
    joints_status_execute_Window = new sStatusWindow(Context, 0, 0, 20, 9, "Execution");
    joints_status_cummulative_Window = new sStatusWindow(Context, 0, 0, 20, 9, "Cummulative");

    joints_configurations_Window = new sStatusWindow(Context, 0, 0, 20, 12, "Configurations");    
    serial_connection_Window = new sStatusWindow(Context, 0, 0, 20, 3, "Serial Port");

    title_Window->set_Text(sString("Version: ") + sPRODUCT + "    " + sCOPYRIGHT + "    " + sURL);
    serial_connection_Window->set_Text("Not connected.");

    Environment.m_Windows.push_back(title_Window);
    Environment.m_Windows.push_back(joints_status_encoder_Window);
    Environment.m_Windows.push_back(joints_status_execute_Window);
    Environment.m_Windows.push_back(joints_status_cummulative_Window);
    Environment.m_Windows.push_back(joints_configurations_Window);        
    Environment.m_Windows.push_back(serial_connection_Window);

    joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
    joints_configurations_Window->redraw();

    joints_status_cummulative_Window->set_Text(joints_stepper_cummulative.to_String());
    joints_status_cummulative_Window->redraw();    

    refresh_Environment();
    signal(SIGWINCH, handle_Winch);    

    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(0, TCSANOW, &t);

    return sRESULT_SUCCESS;
}


void destroy_RRControlPanel(void)
{    
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &t);   
}


sResult run_RRControlPanelMainLoop(void)
{
    sResult result;
    
    int serial_port = -1;
    
    JointsState joints_state_encoder;
    JointsState joints_state_execute;
    JointsState kbhit_joints_execute;
    JointsState config_joints_execute;    

    char status_buffer[1024];
    char command_buffer[1024];
    
    sString serial_number;
    int total_read = 0;
    
    while (true)
    {	
	if (check_KeyboardHit())
	{
	    char ch = getchar();
	    //printf("%d\n", ch);

	    switch (ch)
	    {
	    case ')':
	    {
		if (joints_Configurations[0] != NULL)
		{
		    delete joints_Configurations[0];
		    joints_Configurations[0] = NULL;
		}
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;
	    }
	    case '!':
	    {
		if (joints_Configurations[1] != NULL)
		{
		    delete joints_Configurations[1];
		    joints_Configurations[1] = NULL;
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;
	    }
	    case '@':
	    {
		if (joints_Configurations[2] != NULL)
		{
		    delete joints_Configurations[2];
		    joints_Configurations[2] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '#':
	    {
		if (joints_Configurations[3] != NULL)
		{
		    delete joints_Configurations[3];
		    joints_Configurations[3] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '$':
	    {
		if (joints_Configurations[4] != NULL)
		{
		    delete joints_Configurations[4];
		    joints_Configurations[4] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '%':
	    {
		if (joints_Configurations[5] != NULL)
		{
		    delete joints_Configurations[5];
		    joints_Configurations[5] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '^':
	    {
		if (joints_Configurations[6] != NULL)
		{
		    delete joints_Configurations[6];
		    joints_Configurations[6] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '&':
	    {
		if (joints_Configurations[7] != NULL)
		{
		    delete joints_Configurations[7];
		    joints_Configurations[7] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '*':
	    {
		if (joints_Configurations[8] != NULL)
		{
		    delete joints_Configurations[8];
		    joints_Configurations[8] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '(':
	    {
		if (joints_Configurations[9] != NULL)
		{
		    delete joints_Configurations[9];
		    joints_Configurations[9] = NULL;
		}				
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();				
		continue;
		break;		
	    }
	    case '0':
	    {
		if (joints_Configurations[0] != NULL)
		{
		    config_joints_execute = *joints_Configurations[0];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[0] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '1':
	    {
		if (joints_Configurations[1] != NULL)
		{
		    config_joints_execute = *joints_Configurations[1];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[1] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '2':
	    {
		if (joints_Configurations[2] != NULL)
		{
		    config_joints_execute = *joints_Configurations[2];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[2] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '3':
	    {
		if (joints_Configurations[3] != NULL)
		{
		    config_joints_execute = *joints_Configurations[3];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[3] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '4':
	    {
		if (joints_Configurations[4] != NULL)
		{
		    config_joints_execute = *joints_Configurations[4];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[4] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '5':
	    {
		if (joints_Configurations[5] != NULL)
		{
		    config_joints_execute = *joints_Configurations[5];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[5] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '6':
	    {
		if (joints_Configurations[6] != NULL)
		{
		    config_joints_execute = *joints_Configurations[6];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[6] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '7':
	    {
		if (joints_Configurations[7] != NULL)
		{
		    config_joints_execute = *joints_Configurations[7];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[7] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '8':
	    {
		if (joints_Configurations[8] != NULL)
		{
		    config_joints_execute = *joints_Configurations[8];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[8] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }
	    case '9':
	    {
		if (joints_Configurations[9] != NULL)
		{
		    config_joints_execute = *joints_Configurations[9];
		    config_joints_execute -= joints_stepper_cummulative;
		}
		else
		{
		    joints_Configurations[9] = new JointsState(joints_stepper_cummulative);
		}		
		joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
		joints_configurations_Window->redraw();		
		continue;
		break;
	    }	    	    	    
	    case 'w':
	    {
		kbhit_joints_execute.m_J_S1_state += kbhit_J_S1_steps;
		continue;		
		break;
	    }	
	    case 's':
	    {
		kbhit_joints_execute.m_J_S1_state -= kbhit_J_S1_steps;
		continue;		
		break;
	    }
	    case 'e':
	    {
		kbhit_joints_execute.m_J_S2_state += kbhit_J_S2_steps;
		continue;		
		break;
	    }	
	    case 'd':
	    {
		kbhit_joints_execute.m_J_S2_state -= kbhit_J_S2_steps;
		continue;		
		break;
	    }	
	    case 'r':
	    {
		kbhit_joints_execute.m_J_E1_state += kbhit_J_E1_steps;
		continue;		
		break;
	    }	
	    case 'f':
	    {
		kbhit_joints_execute.m_J_E1_state -= kbhit_J_E1_steps;
		continue;		
		break;
	    }
	    case 't':
	    {
		kbhit_joints_execute.m_J_E2_state += kbhit_J_E2_steps;
		continue;		
		break;
	    }	
	    case 'g':
	    {
		kbhit_joints_execute.m_J_E2_state -= kbhit_J_E2_steps;
		continue;		
		break;
	    }
	    case 'y':
	    {
		kbhit_joints_execute.m_J_W1_state += kbhit_J_W1_steps;
		continue;		
		break;
	    }	
	    case 'h':
	    {
		kbhit_joints_execute.m_J_W1_state -= kbhit_J_W1_steps;
		continue;		
		break;
	    }
	    case 'u':
	    {
		kbhit_joints_execute.m_J_W2_state += kbhit_J_W2_steps;
		continue;		
		break;
	    }	
	    case 'j':
	    {
		kbhit_joints_execute.m_J_W2_state -= kbhit_J_W2_steps;
		continue;		
		break;
	    }
	    case 'i':
	    {
		kbhit_joints_execute.m_J_G_state += kbhit_J_G_steps;
		continue;		
		break;
	    }	
	    case 'k':
	    {
		kbhit_joints_execute.m_J_G_state -= kbhit_J_G_steps;
		continue;
		break;
	    }		    	    	    	       	    
	    case 'c':
	    {
		serial_port = open (RR_serial_port, O_RDWR | O_NOCTTY | O_SYNC);
		
		if (serial_port < 0)
		{
		    char error[1024];
		    sprintf (error, "Error: cannot open serial port %s (strerror = %s).\n", RR_serial_port, strerror (errno));
		    
		    serial_connection_Window->set_Text(error);
		    serial_connection_Window->redraw();		    		    
		    
		    //return sCONTROL_PANEL_PROGRAM_SERIAL_PORT_OPEN_ERROR;
		}
		else
		{
		    serial_connection_Window->set_Text("Connected to RR1 rev.2 (serial number: N/A).");
		    serial_connection_Window->redraw();		    		    
		    
		    if (sFAILED(result = set_InterfaceAttribs (serial_port, B115200, 0))) // set speed to 115,200 bps, 8n1 (no parity)
		    {
			return result;
		    }
		    if (sFAILED(result = set_InterfaceBlocking (serial_port, 0))) // set no blocking
		    {
			return result;
		    }
		}
		break;
	    }
	    case 'x':
	    {
		if (serial_port >= 0)
		{
		    close(serial_port);
		    serial_port = -1;
		}
		serial_connection_Window->set_Text("Not connected.");
		serial_connection_Window->redraw();
		
		break;
	    }	    
	    default:
	    {
		break;
	    }
	    }
	}

	if (serial_port >= 0)
	{
	    int data_waiting;
	    ioctl(serial_port, FIONREAD, &data_waiting);

	    if (data_waiting != 0)
	    {
		sInt_32 n = read(serial_port, status_buffer + total_read, 128);
		//printf("read n:%d\n", n);
	    
		if (n > 0)
		{
		    /*
		    for (int i = 0; i < n; ++i)
		    {
			printf("%c (%d) ", status_buffer[total_read + i], status_buffer[total_read + i]);
		    }
		    */
		    total_read += n;
		   
		    if (total_read >= 256)
		    {
			/*
			printf("total:\n");
			for (int i = 0; i < total_read; ++i)
			{
			    printf("%c (%d) ", status_buffer[i], status_buffer[i]);
			}
			return sRESULT_SUCCESS;
			*/
			const char *position = find_RRMessageHeader(status_buffer, total_read, serial_number);
			sASSERT(position - status_buffer < 128);
		    
			if (position != NULL)
			{
			    parse_JointsStateEncoder(position, joints_state_encoder);
			    parse_JointsStateExecute(position, joints_state_execute);
			
			    joints_status_encoder_Window->set_Text(joints_state_encoder.to_String());
			    joints_status_encoder_Window->redraw();
			    
			    joints_status_execute_Window->set_Text(joints_state_execute.to_String());
			    joints_status_execute_Window->redraw();
			    			    
			    serial_connection_Window->set_Text("Robotic arm recognized. Connected to RR1 rev.2 (serial number: " + serial_number + ").");
			    serial_connection_Window->redraw();		    		    			
			}
			else
			{
			    serial_connection_Window->set_Text("Cannot recognize the RR1 robotic arm.");
			    serial_connection_Window->redraw();
			    
			//printf("Error: cannot read joints status.\n");
			//return sCONTROL_PANEL_PROGRAM_JOINTS_STATE_READ_ERROR;
			}
			total_read = 0;
		    }
		}
	    
		if (!kbhit_joints_execute.is_Zero())
		{
		    if (kbhit_joints_execute.m_J_S1_state > kbhit_J_max_steps)
		    {
			kbhit_joints_execute.m_J_S1_state = kbhit_J_max_steps;
		    }
		    if (kbhit_joints_execute.m_J_S2_state > kbhit_J_max_steps)
		    {
			kbhit_joints_execute.m_J_S2_state = kbhit_J_max_steps;
		    }		
		    if (kbhit_joints_execute.m_J_E1_state > kbhit_J_max_steps)
		    {
			kbhit_joints_execute.m_J_E1_state = kbhit_J_max_steps;
		    }
		    if (kbhit_joints_execute.m_J_E2_state > kbhit_J_max_steps)
		    {
			kbhit_joints_execute.m_J_E2_state = kbhit_J_max_steps;
		    }
		    if (kbhit_joints_execute.m_J_W1_state > kbhit_J_max_steps)
		    {
			kbhit_joints_execute.m_J_W1_state = kbhit_J_max_steps;
		    }
		    if (kbhit_joints_execute.m_J_W2_state > kbhit_J_max_steps)
		    {
			kbhit_joints_execute.m_J_W2_state = kbhit_J_max_steps;
		    }
		    if (kbhit_joints_execute.m_J_G_state > kbhit_J_max_steps)
		    {
			kbhit_joints_execute.m_J_G_state = kbhit_J_max_steps;
		    }
		    
		    sInt_32 command_size = serialize_JointsStateExecute(kbhit_joints_execute, INTERACTIVE_STEPPER_SAFETY_HIGH, command_buffer);
		    /*	
			printf("\n");
		printf("Sendo:%d\n", command_size);
		for (sInt_32 j = 0; j < command_size; ++j)
		{
		    printf("%c (%d) ", command_buffer[j], command_buffer[j]);
		}
		printf("\n");
		*/
		    sInt_32 n_wr = write(serial_port, command_buffer, command_size);
		    
		    if (n_wr < command_size)
		    {
			serial_connection_Window->set_Text("Cannot write keyboard interactive command.");
			serial_connection_Window->redraw();		    
		    }
		    else
		    {
			joints_stepper_cummulative += kbhit_joints_execute;
			
			joints_status_cummulative_Window->set_Text(joints_stepper_cummulative.to_String());
			joints_status_cummulative_Window->redraw();
			
			kbhit_joints_execute.set_Zero();		    
		    }
		    continue;
		}
		if (!config_joints_execute.is_Zero())
		{				
		    sInt_32 command_size = serialize_JointsStateExecute(config_joints_execute, INTERACTIVE_STEPPER_SAFETY_LOW, command_buffer);
		    sInt_32 n_wr = write(serial_port, command_buffer, command_size);
		    
		    if (n_wr < command_size)
		    {
			serial_connection_Window->set_Text("Cannot write configuration interactive command.");
			serial_connection_Window->redraw();		    
		    }
		    else
		    {
			joints_stepper_cummulative += config_joints_execute;
			
			joints_status_cummulative_Window->set_Text(joints_stepper_cummulative.to_String());
			joints_status_cummulative_Window->redraw();
			
			config_joints_execute.set_Zero();		    
		    }
		    continue;
		}
	    }
	}
	
	usleep(256);
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

    if (argc >= 1 && argc <= 2)
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

	if (sFAILED(result = initialize_RRControlPanel()))
	{
	    printf("Error: Cannot initialize user interface (code = %d).\n", result);
	    return result;
	}	

	result = run_RRControlPanelMainLoop();
	
	if (sFAILED(result))
	{
	    destroy_RRControlPanel();
	    return result;
	}
    }
    else
    {
	print_Help();
    }
    return sRESULT_SUCCESS;
}
