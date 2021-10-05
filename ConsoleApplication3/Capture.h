#pragma once
#include <iostream>
#include <cv.h>          //����ó���� ���� ���
#include <highgui.h>   //ī�޶�� ������ �Է¹ްų� �̹����� �о���̰� ȭ�鿡 �����ֱ� ���� ���
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
	void CapAllSrc();//��翵��ĸ��
	void ToDialog(string* word1, string* word2, int size);//�Է¹��� ���ڿ��� â�� ����ؼ��ٽ�ǥ��
	void WaitUse();//��翵���� �巡���������� �簣�������� �ִ� �ּҰ�x ,y �� �޾ƿ�
	int GetEncoderClsid(WCHAR* type, CLSID* clsid);
	void CaptureDesktop(WCHAR* filename);//ĸ���̹����� �ؽ�Ʈ�������
	IplImage* GetImageEntire();
	//����: �巡�׿����� ĸ����
	//���: �巡�׿����� rgb�̹����� ��ȯ
	IplImage* GetCapImg();

	Capture();
	~Capture();
};

