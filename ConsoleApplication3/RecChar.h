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
	string GetAString(int* ShortXY, int SizeX, int SizeY, int NumLabel);///�ϳ��� �ܾ �ν��� ���ڿ���ȯ
	string GetAChar(int* CharXY, int CharX, int CharY);///�ϳ��� ���ڸ� �ν��� �����ϳ� ��ȯ
	int NumCrossgradient(int* CharXY, int SizeX, int SizeY, int x1, int x2, int y1, int y2);//ó������ �ι������� �ִ� ����� ������
	int XOfCrossDotMin(int* CharXY, int SizeX, int SizeY, int Y);////y���� �������� ������ �ּ�x���� ��ȯ
	int	YOfCrossDotMin(int* CharXY, int SizeX, int SizeY, int x);//x���� �������� ������ �ּڰ�����ȯ
	int XOfCrossDotMax(int* CharXY, int SizeX, int SizeY, int Y);////y���� �������� ������ �ִ�x���� ��ȯ
	int	YOfCrossDotMax(int* CharXY, int SizeX, int SizeY, int x);//x���� �������� ������ �ִ�����ȯ
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
	char ManyInBottom(int* CharXY, int SizeX, int SizeY, int StandardY);//����y�κ��� �Ʒ����� ���帹�� �ִ� ��('R' �Ǵ�'L') 
	char ManyInUp(int* CharXY, int SizeX, int SizeY, int StandardY);//����y�κ��� ������ ���帹�� �ִ� ��('R' �Ǵ�'L')
	char NotInBottom(int* CharXY, int SizeX, int SizeY, int StandardY);//����y�κ��� �Ʒ����� �ȼ����� ���� ��('R' �Ǵ�'L') 
	char NotInUp(int* CharXY, int SizeX, int SizeY, int StandardY);//����y�κ��� ������ �ȼ����� ���� ��('R' �Ǵ�'L')
	
	bool IsAlpha_a(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� a�� ã���� true
	bool IsAlpha_b(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� b�� ã���� true
	bool IsAlpha_c(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� c�� ã���� true
	bool IsAlpha_d(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� d�� ã���� true
	bool IsAlpha_e(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� e�� ã���� true
	bool IsAlpha_f(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� f�� ã���� true
	bool IsAlpha_g(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� g�� ã���� true
	bool IsAlpha_h(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� h�� ã���� true
	bool IsAlpha_i(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� l�� ã���� true
	bool IsAlpha_j(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� j�� ã���� true
	bool IsAlpha_k(int* CharXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� k�� ã���� true
	bool IsAlpha_l(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� l�� ã���� true
	bool IsAlpha_m(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� m�� ã���� true
	bool IsAlpha_n(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� n�� ã���� true
	bool IsAlpha_o(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� o�� ã���� true
	bool IsAlpha_p(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� p�� ã���� true
	bool IsAlpha_q(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� q�� ã���� true
	bool IsAlpha_r(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� r�� ã���� true
	bool IsAlpha_s(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� s�� ã���� true
	bool IsAlpha_t(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� t�� ã���� true
	bool IsAlpha_u(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� u�� ã���� true
	bool IsAlpha_v(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� v�� ã���� true
	bool IsAlpha_w(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� w�� ã���� true
	bool IsAlpha_x(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� x�� ã���� true
	bool IsAlpha_y(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� y�� ã���� true
	bool IsAlpha_z(int* charXY, int SizeX, int SizeY);//������ �ܾ�� �� �պκп��� z�� ã���� true

	~RecChar();

private:
	string ReconitionChar;//�νĵ� �ܾ��� ����
	string DicMean;//�νĵ� �ܾ��� ��
};

