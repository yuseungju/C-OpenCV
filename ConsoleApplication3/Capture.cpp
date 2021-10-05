#include "Capture.h"

int tmouse_upX = -1;
int tmouse_upY = -1;
int tmouse_bottomX = -1;
int tmouse_bottomY = -1;
Capture::Capture()
{
	AllSrc = NULL;
	CapImg = NULL;
	//�ܼ�â �����
	HWND hWndConsole = GetConsoleWindow();
	ShowWindow(hWndConsole, SW_HIDE);
	Sleep(800);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (tmouse_bottomX = -1 && tmouse_bottomY == -1){
			tmouse_bottomX = x;
			tmouse_bottomY = y;
		}
	}
	else if (event == EVENT_LBUTTONUP)
	{
		if (tmouse_upX = -1 && tmouse_upY == -1){
			tmouse_upX = x;
			tmouse_upY = y;
		}
	}
}

void Capture::CapAllSrc(){
	if (AllSrc != NULL)
		return;
	GdiplusStartupInput gsi;
	ULONG_PTR g;
	GdiplusStartup(&g, &gsi, 0);
	//������ ������ ������� ĸ��
	CaptureDesktop(L"AllImg.png");
	GdiplusShutdown(g);

	//��ü ĸ�� �̹����� ����
	AllSrc = cvLoadImage("AllImg.png");
}

void Capture::ToDialog(string* word1, string* word2, int size)
{
	string InText = "";
	for (int a = 0; a < size; a++){
		if (word1[a] != "" && word2[a][a])
			InText += word1[a] + "/" + word2[a] + "\n";
	}
	std::wstring stemp = std::wstring(InText.begin(), InText.end());
	LPCWSTR sw = stemp.c_str();
	MessageBox(NULL, CA2T(InText.c_str()), TEXT("�˻� ���ܾ�"), MB_OK);

	return;
}
void Capture::WaitUse(){

	bool correctArea = false;
	while (!correctArea){//�ùٸ� ������ �巡���Ҷ�����
		//Create a window �׸��� ��ü ĸ���̹����� â�� ������
		cvDestroyAllWindows();
		namedWindow("My Window", CV_WINDOW_NORMAL);
		cvMoveWindow("My Window", 0, 0);
		cvShowImage("My Window", AllSrc);
		if (waitKey(0) == 0x09){ //��Ű ������ �巡�׽���
			while (tmouse_upX == -1){//�巡�װ� ������ ���콺 �� �̺�Ʈ�ϋ� ���� ����
				//���콺�� �ٿ� �� ���� ��ǥ�� ���
				setMouseCallback("My Window", CallBackFunc, NULL);
				waitKey(0);
			}
			//���콺 �巡���� ������ ��ǥ ���
			if (tmouse_upX > tmouse_bottomX){
				DragMaxX = tmouse_upX;
				DragMinX = tmouse_bottomX;
			}
			else {
				DragMaxX = tmouse_bottomX;
				DragMinX = tmouse_upX;
			}
			if (tmouse_upY < tmouse_bottomY){
				DragMaxY = tmouse_bottomY;
				DragMinY = tmouse_upY;
			}
			else {
				DragMaxY = tmouse_upY;
				DragMinY = tmouse_bottomY;
			}
			if ((DragMaxY - DragMinY) > 7 && (DragMaxX - DragMinX) > 7 && DragMinY >10 && DragMaxY < AllSrc->height && DragMinX >10 && DragMaxX < AllSrc->width)
				correctArea = true;
			else
				correctArea = false;
			//���콺 �̺�Ʈ ������ �ʱⰪ���� �ٽü���
			tmouse_upX = -1;
			tmouse_upY = -1;
			tmouse_bottomX = -1;
			tmouse_bottomY = -1;
		}
		else {
			exit(0);
		}
	}
}
int Capture::GetEncoderClsid(WCHAR* type, CLSID* clsid)//ĸ�İ��� �ʱ� ����
{
	unsigned int ne = 0, s = 0;

	GetImageEncodersSize(&ne, &s);
	if (s == 0) return -1;

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)malloc(s);
	if (pImageCodecInfo == 0) return -1;

	GetImageEncoders(ne, s, pImageCodecInfo);
	unsigned int i;
	for (i = 0; i < ne; ++i)
	{
		if (wcscmp(pImageCodecInfo[i].MimeType, type) == 0)
		{
			*clsid = pImageCodecInfo[i].Clsid;
		}
	}
	free(pImageCodecInfo);
	return i;
	free(pImageCodecInfo);
	return -1;
}

void Capture::CaptureDesktop(WCHAR* filename)//������ ������ ĸ���� ���Ϸ�����
{
	HDC hdc, hMemdc;
	HBITMAP hBitmap;
	//������ ��ü ������ 
	hdc = GetDC(GetDesktopWindow());
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	hMemdc = CreateCompatibleDC(hdc); 
	hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hMemdc, hBitmap);
	BitBlt(hMemdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY | CAPTUREBLT);

	HBITMAP hFileBitmap;
	hFileBitmap = (HBITMAP)SelectObject(hMemdc, hBitmap);
	Gdiplus::Bitmap fBitmap(hFileBitmap, 0);
	CLSID clsid;
	GetEncoderClsid(L"image/png", &clsid);
	fBitmap.Save(filename, &clsid);
	DeleteObject(hFileBitmap);
}


IplImage* Capture::GetCapImg(){
	CapAllSrc();//��� ���ȭ�� ĸ���Ͽ� ���Ϸ�����
	WaitUse();//ĸ�ĵ� ������ �巡�� ���� ��ǥ ���
	 
	//�巡�� ������ ���� ���� 
	if (DragMaxX >= AllSrc->width)//�̹����� �����ʰ���
		DragMaxX = AllSrc->width - 1; 
	if (DragMinX < 0)//�̹����� �����ʰ���
		DragMinX = 0;
	if (DragMaxY >= AllSrc->height)//�̹����� �����ʰ���
		DragMaxY = AllSrc->height - 1;
	if (DragMinY < 0)//�̹����� �����ʰ���
		DragMinY = 0;

	//�巡�� �̹����� ���������
	int SizeX = DragMaxX - DragMinX + 1;
 	int Sizey = DragMaxY - DragMinY + 1;

	//�巡�� �̹����� CapSrcOfBinary�� ����
	CapImg = cvCreateImage(cvSize(SizeX, Sizey), 8, 3);
	for (int y = DragMinY, iny = 0; y < DragMaxY; y++, iny++){//�巡�׿����� ���� ����
		for (int x = DragMinX * 3, inx = 0; x < DragMaxX * 3; x++, inx++){//�巡�׿����ǰ��ι�����ŭ
			CapImg->imageData[inx + (iny*CapImg->widthStep)] = AllSrc->imageData[x + (y*(AllSrc->widthStep))];//�κ������δ���
		}
	}

	//�巡�׿������� �̹��� ��ȯ
	return CapImg;
}
IplImage* Capture::GetImageEntire()
{
	return AllSrc;
}

Capture::~Capture()
{
	cvReleaseImage(&AllSrc);
	cvReleaseImage(&CapImg);
}
