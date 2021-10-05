#include "ChangeImgToBinary.h"
#include <iostream>
using namespace std;
IplImage* ChangeImgToBinary::ChangeToBlackOrWhite(IplImage* Img)
{
	IplImage* TempImg = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U, 1); //0~255�� ǥ���Ǵ� ����̹������·� �ʱ�ȭ
	cvCvtColor(Img, TempImg, CV_RGB2GRAY); // gray��image�� ��ȯ�ؼ� ����/ ���:������κ�ȯ
	//���̿� �ʺ� �翷���� ���پ�����(���ȭ�� �糡�� -1�� �Ǵ����� ����)
	cvReleaseImage(&Img);
	Img = cvCreateImage(cvSize(TempImg->width - 2, TempImg->height - 2), IPL_DEPTH_8U, 1); //0~255�� ǥ���Ǵ� ����̹������·� �ʱ�ȭ
	for (int y = 0; y < Img->height; y++){
		for (int x = 0; x < Img->width; x++){
			Img->imageData[y* Img->widthStep + x] = TempImg->imageData[(y+1)* TempImg->widthStep + (x+1)];
		}
	}
	cvReleaseImage(&TempImg);//�������ȭ���� ���� ���� �޸� ����	cvShowImage("My Wi2ndow", Img);
	
	return Img;//���img��ȯ
}
IplImage* ChangeImgToBinary::ChangeToBinary(IplImage* Img, int numberOf3, int Area2black)
{
	//����̹����� ��Ⱚ ������ ���ϴ� ���� ������ ������ �ȼ��� ���
	int* TripleArea;//���ҿ�����
	TripleArea = new int[numberOf3];
	int imagedata = 0;//�̹����� �����͸� �ӽ÷ι޾ƿ� ����
	for (int a = 0; a < numberOf3; a++)
		TripleArea[a] = 0;

	for (int y = 0; y < Img->height; y++){
		for (int x = 0; x < Img->width; x++){
			for (int threshold = 1; threshold <= numberOf3; threshold++){
				if ((int)Img->imageData[y*Img->widthStep + x] < 0)//��Ⱚ�� 128�̻����� �����÷ο� ����
					imagedata = Img->imageData[y*Img->widthStep + x] + 256;//����� ��ȯ
				else
					imagedata = Img->imageData[y*Img->widthStep + x];

				if (255 * (threshold - 1) / numberOf3 <= imagedata && imagedata <= 255 * threshold / numberOf3)
					TripleArea[threshold - 1]++;
			}
		}
	}
	
	//  (�ڽſ��� + ������ ��⿵��)�� ���� ���� ū ������ ��ġ ����
	int* NumInArea;//���� ������ ��ģ ���ҿ����� �ȼ���
	NumInArea = new int[numberOf3];
	for (int a = 1; a < numberOf3 - 1; a++){
		NumInArea[a] = TripleArea[a - 1] + TripleArea[a] + TripleArea[a + 1];
	}
	int first = 0;
	int second = 0;
	int max = 0;

	//���帹�� ��⿵�� ���ϱ�
	for (int a = 1; a < numberOf3 - 1; a++){
		if (max < NumInArea[a]){
			max = NumInArea[a];
			first = a;
		}
	}

	max = 0;
	//�ι�°�� ���帹�� ��⿵�� ���ϱ�
	for (int a = 1; a < numberOf3 - 1; a++){
		if (max < NumInArea[a] && abs(a - first) > 2){
			max = NumInArea[a];
			second = a;
		}
	}

	if (second == 0){//��⿵���� �ΰ��� ���������� ��������(�ѿ����� ����°��)
		//opencv ����ȭ �Լ� ����
		cvThreshold(Img, Img, 140, 255, CV_THRESH_BINARY);//������ �� �ȼ����� 128�� ������ 255 �ƴϸ� 0
		//����� ����� �ǵ��� ��ȯ
		//����� �ȼ��� Ȯ�ν�

		int black = 0;
		int white = 0;
		for (int y = 0; y < Img->height; y++){
			for (int x = 0; x < Img->width; x += 3){
				if (Img->imageData[y * Img->widthStep + x] == 0)//�����̸�
					black++;//�������� ����
			}
		}
		white = Img->imageSize / 3 - black;//������� ���
		if (white < black){//����� �����̸�
			//��� ����
			for (int y = 0; y < Img->height; y++){
				for (int x = 0; x < Img->width; x++){
					if (0 <= y&& y < Img->height && 0 <= x&& x < Img->width){//�̹����� ���������
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

		//��⺰ �������� �з��Ͽ� ����ȭ
		int	InAreaData;
		int	DefaultData;
		int SelArea;
		if (Area2black == 1){//���帹�� ������ ���, ����������
			InAreaData = 255;
			DefaultData = 0;
			SelArea = first;
		}
		else {//�ι�°�� ���� ������ ��ó��, ������ ���
			InAreaData = 0;
			DefaultData = 255;
			SelArea = second;
		}
		for (int y = 0; y < Img->height; y++){
			for (int x = 0; x < Img->width; x++){
				if (Img->imageData[y * Img->widthStep + x] < 0)//��Ⱚ�� 128�̻����� �����÷ο� ����
					imagedata = Img->imageData[y * Img->widthStep + x] + 256;//����� ��ȯ
				
					imagedata = Img->imageData[y * Img->widthStep + x];

					if (0 <= y&& y < Img->height && 0 <= x&& x < Img->width){//�̹����� ���������
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
	Img = ChangeToBlackOrWhite(Img);//���ȭ
	ChangeToBinary(Img, sensitive, Area2black);//threshold�� �Ӱ谪���� ����ȭ
	return Img;//��� ����ȭ�� �̹��� ��ȯ
}


