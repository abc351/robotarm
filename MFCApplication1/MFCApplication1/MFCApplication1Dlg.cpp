
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#pragma comment(lib,"opencv_world460.lib")
#pragma comment(lib,"opencv_world460d.lib")
#pragma comment(lib,"dlib19.24.2_release_64bit_msvc1932.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
wchar_t* itoa(int a) {
	static wchar_t data[100];
	wsprintf(data, L"%d", a);
	return data;
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, mdown(_T(""))
	, mleft(_T(""))
	, mright(_T(""))
	, mup(_T(""))
	, mv1(_T(""))
	, mv2(_T(""))
	, mv3(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_pic, pic);
	DDX_Text(pDX, IDC_down, mdown);
	DDX_Text(pDX, IDC_left, mleft);
	DDX_Text(pDX, IDC_right, mright);
	DDX_Control(pDX, IDC_down, medown);
	DDX_Control(pDX, IDC_left, meleft);
	DDX_Control(pDX, IDC_right, meright);
	DDX_Control(pDX, IDC_up, meup);
	DDX_Text(pDX, IDC_up, mup);
	DDX_Text(pDX, IDC_v1, mv1);
	DDX_Control(pDX, IDC_v1, mev1);
	DDX_Control(pDX, IDC_v2, mev2);
	DDX_Text(pDX, IDC_v2, mv2);
	DDX_Control(pDX, IDC_v3, mev3);
	DDX_Text(pDX, IDC_v3, mv3);
	DDX_Control(pDX, IDC_start, sbtn);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
//	ON_EN_CHANGE(IDC_up, &CMFCApplication1Dlg::OnChangeUp)
//ON_WM_KEYDOWN()
//ON_WM_CHAR()
//ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnClickedButton1)
ON_EN_CHANGE(IDC_v3, &CMFCApplication1Dlg::OnChangeV3)
ON_EN_KILLFOCUS(IDC_v3, &CMFCApplication1Dlg::OnKillfocusV3)
ON_BN_CLICKED(IDC_start, &CMFCApplication1Dlg::OnClickedStart)
ON_BN_CLICKED(IDC_reset, &CMFCApplication1Dlg::OnClickedReset)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	capture = new VideoCapture(0);
	if (!capture->isOpened())
	{
		MessageBox(_T("웹캠을 열수 없습니다. \n"));
	}

	//웹캠 크기를  320x240으로 지정    
	capture->set(CAP_PROP_FRAME_WIDTH, 320);
	capture->set(CAP_PROP_FRAME_HEIGHT, 240);
	SetTimer(1000, 100, NULL);
	detector = get_frontal_face_detector();
	deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
	mev3.SetWindowText(CString(itoa(interval * 100)));
	ison = false;
	sbtn.SetWindowTextW(CString("시작"));
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

HBITMAP CMFCApplication1Dlg::Mat2DIB(Mat* Image)
{
	HBITMAP hbmp = NULL;
	int bpp = Image->channels() * 8;

	//Mat tmp;
	//cvtColor(*Image, tmp, CV_BGRA2BGR);

	BITMAPINFO bmpInfo = { 0 };
	LONG lBmpSize = Image->rows * Image->cols * 3;
	bmpInfo.bmiHeader.biBitCount = bpp;
	bmpInfo.bmiHeader.biHeight = Image->rows;
	bmpInfo.bmiHeader.biWidth = Image->cols;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Pointer to access the pixels of bitmap
	BYTE* pPixels = 0, * pP;
	hbmp = CreateDIBSection(NULL, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);

	if (!hbmp)
		return hbmp; // return if invalid bitmaps

	   //SetBitmapBits( hBitmap, lBmpSize, pBits);
	   // Directly Write
	int left_width = ((bpp * Image->cols + 31) / 32) * 4;
	pP = pPixels;
	for (int y = Image->rows - 1, row = 0; row < Image->rows; row++, y--) {
		for (int x = 0, col = 0; col < Image->cols; x += 3, col++) {
			pP[x] = Image->at<Vec3b>(y, col).val[0];
			pP[x + 1] = Image->at<Vec3b>(y, col).val[1];
			pP[x + 2] = Image->at<Vec3b>(y, col).val[2];
		}
		pP += left_width;
	}
	return hbmp;
}

void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	capture->read(mat_frame);
	array2d<bgr_pixel> img;
	assign_image(img, cv_image<bgr_pixel>(mat_frame));
	pyramid_up(img);
	std::vector<dlib::rectangle> dets = detector(img);
	if (dets.size() != 0) {
		shape = sp(img, dets[0]);
		detect(&img);
		CClientDC dc(GetDlgItem(IDC_pic));
		CRect rect;
		GetDlgItem(IDC_pic)->GetClientRect(&rect);
		CDC memDC;
		CBitmap* pOldBitmap, bitmap;
		memDC.CreateCompatibleDC(&dc);
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		pOldBitmap = memDC.SelectObject(&bitmap);
		memDC.PatBlt(0, 0, rect.Width(), rect.Height(), BLACKNESS);
		GetDlgItem(IDC_pic)->GetClientRect(&rect);
		CImage m_img;
		m_img.Attach(Mat2DIB(&mat_frame));
		m_img.Draw(memDC.GetSafeHdc(), rect);
		GetDlgItem(IDC_pic)->GetClientRect(&rect);
		CPen* pOldPen, pen;
		if(reset>=10) pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		else pen.CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		pOldPen = memDC.SelectObject(&pen);
		memDC.MoveTo(shape.part(0).x(), shape.part(0).y());
		for (int i = 0; i < shape.num_parts(); i++) {
			memDC.LineTo(
				(int)((double)shape.part(i).x() / (double)img.nc() * (double)rect.Width()),
				(int)((double)shape.part(i).y() / (double)img.nr() * rect.Height()));
		}
		memDC.SelectObject(pOldPen);
		pen.DeleteObject();
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
	CDialogEx::OnTimer(nIDEvent);
}
#define max1(x,y) ((x>y)?x:y)
#define min1(x,y) ((x<y)?x:y)
int CMFCApplication1Dlg::detect(array2d<bgr_pixel>* img) {
	int ret = 0;
	static short** l = 0, ** r = 0;
	static short** t = 0;
	static int width, height;
	if (l == 0) {
		width = img->nr(); height = img->nc();
		l = (short**)malloc(width * sizeof(short*));
		r = (short**)malloc(width * sizeof(short*));
		t = (short**)malloc(width * sizeof(short*));
		for (int i = 0; i < width; i++) {
			l[i] = (short*)malloc(height * sizeof(short));
			r[i] = (short*)malloc(height * sizeof(short));
			t[i] = (short*)malloc(height * sizeof(short));
		}
	}
	dlib::point ltl = shape.part(37),
		ltr = shape.part(38),
		ll = shape.part(36),
		lr = shape.part(39),
		lbl = shape.part(41),
		lbr = shape.part(40),
		rtl = shape.part(43),
		rtr = shape.part(44),
		rl = shape.part(42),
		rr = shape.part(45),
		rbl = shape.part(47),
		rbr = shape.part(46),
		mt = shape.part(62),
		mb = shape.part(66),
		ml = shape.part(60),
		mr = shape.part(64);
	int lty = min1(ltl.y(), ltr.y()) - 2;
	int lby = max1(lbl.y(), lbr.y()) + 2;
	int lrx = lr.x() + 2;
	int llx = ll.x() - 2;
	int rty = min1(rtl.y(), rtr.y()) - 2;
	int rby = max1(rbl.y(), rbr.y()) + 2;
	int rrx = rr.x() + 2;
	int rlx = rl.x() - 2;
	int mlx = ml.x() - 2;
	int mrx = mr.x() + 2;
	int mty = mt.y() - 2;
	int mby = mb.y() + 2;
	int fx = (lrx - llx) / 2;
	int fy = (lby - lty) / 2;
	for (int i = llx; i < lrx; i++) {
		for (int j = lty; j < lby; j++) {
			l[i][j] = ((*img)[j][i]).green;
			__int64 temp = 0;
			int cnt = 0;
			for (int a = (i > fx ? i - fx : 0); a < i + fx && a < width; a++) {
				for (int b = (j > fy ? j - fy : 0); b < j + fy && b < height; b++) {
					temp += (((*img)[b][a]).green); cnt++;
				}
			}
			t[i][j] = temp / cnt;
			t[i][j] -= 10;
		}
	}
	for (int i = llx; i < lrx; i++) {
		for (int j = lty; j < lby; j++) {
			l[i][j] = (l[i][j] < t[i][j]);
		}
	}
	double lpl, lpr, lpt, lpb;
	int cnt, cnt2;
	cnt = 0; cnt2 = 0;
	for (int i = (llx * 9 + lrx) / 10; i < (llx + lrx) / 2; i++)//l
		for (int j = (lty * 6 + lby * 4) / 10; j < (lty * 4 + lby * 6) / 10; j++) {
			cnt++;
			if (l[i][j] == 0) cnt2++;
		}
	lpl = (double)cnt2 / (double)cnt;
	cnt = 0; cnt2 = 0;
	for (int i = (llx + lrx) / 2; i < (llx + lrx * 9) / 10; i++)//l
		for (int j = (lty * 6 + lby * 4) / 10; j < (lty * 4 + lby * 6) / 10; j++) {
			cnt++;
			if (l[i][j] == 0) cnt2++;
		}
	lpr = (double)cnt2 / (double)cnt;
	cnt = 0; cnt2 = 0;
	for (int i = (llx * 6 + lrx * 4) / 10; i < (llx * 4 + lrx * 6) / 10; i++)//l
		for (int j = (lty * 9 + lby) / 10; j < (lty + lby) / 2; j++) {
			cnt++;
			if (l[i][j] == 0) cnt2++;
		}
	lpt = (double)cnt2 / (double)cnt;
	cnt = 0; cnt2 = 0;
	for (int i = (llx * 6 + lrx * 4) / 10; i < (llx * 4 + lrx * 6) / 10; i++)//l
		for (int j = (lty + lby) / 2; j < (lty + lby * 9) / 10; j++) {
			cnt++;
			if (l[i][j] == 0) cnt2++;
		}
	lpb = (double)cnt2 / (double)cnt;

	fx = (rrx - rlx) / 2;
	fy = (rby - rty) / 2;
	for (int i = rlx; i < rrx; i++) {
		for (int j = rty; j < rby; j++) {
			r[i][j] = ((*img)[j][i]).green;
			__int64 temp = 0;
			int cnt = 0;
			for (int a = (i > fx ? i - fx : 0); a < i + fx && a < width; a++) {
				for (int b = (j > fy ? j - fy : 0); b < j + fy && b < height; b++) {
					temp += (((*img)[b][a]).green); cnt++;
				}
			}
			t[i][j] = temp / cnt;
			t[i][j] -= 10;
		}
	}
	for (int i = rlx; i < rrx; i++) {
		for (int j = rty; j < rby; j++) {
			r[i][j] = (r[i][j] < t[i][j]);
		}
	}
	double rpl, rpr, rpt, rpb;
	cnt = 0; cnt2 = 0;
	for (int i = (rlx * 9 + rrx) / 10; i < (rlx + rrx) / 2; i++)//r
		for (int j = (rty * 6 + rby * 4) / 10; j < (rty * 4 + rby * 6) / 10; j++) {
			cnt++;
			if (r[i][j] == 0) cnt2++;
		}
	rpl = (double)cnt2 / (double)cnt;
	cnt = 0; cnt2 = 0;
	for (int i = (rlx + rrx) / 2; i < (rlx + rrx * 9) / 10; i++)//r
		for (int j = (rty * 6 + rby * 4) / 10; j < (rty * 4 + rby * 6) / 10; j++) {
			cnt++;
			if (r[i][j] == 0) cnt2++;
		}
	rpr = (double)cnt2 / (double)cnt;
	cnt = 0; cnt2 = 0;
	for (int i = (rlx * 6 + rrx * 4) / 10; i < (rlx * 4 + rrx * 6) / 10; i++)//r
		for (int j = (rty * 9 + rby) / 10; j < (rty + rby) / 2; j++) {
			cnt++;
			if (r[i][j] == 0) cnt2++;
		}
	rpt = (double)cnt2 / (double)cnt;
	cnt = 0; cnt2 = 0;
	for (int i = (rlx * 6 + rrx * 4) / 10; i < (rlx * 4 + rrx * 6) / 10; i++)//r
		for (int j = (rty + rby) / 2; j < (rty + rby * 9) / 10; j++) {
			cnt++;
			if (r[i][j] == 0) cnt2++;
		}
	rpb = (double)cnt2 / (double)cnt;
	double pl, pr, pb, pt;
	pl = lpl;// +rpl;
	pr = rpr;// lpr + rpr;
	pb = lpb + rpb;
	pt = lpt + rpt;
	static double pls = 0, prs = 0, pbs = 0, pts = 0;
	if (reset == -11) {
		reset = -10;
		pls = 0, prs = 0, pbs = 0, pts = 0;
	}
	if (0 <= reset && reset < 10) {
		pls += pl; prs += pr; pts += pt; pbs += pb;
		if (reset == 9) {
			pls /= 10; prs /= 10; pts /= 10; pbs /= 10;
		}
	}
	if (reset < 10) reset++;
	if (reset == 10) {
		pl -= pls; pr -= prs; pt -= pts; pb -= pbs;
	}
	pl += .4; pr += .4; pb += .4; pt += .4;
	/*
	if (::rand() % 2 == 0) {
		Mat e(100, 200, CV_8SC3);
		for (int i = 0; i < 100; i++)
			for (int j = 0; j < 200; j++) {
				e.at<Vec3b>(i, j)[0] = 0;
				e.at<Vec3b>(i, j)[1] = 0;
				e.at<Vec3b>(i, j)[2] = 0;
			}
		for (int i = llx; i < lrx; i++)
			for (int j = lty; j < lby; j++) {
				e.at<Vec3b>(j - lty, i - llx)[2] = l[i][j] * 200;
			}
		for (int i = rlx; i < rrx; i++)
			for (int j = rty; j < rby; j++) {
				if (e.at<Vec3b>(j - rty, i - rlx)[2] != 0 && r[i][j] != 0)
					e.at<Vec3b>(j - rty, i - rlx)[1] = 200;
				if (e.at<Vec3b>(j - rty, i - rlx)[2] == 0 && r[i][j] != 0)
					e.at<Vec3b>(j - rty, i - rlx)[0] = 200;
			}

		imshow("a", e);
		//cout << llx << " " << lrx << " " << lty << " " << lby << endl;
	}*/
	bool blink;
	bool bt;
	static char blinkflag = 0;
	{
		static double oratio = -1;
		static double oy = -1;
		double ratio = (double)(lrx - llx) / (double)(lby - lty) + (double)(rrx - rlx) / (double)(rby - rty);
		double yv = (double)(lty + rty)/ (double)(lrx - llx);
		if (oratio == -1) oratio = ratio,oy=yv;
		else oratio = oratio * 0.9 + ratio * 0.1,oy=oy*0.8+ yv*.2;
		bt = (oratio - ratio < -1.1)&&(oy-yv<-.05);
		if (!(blinkflag&0xF)) blink = bt;
		else blink = false;
		blinkflag <<= 1;
		blinkflag |= bt;
	}
	bool lip;
	bool lt;
	static char lipflag = 0;
	{
		static double oratio = -1;
		double ratio = (double)(mrx - mlx+5) / (double)(mby - mty+5);
		if (oratio == -1) oratio = ratio;
		else oratio = oratio * 0.8 + ratio * 0.2;
		lt = (oratio - ratio > 3);
		if (!(lipflag & 0xF)) lip = lt;
		else lip = false;
		lipflag <<= 1;
		lipflag |= lt;
	}
	static char fl = 0, fr = 0, fu = 0, fd = 0;
	//cout << pl << " " << pr << " " << pt << " " << pb<<endl;
	if (reset == 10) {
		fl <<= 1; fr <<= 1; fu <<= 1; fd <<= 1;
		fl |= (pl > pr * 2.5);
		fr |= (pl * 2.5 < pr);
		fu |= (pt * 1.5 < pb);
		fd |= (pt > pb * 1.3);

		iv++;
		if ((fu & 0xFF) == 0xFF) {
			//cout << "u";
			if(iv>=interval&&ison) iv=0, presskey(cup);
		}
		else if ((fd & 0xFF) == 0xFF) {
			//cout << "d";
			if (iv >= interval && ison) iv = 0, presskey(cdown);
		}
		else if ((fl & 0xFF) == 0xFF) {
			//cout << "l";
			if (iv >= interval && ison) iv = 0, presskey(cleft);
		}
		else if ((fr & 0xFF) == 0xFF) {
			//cout << "r";
			if (iv >= interval && ison) iv = 0, presskey(cright);
		}
		else if (blink) {
			//cout << "!";
			if(ison) presskey(cv1);
		}
		if (lip) {
			//cout << "?";
			if (ison) presskey(cv2);
		}
	}
	return ret;
}

int CMFCApplication1Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CMFCApplication1Dlg::keyinput(const char key) {
	int ctrlid = GetFocus()->GetDlgCtrlID();
	if (ctrlid == meup.GetDlgCtrlID()) {
		cup = key; meup.SetWindowText(map(key)); ::SetFocus(NULL);
	}
	else if (ctrlid == medown.GetDlgCtrlID()) {
		cdown = key; medown.SetWindowText(map(key)); ::SetFocus(NULL);
	}
	else if (ctrlid == meleft.GetDlgCtrlID()) {
		cleft = key; meleft.SetWindowText(map(key)); ::SetFocus(NULL);
	}
	else if (ctrlid == meright.GetDlgCtrlID()) {
		cright = key; meright.SetWindowText(map(key)); ::SetFocus(NULL);
	}
	else if (ctrlid == mev1.GetDlgCtrlID()) {
		cv1 = key; mev1.SetWindowText(map(key)); ::SetFocus(NULL);
	}
	else if (ctrlid == mev2.GetDlgCtrlID()) {
		cv2 = key; mev2.SetWindowText(map(key)); ::SetFocus(NULL);
	}
}


BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		keyinput(pMsg->wParam);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CMFCApplication1Dlg::presskey(char key) {

	if (!key) return;
	INPUT in;
	UpdateData(1);
	in.type = INPUT_KEYBOARD;
	in.ki.wVk = key;
	in.ki.wScan = 0;
	in.ki.dwFlags = 0;
	in.ki.time = 0;
	in.ki.dwExtraInfo = 0;
	SendInput(1, &in, sizeof in);
	in.type = INPUT_KEYBOARD;
	in.ki.wVk = key;
	in.ki.wScan = 0;
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	in.ki.time = 0;
	in.ki.dwExtraInfo = 0; SendInput(1, &in, sizeof in);
	UpdateData(0);
}




void CMFCApplication1Dlg::OnChangeV3()
{
	int v = _wtoi(mv3.GetString())/100;
	if (v > 0) interval = v;

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication1Dlg::OnKillfocusV3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	mev3.SetWindowText(CString(itoa(interval * 100)));
	UpdateData(0);
}


void CMFCApplication1Dlg::OnClickedStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	ison = !ison;
	if(ison) sbtn.SetWindowTextW(CString("중지"));
	else sbtn.SetWindowTextW(CString("시작"));
	UpdateData(0);
}


void CMFCApplication1Dlg::OnClickedReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	reset = -11;
}
