
// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/geometry.h>
#include <iostream>
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
using namespace cv;
using namespace dlib;
using namespace std;
// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic pic;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	HBITMAP Mat2DIB(Mat* Image);
	int detect(array2d<bgr_pixel> *img);
	VideoCapture* capture;
	Mat mat_frame;
	CImage cimage_mfc;
	frontal_face_detector detector;
	shape_predictor sp;
	full_object_detection shape;

//	afx_msg void OnChangeUp();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CString mdown;
	CString mleft;
	CString mright;
	CEdit medown;
	CEdit meleft;
	CEdit meright;
	CEdit meup;
	CString mup;
	CString mv1;
	CEdit mev1;
	CEdit mev2;
	CString mv2;
	char cup=0, cdown=0, cleft=0, cright=0, cv1=0, cv2=0;
	inline CString map(const char key) const {
		if (key == VK_NEXT) return _T("Enter");
		else if (key == VK_ESCAPE) return _T("ESC");
		else if (key == VK_SPACE) return _T("Space");
		else if (key == VK_CONTROL) return _T("Ctrl");
		else if (key == VK_BACK) return _T("Bksp");
		else if (key == VK_CANCEL) return _T("Cancel");
		else if (key == VK_CAPITAL) return _T("CapsLock");
		else if (key == VK_CLEAR) return _T("Clear");
		else if (key == VK_DELETE) return _T("Del");
		else if (key == VK_END) return _T("End");
		else if (key == VK_SNAPSHOT) return _T("PrtScr");
		else if (key == VK_TAB) return _T("Tab");
		else if (key == VK_SHIFT) return _T("Shift");
		else if (key == VK_LWIN) return _T("Windows");
		else if (key == VK_MENU) return _T("Alt");
		else if (key == VK_HANJA) return _T("Hanja");
		else if (key == VK_HANGUL) return _T("Hangeul");
		else if (key == VK_SCROLL) return _T("ScrLock");
		else if (key == VK_PAUSE) return _T("Pause");
		else if (key == VK_HOME) return _T("Home");
		else if (key == VK_PRIOR) return _T("PgUp");
		else if (key == VK_NEXT) return _T("PgDn");
		else if (key == VK_INSERT) return _T("Insert");
		else if (key == VK_UP) return _T("UP");
		else if (key == VK_DOWN) return _T("DOWN");
		else if (key == VK_LEFT) return _T("LEFT");
		else if (key == VK_RIGHT) return _T("RIGHT");
		else if (key == VK_NUMLOCK) return _T("NumLock");
		else if (key == VK_F1) return _T("F1");
		else if (key == VK_F2) return _T("F2");
		else if (key == VK_F3) return _T("F3");
		else if (key == VK_F4) return _T("F4");
		else if (key == VK_F5) return _T("F5");
		else if (key == VK_F6) return _T("F6");
		else if (key == VK_F7) return _T("F7");
		else if (key == VK_F8) return _T("F8");
		else if (key == VK_F9) return _T("F9");
		else if (key == VK_F10) return _T("F10");
		else if (key == VK_F11) return _T("F11");
		else if (key == VK_F12) return _T("F12");
		else return CString(key);
	}
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void keyinput(const char key);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void presskey(char key);
};
