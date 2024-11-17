/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                             RR_xoft 0-174_air                             */
/*                                                                            */
/*                  (C) Copyright 2021 - 2024 Pavel Surynek                  */
/*                                                                            */
/*                http://www.surynek.net | <pavel@surynek.net>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* robot.cpp / 0-174_air                                                      */
/*----------------------------------------------------------------------------*/
//
// Robot (model) related data structures and functions.
//
/*----------------------------------------------------------------------------*/


#include <stdio.h>
#include <math.h>

#include "defs.h"
#include "result.h"

#include "common/types.h"
#include "core/robot.h"


using namespace RR_xoft;




/*----------------------------------------------------------------------------*/

namespace RR_xoft
{



    
/*============================================================================*/
// s2D class
/*----------------------------------------------------------------------------*/

    void s2D::normalize(void)
    {
	sDouble D = calc_Length();
	sASSERT(D >= s_EPSILON);
	    
	x /= D;
	y /= D;
    }
    

    sDouble s2D::calc_Length(void) const
    {
	return sqrt(x * x + y * y);
    }
        

/*----------------------------------------------------------------------------*/
    
    void s2D::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s2D::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%s(%.3f,%.3f)\n", indent.c_str(), x, y);
    }


    
    
/*============================================================================*/
// s3D class
/*----------------------------------------------------------------------------*/

    void s3D::normalize(void)
    {
	sDouble D = calc_Length();
	sASSERT(D >= s_EPSILON);
	    
	x /= D;
	y /= D;
	z /= D;
    }
    

    sDouble s3D::calc_Length(void) const
    {
	return sqrt(x * x + y * y + z * z);
    }


    s3D s3D::calc_CrossProduct(const s3D &X, const s3D &Y)
    {
	s3D Z;
	calc_CrossProduct(X, Y, Z);
	
	return Z;
    }

    
    void s3D::calc_CrossProduct(const s3D &X, const s3D &Y, s3D &Z)
    {
	Z.x = X.y * Y.z - X.z * Y.y;
	Z.y = X.z * Y.x - X.x * Y.z;
	Z.z = X.x * Y.y - X.y * Y.z;		
    }


    s3D s3D::calc_NormalizedCrossProduct(const s3D &X, const s3D &Y)
    {
	s3D Z;
	calc_CrossProduct(X, Y, Z);
	Z.normalize();
	
	return Z;	
    }

    
    void s3D::calc_NormalizedCrossProduct(const s3D &X, const s3D &Y, s3D &Z)
    {
	calc_CrossProduct(X, Y, Z);
	Z.normalize();
    }
        

/*----------------------------------------------------------------------------*/
    
    void s3D::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s3D::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%s(%.3f,%.3f,%.3f)\n", indent.c_str(), x, y, z);
    }


    

/*============================================================================*/
// sXYZ class
/*----------------------------------------------------------------------------*/

    s3D sXYZ::calc_XYZ(const s3D &v) const
    {
	s3D w;
	calc_XYZ(v, w);

	return w;
    }

    
    void sXYZ::calc_XYZ(const s3D &v, s3D &w) const
    {
	w.x = v.x * X.x + v.y * Y.x + v.z * Z.x;
	w.y = v.x * X.y + v.y * Y.y + v.z * Z.y;
	w.z = v.x * X.z + v.y * Y.z + v.z * Z.z;
    }


    void sXYZ::calc_PrincipalAxes(sDouble &yaw, sDouble &pitch, sDouble &roll) const
    {
	if (X.x >= s_EPSILON)
	{
	    yaw = atan(X.y / X.x);
	}
	else
	{
	    yaw = M_PI / 2;
	}
	sDouble xy_Xlength = sqrt(X.x * X.x + X.y * X.y);
	
	if (xy_Xlength >= s_EPSILON)
	{
	    pitch = atan(X.z / xy_Xlength);
	}
	else
	{
	    pitch = M_PI / 2;
	}

	sDouble ZZ;	
	sDouble cosp = cos(pitch);
	
	if (cosp >= s_EPSILON)
	{
	    ZZ = Y.z / cosp;
	}
	else
	{
	    ZZ = Y.z;
	}
	roll = asin(ZZ);
    }

    
/*----------------------------------------------------------------------------*/

    void sXYZ::rotate_AroundX(sDouble rotation)
    {
	s3D nY, nZ;
	rotate_AroundX(rotation, nY, nZ);       

	Y = nY;
	Z = nZ;		
    }

    
    void sXYZ::rotate_AroundX(sDouble rotation, s3D &_Y, s3D &_Z) const
    {
	sDouble cosr = cos(rotation);
	sDouble sinr = sin(rotation);

	_Y.x = cosr * Y.x + sinr * Z.x;
	_Y.y = cosr * Y.y + sinr * Z.y;
	_Y.z = cosr * Y.z + sinr * Z.z;	
	
	_Z.x = -sinr * Y.x + cosr * Z.x;
	_Z.y = -sinr * Y.y + cosr * Z.y;
	_Z.z = -sinr * Y.z + cosr * Z.z;
    }    


    void sXYZ::rotate_AroundY(sDouble rotation)
    {
	s3D nX, nZ;
	rotate_AroundY(rotation, nX, nZ);
	
	X = nX;
	Z = nZ;			
    }
    
    
    void sXYZ::rotate_AroundY(sDouble rotation, s3D &_X, s3D &_Z) const
    {
	sDouble cosr = cos(rotation);
	sDouble sinr = sin(rotation);

	_X.x = cosr * X.x + sinr * Z.x;
	_X.y = cosr * X.y + sinr * Z.y;
	_X.z = cosr * X.z + sinr * Z.z;	
	
	_Z.x = -sinr * X.x + cosr * Z.x;
	_Z.y = -sinr * X.y + cosr * Z.y;
	_Z.z = -sinr * X.z + cosr * Z.z;
    }

    
    void sXYZ::rotate_AroundZ(sDouble rotation)
    {
	s3D nX, nY;
	rotate_AroundZ(rotation, nX, nY);

	X = nX;
	Y = nY;
    }


    void sXYZ::rotate_AroundZ(sDouble rotation, s3D &_X, s3D &_Y) const
    {
	sDouble cosr = cos(rotation);
	sDouble sinr = sin(rotation);

	s3D nX, nY;	
	
	_X.x = cosr * X.x + sinr * Y.x;
	_X.y = cosr * X.y + sinr * Y.y;
	_X.z = cosr * X.z + sinr * Y.z;	
	
	_Y.x = -sinr * X.x + cosr * Y.x;
	_Y.y = -sinr * X.y + cosr * Y.y;
	_Y.z = -sinr * X.z + cosr * Y.z;
    }    
    

/*----------------------------------------------------------------------------*/
    
    void sXYZ::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void sXYZ::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%s[\n", indent.c_str());
	fprintf(fw, "%s%sX: (%.3f,%.3f,%.3f)\n", indent.c_str(), s_INDENT.c_str(),  X.x, X.y, X.z);
	fprintf(fw, "%s%sY: (%.3f,%.3f,%.3f)\n", indent.c_str(), s_INDENT.c_str(),  Y.x, Y.y, Y.z);
	fprintf(fw, "%s%sZ: (%.3f,%.3f,%.3f)\n", indent.c_str(), s_INDENT.c_str(),  Z.x, Z.y, Z.z);
	fprintf(fw, "%s]\n", indent.c_str());	
    }
    


/*============================================================================*/
// s2DRobot class
/*----------------------------------------------------------------------------*/

    void s2DRobot::Link::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s2DRobot::Link::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sLink: (end.x = %.3f, end.y = %.3f)\n", indent.c_str(), end.x, end.y);
    }


/*----------------------------------------------------------------------------*/
    
    void s2DRobot::Joint::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s2DRobot::Joint::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sJoint: (rotation = %.3f)\n", indent.c_str(), rotation);
    }


/*----------------------------------------------------------------------------*/
    
    void s2DRobot::Configuration::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);	
    }

    
    void s2DRobot::Configuration::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sConfiguration: (|dimension| = %ld)\n", indent.c_str(), Rotations.size());

	for (Rotations_vector::const_iterator rotation = Rotations.begin(); rotation != Rotations.end(); ++rotation)
	{
	    fprintf(fw, "%s%srotation: %.f\n", indent.c_str(), s_INDENT.c_str(), *rotation);	    
	}
    }
    

/*============================================================================*/
    
    s2DRobot::~s2DRobot()
    {
	for (Joints_vector::const_iterator joint = Joints.begin(); joint != Joints.end(); ++joint)
	{
	    delete *joint;
	}
	for (Links_vector::const_iterator link = Links.begin(); link != Links.end(); ++link)
	{
	    delete *link;
	}		
    }
    
    
/*----------------------------------------------------------------------------*/

    void s2DRobot::add_Joint(sDouble rotation)
    {
	sASSERT(Joints.size() == Links.size());

	Joint *joint = new Joint(rotation);
	Joints.push_back(joint);

	if (Joints.size() == 1)
	{
	    base_joint = Joints[0];
	    base_joint->prev = NULL;
	    base_joint->next = NULL;
	}
	else
	{
	    sInt_32 last = Joints.size() - 1;
	    Joints[last]->prev = Joints[last - 1];
	    Joints[last - 1]->next = Joints[last];
	}	
    }

    
    void s2DRobot::add_Link(s2D end)
    {
	sASSERT(!Joints.empty());
	    
	s2DRobot::Link *link = new s2DRobot::Link(end);
	Links.push_back(link);

	sInt_32 last = Joints.size() - 1;
	Joints[last]->child = link;
    }

    
    void s2DRobot::add_Link(sDouble length)
    {
	sASSERT(!Joints.empty());
	
	s2DRobot::Link *link = new s2DRobot::Link(length);
	Links.push_back(link);
	
	sInt_32 last = Joints.size() - 1;
	Joints[last]->child = link;	
    }


    void s2DRobot::attach_Constraint(Joint *joint, const Constraint &constraint)
    {
	joint->constraint = new Constraint(constraint);
    }

    
/*----------------------------------------------------------------------------*/
    
    void s2DRobot::save_RobotConfiguration(Configuration &configuration) const
    {
	sASSERT(base_joint != NULL);
	
	configuration.Rotations.clear();
	const Joint *joint = base_joint;	

	while (joint != NULL)
	{
	    configuration.Rotations.push_back(joint->rotation);
	    joint = joint->next;	    
	}	
    }

    
    void s2DRobot::restore_RobotConfiguration(const Configuration &configuration)
    {
	sASSERT(base_joint != NULL);
	
	Rotations_vector::const_iterator rotation = configuration.Rotations.begin();
	Joint *joint = base_joint;

	while (joint != NULL)
	{
	    sASSERT(rotation != configuration.Rotations.end());
	    
	    joint->rotation = *rotation++;
	    joint = joint->next;	    
	}		
    }


    void s2DRobot::randomize_RobotConfiguration(void)
    {
	Joint *joint = base_joint;

	while (joint != NULL)
	{
	    joint->rotation = 2 * M_PI * (rand() / (sDouble)RAND_MAX);
	    joint = joint->next;
	}
    }
	

/*----------------------------------------------------------------------------*/
    

    void s2DRobot::calc_EndPosition(const s2D &origin, s2D &end) const
    {
	sASSERT(base_joint != NULL);
	
	calc_EndPosition(base_joint, origin, end);
    }

    
    void s2DRobot::calc_EndPosition(const Joint *_base_joint, const s2D &origin, s2D &end) const	
    {
	end.x = origin.x;
	end.y = origin.y;

	const Joint *joint = _base_joint;
	sDouble rotation = 0.0;
	
	while (joint != NULL)
	{
	    Link *link = joint->child;
	    rotation += joint->rotation;

	    sDouble cosr = cos(rotation);
	    sDouble sinr = sin(rotation);	    
	    
	    end.x += cosr * link->end.x - sinr * link->end.y;	
	    end.y += sinr * link->end.x + cosr * link->end.y;

	    joint = joint->next;
	}
    }


    sDouble s2DRobot::calc_PositionDifference(const s2D &origin, const s2D &position) const
    {
	s2D end;
	calc_EndPosition(origin, end);

	sDouble dx = end.x - position.x;
	sDouble dy = end.y - position.y;

	return (dx * dx + dy * dy);
    }

    
    sDouble s2DRobot::calc_JointRotationDerivative(Joint *joint, const s2D &origin, const s2D &position)
    {
	sDouble rotation_save = joint->rotation;
	
	sDouble pos_diff1 = calc_PositionDifference(origin, position);
	joint->rotation += s_DELTION;
	sDouble pos_diff2 = calc_PositionDifference(origin, position);
	joint->rotation = rotation_save;

	//printf("pos diffs: %f, %f\n", pos_diff1, pos_diff2);

	return ((pos_diff2 - pos_diff1) / s_DELTION);
    }

    
    sDouble s2DRobot::calc_JointRotation2Derivative(Joint *joint, const s2D &origin, const s2D &position)
    {
	sDouble rotation_save = joint->rotation;
	
	sDouble pos_diff_derivative1 = calc_JointRotationDerivative(joint, origin, position);
	joint->rotation += s_DELTION;
	sDouble pos_diff_derivative2 = calc_JointRotationDerivative(joint, origin, position);
	joint->rotation = rotation_save;

	//printf("pos diffs 2: %f, %f\n", pos_diff_derivative1, pos_diff_derivative2);

	return ((pos_diff_derivative2 - pos_diff_derivative1) / s_DELTION);
    }


    bool s2DRobot::optimize_JointRotation(Joint *joint, const s2D &origin, const s2D &position)
    {
	sDouble drot, ddrot;
	
	for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	{
	    drot = calc_JointRotationDerivative(joint, origin, position);
	    ddrot = calc_JointRotation2Derivative(joint, origin, position);

	    printf("%.10f,%.3f,%d <-- %f\n", drot, ddrot, i, joint->rotation);
	    
	    if (sABS(drot) > s_PRECISION)
	    {
		joint->rotation = joint->rotation - (drot / ddrot);		
	    }
	    else
	    {
		break;
	    }
	}
	joint->rotation -= sSGN(joint->rotation) * 2 * M_PI * floor(joint->rotation / (2 * M_PI));	

	if (sABS(drot) <= s_PRECISION)
	{
	    s2D end;
	    calc_EndPosition(origin, end);
	    printf("Optimum found: %.3f\n", joint->rotation);
	    end.to_Screen();
	    return true;
	}
	else
	{
	    printf("Failed to find optimum (joint rot).\n");	    
	    return false;
	}	
    }

    
    bool s2DRobot::optimize_JointRotation(Joint *joint, const s2D &origin, const s2D &position, sDouble &rotation)
    {
	sDouble rotation_save = joint->rotation;
	sDouble drot, ddrot;
	
	for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	{
	    drot = calc_JointRotationDerivative(joint, origin, position);
	    ddrot = calc_JointRotation2Derivative(joint, origin, position);

	    printf("%.3f,%.3f,%d <-- %f\n", drot, ddrot, i, joint->rotation);
	    
	    if (sABS(drot) > s_PRECISION)
	    {
		joint->rotation = joint->rotation - (drot / ddrot);		
	    }
	    else
	    {
		break;
	    }
	}
	joint->rotation -= sSGN(joint->rotation) * 2 * M_PI * floor(joint->rotation / (2 * M_PI));
	rotation = joint->rotation;
	
	joint->rotation = rotation_save;

	if (sABS(drot) <= s_PRECISION)
	{
	    printf("Optimum found: %.3f\n", rotation);
	    return true;
	}
	else
	{
	    printf("Failed to find optimum.\n");
	    return false;
	}
    }


    bool s2DRobot::optimize_JointConstraint(Joint *joint, const s2D &origin, const s2D &position)
    {
	sDouble rotation;
	
	if (optimize_JointConstraint(joint, origin, position, rotation))
	{
	    joint->rotation = rotation;
	    return true;
	}
	else
	{
	    return false;
	}
    }

    
    bool s2DRobot::optimize_JointConstraint(Joint *joint, const s2D &sUNUSED(origin), const s2D &sUNUSED(position), sDouble &rotation)
    {
	switch (joint->constraint->type)
	{
	case Constraint::TYPE_ANGULAR:
	{
	    Joint *_joint = base_joint;
	    sDouble _rotation = 0.0;
	
	    while (_joint != NULL)
	    {
		if (_joint == joint)
		{
		    rotation = joint->constraint->angle - _rotation;
		    rotation -= sSGN(rotation) * 2 * M_PI * floor(rotation / (2 * M_PI));
		    printf("constraint rotation:%.3f\n", rotation);
		    break;
		}
		_rotation += _joint->rotation;	    
		_joint = _joint->next;
	    }
	    return true;
	}
	default:
	{
	    break;
	}
	}
	return false;
    }


    bool s2DRobot::optimize_RobotConfiguration(Joint *base_joint, const s2D &origin, const s2D &position)
    {
	Configuration configuration;
	save_RobotConfiguration(configuration);
	
	for (sInt_32 r = 0; r < MAX_OPTIMIZATION_RESTARTS; ++r)
	{
	    randomize_RobotConfiguration();
	
	    for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	    {
		Joint *joint = base_joint;
		
		while (joint != NULL)
		{
		    if (!optimize_JointRotation(joint, origin, position))
		    {
			restore_RobotConfiguration(configuration);
			return false;
		    }
		    joint = joint->next;
		}
		sDouble diff = calc_PositionDifference(origin, position);
		printf("Diff:%.10f\n", diff);
	    
		if (diff < s_PRECISION)
		{
		    printf("Optimal configuration found.\n");
		    return true;
		}
	    }
	}
	restore_RobotConfiguration(configuration);
	printf("Failed to optimize configuration.\n");	
	return false;
    }


    bool s2DRobot::optimize_ConstrainedRobotConfiguration(Joint *base_joint, const s2D &origin, const s2D &position)
    {
	Configuration configuration;
	save_RobotConfiguration(configuration);
	    
	for (sInt_32 r = 0; r < MAX_OPTIMIZATION_RESTARTS; ++r)
	{
	    randomize_RobotConfiguration();
		
	    for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	    {
		Joint *joint = base_joint;

		while (joint != NULL)
		{
		    if (joint->constraint != NULL)
		    {
			if (!optimize_JointConstraint(joint, origin, position))
			{
			    restore_RobotConfiguration(configuration);
			    return false;
			}		    
		    }
		    else
		    {
			if (!optimize_JointRotation(joint, origin, position))
			{
			    restore_RobotConfiguration(configuration);			    
			    return false;
			}
		    }
		    joint = joint->next;		
		}
		sDouble diff = calc_PositionDifference(origin, position);
		printf("Diff:%.10f,%.10f\n", diff, s_EPSILON);
	    
		if (diff < s_PRECISION)
		{
		    printf("Optimal configuration found.\n");
		    return true;
		}
	    }
	}
	restore_RobotConfiguration(configuration);	
	printf("Failed to optimize configuration.\n");	
	return false;
    }    


/*----------------------------------------------------------------------------*/
    
    void s2DRobot::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);	
    }

    
    void s2DRobot::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%s2DRobot: (|joints| = %ld, |links| = %ld)\n", indent.c_str(), Joints.size(), Links.size());
	
	fprintf(fw, "%s%sJoints:\n", indent.c_str(), s_INDENT.c_str());
	for (Joints_vector::const_iterator joint = Joints.begin(); joint != Joints.end(); ++joint)
	{
	    (*joint)->to_Screen(indent + s2_INDENT);
	}

	fprintf(fw, "%s%sLinks:\n", indent.c_str(), s_INDENT.c_str());
	for (Links_vector::const_iterator link = Links.begin(); link != Links.end(); ++link)
	{
	    (*link)->to_Screen(indent + s2_INDENT);
	}	
    }
    



/*============================================================================*/
// s3DRobot class
/*----------------------------------------------------------------------------*/

    void s3DRobot::Link::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s3DRobot::Link::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sLink: (end.x = %.3f, end.y = %.3f)\n", indent.c_str(), end.x, end.y);
    }


/*----------------------------------------------------------------------------*/
    
    void s3DRobot::Constraint::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s3DRobot::Constraint::to_Stream(FILE *fw, const sString &indent) const
    {
	sString axis_text;

	switch (axis)
	{
	case AXIS_UNDEFINED:
	{
	    axis_text = "\"undefined\"";
	    break;
	}
	case AXIS_YAW:
	{
	    axis_text = "\"yaw\"";	    
	    break;	    
	}
	case AXIS_PITCH:
	{
	    axis_text = "\"pitch\"";	    
	    break;	    
	}
	case AXIS_ROLL:
	{
	    axis_text = "\"roll\"";
	    break;
	}
	default:
	{
	    sASSERT(false);
	}
	}
	
	fprintf(fw, "%s(axis = %s, angle = %.3f)\n", indent.c_str(), axis_text.c_str(), angle);
    }


/*----------------------------------------------------------------------------*/
    
    void s3DRobot::Limiter::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s3DRobot::Limiter::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%s(-rot = %.3f, +rot = %.3f)\n", indent.c_str(), rotation_low, rotation_high);	
    }


/*----------------------------------------------------------------------------*/
    
    void s3DRobot::Joint::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s3DRobot::Joint::to_Stream(FILE *fw, const sString &indent) const
    {
	sString orientation_text;

	switch (orientation)
	{
	case ORIENTATION_UNDEFINED:
	{
	    orientation_text = "\"undefined\"";
	    break;
	}
	case ORIENTATION_X:
	{
	    orientation_text = "\"X axis\"";
	    break;
	}
	case ORIENTATION_Y:
	{
	    orientation_text = "\"Y axis\"";	    
	    break;
	}
	case ORIENTATION_Z:
	{
	    orientation_text = "\"Z axis\"";
	    break;
	}
	default:
	{
	    break;
	}
	}
	fprintf(fw, "%sJoint: (orientation = %s, rotation = %.3f)\n", indent.c_str(), orientation_text.c_str(), rotation);

	if (!Limiters.empty())
	{
	    fprintf(fw, "%s%sLimiters\n", indent.c_str(), s_INDENT.c_str());
	    for (Limiters_vector::const_iterator limiter = Limiters.begin(); limiter != Limiters.end(); ++limiter)
	    {
		(*limiter)->to_Stream(fw, indent + s2_INDENT);
	    }
	}
	if (!Constraints.empty())
	{
	    fprintf(fw, "%s%sConstraints\n", indent.c_str(), s_INDENT.c_str());
	    for (Constraints_vector::const_iterator constraint = Constraints.begin(); constraint != Constraints.end(); ++constraint)
	    {
		(*constraint)->to_Stream(fw, indent + s2_INDENT);
	    }
	}
    }


/*----------------------------------------------------------------------------*/

    void s3DRobot::Configuration::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);	
    }

    
    void s3DRobot::Configuration::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sConfiguration: (|dimension| = %ld)\n", indent.c_str(), Rotations.size());

	for (Rotations_vector::const_iterator rotation = Rotations.begin(); rotation != Rotations.end(); ++rotation)
	{
	    fprintf(fw, "%s%srotation: %.f\n", indent.c_str(), s_INDENT.c_str(), *rotation);	    
	}
    }


/*============================================================================*/
    
    s3DRobot::~s3DRobot()
    {
	for (Joints_vector::const_iterator joint = Joints.begin(); joint != Joints.end(); ++joint)
	{
	    delete *joint;
	}
	for (Links_vector::const_iterator link = Links.begin(); link != Links.end(); ++link)
	{
	    delete *link;
	}		
    }
    
    
/*----------------------------------------------------------------------------*/

    void s3DRobot::add_Joint(Joint::Orientation orientation, sDouble rotation)
    {
	sASSERT(Joints.size() == Links.size());

	Joint *joint = new Joint(orientation, rotation);
	Joints.push_back(joint);

	if (Joints.size() == 1)
	{
	    base_joint = Joints[0];
	    base_joint->prev = NULL;
	    base_joint->next = NULL;
	}
	else
	{
	    sInt_32 last = Joints.size() - 1;
	    Joints[last]->prev = Joints[last - 1];
	    Joints[last - 1]->next = Joints[last];
	}	
    }

    
    void s3DRobot::add_Link(s3D end)
    {
	sASSERT(!Joints.empty());
	    
	s3DRobot::Link *link = new s3DRobot::Link(end);
	Links.push_back(link);

	sInt_32 last = Joints.size() - 1;
	Joints[last]->child = link;
    }

    
    void s3DRobot::add_Link(sDouble length)
    {
	sASSERT(!Joints.empty());
	
	s3DRobot::Link *link = new s3DRobot::Link(length);
	Links.push_back(link);
	
	sInt_32 last = Joints.size() - 1;
	Joints[last]->child = link;	
    }


    void s3DRobot::attach_Limiter(Joint *joint, const Limiter &limiter)
    {
	joint->Limiters.push_back(new Limiter(limiter));
    }

    
    void s3DRobot::attach_Constraint(Joint *joint, const Constraint &constraint)
    {
	joint->Constraints.push_back(new Constraint(constraint));
    }

    
/*----------------------------------------------------------------------------*/
    
    void s3DRobot::save_RobotConfiguration(Configuration &configuration) const
    {
	sASSERT(base_joint != NULL);
	
	configuration.Rotations.clear();
	const Joint *joint = base_joint;	

	while (joint != NULL)
	{
	    configuration.Rotations.push_back(joint->rotation);
	    joint = joint->next;	    
	}	
    }

    
    void s3DRobot::restore_RobotConfiguration(const Configuration &configuration)
    {
	sASSERT(base_joint != NULL);
	
	Rotations_vector::const_iterator rotation = configuration.Rotations.begin();
	Joint *joint = base_joint;

	while (joint != NULL)
	{
	    sASSERT(rotation != configuration.Rotations.end());
	    
	    joint->rotation = *rotation++;
	    joint = joint->next;	    
	}		
    }


    void s3DRobot::randomize_RobotConfiguration(void)
    {
	Joint *joint = base_joint;

	while (joint != NULL)
	{
	    joint->rotation = 2 * M_PI * (rand() / (sDouble)RAND_MAX);
	    joint = joint->next;
	}
    }


    void s3DRobot::randomize_LimitedRobotConfiguration(void)
    {	
	Joint *joint = base_joint;

	while (joint != NULL)
	{
	    if (!joint->Limiters.empty())
	    {
		sASSERT(joint->Limiters.size() == 1);

		joint->rotation = joint->Limiters[0]->rotation_low + (joint->Limiters[0]->rotation_high - joint->Limiters[0]->rotation_low) *  (rand() / (sDouble)RAND_MAX);		
	    }
	    else
	    {
		joint->rotation = 2 * M_PI * (rand() / (sDouble)RAND_MAX);
	    }
	    joint = joint->next;
	}
    }    
	

/*----------------------------------------------------------------------------*/
    

    void s3DRobot::calc_EndPosition(const s3D &origin, s3D &end) const
    {
	sASSERT(base_joint != NULL);
	
	calc_EndPosition(base_joint, origin, end);
    }

    
    void s3DRobot::calc_EndPosition(const Joint *_base_joint, const s3D &origin, s3D &end) const	
    {
	end.x = origin.x;
	end.y = origin.y;
	end.z = origin.z;

	const Joint *joint = _base_joint;
	sXYZ xyz = sXYZ(s3D(1.0, 0.0, 0.0),
			s3D(0.0, 1.0, 0.0),
			s3D(0.0, 0.0, 1.0));
	
	while (joint != NULL)
	{
	    switch (joint->orientation)
	    {
	    case Joint::ORIENTATION_X:
	    {
		xyz.rotate_AroundX(joint->rotation);
		break;
	    }
	    case Joint::ORIENTATION_Y:
	    {
		xyz.rotate_AroundY(joint->rotation);		
		break;
	    }
	    case Joint::ORIENTATION_Z:
	    {
		xyz.rotate_AroundZ(joint->rotation);		
		break;
	    }
	    default:
	    {
		sASSERT(false);
		break;
	    }
	    }
	    Link *link = joint->child;
	    
	    s3D end_xyz;
	    xyz.calc_XYZ(link->end, end_xyz);

	    end.x += end_xyz.x;
	    end.y += end_xyz.y;
	    end.z += end_xyz.z;	    
	   
	    joint = joint->next;
	}
    }


    sDouble s3DRobot::calc_PositionDifference(const s3D &origin, const s3D &position) const
    {
	s3D end;
	calc_EndPosition(origin, end);

	sDouble dx = end.x - position.x;
	sDouble dy = end.y - position.y;
	sDouble dz = end.z - position.z;	

	return (dx * dx + dy * dy + dz * dz);
    }

    
    sDouble s3DRobot::calc_JointRotationDerivative(Joint *joint, const s3D &origin, const s3D &position)
    {
	sDouble rotation_save = joint->rotation;
	
	sDouble pos_diff1 = calc_PositionDifference(origin, position);
	joint->rotation += s_DELTION;
	sDouble pos_diff2 = calc_PositionDifference(origin, position);
	joint->rotation = rotation_save;

	//printf("pos diffs: %f, %f\n", pos_diff1, pos_diff2);

	return ((pos_diff2 - pos_diff1) / s_DELTION);
    }

    
    sDouble s3DRobot::calc_JointRotation2Derivative(Joint *joint, const s3D &origin, const s3D &position)
    {
	sDouble rotation_save = joint->rotation;
	
	sDouble pos_diff_derivative1 = calc_JointRotationDerivative(joint, origin, position);
	joint->rotation += s_DELTION;
	sDouble pos_diff_derivative2 = calc_JointRotationDerivative(joint, origin, position);
	joint->rotation = rotation_save;

	//printf("pos diffs 2: %f, %f\n", pos_diff_derivative1, pos_diff_derivative2);

	return ((pos_diff_derivative2 - pos_diff_derivative1) / s_DELTION);
    }


    bool s3DRobot::optimize_JointRotation(Joint *joint, const s3D &origin, const s3D &position)
    {
	sDouble drot, ddrot;
	
	for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	{
	    drot = calc_JointRotationDerivative(joint, origin, position);
	    ddrot = calc_JointRotation2Derivative(joint, origin, position);

	    printf("%.10f,%.3f,%d <-- %f\n", drot, ddrot, i, joint->rotation);
	    
	    if (sABS(drot) > s_PRECISION)
	    {
		joint->rotation = joint->rotation - (drot / ddrot);		
	    }
	    else
	    {
		break;
	    }
	}
	joint->rotation -= sSGN(joint->rotation) * 2 * M_PI * floor(joint->rotation / (2 * M_PI));	

	if (sABS(drot) <= s_PRECISION)
	{
	    s3D end;
	    calc_EndPosition(origin, end);
	    printf("Optimum found: %.3f\n", joint->rotation);
	    end.to_Screen();
	    return true;
	}
	else
	{
	    printf("Failed to find optimum (joint rot).\n");	    
	    return false;
	}	
    }


    bool s3DRobot::optimize_JointRotation(Joint *joint, const s3D &origin, const s3D &position, sDouble &rotation)
    {
	sDouble rotation_save = joint->rotation;
	sDouble drot, ddrot;
	
	for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	{
	    drot = calc_JointRotationDerivative(joint, origin, position);
	    ddrot = calc_JointRotation2Derivative(joint, origin, position);

	    printf("%.3f,%.3f,%d <-- %f\n", drot, ddrot, i, joint->rotation);
	    
	    if (sABS(drot) > s_PRECISION)
	    {
		joint->rotation = joint->rotation - (drot / ddrot);		
	    }
	    else
	    {
		break;
	    }
	}
	joint->rotation -= sSGN(joint->rotation) * 2 * M_PI * floor(joint->rotation / (2 * M_PI));
	rotation = joint->rotation;
	
	joint->rotation = rotation_save;

	if (sABS(drot) <= s_PRECISION)
	{
	    printf("Optimum found: %.3f\n", rotation);
	    return true;
	}
	else
	{
	    printf("Failed to find optimum.\n");
	    return false;
	}
    }
    

    sDouble s3DRobot::calc_ConstraintDeviation(void) const
    {
	sASSERT(base_joint != NULL);	
	return calc_ConstraintDeviation(base_joint);
    }

    
    sDouble s3DRobot::calc_ConstraintDeviation(const Joint *joint) const
    {
	sDouble deviation = 0.0;
	
	sXYZ xyz = sXYZ(s3D(1.0, 0.0, 0.0),
			s3D(0.0, 1.0, 0.0),
			s3D(0.0, 0.0, 1.0));
	
	while (joint != NULL)
	{
	    switch (joint->orientation)
	    {
	    case Joint::ORIENTATION_X:
	    {
		xyz.rotate_AroundX(joint->rotation);
		break;
	    }
	    case Joint::ORIENTATION_Y:
	    {
		xyz.rotate_AroundY(joint->rotation);		
		break;
	    }
	    case Joint::ORIENTATION_Z:
	    {
		xyz.rotate_AroundZ(joint->rotation);		
		break;
	    }
	    default:
	    {
		sASSERT(false);
		break;
	    }
	    }
	    if (!joint->Constraints.empty())
	    {
		sDouble yaw, pitch, roll;	    
		xyz.calc_PrincipalAxes(yaw, pitch, roll);

		for (Constraints_vector::const_iterator constraint = joint->Constraints.begin(); constraint != joint->Constraints.end(); ++constraint)
		{
		    sDouble diff;
		    
		    switch ((*constraint)->axis)
		    {
		    case Constraint::AXIS_YAW:
		    {
			diff = sABS((*constraint)->angle - yaw);
			break;
		    }
		    case Constraint::AXIS_PITCH:
		    {
			diff = sABS((*constraint)->angle - pitch);
			break;			
		    }
		    case Constraint::AXIS_ROLL:
		    {
			diff = sABS((*constraint)->angle - roll);
			break;			
		    }
		    default:
		    {
			sASSERT(false);
			break;
		    }
		    }
		    deviation += diff * diff;
		}
	    }
	    
	    joint = joint->next;
	}

	return deviation;
    }

    
    sDouble s3DRobot::calc_LimiterViolation(void) const
    {
	sASSERT(base_joint != NULL);
	
	return calc_LimiterViolation(base_joint);
    }

    
    sDouble s3DRobot::calc_LimiterViolation(const Joint *joint) const
    {
	sDouble violation = 0.0;
	
	while (joint != NULL)
	{	
	    for (Limiters_vector::const_iterator limiter = joint->Limiters.begin(); limiter != joint->Limiters.end(); ++limiter)
	    {
		if (joint->rotation > (*limiter)->rotation_high)
		{
		    violation += exp(sABS(joint->rotation - (*limiter)->rotation_high));
		}
		else
		{
		    if (joint->rotation < (*limiter)->rotation_low)
		    {
			violation += exp(sABS(joint->rotation - (*limiter)->rotation_low));
		    }
		}
	    }
	    joint = joint->next;
	}
	return violation;
    }


    sDouble s3DRobot::calc_ConstrainedPositionDifference(const s3D &origin, const s3D &position) const
    {
	sDouble pos_diff = calc_PositionDifference(origin, position);
	sDouble deviation = calc_ConstraintDeviation();
	sDouble violation =  calc_LimiterViolation();

	return (  POSITION_OPTIMIZATION_WEIGHT * pos_diff
		+ CONSTRAINT_OPTIMIZATION_WEIGHT * deviation
		+ LIMITER_OPTIMIZATION_WEIGHT * violation);
    }

    
    sDouble s3DRobot::calc_ConstrainedJointRotationDerivative(Joint *joint, const s3D &origin, const s3D &position)
    {
	sDouble rotation_save = joint->rotation;
	
	sDouble pos_diff1 = calc_ConstrainedPositionDifference(origin, position);
	joint->rotation += s_DELTION;
	sDouble pos_diff2 = calc_ConstrainedPositionDifference(origin, position);
	joint->rotation = rotation_save;

	//printf("pos diffs: %f, %f\n", pos_diff1, pos_diff2);

	return ((pos_diff2 - pos_diff1) / s_DELTION);
    }

    
    sDouble s3DRobot::calc_ConstrainedJointRotation2Derivative(Joint *joint, const s3D &origin, const s3D &position)
    {
	sDouble rotation_save = joint->rotation;
	
	sDouble pos_diff_derivative1 = calc_ConstrainedJointRotationDerivative(joint, origin, position);
	joint->rotation += s_DELTION;
	sDouble pos_diff_derivative2 = calc_ConstrainedJointRotationDerivative(joint, origin, position);
	joint->rotation = rotation_save;

	//printf("pos diffs 2: %f, %f\n", pos_diff_derivative1, pos_diff_derivative2);

	return ((pos_diff_derivative2 - pos_diff_derivative1) / s_DELTION);
    }


    bool s3DRobot::optimize_ConstrainedJointRotation(Joint *joint, const s3D &origin, const s3D &position)
    {
	sDouble drot, ddrot;
	
	for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	{
	    drot = calc_ConstrainedJointRotationDerivative(joint, origin, position);
	    ddrot = calc_ConstrainedJointRotation2Derivative(joint, origin, position);

	    printf("%.10f,%.3f,%d <-- %f\n", drot, ddrot, i, joint->rotation);
	    
	    if (sABS(drot) > s_PRECISION)
	    {
		joint->rotation = joint->rotation - (drot / ddrot);		
	    }
	    else
	    {
		break;
	    }
	}
	//joint->rotation -= sSGN(joint->rotation) * 2 * M_PI * floor(joint->rotation / (2 * M_PI));	

	if (sABS(drot) <= s_PRECISION)
	{
	    s3D end;
	    calc_EndPosition(origin, end);
	    printf("Optimum found: %.3f\n", joint->rotation);
	    end.to_Screen();
	    return true;
	}
	else
	{
	    printf("Failed to find optimum (joint rot).\n");	    
	    return false;
	}	
    }


    bool s3DRobot::optimize_ConstrainedJointRotation(Joint *joint, const s3D &origin, const s3D &position, sDouble &rotation)
    {
	sDouble rotation_save = joint->rotation;
	sDouble drot, ddrot;
	
	for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	{
	    drot = calc_ConstrainedJointRotationDerivative(joint, origin, position);
	    ddrot = calc_ConstrainedJointRotation2Derivative(joint, origin, position);

	    printf("%.3f,%.3f,%d <-- %f\n", drot, ddrot, i, joint->rotation);
	    
	    if (sABS(drot) > s_PRECISION)
	    {
		joint->rotation = joint->rotation - (drot / ddrot);		
	    }
	    else
	    {
		break;
	    }
	}
	//joint->rotation -= sSGN(joint->rotation) * 2 * M_PI * floor(joint->rotation / (2 * M_PI));
	rotation = joint->rotation;
	
	joint->rotation = rotation_save;

	if (sABS(drot) <= s_PRECISION)
	{
	    printf("Optimum found: %.3f\n", rotation);
	    return true;
	}
	else
	{
	    printf("Failed to find optimum.\n");
	    return false;
	}
    }    
    

    bool s3DRobot::optimize_RobotConfiguration(Joint *base_joint, const s3D &origin, const s3D &position)
    {
	Configuration configuration;
	save_RobotConfiguration(configuration);
	
	for (sInt_32 r = 0; r < MAX_OPTIMIZATION_RESTARTS; ++r)
	{
	    randomize_RobotConfiguration();
	
	    for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	    {
		Joint *joint = base_joint;
		
		while (joint != NULL)
		{
		    if (!optimize_JointRotation(joint, origin, position))
		    {
			restore_RobotConfiguration(configuration);
			return false;
		    }
		    joint = joint->next;
		}
		sDouble diff = calc_PositionDifference(origin, position);
		printf("Diff:%.10f\n", diff);
	    
		if (diff < s_PRECISION)
		{
		    s3D final_end;
		    calc_EndPosition(base_joint, origin, final_end);
		    printf("Final end position:\n");
		    final_end.to_Screen();
		    printf("Optimal configuration found.\n");
		    return true;
		}
	    }
	}
	restore_RobotConfiguration(configuration);
	printf("Failed to optimize configuration.\n");	
	return false;
    }


    bool s3DRobot::optimize_ConstrainedRobotConfiguration(Joint *base_joint, const s3D &origin, const s3D &position)
    {
	Configuration configuration;
	save_RobotConfiguration(configuration);
	
	for (sInt_32 r = 0; r < MAX_OPTIMIZATION_RESTARTS; ++r)
	{
	    randomize_LimitedRobotConfiguration();
	
	    for (sInt_32 i = 0; i < MAX_OPTIMIZATION_ITERATIONS; ++i)
	    {
		Joint *joint = base_joint;
		
		while (joint != NULL)
		{
		    if (!optimize_ConstrainedJointRotation(joint, origin, position))
		    {
			restore_RobotConfiguration(configuration);
			return false;
		    }
		    joint = joint->next;
		}
		sDouble diff = calc_PositionDifference(origin, position);
		sDouble devi = calc_ConstraintDeviation();
		printf("Diff:%.10f, devi:%.10f\n", diff, devi);
	    
		if (diff < s_PRECISION)
		{
		    s3D final_end;
		    calc_EndPosition(base_joint, origin, final_end);
		    printf("Final end position:\n");
		    final_end.to_Screen();		    
		    printf("Optimal configuration found.\n");
		    return true;
		}
	    }
	}
	restore_RobotConfiguration(configuration);
	printf("Failed to optimize configuration.\n");	
	return false;
    }    


/*----------------------------------------------------------------------------*/
    
    void s3DRobot::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);	
    }

    
    void s3DRobot::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%s3DRobot: (|joints| = %ld, |links| = %ld)\n", indent.c_str(), Joints.size(), Links.size());
	
	fprintf(fw, "%s%sJoints:\n", indent.c_str(), s_INDENT.c_str());
	for (Joints_vector::const_iterator joint = Joints.begin(); joint != Joints.end(); ++joint)
	{
	    (*joint)->to_Screen(indent + s2_INDENT);
	}

	fprintf(fw, "%s%sLinks:\n", indent.c_str(), s_INDENT.c_str());
	for (Links_vector::const_iterator link = Links.begin(); link != Links.end(); ++link)
	{
	    (*link)->to_Screen(indent + s2_INDENT);
	}	
    }

    
/*----------------------------------------------------------------------------*/

} // namespace RR_xoft
