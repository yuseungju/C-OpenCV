#include "Capture.h"

int tmouse_upX = -1;
int tmouse_upY = -1;
int tmouse_bottomX = -1;
int tmouse_bottomY = -1;
Capture::Capture()
{
	AllSrc = NULL;
	CapImg = NULL;
	//콘솔창 숨기기
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
	//윈도우 정해진 사이즈로 캡쳐
	CaptureDesktop(L"AllImg.png");
	GdiplusShutdown(g);

	//전체 캡쳐 이미지를 저장
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
	MessageBox(NULL, CA2T(InText.c_str()), TEXT("검색 영단어"), MB_OK);

	return;
}
void Capture::WaitUse(){

	bool correctArea = false;
	while (!correctArea){//올바른 영역을 드래그할때까지
		//Create a window 그리고 전체 캡쳐이미지로 창을 보여줌
		cvDestroyAllWindows();
		namedWindow("My Window", CV_WINDOW_NORMAL);
		cvMoveWindow("My Window", 0, 0);
		cvShowImage("My Window", AllSrc);
		if (waitKey(0) == 0x09){ //텝키 누르면 드래그시작
			while (tmouse_upX == -1){//드래그가 끝나는 마우스 업 이벤트일떄 까지 동작
				//마우스의 다운 과 업의 좌표를 기록
				setMouseCallback("My Window", CallBackFunc, NULL);
				waitKey(0);
			}
			//마우스 드래그한 정보로 좌표 기록
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
			//마우스 이벤트 없었던 초기값으로 다시설정
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
int Capture::GetEncoderClsid(WCHAR* type, CLSID* clsid)//캡쳐관련 초기 설정
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

void Capture::CaptureDesktop(WCHAR* filename)//실질적 윈도우 캡쳐후 파일로저장
{
	HDC hdc, hMemdc;
	HBITMAP hBitmap;
	//윈도우 전체 사이즈 
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
	CapAllSrc();//모든 배경화면 캡쳐하여 파일로저장
	WaitUse();//캡쳐된 영역내 드래그 영역 좌표 얻기
	 
	//드래그 영역의 범위 설정 
	if (DragMaxX >= AllSrc->width)//이미지의 영역초과시
		DragMaxX = AllSrc->width - 1; 
	if (DragMinX < 0)//이미지의 영역초과시
		DragMinX = 0;
	if (DragMaxY >= AllSrc->height)//이미지의 영역초과시
		DragMaxY = AllSrc->height - 1;
	if (DragMinY < 0)//이미지의 영역초과시
		DragMinY = 0;

	//드래그 이미지의 사이즈산출
	int SizeX = DragMaxX - DragMinX + 1;
 	int Sizey = DragMaxY - DragMinY + 1;

	//드래그 이미지를 CapSrcOfBinary로 얻음
	CapImg = cvCreateImage(cvSize(SizeX, Sizey), 8, 3);
	for (int y = DragMinY, iny = 0; y < DragMaxY; y++, iny++){//드래그영역의 세로 범위
		for (int x = DragMinX * 3, inx = 0; x < DragMaxX * 3; x++, inx++){//드래그영역의가로범위만큼
			CapImg->imageData[inx + (iny*CapImg->widthStep)] = AllSrc->imageData[x + (y*(AllSrc->widthStep))];//부분적으로대입
		}
	}

	//드래그영역내의 이미지 반환
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
