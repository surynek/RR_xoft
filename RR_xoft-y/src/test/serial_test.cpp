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
/* serial_test.cpp / 0-173_air                                                */
/*----------------------------------------------------------------------------*/
//
// Serial port test.
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "defs.h"
#include "compile.h"
#include "result.h"
#include "version.h"

#include "conio/conio.h"

#include "common/types.h"
#include "core/robot.h"
#include "util/geometry.h"

#include "test/serial_test.h"

using namespace std;
using namespace RR_xoft;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{




/*----------------------------------------------------------------------------*/

void print_Introduction(void)
{
    printf("----------------------------------------------------------------\n");    
    printf("%s : Serial Test Program\n", sPRODUCT);
    printf("%s\n", sCOPYRIGHT);
    printf("================================================================\n");
}



sResult set_interface_attribs (int fd, int speed, int parity)
{
    struct termios tty;
    if (tcgetattr (fd, &tty) != 0)
    {
	printf("error %d from tcgetattr\n", errno);
	return -1;
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
	printf("error %d from tcsetattr\n", errno);
	return -2;
    }
    return sRESULT_SUCCESS;
}

sResult set_blocking(int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
	printf("error %d from tggetattr\n", errno);
	return -1;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
	printf("error %d setting term attributes\n", errno);
	return -2;
    }
    return sRESULT_SUCCESS;
}


const char header[] = "RR1-header";
    
const char* find_header(const char *buffer, int buffer_size)
{
    for (int i = 0; i < buffer_size - sizeof(header); ++i)
    {
	bool match = true;
	for (int j = 0; j < sizeof(header) - 1; ++j)
	{
	    if (buffer[i + j] != header[j])
	    {
		match = false;
		break;
	    }	   
	}
	if (match)
	{
	    return buffer + i;
	}
    }
    return NULL;
}


struct Joints
{
    int J_S1;
    int J_S2;
    int J_E1;
    int J_E2;
    int J_W1;
    int J_W2;
    int J_G;                
};
    

void parse_joints(const char *buffer, Joints *joints)
{
    const char *joints_status = buffer + sizeof(header) - 1;

    joints->J_S1 = *((int*)(joints_status + 0 * sizeof(int)));
    joints->J_S2 = *((int*)(joints_status + 1 * sizeof(int)));
    joints->J_E1 = *((int*)(joints_status + 2 * sizeof(int)));
    joints->J_E2 = *((int*)(joints_status + 3 * sizeof(int)));
    joints->J_W1 = *((int*)(joints_status + 4 * sizeof(int)));
    joints->J_W2 = *((int*)(joints_status + 5 * sizeof(int)));
    joints->J_G = *((int*)(joints_status + 6 * sizeof(int)));	            
}

int xp, yp;    

void print_joints(const Joints *joints)
{
    gotoxy(0, 10);

    printf("RR1 joints status\n");
    printf("  Shoulder J-S1:               \n");
    printf("           J-S2:               \n");
    printf("  Elbow    J-E1:               \n");
    printf("           J-E2:               \n");
    printf("  Wrist    J-W1:               \n");
    printf("           J-W2:               \n");
    printf("  Gripper  J-G :               \n");        

    gotoxy(0, 10);
    
    printf("RR1 joints status\n");
    printf("  Shoulder J-S1: %d\n", joints->J_S1);
    printf("           J-S2: %d\n", joints->J_S2);
    printf("  Elbow    J-E1: %d\n", joints->J_E1);
    printf("           J-E2: %d\n", joints->J_E2);
    printf("  Wrist    J-W1: %d\n", joints->J_W1);
    printf("           J-W2: %d\n", joints->J_W2);
    printf("  Gripper  J-G : %d\n", joints->J_G);    
}
       
	
sResult test_serial_1(void)
{
    printf("Testing serial 1 ...\n");

    int fd = open ("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf ("error %d opening: %s\n", errno, strerror (errno));
        return -1;
    }

    set_interface_attribs (fd, B115200, 0);  // set	 speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                    // set no blocking	

    char status_buf[1024];
    int total_read = 0;
    
    while (true)
    {
	int n = read(fd, status_buf + total_read, 128);
	//printf("number of chars read: %d\n", n);

	if (n > 0)
	{
	    /*
	    for (int i = 0; i < n; ++i)
	    {
		printf("%c", status_buf[total_read + i]);
	    }
	    printf("\n");
	    */
	    total_read += n;

	    if (total_read >= 512)
	    {
		const char *position = find_header(status_buf, total_read);

		/*
		printf("***<");
		for (int i = 0; i < total_read; ++i)
		{
		    printf("%c", status_buf[i]);
		}
		printf(">***\n");
		*/
		
		if (position != NULL)
		{
		    Joints joints;
		    parse_joints(position, &joints);
		    print_joints(&joints);		    
		}
		else
		{
		    printf("Very very strange !\n");
		    return -1;
		}
		total_read = 0;
	    }
	}
	usleep(2048);
    }    
    printf("Testing serial 1 ... finished\n");
    
    return sRESULT_SUCCESS;
}

  

}  // namespace rrOST


/*----------------------------------------------------------------------------*/

int main(int sUNUSED(argc), const char **sUNUSED(argv))
{
    sResult result;

    print_Introduction();

    __CONIO_H output;    
    output.wherexy(xp, yp);    
    
    if (sFAILED(result = test_serial_1()))
    {
	printf("Test serial 1 failed (error:%d).\n", result);
	return result;
    }
    
    return 0;
}
