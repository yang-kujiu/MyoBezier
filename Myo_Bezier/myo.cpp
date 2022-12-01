#include "pch.h"
#include "myo.h"
#include <iostream>
#include <myo/myo.hpp>
#include "Myo_Bezier.h"
#include "Myo_BezierDlg.h"
#include "VariableDefine.h"
#include "Resource.h"

#include "CSeries.h"

void dealMYO::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
	//显示手势
	currentPose = pose;
	g_currentPose = pose;
	//g_currentPose = pose;
	if (pose != myo::Pose::unknown && pose != myo::Pose::rest)
	{
		// Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
		// Myo becoming locked.
		myo->unlock(myo::Myo::unlockHold);

		// Notify the Myo that the pose has resulted in an action, in this case changing
		// the text on the screen. The Myo will vibrate.
		myo->notifyUserAction();

	}
	else
	{
		// Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
		// are being performed, but lock after inactivity.
		myo->unlock(myo::Myo::unlockHold);
	}
}


void CMyoBezierDlg::myolink()
{
	if (myoConnent)
	{
		return;
	}
	lkw_myo = hub.waitForMyo(3000);
	if (!lkw_myo)
	{
		myoConnent = false;
	}
	else
	{
		myoConnent = true;
		hub.addListener(&collector);
	}
}
void CMyoBezierDlg::LoopDealPole()
{
	CString strCurGesture;
	if (myoConnent)
	{
		hub.run(1000 / 20);
		strCurGesture.Format(_T("已连接"));
		SetDlgItemText(IDC_EDIT_Myo, strCurGesture);
		if (g_currentPose == myo::Pose::fist)
		{
			strCurGesture.Format(_T("握拳，极轴减"));
			SetDlgItemText(IDC_EDIT_Myo, strCurGesture);
			//OnBnClickedButtonBack();
			//Bezier_Reduce_End_y();

		}
		else if (g_currentPose == myo::Pose::fingersSpread)
		{
			strCurGesture.Format(_T("开掌，极轴加"));
			SetDlgItemText(IDC_EDIT_Myo, strCurGesture);
			//OnBnClickedButtonForword();
			//Bezier_Add_End_y();

		}
		else if (g_currentPose == myo::Pose::waveIn)
		{
			strCurGesture.Format(_T("掌左，极角加"));
			SetDlgItemText(IDC_EDIT_Myo, strCurGesture);
			//		OnBnClickedButtonLeft();
			//Bezier_Reduce_End_x();

		}
		else if (g_currentPose == myo::Pose::waveOut)
		{
			strCurGesture.Format(_T("掌右，极角减"));
			SetDlgItemText(IDC_EDIT_Myo, strCurGesture);
			//OnBnClickedButtonRight();
			//Bezier_Add_End_x();

		}
	}
	else
	{
		strCurGesture.Format(_T("没链接"));
		//	SetDlgItemText(IDC_EDIT1, strCurGesture);

	}
}
