#pragma once
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;

class Labeling
{
private:
	IplImage *src;//255�̸��� ��ȣ��� �󺧸��� �̹��� 
	int* XYOfLabelImg;//int��ȣ��� ������ �󺧸��� �󺧰��迭
	int MaxLabelXY;//xy��ǥ�� ���� ���̺��ȣ�� ����
	int height;//�󺧸��� ��ǥ ����
	int width;//�󺧸��� ��ǥ �ʺ�
	int widthstep;//�������迭 �̹������� y�� �ϳ�����

public:
	Labeling(IplImage *src);
	Labeling(int height, int width);//���̿� �ʺ�� �ʱ�ȭ

	//����:�󺧸��� ��ǥ�� ���ϰ�, x �������� �Ϻΰ� ���� �����Ѵٸ� �����󺧷� ����
	//���:���� ���μ��� �ִ� ��ǥ���� ���� �������� ���� ����ǥ
	void GetXLabelingXY(int* arr);
	
	//����:�󺧵� ��ǥ�� �ٽ� �󺧸�
	//���:���� �����ȼ��������� ���̺� ��ǥ ����
	int* GetLabelingStringXY(int* XYOfLabelxy);

	//���:xy��ǥ�� ���� ���̺��ȣ�� ���� ��ȯ
	int GetMaxLabelXY();
	
	//���:xy��ǥ�� ũ���ȯ
	int GetSize();
	//���:���̹�ȯ
	int GetHeigth();
	//���:�ʺ��ȯ
	int GetWidth();
	//���:�����ٷ� �Ѿ������ �÷�����
	int GetWidthStep();

	//�Է�:�󺥹�ȣ
	//���:�󺧹�ȣ�� �ش�Ǵ� ������ ��ȯ
	int GetMaxLabelXY(int LabelNUm);

	~Labeling();
};

