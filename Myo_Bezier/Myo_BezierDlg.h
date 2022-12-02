
// Myo_BezierDlg.h: 头文件
//

#pragma once
#include "VariableDefine.h"
#include <myo/myo.hpp>
#include "myo.h"
#include "COCX.h"
// CMyoBezierDlg 对话框
class CMyoBezierDlg : public CDialogEx
{
// 构造
public:
	CMyoBezierDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYO_BEZIER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonForword();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonEnd();
	void InitRobot();
	void InitKvaser();
	bool isInitKvaser;
	void myolink();
	void LoopDealPole();
	bool myoConnent;
	int isStartMYO;

	myo::Hub hub;

	myo::Myo* lkw_myo;
	dealMYO collector;
	afx_msg void OnEnChangeEditMyo();

public:
	double Time_Length_Bezier;
	double Time_Add_Bezier;
	double Time_Add;
	double m_Xe;
	double m_Ye;
	double m_Thetae;
	double k1;
	double k2;
	double k3;
	double Vc;
	double Wc;

	double m_V_bezier;
	double m_W_bezier;
	double Dx_bezier;
	double Dy_bezier;
	double DDx_bezier;
	double DDy_bezier;
	double V_Bezier;
	double W_Bezier;
	double VL_Bezier;
	double VR_Bezier;
	double theta;
	double x_bezier;
	double y_bezier;
	afx_msg void OnBnClickedButtonLinkmyo();
	void Bezier_init();
	void Bezier_Add_End_y();
	void Bezier_Reduce_End_y();
	void Bezier_Add_End_x();
	void Bezier_Reduce_End_x();
	bool Bezier_Solve();
	void UpdataBezier2();
	bool Bezier_Solve2();
	double m_Xout[6];
	double m_Yout[6];
	double m_X2out[6];
	double m_Y2out[6];
	LARGE_INTEGER m_nBegin_Bezier;
	LARGE_INTEGER m_nFreq_Bezier;

	afx_msg void OnBnClickedButtonGoBezier();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double Time_Calculate_Bezier();
	void PredictPosition(double t1);
	void draw_Bezier();
	void Get_VandW1(double t1);
	void Get_VandW2(double t1);
	void Set_PWM_Bezier(long can_id, long speed);
	void text();
	COCX m_CTchart_Bezier_path;
	COCX m_CTChart_Timer_interpurt_Bezier;
	COCX m_CTChart_V_bezier;
	COCX m_CTChart_W_bezier;
	COCX m_CTChart_VL_bezier;
	COCX m_CTChart_VR_bezier;
	COCX m_CTchart_error_Xe;
	COCX m_CTchart_error_Ye;
	COCX m_CTchart_error_V;
	COCX m_CTchart_error_w;
	COCX m_CTchart_error_theta;
	double Sm_multiple;
};
