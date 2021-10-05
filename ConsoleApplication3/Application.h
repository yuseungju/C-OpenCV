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
	IplImage* src;//드래그된 이미지로 인식할 영역
	IplImage* HandledSrc;//드래그된 이미지로 전처리될 이미지
	string RecWord;
	Capture captureImg;
public:
	Application();
	void run();
	void ShowAMean();
	void ConverToDicString(string RecString, string* SimilarWord, string* Similarmean, int LimitWordNum);//단어뜻을 찾아옴
	int NumSame(string MyWord, string DicWord);//내단어와 비슷한 단어를찾아보고 결과를 알려줌
	~Application();
};

