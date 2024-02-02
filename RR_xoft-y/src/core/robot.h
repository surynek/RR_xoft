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
/* robot.h / 0-125_air                                                        */
/*----------------------------------------------------------------------------*/
//
// Robot (model) related data structures and functions.
//
/*----------------------------------------------------------------------------*/


#ifndef __ROBOT_H__
#define __ROBOT_H__


#include <vector>

#include "common/types.h"


using namespace std;
using namespace RR_xoft;


/*----------------------------------------------------------------------------*/

namespace RR_xoft
{



    
/*----------------------------------------------------------------------------*/
// s2D
    
    class s2D
    {
    public:
	s2D() { /* nothing */ }
	s2D(sDouble _x, sDouble _y)
	    : x(_x), y(_y)
	{
	    // nothing
	}

	void normalize(void);	
	sDouble calc_Length(void) const;	

    public:
	virtual void to_Screen(const sString &indent = "") const;
	virtual void to_Stream(FILE *fw, const sString &indent = "") const;

    public:
	sDouble x, y;
	
    };


/*----------------------------------------------------------------------------*/
// s3D
    
    class s3D
    {
    public:
	s3D() { /* nothing */ }
	s3D(sDouble _x, sDouble _y, sDouble _z)
	    : x(_x), y(_y), z(_z)
	{
	    // nothing
	}

	void normalize(void);	
	sDouble calc_Length(void) const;
	
	static s3D calc_CrossProduct(const s3D &X, const s3D &Y);
	static void calc_CrossProduct(const s3D &X, const s3D &Y, s3D &Z);

	static s3D calc_NormalizedCrossProduct(const s3D &X, const s3D &Y);
	static void calc_NormalizedCrossProduct(const s3D &X, const s3D &Y, s3D &Z);

    public:
	virtual void to_Screen(const sString &indent = "") const;
	virtual void to_Stream(FILE *fw, const sString &indent = "") const;	

    public:
	sDouble x, y, z;
    };
    

/*----------------------------------------------------------------------------*/
// sXYZ

    class sXYZ
    {
    public:
	sXYZ() { /* nothing */ }
	
	sXYZ(const s3D &_X, const s3D &_Y)
	    : X(_X)
	    , Y(_Y)	    
	{
	}
	
	sXYZ(const s3D &_X, const s3D &_Y, const s3D &_Z)
	    : X(_X)
	    , Y(_Y)
	    , Z(_Z)	    
	{
	    /* nothing */
	}

    public:
	s3D calc_XYZ(const s3D &v) const;
	void calc_XYZ(const s3D &v, s3D &w) const;
	void calc_PrincipalAxes(sDouble &yaw, sDouble &pitch, sDouble &roll) const;
	
	void rotate_AroundX(sDouble rotation);
	void rotate_AroundX(sDouble rotation, s3D &_Y, s3D &_Z) const;
	
	void rotate_AroundY(sDouble rotation);
	void rotate_AroundY(sDouble rotation, s3D &_X, s3D &_Z) const;
	
	void rotate_AroundZ(sDouble rotation);
	void rotate_AroundZ(sDouble rotation, s3D &_X, s3D &_Y) const;	

    public:
	virtual void to_Screen(const sString &indent = "") const;
	virtual void to_Stream(FILE *fw, const sString &indent = "") const;

    public:
	s3D X;
	s3D Y;
	s3D Z;
    };    
    

/*----------------------------------------------------------------------------*/
// sRobot
    
    class sRobot
    {
    public:	    	
	const sInt_32 MAX_OPTIMIZATION_ITERATIONS = s__MAX_OPTIMIZATION_ITERATIONS;
	const sInt_32 MAX_OPTIMIZATION_RESTARTS   = s__MAX_OPTIMIZATION_RESTARTS;
	
        /*----------------------------------------------------------------*/
	
    public:
	// nothing
	
    public:
	// nothing
    };


/*----------------------------------------------------------------------------*/
// s2DRobot
    
    class s2DRobot
	: public sRobot
    {
    public:		
	struct Link
	{
	    Link(sDouble length)
	        : end(length, 0.0)
	    {
		// nothing
	    }
	    
	    Link(const s2D _end)
		: end(_end)
	    {
		// nothing
	    }
	    
	    s2D end;

	public:
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;		    
	};

	struct Constraint
	{
	    enum Type
	    {
		TYPE_UNDEFINED,
		TYPE_ANGULAR
	    };

	    Constraint(sDouble _angle)
	        : type(TYPE_ANGULAR)
	        , angle(_angle)
	    {
		// nothing
	    }

	    Type type;
	    sDouble angle;
	};

	struct Joint
	{	       
	    Joint(sDouble _rotation)
	        : child(NULL)
		, rotation(_rotation)
		, constraint(NULL)
	    {
		// nothing
	    }
	    
	    Joint(Link *_child, sDouble _rotation)
	        : child(_child)
		, rotation(_rotation)
		, constraint(NULL)		
	    {
		// nothing
	    }

	    Link *child;

	    Joint *prev;
	    Joint *next;

	    sDouble rotation;
	    Constraint *constraint;
	    
	public:	    
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;		    
	};

	typedef vector<Link*> Links_vector;	
	typedef vector<Joint*> Joints_vector;

	typedef vector<sDouble> Rotations_vector;

	
	struct Configuration
	{
	    Rotations_vector Rotations;

	public:	    
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;		    	    
	};
	
    public:
	s2DRobot()
	    : base_joint(NULL)
	{
	    // nothing
	}

	virtual ~s2DRobot();
        /*----------------------------------------------------------------*/

	void add_Joint(sDouble rotation);
	void add_Link(s2D end);
	void add_Link(sDouble length);

	void attach_Constraint(Joint *joint, const Constraint &constraint);
        /*----------------------------------------------------------------*/

	void save_RobotConfiguration(Configuration &configuration) const;
	void restore_RobotConfiguration(const Configuration &configuration);
	void randomize_RobotConfiguration(void);
        /*----------------------------------------------------------------*/	

	void calc_EndPosition(const s2D &origin, s2D &end) const;
	void calc_EndPosition(const Joint *_base_joint, const s2D &origin, s2D &end) const;

	sDouble calc_PositionDifference(const s2D &origin, const s2D &position) const;
	sDouble calc_JointRotationDerivative(Joint *joint, const s2D &origin, const s2D &position);
	sDouble calc_JointRotation2Derivative(Joint *joint, const s2D &origin, const s2D &position);

	bool optimize_JointRotation(Joint *joint, const s2D &origin, const s2D &position);
	bool optimize_JointRotation(Joint *joint, const s2D &origin, const s2D &position, sDouble &rotation);

	bool optimize_JointConstraint(Joint *joint, const s2D &origin, const s2D &position);
	bool optimize_JointConstraint(Joint *joint, const s2D &origin, const s2D &position, sDouble &rotation);	
	
	bool optimize_RobotConfiguration(Joint *base_joint, const s2D &origin, const s2D &position);
	bool optimize_ConstrainedRobotConfiguration(Joint *base_joint, const s2D &origin, const s2D &position);	
        /*----------------------------------------------------------------*/
	
	virtual void to_Screen(const sString &indent = "") const;
	virtual void to_Stream(FILE *fw, const sString &indent = "") const;

    public:
	Joint *base_joint;
	
	Links_vector Links;
	Joints_vector Joints;
    };
    

/*----------------------------------------------------------------------------*/
// s3DRobot
    
    class s3DRobot
	: public sRobot
    {
    public:
	const sDouble POSITION_OPTIMIZATION_WEIGHT   = 1.0;
	const sDouble CONSTRAINT_OPTIMIZATION_WEIGHT = 1.0;
	const sDouble LIMITER_OPTIMIZATION_WEIGHT    = 8.0;
	    
    public:		
	struct Link
	{
	    Link(sDouble length)
	    : end(length, 0.0, 0.0)
	    {
		// nothing
	    }
	    
	    Link(const s3D _end)
		: end(_end)
	    {
		// nothing
	    }
	    
	    s3D end;

	public:
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;		    
	};

	struct Constraint
	{
	    enum Axis
	    {
		AXIS_UNDEFINED,
		AXIS_YAW,
		AXIS_PITCH,
		AXIS_ROLL
	    };

	    Constraint(Axis _axis, sDouble _angle)
	        : axis(_axis)
	        , angle(_angle)
	    {
		// nothing
	    }

	    Axis axis;
	    sDouble angle;

	public:	    
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;		    	    
	};

	struct Limiter
	{
	    Limiter(sDouble _rotation_low, sDouble _rotation_high)
	        : rotation_low(_rotation_low)
	        , rotation_high(_rotation_high)
	    {
		// nothing
	    }
	    
	    sDouble rotation_low;
	    sDouble rotation_high;

	public:
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;	    
	};

	typedef vector<Constraint*> Constraints_vector;
	typedef vector<Limiter*> Limiters_vector;	

	struct Joint
	{
	    enum Orientation
	    {
		ORIENTATION_UNDEFINED,
		ORIENTATION_X,
		ORIENTATION_Y,
		ORIENTATION_Z
	    };
	   
	    Joint(Orientation _orientation, sDouble _rotation)
	        : child(NULL)
		, orientation(_orientation)
		, rotation(_rotation)
	    {
		// nothing
	    }
	    
	    Joint(Link *_child, Orientation _orientation, sDouble _rotation)
	        : child(_child)
		, orientation(_orientation)
		, rotation(_rotation)
	    {
		// nothing
	    }

	    virtual ~Joint()
	    {
		for (Limiters_vector::iterator limiter = Limiters.begin(); limiter != Limiters.end(); ++limiter)
		{
		    delete *limiter;
		}
		
		for (Constraints_vector::iterator constraint = Constraints.begin(); constraint != Constraints.end(); ++constraint)
		{
		    delete *constraint;
		}
	    }

	    Link *child;

	    Joint *prev;
	    Joint *next;

	    Orientation orientation;	    
	    sDouble rotation;

	    Limiters_vector Limiters;
	    Constraints_vector Constraints;
	    
	public:	    
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;		    
	};

	typedef vector<Link*> Links_vector;	
	typedef vector<Joint*> Joints_vector;

	typedef vector<sDouble> Rotations_vector;


	struct Configuration
	{
	    Rotations_vector Rotations;

	public:	    
	    virtual void to_Screen(const sString &indent = "") const;
	    virtual void to_Stream(FILE *fw, const sString &indent = "") const;		    	    
	};

	
    public:
	s3DRobot()
	    : base_joint(NULL)
	{
	    // nothing
	}

	virtual ~s3DRobot();
        /*----------------------------------------------------------------*/

	void add_Joint(Joint::Orientation orientation, sDouble rotation);
	void add_Link(s3D end);
	void add_Link(sDouble length);

	void attach_Limiter(Joint *joint, const Limiter &limiter);		
	void attach_Constraint(Joint *joint, const Constraint &constraint);
        /*----------------------------------------------------------------*/

	void save_RobotConfiguration(Configuration &configuration) const;
	void restore_RobotConfiguration(const Configuration &configuration);
	
	void randomize_RobotConfiguration(void);
	void randomize_LimitedRobotConfiguration(void);		
        /*----------------------------------------------------------------*/	

	void calc_EndPosition(const s3D &origin, s3D &end) const;
	void calc_EndPosition(const Joint *_base_joint, const s3D &origin, s3D &end) const;

	sDouble calc_PositionDifference(const s3D &origin, const s3D &position) const;
	sDouble calc_JointRotationDerivative(Joint *joint, const s3D &origin, const s3D &position);
	sDouble calc_JointRotation2Derivative(Joint *joint, const s3D &origin, const s3D &position);

	sDouble calc_ConstraintDeviation(void) const;	
	sDouble calc_ConstraintDeviation(const Joint *joint) const;

	sDouble calc_LimiterViolation(void) const;
	sDouble calc_LimiterViolation(const Joint *joint) const;

	sDouble calc_ConstrainedPositionDifference(const s3D &origin, const s3D &position) const;
	sDouble calc_ConstrainedJointRotationDerivative(Joint *joint, const s3D &origin, const s3D &position);
	sDouble calc_ConstrainedJointRotation2Derivative(Joint *joint, const s3D &origin, const s3D &position);		

	bool optimize_JointRotation(Joint *joint, const s3D &origin, const s3D &position);
	bool optimize_JointRotation(Joint *joint, const s3D &origin, const s3D &position, sDouble &rotation);

	bool optimize_ConstrainedJointRotation(Joint *joint, const s3D &origin, const s3D &position);
	bool optimize_ConstrainedJointRotation(Joint *joint, const s3D &origin, const s3D &position, sDouble &rotation);	

	bool optimize_RobotConfiguration(Joint *base_joint, const s3D &origin, const s3D &position);
	bool optimize_ConstrainedRobotConfiguration(Joint *base_joint, const s3D &origin, const s3D &position);	
        /*----------------------------------------------------------------*/
	
	virtual void to_Screen(const sString &indent = "") const;
	virtual void to_Stream(FILE *fw, const sString &indent = "") const;

    public:
	Joint *base_joint;
	
	Links_vector Links;
	Joints_vector Joints;
    };    
    
  

/*----------------------------------------------------------------------------*/

} // namespace RR_xoft

#endif /* __ROBOT_H__ */
