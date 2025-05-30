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
/* control_panel_main.cpp / 0-189_air                                         */
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
#include "util/io.h"
#include "util/statistics.h"

#include "main/control_panel_main.h"


using namespace std;
using namespace RR_xoft;

/*----------------------------------------------------------------------------*/

namespace RR_xoft
{


const char RR_serial_port[] = "/dev/ttyACM0";
const sInt_32 RR_configurations_count = 10;


const sUInt_32 J_S1_LIMITER_A_MASK = 0b00000000000000000000000000000001;
const sUInt_32 J_S1_LIMITER_B_MASK = 0b00000000000000000000000000000010;
    
const sUInt_32 J_S2_LIMITER_A_MASK = 0b00000000000000000000000000000100;
const sUInt_32 J_S2_LIMITER_B_MASK = 0b00000000000000000000000000001000;

const sUInt_32 J_E1_LIMITER_A_MASK = 0b00000000000000000000000000010000;
const sUInt_32 J_E1_LIMITER_B_MASK = 0b00000000000000000000000000100000;
    
const sUInt_32 J_E2_LIMITER_A_MASK = 0b00000000000000000000000001000000;
const sUInt_32 J_E2_LIMITER_B_MASK = 0b00000000000000000000000010000000;

const sUInt_32 J_W1_LIMITER_A_MASK = 0b00000000000000000000000100000000;
const sUInt_32 J_W1_LIMITER_B_MASK = 0b00000000000000000000001000000000;
    
const sUInt_32 J_W2_LIMITER_A_MASK = 0b00000000000000000000010000000000;
const sUInt_32 J_W2_LIMITER_B_MASK = 0b00000000000000000000100000000000;

const sUInt_32 J_G_LIMITER_A_MASK  = 0b00000000000000000001000000000000;
const sUInt_32 J_G_LIMITER_B_MASK  = 0b00000000000000000010000000000000;                            
    

/*----------------------------------------------------------------------------*/
    
sRRControlPanel RR_Control_Panel;

    
/*----------------------------------------------------------------------------*/

sCommandParameters::sCommandParameters()
    : m_seed(0)
{
    // nothing
}


/*----------------------------------------------------------------------------*/

void handle_Winch(sInt_32 sUNUSED(sig))
{
    signal(SIGWINCH, SIG_IGN);
    RR_Control_Panel.refresh_Environment();

    signal(SIGWINCH, handle_Winch);
}


/*----------------------------------------------------------------------------*/
    
void sRRControlPanel::print_IntroductoryMessage(void)
{
    printf("----------------------------------------------------------------\n");
    printf("%s : Conrol Panel\n", sPRODUCT);
    printf("%s\n", sCOPYRIGHT);
    printf("================================================================\n");	
}


void sRRControlPanel::print_ConcludingMessage(void)
{
    printf("----------------------------------------------------------------\n");
}


void sRRControlPanel::print_Help(void)
{
    printf("Usage:\n");
    printf("control_panel_RR\n");
    printf("             [--configs-file=<filename>]\n");
    printf("             [--seed=<int>\n");
    printf("\n");
    printf("Examples:\n");
    printf("control_panel_RR\n");
    printf("             --configs-file=RR1_blocksworld.txt\n");
    printf("             --seed=12345\n");
    printf("\n");
    printf("Defaults:\n");
    printf("          --configs-file=RR1_configurations.txt\n");    
    printf("          --seed=0\n");
    printf("\n");
}    


sResult sRRControlPanel::parse_CommandLineParameter(const sString &parameter, sCommandParameters &command_parameters)
{
    if (parameter.find("--seed=") == 0)
    {
	command_parameters.m_seed = sInt_32_from_String(parameter.substr(7, parameter.size()));
    }
    else if (parameter.find("--configs-file=") == 0)
    {
	command_parameters.m_configurations_filename = parameter.substr(15, parameter.size());
    }
    else
    {
	return sCONTROL_PANEL_PROGRAM_UNRECOGNIZED_PARAMETER_ERROR;
    }
    return sRESULT_SUCCESS;
}


const char sRR_message_header[] = "RR1-rev.2-robot";
const char sRR_serial_number[] = "0000";

const char* sRRControlPanel::find_RRMessageHeader(const char *message_buffer, sInt_32 message_buffer_size, sString &serial_number)
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


void sRRControlPanel::parse_MainRobotState(const char *message_buffer, sUInt_32 &main_robot_state)
{
    const char *state_buffer = message_buffer + sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2;

    main_robot_state = *((sUInt_32*)(state_buffer + 0 * sizeof(sUInt_32)));    
}


void sRRControlPanel::parse_JointsLimitersState(const char *message_buffer, sUInt_32 &limiters_state)
{
    const char *joints_state_buffer = message_buffer + sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2 + sizeof(sUInt_32);

    limiters_state = *((sUInt_32*)(joints_state_buffer + 0 * sizeof(sUInt_32)));
}

    
void sRRControlPanel::parse_JointsStateEncoder(const char *message_buffer, JointsState &joints_state)
{
    const char *joints_state_buffer = message_buffer + sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2 + 2 * sizeof(sUInt_32);

    joints_state.m_J_S1_state = *((sInt_32*)(joints_state_buffer + 0 * sizeof(sInt_32)));
    joints_state.m_J_S2_state = *((sInt_32*)(joints_state_buffer + 1 * sizeof(sInt_32)));
    joints_state.m_J_E1_state = *((sInt_32*)(joints_state_buffer + 2 * sizeof(sInt_32)));
    joints_state.m_J_E2_state = *((sInt_32*)(joints_state_buffer + 3 * sizeof(sInt_32)));
    joints_state.m_J_W1_state = *((sInt_32*)(joints_state_buffer + 4 * sizeof(sInt_32)));
    joints_state.m_J_W2_state = *((sInt_32*)(joints_state_buffer + 5 * sizeof(sInt_32)));
    joints_state.m_J_G_state  = *((sInt_32*)(joints_state_buffer + 6 * sizeof(sInt_32)));
}


void sRRControlPanel::parse_JointsStateExecute(const char *message_buffer, JointsState &joints_state)
{
    const char *joints_state_buffer = message_buffer + sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2 + 9 * sizeof(sUInt_32);

    joints_state.m_J_S1_state = *((sInt_32*)(joints_state_buffer + 0 * sizeof(sInt_32)));
    joints_state.m_J_S2_state = *((sInt_32*)(joints_state_buffer + 1 * sizeof(sInt_32)));
    joints_state.m_J_E1_state = *((sInt_32*)(joints_state_buffer + 2 * sizeof(sInt_32)));
    joints_state.m_J_E2_state = *((sInt_32*)(joints_state_buffer + 3 * sizeof(sInt_32)));
    joints_state.m_J_W1_state = *((sInt_32*)(joints_state_buffer + 4 * sizeof(sInt_32)));
    joints_state.m_J_W2_state = *((sInt_32*)(joints_state_buffer + 5 * sizeof(sInt_32)));
    joints_state.m_J_G_state  = *((sInt_32*)(joints_state_buffer + 6 * sizeof(sInt_32)));    
}


void sRRControlPanel::parse_JointsStateCummulative(const char *message_buffer, JointsState &joints_state)
{
    const char *joints_state_buffer = message_buffer + sizeof(sRR_message_header) + sizeof(sRR_serial_number) - 2 + 16 * sizeof(sUInt_32);

    joints_state.m_J_S1_state = *((sInt_32*)(joints_state_buffer + 0 * sizeof(sInt_32)));
    joints_state.m_J_S2_state = *((sInt_32*)(joints_state_buffer + 1 * sizeof(sInt_32)));
    joints_state.m_J_E1_state = *((sInt_32*)(joints_state_buffer + 2 * sizeof(sInt_32)));
    joints_state.m_J_E2_state = *((sInt_32*)(joints_state_buffer + 3 * sizeof(sInt_32)));
    joints_state.m_J_W1_state = *((sInt_32*)(joints_state_buffer + 4 * sizeof(sInt_32)));
    joints_state.m_J_W2_state = *((sInt_32*)(joints_state_buffer + 5 * sizeof(sInt_32)));
    joints_state.m_J_G_state  = *((sInt_32*)(joints_state_buffer + 6 * sizeof(sInt_32)));    
}    


sInt_32 sRRControlPanel::serialize_JointsStateExecute(const JointsState &joints_state, InteractiveStepperSafety interactive_stepper_safety, char *message_buffer)
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


sResult sRRControlPanel::set_InterfaceAttribs(sInt_32 fd, sInt_32 speed, sInt_32 parity)
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


sResult sRRControlPanel::set_InterfaceBlocking(sInt_32 fd, bool should_block)
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


sInt_32 sRRControlPanel::check_KeyboardHit()
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


/*----------------------------------------------------------------------------*/

sResult sRRControlPanel::save_ConfigurationFilenames(const sString &configurations_filename) const
{
    FILE *file;

    if ((file = fopen((sRR_default_configurations_direcotry + configurations_filename).c_str(), "w")) == NULL)
    {
	return sCONTROL_PANEL_PROGRAM_CONFIGURATIONS_FILENAMES_FILE_OPEN_ERROR;
    }

    for (auto filename: joint_configuration_Filenames)
    {
	if (!filename.empty())
	{
	    fprintf(file, "%s\n", filename.c_str());
	}
	else
	{
	    fprintf(file, "%s\n", "<-- EMPTY -->");	    
	}
    }
    fclose(file);
    
    return sRESULT_SUCCESS;
}


sResult sRRControlPanel::load_ConfigurationFilenames(const sString &configurations_filename)
{
    FILE *file;

    if ((file = fopen((sRR_default_configurations_direcotry + configurations_filename).c_str(), "r")) == NULL)
    {
	joint_configuration_Filenames.clear();
	
	for (sInt_32 i = 0; i < sRR_default_number_of_configurations; ++i)
	{
	    joint_configuration_Filenames.push_back("");	    
	}
    }    
    else
    {
	while (!feof(file))
	{	
	    sString filename;
	    sReadPrintableString(file, filename);
	    
	    if (filename != "<-- EMPTY -->")
	    {
		joint_configuration_Filenames.push_back(filename);	    
	    }
	    else
	    {
		joint_configuration_Filenames.push_back("");
	    }
	    sConsumeWhiteSpaces(file);
	}
	fclose(file);
    }
    
    return sRESULT_SUCCESS;    
}


sResult sRRControlPanel::load_JointsConfigurations(const sString &filename)
{
    FILE *file;
    
    if ((file = fopen((sRR_default_configurations_direcotry + filename + ".txt").c_str(), "r")) == NULL)
    {
	return sCONTROL_PANEL_PROGRAM_JOINT_CONFIGURATIONS_FILE_OPEN_ERROR;
    }

    int conf = 0;

    while (!feof(file))
    {
	sString line;
	sReadUntilChar(file, '\n', line);

	if (line.length() > 0)
	{
	    if (conf >= joints_Configurations.size())
	    {
		joints_Configurations.push_back(NULL);
	    }

	    if (line.find("NULL") != -1)
	    {
		if (joints_Configurations[conf] != NULL)
		{
		    delete joints_Configurations[conf];
		    joints_Configurations[conf] = NULL;
		}	    		
	    }
	    else
	    {
		if (joints_Configurations[conf] == NULL)
		{
		    joints_Configurations[conf] = new JointsState();
		}
		joints_Configurations[conf]->from_String_linear(line);
	    }
	    ++conf;
	}
    }    
    fclose(file);

    return sRESULT_SUCCESS;
}    


sResult sRRControlPanel::save_JointsConfigurations(const sString &filename) const
{
    FILE *file;
    
    if ((file = fopen((sRR_default_configurations_direcotry + filename + ".txt").c_str(), "w")) == NULL)
    {
	return sCONTROL_PANEL_PROGRAM_JOINT_CONFIGURATIONS_FILE_OPEN_ERROR;
    }
    
    for (auto configuration: joints_Configurations)
    {
	if (configuration != NULL)
	{
	    sString output;
	    output = configuration->to_String_linear();
	    fprintf(file, "%s\n", output.c_str());	    
	}
	else
	{
	    fprintf(file, "%s\n", "NULL");
	}
    }
    fclose(file);

    return sRESULT_SUCCESS;
}


/*----------------------------------------------------------------------------*/

void sRRControlPanel::refresh_Environment()
{
    Context.clear_Screen();
    Environment.refresh();    

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
    
    joints_limiters_status_Window->m_x = 3 + (2 * Environment.m_screen_width) / 3;
    joints_limiters_status_Window->m_y = 5;

    saved_configurations_Window->m_x = 2 + Environment.m_screen_width / 3;
    saved_configurations_Window->m_y = 15;

    serial_connection_Window->m_x = 1;
    serial_connection_Window->m_y = 38;
    
    title_Window->m_width = Environment.m_screen_width;
    
    joints_status_encoder_Window->m_width = Environment.m_screen_width / 3;
    joints_status_execute_Window->m_width = Environment.m_screen_width / 3;
    joints_status_cummulative_Window->m_width = Environment.m_screen_width / 3;
    joints_limiters_status_Window->m_width = Environment.m_screen_width - 2 - (2 * Environment.m_screen_width) / 3;    
    saved_configurations_Window->m_width = Environment.m_screen_width - 1 - (Environment.m_screen_width / 3);

    joints_configurations_Window->m_width = Environment.m_screen_width - 1 - (Environment.m_screen_width / 6);

    main_state_Window->m_x = joints_configurations_Window->m_x + joints_configurations_Window->m_width + 1;
    main_state_Window->m_y = 25;    
    main_state_Window->m_width = Environment.m_screen_width - joints_configurations_Window->m_width - 1;
    
    serial_connection_Window->m_width = Environment.m_screen_width;

    Environment.redraw();    
}


void sRRControlPanel::switch_EnvironmentEchoON(void)
{
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &t);       
}


void sRRControlPanel::switch_EnvironmentEchoOFF(void)
{
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(0, TCSANOW, &t);    
}


sString sRRControlPanel::configurations_to_String(const JointsStates_pvector &joints_configurations)
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


sString sRRControlPanel::limiters_to_String(sUInt_32 limiters_state)
{
    sString output;

    sString J_S1_A = (limiters_state & J_S1_LIMITER_A_MASK) ? "1" : "0";
    sString J_S1_B = (limiters_state & J_S1_LIMITER_B_MASK) ? "1" : "0";

    sString J_S2_A = (limiters_state & J_S2_LIMITER_A_MASK) ? "1" : "0";
    sString J_S2_B = (limiters_state & J_S2_LIMITER_B_MASK) ? "1" : "0";

    sString J_E1_A = (limiters_state & J_E1_LIMITER_A_MASK) ? "1" : "0";
    sString J_E1_B = (limiters_state & J_E1_LIMITER_B_MASK) ? "1" : "0";

    sString J_E2_A = (limiters_state & J_E2_LIMITER_A_MASK) ? "1" : "0";
    sString J_E2_B = (limiters_state & J_E2_LIMITER_B_MASK) ? "1" : "0";

    sString J_W1_A = (limiters_state & J_W1_LIMITER_A_MASK) ? "1" : "0";
    sString J_W1_B = (limiters_state & J_W1_LIMITER_B_MASK) ? "1" : "0";
    
    sString J_W2_A = (limiters_state & J_W2_LIMITER_A_MASK) ? "1" : "0";
    sString J_W2_B = (limiters_state & J_W2_LIMITER_B_MASK) ? "1" : "0";

    sString J_G_A = (limiters_state & J_G_LIMITER_A_MASK) ? "1" : "0";
    sString J_G_B = (limiters_state & J_G_LIMITER_B_MASK) ? "1" : "0";                    

    output += sString(" ") + "J-S1 " + "A:" + J_S1_A + "  B:" + J_S1_B + "\n";
    output += sString(" ") + "J-S2 " + "A:" + J_S2_A + "  B:" + J_S2_B + "\n";
    output += sString(" ") + "J-E1 " + "A:" + J_E1_A + "  B:" + J_E1_B + "\n";
    output += sString(" ") + "J-E2 " + "A:" + J_E2_A + "  B:" + J_E2_B + "\n";
    output += sString(" ") + "J-W1 " + "A:" + J_W1_A + "  B:" + J_W1_B + "\n";
    output += sString(" ") + "J-W2 " + "A:" + J_W2_A + "  B:" + J_W2_B + "\n";
    output += sString(" ") + "J-G  " + "A:" + J_G_A +  "  B:" + J_G_B  + "\n";

    return output;
}


sResult sRRControlPanel::initialize_RRControlPanel(const sCommandParameters &command_parameters)
{
    sResult result;
    
    joints_Configurations.resize(RR_configurations_count, NULL);

    title_Window = new sStatusWindow(Context, 0, 0, 20, 3, "RR1: Real Robot One - Control Panel");
    joints_status_encoder_Window = new sStatusWindow(Context, 0, 0, 20, 9, "Joints State");
    joints_status_execute_Window = new sStatusWindow(Context, 0, 0, 20, 9, "Execution", true);
    joints_limiters_status_Window = new sStatusWindow(Context, 0, 0, 20, 9, "Limiters");
    joints_status_cummulative_Window = new sStatusWindow(Context, 0, 0, 20, 9, "Cummulative");
    
    saved_configurations_Window = new sMenuWindow(Context, 0, 0, 20, 9, "Saved Configurations", true);
    joints_configurations_Window = new sStatusWindow(Context, 0, 0, 20, 12, "Configurations");
    main_state_Window = new sSignalWindow(Context, 0, 0, 20, 12, "Main State");
					  
    serial_connection_Window = new sMultilineWindow(Context, 0, 0, 20, 7, "Serial Port and Messages");

    title_Window->set_Text(sString("Version: ") + sPRODUCT + "    " + sCOPYRIGHT + "    " + sURL);

    Environment.m_Windows.push_back(title_Window);
    Environment.m_Windows.push_back(joints_status_encoder_Window);
    Environment.m_Windows.push_back(joints_status_execute_Window);
    Environment.m_Windows.push_back(joints_status_cummulative_Window);
    Environment.m_Windows.push_back(joints_configurations_Window);
    Environment.m_Windows.push_back(main_state_Window);
    Environment.m_Windows.push_back(joints_limiters_status_Window);            
    Environment.m_Windows.push_back(serial_connection_Window);
    Environment.m_Windows.push_back(saved_configurations_Window);

    if ((result = load_ConfigurationFilenames(command_parameters.m_configurations_filename)) != sRESULT_SUCCESS)
    {
	return result;
    }

    for (auto filename: joint_configuration_Filenames)
    {
	if (!filename.empty())
	{
	    saved_configurations_Window->add_Item(filename, sMenuWindow::ITEM_STATE_OCCUPIED);
	}
	else
	{
	    saved_configurations_Window->add_Item("<-- EMPTY SLOT -->", sMenuWindow::ITEM_STATE_EMPTY);	    
	}
    }
    joints_status_execute_Window->m_focused = true;

    joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
    joints_configurations_Window->redraw();

    joints_status_cummulative_Window->set_Text(joints_stepper_cummulative.to_String());
    joints_status_cummulative_Window->redraw();    

    refresh_Environment();
    signal(SIGWINCH, handle_Winch);
    
    switch_EnvironmentEchoOFF();
    keyboard_control_state = KEYBOARD_CONTROL_STATE_INTERACTIVE;

    serial_connection_Window->set_Text("Entering interactive keyboard control mode.");
    serial_connection_Window->set_Text("Not connected.");    
    serial_connection_Window->redraw();    

    return sRESULT_SUCCESS;
}


void sRRControlPanel::destroy_RRControlPanel(const sCommandParameters &command_parameters)
{
    save_ConfigurationFilenames(command_parameters.m_configurations_filename);
    switch_EnvironmentEchoON();
}


sResult sRRControlPanel::run_RRControlPanelMainLoop(const sCommandParameters &sUNUSED(command_parameters))
{
    sResult result;    
    
    sInt_32 serial_port = -1;
    bool port_messaged = false;

    joints_limiters_state = 0;
    limiters_state_change = 0;    
    
    JointsState joints_state_encoder;
    JointsState joints_state_execute;
    JointsState joints_state_temp;        
    JointsState kbhit_joints_execute;
    JointsState config_joints_execute;

    char status_buffer[1024];
    char command_buffer[1024];
    
    sString serial_number;
    sInt_32 total_read = 0;

    sInt_32 batch_processing_slot_position = -1;
    sInt_32 batch_processing_menu_position = -1;

    sInt_32 c = 0;
    
    while (true)
    {
	bool menu_after_exit = false;

	switch (keyboard_control_state)
	{
	case KEYBOARD_CONTROL_STATE_INTERACTIVE:
	{
	if (check_KeyboardHit() || menu_after_exit)
	{
	    char ch = getchar();
	    /*
	    #ifdef sDEBUG
	    {
		printf("pressed character:%d,%d\n", c, ch);
	    }
	    #endif
	    */
	    ++c;
	    
	    switch (ch)
	    {
	    case 9: // TAB
	    {
		/*
		#ifdef sDEBUG
		{
		    printf("TABing\n");
		}
		#endif
		*/
		Environment.rotate_Focus();
		Environment.redraw();		
		break;
	    }
	    case 10: // ENTER
	    {
		#ifdef sDEBUG
		{		
		    printf("Enter\n");
		}
		#endif
		
		if (saved_configurations_Window->m_focused)
		{
		    switch (saved_configurations_Window->get_CurrentItemState())
		    {
		    case sMenuWindow::ITEM_STATE_OCCUPIED:
		    {
			serial_connection_Window->set_Text(  "Configurations slot "
							   + joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()]
							   + " should be DELETED first to allow new name.");
			serial_connection_Window->redraw();			
			break;
		    }
		    case sMenuWindow::ITEM_STATE_EMPTY:
		    {
			sString item_text;
			
			switch_EnvironmentEchoON();
			item_text = saved_configurations_Window->enter_ItemFromKeyboard();
			joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()] = item_text;
			switch_EnvironmentEchoOFF();			
    
			menu_after_exit = true;
			saved_configurations_Window->redraw();
			break;
		    }
		    default:
		    {
			break;
		    }
		    }
		}
		break;
	    }
	    case 126: // DEL
	    {
		if (saved_configurations_Window->m_focused)
		{
		    switch (saved_configurations_Window->get_CurrentItemState())
		    {
		    case sMenuWindow::ITEM_STATE_OCCUPIED:
		    {
			sString text = joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()];
			joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()] = "";
			saved_configurations_Window->set_Item(saved_configurations_Window->get_CurrentItem(),
							      "<-- EMPTY SLOT -->",
							      sMenuWindow::ITEM_STATE_EMPTY);
			serial_connection_Window->set_Text("Configuration slot deleted: " +  text);
			serial_connection_Window->redraw();
			break;
		    }
		    case sMenuWindow::ITEM_STATE_EMPTY:
		    {
			serial_connection_Window->set_Text("Configuration slot aready empty.");
			serial_connection_Window->redraw();
			break;
		    }
		    default:
		    {
			sASSERT(false);
			break;
		    }
		    }
		    saved_configurations_Window->redraw();
		}
		break;
	    }
	    case 27: // ESC	
	    {
		if (check_KeyboardHit())
		{
		    char ch = getchar();
		    /*
	            #ifdef sDEBUG
		    {
			printf("secondary pressed characted:%d\n", ch);
		    }
	            #endif
		    */
		    if (ch == 91)
		    {
			if (check_KeyboardHit())
			{
			    char ch = getchar();
			    switch(ch)
			    {				
			    case 65: // UP
			    {
				if (saved_configurations_Window->m_focused)
				{
				    saved_configurations_Window->go_UP();
				    saved_configurations_Window->redraw();
				}
				break;
			    }				
			    case 66: // DOWN	
			    {
				if (saved_configurations_Window->m_focused)
				{
				    saved_configurations_Window->go_DOWN();
				    saved_configurations_Window->redraw();
				}				
				break;				
			    }
			    case 70: // END
			    {
				if (saved_configurations_Window->m_focused)
				{
				    saved_configurations_Window->go_END();
				    saved_configurations_Window->redraw();
				}
				break;
			    }
			    case 72: // HOME
			    {
				if (saved_configurations_Window->m_focused)
				{
				    saved_configurations_Window->go_HOME();
				    saved_configurations_Window->redraw();
				}				
				break;
			    }
			    case 53: // PGUP
			    {
				if (check_KeyboardHit())
				{
				    char ch = getchar();
				    if (ch == 126)
				    {
					if (saved_configurations_Window->m_focused)
					{
					    saved_configurations_Window->go_PAGEUP();
					    saved_configurations_Window->redraw();
					}
				    }
				}
				break;
			    }
			    case 54: // PGDOWN
			    {
				if (check_KeyboardHit())
				{
				    char ch = getchar();
				    if (ch == 126)
				    {
					if (saved_configurations_Window->m_focused)
					{
					    saved_configurations_Window->go_PAGEDOWN();
					    saved_configurations_Window->redraw();
					}					
				    }
				}				
				break;
			    }
			    case 90: // Shift TAB
			    {
				Environment.rotate_Focus(false);				
				Environment.redraw();
				break;
			    }
			    default:
			    {
				break;
			    }
			    }
			    break;
			}
		    }
		    else
		    {
			switch (ch)
			{
			case '0':
			{
			    batch_processing_slot_position = 0;
			    break;
			}
			case '1':
			{
			    batch_processing_slot_position = 1;			    
			    break;		
			}
			case '2':
			{
			    batch_processing_slot_position = 2;
			    break;		
			}		
			case '3':
			{
			    batch_processing_slot_position = 3;			    
			    break;		
			}		
			case '4':
			{
			    batch_processing_slot_position = 4;			    
			    break;		
			}		
			case '5':
			{
			    batch_processing_slot_position = 5;			    
			    break;		
			}		
			case '6':
			{
			    batch_processing_slot_position = 6;			    
			    break;		
			}		
			case '7':
			{
			    batch_processing_slot_position = 7;			    
			    break;		
			}		
			case '8':
			{
			    batch_processing_slot_position = 8;			    
			    break;		
			}		
			case '9':
			{
			    batch_processing_slot_position = 9;
			    break;		
			}			
			default:
			{			    
			    break;
			}
			}
			if (batch_processing_slot_position >= 0)
			{
			    if (main_robot_state == MAIN_STATE_STEPPER_IDLE)
			    {
				keyboard_control_state = KEYBOARD_CONTROL_STATE_SMALL_BATCH;
		
				serial_connection_Window->set_Text("Entering batch motion execution starting at configuration slot position [" + sInt_32_to_String(batch_processing_slot_position) + "].");
				serial_connection_Window->redraw();
			    }
			    else
			    {
				batch_processing_slot_position = -1;				
			    }
			}
		    }
		}
		else
		{
		    return sRESULT_SUCCESS;
		}
		break;
	    }
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[0];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[1];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[2];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[3];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[4];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[5];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[6];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[7];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[8];
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
		    config_joints_execute = joints_stepper_cummulative;		    
		    config_joints_execute -= *joints_Configurations[9];
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
		if (saved_configurations_Window->m_focused)
		{
		    switch (saved_configurations_Window->get_CurrentItemState())
		    {
		    case sMenuWindow::ITEM_STATE_OCCUPIED:
		    {
			if (sFAILED(result = save_JointsConfigurations(joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()])))
			{
			    serial_connection_Window->set_Text("Cannot save joints configurations: " + joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()] + " (error:" + sInt_32_to_String(result) + ").");
			    serial_connection_Window->redraw();
			}
			else
			{
			    serial_connection_Window->set_Text("Joints configuration saved into: " + joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()]);
			    serial_connection_Window->redraw();
			}
			break;
		    }
		    case sMenuWindow::ITEM_STATE_EMPTY:
		    {
			serial_connection_Window->set_Text("Cannot save an EMPTY CONFIGURATION slot, enter filename first.");
			serial_connection_Window->redraw();		    		    
			break;
		    }
		    default:
		    {
			break;
		    }
		    }		    
		    continue;
		}
		else
		{
		    kbhit_joints_execute.m_J_S1_state -= kbhit_J_S1_steps;
		    continue;
		}
		break;
	    }
	    case 'p':
	    {
		if (saved_configurations_Window->m_focused)
		{
		    batch_processing_menu_position = saved_configurations_Window->get_CurrentItem();
		    keyboard_control_state = KEYBOARD_CONTROL_STATE_BIG_BATCH;
		
		    serial_connection_Window->set_Text("Entering big batch motion execution starting at joint configurations '" + joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()] + "'");
		    serial_connection_Window->redraw();
		}
		break;
	    }
	    
	    case 'l':
	    {
		if (saved_configurations_Window->m_focused)
		{
		    switch (saved_configurations_Window->get_CurrentItemState())
		    {
		    case sMenuWindow::ITEM_STATE_OCCUPIED:
		    {
			if (sFAILED(result = load_JointsConfigurations(joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()])))
			{
			    serial_connection_Window->set_Text("Cannot load joints configurations: " + joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()] + " (error:" + sInt_32_to_String(result) + ").");
			    serial_connection_Window->redraw();			    
			}
			serial_connection_Window->set_Text("Configuration loaded from: " + joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()]);
			serial_connection_Window->redraw();
			
			joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
			joints_configurations_Window->redraw();					
			break;
		    }
		    case sMenuWindow::ITEM_STATE_EMPTY:
		    {
			serial_connection_Window->set_Text("Cannot load an EMPTY CONFIGURATION slot, enter filename first.");
			serial_connection_Window->redraw();		    		    
			break;
		    }
		    default:
		    {
			break;
		    }
		    }		    
		    continue;
		}
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
	break;
	}
	case KEYBOARD_CONTROL_STATE_SMALL_BATCH:
	case KEYBOARD_CONTROL_STATE_BIG_BATCH:	    
	{
	    if (check_KeyboardHit())
	    {
		keyboard_control_state = KEYBOARD_CONTROL_STATE_INTERACTIVE;
		batch_processing_slot_position = -1;		    

		serial_connection_Window->set_Text("Batch motion execution terminated (by any key).");		    
		serial_connection_Window->set_Text("Entering interactive keyboard control mode.");
		serial_connection_Window->redraw();
	    }
	    if (main_robot_state == MAIN_STATE_STEPPER_IDLE)
	    {
		if (batch_processing_menu_position >= 0)
		{
		    if (batch_processing_slot_position < 0)
		    {
			while (batch_processing_menu_position < saved_configurations_Window->get_ItemCount() && saved_configurations_Window->get_ItemState(batch_processing_menu_position) != sMenuWindow::ITEM_STATE_OCCUPIED)
			{
			    serial_connection_Window->set_Text("alpha -1:" + sInt_32_to_String(batch_processing_menu_position));
			    serial_connection_Window->redraw();			
			    ++batch_processing_menu_position;
			}
			serial_connection_Window->set_Text("alpha 2:" + sInt_32_to_String(batch_processing_menu_position) + "," + sInt_32_to_String(saved_configurations_Window->get_ItemCount()));
			serial_connection_Window->redraw();		    
			if (batch_processing_menu_position < saved_configurations_Window->get_ItemCount())
			{
			    if (sFAILED(result = load_JointsConfigurations(joint_configuration_Filenames[batch_processing_menu_position])))
			    {
				serial_connection_Window->set_Text("Cannot load joints configurations: " + joint_configuration_Filenames[batch_processing_menu_position] + " (error:" + sInt_32_to_String(result) + ").");
				serial_connection_Window->redraw();			    
			    }
			    serial_connection_Window->set_Text("Configuration loaded from: " + joint_configuration_Filenames[saved_configurations_Window->get_CurrentItem()]);
			    serial_connection_Window->redraw();
			    
			    joints_configurations_Window->set_Text(configurations_to_String(joints_Configurations));
			    joints_configurations_Window->redraw();
			    
			    batch_processing_slot_position = 0;
			    serial_connection_Window->set_Text("alpha 0");
			    serial_connection_Window->redraw();			
			}		    
			else
			{
			    keyboard_control_state = KEYBOARD_CONTROL_STATE_INTERACTIVE;
			    batch_processing_menu_position = -1;
			    
			    serial_connection_Window->set_Text("Big batch motion execution finished.");		    
			    serial_connection_Window->set_Text("Entering interactive keyboard control mode.");
			    serial_connection_Window->redraw();   		    		    
			}
		    }
		}

		if (batch_processing_slot_position >= 0)
		{
		    while (batch_processing_slot_position < RR_configurations_count && joints_Configurations[batch_processing_slot_position] == NULL)
		    {
			++batch_processing_slot_position;
		    }	
		    if (batch_processing_slot_position < RR_configurations_count)		    
		    {
			serial_connection_Window->set_Text("Executing motion to reach configuration at slot [" + sInt_32_to_String(batch_processing_slot_position) + "].");
			serial_connection_Window->redraw();    		    
			
			config_joints_execute = joints_stepper_cummulative;		    
			config_joints_execute -= *joints_Configurations[batch_processing_slot_position++];
			if (!config_joints_execute.is_Zero())
			{
			    main_robot_state = MAIN_STATE_STARTING_BATCH;
			}
		    }
		    else
		    {
			batch_processing_slot_position = -1;		    
			
			if (batch_processing_menu_position < 0)
			{
			    keyboard_control_state = KEYBOARD_CONTROL_STATE_INTERACTIVE;		    
			    serial_connection_Window->set_Text("Batch motion execution finished.");
			    serial_connection_Window->set_Text("Entering interactive keyboard control mode.");
			    serial_connection_Window->redraw();
			}
			else
			{
			    ++batch_processing_menu_position;
				saved_configurations_Window->go_TO(batch_processing_menu_position);
				saved_configurations_Window->redraw();
			}
		    }
		}
	    }
	    break;
	}
	default:
	{
	    sASSERT(false);
	    break;
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
		    
			if (position != NULL && position - status_buffer < 128)
			{
			    sUInt_32 robot_state;
			    parse_MainRobotState(position, robot_state);			    
			    if (main_robot_state == MAIN_STATE_STARTING_BATCH)
			    {
				if (robot_state != MAIN_STATE_STEPPER_IDLE)
				{
				    main_robot_state = robot_state;
				}
			    }
			    else
			    {
				main_robot_state = robot_state;
			    }
			    parse_JointsLimitersState(position, joints_limiters_state);
			    parse_JointsStateEncoder(position, joints_state_encoder);
			    parse_JointsStateExecute(position, joints_state_execute);
			    parse_JointsStateCummulative(position, joints_stepper_cummulative);

			    switch (keyboard_control_state)
			    {
			    case KEYBOARD_CONTROL_STATE_INTERACTIVE:
			    {
				switch (main_robot_state)
				{
				case MAIN_STATE_STEPPER_IDLE:
				{
				    main_state_Window->set_SignalGreen("READY");
				    break;
				}
				case MAIN_STATE_STEPPER_ACTIVE:
				{
				    main_state_Window->set_SignalRed("OPERATING");
				    break;				
				}
				default:
				{
				    break;
				}
				}
				main_state_Window->redraw();				
				break;
			    }
			    case KEYBOARD_CONTROL_STATE_SMALL_BATCH:
			    case KEYBOARD_CONTROL_STATE_BIG_BATCH:				
			    {
				switch (main_robot_state)
				{
				case MAIN_STATE_STEPPER_IDLE:				    
				case MAIN_STATE_STEPPER_ACTIVE:
				{
				    main_state_Window->set_SignalRed("OPERATING");
				    break;				
				}
				case MAIN_STATE_STARTING_BATCH:
				{
				    main_state_Window->set_SignalYellow("BUSY");
				    break;				
				}				    
				default:
				{
				    break;
				}
				}
				main_state_Window->redraw();
				break;
			    }
			    default:
			    {
				break;
			    }
			    }
			    
			    joints_status_encoder_Window->set_Text(joints_state_encoder.to_String());
			    joints_status_encoder_Window->redraw();
			    
			    joints_status_execute_Window->set_Text(joints_state_execute.to_String());
			    joints_status_execute_Window->redraw();

			    joints_limiters_status_Window->set_Text(limiters_to_String(joints_limiters_state));
			    joints_limiters_status_Window->redraw();			    

			    if (!port_messaged)
			    {
				serial_connection_Window->set_Text("Robotic arm recognized. Connected to RR1 rev.2 (serial number: " + serial_number + ").");
				serial_connection_Window->redraw();

				port_messaged = true;
			    }

			    /*
			    if (   (joints_limiters_state & J_S1_LIMITER_A_MASK) || (joints_limiters_state & J_S1_LIMITER_B_MASK)
				|| (joints_limiters_state & J_S2_LIMITER_A_MASK) || (joints_limiters_state & J_S2_LIMITER_B_MASK)
				|| (joints_limiters_state & J_E1_LIMITER_A_MASK) || (joints_limiters_state & J_E1_LIMITER_B_MASK)
				|| (joints_limiters_state & J_E2_LIMITER_A_MASK) || (joints_limiters_state & J_E2_LIMITER_B_MASK)
				|| (joints_limiters_state & J_W1_LIMITER_A_MASK) || (joints_limiters_state & J_W1_LIMITER_B_MASK)
				|| (joints_limiters_state & J_W2_LIMITER_A_MASK) || (joints_limiters_state & J_W2_LIMITER_B_MASK)
			    */
			    joints_status_cummulative_Window->set_Text(joints_stepper_cummulative.to_String());
			    joints_status_cummulative_Window->redraw();			
			    
			}
			else
			{
			    serial_connection_Window->set_Text("Cannot recognize the RR1 robotic arm.");
			    serial_connection_Window->redraw();

			    port_messaged = false;			    
			    
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


sRRControlPanel:: ~sRRControlPanel()
{    
    destroy_RRControlPanel(command_parameters);

    if (title_Window != NULL)
    {
	delete title_Window;
    }
    if (joints_status_encoder_Window != NULL)
    {
	delete joints_status_encoder_Window;
    }
    if (joints_status_execute_Window != NULL)
    {
	delete joints_status_execute_Window;
    }
    if (joints_status_cummulative_Window != NULL)
    {
	delete joints_status_cummulative_Window;
    }
    if (joints_configurations_Window != NULL)
    {
	delete joints_configurations_Window;
    }
    if (joints_limiters_status_Window != NULL)
    {
	delete joints_limiters_status_Window;
    }
    if (saved_configurations_Window != NULL)
    {
	delete saved_configurations_Window;
    }
    if (serial_connection_Window != NULL)
    {
	delete serial_connection_Window;
    }
    if (main_state_Window != NULL)
    {
	delete main_state_Window;
    }

    for (auto configuration: joints_Configurations)
    {
	if (configuration != NULL)
	{
	    delete configuration;
	}
    }
}


/*----------------------------------------------------------------------------*/

} // namespace RR_xoft



/*----------------------------------------------------------------------------*/
// main program

int main(int argc, char **argv)
{
    sResult result;

    RR_Control_Panel.print_IntroductoryMessage();

    if (argc >= 1 && argc <= 2)
    {
	for (int i = 1; i < argc; ++i)
	{
	    result = RR_Control_Panel.parse_CommandLineParameter(argv[i], RR_Control_Panel.command_parameters);
	    if (sFAILED(result))
	    {
		printf("Error: Cannot parse command line parameters (code = %d).\n", result);
		RR_Control_Panel.print_Help();

		return result;
	    }
	}

	if (sFAILED(result = RR_Control_Panel.initialize_RRControlPanel(RR_Control_Panel.command_parameters)))
	{
	    printf("Error: Cannot initialize user interface (code = %d).\n", result);
	    return result;
	}

	result = RR_Control_Panel.run_RRControlPanelMainLoop(RR_Control_Panel.command_parameters);
	
	if (sFAILED(result))
	{
	    RR_Control_Panel.destroy_RRControlPanel(RR_Control_Panel.command_parameters);
	    return result;
	}
	RR_Control_Panel.destroy_RRControlPanel(RR_Control_Panel.command_parameters);
    }
    else
    {
	RR_Control_Panel.print_Help();
    }
    return sRESULT_SUCCESS;
}
