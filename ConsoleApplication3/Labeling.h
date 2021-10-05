#pragma once
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;

class Labeling
{
private:
	IplImage *src;//255미만의 번호들로 라벨링할 이미지 
	int* XYOfLabelImg;//int번호들로 영역을 라벨링한 라벨값배열
	int MaxLabelXY;//xy좌표로 담은 레이블번호의 갯수
	int height;//라벨링할 좌표 높이
	int width;//라벨링할 좌표 너비
	int widthstep;//일차원배열 이미지에서 y값 하나차이

public:
	Labeling(IplImage *src);
	Labeling(int height, int width);//높이와 너비로 초기화

	//설명:라벨링된 좌표를 구하고, x 수직선상에 일부가 같이 존재한다면 같은라벨로 묶음
	//출력:같은 세로선상에 있는 좌표들을 같은 영역으로 묶은 라벨좌표
	void GetXLabelingXY(int* arr);
	
	//설명:라벨된 좌표를 다시 라벨링
	//출력:주위 인접픽셀만들고려한 레이블 좌표 값들
	int* GetLabelingStringXY(int* XYOfLabelxy);

	//출력:xy좌표로 담은 레이블번호의 갯수 반환
	int GetMaxLabelXY();
	
	//출력:xy좌표의 크기반환
	int GetSize();
	//출력:높이반환
	int GetHeigth();
	//출력:너비반환
	int GetWidth();
	//출력:다음줄로 넘어가기위한 플러스값
	int GetWidthStep();

	//입력:라벤번호
	//출력:라벨번호에 해당되는 영역수 반환
	int GetMaxLabelXY(int LabelNUm);

	~Labeling();
};

