#include "RecChar.h"
#include <fstream>

RecChar::RecChar()
{
}

string RecChar::GetAString(int* StringXY, int SizeX, int SizeY, int NumLabel)
{
	//한글자마다의 최댓값 최솟값좌표 사이즈를 가지는 포인터선언 및 초기화
	int* SizeCharX = new int[NumLabel];//글자하나의 크기
	int* SizeCharY = new int[NumLabel];//글자하나의 크기
	int* MaxCharX = new int[NumLabel];//글자하나의 x최댓값
	int* MaxCharY = new int[NumLabel];//글자하나의 y최댓값
	int* MinCharX = new int[NumLabel];//글자하나의 x최솟값
	int* MinCharY = new int[NumLabel];//글자하나의 y최솟값
	int* NumAchar = new int[NumLabel];//글자하나의 라벨값	
	for (int i = 0; i < NumLabel; i++){
		MaxCharX[i] = -1;
		MaxCharY[i] = -1;
		MinCharX[i] = SizeX;
		MinCharY[i] = SizeY;
		NumAchar[i] = i;
	}

	//한글자마다의 최댓값 최솟값좌표 사이즈를 계산
	int LabelNumber = 0;
	for (int y = 0; y < SizeY; y++){//단어높이만큼
		for (int x = 0; x < SizeX; x++){//단어너비만큼
			if (StringXY[SizeX * y + x] != -1){//라벨값이 존재하면
				LabelNumber = StringXY[SizeX * y + x] - 1;//라벨값-1의 자리로 LabelNumber설정
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

	//왼쪽에 배치한 글자부터 앞으로배치
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

	//사이즈구함
	for (int LabelNumber = 0; LabelNumber < NumLabel; LabelNumber++) {
		SizeCharX[LabelNumber] = MaxCharX[LabelNumber] - MinCharX[LabelNumber] + 1;//선택된 단어하나의 너비
		SizeCharY[LabelNumber] = MaxCharY[LabelNumber] - MinCharY[LabelNumber] + 1;//선택된 단어하나의 높이
	}

	//글자하나마다 가지는 라벨좌표값 포인터 선언 및 초기화
	//CharXY에 글자하나마다의 레이블들을 담음
	int** CharXY = new int*[NumLabel];//단어내 글자수만큼 할당
	for (int CharRow = 0; CharRow < NumLabel; CharRow++){//단어내 글자수만큼
		CharXY[CharRow] = new int[SizeCharX[CharRow] * SizeCharY[CharRow]];//글자내 레이블수만큼 할당
		for (int y = MinCharY[CharRow]; y <= MaxCharY[CharRow]; y++) {//글자하나의 높이만큼
			for (int x = MinCharX[CharRow]; x <= MaxCharX[CharRow]; x++) {//글자하나의 너비만큼
				if (NumAchar[CharRow] + 1 == StringXY[y * SizeX + x])//단어 라벨자리에 글자하나의 라벨번호가 있으면
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = 1;//1로 라벨링
				else//단어하나의 레이블 자리에 라벨번호가 없으면
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = -1;//-1로 라벨링
			}
		}
	}

	if (NumLabel == 0)
		return "";
	//레이블 수의 평균을 산출
	int average = 0;
	for (int a = 0; a < NumLabel; a++)
		average += SizeCharX[a] * SizeCharY[a];
	average /= NumLabel;

	//글자레이블마다의 인식된 문자값을 RecString 로가져온다
	string RecChar;
	string RecString = "";//단어내 인식한 글자를 담을 문자열
	for (int CharRow = 0; CharRow < NumLabel; CharRow++){
		
		if (average /4  < SizeCharX[CharRow] * SizeCharY[CharRow]){//글자들 평균 크기의4분의 1이상이면 
			RecChar = GetAChar(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow]);//문자가 인식되면 문자, 아니면 '/' 를 가져옴
			RecString = RecString + RecChar;//문자열에 문자하나 추가
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
		//cvNamedWindow("pooo", 1);      // result 라는 이름의 윈도우를 만들고
		//cvShowImage("pooo", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
		//cvWaitKey(0);
		return "o";
	}
	if (IsAlpha_l(CharXY, CharX, CharY)){
	//	cvNamedWindow("plll", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("plll", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
	    return "l";
	}
	if (IsAlpha_j(CharXY, CharX, CharY)){
	//	cvNamedWindow("jjjjjjj", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("jjjjjjj", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "j";

	}
	if (IsAlpha_i(CharXY, CharX, CharY)){
	//	cvNamedWindow("piiii", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("piiii", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "i";
	}
	if (IsAlpha_q(CharXY, CharX, CharY)){
	//	cvNamedWindow("qqq", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("qqq", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "q";
	}
	if (IsAlpha_p(CharXY, CharX, CharY)){

	//	cvNamedWindow("pp", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("pp", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "p";
	}
	if (IsAlpha_b(CharXY, CharX, CharY)){
	//	cvNamedWindow("bbb", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("bbb", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "b";
	}
	if (IsAlpha_d(CharXY, CharX, CharY)){
		//cvNamedWindow("dd", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("dd", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "d";
	}
	if (IsAlpha_m(CharXY, CharX, CharY)){
	//	cvNamedWindow("mm", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("mm", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "m";
	}

	if (IsAlpha_w(CharXY, CharX, CharY)){//*보완필요
	//	cvNamedWindow("ww", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("ww", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "w";
	}
	if (IsAlpha_r(CharXY, CharX, CharY)){
	//	cvNamedWindow("r", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("r", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "r";
	}
	if (IsAlpha_n(CharXY, CharX, CharY)){
	//	cvNamedWindow("nn", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("nn", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "n";
	}
	if (IsAlpha_u(CharXY, CharX, CharY)){
	//	cvNamedWindow("uu", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("uu", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "u";
	}
	if (IsAlpha_v(CharXY, CharX, CharY)){
	//	cvNamedWindow("vv", 1);      // result 라는 이름의 윈도우를 만들고
	//	cvShowImage("vv", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	//	cvWaitKey(0);
		return "v";
	}
	if (IsAlpha_c(CharXY, CharX, CharY)){
		//cvNamedWindow("cc", 1);      // result 라는 이름의 윈도우를 만들고
		//cvShowImage("cc", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
		//cvWaitKey(0);
		return "c";
	}
	if (IsAlpha_g(CharXY, CharX, CharY)){
		//cvNamedWindow("g", 1);      // result 라는 이름의 윈도우를 만들고
		//cvShowImage("g", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
		//cvWaitKey(0);
		return "g"; 
	}
	if (IsAlpha_k(CharXY, CharX, CharY)){
			cvNamedWindow("KKK", 1);      // result 라는 이름의 윈도우를 만들고
			cvShowImage("KKK", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
			cvWaitKey(0);
		return "k";
	}
	if (IsAlpha_z(CharXY, CharX, CharY)){
		cvNamedWindow("z", 1);      // result 라는 이름의 윈도우를 만들고
		cvShowImage("z", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
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
		xMin = x;//교점위치 y값중 가장 최댓값을얻음

	return xMin;//교점위치 y값반환
}
int	RecChar::YOfCrossDotMin(int* CharXY, int SizeX, int SizeY, int x)
{
	int YMin = -1;
	for (int y = SizeY-1; y >= 0; y--)
	if (CharXY[SizeX*y + x] == 1)
		YMin = y;//교점위치 y값중 가장 최소값을얻음

	return YMin;//교점위치 y값반환
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
		yMax = y;//교점위치 y값중 가장 최댓값을얻음
	
	return yMax;//교점위치 y값반환
}
int RecChar::XOfCrossDotMax(int* CharXY, int SizeX, int SizeY, int Y)
{
	int xMax = -1;
	for (int x= 0; x < SizeX; x++)
	if (CharXY[Y*(SizeX) + x] == 1)
		xMax = x;//교점위치 y값중 가장 최댓값을얻음
	
	return xMax;//교점위치 y값반환
}
int RecChar::NumCrossDotX8(int* CharXY, int SizeX, int SizeY, int x)
{
	int CROSS = 0;
	int POS = 1;//셈한 값으로부터 떨어진 픽셀거리
	for (int y = 0; y < SizeY; y++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 1) {//셈한 픽셀값으로부터 1이상벗어났고 값이 존재하면
			CROSS++;//같은y값에 있는 픽셀수 하나증가
			POS = 0;//셈한 픽셀로부터 거리 0
		}
		else if (CharXY[y*SizeX + x] == -1){//값이 존재하지않으면
			POS++;//셈한 픽셀로부터 거리 증가
		}
		else if (CharXY[y*SizeX + x] == 1){//값이 존재하면
			POS = 0;//셈한 픽셀로부터 거리 0으로 설정
		}
	}

	return CROSS;//교점수 반환
}
int RecChar::NumCrossDotY8(int* CharXY, int SizeX, int SizeY, int y)
{
	int CROSS = 0;
	int POS = 1;//셈한 값으로부터 떨어진 픽셀거리
	for (int x = 0; x < SizeX; x++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 1) {//셈한 픽셀값으로부터 1이상벗어났고 값이 존재하면
			CROSS++;//같은y값에 있는 픽셀수 하나증가
			POS = 0;//셈한 픽셀로부터 거리 0
		}
		else if (CharXY[y*SizeX + x] == -1){//값이 존재하지않으면
			POS++;//셈한 픽셀로부터 거리 증가
		}
		else if (CharXY[y*SizeX + x] == 1){//값이 존재하면
			POS = 0;//셈한 픽셀로부터 거리 0으로 설정
		}
	}

	return CROSS;//교점수 반환
}
int RecChar::NumCrossDotX2(int* CharXY, int SizeX, int SizeY, int x)
{
	int CROSS = 0;
	int POS = 2;//셈한 값으로부터 떨어진 픽셀거리
	for (int y = 0; y < SizeY; y++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 2) {//셈한 픽셀값으로부터 2이상벗어났고 값이 존재하면
			CROSS++;//같은y값에 있는 픽셀수 하나증가
			POS = 0;//셈한 픽셀로부터 거리 0
		}
		else if (CharXY[y*SizeX + x] == -1){//값이 존재하지않으면
			POS++;//셈한 픽셀로부터 거리 증가
		}
		else if (CharXY[y*SizeX + x] == 1){//값이 존재하면
			POS = 0;//셈한 픽셀로부터 거리 0으로 설정
		}
	}

	return CROSS;//교점수 반환
}
int RecChar::NumCrossDotY2(int* CharXY, int SizeX, int SizeY, int y)
{
	int CROSS = 0;
	int POS = 2;//셈한 값으로부터 떨어진 픽셀거리
	for (int x = 0; x < SizeX; x++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 2) {//셈한 픽셀값으로부터 2이상벗어났고 값이 존재하면
			CROSS++;//같은y값에 있는 픽셀수 하나증가
			POS = 0;//셈한 픽셀로부터 거리 0
		}
		else if (CharXY[y*SizeX + x] == -1){//값이 존재하지않으면
			POS++;//셈한 픽셀로부터 거리 증가
		}
		else if (CharXY[y*SizeX + x] == 1){//값이 존재하면
			POS = 0;//셈한 픽셀로부터 거리 0으로 설정
		}
	}

	return CROSS;//교점수 반환
}
int RecChar::NumOfRight(int* CharXY, int SizeX, int SizeY, int StandardX)
{
	int NumRight = 1;

	//기준으로부터 오른쪽 필셀값 1을 조사
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
	//기준으로부터 왼쪽 필셀값 1을 조사
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
	//기준으로부터 아래쪽 필셀값 1을 조사
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
	//기준으로부터 위쪽 필셀값 1을 조사
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
	//기준으로부터 위쪽 필셀값을 조사
	for (int y = 0; y < StandardY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Right < x)
					Right = x;
			}
		}
	}
	return Right;//가장오른쪽 반환
}
int RecChar::LeftOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Left = SizeX;
	//기준으로부터 위쪽 필셀값을 조사
	for (int y = 0; y < StandardY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Left > x)
					Left = x;
			}
		}
	}
	return Left;//가장왼쪽 반환
}
int RecChar::RightOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Right = 0;
	//기준으로부터 아래쪽 필셀값을 조사
	for (int y = StandardY + 0.5; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Right < x)
					Right = x;
			}
		}
	}
	return Right;//가장오른쪽 반환
}
int RecChar::LeftOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Left = SizeX;
	//기준으로부터 아래쪽 필셀값을 조사
	for (int y = StandardY + 0.5; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Left > x)
					Left = x;  
			}
		}  
	}
	return Left;//가장왼쪽 반환
}
char RecChar::ManyInBottom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumR = 0;
	int NumL = 0;

	//기준으로부터 아래쪽 필셀값을 조사
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

	//기준으로부터 위쪽 필셀값을 조사
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
	//기준으로부터 아래쪽 필셀값을 조사
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
	//기준으로부터 위쪽 필셀값을 조사
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
	//높이의 2분의1에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 2)
		return false;
	//높이의 3분의1에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 3) != 2)
		return false;
	//높이의 3분의2에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY * 2 / 3) != 2)
		return false;
	//너비의 2분의1에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)
		return false;
	//너비의 2분의1에서 + 너비의 9분의1을 더한x축에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 + SizeX / 9) != 2)
		return false;
	//양끝중 오른쪽과 왼쪽의 비율이 1.2이상 이면
	if ((NumOfRight(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfLeft(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	if ((NumOfLeft(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfRight(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	//위의 비율이 아래보다 1.5배 크면
	if (NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) < NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) * 3 / 4)
		return false;
	//아래의 비율이 위보다 1.3배 크면
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
	//양끝중 오른쪽과 왼쪽의 비율이 1.2이상 이면
	if ((NumOfRight(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfLeft(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	if ((NumOfLeft(CharXY, SizeX, SizeY, SizeX / 4)) / (NumOfRight(CharXY, SizeX, SizeY, SizeX * 3 / 4)) > 1.15)
		return false;
	//위의 비율이 아래보다 1.5배 크면
	if (NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) < NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) * 3 / 4)
		return false;
	//아래의 비율이 위보다 1.3배 크면
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) > NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) * 1.3)
		return false;
	if (NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 3) == 'L' || NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 3) == 'R')
		return false;
	if (NotInBottom(CharXY, SizeX, SizeY, SizeY * 3 / 4) == 'L' || NotInBottom(CharXY, SizeX, SizeY, SizeY * 3 / 4) == 'R')
		return false;
	return true;
}
bool RecChar::IsAlpha_l(int* CharXY, int SizeX, int SizeY) {

	if (SizeX * 1.5 > SizeY)//높이가 너비의 2.7배보다 크지못하면 l이 아님
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)//너비의 반에서 만나는 지점이 하나가아니면 l이 아님
		return false;
	if (0.9 > NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) / NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) &&
		NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2) / NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) > 1.1)//위 아래 비율차가 크면 l이아님
	
	//다양한높이를 기준으로 가로축과 만나는 지점이 한개가 아니면 l이아님
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
	
	//영역의 수가 두개 이상으로 나누어지면 l이 아님
	Labeling lb(SizeY, SizeX);
	for (int a = 0; a < SizeY * SizeX; a++)
		CharXY[a] = lb.GetLabelingStringXY(CharXY)[a];
	if (lb.GetMaxLabelXY() > 1)
		return false;

	//모든y축에서의 교점들의 수중  두개또는 세개가 가장많으면 
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

	if (SizeX * 2.7 > SizeY)//높이가 너비의 2.7배보다 크지못하면 l이 아님
		return false;

	//영역수가 한개이하이면
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
	//양끝중 오른쪽에 픽셀값이 1.2배 더 많지않으면 j가 아님
	if (NumOfRight(CharXY, SizeX, SizeY, SizeX * 2 / 3) < NumOfLeft(CharXY, SizeX, SizeY, SizeX / 3) *1.2)
		return false;

	return true;
}
bool RecChar::IsAlpha_i(int* CharXY, int SizeX, int SizeY)
{
	if (!( 1.55 < (float)(SizeY * 1.0 / SizeX) && (float)(SizeY * 1.0 / SizeX) <  1.85))//가로 세로 비율이 i를 만족시키지못하면
		return false;

	//영역수가 한개이하이면
	Labeling lb(SizeY, SizeX);
	for (int a = 0; a < SizeY * SizeX; a++)
		CharXY[a] = lb.GetLabelingStringXY(CharXY)[a];
	if (lb.GetMaxLabelXY() <= 1)
		return false;

	return true;
}
bool RecChar::IsAlpha_r(int* CharXY, int SizeX, int SizeY)
{
	//위쪽이 아래쪽보다 더오른쪽에 있지않으면 r이 아님
	if (RightOfUp(CharXY, SizeX, SizeY, SizeY / 2) < RightOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//위쪽 픽셀 값이 아래쪽 값보다 많지않으면  r이 아님
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) < NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//높이의 2분의1에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY / 2) != 1)
		return false;
	//높이의 6분의5에서 8분의3까지 중 서로떨어진 값이 하나가 아닌것이 있으면
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 5 / 6) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 4 / 7) != 1)
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 2 ) != 1)
		return false;

	//너비의 6분의 5에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님 
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

	//모든x축에서의 교점들의 수중 두개 가 존재하면
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
	if (!(1.35 < (SizeY*1.0 / SizeX) && (SizeY*1.0 / SizeX) < 1.77))//높이 너비의 비율이 q가아니면
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) < 2)//너비의 반에서 만나는 지점이 두개미만이면 q이 아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1/3) != 2)//높이의 1/3 에서 만나는 지점이 두개가 아니면 q가아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1/4) != 2)//높이의 1/4 에서 만나는 지점이 두개가 아니면 q가아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 7 /8) != 1)//높이의7/8 에서 만나는 지점이 한개가아니면 q가 아님
		return false;
	if (NotInBottom(CharXY, SizeX, SizeY, SizeY * 7 / 8) != 'L')//아래쪽 부분에서 왼쪽부분에픽셀이 존재하면 q가아님
		return false;
	return true;
}
bool RecChar::IsAlpha_p(int* CharXY, int SizeX, int SizeY) {
	if (!(1.35 < (SizeY*1.0 / SizeX) && (SizeY*1.0 / SizeX) < 1.77))//높이 너비의 비율이 p가아니면
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2)  < 2)//너비의 반에서 만나는 지점이 두개미만이면 p이 아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 3) != 2)//높이의 1/3 에서 만나는 지점이 두개가 아니면 p가아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 4) != 2)//높이의 1/4 에서 만나는 지점이 두개가 아니면 p가아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 7 / 8) != 1)//높이의7/8 에서 만나는 지점이 한개가아니면 p가 아님
		return false;
	if (NotInBottom(CharXY, SizeX, SizeY, SizeY * 7 / 8) != 'R')//아래쪽 부분에서 오른쪽 부분에픽셀이 존재하면 p가아님
		return false;
	return true;
}
bool RecChar::IsAlpha_b(int* CharXY, int SizeX, int SizeY) {
	if (!(1.45 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.61))//높이가 너비의비율이 b를 만족하지못하면 b 아님
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)//너비의 반에서 만나는 지점이 두개가 아니면 b 아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 8) != 1)//높이의 1/8 에서 만나는 지점이 한개가 아니면 b가아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 3 / 4) != 2)//높이의 3/4 에서 만나는 지점이 두개가 아니면 b가아님
		return false;
	if (NotInUp(CharXY, SizeX, SizeY, SizeY * 1/5) != 'R')//위쪽 부분에서 오른쪽 부분에픽셀이 존재하면 b가아님
		return false;
	if (NumOfLeft(CharXY, SizeX, SizeY, SizeY / 2) > NumOfRight(CharXY, SizeX, SizeY, SizeY / 2)*5)//위쪽 부분에서 오른쪽 부분에픽셀이 존재하면 b가아님
		return false;

	return true;
}
bool RecChar::IsAlpha_k(int* CharXY, int SizeX, int SizeY) {
	if (!(1 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.9))//높이가 너비의비율이 b를 만족하지못하면 b 아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 8) != 1)//높이의 1/8 에서 만나는 지점이 한개가 아니면 b가아님
		return false;
	if (NumOfLeft(CharXY, SizeX, SizeY, SizeY / 2) < NumOfRight(CharXY, SizeX, SizeY, SizeY / 2) * 3)//위쪽 부분에서 오른쪽 부분에픽셀이 존재하면 b가아님
		return false;

	return true;
}
bool RecChar::IsAlpha_d(int* CharXY, int SizeX, int SizeY) {
	if (!(1.45 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.61))//높이가 너비의비율이 d를 만족하지못하면 d 아님
		return false;
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)//너비의 반에서 만나는 지점이 두개가 아니면 d 아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 1 / 8) != 1)//높이의 1/8 에서 만나는 지점이 한개가 아니면 d가아님
		return false;
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 3 / 4) != 2)//높이의 3/4 에서 만나는 지점이 두개가 아니면 d가아님
		return false;
	if (NotInUp(CharXY, SizeX, SizeY, SizeY * 1 / 5) != 'L')//위쪽 부분에서 오른쪽 부분에픽셀이 존재하면 d가아님
		return false;
	return true;
}
bool RecChar::IsAlpha_m(int* CharXY, int SizeX, int SizeY)
{
	if (!(0.4< SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.1))//높이가 너비의비율이 m을 만족하지못하면 m 아님
		return false;
	//모든y축에서의 교점들의 수중 3개가 훨씬 많지않으면
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
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)//너비의 반에서 만나는 지점이 한개가 아니면 w아님
		return false;
	return true;
}
bool RecChar::IsAlpha_w(int* CharXY, int SizeX, int SizeY)
{
	if (!(0.60< SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.1))//높이가 너비의비율이 m을 만족하지못하면 m 아님
		return false;

	//모든y축에서의 교점들의 수중  두개또는 세개가 가장많으면 
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
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)//너비의 반에서 만나는 지점이 한개가 아니면 w아님
		return false;
	return true;
}
bool RecChar::IsAlpha_n(int* CharXY, int SizeX, int SizeY)
{
	//모든y축에서의 교점들의 수중  두개또는 세개가 적으면
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

	//모든x축에서의 교점들의 수중  한개가 가장많지않으면 
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
	//모든y축에서의 교점들의 수중  두개또는 세개가 가장많으면 
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
	//모든y축에서의 교점들의 수중  두개또는 세개가 가장많으면 
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
	//모든y축에서의 교점들의 수중  한개가 가장많으면 
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
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 2)//너비의 반에서 만나는 지점이 두개가 아니면 c아님
		return false;

	//모든X축에서의 교점들의 수중  두개가가장많으면 
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
{//모든y축에서의 교점들의 수중  두개또는 세개가 적으면
	if (!(1.45 < SizeY*1.0 / SizeX && SizeY*1.0 / SizeX < 1.61))//높이가 너비의비율이 d를 만족하지못하면 d 아님
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

	//모든x축에서의 교점들의 수중 4개가없으면 g가아님
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
	//모든y축에서의 교점들의 수중  한개가 가장많으면 
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
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 1)//높이의 반에서 만나는 지점이 두개가 아니면 f아님
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 3) != 1)//높이의의 1/3에서 만나는 지점이 두개가 아니면 f아님
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY * 2 / 3) != 1)//높이의 2/3에서 만나는 지점이 두개가 아니면 f아님
		return false;
	if (SizeX * 1.1 > SizeY)//너비가 높이에비해 어느정도 크면
		return false;

	return true;
}
bool RecChar::IsAlpha_z(int* CharXY, int SizeX, int SizeY)
{
	//모든y축에서의 교점들의 수중  한개가 가장많으면 
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
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 1)//높이의 반에서 만나는 지점이 두개가 아니면 f아님
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 3) != 1)//높이의의 1/3에서 만나는 지점이 두개가 아니면 f아님
		return false;
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY * 2 / 3) != 1)//높이의 2/3에서 만나는 지점이 두개가 아니면 f아님
		return false;


	//모든x축에서의 교점들의 수중 4개가없으면 g가아님
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
