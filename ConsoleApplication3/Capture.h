#pragma once
#include <iostream>
#include <cv.h>          //영상처리를 위한 헤더
#include <highgui.h>   //카메라로 영상을 입력받거나 이미지를 읽어들이고 화면에 보여주기 위한 헤더
#include <atlimage.h>
#include <Windows.h>
#include <GdiPlus.h>
#include "opencv2/highgui/highgui.hpp"
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
using namespace std;
using namespace cv;

class Capture
{
private:
	IplImage* AllSrc;
	IplImage* CapImg;
	int DragMinX;
	int DragMaxX;
	int DragMinY;
	int DragMaxY;
public:
	void CapAllSrc();//모든영역캡쳐
	void ToDialog(string* word1, string* word2, int size);//입력받은 문자열을 창에 출력해서다시표시
	void WaitUse();//모든영역중 드래그했을때의 사간형영역의 최대 최소값x ,y 를 받아옴
	int GetEncoderClsid(WCHAR* type, CLSID* clsid);
	void CaptureDesktop(WCHAR* filename);//캡쳐이미지에 텍스트출력해줌
	IplImage* GetImageEntire();
	//설명: 드래그영역을 캡쳐함
	//출력: 드래그영역의 rgb이미지를 반환
	IplImage* GetCapImg();

	Capture();
	~Capture();
};

