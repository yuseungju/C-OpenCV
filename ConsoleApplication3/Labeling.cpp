

#include "Labeling.h"

Labeling::Labeling(IplImage *src)
{
	this->height = src->height;
	this->width = src->width;
	this->widthstep = src->widthStep;
	this->src = cvCloneImage(src);//�����ؼ� �ִ´�
	XYOfLabelImg = new int[height * width];
	for (int j = 0; j < height * width; j++){
		XYOfLabelImg[j] = -1;
	}//�󺧸��Ǵ� �̹����� ��ǥ �ʱ�ȭ
}


Labeling::Labeling(int height, int width)
{
	this->height = height;
	this->width = width;
	XYOfLabelImg = new int[height * width];
	for (int j = 0; j < height * width; j++){
		XYOfLabelImg[j] = -1;
	}//�󺧸��Ǵ� ��ǥ �ʱ�ȭ
}

void Labeling::GetXLabelingXY(int* arr)
{
	int* inputImage = new int[height * width];//�ӽÿ�����ǥ

	for (int y = 0; y < height; y++){//������ ���̸�ŭ
		for (int x = 0; x < width; x++){//������ �ʺ�ŭ
			inputImage[width * y + x] = src->imageData[src->widthStep * y + x];//�����̹����� �ȼ����� ����
		}
	}

	int* SaveX = new int[width * height];//�ֺ��� ã�� ������ǥ x�� ����
	int* SaveY = new int[width * height];//�ֺ��� ã�� ������ǥ y�� ����
	for (int i = 0; i < width * height; i++){
		SaveX[i] = 0;
		SaveY[i] = 0;
	}
	int SaveLenght = 0;//��ǥ ���尹��
	int labelNumber = 0;//�ش���ڷ� �󺧸�
	for (int y = 0; y < height; y++){//������ǥ ���̸�ŭ
		for (int x = 0; x < width; x++){//������ǥ �ʺ�ŭ

			if (inputImage[width * y + x] != -1 && XYOfLabelImg[width * y + x] == -1) {
				//������ǥ�� �ȼ����� �����ϰ�, ��� ��ǥ���� �󺧸��� ���ڰ� ������
				
				labelNumber++;//�󺧸��� ��ȣ�� �� ���ٸ��� ����

				//������ ������ǥ����
				SaveX[SaveLenght] = x;
				SaveY[SaveLenght] = y;
				SaveLenght++;//���尹������

				while (SaveLenght > 0){//������ ��ǥ�� ����������
					int kx = SaveX[SaveLenght - 1];//�󺧸��ϱ����� ������ �Ǵ� �ȼ� x�� �ϳ�
					int ky = SaveY[SaveLenght - 1];//�󺧸��ϱ����� ������ �Ǵ� �ȼ� y�� �ϳ�
					SaveLenght--;

					XYOfLabelImg[width * ky + kx] = labelNumber;//������ ������� �󺧸�(���ںο�)

					// ����3 ����3�� �ȼ� �� ���� ������ �������ڷ� �󺧸�
					for (int ny = ky - height * 3 / 7; ny <= ky + height * 3 / 7; ny++){//����3���� ���������� 
						if (ny > 0 && ny < height){//�̹����� �����ȿ�������

							for (int nx = kx - 1; nx <= kx + 1; nx++){//����3���� ���ʿ������� 
								if (nx > 0 && nx < width){//�̹����� �����ȿ�������

									if (inputImage[width * ny + nx] != -1 && XYOfLabelImg[width * ny + nx] == -1) {
										//������ǥ���� �����ϰ�, ��� ��ǥ���� �󺧸��� ���ڰ� ������
										XYOfLabelImg[width * ny + nx] = labelNumber;//�ش��ȼ� �󺧸��Ͽ� ����
										//������ ������ǥ����
										SaveX[SaveLenght] = nx;
										SaveY[SaveLenght] = ny;
										SaveLenght++;//���尹������
									}
								}
							}
						}
					}//for
				}//while
			}//if
		}
	}
	//MaxLabelXY�� ���� ������ ��������
	MaxLabelXY = labelNumber;
	//�Ű����� int*�� �����͸� ����
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){	
			arr[width * y + x] = XYOfLabelImg[src->width * y + x];
		}
	}

	/*
	//����� �ȼ��� Ȯ�ν�
	cvNamedWindow("�󺧸�����", 1);
	cvShowImage("�󺧸�����", src);
	cvWaitKey(0);
	cout << "�󺧸������󺧸������󺧸������󺧸������󺧸������󺧸�����" << endl;; //�����̹����� �ȼ����� ����
	for (int y = 0; y < height; y++){//������ ���̸�ŭ
		for (int x = 0; x < width; x++){//������ �ʺ�ŭ
			cout << (int)arr[width * y + x]; //�����̹����� �ȼ����� ����
		}
		cout << endl;
	}*/

	//�����Ҵ�����
	delete[] inputImage;
	delete[] SaveX;
	delete[] SaveY;
}

int* Labeling::GetLabelingStringXY(int* XYOfLabelxy)
{
	for (int j = 0; j < height * width; j++){
		XYOfLabelImg[j] = -1;
	}//�󺧸��Ǵ� �̹����� ��ǥ �ʱ�ȭ

	int* inputImage = new int[height * width];//�ӽÿ�����ǥ

	for (int y = 0; y < height; y++){//������ ���̸�ŭ
		for (int x = 0; x < width; x++){//������ �ʺ�ŭ
			inputImage[width * y + x] = XYOfLabelxy[width * y + x];//�����̹����� �ȼ����� ����
		}
	}

	int* SaveX = new int[width * height];//�ֺ��� ã�� ������ǥ x�� ����
	int* SaveY = new int[width * height];//�ֺ��� ã�� ������ǥ y�� ����
	for (int i = 0; i < width * height; i++){
		SaveX[i] = 0;
		SaveY[i] = 0;
	}
	int SaveLenght = 0;//��ǥ ���尹��

	int labelNumber = 0;//�ش���ڷ� �󺧸�
	for (int y = 1; y < height - 1; y++){//������ǥ ���̸�ŭ
		for (int x = 1; x < width - 1; x++){//������ǥ �ʺ�ŭ

			if (inputImage[width * y + x] != -1 && XYOfLabelImg[width * y + x] == -1) {
				//������ǥ�� �ȼ����� �����ϰ�, ��� ��ǥ���� �󺧸��� ���ڰ� ������
				labelNumber++;//�󺧸��� ��ȣ�� �� ���ٸ��� ����

				//������ ������ǥ����
				SaveX[SaveLenght] = x;
				SaveY[SaveLenght] = y;
				SaveLenght++;//���尹������

				while (SaveLenght > 0){//������ ��ǥ�� ����������
					int kx = SaveX[SaveLenght - 1];//�󺧸��ϱ����� ������ �Ǵ� �ȼ� x�� �ϳ�
					int ky = SaveY[SaveLenght - 1];//�󺧸��ϱ����� ������ �Ǵ� �ȼ� y�� �ϳ�
					SaveLenght--;

					if (XYOfLabelImg[width * ky + kx] == -1)//�󺧰��� ������
					XYOfLabelImg[width * ky + kx] = labelNumber;//������ ������� �󺧸�(���ںο�)

					// ����1 ����a�� �ȼ� �� ���� ������ �������ڷ� �󺧸�
					for (int ny = ky - 1; ny <= ky + 1; ny++){//����3���� ���������� 
						if (ny > 0 && ny < height){//�̹����� �����ȿ�������

							for (int nx = kx - 1; nx <= kx + 1; nx++){//����3���� ���ʿ������� 
								if (nx > 0 && nx < width){//�̹����� �����ȿ�������

									if (inputImage[width * ny + nx] != -1 && XYOfLabelImg[width * ny + nx] == -1) {
										//������ǥ���� �����ϰ�, ��� ��ǥ���� �󺧸��� ���ڰ� ������
										XYOfLabelImg[width * ny + nx] = labelNumber;//�ش��ȼ� �󺧸��Ͽ� ����
										//������ ������ǥ����
										SaveX[SaveLenght] = nx;
										SaveY[SaveLenght] = ny;
										SaveLenght++;//���尹������
									}
								}
							}
						}
					}//for
				}//while
			}//if
		}
	}

	MaxLabelXY = labelNumber;//�ο��� ���̺������ ����
	delete[] inputImage;
	delete[] SaveX;
	delete[] SaveY;
	return XYOfLabelImg;//�󺧸��� ��ǥ��ȯ
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
	//�����Ҵ� �޸�����
	cvReleaseImage(&src);
	delete[] XYOfLabelImg;
}

































