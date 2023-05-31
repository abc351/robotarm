
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_pic, pic);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
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
	}
	//이곳에 OpenCV 함수들을 적용합니다.
	//여기에서는 그레이스케일 이미지로 변환합니다.
	//cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);
	//화면에 보여주기 위한 처리입니다.
	CClientDC dc(GetDlgItem(IDC_pic));

	// Picture Control 크기를 얻는다.
	CRect rect;
	GetDlgItem(IDC_pic)->GetClientRect(&rect);

	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	// Picture Control DC에 호환되는 새로운 CDC를 생성. (임시 버퍼)
	memDC.CreateCompatibleDC(&dc);

	// Picture Control의 크기와 동일한 비트맵을 생성.
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	// 임시 버퍼에서 방금 생성한 비트맵을 선택하면서, 이전 비트맵을 보존.
	pOldBitmap = memDC.SelectObject(&bitmap);

	// 임시 버퍼에 검은색으로 채움.
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), BLACKNESS);

	// 임시 버퍼에 그리는 동작을 수행.
	//CRect rect;
	GetDlgItem(IDC_pic)->GetClientRect(&rect);
	CImage m_img;
	m_img.Attach(Mat2DIB(&mat_frame));
	m_img.Draw(memDC.GetSafeHdc(), rect);
	//CRect rect;
	GetDlgItem(IDC_pic)->GetClientRect(&rect);

	// 굵기 1, 빨간색, 실선 생성하기.
	/*
	CPen* pOldPen, pen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	pOldPen = memDC.SelectObject(&pen);
	memDC.MoveTo(shape.part(0).x(), shape.part(0).y());
	for (int i = 0; i < shape.num_parts(); i++) {
		memDC.LineTo(
			(int)((double)shape.part(i).x()/(double)img.nc()*(double)rect.Width()),
			(int)((double)shape.part(i).y()/ (double)img.nr()*rect.Height())
		
		);
	}
	memDC.SelectObject(pOldPen);

	pen.DeleteObject();
	*/
	// 임시 버퍼를 Picture Control에 그린다.
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	// 이전 비트맵으로 재설정.
	memDC.SelectObject(pOldBitmap);

	// 생성한 리소스 해제.
	memDC.DeleteDC();
	bitmap.DeleteObject();

	CDialogEx::OnTimer(nIDEvent);
}
#define max1(x,y) ((x>y)?x:y)
#define min1(x,y) ((x<y)?x:y)
int CMFCApplication1Dlg::detect(array2d<bgr_pixel> *img) {
	int ret=0;
	static short** l = 0, ** r = 0;
	static short **t = 0;
	static int width, height;
	if (l == 0) {
		width = img->nr(); height = img->nc();
		l = (short**)malloc(width*sizeof(short*));
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
	int lty = min1(ltl.y(), ltr.y())-2;
	int lby = max1(lbl.y(), lbr.y())+2;
	int lrx = lr.x()+2;
	int llx = ll.x()-2;
	int rty = min1(rtl.y(), rtr.y())-2;
	int rby = max1(rbl.y(), rbr.y())+2;
	int rrx = rr.x()+2;
	int rlx = rl.x()-2;
	int mlx = ml.x()-2;
	int mrx = mr.x()+2;
	int mty = mt.y()-2;
	int mby = mb.y()+2;
	int fx = (lrx - llx) / 2;
	int fy = (lby - lty) / 2;
	for (int i = llx; i < lrx ; i++) {
		for (int j = lty; j < lby; j++) {
			l[i][j] = ((*img)[j][i]).green;
			__int64 temp = 0;
			int cnt = 0;
			for (int a = (i > fx ? i - fx : 0); a < i + fx && a < width; a++) {
				for (int b = (j > fy ? j - fy : 0); b < j + fy && b < height; b++) {
					temp += (((*img)[b][a]).green); cnt++;
				}
			}
			t[i][j] = temp/cnt;
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
	static double pls=0, prs=0, pbs=0, pts=0;
	static int reset = -10;
	
	if (0<=reset&&reset < 10) {
		pls += pl; prs += pr; pts += pt; pbs += pb;
		if (reset == 9) {
			pls /= 10; prs /= 10; pts /= 10; pbs /= 10;
		}
	}
	if (reset < 10) reset++;
	if(reset==10) {
		pl -= pls; pr -= prs; pt -= pts; pb -= pbs;
	}
	pl += .4; pr += .4; pb += .4; pt += .4;
	//cout << setw(4) << pl << " " << pr << " " << pt << " " << pb << " ";

	/*
	if (::rand() % 2==0) {
		Mat e(100,200,CV_8SC3);
		for(int i=0;i<100;i++)
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
					e.at<Vec3b>( j - rty, i - rlx)[0] = 200;
			}
		
		imshow("a", e);
		cout << llx <<" "<< lrx << " " << lty << " " << lby<<endl;
	}
	*/
	bool blink;
	{
		static double oratio = -1;
		double ratio = (double)(lrx - llx) / (double)(lby - lty) + (double)(rrx - rlx) / (double)(rby - rty);
		if (oratio == -1) oratio = ratio;
		else oratio = oratio * 0.9 + ratio * 0.1;
		blink = (oratio - ratio < -1.2);
	}
	bool mouth;
	{
		static double oratio = -1;
		double ratio = (double)(mrx - mlx) / (double)(mby - mty);
		if (oratio == -1) oratio = ratio;
		else oratio = oratio * 0.8 + ratio * 0.2;
		//cout << oratio-ratio<< endl;
		mouth = (oratio - ratio > 4);
	}
	static char fl=0, fr=0, fu=0, fd=0;
	//cout << pl << " " << pr << " " << pt << " " << pb<<endl;
	if (reset == 10) {
		fl <<= 1; fr <<= 1; fu <<= 1; fd <<= 1;
		fl |= (pl > pr * 2);
		fr |= (pl * 2 < pr);
		fu |= (pt * 1.5 < pb);
		fd |= (pt > pb * 1.5);
		if ((fu & 0xF) == 0xF) cout << "u";
		else if ((fd & 0xF) == 0xF) cout << "d";
		if ((fl & 0xF) == 0xF) cout << "l";
		else if ((fr & 0xF) == 0xF) cout << "r";
		if (blink) cout << "!";
		if (mouth) cout << "?";
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
