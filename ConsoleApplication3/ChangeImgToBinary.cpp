#include "ChangeImgToBinary.h"
#include <iostream>
using namespace std;
IplImage* ChangeImgToBinary::ChangeToBlackOrWhite(IplImage* Img)
{
	IplImage* TempImg = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U, 1); //0~255로 표현되는 흑백이미지형태로 초기화
	cvCvtColor(Img, TempImg, CV_RGB2GRAY); // gray로image를 변환해서 넣음/ 방식:흑백으로변환
	//높이와 너비를 양옆으로 한줄씩줄임(흑백화시 양끝이 -1로 되는현상 방지)
	cvReleaseImage(&Img);
	Img = cvCreateImage(cvSize(TempImg->width - 2, TempImg->height - 2), IPL_DEPTH_8U, 1); //0~255로 표현되는 흑백이미지형태로 초기화
	for (int y = 0; y < Img->height; y++){
		for (int x = 0; x < Img->width; x++){
			Img->imageData[y* Img->widthStep + x] = TempImg->imageData[(y+1)* TempImg->widthStep + (x+1)];
		}
	}
	cvReleaseImage(&TempImg);//흑백이진화되지 않은 원본 메모리 해제	cvShowImage("My Wi2ndow", Img);
	
	return Img;//흑백img반환
}
IplImage* ChangeImgToBinary::ChangeToBinary(IplImage* Img, int numberOf3, int Area2black)
{
	//흑백이미지의 밝기값 영역을 원하는 수로 나누어 영역내 픽셀수 계산
	int* TripleArea;//분할영역수
	TripleArea = new int[numberOf3];
	int imagedata = 0;//이미지의 데이터를 임시로받아와 사용됨
	for (int a = 0; a < numberOf3; a++)
		TripleArea[a] = 0;

	for (int y = 0; y < Img->height; y++){
		for (int x = 0; x < Img->width; x++){
			for (int threshold = 1; threshold <= numberOf3; threshold++){
				if ((int)Img->imageData[y*Img->widthStep + x] < 0)//밝기값이 128이상으로 오버플로우 나면
					imagedata = Img->imageData[y*Img->widthStep + x] + 256;//양수로 변환
				else
					imagedata = Img->imageData[y*Img->widthStep + x];

				if (255 * (threshold - 1) / numberOf3 <= imagedata && imagedata <= 255 * threshold / numberOf3)
					TripleArea[threshold - 1]++;
			}
		}
	}
	
	//  (자신영역 + 인접한 밝기영역)의 합이 재일 큰 영역의 위치 설정
	int* NumInArea;//인접 영역을 합친 분할영역내 픽셀수
	NumInArea = new int[numberOf3];
	for (int a = 1; a < numberOf3 - 1; a++){
		NumInArea[a] = TripleArea[a - 1] + TripleArea[a] + TripleArea[a + 1];
	}
	int first = 0;
	int second = 0;
	int max = 0;

	//가장많은 밝기영역 구하기
	for (int a = 1; a < numberOf3 - 1; a++){
		if (max < NumInArea[a]){
			max = NumInArea[a];
			first = a;
		}
	}

	max = 0;
	//두번째로 가장많은 밝기영역 구하기
	for (int a = 1; a < numberOf3 - 1; a++){
		if (max < NumInArea[a] && abs(a - first) > 2){
			max = NumInArea[a];
			second = a;
		}
	}

	if (second == 0){//밝기영역이 두개로 나누어지지 못했으면(한영역만 생기는경우)
		//opencv 이진화 함수 적용
		cvThreshold(Img, Img, 140, 255, CV_THRESH_BINARY);//영상의 각 픽셀값이 128을 넘으면 255 아니면 0
		//흰색이 배경이 되도록 변환
		//디버깅 픽셀값 확인시

		int black = 0;
		int white = 0;
		for (int y = 0; y < Img->height; y++){
			for (int x = 0; x < Img->width; x += 3){
				if (Img->imageData[y * Img->widthStep + x] == 0)//검정이면
					black++;//검정개수 증가
			}
		}
		white = Img->imageSize / 3 - black;//흰색개수 계산
		if (white < black){//배경이 검정이면
			//흑백 반전
			for (int y = 0; y < Img->height; y++){
				for (int x = 0; x < Img->width; x++){
					if (0 <= y&& y < Img->height && 0 <= x&& x < Img->width){//이미지의 사이즈내에서
						if (Img->imageData[y * Img->widthStep + x] == 0)
							Img->imageData[y * Img->widthStep + x] = 255;
						else
							Img->imageData[y * Img->widthStep + x] = 0;
					}
				}
			}
		}
		return Img;
	}

		//밝기별 영역으로 분류하여 이진화
		int	InAreaData;
		int	DefaultData;
		int SelArea;
		if (Area2black == 1){//가장많은 영역만 흰색, 나머지검정
			InAreaData = 255;
			DefaultData = 0;
			SelArea = first;
		}
		else {//두번째로 많은 영역만 블랙처리, 나머지 흰색
			InAreaData = 0;
			DefaultData = 255;
			SelArea = second;
		}
		for (int y = 0; y < Img->height; y++){
			for (int x = 0; x < Img->width; x++){
				if (Img->imageData[y * Img->widthStep + x] < 0)//밝기값이 128이상으로 오버플로우 나면
					imagedata = Img->imageData[y * Img->widthStep + x] + 256;//양수로 변환
				
					imagedata = Img->imageData[y * Img->widthStep + x];

					if (0 <= y&& y < Img->height && 0 <= x&& x < Img->width){//이미지의 사이즈내에서
						if (255 * (SelArea - 1) / numberOf3 <= imagedata && imagedata <= 255 * (SelArea + 2) / numberOf3)
							Img->imageData[y * Img->widthStep + x] = InAreaData;
						else
							Img->imageData[y * Img->widthStep + x] = DefaultData;
					}
			}
		}
	return Img;
}

IplImage* ChangeImgToBinary::GetPreProcess(IplImage* Img, int sensitive, int Area2black)
{
	Img = ChangeToBlackOrWhite(Img);//흑백화
	ChangeToBinary(Img, sensitive, Area2black);//threshold을 임계값으로 이진화
	return Img;//흑백 이진화된 이미지 반환
}


