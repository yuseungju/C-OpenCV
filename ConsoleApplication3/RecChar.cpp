#include "RecChar.h"
#include <fstream>

RecChar::RecChar()
{
}

string RecChar::GetAString(int* StringXY, int SizeX, int SizeY, int NumLabel)
{
	//�ѱ��ڸ����� �ִ� �ּڰ���ǥ ����� ������ �����ͼ��� �� �ʱ�ȭ
	int* SizeCharX = new int[NumLabel];//�����ϳ��� ũ��
	int* SizeCharY = new int[NumLabel];//�����ϳ��� ũ��
	int* MaxCharX = new int[NumLabel];//�����ϳ��� x�ִ�
	int* MaxCharY = new int[NumLabel];//�����ϳ��� y�ִ�
	int* MinCharX = new int[NumLabel];//�����ϳ��� x�ּڰ�
	int* MinCharY = new int[NumLabel];//�����ϳ��� y�ּڰ�
	int* NumAchar = new int[NumLabel];//�����ϳ��� �󺧰�	
	for (int i = 0; i < NumLabel; i++){
		MaxCharX[i] = -1;
		MaxCharY[i] = -1;
		MinCharX[i] = SizeX;
		MinCharY[i] = SizeY;
		NumAchar[i] = i;
	}

	//�ѱ��ڸ����� �ִ� �ּڰ���ǥ ����� ���
	int LabelNumber = 0;
	for (int y = 0; y < SizeY; y++){//�ܾ���̸�ŭ
		for (int x = 0; x < SizeX; x++){//�ܾ�ʺ�ŭ
			if (StringXY[SizeX * y + x] != -1){//�󺧰��� �����ϸ�
				LabelNumber = StringXY[SizeX * y + x] - 1;//�󺧰�-1�� �ڸ��� LabelNumber����
				if (MaxCharX[LabelNumber] < x) {
					MaxCharX[LabelNumber] = x;
				}
				if (MaxCharY[LabelNumber] < y){
					MaxCharY[LabelNumber] = y;
				}
				if (MinCharX[LabelNumber] > x){
					MinCharX[LabelNumber] = x;
				}
				if (MinCharY[LabelNumber] > y){
					MinCharY[LabelNumber] = y;
				}
			}
		}
	}

	//���ʿ� ��ġ�� ���ں��� �����ι�ġ
	int TempRow;
	for (int i = 0; i < NumLabel - 1; i++){
		for (int j = i + 1; j < NumLabel; j++){
			if
				(MaxCharX[i] > MaxCharX[j]) {
				TempRow = NumAchar[i];
				NumAchar[i] = NumAchar[j];
				NumAchar[j] = TempRow;
				TempRow = MaxCharX[i];
				MaxCharX[i] = MaxCharX[j];
				MaxCharX[j] = TempRow;
				TempRow = MaxCharY[i];
				MaxCharY[i] = MaxCharY[j];
				MaxCharY[j] = TempRow;
				TempRow = MinCharX[i];
				MinCharX[i] = MinCharX[j];
				MinCharX[j] = TempRow;
				TempRow = MinCharY[i];
				MinCharY[i] = MinCharY[j];
				MinCharY[j] = TempRow;
			}
		}
	}

	//�������
	for (int LabelNumber = 0; LabelNumber < NumLabel; LabelNumber++) {
		SizeCharX[LabelNumber] = MaxCharX[LabelNumber] - MinCharX[LabelNumber] + 1;//���õ� �ܾ��ϳ��� �ʺ�
		SizeCharY[LabelNumber] = MaxCharY[LabelNumber] - MinCharY[LabelNumber] + 1;//���õ� �ܾ��ϳ��� ����
	}

	//�����ϳ����� ������ ����ǥ�� ������ ���� �� �ʱ�ȭ
	//CharXY�� �����ϳ������� ���̺���� ����
	int** CharXY = new int*[NumLabel];//�ܾ ���ڼ���ŭ �Ҵ�
	for (int CharRow = 0; CharRow < NumLabel; CharRow++){//�ܾ ���ڼ���ŭ
		CharXY[CharRow] = new int[SizeCharX[CharRow] * SizeCharY[CharRow]];//���ڳ� ���̺����ŭ �Ҵ�
		for (int y = MinCharY[CharRow]; y <= MaxCharY[CharRow]; y++) {//�����ϳ��� ���̸�ŭ
			for (int x = MinCharX[CharRow]; x <= MaxCharX[CharRow]; x++) {//�����ϳ��� �ʺ�ŭ
				if (NumAchar[CharRow] + 1 == StringXY[y * SizeX + x])//�ܾ� ���ڸ��� �����ϳ��� �󺧹�ȣ�� ������
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = 1;//1�� �󺧸�
				else//�ܾ��ϳ��� ���̺� �ڸ��� �󺧹�ȣ�� ������
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = -1;//-1�� �󺧸�
			}
		}
	}

	if (NumLabel == 0)
		return "";
	//���̺� ���� ����� ����
	int average = 0;
	for (int a = 0; a < NumLabel; a++)
		average += SizeCharX[a] * SizeCharY[a];
	average /= NumLabel;

	//���ڷ��̺����� �νĵ� ���ڰ��� RecString �ΰ����´�
	string RecChar;
	string RecString = "";//�ܾ �ν��� ���ڸ� ���� ���ڿ�
	for (int CharRow = 0; CharRow < NumLabel; CharRow++){
		
		if (average /4  < SizeCharX[CharRow] * SizeCharY[CharRow]){//���ڵ� ��� ũ����4���� 1�̻��̸� 
			RecChar = GetAChar(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow]);//���ڰ� �νĵǸ� ����, �ƴϸ� '/' �� ������
			RecString = RecString + RecChar;//���ڿ��� �����ϳ� �߰�
		}
	}
	for (int i = 0; i < NumLabel; i++)
		delete[] CharXY[i];
	delete[] CharXY;
	delete[] SizeCharX;
	delete[] SizeCharY;
	delete[] MaxCharX;
	delete[] MaxCharY;
	delete[] MinCharX;
	delete[] MinCharY;

	return RecString;
}
string RecChar::GetAChar(int* CharXY, int CharX, int CharY)
{
	IplImage* WhiteBlack;
	WhiteBlack = cvCreateImage(cvSize(300, 200), 8, 1);
	for (int a = 0; a < CharY; a++){
		for (int aa = 0; aa < CharX; aa++){
			WhiteBlack->imageData[a*WhiteBlack->widthStep + aa] = CharXY[a *CharX + aa];
		}
	}
	cvMoveWindow("My Window2", 0, 0);
	cvShowImage("My Window2", WhiteBlack);
	cvWaitKey(0);
	if (IsAlpha_o(CharXY, CharX, CharY)){
		//cvNamedWindow("pooo", 1);      // result ��� �̸��� �����츦 �����
		//cvShowImage("pooo", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
		//cvWaitKey(0);
		return "o";
	}
	if (IsAlpha_l(CharXY, CharX, CharY)){
	//	cvNamedWindow("plll", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("plll", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
	    return "l";
	}
	if (IsAlpha_j(CharXY, CharX, CharY)){
	//	cvNamedWindow("jjjjjjj", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("jjjjjjj", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "j";

	}
	if (IsAlpha_i(CharXY, CharX, CharY)){
	//	cvNamedWindow("piiii", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("piiii", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "i";
	}
	if (IsAlpha_q(CharXY, CharX, CharY)){
	//	cvNamedWindow("qqq", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("qqq", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "q";
	}
	if (IsAlpha_p(CharXY, CharX, CharY)){

	//	cvNamedWindow("pp", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("pp", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "p";
	}
	if (IsAlpha_b(CharXY, CharX, CharY)){
	//	cvNamedWindow("bbb", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("bbb", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "b";
	}
	if (IsAlpha_d(CharXY, CharX, CharY)){
		//cvNamedWindow("dd", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("dd", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "d";
	}
	if (IsAlpha_m(CharXY, CharX, CharY)){
	//	cvNamedWindow("mm", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("mm", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "m";
	}

	if (IsAlpha_w(CharXY, CharX, CharY)){//*�����ʿ�
	//	cvNamedWindow("ww", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("ww", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "w";
	}
	if (IsAlpha_r(CharXY, CharX, CharY)){
	//	cvNamedWindow("r", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("r", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "r";
	}
	if (IsAlpha_n(CharXY, CharX, CharY)){
	//	cvNamedWindow("nn", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("nn", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "n";
	}
	if (IsAlpha_u(CharXY, CharX, CharY)){
	//	cvNamedWindow("uu", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("uu", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "u";
	}
	if (IsAlpha_v(CharXY, CharX, CharY)){
	//	cvNamedWindow("vv", 1);      // result ��� �̸��� �����츦 �����
	//	cvShowImage("vv", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	//	cvWaitKey(0);
		return "v";
	}
	if (IsAlpha_c(CharXY, CharX, CharY)){
		//cvNamedWindow("cc", 1);      // result ��� �̸��� �����츦 �����
		//cvShowImage("cc", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
		//cvWaitKey(0);
		return "c";
	}
	if (IsAlpha_g(CharXY, CharX, CharY)){
		//cvNamedWindow("g", 1);      // result ��� �̸��� �����츦 �����
		//cvShowImage("g", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
		//cvWaitKey(0);
		return "g"; 
	}
	if (IsAlpha_k(CharXY, CharX, CharY)){
			cvNamedWindow("KKK", 1);      // result ��� �̸��� �����츦 �����
			cvShowImage("KKK", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
			cvWaitKey(0);
		return "k";
	}
	if (IsAlpha_z(CharXY, CharX, CharY)){
		cvNamedWindow("z", 1);      // result ��� �̸��� �����츦 �����
		cvShowImage("z", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
		cvWaitKey(0);
		return "z";
	}
	return "/";
}
int RecChar::NumCrossgradient(int* CharXY, int SizeX, int SizeY, int x1, int x2, int y1, int y2)
{/*
	int temp;
	if (x1 < x2){
		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if (y1 < y2){
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	int spareX = x2-x1; 
	int spareY = y2-y1;
	int CrossNum = 0;
	for (int y = y1; y < y2; spareY/spareX){
		for (int x = x1; x < x2; x++){
			if (CharXY[y*(SizeX)+x] == 1){
				CrossNum++;
			}
			spareX = x2 - x1;
			spareY = y2 - y1;
			y += spareY / spareX;
		}
	}*/
	return NULL;
}
int RecChar::XOfCrossDotMin(int* CharXY, int SizeX, int SizeY, int Y)
{
	int xMin = -1;
	for (int x = SizeX-1; x >= 0; x--)
	if (CharXY[Y*(SizeX)+x] == 1)
		xMin = x;//������ġ y���� ���� �ִ�������

	return xMin;//������ġ y����ȯ
}
int	RecChar::YOfCrossDotMin(int* CharXY, int SizeX, int SizeY, int x)
{
	int YMin = -1;
	for (int y = SizeY-1; y >= 0; y--)
	if (CharXY[SizeX*y + x] == 1)
		YMin = y;//������ġ y���� ���� �ּҰ�������

	return YMin;//������ġ y����ȯ
}
int RecChar::YOfCrossDotMax(int* CharXY, int SizeX, int SizeY, int x)
{/*
	for (int g = 0; g < SizeY; g++){
		for (int z = 0; z < SizeX; z++){
			cout << (int)CharXY[SizeX*g + z];

		}
		cout << endl;
	}*/
	int yMax = -1;
	for (int y = 0; y < SizeY; y++)
	if (CharXY[SizeX*y + x] == 1)
		yMax = y;//������ġ y���� ���� �ִ�������
	
	return yMax;//������ġ y����ȯ
}
int RecChar::XOfCrossDotMax(int* CharXY, int SizeX, int SizeY, int Y)
{
	int xMax = -1;
	for (int x= 0; x < SizeX; x++)
	if (CharXY[Y*(SizeX) + x] == 1)
		xMax = x;//������ġ y���� ���� �ִ�������
	
	return xMax;//������ġ y����ȯ
}
int RecChar::NumCrossDotX8(int* CharXY, int SizeX, int SizeY, int x)
{
	int CROSS = 0;
	int POS = 1;//���� �����κ��� ������ �ȼ��Ÿ�
	for (int y = 0; y < SizeY; y++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 1) {//���� �ȼ������κ��� 1�̻����� ���� �����ϸ�
			CROSS++;//����y���� �ִ� �ȼ��� �ϳ�����
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0
		}
		else if (CharXY[y*SizeX + x] == -1){//���� ��������������
			POS++;//���� �ȼ��κ��� �Ÿ� ����
		}
		else if (CharXY[y*SizeX + x] == 1){//���� �����ϸ�
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0���� ����
		}
	}

	return CROSS;//������ ��ȯ
}
int RecChar::NumCrossDotY8(int* CharXY, int SizeX, int SizeY, int y)
{
	int CROSS = 0;
	int POS = 1;//���� �����κ��� ������ �ȼ��Ÿ�
	for (int x = 0; x < SizeX; x++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 1) {//���� �ȼ������κ��� 1�̻����� ���� �����ϸ�
			CROSS++;//����y���� �ִ� �ȼ��� �ϳ�����
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0
		}
		else if (CharXY[y*SizeX + x] == -1){//���� ��������������
			POS++;//���� �ȼ��κ��� �Ÿ� ����
		}
		else if (CharXY[y*SizeX + x] == 1){//���� �����ϸ�
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0���� ����
		}
	}

	return CROSS;//������ ��ȯ
}
int RecChar::NumCrossDotX2(int* CharXY, int SizeX, int SizeY, int x)
{
	int CROSS = 0;
	int POS = 2;//���� �����κ��� ������ �ȼ��Ÿ�
	for (int y = 0; y < SizeY; y++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 2) {//���� �ȼ������κ��� 2�̻����� ���� �����ϸ�
			CROSS++;//����y���� �ִ� �ȼ��� �ϳ�����
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0
		}
		else if (CharXY[y*SizeX + x] == -1){//���� ��������������
			POS++;//���� �ȼ��κ��� �Ÿ� ����
		}
		else if (CharXY[y*SizeX + x] == 1){//���� �����ϸ�
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0���� ����
		}
	}

	return CROSS;//������ ��ȯ
}
int RecChar::NumCrossDotY2(int* CharXY, int SizeX, int SizeY, int y)
{
	int CROSS = 0;
	int POS = 2;//���� �����κ��� ������ �ȼ��Ÿ�
	for (int x = 0; x < SizeX; x++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 2) {//���� �ȼ������κ��� 2�̻����� ���� �����ϸ�
			CROSS++;//����y���� �ִ� �ȼ��� �ϳ�����
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0
		}
		else if (CharXY[y*SizeX + x] == -1){//���� ��������������
			POS++;//���� �ȼ��κ��� �Ÿ� ����
		}
		else if (CharXY[y*SizeX + x] == 1){//���� �����ϸ�
			POS = 0;//���� �ȼ��κ��� �Ÿ� 0���� ����
		}
	}

	return CROSS;//������ ��ȯ
}
int RecChar::NumOfRight(int* CharXY, int SizeX, int SizeY, int StandardX)
{
	int NumRight = 1;

	//�������κ��� ������ �ʼ��� 1�� ����
	for (int y = 0; y < SizeY; y++) {
		for (int x = StandardX + 0.5; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumRight++;
			}
		}
	}
	return NumRight;
}
int RecChar::NumOfLeft(int* CharXY, int SizeX, int SizeY, int StandardX)
{
	int NumLeft = 1;
	//�������κ��� ���� �ʼ��� 1�� ����
	for (int y = 0; y < SizeY; y++) {
		for (int x = 0; x < StandardX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumLeft++;
			}
		}
	}
	return NumLeft;
}
int RecChar::NumOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumButtom = 1;
	//�������κ��� �Ʒ��� �ʼ��� 1�� ����
	for (int y = StandardY + 0.5; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumButtom++;
			}
		}
	}
	return NumButtom;
}
int RecChar::NumOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumUp = 1;
	//�������κ��� ���� �ʼ��� 1�� ����
	for (int y = 0; y < StandardY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumUp++;
			}
		}
	}
	return NumUp;
}
int RecChar::RightOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Right = 0;
	//�������κ��� ���� �ʼ����� ����
	for (int y = 0; y < StandardY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Right < x)
					Right = x;
			}
		}
	}
	return Right;//��������� ��ȯ
}
int RecChar::LeftOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Left = SizeX;
	//�������κ��� ���� �ʼ����� ����
	for (int y = 0; y < StandardY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Left > x)
					Left = x;
			}
		}
	}
	return Left;//������� ��ȯ
}
int RecChar::RightOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Right = 0;
	//�������κ��� �Ʒ��� �ʼ����� ����
	for (int y = StandardY + 0.5; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Right < x)
					Right = x;
			}
		}
	}
	return Right;//��������� ��ȯ
}
int RecChar::LeftOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Left = SizeX;
	//�������κ��� �Ʒ��� �ʼ����� ����
	for (int y = StandardY + 0.5; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Left > x)
					Left = x;  
			}
		}  
	}
	return Left;//������� ��ȯ
}
char RecChar::ManyInBottom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumR = 0;
	int NumL = 0;

	//�������κ��� �Ʒ��� �ʼ����� ����
	for (int y = StandardY; y < SizeY; y++) {
		for (int x = 0; x <= SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumL++;
			}
		}
	}
	for (int y = StandardY; y < SizeY; y++) {
		for (int x = SizeX / 2 + 0.5; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumR++;
			}
		}
	}

	if (NumR > NumL)
		return 'R';
	else
		return 'L';
}
char RecChar::ManyInUp(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumR = 0;
	int NumL = 0;

	//�������κ��� ���� �ʼ����� ����
	for (int y = 0; y < StandardY; y++) {
		for (int x = 0; x <= SizeX * 2 / 5; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumL++;
			}
		}
	}
	for (int y = 0; y < StandardY; y++) {
		for (int x = SizeX * 4 / 5; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumR++;
			}
		}
	}

	if (NumR > NumL)
		return 'R';
	else
		return 'L';
}
char RecChar::NotInBottom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumR = 0;
	int NumL = 0;
	//�������κ��� �Ʒ��� �ʼ����� ����
	for (int y = StandardY; y < SizeY; y++) {
		for (int x = 0; x <= SizeX * 2/ 5; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumL++;
			}
		}
	}
	for (int y = StandardY; y < SizeY; y++) {
		for (int x = SizeX *3/ 5; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumR++;
			}
		}
	}
	if (NumR == 0)
		return 'R';
	else if (NumL == 0)
		return 'L';
	else
		return 'X';
}
char RecChar::NotInUp(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumR = 0;
	int NumL = 0;
	//�������κ��� ���� �ʼ����� ����
	for (int y = 0; y < StandardY; y++) {
		for (int x = 0; x <= SizeX * 2 / 5; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumL++;
			}
		}
	}
	for (int y = 0; y < StandardY; y++) {
		for (int x = SizeX * 3 / 5; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumR++;
			}
		}
	}

	if (NumR == 0)
		return 'R';
	else if (NumL == 0)
		return 'L';
	else
		return 'X';
}
bool RecChar::IsAlpha_o(int* CharXY, int SizeX, int SizeY)
{
	//������ 2����1���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 2)
		return false;
	//������ 3����1���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 3) != 2)
		return false;
	//������ 3����2���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY * 2 / 3) != 2)
		return false;
	//�ʺ��� 2����1���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)
		return false;
	//�ʺ��� 2����1���� + �ʺ��� 9����1�� ����x�࿡�� ���� �������մ� ���� ������ 2���ƴϸ� ���� 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 + SizeX / 9) != 2)
		return false;
	//�糡�� �����ʰ� ������ ������ 1.2�̻� �̸�
	if ((NumOfRight(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfLeft(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	if ((NumOfLeft(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfRight(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	//���� ������ �Ʒ����� 1.5�� ũ��
	if (NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) < NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) * 3 / 4)
		return false;
	//�Ʒ��� ������ ������ 1.3�� ũ��
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) > NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) * 1.3)
		return false;
	if (NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 3) == 'L' || NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 3) == 'R')
		return false;
	if (NotInBottom(CharXY, SizeX, SizeY, SizeY * 3 / 4) == 'L' || NotInBottom(CharXY, SizeX, SizeY, SizeY * 3 / 4) == 'R')
		return false;

	return true;
}
bool RecChar::IsAlpha_s(int* CharXY, int SizeX, int SizeY)
{
	//�糡�� �����ʰ� ������ ������ 1.2�̻� �̸�
	if ((NumOfRight(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfLeft(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	if ((NumOfLeft(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfRight(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	//���� ������ �Ʒ����� 1.5�� ũ��
	if (NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) < NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) * 3 / 4)
		return false;
	//�Ʒ��� ������ ������ 1.3�� ũ��
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) > NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) * 1.3)
		return false;
	if (NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 3) == 'L' || NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 3) == 'R')
		return false;
	if (NotInBottom(CharXY, SizeX, SizeY, SizeY * 3 / 4) == 'L' || NotInBottom(CharXY, SizeX, SizeY, SizeY * 3 / 4) == 'R')
		return false;
	return true;
}
bool RecChar::IsAlpha_l(int* CharXY, int SizeX, int SizeY) {

	if (SizeX * 1.5 > SizeY)//���̰� �ʺ��� 2.7�躸�� ũ�����ϸ� l�� �ƴ�
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)//�ʺ��� �ݿ��� ������ ������ �ϳ����ƴϸ� l�� �ƴ�
		return false;
	if (0.9 > NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) / NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) &&
		NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) / NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) > 1.1)//�� �Ʒ� �������� ũ�� l�̾ƴ�
	
	//�پ��ѳ��̸� �������� ������� ������ ������ �Ѱ��� �ƴϸ� l�̾ƴ�
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY *1/ 5) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 2 / 5) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY / 2) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 3 / 5) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 4 / 5) != 1)
		return false;
	
	//������ ���� �ΰ� �̻����� ���������� l�� �ƴ�
	Labeling lb(SizeY, SizeX);
	for (int a = 0; a < SizeY * SizeX; a++)
		CharXY[a] = lb.GetLabelingStringXY(CharXY)[a];
	if (lb.GetMaxLabelXY() > 1)
		return false;

	//���y�࿡���� �������� ����  �ΰ��Ǵ� ������ ���帹���� 
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (three != 0 || four != 0 || two*5 > one)
		return false;

	return true;
}
bool RecChar::IsAlpha_j(int* CharXY, int SizeX, int SizeY)
{

	if (SizeX * 2.7 > SizeY)//���̰� �ʺ��� 2.7�躸�� ũ�����ϸ� l�� �ƴ�
		return false;

	//�������� �Ѱ������̸�
	Labeling lb(SizeY, SizeX);
	for (int a = 0; a < SizeY * SizeX; a++)
		CharXY[a] = lb.GetLabelingStringXY(CharXY)[a];
	if (lb.GetMaxLabelXY() <= 1)
		return false;
	for (int a = 0; a < SizeY; a++){
		for (int s = 0; s < SizeX; s++){
			cout << CharXY[a * SizeX + s];
		}cout << endl;
	}
	//�糡�� �����ʿ� �ȼ����� 1.2�� �� ���������� j�� �ƴ�
	if (NumOfRight(CharXY, SizeX, SizeY, SizeX * 2 / 3) < NumOfLeft(CharXY, SizeX, SizeY, SizeX / 3) *1.2)
		return false;

	return true;
}
bool RecChar::IsAlpha_i(int* CharXY, int SizeX, int SizeY)
{
	if (!( 1.55 < (float)(SizeY * 1.0 / SizeX) && (float)(SizeY * 1.0 / SizeX) <  1.85))//���� ���� ������ i�� ������Ű�����ϸ�
		return false;

	//�������� �Ѱ������̸�
	Labeling lb(SizeY, SizeX);
	for (int a = 0; a < SizeY * SizeX; a++)
		CharXY[a] = lb.GetLabelingStringXY(CharXY)[a];
	if (lb.GetMaxLabelXY() <= 1)
		return false;

	return true;
}
bool RecChar::IsAlpha_r(int* CharXY, int SizeX, int SizeY)
{
	//������ �Ʒ��ʺ��� �������ʿ� ���������� r�� �ƴ�
	if (RightOfUp(CharXY, SizeX, SizeY, SizeY / 2) < RightOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//���� �ȼ� ���� �Ʒ��� ������ ����������  r�� �ƴ�
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) < NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//������ 2����1���� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ�
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY / 2) != 1)
		return false;
	//������ 6����5���� 8����3���� �� ���ζ����� ���� �ϳ��� �ƴѰ��� ������
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 5 / 6) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 4 / 7) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 2 ) != 1)
		return false;

	//�ʺ��� 6���� 5���� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ� 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX * 5/6) != 1)
		return false;
	
	if (SizeX - 1 == YOfCrossDotMax(CharXY, SizeX, SizeY, SizeX -1 ) ||
		(YOfCrossDotMax(CharXY, SizeX, SizeY, SizeX - 1) >  SizeY * 1 / 6) ||
		((CharXY, SizeX, SizeY, 0) >  SizeY * 1 / 5))
		return false;
	
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY * 9 / 10) != 1)
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 1)
		return false;

	//���x�࿡���� �������� ���� �ΰ� �� �����ϸ�
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int x = 0; x < SizeX; x++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, x);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (two * 1.4 > (one + three + four))
		return false;


	return true;
}
bool RecChar::IsAlpha_q(int* CharXY, int SizeX, int SizeY) {
	if (!(1.35 < (SizeY*1.0 / SizeX) && (SizeY*1.0 / SizeX) < 1.77))//���� �ʺ��� ������ q���ƴϸ�
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) < 2)//�ʺ��� �ݿ��� ������ ������ �ΰ��̸��̸� q�� �ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1/3) != 2)//������ 1/3 ���� ������ ������ �ΰ��� �ƴϸ� q���ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1/4) != 2)//������ 1/4 ���� ������ ������ �ΰ��� �ƴϸ� q���ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 7 /8) != 1)//������7/8 ���� ������ ������ �Ѱ����ƴϸ� q�� �ƴ�
		return false;
	if (NotInBottom(CharXY, SizeX, SizeY, SizeY * 7 / 8) != 'L')//�Ʒ��� �κп��� ���ʺκп��ȼ��� �����ϸ� q���ƴ�
		return false;
	return true;
}
bool RecChar::IsAlpha_p(int* CharXY, int SizeX, int SizeY) {
	if (!(1.35 < (SizeY*1.0 / SizeX) && (SizeY*1.0 / SizeX) < 1.77))//���� �ʺ��� ������ p���ƴϸ�
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2)  < 2)//�ʺ��� �ݿ��� ������ ������ �ΰ��̸��̸� p�� �ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 3) != 2)//������ 1/3 ���� ������ ������ �ΰ��� �ƴϸ� p���ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 4) != 2)//������ 1/4 ���� ������ ������ �ΰ��� �ƴϸ� p���ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 7 / 8) != 1)//������7/8 ���� ������ ������ �Ѱ����ƴϸ� p�� �ƴ�
		return false;
	if (NotInBottom(CharXY, SizeX, SizeY, SizeY * 7 / 8) != 'R')//�Ʒ��� �κп��� ������ �κп��ȼ��� �����ϸ� p���ƴ�
		return false;
	return true;
}
bool RecChar::IsAlpha_b(int* CharXY, int SizeX, int SizeY) {
	if (!(1.45 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.61))//���̰� �ʺ��Ǻ����� b�� �����������ϸ� b �ƴ�
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)//�ʺ��� �ݿ��� ������ ������ �ΰ��� �ƴϸ� b �ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 8) != 1)//������ 1/8 ���� ������ ������ �Ѱ��� �ƴϸ� b���ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 3 / 4) != 2)//������ 3/4 ���� ������ ������ �ΰ��� �ƴϸ� b���ƴ�
		return false;
	if (NotInUp(CharXY, SizeX, SizeY, SizeY * 1/5) != 'R')//���� �κп��� ������ �κп��ȼ��� �����ϸ� b���ƴ�
		return false;
	if (NumOfLeft(CharXY, SizeX, SizeY, SizeY / 2) > NumOfRight(CharXY, SizeX, SizeY, SizeY / 2)*5)//���� �κп��� ������ �κп��ȼ��� �����ϸ� b���ƴ�
		return false;

	return true;
}
bool RecChar::IsAlpha_k(int* CharXY, int SizeX, int SizeY) {
	if (!(1 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.9))//���̰� �ʺ��Ǻ����� b�� �����������ϸ� b �ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 8) != 1)//������ 1/8 ���� ������ ������ �Ѱ��� �ƴϸ� b���ƴ�
		return false;
	if (NumOfLeft(CharXY, SizeX, SizeY, SizeY / 2) < NumOfRight(CharXY, SizeX, SizeY, SizeY / 2) * 3)//���� �κп��� ������ �κп��ȼ��� �����ϸ� b���ƴ�
		return false;

	return true;
}
bool RecChar::IsAlpha_d(int* CharXY, int SizeX, int SizeY) {
	if (!(1.45 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.61))//���̰� �ʺ��Ǻ����� d�� �����������ϸ� d �ƴ�
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)//�ʺ��� �ݿ��� ������ ������ �ΰ��� �ƴϸ� d �ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 8) != 1)//������ 1/8 ���� ������ ������ �Ѱ��� �ƴϸ� d���ƴ�
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 3 / 4) != 2)//������ 3/4 ���� ������ ������ �ΰ��� �ƴϸ� d���ƴ�
		return false;
	if (NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 5) != 'L')//���� �κп��� ������ �κп��ȼ��� �����ϸ� d���ƴ�
		return false;
	return true;
}
bool RecChar::IsAlpha_m(int* CharXY, int SizeX, int SizeY)
{
	if (!(0.4< SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.1))//���̰� �ʺ��Ǻ����� m�� �����������ϸ� m �ƴ�
		return false;
	//���y�࿡���� �������� ���� 3���� �ξ� ����������
	int one = 0; 
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (three < ( one + two + four)*2)
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)//�ʺ��� �ݿ��� ������ ������ �Ѱ��� �ƴϸ� w�ƴ�
		return false;
	return true;
}
bool RecChar::IsAlpha_w(int* CharXY, int SizeX, int SizeY)
{
	if (!(0.60< SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.1))//���̰� �ʺ��Ǻ����� m�� �����������ϸ� m �ƴ�
		return false;

	//���y�࿡���� �������� ����  �ΰ��Ǵ� ������ ���帹���� 
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (three < one || two < one  || three< two/4)
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)//�ʺ��� �ݿ��� ������ ������ �Ѱ��� �ƴϸ� w�ƴ�
		return false;
	return true;
}
bool RecChar::IsAlpha_n(int* CharXY, int SizeX, int SizeY)
{
	//���y�࿡���� �������� ����  �ΰ��Ǵ� ������ ������
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (two < one * 2 || two < three * 3)
		return false;
	if (YOfCrossDotMax(CharXY, SizeX, SizeY, SizeX/2) > SizeY * 1/8)
		return false;
	if (YOfCrossDotMin(CharXY, SizeX, SizeY, SizeX / 2) > SizeY * 1 /6)
		return false;

	//���x�࿡���� �������� ����  �Ѱ��� ���帹�������� 
	one = 0;
	two = 0;
	three = 0;
	four = 0;
	temp = 0;
	for (int x = 0; x < SizeX; x++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, x);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (two < (one + three + four)* 1.7)
		return false;
	return true;
}
bool RecChar::IsAlpha_u(int* CharXY, int SizeX, int SizeY)
{
	//���y�࿡���� �������� ����  �ΰ��Ǵ� ������ ���帹���� 
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (two < one * 2 || two < three * 3)
		return false;
	if (YOfCrossDotMax(CharXY, SizeX, SizeY, SizeX / 2) < SizeY * 5/6)
		return false;
	if (YOfCrossDotMin(CharXY, SizeX, SizeY, SizeX / 2) < SizeY / 2)
		return false;
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) >  NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
}
bool RecChar::IsAlpha_v(int* CharXY, int SizeX, int SizeY)
{
	//���y�࿡���� �������� ����  �ΰ��Ǵ� ������ ���帹���� 
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (two < one || two < three * 3)
		return false;

	if (YOfCrossDotMax(CharXY, SizeX, SizeY, SizeX / 2) < SizeY * 5 / 6)
		return false;
	if (YOfCrossDotMin(CharXY, SizeX, SizeY, SizeX / 2) < SizeY / 2)
		return false;

}
bool RecChar::IsAlpha_c(int* CharXY, int SizeX, int SizeY)
{
	//���y�࿡���� �������� ����  �Ѱ��� ���帹���� 
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (one * 1.0 / two < 0.8  || one < three *2.2 || one < four *2.2)
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)//�ʺ��� �ݿ��� ������ ������ �ΰ��� �ƴϸ� c�ƴ�
		return false;

	//���X�࿡���� �������� ����  �ΰ������帹���� 
	one = 0;
	two = 0;
	three = 0;
	four = 0;
	temp = 0;
	for (int x = 0; x < SizeY; x++){
		temp = NumCrossDotX2(CharXY, SizeX, SizeY, x);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (one > two || one < three *3 || one < four * 3) 
		return false;

	if (NumOfButtom(CharXY, SizeX, SizeY, SizeX / 2)  * 1.0 / NumOfUp(CharXY, SizeX, SizeY, SizeX / 2)< 0.9 ||
		NumOfButtom(CharXY, SizeX, SizeY, SizeX / 2) * 1.0 / NumOfUp(CharXY, SizeX, SizeY, SizeX / 2)  > 1.6)
		return false;


	return true;
}
bool RecChar::IsAlpha_g(int* CharXY, int SizeX, int SizeY)
{//���y�࿡���� �������� ����  �ΰ��Ǵ� ������ ������
	if (!(1.45 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.61))//���̰� �ʺ��Ǻ����� d�� �����������ϸ� d �ƴ�
		return false;

	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (YOfCrossDotMax(CharXY, SizeX, SizeY, SizeX / 2) > SizeY * 1 / 8)
		return false;
	if (YOfCrossDotMin(CharXY, SizeX, SizeY, SizeX / 2) > SizeY * 1 / 6)
		return false;

	//���x�࿡���� �������� ���� 4���������� g���ƴ�
	one = 0;
	two = 0;
	three = 0;
	four = 0;
	temp = 0;
	for (int X = 0; X < SizeX; X++){
		temp = NumCrossDotX8(CharXY, SizeX, SizeY, X);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (four == 0 && three == 0)
		return false;

	if (XOfCrossDotMin(CharXY, SizeX, SizeY, SizeY / 2) > SizeX* 0.2)
		return false;
	if (YOfCrossDotMin(CharXY, SizeX, SizeY, SizeY / 2) > SizeY* 0.2)
		return false;
	if (YOfCrossDotMax(CharXY, SizeX, SizeY, SizeY / 2) < SizeY* 0.8)
		return false;
	if (SizeX * 1 / 3 > XOfCrossDotMin(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	if (SizeX * 2 / 3 < XOfCrossDotMax(CharXY, SizeX, SizeY, SizeY / 2))
		return false;


	return true;
}
bool RecChar::IsAlpha_f(int* CharXY, int SizeX, int SizeY)
{
	//���y�࿡���� �������� ����  �Ѱ��� ���帹���� 
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (one < two *2.2 || one < three *2.2 || one < four *2.2)
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 1)//������ �ݿ��� ������ ������ �ΰ��� �ƴϸ� f�ƴ�
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 3) != 1)//�������� 1/3���� ������ ������ �ΰ��� �ƴϸ� f�ƴ�
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY * 2 / 3) != 1)//������ 2/3���� ������ ������ �ΰ��� �ƴϸ� f�ƴ�
		return false;
	if (SizeX * 1.1 > SizeY)//�ʺ� ���̿����� ������� ũ��
		return false;

	return true;
}
bool RecChar::IsAlpha_z(int* CharXY, int SizeX, int SizeY)
{
	//���y�࿡���� �������� ����  �Ѱ��� ���帹���� 
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int temp = 0;
	for (int y = 0; y < SizeY; y++){
		temp = NumCrossDotY2(CharXY, SizeX, SizeY, y);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (one < two *3.2 || one < three *3.2 || one < four *3.2)
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 1)//������ �ݿ��� ������ ������ �ΰ��� �ƴϸ� f�ƴ�
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 3) != 1)//�������� 1/3���� ������ ������ �ΰ��� �ƴϸ� f�ƴ�
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY * 2 / 3) != 1)//������ 2/3���� ������ ������ �ΰ��� �ƴϸ� f�ƴ�
		return false;


	//���x�࿡���� �������� ���� 4���������� g���ƴ�
	one = 0;
	two = 0;
	three = 0;
	four = 0;
	temp = 0;
	for (int x = 0; x < SizeX; x++){
		temp = NumCrossDotX8(CharXY, SizeX, SizeY, x);
		if (temp == 1)
			one++;
		if (temp == 2)
			two++;
		if (temp == 3)
			three++;
		if (temp == 4)
			four++;
	}
	if (two <one || three <one)
		return false;

	/*if (NumCrossgradient(CharXY, SizeX, SizeY, 0, SizeX, 0, SizeY) < NumCrossgradient(CharXY, SizeX, 0, 0, SizeX, 0, SizeY))
		return false;*/

	return true;
}
RecChar::~RecChar()
{
}
