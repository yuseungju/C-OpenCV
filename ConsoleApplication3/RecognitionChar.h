#pragma once
#include <cv.h>
#include <highgui.h>
#include "Labeling.h"
#include <iostream>
using namespace std;

class RecognitionChar
{
public:
	RecognitionChar(int* WordLabel, int MaxWordLabel, int height, int width);//��ǥ��, ���̺��ȣ�� ����, ��ǥ��ũ�⸦ �޾ƿ�
	void GetInformation(int*& MaxX, int*& MaxY, int*& MinX, int*& MinY, 
		string*& ReconitionChar, string*& DicMean, int& ShortXYNum);//�νĵȹ����� ������ ��ȯ
	void CalShortXY();//���̺��ش��ϴ� �ܾ��ϳ������� ���̺���ǥ���� ����
	void FromLabelToInformation();//���� �ν��Ͽ� ���������� ȹ��
	string GetAString(int* ShortXY, int SizeX, int SizeY);///�ϳ��� �ܾ �ν��� ���ڿ���ȯ
	string GetAChar(int* CharXY, int CharX, int CharY);///�ϳ��� ���ڸ� �ν��� �����ϳ� ��ȯ
	int NumCrossDotY8(int* CharXY, int SizeX, int SizeY, int y);//y�����Է¹޾� x����� �������� ������
	int NumCrossDotX8(int* CharXY, int SizeX, int SizeY, int x);//x�����Է¹޾� y����� �������� ������
	int NumCrossDotY2(int* CharXY, int SizeX, int SizeY, int y);//y�����Է¹޾� x����� �������� ������
	int NumCrossDotX2(int* CharXY, int SizeX, int SizeY, int x);//x�����Է¹޾� y����� �������� ������
	int NumOfRight(int* CharXY, int SizeX, int SizeY, int StandardX);//����X�κ��� ������ 1�� ���� �ȼ���
	int NumOfLeft(int* CharXY, int SizeX, int SizeY, int StandardX);//����X�κ��� �������� 1�� ���� �ȼ���
	int NumOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY);//����Y�κ��� �Ʒ����� 1�ǰ��� �ȼ���
	int NumOfUp(int* CharXY, int SizeX, int SizeY, int StandardY);//����Y�κ��� ������ 1�ǰ��� �ȼ���
	int RightOfUp(int* CharXY, int SizeX, int SizeY, int StandardY);//����Y�κ��� ������ ��������� x��
	int LeftOfUp(int* CharXY, int SizeX, int SizeY, int StandardY);//����Y�κ��� ������ ������� x��
	int RightOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY);//����Y�κ��� ������ ��������� x��
	int LeftOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY);//����Y�κ��� �Ʒ����� ������� x��

	bool IsAlpha_o(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� o�� ã���� true
	bool IsAlpha_r(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� r�� ã���� true
	bool IsAlpha_l(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� l�� ã���� true

	bool FitWord(string MyWord, string DicWord);//�����ܾ��ϳ��� �νĴܾ��ϳ��� �������ɼ����ִ��� 
	void ConverToDicString(string& RecString, string& DicMean, int* StringLabel);//�ν��� �ܾ �������� ã�ƿϺ��� �ܾ�� ��ȯ�ϰ� �ܾ���� �޾ƿ�
	~RecognitionChar();
private:
	int* WordLabel;//�󺧸��� �̹����� ��ǥ�󺧰���
	int** ShortXY;//���̺��ش��ϴ� �ܾ��ϳ������� ���̺���ǥ��
	int MaxWordLabel;//���� ����(�󺧹�ȣ�� �ִ밪)
	int height;//���̺���ǥ�� ����ũ�⿡ �ش�
	int width;//���̺���ǥ ����ũ�⿡ �ش�
	int ShortXYNum;//�ĺ��� �ܾ��� ����

	//�����ν����� �ν�����
	int* MaxX;//�ν��ϴ� �ܾ��� �ִ� x
	int* MaxY;//�ν��ϴ� �ܾ��� �ִ� y
	int* MinX;//�ν��ϴ� �ܾ��� �ּ� x
	int* MinY;//�ν��ϴ� �ܾ��� �ּ� y
	string* ReconitionChar;//�νĵ� �ܾ��� ����
	string* DicMean;//�νĵ� �ܾ��� ��

};

