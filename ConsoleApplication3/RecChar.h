#pragma once
#include <cv.h>
#include <highgui.h>
#include "Labeling.h"
#include <iostream>
using namespace std;

class RecChar
{
public:
	RecChar();
	string GetAString(int* ShortXY, int SizeX, int SizeY, int NumLabel);///하나의 단어를 인식해 문자열반환
	string GetAChar(int* CharXY, int CharX, int CharY);///하나의 글자를 인식해 문자하나 반환
	int NumCrossgradient(int* CharXY, int SizeX, int SizeY, int x1, int x2, int y1, int y2);//처음값과 두번쨰값을 있는 축과의 교점수
	int XOfCrossDotMin(int* CharXY, int SizeX, int SizeY, int Y);////y값을 기준으로 교점의 최소x값을 반환
	int	YOfCrossDotMin(int* CharXY, int SizeX, int SizeY, int x);//x값을 기중으로 교점의 최솟값을반환
	int XOfCrossDotMax(int* CharXY, int SizeX, int SizeY, int Y);////y값을 기준으로 교점의 최대x값을 반환
	int	YOfCrossDotMax(int* CharXY, int SizeX, int SizeY, int x);//x값을 기중으로 교점의 최댓값을반환
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
	char ManyInBottom(int* CharXY, int SizeX, int SizeY, int StandardY);//기준y로부터 아래쪽중 가장많이 있는 쪽('R' 또는'L') 
	char ManyInUp(int* CharXY, int SizeX, int SizeY, int StandardY);//기준y로부터 위쪽중 가장많이 있는 쪽('R' 또는'L')
	char NotInBottom(int* CharXY, int SizeX, int SizeY, int StandardY);//기준y로부터 아래쪽중 픽셀값이 없는 쪽('R' 또는'L') 
	char NotInUp(int* CharXY, int SizeX, int SizeY, int StandardY);//기준y로부터 위쪽중 픽셀값이 없는 쪽('R' 또는'L')
	
	bool IsAlpha_a(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 a를 찾으면 true
	bool IsAlpha_b(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 b를 찾으면 true
	bool IsAlpha_c(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 c을 찾으면 true
	bool IsAlpha_d(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 d를 찾으면 true
	bool IsAlpha_e(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 e를 찾으면 true
	bool IsAlpha_f(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 f을 찾으면 true
	bool IsAlpha_g(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 g를 찾으면 true
	bool IsAlpha_h(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 h를 찾으면 true
	bool IsAlpha_i(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 l을 찾으면 true
	bool IsAlpha_j(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 j를 찾으면 true
	bool IsAlpha_k(int* CharXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 k를 찾으면 true
	bool IsAlpha_l(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 l을 찾으면 true
	bool IsAlpha_m(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 m을 찾으면 true
	bool IsAlpha_n(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 n을 찾으면 true
	bool IsAlpha_o(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 o을 찾으면 true
	bool IsAlpha_p(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 p을 찾으면 true
	bool IsAlpha_q(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 q을 찾으면 true
	bool IsAlpha_r(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 r을 찾으면 true
	bool IsAlpha_s(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 s을 찾으면 true
	bool IsAlpha_t(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 t을 찾으면 true
	bool IsAlpha_u(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 u을 찾으면 true
	bool IsAlpha_v(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 v을 찾으면 true
	bool IsAlpha_w(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 w을 찾으면 true
	bool IsAlpha_x(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 x을 찾으면 true
	bool IsAlpha_y(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 y을 찾으면 true
	bool IsAlpha_z(int* charXY, int SizeX, int SizeY);//조사한 단어라벨 맨 앞부분에서 z을 찾으면 true

	~RecChar();

private:
	string ReconitionChar;//인식된 단어의 문자
	string DicMean;//인식된 단어의 뜻
};

