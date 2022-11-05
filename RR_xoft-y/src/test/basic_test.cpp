/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-057_air                              */
/*                                                                            */
/*                  (C) Copyright 2021 - 2022 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* basic_test.cpp / 0-057_air                                                 */
/*----------------------------------------------------------------------------*/
//
// Basic initial test.
//
/*----------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>

#include "defs.h"
#include "compile.h"
#include "result.h"
#include "version.h"

#include "common/types.h"
#include "core/robot.h"
#include "util/geometry.h"

#include "test/basic_test.h"


using namespace std;
using namespace RR_xoft;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{




/*----------------------------------------------------------------------------*/

void print_Introduction(void)
{
    printf("----------------------------------------------------------------\n");    
    printf("%s : Basic Test Program\n", sPRODUCT);
    printf("%s\n", sCOPYRIGHT);
    printf("================================================================\n");
}


int test_basic_1(void)
{
    printf("Testing basic 1 ...\n");
    s2DRobot robot_1;
    s2DRobot::Link *link_1 = new s2DRobot::Link(2.0);
    s2DRobot::Link *link_2 = new s2DRobot::Link(1.0);
    
    link_1->to_Screen();
    link_2->to_Screen();    

    s2D origin_1(0.0, 0.0);
    s2D end_1;

//    s2D position_1(2.15, 1.95);
//    s2D position_1(2.15, 1.25);
//    s2D position_1(1.5, 1.6);
//    s2D position_1(2.0, 1.6);
//    s2D position_1(2.0, 2.0);
    s2D position_1(3.0, 3.0);            
  

    robot_1.Links.push_back(link_1);
    robot_1.Links.push_back(link_2);

    s2DRobot::Joint *joint_1 = new s2DRobot::Joint(robot_1.Links[0], 0.0);    
    s2DRobot::Joint *joint_2 = new s2DRobot::Joint(robot_1.Links[1], 0.0);
    
    robot_1.Joints.push_back(joint_1);
    robot_1.Joints.push_back(joint_2);

    robot_1.Joints[0]->next = robot_1.Joints[1];
    robot_1.Joints[1]->next = NULL;    

    robot_1.base_joint = robot_1.Joints[0];
    
    end_1.to_Screen();
    
    for (sInt_32 d = 0; d <= 180; d += 10)
    {
	robot_1.Joints[0]->rotation = sDEG_2_RAD(d);
	robot_1.calc_EndPosition(robot_1.base_joint, origin_1, end_1);	
	end_1.to_Screen();
    }

    for (sInt_32 d = 0; d <= 180; d += 10)
    {
	robot_1.Joints[1]->rotation = sDEG_2_RAD(d);
	robot_1.calc_EndPosition(robot_1.base_joint, origin_1, end_1);	
	end_1.to_Screen();
    }

    sDouble derivative = robot_1.calc_JointRotationDerivative(joint_1, origin_1, position_1);
    printf("Derivation: %.3f\n", derivative);

    for (sInt_32 d = 0; d <= 180; d += 10)
    {
	robot_1.Joints[0]->rotation = sDEG_2_RAD(d);
	robot_1.calc_EndPosition(robot_1.base_joint, origin_1, end_1);	
	end_1.to_Screen();

	sDouble derivative = robot_1.calc_JointRotationDerivative(joint_1, origin_1, position_1);
	sDouble derivative2 = robot_1.calc_JointRotation2Derivative(joint_1, origin_1, position_1);	
	printf("    derivative:%.3f  2nd-derivative:%.3f\n", derivative, derivative2);
    }

    robot_1.Joints[0]->rotation = 0.0;
    robot_1.Joints[1]->rotation = 0.0;    
    
    printf("Optimizing...\n");
    sDouble rotation_1;
    robot_1.optimize_JointRotation(joint_1, origin_1, position_1, rotation_1);
    
    printf("Optimized rotation: %.3f\n", rotation_1);

    sDouble rotation_2;
    robot_1.optimize_JointRotation(joint_2, origin_1, position_1, rotation_2);
    
    printf("Optimized rotation: %.3f\n", rotation_2);

    robot_1.optimize_RobotConfiguration(joint_1, origin_1, position_1);

    robot_1.to_Screen();
    
    printf("Testing basic 1 ... finished\n");
    
    return sRESULT_SUCCESS;
}


int test_basic_2(void)
{
    printf("Testing basic 2 ...\n");
    s2DRobot robot_1;

    robot_1.add_Joint(0.0);
    robot_1.add_Link(2.0);
    robot_1.add_Joint(0.0);
    robot_1.add_Link(1.0);
    robot_1.add_Joint(0.0);
    robot_1.add_Link(2.0);    

    s2D origin_1(0.0, 0.0);    
    s2D end_1;
    s2D position_1(3.0, 3.0);
    s2D position_2(2.5, 2.5);
      
    end_1.to_Screen();
    
    for (sInt_32 d = 0; d <= 180; d += 10)
    {
	robot_1.Joints[0]->rotation = sDEG_2_RAD(d);
	robot_1.calc_EndPosition(robot_1.base_joint, origin_1, end_1);	
	end_1.to_Screen();
    }

    for (sInt_32 d = 0; d <= 180; d += 10)
    {
	robot_1.Joints[1]->rotation = sDEG_2_RAD(d);
	robot_1.calc_EndPosition(robot_1.base_joint, origin_1, end_1);	
	end_1.to_Screen();
    }

    sDouble derivative = robot_1.calc_JointRotationDerivative(robot_1.Joints[0], origin_1, position_1);
    printf("Derivation: %.3f\n", derivative);

    for (sInt_32 d = 0; d <= 180; d += 10)
    {
	robot_1.Joints[0]->rotation = sDEG_2_RAD(d);
	robot_1.calc_EndPosition(robot_1.base_joint, origin_1, end_1);	
	end_1.to_Screen();

	sDouble derivative = robot_1.calc_JointRotationDerivative(robot_1.Joints[0], origin_1, position_1);
	sDouble derivative2 = robot_1.calc_JointRotation2Derivative(robot_1.Joints[0], origin_1, position_1);	
	printf("    derivative:%.3f  2nd-derivative:%.3f\n", derivative, derivative2);
    }

    robot_1.Joints[0]->rotation = 0.0;
    robot_1.Joints[1]->rotation = 0.0;
    robot_1.Joints[2]->rotation = 0.0;

//    robot_1.attach_Constraint(robot_1.Joints[1], s2DRobot::Constraint(M_PI / 2));
    robot_1.attach_Constraint(robot_1.Joints[2], 0.0);    
    
    printf("Optimizing...\n");
    sDouble rotation_1;
    robot_1.optimize_JointRotation(robot_1.Joints[0], origin_1, position_1, rotation_1);
    
    printf("Optimized rotation: %.3f\n", rotation_1);

    sDouble rotation_2;
    robot_1.optimize_JointRotation(robot_1.Joints[1], origin_1, position_1, rotation_2);
    
    printf("Optimized rotation: %.3f\n", rotation_2);

//    robot_1.optimize_RobotConfiguration(robot_1.Joints[0], origin_1, position_1);
    robot_1.optimize_ConstrainedRobotConfiguration(robot_1.Joints[0], origin_1, position_2);

    robot_1.to_Screen();
    
    printf("Testing basic 2 ... finished\n");
    
    return sRESULT_SUCCESS;
}


int test_basic_3(void)
{    
    printf("Testing basic 3 ...\n");

    s3D u(1.0, 0.0, 0.0);
    s3D v(0.0, 1.0, 0.0);
    s3D w;

    u.to_Screen();
    v.to_Screen();    
    s3D::calc_CrossProduct(u, v, w);

    w.to_Screen();

    printf("Testing basic 3 ... finished\n");
    
    return sRESULT_SUCCESS;
}


int test_basic_4(void)
{
    printf("Testing basic 4 ...\n");
    s3DRobot robot_1;

    robot_1.add_Joint(s3DRobot::Joint::ORIENTATION_Y, 0.0);
    robot_1.add_Link(s3D(0.0, 0.0, 10.0));
    robot_1.add_Joint(s3DRobot::Joint::ORIENTATION_Y, 0.0);
    robot_1.add_Link(s3D(-2.0, 0.0, 2.0));
    robot_1.add_Joint(s3DRobot::Joint::ORIENTATION_X, 0.0);
    robot_1.add_Link(s3D(8.0, 0.0, 0.0));
    robot_1.add_Joint(s3DRobot::Joint::ORIENTATION_Y, 0.0);
    robot_1.add_Link(s3D(0.0, 0.0, -4.0));

    s3D origin_1(0.0, 0.0, 0.0);
    s3D position_1(2.0, 0.0, 10.0);
    s3D end_1;
      
    origin_1.to_Screen();
    
    for (sInt_32 d = 0; d <= 180; d += 10)
    {
	robot_1.Joints[2]->rotation = sDEG_2_RAD(d);
	robot_1.calc_EndPosition(robot_1.base_joint, origin_1, end_1);	
	end_1.to_Screen();
    }

    robot_1.to_Screen();

    printf("Optimizing...\n");
    sDouble rotation_1;    
    robot_1.optimize_JointRotation(robot_1.Joints[1], origin_1, position_1, rotation_1);
    
    printf("Optimized rotation: %.3f\n", rotation_1);

    robot_1.optimize_RobotConfiguration(robot_1.Joints[0], origin_1, position_1);
    robot_1.to_Screen();

    sDouble yaw, pitch, roll;

    sXYZ xyz_1(s3D(1.0, 0.0, 0.0),
	       s3D(0.0, 1.0, 0.0),
	       s3D(0.0, 0.0, 1.0));
    
    xyz_1.to_Screen();
    xyz_1.rotate_AroundZ(0.1);
    xyz_1.rotate_AroundY(0.2);    
    xyz_1.rotate_AroundX(0.3);
    xyz_1.to_Screen();    
    
    xyz_1.calc_PrincipalAxes(yaw, pitch, roll);    
    
    printf("yaw:%.3f, pitch:%.3f, roll:%.3f\n", yaw, pitch, roll);

//    robot_1.attach_Constraint(robot_1.Joints[3], s3DRobot::Constraint(s3DRobot::Constraint::AXIS_PITCH, 0.0));
    robot_1.attach_Constraint(robot_1.Joints[3], s3DRobot::Constraint(s3DRobot::Constraint::AXIS_PITCH, M_PI / 2));

    robot_1.attach_Limiter(robot_1.Joints[0], s3DRobot::Limiter(-0.4 * M_PI, 0.4 * M_PI));
    robot_1.attach_Limiter(robot_1.Joints[1], s3DRobot::Limiter(-0.5 * M_PI, 0.5 * M_PI));
    robot_1.attach_Limiter(robot_1.Joints[2], s3DRobot::Limiter(-0.5 * M_PI, 0.5 * M_PI));
    robot_1.attach_Limiter(robot_1.Joints[3], s3DRobot::Limiter(-0.5 * M_PI, 0.5 * M_PI));

    robot_1.optimize_ConstrainedRobotConfiguration(robot_1.Joints[0], origin_1, position_1);
    robot_1.to_Screen();    
    
    printf("Testing basic 4 ... finished\n");
    
    return sRESULT_SUCCESS;
}
   

}  // namespace rrOST


/*----------------------------------------------------------------------------*/

int main(int sUNUSED(argc), const char **sUNUSED(argv))
{
    sResult result;

    print_Introduction();

    /*
    if (sFAILED(result = test_basic_1()))
    {
	printf("Test basic 1 failed (error:%d).\n", result);
	return result;
    }
    */
    /*
    if (sFAILED(result = test_basic_2()))
    {
	printf("Test basic 2 failed (error:%d).\n", result);
	return result;
    }
    */
    /*
    if (sFAILED(result = test_basic_3()))
    {
	printf("Test basic 3 failed (error:%d).\n", result);
	return result;
    } 
    */
    if (sFAILED(result = test_basic_4()))
    {
	printf("Test basic 4 failed (error:%d).\n", result);
	return result;
    }     
    
    return 0;
}
