#pragma once
#include <iostream>
#include "Capture.h"
#include "ChangeImgToBinary.h"
#include <cv.h>         
#include <highgui.h>  
#include <Windows.h>
#include <string.h>
#include "Labeling.h"
#include "ChangeImgToBinary.h"
#include "opencv2/highgui/highgui.hpp"
#include "RecChar.h"
#include <fstream>

class Application
{
private:
	IplImage* src;//�巡�׵� �̹����� �ν��� ����
	IplImage* HandledSrc;//�巡�׵� �̹����� ��ó���� �̹���
	string RecWord;
	Capture captureImg;
public:
	Application();
	void run();
	void ShowAMean();
	void ConverToDicString(string RecString, string* SimilarWord, string* Similarmean, int LimitWordNum);//�ܾ���� ã�ƿ�
	int NumSame(string MyWord, string DicWord);//���ܾ�� ����� �ܾã�ƺ��� ����� �˷���
	~Application();
};

