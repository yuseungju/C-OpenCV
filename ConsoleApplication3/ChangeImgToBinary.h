#pragma once
#include <cv.h>
#include <highgui.h>

class ChangeImgToBinary
{
public:
	//입력: 칼라이미지   
	// 반환: 흑백이미지
	IplImage* ChangeToBlackOrWhite(IplImage* Img);
	
	//설명: 분할밝기값영역의 차지 비율에 따라 이진화
	//입력: 흑백이미지 , 분할영역수,  영역 1: 가장많은 영역을 제외하고 모두 블랙 / 2: 두번쨰로많은영역만은 블랙
	//반환: 이진화된 이미지
	IplImage* ChangeToBinary(IplImage* Img, int numberOf3, int Area2black);
	
	//입력: 칼라이미지  , 분할영역수
	//반환: 흑백화 and 이진화된 이미지
	IplImage* GetPreProcess(IplImage* Img, int sensitive, int Area2black);
};