
// Myo_BezierDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Myo_Bezier.h"
#include "Myo_BezierDlg.h"
#include "afxdialogex.h"
#include "VariableDefine.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Mode_Simulation true 
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyoBezierDlg 对话框



CMyoBezierDlg::CMyoBezierDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYO_BEZIER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isStartMYO = 0;
	myoConnent = 0;
	Time_Add_Bezier = 0;
	theta = 0;
	x_bezier = 0;
	y_bezier = 0;
	m_Xe = 0.0;
	m_Ye = 0;
	m_Thetae = 0;
	Time_Add = 0;
	m_TargePoint.angle = 0;
	m_TargePoint.x = 8;
	m_TargePoint.y = 0.5;
	Sm_multiple = 8;
}

void CMyoBezierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_CTchart_Bezier_path);
	DDX_Control(pDX, IDC_TCHART2, m_CTChart_Timer_interpurt_Bezier);
	DDX_Control(pDX, IDC_TCHART3, m_CTChart_V_bezier);
	DDX_Control(pDX, IDC_TCHART4, m_CTChart_W_bezier);
	DDX_Control(pDX, IDC_TCHART5, m_CTChart_VL_bezier);
	DDX_Control(pDX, IDC_TCHART6, m_CTChart_VR_bezier);
	DDX_Control(pDX, IDC_TCHART7, m_CTchart_error_Xe);
	DDX_Control(pDX, IDC_TCHART8, m_CTchart_error_Ye);
	DDX_Control(pDX, IDC_TCHART9, m_CTchart_error_V);
	DDX_Control(pDX, IDC_TCHART10, m_CTchart_error_w);
	DDX_Control(pDX, IDC_TCHART11, m_CTchart_error_theta);
}

BEGIN_MESSAGE_MAP(CMyoBezierDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FORWORD, &CMyoBezierDlg::OnBnClickedButtonForword)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMyoBezierDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CMyoBezierDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CMyoBezierDlg::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CMyoBezierDlg::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CMyoBezierDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_END, &CMyoBezierDlg::OnBnClickedButtonEnd)
	ON_EN_CHANGE(IDC_EDIT_Myo, &CMyoBezierDlg::OnEnChangeEditMyo)
	ON_BN_CLICKED(IDC_BUTTON_LinkMyo, &CMyoBezierDlg::OnBnClickedButtonLinkmyo)
	ON_BN_CLICKED(IDC_BUTTON_Go_Bezier, &CMyoBezierDlg::OnBnClickedButtonGoBezier)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMyoBezierDlg 消息处理程序

BOOL CMyoBezierDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyoBezierDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyoBezierDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyoBezierDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyoBezierDlg::OnBnClickedButtonForword()
{
	// TODO: 在此添加控件通知处理程序代码

	m_kvaser->speedMode(1, 5000);
	m_kvaser->speedMode(2, 5000);

}


void CMyoBezierDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_kvaser->speedMode(1, 0);
	m_kvaser->speedMode(2, 0);
}


void CMyoBezierDlg::OnBnClickedButtonBack()
{
	// TODO: 在此添加控件通知处理程序代码
	m_kvaser->speedMode(1, -5000);
	m_kvaser->speedMode(2, -5000);
}


void CMyoBezierDlg::OnBnClickedButtonRight()
{
	// TODO: 在此添加控件通知处理程序代码
	m_kvaser->speedMode(1, 5000);
	m_kvaser->speedMode(2, -5000);
}


void CMyoBezierDlg::OnBnClickedButtonLeft()
{
	// TODO: 在此添加控件通知处理程序代码  


	  
	m_kvaser->speedMode(1, -5000);
	m_kvaser->speedMode(2, 5000);
}


void CMyoBezierDlg::OnBnClickedButtonInit()
{
	InitRobot();//初始化机器人
	InitKvaser();//初始化Kvaser
	// TODO: 在此添加控件通知处理程序代码
}


void CMyoBezierDlg::OnBnClickedButtonEnd()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!isInitKvaser)
	{
		return;
	}
	isInitKvaser = false;

	m_kvaser->motorDisable(1);//关闭电机
	m_kvaser->motorDisable(2);
	m_kvaser->canRelease();//释放电机


	delete m_kvaser;
}
void CMyoBezierDlg::InitRobot()
{
	m_robot.x = 0.0;
	m_robot.y = 0.0;
	m_robot.angle = 0;
	m_robot.theta = 0;
}
//初始化电机//
void CMyoBezierDlg::InitKvaser()
{
	if (isInitKvaser)
	{
		return;
	}
	isInitKvaser = true;

	m_kvaser->canInit(0);//初始化电机

	m_kvaser->connectMotor(1);//连接电机
	m_kvaser->connectMotor(2);

	m_kvaser->motorEnable(1);//使能电机
	m_kvaser->motorEnable(2);

	m_kvaser->modeChoose(1, m_kvaser->SPEED_MODE);//速度模式
	m_kvaser->modeChoose(2, m_kvaser->SPEED_MODE);
}

void CMyoBezierDlg::OnEnChangeEditMyo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMyoBezierDlg::OnBnClickedButtonLinkmyo()
{
	// TODO: 在此添加控件通知处理程序代码
	Bezier_init();
	LoopDealPole();
	Bezier_Solve();
	UpdataBezier2();
	Bezier_Solve2();
	if (isStartMYO)
	{
		return;
	}
	isStartMYO = true;
	myolink();
}
void CMyoBezierDlg::Bezier_init()
{

	m_bezier.End_angle = 0;
	m_bezier.End_Point_x = 4;
	m_bezier.End_Point_y = 0.3;
	m_bezier.End_Speed = 0.1;

	m_bezier.Start_angle = 0;
	m_bezier.Start_Point_x = 0;
	m_bezier.Start_Point_y = 0;
	m_bezier.Start_Speed = 0.1;
	m_bezier.Sm = 30;
	m_robot.angle = m_bezier.Start_angle;

	m_bezier2.End_angle = m_TargePoint.angle;
	m_bezier2.End_Point_x = m_TargePoint.x;
	m_bezier2.End_Point_y = m_TargePoint.y;
	m_bezier2.End_Speed = m_TargePoint.Speed;

	//CSeries cys1 = (CSeries)m_CTChart_bezier.Series(1);
	//cys1.AddXY(m_bezier.Start_Point_x, m_bezier.Start_Point_y, NULL, RGB(255, 125, 125));
}


bool CMyoBezierDlg::Bezier_Solve()
{

	double sm1 = min(abs((YBound.second - m_bezier.Start_Point_y) / (m_bezier.Start_Speed*sin(m_bezier.Start_angle))),
		abs((YBound.second - m_bezier.Start_Point_y) / (m_bezier.Start_Speed*sin(m_bezier.End_angle))));
	double sm2 = min(abs((YBound.second + m_bezier.Start_Point_y) / (m_bezier.Start_Speed*sin(m_bezier.Start_angle))),
		abs((YBound.second + m_bezier.Start_Point_y) / (m_bezier.Start_Speed*sin(m_bezier.End_angle))));
	m_bezier.Sm = Sm_multiple * min(sm1, sm2);


	if (m_bezier.Sm > sqrt((m_bezier.End_Point_x - m_bezier.Start_Point_x)*(m_bezier.End_Point_x - m_bezier.Start_Point_x) +
		(m_bezier.End_Point_y - m_bezier.Start_Point_y)*(m_bezier.End_Point_y - m_bezier.Start_Point_y)) * Sm_multiple)
	{
		m_bezier.Sm = sqrt((m_bezier.Start_Point_x - m_bezier.End_Point_x)*(m_bezier.Start_Point_x - m_bezier.End_Point_x) +
			(m_bezier.Start_Point_y - m_bezier.End_Point_y)*(m_bezier.Start_Point_y - m_bezier.End_Point_y)) * Sm_multiple;
	}

	Eigen::SparseMatrix<double> hessian;

	hessian.resize(6, 6);

	hessian.insert(0, 0) = 14400;
	hessian.insert(1, 0) = -50400;
	hessian.insert(2, 0) = 57600;
	hessian.insert(3, 0) = -14400;
	hessian.insert(4, 0) = -14400;
	hessian.insert(5, 0) = 7200;

	hessian.insert(0, 1) = -50400;
	hessian.insert(1, 1) = 187200;
	hessian.insert(2, 1) = -244800;
	hessian.insert(3, 1) = 115200;
	hessian.insert(4, 1) = 7200;
	hessian.insert(5, 1) = -14400;


	hessian.insert(0, 2) = 57600;
	hessian.insert(1, 2) = -244800;
	hessian.insert(2, 2) = 403200;
	hessian.insert(3, 2) = -316800;
	hessian.insert(4, 2) = 115200;
	hessian.insert(5, 2) = -14400;

	hessian.insert(0, 3) = -14400;
	hessian.insert(1, 3) = 115200;
	hessian.insert(2, 3) = -316800;
	hessian.insert(3, 3) = 403200;
	hessian.insert(4, 3) = -244800;
	hessian.insert(5, 3) = 57600;

	hessian.insert(0, 4) = -14400;
	hessian.insert(1, 4) = 7200;
	hessian.insert(2, 4) = 115200;
	hessian.insert(3, 4) = -244800;
	hessian.insert(4, 4) = 187200;
	hessian.insert(5, 4) = -50400;

	hessian.insert(0, 5) = 7200;
	hessian.insert(1, 5) = -14400;
	hessian.insert(2, 5) = -14400;
	hessian.insert(3, 5) = 57600;
	hessian.insert(4, 5) = -50400;
	hessian.insert(5, 5) = 14400;

	Eigen::VectorXd gradient;
	gradient.resize(6);
	gradient << 0, 0, 0, 0, 0, 0;

	Eigen::SparseMatrix<double> linearMatrix;
	linearMatrix.resize(6, 6);
	linearMatrix.insert(0, 0) = 1;
	linearMatrix.insert(1, 0) = 0;
	linearMatrix.insert(2, 0) = -5;
	linearMatrix.insert(3, 0) = 0;
	linearMatrix.insert(4, 0) = 0;
	linearMatrix.insert(5, 0) = 0;


	linearMatrix.insert(0, 1) = 0;
	linearMatrix.insert(1, 1) = 0;
	linearMatrix.insert(2, 1) = 5;
	linearMatrix.insert(3, 1) = 0;
	linearMatrix.insert(4, 1) = 0;
	linearMatrix.insert(5, 1) = 0;



	linearMatrix.insert(0, 2) = 0;
	linearMatrix.insert(1, 2) = 0;
	linearMatrix.insert(2, 2) = 0;
	linearMatrix.insert(3, 2) = 0;
	linearMatrix.insert(4, 2) = 1;
	linearMatrix.insert(5, 2) = 0;


	linearMatrix.insert(0, 3) = 0;
	linearMatrix.insert(1, 3) = 0;
	linearMatrix.insert(2, 3) = 0;
	linearMatrix.insert(3, 3) = 0;
	linearMatrix.insert(4, 3) = 0;
	linearMatrix.insert(5, 3) = 1;



	linearMatrix.insert(0, 4) = 0;
	linearMatrix.insert(1, 4) = 0;
	linearMatrix.insert(2, 4) = 0;
	linearMatrix.insert(3, 4) = -5;
	linearMatrix.insert(4, 4) = 0;
	linearMatrix.insert(5, 4) = 0;



	linearMatrix.insert(0, 5) = 0;
	linearMatrix.insert(1, 5) = 1;
	linearMatrix.insert(2, 5) = 0;
	linearMatrix.insert(3, 5) = 5;
	linearMatrix.insert(4, 5) = 0;
	linearMatrix.insert(5, 5) = 0;

	Eigen::VectorXd APX_lowerBound;
	Eigen::VectorXd APX_upperBound;
	Eigen::VectorXd APY_lowerBound;
	Eigen::VectorXd APY_upperBound;
	APX_lowerBound.resize(6);
	APX_upperBound.resize(6);
	APY_lowerBound.resize(6);
	APY_upperBound.resize(6);
	APX_lowerBound << m_bezier.Start_Point_x / m_bezier.Sm, m_bezier.End_Point_x / m_bezier.Sm, m_bezier.Start_Speed*cos(m_bezier.Start_angle), m_bezier.End_Speed*cos(m_bezier.End_angle),
		XBound.first / m_bezier.Sm, XBound.first / m_bezier.Sm;
	APX_upperBound << m_bezier.Start_Point_x / m_bezier.Sm, m_bezier.End_Point_x / m_bezier.Sm, m_bezier.Start_Speed*cos(m_bezier.Start_angle), m_bezier.End_Speed*cos(m_bezier.End_angle),
		XBound.second / m_bezier.Sm, XBound.second / m_bezier.Sm;


	APY_lowerBound << m_bezier.Start_Point_y / m_bezier.Sm, m_bezier.End_Point_y / m_bezier.Sm, m_bezier.Start_Speed*sin(m_bezier.Start_angle), m_bezier.End_Speed*sin(m_bezier.End_angle),
		YBound.first / m_bezier.Sm, YBound.first / m_bezier.Sm;
	APY_upperBound << m_bezier.Start_Point_y / m_bezier.Sm, m_bezier.End_Point_y / m_bezier.Sm, m_bezier.Start_Speed*sin(m_bezier.Start_angle), m_bezier.End_Speed*sin(m_bezier.End_angle),
		YBound.second / m_bezier.Sm, YBound.second / m_bezier.Sm;



	int NumberOfVariables = 6; //A矩阵的列数
	int NumberOfConstraints = 6; //A矩阵的行数
	OsqpEigen::Solver AP_solver;
	AP_solver.settings()->setWarmStart(true);

	AP_solver.data()->setNumberOfVariables(NumberOfVariables); //设置A矩阵的列数，即n
	AP_solver.data()->setNumberOfConstraints(NumberOfConstraints); //设置A矩阵的行数，即m
	if (!AP_solver.data()->setHessianMatrix(hessian)) return 1;//设置P矩阵
	if (!AP_solver.data()->setGradient(gradient)) return 1; //设置q or f矩阵。当没有时设置为全0向量
	if (!AP_solver.data()->setLinearConstraintsMatrix(linearMatrix)) return 1;//设置线性约束的A矩阵
	if (!AP_solver.data()->setLowerBound(APX_lowerBound)) return 1;//设置下边界
	if (!AP_solver.data()->setUpperBound(APX_upperBound)) return 1;//设置上边界
	// instantiate the solver
	if (!AP_solver.initSolver()) return 1;			//初始化QP求解器

	if (AP_solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) return 1;	//判断是否有解，无解则返回1

	Eigen::VectorXd QPSolution_APX = AP_solver.getSolution();			//得到优化的值

	if (!AP_solver.updateBounds(APY_lowerBound, APY_upperBound)) return 1;	//检测是否更新完成
	if (AP_solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) return 1;	//判断是否有解，无解则返回1
	Eigen::VectorXd QPSolution_APY = AP_solver.getSolution(); 
	int t3 = 0;

	for (t3 = 0; t3 < 6; t3++)
	{
		m_Xout[t3] = QPSolution_APX[t3];
		m_Yout[t3] = QPSolution_APY[t3];
		TRACE("X%d=%lf\n", t3, m_Xout[t3]*m_bezier.Sm);
		TRACE("Y%d=%lf\n", t3, m_Yout[t3] * m_bezier.Sm);
	}

	return 0;
}

void CMyoBezierDlg::UpdataBezier2()
{

	m_bezier2.Start_Point_x = m_bezier.End_Point_x;
	m_bezier2.Start_Point_y = m_bezier.End_Point_y;
	m_bezier2.Start_angle = m_bezier.End_angle;
	m_bezier2.Start_Speed = m_bezier.End_Speed;
}

bool CMyoBezierDlg::Bezier_Solve2()
{

	double sm1 = min(abs((YBound.second - m_bezier2.Start_Point_y) / (m_bezier2.Start_Speed*sin(m_bezier2.Start_angle))),
		abs((YBound.second - m_bezier2.Start_Point_y) / (m_bezier2.Start_Speed*sin(m_bezier2.End_angle))));
	double sm2 = min(abs((YBound.second + m_bezier2.Start_Point_y) / (m_bezier2.Start_Speed*sin(m_bezier2.Start_angle))),
		abs((YBound.second + m_bezier2.Start_Point_y) / (m_bezier2.Start_Speed*sin(m_bezier2.End_angle))));
	m_bezier2.Sm = Sm_multiple * min(sm1, sm2);


	if (m_bezier2.Sm > sqrt((m_bezier2.End_Point_x - m_bezier2.Start_Point_x)*(m_bezier2.End_Point_x - m_bezier2.Start_Point_x) +
		(m_bezier2.End_Point_y - m_bezier2.Start_Point_y)*(m_bezier2.End_Point_y - m_bezier2.Start_Point_y)) * Sm_multiple)
	{
		m_bezier2.Sm = sqrt((m_bezier2.Start_Point_x - m_bezier2.End_Point_x)*(m_bezier2.Start_Point_x - m_bezier2.End_Point_x) +
			(m_bezier2.Start_Point_y - m_bezier2.End_Point_y)*(m_bezier2.Start_Point_y - m_bezier2.End_Point_y)) * Sm_multiple;
	}

	Eigen::SparseMatrix<double> hessian;

	hessian.resize(6, 6);

	hessian.insert(0, 0) = 14400;
	hessian.insert(1, 0) = -50400;
	hessian.insert(2, 0) = 57600;
	hessian.insert(3, 0) = -14400;
	hessian.insert(4, 0) = -14400;
	hessian.insert(5, 0) = 7200;

	hessian.insert(0, 1) = -50400;
	hessian.insert(1, 1) = 187200;
	hessian.insert(2, 1) = -244800;
	hessian.insert(3, 1) = 115200;
	hessian.insert(4, 1) = 7200;
	hessian.insert(5, 1) = -14400;


	hessian.insert(0, 2) = 57600;
	hessian.insert(1, 2) = -244800;
	hessian.insert(2, 2) = 403200;
	hessian.insert(3, 2) = -316800;
	hessian.insert(4, 2) = 115200;
	hessian.insert(5, 2) = -14400;

	hessian.insert(0, 3) = -14400;
	hessian.insert(1, 3) = 115200;
	hessian.insert(2, 3) = -316800;
	hessian.insert(3, 3) = 403200;
	hessian.insert(4, 3) = -244800;
	hessian.insert(5, 3) = 57600;

	hessian.insert(0, 4) = -14400;
	hessian.insert(1, 4) = 7200;
	hessian.insert(2, 4) = 115200;
	hessian.insert(3, 4) = -244800;
	hessian.insert(4, 4) = 187200;
	hessian.insert(5, 4) = -50400;

	hessian.insert(0, 5) = 7200;
	hessian.insert(1, 5) = -14400;
	hessian.insert(2, 5) = -14400;
	hessian.insert(3, 5) = 57600;
	hessian.insert(4, 5) = -50400;
	hessian.insert(5, 5) = 14400;

	Eigen::VectorXd gradient;
	gradient.resize(6);
	gradient << 0, 0, 0, 0, 0, 0;

	Eigen::SparseMatrix<double> linearMatrix;
	linearMatrix.resize(6, 6);
	linearMatrix.insert(0, 0) = 1;
	linearMatrix.insert(1, 0) = 0;
	linearMatrix.insert(2, 0) = -5;
	linearMatrix.insert(3, 0) = 0;
	linearMatrix.insert(4, 0) = 0;
	linearMatrix.insert(5, 0) = 0;


	linearMatrix.insert(0, 1) = 0;
	linearMatrix.insert(1, 1) = 0;
	linearMatrix.insert(2, 1) = 5;
	linearMatrix.insert(3, 1) = 0;
	linearMatrix.insert(4, 1) = 0;
	linearMatrix.insert(5, 1) = 0;



	linearMatrix.insert(0, 2) = 0;
	linearMatrix.insert(1, 2) = 0;
	linearMatrix.insert(2, 2) = 0;
	linearMatrix.insert(3, 2) = 0;
	linearMatrix.insert(4, 2) = 1;
	linearMatrix.insert(5, 2) = 0;


	linearMatrix.insert(0, 3) = 0;
	linearMatrix.insert(1, 3) = 0;
	linearMatrix.insert(2, 3) = 0;
	linearMatrix.insert(3, 3) = 0;
	linearMatrix.insert(4, 3) = 0;
	linearMatrix.insert(5, 3) = 1;



	linearMatrix.insert(0, 4) = 0;
	linearMatrix.insert(1, 4) = 0;
	linearMatrix.insert(2, 4) = 0;
	linearMatrix.insert(3, 4) = -5;
	linearMatrix.insert(4, 4) = 0;
	linearMatrix.insert(5, 4) = 0;



	linearMatrix.insert(0, 5) = 0;
	linearMatrix.insert(1, 5) = 1;
	linearMatrix.insert(2, 5) = 0;
	linearMatrix.insert(3, 5) = 5;
	linearMatrix.insert(4, 5) = 0;
	linearMatrix.insert(5, 5) = 0;

	Eigen::VectorXd APX_lowerBound;
	Eigen::VectorXd APX_upperBound;
	Eigen::VectorXd APY_lowerBound;
	Eigen::VectorXd APY_upperBound;
	APX_lowerBound.resize(6);
	APX_upperBound.resize(6);
	APY_lowerBound.resize(6);
	APY_upperBound.resize(6);
	APX_lowerBound << m_bezier2.Start_Point_x / m_bezier2.Sm, m_bezier2.End_Point_x / m_bezier2.Sm, m_bezier2.Start_Speed*cos(m_bezier2.Start_angle), m_bezier2.End_Speed*cos(m_bezier2.End_angle),
		XBound.first / m_bezier2.Sm, XBound.first / m_bezier2.Sm;
	APX_upperBound << m_bezier2.Start_Point_x / m_bezier2.Sm, m_bezier2.End_Point_x / m_bezier2.Sm, m_bezier2.Start_Speed*cos(m_bezier2.Start_angle), m_bezier2.End_Speed*cos(m_bezier2.End_angle),
		XBound.second / m_bezier2.Sm, XBound.second / m_bezier2.Sm;


	APY_lowerBound << m_bezier2.Start_Point_y / m_bezier2.Sm, m_bezier2.End_Point_y / m_bezier2.Sm, m_bezier2.Start_Speed*sin(m_bezier2.Start_angle), m_bezier2.End_Speed*sin(m_bezier2.End_angle),
		YBound.first / m_bezier2.Sm, YBound.first / m_bezier2.Sm;
	APY_upperBound << m_bezier2.Start_Point_y / m_bezier2.Sm, m_bezier2.End_Point_y / m_bezier2.Sm, m_bezier2.Start_Speed*sin(m_bezier2.Start_angle), m_bezier2.End_Speed*sin(m_bezier2.End_angle),
		YBound.second / m_bezier2.Sm, YBound.second / m_bezier2.Sm;



	int NumberOfVariables = 6; //A矩阵的列数
	int NumberOfConstraints = 6; //A矩阵的行数
	OsqpEigen::Solver AP_solver;
	AP_solver.settings()->setWarmStart(true);

	AP_solver.data()->setNumberOfVariables(NumberOfVariables); //设置A矩阵的列数，即n
	AP_solver.data()->setNumberOfConstraints(NumberOfConstraints); //设置A矩阵的行数，即m
	if (!AP_solver.data()->setHessianMatrix(hessian)) return 1;//设置P矩阵
	if (!AP_solver.data()->setGradient(gradient)) return 1; //设置q or f矩阵。当没有时设置为全0向量
	if (!AP_solver.data()->setLinearConstraintsMatrix(linearMatrix)) return 1;//设置线性约束的A矩阵
	if (!AP_solver.data()->setLowerBound(APX_lowerBound)) return 1;//设置下边界
	if (!AP_solver.data()->setUpperBound(APX_upperBound)) return 1;//设置上边界
	// instantiate the solver
	if (!AP_solver.initSolver()) return 1;			//初始化QP求解器

	if (AP_solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) return 1;	//判断是否有解，无解则返回1

	Eigen::VectorXd QPSolution_APX = AP_solver.getSolution();			//得到优化的值

	if (!AP_solver.updateBounds(APY_lowerBound, APY_upperBound)) return 1;	//检测是否更新完成
	if (AP_solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) return 1;	//判断是否有解，无解则返回1
	Eigen::VectorXd QPSolution_APY = AP_solver.getSolution();
	int t3 = 0;

	for (t3 = 0; t3 < 6; t3++)
	{
		m_X2out[t3] = QPSolution_APX[t3];
		m_Y2out[t3] = QPSolution_APY[t3];
		TRACE("X%d=%lf\n", t3, m_X2out[t3] * m_bezier2.Sm);
		TRACE("Y%d=%lf\n", t3, m_Y2out[t3] * m_bezier2.Sm);
	}

	return 0;
}




void CMyoBezierDlg::OnBnClickedButtonGoBezier()
{
	// TODO: 在此添加控件通知处理程序代码
	//Sleep(5000);
	QueryPerformanceCounter(&m_nBegin_Bezier);
	QueryPerformanceFrequency(&m_nFreq_Bezier);
	SetTimer(1, 100, NULL);
}


void CMyoBezierDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
		case 1:
			double	t1;
			t1 =Time_Calculate_Bezier();
			if (t1<m_bezier.Sm)
			{
				Get_VandW1(t1);
			}
			else
			{
				Get_VandW2(t1-m_bezier.Sm);
			}

			draw_Bezier();
			if (!Mode_Simulation)
			{
				PredictPosition(Time_Length_Bezier);
			}
			LoopDealPole();
			Bezier_Solve();
			UpdataBezier2();
			Bezier_Solve2();
			text();
			break;
		case 2:
			break;
		default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


double CMyoBezierDlg::Time_Calculate_Bezier()
{
	LARGE_INTEGER nTime;
	QueryPerformanceCounter(&nTime);
	QueryPerformanceFrequency(&m_nFreq_Bezier);
	Time_Length_Bezier = (nTime.QuadPart - m_nBegin_Bezier.QuadPart) / (double)m_nFreq_Bezier.QuadPart * 1000;
	m_nBegin_Bezier.QuadPart = nTime.QuadPart;
	if (Time_Length_Bezier > 125)
	{
		Time_Length_Bezier = 100;
	}

	Time_Add_Bezier += Time_Length_Bezier / 1000;
	Time_Add += Time_Length_Bezier / 1000;
	return	Time_Add_Bezier;

}


void CMyoBezierDlg::PredictPosition(double t1)
{

	leftMeter_Eight = m_kvaser->getVelocity(1, 1024, 32)*wheelRadius;//一个周期内记录的轮子线位移
	rightMeter_Eight = m_kvaser->getVelocity(2, 1024, 32)*wheelRadius;//0.12*0.1*0.1=0.0012

	TRACE("leftMeter=%.4f,rightMeter=%.4f,t=%.2f\n", leftMeter_Eight * 1000 / Time_Length_Bezier, rightMeter_Eight * 1000 / Time_Length_Bezier, Time_Length_Bezier);
	double mtheta = (rightMeter_Eight - leftMeter_Eight) / twoWheelDis;
	if (t1 > 150)
	{
		t1 = 0;
	}


	////初始位置室内坐标系x轴正半轴为起始，向左()为正，向右为负。-180度到180度

	m_robot.angle += (t1 / 1000 * mtheta);
	if (m_robot.angle < -PI)
	{
		m_robot.angle += 2 * PI;
	}
	if (m_robot.angle >= PI)
	{
		m_robot.angle -= 2 * PI;
	}
	m_robot.x += t1 / 1000 * ((leftMeter_Eight + rightMeter_Eight) / 2) * cos(m_robot.angle);
	m_robot.y += t1 / 1000 * ((leftMeter_Eight + rightMeter_Eight) / 2) * sin(m_robot.angle);



	m_robot.theta = m_robot.angle * 180 / PI;


	//	((CSeries)m_CTChart_bezier.Series(3)).Clear();

	//	CSeries cys_path_Robot = (CSeries)m_CTChart_bezier.Series(1);
	//	cys_path_Robot.AddXY(m_robot.x, m_robot.y, NULL, RGB(255, 0, 0));

	//	CSeries cys_path_Robot_Point = (CSeries)m_CTChart_bezier.Series(3);
	//	cys_path_Robot_Point.AddXY(m_robot.x, m_robot.y, NULL, RGB(0, 255, 0));

		//	FILE *fp = fopen("control_vc14\\control_vc14\\x64\\map.txt", "a+");
			//fprintf(fp, "%.4lf  %.4lf  %.4lf  %.4lf %.4lf\n", leftMeter, rightMeter, m_robot.x, m_robot.y, t1);
			//fclose(fp);

}
void CMyoBezierDlg::draw_Bezier()
{
	double t1 = 0, t2;
	CSeries cys0 = (CSeries)m_CTchart_Bezier_path.Series(0);
	CSeries cys_robot_path = (CSeries)m_CTchart_Bezier_path.Series(1);
	CSeries cys_end = (CSeries)m_CTchart_Bezier_path.Series(2);
	CSeries cys_robot = (CSeries)m_CTchart_Bezier_path.Series(3);
	CSeries cys_control = (CSeries)m_CTchart_Bezier_path.Series(4);
	CSeries cys_controlline = (CSeries)m_CTchart_Bezier_path.Series(5);
	cys0.Clear();
	cys_end.Clear();
	cys_robot.Clear();
	cys_control.Clear();
	cys_controlline.Clear();

	for (t1 = 0; t1 < (m_bezier2.Sm*2); t1++)
	{
		t2 = 1 - t1 / m_bezier2.Sm / 2;
		cys0.AddXY(m_bezier2.Sm*(m_X2out[0] * pow(1 - t2, 5) + 5 * m_X2out[1] * pow(1 - t2, 4)* t2 + 10 * m_X2out[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_X2out[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_X2out[4] * pow(1 - t2, 1)*pow(t2, 4) + m_X2out[5] * pow(t2, 5)), m_bezier2.Sm*(m_Y2out[0] * pow(1 - t2, 5) + 5 * m_Y2out[1] * pow(1 - t2, 4)* t2 + 10 * m_Y2out[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_Y2out[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_Y2out[4] * pow(1 - t2, 1)*pow(t2, 4) + m_Y2out[5] * pow(t2, 5)), NULL, RGB(0, 0, 250));
	}

	for (t1=0; t1 < (m_bezier.Sm*2 ); t1++)
	{
		t2 = 1 - t1 / m_bezier.Sm/2;
		cys0.AddXY(m_bezier.Sm*(m_Xout[0] * pow(1 - t2, 5) + 5 * m_Xout[1] * pow(1 - t2, 4)* t2 + 10 * m_Xout[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_Xout[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_Xout[4] * pow(1 - t2, 1)*pow(t2, 4) + m_Xout[5] * pow(t2, 5)), m_bezier.Sm*(m_Yout[0] * pow(1 - t2, 5) + 5 * m_Yout[1] * pow(1 - t2, 4)* t2 + 10 * m_Yout[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_Yout[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_Yout[4] * pow(1 - t2, 1)*pow(t2, 4) + m_Yout[5] * pow(t2, 5)), NULL, RGB(0, 0, 250));
	}

	for (size_t i = 0; i < 6; i++)
	{
		cys_controlline.AddXY(m_bezier.Sm*m_Xout[i], m_bezier.Sm*m_Yout[i],NULL,NULL);
		cys_control.AddXY(m_bezier.Sm*m_Xout[i], m_bezier.Sm*m_Yout[i], NULL, NULL);
	}
	for (size_t i = 0; i < 6; i++)
	{
		cys_controlline.AddXY(m_bezier2.Sm*m_X2out[i], m_bezier2.Sm*m_Y2out[i], NULL, NULL);
		cys_control.AddXY(m_bezier2.Sm*m_X2out[i], m_bezier2.Sm*m_Y2out[i], NULL, NULL);
	}
//
//
	cys_end.AddXY(m_bezier.Sm*m_Xout[5], m_bezier.Sm*m_Yout[5],NULL, RGB(125, 125, 125));
////
	cys_robot.AddXY(m_robot.x, m_robot.y, NULL, RGB(255, 255, 0));
	cys_robot_path.AddXY( m_robot.x, m_robot.y, NULL, RGB(255, 0, 0));


	CSeries myc_Time_intreeupted = (CSeries)m_CTChart_Timer_interpurt_Bezier.Series(0);
	myc_Time_intreeupted.AddXY(Time_Add, Time_Length_Bezier, NULL, RGB(255, 0, 0));

	CSeries cys_bezier_V = (CSeries)m_CTChart_V_bezier.Series(0);
	CSeries cys_control_V = (CSeries)m_CTChart_V_bezier.Series(1);
	cys_bezier_V.AddXY(Time_Add, V_Bezier, NULL, RGB(255, 0, 0));
	cys_control_V.AddXY(Time_Add, Vc, NULL, RGB(0, 0, 255));

	CSeries cys_bezier_W = (CSeries)m_CTChart_W_bezier.Series(0);
	CSeries cys_control_W = (CSeries)m_CTChart_W_bezier.Series(1);
	cys_bezier_W.AddXY(Time_Add, W_Bezier, NULL, RGB(255, 0, 0));
	cys_control_W.AddXY(Time_Add, Wc, NULL, RGB(0, 0, 255));
	


	VR_Bezier = (2 * V_Bezier + twoWheelDis * W_Bezier) / 2 * 32 * 4096 / 2 / PI / wheelRadius;

	CSeries cys_control_VLPlan = (CSeries)m_CTChart_VL_bezier.Series(0);
	cys_control_VLPlan.AddXY(Time_Add, (2 * V_Bezier - twoWheelDis * W_Bezier) / 2 , NULL, RGB(255, 255, 0));

	CSeries cys_control_VLControl = (CSeries)m_CTChart_VL_bezier.Series(1);
	cys_control_VLControl.AddXY(Time_Add, VL_Bezier / 32 / 4096 * 2 * PI * wheelRadius, NULL, RGB(255, 0, 0));

	if (!Mode_Simulation)
	{
		CSeries cys_Robot_VLRobot = (CSeries)m_CTChart_VL_bezier.Series(2);
		cys_Robot_VLRobot.AddXY(Time_Add, leftMeter_Eight, NULL, RGB(0, 0, 255));
		CSeries cys_Robot_VLPR = (CSeries)m_CTChart_VL_bezier.Series(3);
		cys_Robot_VLPR.AddXY(Time_Add, (2 * V_Bezier - twoWheelDis * W_Bezier) / 2 -leftMeter_Eight, NULL, RGB(0, 255, 255));

		CSeries cys_Robot_VRRobot = (CSeries)m_CTChart_VR_bezier.Series(2);
		cys_Robot_VRRobot.AddXY(Time_Add, rightMeter_Eight, NULL, RGB(0, 0, 255));
		CSeries cys_Robot_VRPR = (CSeries)m_CTChart_VR_bezier.Series(3);
		cys_Robot_VRPR.AddXY(Time_Add, (2 * V_Bezier + twoWheelDis * W_Bezier) / 2-rightMeter_Eight, NULL, RGB(0, 0, 255));
	}


	CSeries cys_control_VRPlan = (CSeries)m_CTChart_VR_bezier.Series(0);
	cys_control_VRPlan.AddXY(Time_Add, (2 * V_Bezier + twoWheelDis * W_Bezier) / 2, NULL, RGB(255, 255, 0));
	CSeries cys_control_VRControl = (CSeries)m_CTChart_VR_bezier.Series(1);
	cys_control_VRControl.AddXY(Time_Add, VR_Bezier / 32 / 4096 * 2 * PI * wheelRadius, NULL, RGB(255, 0, 0));
	
	CSeries cys_bezier_error_Xe = (CSeries)m_CTchart_error_Xe.Series(0);
	CSeries cys_bezier_error_Ye = (CSeries)m_CTchart_error_Ye.Series(0);
	CSeries cys_bezier_error_thetae = (CSeries)m_CTchart_error_theta.Series(0);
	cys_bezier_error_Xe.AddXY(Time_Add, m_Xe, NULL, RGB(255, 0, 0));
	cys_bezier_error_Ye.AddXY(Time_Add, m_Ye, NULL, RGB(255, 0, 0));
	cys_bezier_error_thetae.AddXY(Time_Add, m_Thetae, NULL, RGB(255, 0, 0));
	
}

void CMyoBezierDlg::Get_VandW1(double t1)
{
	double t2 = t1 / m_bezier.Sm;


	Dx_bezier = (-5 * m_Xout[0] + 5 * m_Xout[1]) * pow(1 - t2, 4) + (-20 * m_Xout[1] + 20 * m_Xout[2])* pow(1 - t2, 3)* pow(t2, 1) + (-30 * m_Xout[2] + 30 * m_Xout[3])* pow(1 - t2, 2)* pow(t2, 2) + (-20 * m_Xout[3] + 20 * m_Xout[4])* pow(1 - t2, 1)* pow(t2, 3) + (-5 * m_Xout[4] + 5 * m_Xout[5])* pow(t2, 4);
	Dy_bezier = (-5 * m_Yout[0] + 5 * m_Yout[1]) * pow(1 - t2, 4) + (-20 * m_Yout[1] + 20 * m_Yout[2])* pow(1 - t2, 3)* pow(t2, 1) + (-30 * m_Yout[2] + 30 * m_Yout[3])* pow(1 - t2, 2)* pow(t2, 2) + (-20 * m_Yout[3] + 20 * m_Yout[4])* pow(1 - t2, 1)* pow(t2, 3) + (-5 * m_Yout[4] + 5 * m_Yout[5])* pow(t2, 4);
	DDx_bezier = (20 * m_Xout[0] - 40 * m_Xout[1] + 20 * m_Xout[2])* pow(1 - t2, 3) + (60 * m_Xout[1] - 120 * m_Xout[2] + 60 * m_Xout[3])* pow(1 - t2, 2)*pow(t2, 1) + (60 * m_Xout[2] - 120 * m_Xout[3] + 60 * m_Xout[4])* pow(1 - t2, 1)*pow(t2, 2) + (20 * m_Xout[3] - 40 * m_Xout[4] + 20 * m_Xout[5])*pow(t2, 3);
	DDy_bezier = (20 * m_Yout[0] - 40 * m_Yout[1] + 20 * m_Yout[2])* pow(1 - t2, 3) + (60 * m_Yout[1] - 120 * m_Yout[2] + 60 * m_Yout[3])* pow(1 - t2, 2)*pow(t2, 1) + (60 * m_Yout[2] - 120 * m_Yout[3] + 60 * m_Yout[4])* pow(1 - t2, 1)*pow(t2, 2) + (20 * m_Yout[3] - 40 * m_Yout[4] + 20 * m_Yout[5])*pow(t2, 3);

	V_Bezier = sqrt(pow(Dx_bezier, 2) + pow(Dy_bezier, 2));
	W_Bezier = (DDy_bezier / m_bezier.Sm*Dx_bezier - DDx_bezier / m_bezier.Sm * Dy_bezier) / Dx_bezier / Dx_bezier / (1 + Dy_bezier * Dy_bezier / Dx_bezier / Dx_bezier);

	if (t1 > m_bezier.Sm)
	{
		V_Bezier = 0;
		W_Bezier = 0;
	}


	theta = atan2l(Dy_bezier, Dx_bezier);

	//theta += W_Bezier * Time_Length_Bezier / 1000;


	if (theta < -PI)
	{
		theta += 2 * PI;
	}
	if (theta >= PI)
	{
		theta -= 2 * PI;
	}

	x_bezier = m_bezier.Sm*(m_Xout[0] * pow(1 - t2, 5) + 5 * m_Xout[1] * pow(1 - t2, 4)* t2 + 10 * m_Xout[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_Xout[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_Xout[4] * pow(1 - t2, 1)*pow(t2, 4) + m_Xout[5] * pow(t2, 5));
	y_bezier = m_bezier.Sm*(m_Yout[0] * pow(1 - t2, 5) + 5 * m_Yout[1] * pow(1 - t2, 4)* t2 + 10 * m_Yout[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_Yout[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_Yout[4] * pow(1 - t2, 1)*pow(t2, 4) + m_Yout[5] * pow(t2, 5));

	if (t1 > m_bezier.Sm)
	{
		x_bezier = m_bezier.Sm*m_Xout[5];
		y_bezier = m_bezier.Sm*m_Yout[5];
	}


	m_Xe = cos(theta)*(x_bezier - m_robot.x) + sin(theta)*(y_bezier - m_robot.y);
	m_Ye = -sin(theta)*(x_bezier - m_robot.x) + cos(theta)*(y_bezier - m_robot.y);
	m_Thetae = theta - m_robot.angle;

	k1 = 1;
	k2 = 1;//1800
	k3 = 1;
	//if (Time_Add_Bezier < 4)
	//{
	//	k1 = 1;
	//	k2 = 1;
	//	k3 = 2;
	//}

	Vc = V_Bezier * cos(m_Thetae) + k1 * m_Xe;
	Wc = W_Bezier + k2 * V_Bezier*m_Ye + k3 * sin(m_Thetae);




	VL_Bezier = (2 * Vc - twoWheelDis * Wc) / 2 * 32 * 4096 / 2 / PI / wheelRadius;
	if (t1 > m_bezier.Sm)
	{
		VL_Bezier = 0;
	}

	if (VL_Bezier > 0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VL_Bezier = 0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}
	if (VL_Bezier < -0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VL_Bezier = -0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}

	VR_Bezier = (2 * Vc + twoWheelDis * Wc) / 2 * 32 * 4096 / 2 / PI / wheelRadius;
	if (t1 > m_bezier.Sm)
	{
		VR_Bezier = 0;
	}
	if (VR_Bezier > 0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VR_Bezier = 0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}
	if (VR_Bezier < -0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VR_Bezier = -0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}
	if (Mode_Simulation)
	{
		m_robot.angle = m_robot.angle + Time_Length_Bezier / 1000 * Wc;
		m_robot.x = m_robot.x + Time_Length_Bezier / 1000 * Vc*cos(m_robot.angle);
		m_robot.y = m_robot.y + Time_Length_Bezier / 1000 * Vc*sin(m_robot.angle);
	}
	else
	{
		Set_PWM_Bezier(1, VL_Bezier);
		Set_PWM_Bezier(2, VR_Bezier);
	}

}

void CMyoBezierDlg::Get_VandW2(double t1)
{
	double t2 = t1 / m_bezier2.Sm;


	Dx_bezier = (-5 * m_X2out[0] + 5 * m_X2out[1]) * pow(1 - t2, 4) + (-20 * m_X2out[1] + 20 * m_X2out[2])* pow(1 - t2, 3)* pow(t2, 1) + (-30 * m_X2out[2] + 30 * m_X2out[3])* pow(1 - t2, 2)* pow(t2, 2) + (-20 * m_X2out[3] + 20 * m_X2out[4])* pow(1 - t2, 1)* pow(t2, 3) + (-5 * m_X2out[4] + 5 * m_X2out[5])* pow(t2, 4);
	Dy_bezier = (-5 * m_Y2out[0] + 5 * m_Y2out[1]) * pow(1 - t2, 4) + (-20 * m_Y2out[1] + 20 * m_Y2out[2])* pow(1 - t2, 3)* pow(t2, 1) + (-30 * m_Y2out[2] + 30 * m_Y2out[3])* pow(1 - t2, 2)* pow(t2, 2) + (-20 * m_Y2out[3] + 20 * m_Y2out[4])* pow(1 - t2, 1)* pow(t2, 3) + (-5 * m_Y2out[4] + 5 * m_Y2out[5])* pow(t2, 4);
	DDx_bezier = (20 * m_X2out[0] - 40 * m_X2out[1] + 20 * m_X2out[2])* pow(1 - t2, 3) + (60 * m_X2out[1] - 120 * m_X2out[2] + 60 * m_X2out[3])* pow(1 - t2, 2)*pow(t2, 1) + (60 * m_X2out[2] - 120 * m_X2out[3] + 60 * m_X2out[4])* pow(1 - t2, 1)*pow(t2, 2) + (20 * m_X2out[3] - 40 * m_X2out[4] + 20 * m_X2out[5])*pow(t2, 3);
	DDy_bezier = (20 * m_Y2out[0] - 40 * m_Y2out[1] + 20 * m_Y2out[2])* pow(1 - t2, 3) + (60 * m_Y2out[1] - 120 * m_Y2out[2] + 60 * m_Y2out[3])* pow(1 - t2, 2)*pow(t2, 1) + (60 * m_Y2out[2] - 120 * m_Y2out[3] + 60 * m_Y2out[4])* pow(1 - t2, 1)*pow(t2, 2) + (20 * m_Y2out[3] - 40 * m_Y2out[4] + 20 * m_Y2out[5])*pow(t2, 3);

	V_Bezier = sqrt(pow(Dx_bezier, 2) + pow(Dy_bezier, 2));
	W_Bezier = (DDy_bezier / m_bezier2.Sm*Dx_bezier - DDx_bezier / m_bezier2.Sm * Dy_bezier) / Dx_bezier / Dx_bezier / (1 + Dy_bezier * Dy_bezier / Dx_bezier / Dx_bezier);

	if (t1 > m_bezier2.Sm)
	{
		V_Bezier = 0;
		W_Bezier = 0;
	}


	theta = atan2l(Dy_bezier, Dx_bezier);

	//theta += W_Bezier * Time_Length_Bezier / 1000;


	if (theta < -PI)
	{
		theta += 2 * PI;
	}
	if (theta >= PI)
	{
		theta -= 2 * PI;
	}

	x_bezier = m_bezier2.Sm*(m_X2out[0] * pow(1 - t2, 5) + 5 * m_X2out[1] * pow(1 - t2, 4)* t2 + 10 * m_X2out[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_X2out[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_X2out[4] * pow(1 - t2, 1)*pow(t2, 4) + m_X2out[5] * pow(t2, 5));
	y_bezier = m_bezier2.Sm*(m_Y2out[0] * pow(1 - t2, 5) + 5 * m_Y2out[1] * pow(1 - t2, 4)* t2 + 10 * m_Y2out[2] * pow(1 - t2, 3)* pow(t2, 2) + 10 * m_Y2out[3] * pow(1 - t2, 2)* pow(t2, 3) + 5 * m_Y2out[4] * pow(1 - t2, 1)*pow(t2, 4) + m_Y2out[5] * pow(t2, 5));

	if (t1 > m_bezier2.Sm)
	{
		x_bezier = m_bezier2.Sm*m_X2out[5];
		y_bezier = m_bezier2.Sm*m_Y2out[5];
	}


	m_Xe = cos(theta)*(x_bezier - m_robot.x) + sin(theta)*(y_bezier - m_robot.y);
	m_Ye = -sin(theta)*(x_bezier - m_robot.x) + cos(theta)*(y_bezier - m_robot.y);
	m_Thetae = theta - m_robot.angle;

	k1 = 1;
	k2 = 1;//1800
	k3 = 1;
	//if (Time_Add_Bezier < 4)
	//{
	//	k1 = 1;
	//	k2 = 1;
	//	k3 = 2;
	//}

	Vc = V_Bezier * cos(m_Thetae) + k1 * m_Xe;
	Wc = W_Bezier + k2 * V_Bezier*m_Ye + k3 * sin(m_Thetae);

	if (t1 > m_bezier2.Sm)
	{
		Vc = 0;
		Wc = 0;
	}


	VL_Bezier = (2 * Vc - twoWheelDis * Wc) / 2 * 32 * 4096 / 2 / PI / wheelRadius;
	if (t1 > m_bezier2.Sm)
	{
		VL_Bezier = 0;
	}

	if (VL_Bezier > 0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VL_Bezier = 0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}
	if (VL_Bezier < -0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VL_Bezier = -0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}

	VR_Bezier = (2 * Vc + twoWheelDis * Wc) / 2 * 32 * 4096 / 2 / PI / wheelRadius;
	if (t1 > m_bezier2.Sm)
	{
		VR_Bezier = 0;
	}
	if (VR_Bezier > 0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VR_Bezier = 0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}
	if (VR_Bezier < -0.3 * 32 * 4096 / 2 / PI / wheelRadius)
	{
		VR_Bezier = -0.3 * 32 * 4096 / 2 / PI / wheelRadius;
	}
	if (Mode_Simulation)
	{
		m_robot.angle = m_robot.angle + Time_Length_Bezier / 1000 * Wc;
		m_robot.x = m_robot.x + Time_Length_Bezier / 1000 * Vc*cos(m_robot.angle);
		m_robot.y = m_robot.y + Time_Length_Bezier / 1000 * Vc*sin(m_robot.angle);
	}
	else
	{
		Set_PWM_Bezier(1, VL_Bezier);
		Set_PWM_Bezier(2, VR_Bezier);
	}

}



void CMyoBezierDlg::Set_PWM_Bezier(long can_id, long speed)
{
	m_kvaser->speedMode(can_id, speed);
}

void CMyoBezierDlg::text()
{
	CSeries cys0 = (CSeries)m_CTchart_Bezier_path.Series(6);
	cys0.Clear();
	double x, y;
	x = (m_robot.x + m_TargePoint.x) / 2;
	y = (m_robot.y + m_TargePoint.y) / 2;
	cys0.AddXY(x, y, NULL, NULL);
}


void CMyoBezierDlg::Bezier_Add_End_y()
{
	m_bezier.End_Point_y = m_bezier.End_Point_y + 0.1;
	if (m_bezier.End_Point_y > YBound.second)
	{
		m_bezier.End_Point_y = YBound.second;

	}
	if (Time_Add_Bezier>m_bezier.Sm)
	{
		m_bezier.End_Point_y = (m_robot.y + m_TargePoint.y) / 2;
		m_bezier.End_Point_x = (m_robot.x + m_TargePoint.x) / 2;
	}
	Time_Add_Bezier = 0;
	m_bezier.Start_Point_x = m_robot.x;
	m_bezier.Start_Point_y = m_robot.y;
	m_bezier.Start_angle = m_robot.angle;
	m_bezier.Start_Speed = V_Bezier;
}
void CMyoBezierDlg::Bezier_Reduce_End_y()
{
	m_bezier.End_Point_y = m_bezier.End_Point_y - 0.1;
	if (m_bezier.End_Point_y < YBound.first + 0.1)
	{
		m_bezier.End_Point_y = YBound.first + 0.1;

	}
	if (Time_Add_Bezier > m_bezier.Sm)
	{
		m_bezier.End_Point_y = (m_robot.y + m_TargePoint.y) / 2;
		m_bezier.End_Point_x = (m_robot.x + m_TargePoint.x) / 2;
	}

	Time_Add_Bezier = 0;
	m_bezier.Start_Point_x = m_robot.x;
	m_bezier.Start_Point_y = m_robot.y;
	m_bezier.Start_angle = m_robot.angle;
	m_bezier.Start_Speed = V_Bezier;
}

void CMyoBezierDlg::Bezier_Add_End_x()
{
	m_bezier.End_Point_x = m_bezier.End_Point_x + 0.2;
	if (m_bezier.End_Point_x > XBound.second)
	{
		m_bezier.End_Point_x = XBound.second;

	}
	if (Time_Add_Bezier > m_bezier.Sm)
	{
		m_bezier.End_Point_y = (m_robot.y + m_TargePoint.y) / 2;
		m_bezier.End_Point_x = (m_robot.x + m_TargePoint.x) / 2;
	}

	Time_Add_Bezier = 0;
	m_bezier.Start_Point_x = m_robot.x;
	m_bezier.Start_Point_y = m_robot.y;
	m_bezier.Start_angle = m_robot.angle;
	m_bezier.Start_Speed = V_Bezier;
}
void CMyoBezierDlg::Bezier_Reduce_End_x()
{
	m_bezier.End_Point_x = m_bezier.End_Point_x - 0.2;
	if (m_bezier.End_Point_x < XBound.first)
	{
		m_bezier.End_Point_x = XBound.first;
	}
	if (Time_Add_Bezier > m_bezier.Sm)
	{
		m_bezier.End_Point_y = (m_robot.y + m_TargePoint.y) / 2;
		m_bezier.End_Point_x = (m_robot.x + m_TargePoint.x) / 2;
	}
	Time_Add_Bezier = 0;
	m_bezier.Start_Point_x = m_robot.x;
	m_bezier.Start_Point_y = m_robot.y;
	m_bezier.Start_angle = m_robot.angle;
	m_bezier.Start_Speed = V_Bezier;
}


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      