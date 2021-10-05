#pragma once
#include <cv.h>
#include <highgui.h>
#include "Labeling.h"
#include <iostream>
using namespace std;

class RecognitionChar
{
public:
	RecognitionChar(int* WordLabel, int MaxWordLabel, int height, int width);//좌표값, 레이블번호의 갯수, 좌표의크기를 받아옴
	void GetInformation(int*& MaxX, int*& MaxY, int*& MinX, int*& MinY, 
		string*& ReconitionChar, string*& DicMean, int& ShortXYNum);//인식된문자의 정보들 반환
	void CalShortXY();//레이블에해당하는 단어하나마다의 레이블좌표들을 구함
	void FromLabelToInformation();//라벨을 인식하여 문자정보를 획득
	string GetAString(int* ShortXY, int SizeX, int SizeY);///하나의 단어를 인식해 문자열반환
	string GetAChar(int* CharXY, int CharX, int CharY);///하나의 글자를 인식해 문자하나 반환
	int NumCrossDotY8(int* CharXY, int SizeX, int SizeY, int y);//y축을입력받아 x축과의 교점수를 가져옴
	int NumCrossDotX8(int* CharXY, int SizeX, int SizeY, int x);//x축을입력받아 y축과의 교점수를 가져옴
	int NumCrossDotY2(int* CharXY, int SizeX, int SizeY, int y);//y축을입력받아 x축과의 교점수를 가져옴
	int NumCrossDotX2(int* CharXY, int SizeX, int SizeY, int x);//x축을입력받아 y축과의 교점수를 가져옴
	int NumOfRight(int* CharXY, int SizeX, int SizeY, int StandardX);//기준X로부터 왼쪽의 1의 값의 픽셀수
	int NumOfLeft(int* CharXY, int SizeX, int SizeY, int StandardX);//기준X로부터 오른쪽의 1의 값의 픽셀수
	int NumOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY);//기준Y로부터 아래쪽의 1의값의 픽셀수
	int NumOfUp(int* CharXY, int SizeX, int SizeY, int StandardY);//기준Y로부터 위쪽의 1의값의 픽셀수
	int RightOfUp(int* CharXY, int SizeX, int SizeY, int StandardY);//기준Y로부터 위쪽의 가장오른쪽 x값
	int LeftOfUp(int* CharXY, int SizeX, int SizeY, int StandardY);//기준Y로부터 위쪽의 가장왼쪽 x값
	int RightOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY);//기준Y로부터 위쪽의 가장오른쪽 x값
	int LeftOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY);//기준Y로부터 아래쪽의 가장왼쪽 x값

	bool IsAlpha_o(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 o를 찾으면 true
	bool IsAlpha_r(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 r를 찾으면 true
	bool IsAlpha_l(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 l을 찾으면 true

	bool FitWord(string MyWord, string DicWord);//사전단어하나와 인식단어하나가 같을가능성이있는지 
	void ConverToDicString(string& RecString, string& DicMean, int* StringLabel);//인식한 단어를 사전에서 찾아완벽한 단어로 변환하고 단어뜻을 받아옴
	~RecognitionChar();
private:
	int* WordLabel;//라벨링된 이미지의 좌표라벨값들
	int** ShortXY;//레이블에해당하는 단어하나마다의 레이블좌표들
	int MaxWordLabel;//라벨의 갯수(라벨번호의 최대값)
	int height;//레이블좌표의 세로크기에 해당
	int width;//레이블좌표 가로크기에 해당
	int ShortXYNum;//식별할 단어의 갯수

	//문자인식후의 인식정보
	int* MaxX;//인식하는 단어의 최대 x
	int* MaxY;//인식하는 단어의 최대 y
	int* MinX;//인식하는 단어의 최소 x
	int* MinY;//인식하는 단어의 최소 y
	string* ReconitionChar;//인식된 단어의 문자
	string* DicMean;//인식된 단어의 뜻

};

