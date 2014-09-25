/*
 * RobotinoMotionServer.h
 *
 *  Created on: 13.12.2011
 *      Author: indorewala@servicerobotics.eu
 */

#ifndef ROBOTINOMOTIONSERVER_H_
#define ROBOTINOMOTIONSERVER_H_

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point32.h>

#include "robotino_motion/MotionAction.h"
#include "robotino_motion/MotionActionGoal.h"

#define PI 3.14159
#define VEL 0
#define VEL_LIN 0.2 
#define VEL_ANG 0.5
#define sign(a) (((a) < 0) ? -1 : (((a) > 0) ? 1 : 0))

typedef actionlib::SimpleActionServer<robotino_motion::MotionAction> Server;
typedef enum { IDLE,
		MOVING,
		FINISHED } State;
typedef enum { TRANSLATIONAL,
		ROTATIONAL,
		TRANSLATIONAL_ROTATIONAL,
		TANGENT } MovementType;
typedef enum { ALIGN, 
		MOVE, 
		COUNT, 
		FOLLOW } TaskType;
typedef enum { MOVED_DISTANCE, 
		HIGH_OPTICAL_SIGNAL, 
		LOW_OPTICAL_SIGNAL, 
		HIGH_INDUCTIVE_SIGNAL, 
		LOW_INDUCTIVE_SIGNAL, 
		CAMERA, 
		OBSTACLE,
		BUMPER,
		TIME,
		SECURED_INFRARED_SIGNAL } InterruptionCondition;
typedef enum { NONE, 
		INFRARED, 
		OPTICAL, 
		INDUCTIVE, 
		CAMERAAL, 
		ULTRASONIC,
		COMPASS } AlignmentDevice;

class RobotinoMotionServer
{
public:
	RobotinoMotionServer();
	~RobotinoMotionServer();

private:
	ros::NodeHandle nh_;

	ros::Subscriber odometry_sub_;

	ros::Publisher cmd_vel_pub_;

	Server server_;

	State state_;
	MovementType movement_type_;
	TaskType task_type_;
	InterruptionCondition interruption_condition_;
	AlignmentDevice alignment_device_;

	robotino_motion::MotionResult result_;
	robotino_motion::MotionFeedback feedback_;

	geometry_msgs::Twist cmd_vel_msg_;
	nav_msgs::Odometry current_odom_msg_;
	nav_msgs::Odometry start_odom_msgs_;

	double curr_x_, curr_y_, curr_phi_, prev_phi_;
	double dist_moved_x_, dist_moved_y_, dist_rotated_;
	double forward_goal_x_, forward_goal_y_, rotation_goal_;
	double start_x_, start_y_, start_phi_;

//	std::vector<geometry_msgs::Point32> forward_vel_vector_;
//	std::vector<geometry_msgs::Point32> rotation_vel_vector_;

	double min_linear_vel_;
	double max_linear_vel_;
	double linear_acc_;
	double min_angular_vel_;
	double max_angular_vel_;
	double angular_acc_;
	double percentage_;

	bool odom_set_;

	void odomCallback( const nav_msgs::OdometryConstPtr& msg );
	void teleopActivatedCallback( const std_msgs::BoolConstPtr& msg );
	void execute( const robotino_motion::MotionGoalConstPtr& goal );
	void setCmdVel( double vx, double vy, double omega );
	void controlLoop();
	bool acceptNewGoal( const robotino_motion::MotionGoalConstPtr& goal );
	void readParameters( ros::NodeHandle& n);
	template< typename InputIterator > double linearApproximator(
			InputIterator iter, InputIterator end, const double x );

public:
	void spin();
};

#endif /* ROBOTINOMOTIONSERVER_H_ */