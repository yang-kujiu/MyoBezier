#pragma once

#include"kvaser.h"
#include <myo/myo.hpp>
struct ROBOT
{
	double x;
	double y;
	double angle;//rad
	double theta;//бу
};
extern ROBOT m_robot;
struct Bezier
{
	double Start_Point_x;
	double Start_Point_y;
	double End_Point_x;//rad
	double End_Point_y;
	double Start_Speed;
	double End_Speed;
	double	Start_angle;
	double	End_angle;
	double Sm;
};

extern Bezier m_bezier;
extern Bezier m_bezier2;
extern double leftMeter;
extern double rightMeter;
extern double wheelRadius;
extern double twoWheelDis;

extern Kvaser *m_kvaser;

extern  double left;
extern double right;
extern double leftMeter_Eight;
extern double rightMeter_Eight;

struct TimerID
{
	int TimerID1;
	int TimerID2;
	int TimerID3;
	int TimerID4;
};
extern TimerID m_TimerID;

extern myo::Pose g_currentPose;

extern std::pair<double, double>YBound;
extern std::pair<double, double>XBound;

struct TargetPoint
{
	double x;
	double y;
	double angle;//rad
	double Speed;
};

extern TargetPoint m_TargePoint;