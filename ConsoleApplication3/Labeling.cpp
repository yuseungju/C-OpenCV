

#include "Labeling.h"

Labeling::Labeling(IplImage *src)
{
	this->height = src->height;
	this->width = src->width;
	this->widthstep = src->widthStep;
	this->src = cvCloneImage(src);//복사해서 넣는다
	XYOfLabelImg = new int[height * width];
	for (int j = 0; j < height * width; j++){
		XYOfLabelImg[j] = -1;
	}//라벨링되는 이미지의 좌표 초기화
}


Labeling::Labeling(int height, int width)
{
	this->height = height;
	this->width = width;
	XYOfLabelImg = new int[height * width];
	for (int j = 0; j < height * width; j++){
		XYOfLabelImg[j] = -1;
	}//라벨링되는 좌표 초기화
}

void Labeling::GetXLabelingXY(int* arr)
{
	int* inputImage = new int[height * width];//임시원본좌표

	for (int y = 0; y < height; y++){//원본의 높이만큼
		for (int x = 0; x < width; x++){//원본의 너비만큼
			inputImage[width * y + x] = src->imageData[src->widthStep * y + x];//원본이미지의 픽셀값을 가짐
		}
	}

	int* SaveX = new int[width * height];//주변을 찾을 기준좌표 x값 저장
	int* SaveY = new int[width * height];//주변을 찾을 기준좌표 y값 저장
	for (int i = 0; i < width * height; i++){
		SaveX[i] = 0;
		SaveY[i] = 0;
	}
	int SaveLenght = 0;//좌표 저장갯수
	int labelNumber = 0;//해당숫자로 라벨링
	for (int y = 0; y < height; y++){//원본좌표 높이만큼
		for (int x = 0; x < width; x++){//원본좌표 너비만큼

			if (inputImage[width * y + x] != -1 && XYOfLabelImg[width * y + x] == -1) {
				//원본좌표의 픽셀값이 존재하고, 결과 좌표값에 라벨링된 숫자가 없으면
				
				labelNumber++;//라벨링할 번호를 전 과다르게 설정

				//조사할 기준좌표저장
				SaveX[SaveLenght] = x;
				SaveY[SaveLenght] = y;
				SaveLenght++;//저장갯수증가

				while (SaveLenght > 0){//조사할 좌표가 남아있으면
					int kx = SaveX[SaveLenght - 1];//라벨링하기위해 기준이 되는 픽셀 x값 하나
					int ky = SaveY[SaveLenght - 1];//라벨링하기위해 기준이 되는 픽셀 y값 하나
					SaveLenght--;

					XYOfLabelImg[width * ky + kx] = labelNumber;//기준을 가장먼저 라벨링(숫자부여)

					// 가로3 세로3의 픽셀 중 색이 있으면 같은숫자로 라벨링
					for (int ny = ky - height * 3 / 7; ny <= ky + height * 3 / 7; ny++){//세로3개중 위에서부터 
						if (ny > 0 && ny < height){//이미지의 범위안에있으면

							for (int nx = kx - 1; nx <= kx + 1; nx++){//가로3개중 왼쪽에서부터 
								if (nx > 0 && nx < width){//이미지의 범위안에있으면

									if (inputImage[width * ny + nx] != -1 && XYOfLabelImg[width * ny + nx] == -1) {
										//기존좌표값이 존재하고, 결과 좌표값에 라벨링된 숫자가 없으면
										XYOfLabelImg[width * ny + nx] = labelNumber;//해당픽셀 라벨링하여 가짐
										//조사할 기준좌표저장
										SaveX[SaveLenght] = nx;
										SaveY[SaveLenght] = ny;
										SaveLenght++;//저장갯수증가
									}
								}
							}
						}
					}//for
				}//while
			}//if
		}
	}
	//MaxLabelXY에 라벨의 개수를 가지게함
	MaxLabelXY = labelNumber;
	//매개변수 int*로 데이터를 복사
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){	
			arr[width * y + x] = XYOfLabelImg[src->width * y + x];
		}
	}

	/*
	//디버깅 픽셀값 확인시
	cvNamedWindow("라벨링과정", 1);
	cvShowImage("라벨링과정", src);
	cvWaitKey(0);
	cout << "라벨링과정라벨링과정라벨링과정라벨링과정라벨링과정라벨링과정" << endl;; //원본이미지의 픽셀값을 가짐
	for (int y = 0; y < height; y++){//원본의 높이만큼
		for (int x = 0; x < width; x++){//원본의 너비만큼
			cout << (int)arr[width * y + x]; //원본이미지의 픽셀값을 가짐
		}
		cout << endl;
	}*/

	//동적할당해제
	delete[] inputImage;
	delete[] SaveX;
	delete[] SaveY;
}

int* Labeling::GetLabelingStringXY(int* XYOfLabelxy)
{
	for (int j = 0; j < height * width; j++){
		XYOfLabelImg[j] = -1;
	}//라벨링되는 이미지의 좌표 초기화

	int* inputImage = new int[height * width];//임시원본좌표

	for (int y = 0; y < height; y++){//원본의 높이만큼
		for (int x = 0; x < width; x++){//원본의 너비만큼
			inputImage[width * y + x] = XYOfLabelxy[width * y + x];//원본이미지의 픽셀값을 가짐
		}
	}

	int* SaveX = new int[width * height];//주변을 찾을 기준좌표 x값 저장
	int* SaveY = new int[width * height];//주변을 찾을 기준좌표 y값 저장
	for (int i = 0; i < width * height; i++){
		SaveX[i] = 0;
		SaveY[i] = 0;
	}
	int SaveLenght = 0;//좌표 저장갯수

	int labelNumber = 0;//해당숫자로 라벨링
	for (int y = 1; y < height - 1; y++){//원본좌표 높이만큼
		for (int x = 1; x < width - 1; x++){//원본좌표 너비만큼

			if (inputImage[width * y + x] != -1 && XYOfLabelImg[width * y + x] == -1) {
				//원본좌표의 픽셀값이 존재하고, 결과 좌표값에 라벨링된 숫자가 없으면
				labelNumber++;//라벨링할 번호를 전 과다르게 설정

				//조사할 기준좌표저장
				SaveX[SaveLenght] = x;
				SaveY[SaveLenght] = y;
				SaveLenght++;//저장갯수증가

				while (SaveLenght > 0){//조사할 좌표가 남아있으면
					int kx = SaveX[SaveLenght - 1];//라벨링하기위해 기준이 되는 픽셀 x값 하나
					int ky = SaveY[SaveLenght - 1];//라벨링하기위해 기준이 되는 픽셀 y값 하나
					SaveLenght--;

					if (XYOfLabelImg[width * ky + kx] == -1)//라벨값이 없으면
					XYOfLabelImg[width * ky + kx] = labelNumber;//기준을 가장먼저 라벨링(숫자부여)

					// 가로1 세로a의 픽셀 중 색이 있으면 같은숫자로 라벨링
					for (int ny = ky - 1; ny <= ky + 1; ny++){//세로3개중 위에서부터 
						if (ny > 0 && ny < height){//이미지의 범위안에있으면

							for (int nx = kx - 1; nx <= kx + 1; nx++){//가로3개중 왼쪽에서부터 
								if (nx > 0 && nx < width){//이미지의 범위안에있으면

									if (inputImage[width * ny + nx] != -1 && XYOfLabelImg[width * ny + nx] == -1) {
										//기존좌표값이 존재하고, 결과 좌표값에 라벨링된 숫자가 없으면
										XYOfLabelImg[width * ny + nx] = labelNumber;//해당픽셀 라벨링하여 가짐
										//조사할 기준좌표저장
										SaveX[SaveLenght] = nx;
										SaveY[SaveLenght] = ny;
										SaveLenght++;//저장갯수증가
									}
								}
							}
						}
					}//for
				}//while
			}//if
		}
	}

	MaxLabelXY = labelNumber;//부여한 레이블숫자의 갯수
	delete[] inputImage;
	delete[] SaveX;
	delete[] SaveY;
	return XYOfLabelImg;//라벨링된 좌표반환
}

int Labeling::GetMaxLabelXY()
{
	return MaxLabelXY;
}
int Labeling::GetHeigth()
{
	return height;
}
int Labeling::GetWidth()
{
	return width;
}
int Labeling::GetWidthStep()
{
	return widthstep;
}

int Labeling::GetSize(){
	return height*width;
}
Labeling::~Labeling()
{
	//동적할당 메모리해제
	cvReleaseImage(&src);
	delete[] XYOfLabelImg;
}

































