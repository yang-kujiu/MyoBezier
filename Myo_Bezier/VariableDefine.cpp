#include "pch.h"
#include"VariableDefine.h"
#include <myo/myo.hpp>
#include "myo.h"
struct ROBOT m_robot;
struct TimerID m_TimerID;
struct Bezier m_bezier;
struct Bezier m_bezier2;
struct TargetPoint m_TargePoint;
double leftMeter = 0;
double rightMeter = 0;
double wheelRadius = 0.315 / 2;
double twoWheelDis = 0.585;

double leftMeter_Eight = 0;
double rightMeter_Eight = 0;

Kvaser *m_kvaser = new Kvaser();
myo::Pose g_currentPose;

std::pair<double, double>YBound(-1, 1);
std::pair<double, double>XBound(-2, 15);