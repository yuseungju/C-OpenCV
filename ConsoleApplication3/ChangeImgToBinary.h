#pragma once
#include <cv.h>
#include <highgui.h>

class ChangeImgToBinary
{
public:
	//�Է�: Į���̹���   
	// ��ȯ: ����̹���
	IplImage* ChangeToBlackOrWhite(IplImage* Img);
	
	//����: ���ҹ�Ⱚ������ ���� ������ ���� ����ȭ
	//�Է�: ����̹��� , ���ҿ�����,  ���� 1: ���帹�� ������ �����ϰ� ��� �� / 2: �ι����θ����������� ��
	//��ȯ: ����ȭ�� �̹���
	IplImage* ChangeToBinary(IplImage* Img, int numberOf3, int Area2black);
	
	//�Է�: Į���̹���  , ���ҿ�����
	//��ȯ: ���ȭ and ����ȭ�� �̹���
	IplImage* GetPreProcess(IplImage* Img, int sensitive, int Area2black);
};