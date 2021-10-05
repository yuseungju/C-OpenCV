#include "RecognitionChar.h"
#include "ChangeImgToBinary.h"
#include <fstream>

RecognitionChar::RecognitionChar(int* WordLabel, int MaxWordLabel, int height, int width)
{
	this->WordLabel = new int[height * width];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			this->WordLabel[width * y + x] = WordLabel[width * y + x];//라벨링된 이미지좌표의 레이블번호를 복사
		}
	}





	//////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* image;
	image = cvLoadImage("ImageFolder/8-1.jpg");
	//src = ReceiveImg();//글자인식할 이미지를 수신하여 반환
	//이미지에맞는 좌표 할당
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//이진화관련 객체 생성
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//이미지를 흑백화와 이진화시켜 가져옴

	for (int a = 0; a < height; a++) {
		for (int as = 0; as < width; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = this->WordLabel[a*width + as];
		}
	}
	cvNamedWindow("result",2);      // result 라는 이름의 윈도우를 만들고
	cvShowImage("result", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	cvWaitKey(0);
	cvDestroyWindow("result");
	////////////////////////////////////////////////////////////////////////////////////////////




	this->MaxWordLabel = MaxWordLabel;//라벨갯수 대입
	this->height = height;//2차배열의 높이 대입
	this->width = width;//2차배열의 너비 대입

	//단어 수만큼 메모리확보
	ShortXY = new int*[MaxWordLabel];
	MaxX = new int[MaxWordLabel];
	MaxY = new int[MaxWordLabel];
	MinX = new int[MaxWordLabel];
	MinY = new int[MaxWordLabel];
	ReconitionChar = new string[MaxWordLabel];
	DicMean = new string[MaxWordLabel];

	//초기화
	for (int i = 0; i < MaxWordLabel; i++) {
		MaxX[i] = -1;
		MaxY[i] = -1;
		MinX[i] = width;
		MinY[i] = height;
		ReconitionChar[i] = "/";
		DicMean[i] = "/";
	}
}

void RecognitionChar::GetInformation(int*& MaxX, int*& MaxY, int*& MinX, int*& MinY,
	string*& ReconitionChar, string*& DicMean, int& ShortXYNum)
{
	//문자를 인식하여 정보획득
	CalShortXY();//라벨을 단어별로분리하여 좌표에담음
	ShortXYNum = this->ShortXYNum;//식별된 단어의 갯수
	FromLabelToInformation();//단어별로 분리된 단어라벨 모두를 인식된 문자정보로 저장
	for (int row = 0; row < ShortXYNum; row++){//단어별로분리된 갯수만큼
		ConverToDicString(this->ReconitionChar[row], this->DicMean[row], this->ShortXY[row]);//사전에있는단어와 그의미를 가져옴
	}
	//인식된 문자정보  반환
	MaxX = this->MaxX;
	MaxY = this->MaxY;
	MinX = this->MinX;
	MinY = this->MinY;
	ReconitionChar = this->ReconitionChar;
	DicMean = this->DicMean;
}

void RecognitionChar::CalShortXY()
{
	//단어들의 최댓값최솟값 구함
	int LabelNumber;//비교할 차례의 좌표
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			if (WordLabel[width * y + x] != -1){
				LabelNumber = WordLabel[width * y + x] - 1; //알아볼 좌표의 라벨번호 - 1
				if (MaxX[LabelNumber] < x) {
					MaxX[LabelNumber] = x;
				}
				if (MaxY[LabelNumber] < y){
					MaxY[LabelNumber] = y;
				}
				if (MinX[LabelNumber] > x){
					MinX[LabelNumber] = x;
				}
				if (MinY[LabelNumber] > y){
					MinY[LabelNumber] = y;
				}
			}
		}
	}

	//단어들의 최댓값 최솟값중에서 인식하기 힘든크기의 단어를 제거하고 남은단어의 레이블을 구함
	int InLabelRow = 0;//단어정보 값을 넣는 자리
	int SizeX = 0;//단어너비
	int SizeY = 0;//단어높이
	for (int OutLabelRow = 0; OutLabelRow < MaxWordLabel; OutLabelRow++){//최대라벨갯수만큼
		SizeX = MaxX[OutLabelRow] - MinX[OutLabelRow] + 1;//비교 할 단어의 너비
		SizeY = MaxY[OutLabelRow] - MinY[OutLabelRow] + 1;//비교 할 단어의 높이
		if (SizeX  > 30 || SizeY > 30) {//비교한 사이즈가 적절한 단어식별 크기이면
			MaxX[InLabelRow] = MaxX[OutLabelRow];//단어의 최댓값X확정
			MinX[InLabelRow] = MinX[OutLabelRow];//단어의 최솟값X확정
			MaxY[InLabelRow] = MaxY[OutLabelRow];//단어의 최댓값Y확정
			MinY[InLabelRow] = MinY[OutLabelRow];//단어의 최솟값Y확정
			ShortXY[InLabelRow] = new int[SizeX * SizeY];//단어하나의 영역만큼 할당

			//해당단어 라벨이 담긴 단어영역을 구함
			for (int y = MinY[InLabelRow]; y <= MaxY[InLabelRow]; y++) {//단어 높이만큼
				for (int x = MinX[InLabelRow]; x <= MaxX[InLabelRow]; x++) {//단어너비만큼
					if (OutLabelRow + 1 == WordLabel[y * width + x])//같은 단어라벨번호이면
						ShortXY[InLabelRow][(y - MinY[InLabelRow]) * SizeX + (x - MinX[InLabelRow])]
						= WordLabel[y * width + x];//좌표 복사
					else//라벨번호가 다르면
						ShortXY[InLabelRow][(y - MinY[InLabelRow]) * SizeX + (x - MinX[InLabelRow])] = -1;
				}
			}

			InLabelRow++;//들어갈자리 다음으로
		} 
	}
	ShortXYNum = InLabelRow;//식별할 단어의 갯수저장
}

void RecognitionChar::FromLabelToInformation()
{
	int* ShortXY;//단어하나의 좌표를 받음
	int SizeX = 0;//단어너비
	int SizeY = 0;//단어높이
	for (int LabelRow = 48; LabelRow < ShortXYNum; LabelRow++) {//인식할수 있는 단어라벨수만큼
		SizeX = MaxX[LabelRow] - MinX[LabelRow] + 1;//선택된 단어하나의 너비
		SizeY = MaxY[LabelRow] - MinY[LabelRow] + 1;//선택된 단어하나의 높이
		ShortXY = new int[SizeX * SizeY];//선택된단어의 좌표수만큼할당
		for (int i = 0; i < SizeX * SizeY; i++)
			ShortXY[i] = this->ShortXY[LabelRow][i];//복사
		

		//////단어라벨중 한단어 출력////////////////////////////////////////////////////////////////////////////////////////
		IplImage* image;
		image = cvLoadImage("ImageFolder/8-1.jpg");
		//src = ReceiveImg();//글자인식할 이미지를 수신하여 반환
		ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//이진화관련 객체 생성
		IplImage* WhiteBlack;
		WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//이미지를 흑백화와 이진화시켜 가져옴

		for (int a = 0; a < height; a++) {
			for (int as = 0; as < width; as++) {
				WhiteBlack->imageData[a*WhiteBlack->width + as] = -1;
			}
		}

		for (int a = 0; a < SizeY; a++) {
			for (int as = 0; as < SizeX; as++) {
				WhiteBlack->imageData[a*WhiteBlack->width + as] = ShortXY[a*SizeX + as];
			}
		}
		cvNamedWindow("ShortXY", 2);      // result 라는 이름의 윈도우를 만들고
		cvShowImage("ShortXY", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
		cvWaitKey(0);
		cvDestroyWindow("ShortXY");
		////////////////////////////////////////////////////////////////////////////////////////////




		ReconitionChar[LabelRow] = GetAString(ShortXY, SizeX, SizeY);//단어 하나를 인식해서 문자로 저장
		delete[] ShortXY;
	}
}

string RecognitionChar::GetAString(int* StringXY, int SizeX, int SizeY)
{
	//단어를 글자단위로 라벨링
	//"NumOfChar:단어내 문자의갯수,   LabeledXY:단어를 라벨링하여 가지는좌표" 구함
	Labeling LabellingWord = Labeling(SizeY, SizeX);//한단어를 라벨링하는 라벨관련 객체생성
	int* LabeledXY;//단어를 라벨링하여 사용할 좌표
	LabeledXY = new int[SizeX * SizeY];//단어크기만큼할당
	int * temp;//임시로 단어의 좌표받음
	temp = LabellingWord.GetLabelingStringXY(StringXY);//단어를 글자단위로 라벨링
	int NumOfChar = LabellingWord.GetMaxLabelXY();//단어를 나눈 글자라벨의 최댓값 받음
	for (int y = 0; y < SizeY; y++) {//단어높이만큼
		for (int x = 0; x < SizeX; x++) {//단어너비만큼
			LabeledXY[y * SizeX + x] = temp[y * SizeX + x];//복사
		}
	}

	//한글자마다의 최댓값 최솟값좌표 사이즈를 가지는 포인터선언 및 초기화
	int* SizeCharX = new int[NumOfChar];//글자하나의 크기
	int* SizeCharY = new int[NumOfChar];//글자하나의 크기
	int* MaxCharX = new int[NumOfChar];//글자하나의 x최댓값
	int* MaxCharY = new int[NumOfChar];//글자하나의 y최댓값
	int* MinCharX = new int[NumOfChar];//글자하나의 x최솟값
	int* MinCharY = new int[NumOfChar];//글자하나의 y최솟값
	int* NumAchar = new int[NumOfChar];//글자하나의 라벨값	
	for (int i = 0; i < NumOfChar; i++){
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
			if (LabeledXY[SizeX * y + x] != -1){//라벨값이 존재하면
				LabelNumber = LabeledXY[SizeX * y + x] - 1;//라벨값-1의 자리로 LabelNumber설정
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
	}//최댓값 최솟값 구함

	//왼쪽에 배치한 글자부터 앞으로배치
	int TempRow;
	for (int i = 0; i < NumOfChar - 1; i++){
		for (int j = i + 1; j < NumOfChar; j++){
			if (MaxCharX[i] > MaxCharX[j]) {
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
	for (int LabelNumber = 0; LabelNumber < NumOfChar; LabelNumber++) {
		SizeCharX[LabelNumber] = MaxCharX[LabelNumber] - MinCharX[LabelNumber] + 1;//선택된 단어하나의 너비
		SizeCharY[LabelNumber] = MaxCharY[LabelNumber] - MinCharY[LabelNumber] + 1;//선택된 단어하나의 높이
	}

	//글자하나마다 가지는 라벨좌표값 포인터 선언 및 초기화
	//CharXY에 글자하나마다의 레이블들을 담음
	int** CharXY = new int*[NumOfChar];//단어내 글자수만큼 할당
	for (int CharRow = 0; CharRow < NumOfChar; CharRow++){//단어내 글자수만큼
		CharXY[CharRow] = new int[SizeCharX[CharRow] * SizeCharY[CharRow]];//글자내 레이블수만큼 할당
		for (int y = MinCharY[CharRow]; y <= MaxCharY[CharRow]; y++) {//글자하나의 높이만큼
			for (int x = MinCharX[CharRow]; x <= MaxCharX[CharRow]; x++) {//글자하나의 너비만큼
				if (NumAchar[CharRow] + 1 == LabeledXY[y * SizeX + x])//단어 라벨자리에 글자하나의 라벨번호가 있으면
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = 1;//1로 라벨링
				else//단어하나의 레이블 자리에 라벨번호가 없으면
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = -1;//-1로 라벨링
			}
		}
	}

	//단어의 평균y길이 구하기
	int AverageY = 0;
	for (int CharRow = 0; CharRow < NumOfChar; CharRow++)//단어내 글자수만큼
		AverageY += SizeCharY[CharRow];
	AverageY = AverageY / NumOfChar;
    
	//글자레이블마다의 인식된 문자값을 RecString 로가져온다
	string RecChar;
	string RecString = "";//단어내 인식한 글자를 담을 문자열
	for (int CharRow = 0; CharRow < NumOfChar; CharRow++){
		//문자가 인식되면 문자, 아니면 '/' 를 가져옴
		if (SizeCharY[CharRow] > AverageY / 5 && SizeCharY[CharRow] > 6){//하나의 글자정도의 높이와 픽셀수이면
			RecChar = GetAChar(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow]);//문자하나 가져오기

			if (RecChar == "l") {//l로인식된 문자이면
				int RecY;//l의 상단점에서 y축으로 픽셀값을 조사하고 난 위치
				int RecX;//l의 상단점에서 x축으로 픽셀값을 조사하고 난 위치
				int Gradiant;// 조사하지 않은 남은공간중, 조사방향의 기울기절대값
				Gradiant = SizeCharY[CharRow] / SizeCharX[CharRow];
				if (SizeCharX[CharRow] == 1) {//너비가적으면
					//처음 조사위치 지정
					RecX = MinCharX[CharRow];
					RecY = MinCharY[CharRow];

					RecChar = "i";//i로인식
				}
				else if (SizeCharY[CharRow] > SizeCharX[CharRow] && SizeCharX[CharRow] > 1) { //기울기 절대값이 1 이상일 경우이면 i가 맞는지 조사
					if (RightOfUp(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow], 1 / 2) < LeftOfUp(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow], 1 / 2)){//기울기가 음수이면
						//처음 조사위치 지정
						RecX = MinCharX[CharRow];
						RecY = MinCharY[CharRow];
						
						while ((RecX > 1) || (RecY > 1)){//조사위치가 0.0 이 되기전까지반복 
							//조사위치를 0.0 을향해 단위별로 옮김
							RecX = RecX - 1;// x를 왼쪽으로
							RecY = RecY - Gradiant;//y를 기울기 정수값 만큼 위로
							Gradiant = RecY / RecX;//X하나당 Y의변화량
							if (CharXY[CharRow][RecY * SizeCharX[CharRow] + RecX] == 1 || CharXY[CharRow][(RecY - 1) * SizeCharX[CharRow] + RecX - 1] == 1 
								|| CharXY[CharRow][(RecY +1) * SizeCharX[CharRow] + RecX + 1] == 1)//기준과 기준으로부터 얖옆 픽셀 중 점이있으면
								RecChar = "i";//i로인식
						}
					}
					else {//기울기가 양수이면
						//처음 조사위치 지정
						RecX = MinCharX[CharRow];
						RecY = MinCharY[CharRow];

						while ((RecX < MaxCharX[CharRow] - 1) || (RecY >= 0)){//조사위치가 0.X최댓값 이 되기전까지반복 
							//조사위치를 0.X최댓값 을향해 단위별로 옮김
							RecX = RecX + 1;// x를 오른쪽쪽으로
							RecY = RecY - Gradiant;//y를 기울기 정수값 만큼 위로
							Gradiant = RecY / RecX;//X하나당 Y의변화량
							if (CharXY[CharRow][RecY * SizeCharX[CharRow] + RecX] == 1 || CharXY[CharRow][(RecY - 1) * SizeCharX[CharRow] + RecX - 1] == 1
								|| CharXY[CharRow][(RecY + 1) * SizeCharX[CharRow] + RecX + 1] == 1)//기준과 기준으로부터 얖옆 픽셀 중 점이있으면
								RecChar = "i";//i로인식
						}
					}
				}
			}
			RecString = RecString + RecChar;//문자열에 문자하나 추가
		}
	}

	delete[] LabeledXY;
	delete[] SizeCharX;
	delete[] SizeCharY;
	delete[] MaxCharX;
	delete[] MaxCharY;
	delete[] MinCharX;
	delete[] MinCharY;
	for (int i = 0; i < NumOfChar; i++)
		delete[] CharXY[i];
	delete[] CharXY;
	return RecString;
}
string RecognitionChar::GetAChar(int* CharXY, int CharX, int CharY)
{
	if (IsAlpha_o(CharXY, CharX, CharY)){
		return "o";
	}
	if (IsAlpha_r(CharXY, CharX, CharY)){
		return "r";
	}
	if (IsAlpha_l(CharXY, CharX, CharY)){
		return "l";
	}


	return "/";
}
int RecognitionChar::NumCrossDotX8(int* CharXY, int SizeX, int SizeY, int x)
{
	int CROSS = 0;
	int POS = 8;//셈한 값으로부터 떨어진 픽셀거리
	for (int y = 0; y < SizeY; y++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 8) {//셈한 픽셀값으로부터 8이상벗어났고 값이 존재하면
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
int RecognitionChar::NumCrossDotY8(int* CharXY, int SizeX, int SizeY, int y)
{
	int CROSS = 0;
	int POS = 8;//셈한 값으로부터 떨어진 픽셀거리
	for (int x = 0; x < SizeX; x++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 8) {//셈한 픽셀값으로부터 8이상벗어났고 값이 존재하면
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
int RecognitionChar::NumCrossDotX2(int* CharXY, int SizeX, int SizeY, int x)
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
int RecognitionChar::NumCrossDotY2(int* CharXY, int SizeX, int SizeY, int y)
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
int RecognitionChar::NumOfRight(int* CharXY, int SizeX, int SizeY, int StandardX)
{
	int NumRight = 1;
	
	//기준으로부터 오른쪽 필셀값 1을 조사
	for (int y = 0; y < SizeY; y++) {
		for (int x = StandardX + 1; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumRight++;
			}
		}
	}
	return NumRight;
}
int RecognitionChar::NumOfLeft(int* CharXY, int SizeX, int SizeY, int StandardX)
{
	int NumReft = 1;
	//기준으로부터 왼쪽 필셀값 1을 조사
	for (int y = 0; y < SizeY; y++) {
		for (int x = 0; x < StandardX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumReft++;
			}
		}
	}
	return NumReft;
}
int RecognitionChar::NumOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int NumButtom = 1;
	//기준으로부터 아래쪽 필셀값 1을 조사
	for (int y = StandardY + 1; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				NumButtom++;
			}
		}
	}
	return NumButtom;
}
int RecognitionChar::NumOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
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

int RecognitionChar::RightOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
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
int RecognitionChar::LeftOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
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
int RecognitionChar::RightOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Right = 0;
	//기준으로부터 아래쪽 필셀값을 조사
	for (int y = StandardY + 1; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Right < x)
					Right = x;
			}
		}
	}
	return Right;//가장왼쪽 반환
}
int RecognitionChar::LeftOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Left = SizeX;
	//기준으로부터 아래쪽 필셀값을 조사
	for (int y = StandardY + 1; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Left > x)
					Left = x;
			}
		}
	}
	return Left;//가장왼쪽 반환
}
bool RecognitionChar::IsAlpha_o(int* CharXY, int SizeX, int SizeY)
{
	//높이의 2분의1에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 2)
		return false;
	//높이의 4분의1에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 4) != 2)
		return false;
	//높이의 4분의3에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY*3 / 4) != 2)
		return false;
	//너비의 2분의1에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX/2) != 2)
		return false;
	//너비의 2분의1에서 + 너비의 9분의1을 더한x축에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 + SizeX / 9) != 2)
		return false;
	//너비의 2분의1에서 - 너비의 9분의1을 더한x축에서 서로 떨어져잇는 값의 갯수가 2가아니면 종료 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 - SizeX / 9) != 2)
		return false;
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* image;
	image = cvLoadImage("ImageFolder/8-1.jpg");
	//src = ReceiveImg();//글자인식할 이미지를 수신하여 반환
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//이진화관련 객체 생성
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//이미지를 흑백화와 이진화시켜 가져옴

	for (int a = 0; a < height; a++) {
		for (int as = 0; as < width; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = 100;
		}
	}

	for (int a = 0; a < SizeY; a++) {
		for (int as = 0; as < SizeX; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = CharXY[a*SizeX + as];
		}
	}
	cvNamedWindow("ooooo", 2);      // result 라는 이름의 윈도우를 만들고
	cvShowImage("ooooo", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	cvWaitKey(0);
	cvDestroyWindow("ooooo");
	////////////////////////////////////////////////////////////////////////////////////////////


	return true;
}
bool RecognitionChar::IsAlpha_r(int* CharXY, int SizeX, int SizeY)
{
	//위쪽이 아래쪽보다 더오른쪽에 있지않으면 r이 아님
	if (RightOfUp(CharXY, SizeX, SizeY, SizeY / 2) < RightOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//위쪽 픽셀 값이 아래쪽 값보다 많지않으면  r이 아님
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) < NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//왼쪽의 픽셀 값이 오른쪽의 값보다 3배 이상 많지않으면  r이 아님
	if (NumOfLeft(CharXY, SizeX, SizeY, SizeX / 2) < NumOfRight(CharXY, SizeX, SizeY, SizeX / 2) * 3)
		return false;
	//높이의 2분의1에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY / 2) != 1)
		return false;
	//높이의 4분의1에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY / 4) != 1)
		return false;
	//높이의 4분의3에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 3 / 4) != 1)
		return false;
	//너비의 2분의1에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)
		return false;
	//너비의 2분의1에서 + 너비의 9분의1을 더한x축에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 + SizeX / 9) != 1)
		return false;
	//너비의 2분의1에서 - 너비의 9분의1을 더한x축에서 서로 떨어져잇는 값의 갯수가 1가아니면  r이 아님 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 - SizeX / 9) != 1)
		return false;
	//높이의 1/2에서 맨위까지y값중서로 떨어져잇는 값의 갯수 2가 하나도 없으면 r이 아님 
	int Standard;
	for (Standard = SizeY / 2; Standard > 0; Standard--){
		if (NumCrossDotY2(CharXY, SizeX, SizeY, Standard) == 2)
			break;
	}
	if (Standard == 0)
		return false;

	//////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* image;
	image = cvLoadImage("ImageFolder/8-1.jpg");
	//src = ReceiveImg();//글자인식할 이미지를 수신하여 반환
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//이진화관련 객체 생성
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//이미지를 흑백화와 이진화시켜 가져옴

	for (int a = 0; a < height; a++) {
		for (int as = 0; as < width; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = 100;
		}
	}

	for (int a = 0; a < SizeY; a++) {
		for (int as = 0; as < SizeX; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = CharXY[a*SizeX + as];
		}
	}
	cvNamedWindow("rrr", 2);      // result 라는 이름의 윈도우를 만들고
	cvShowImage("rrr", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	cvWaitKey(0);
	cvDestroyWindow("rrr");
	////////////////////////////////////////////////////////////////////////////////////////////	


	return true;
}
bool RecognitionChar::IsAlpha_l(int* CharXY, int SizeX, int SizeY) {

	//////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* image;
	image = cvLoadImage("ImageFolder/8-1.jpg");
	//src = ReceiveImg();//글자인식할 이미지를 수신하여 반환
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//이진화관련 객체 생성
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//이미지를 흑백화와 이진화시켜 가져옴

	for (int a = 0; a < height; a++) {
		for (int as = 0; as < width; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = 100;
		}
	}

	for (int a = 0; a < SizeY; a++) {
		for (int as = 0; as < SizeX; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = CharXY[a*SizeX + as];
		}
	}
	cvNamedWindow("l을조사", 2);      // result 라는 이름의 윈도우를 만들고
	cvShowImage("l을조사", WhiteBlack);  // 이 윈도우에 내가 만든 ipl_Img 이미지를 보이고
	cvWaitKey(0);
	cvDestroyWindow("l을조사");
	////////////////////////////////////////////////////////////////////////////////////////////	

	//////////////////수리중
	//높이의 반을 기준으로 위와 아래의 픽셀 수가 비슷하지않으면 l이 아님
	if (!(-(SizeY * SizeX/10) < (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) - NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2)) &&
		(NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) - NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2)) < +(SizeY * SizeX / 10)))
		return false;
	//높이가 너비보다 3배이상 크지않으면 l이 아님
	if (SizeX * 3 > SizeY)
		return false;
	
 	cout << "asd" << endl;



}
bool RecognitionChar::FitWord(string MyWord, string DicWord) {

	//인식글자내용여부
	if (MyWord.length() < 0)//인식글자의 내용이 없으면
		return false;

	int MyPos = 0;//인식한단어의 위치
	int DicPos = 0;//사전단어의 위치
	char MyChar = ' ';//인식단어의 문자하나
	char DicChar = ' ';//사전단어의 문자하나

	//단어의 처음과 끝이 같은지
	if ((MyWord.at(0) != DicWord.at(0) && MyWord.at(0) != '/') ||
		//인식문자와 사전문자의 앞글자가같지않고 인식되지않은 '/'값이면
		(MyWord.at(MyWord.length() - 1) != DicWord.at(DicWord.length() - 1)
		&& MyWord.at(MyWord.length() - 1) != '/')){
		//인식문자와 사전문자의 뒷글자가같지않고  인식되지않은 '/'값이면
		return false;
	}
	
	//같을 가능성이 있는 단어를 상세히 비교
	MyPos = 0;//인식단어위치 처음으로
	while (true) {
		MyChar = MyWord.at(MyPos);//인식단어의 위치에 해당하는 문자를 가져옴
		DicChar = DicWord.at(DicPos);//사전단어의 위치에 해당하는 문자를 가져옴

		if (MyChar == '/') {//인식단어의 문자하나가 불확실한 값의 문자이면
			if (MyPos + 1 == MyWord.length()) {//인식한 단어의 다음 문자가 없으면
				return true;
			}
			else if (DicPos + 1 == DicWord.length()) {//사전단어의 다음 문자가 없으면
				return false;
			}
			MyPos++;//인식한 단어를 다음위치로
			DicPos++;//사전 단어를 다음위치로
		}
		else if (MyChar == DicChar) {//두단어의 위치에 있는 문자값이 같으면
			if (MyPos + 1 == MyWord.length()) {//인식한 단어의 다음 문자가 없으면
				return true;    
			}
			else if (DicPos + 1 == DicWord.length()) {//사전단어의 다음 문자가 없으면
				return false;
			}
			MyPos++;//인식단어의 다음위치로
			DicPos++;//사전단어의 다음위치로
		}
		else{// 두단어의 위치에있는 문자값이 다르면
			if (0 < MyPos){//인식단어의 비교글자위치가 처음이아니면
				if (MyWord.at(MyPos - 1) != '/'){//바로앞 글자가 불확실한값이 아니였으면
					MyPos--;//앞 글자위치로
					return false;
				}
			}
			if (DicPos + 1 == DicWord.length()) {//사전단어의 다음 문자가 없으면
				return false;
			}
			DicPos++;//사전단어의 다음위치로
		}
	}
}

void RecognitionChar::ConverToDicString(string& RecString, string& DicMean, int* StringLabel)
{
	//인식한 문자열중 불확실한 문자열 처리
	int MyPos;
	for (MyPos = 0; MyPos < RecString.length(); MyPos++) {//인식문자길이만큼
		if (MyPos > 0){
			if (RecString.at(MyPos - 1) == RecString.at(MyPos) && RecString.at(MyPos) == '/'){// 맨앞 다음이거나 '/'가 앞에도 있는 경우
				RecString.erase(MyPos, 1);//  '/'을 문자열에서 제거
				MyPos--;
			}
		}
	}

	//인식된 문자열중 불확실한값만 있을경우 
	for (MyPos = 0; MyPos < RecString.length(); MyPos++) {//인식문자길이만큼
		if (RecString.at(MyPos) != '/')//확실하게 인식된 문자이면
			break;
	}
	if (MyPos == RecString.length()){//인식문자중 확실한문자가 하나도없으면
		RecString = "NOT";
		DicMean = "NOT";
		return;
	}

	//인식된 문자열중 불확실한값이 많을경우
	int num = 0;
	for (MyPos = 0; MyPos < RecString.length(); MyPos++) {//인식문자길이만큼
		if (RecString.at(MyPos) == '/')//확실하게 인식된 문자이면
			num++;
	}
	if (num > RecString.length() / 2.5) {//  모르는 문자가 단어의 1/ 2.5 이 넘으면
		RecString = "NOT";
		DicMean = "NOT";
		return;
	}

	string FirstChar;
	FirstChar = RecString.at(0);
	FirstChar = "영어사전자료파일\dic_" + FirstChar;
	FirstChar = FirstChar + ".txt";

	fstream inFile;
	DicMean = "";
	string TempMean;
	string DicWord;
	string SimilarWord[50] = { "" };//인식단어와 유사한 사전단어후보
	string SimilarMean[50] = { "" };//인식단어와 유사한 사전단어후보의 뜻,의미
	int WLength = 0;//사전단어후보길이		
	int MLength = 0;//사전단어후보의 뜻 길이
	inFile.open(FirstChar);//사전파일열기

	while (!inFile.eof()){//파일의 모든단어중 인식단어와 유사하면 값을보관
		inFile >> DicWord;

		while (inFile.get() != '\n' && !inFile.eof()){
			inFile >> TempMean;
			DicMean += TempMean;
		}

		if (FitWord(RecString, DicWord)){//유사단어와 인식단어가 유사하면
			SimilarWord[WLength] = DicWord;
			SimilarMean[MLength] = DicMean;
			WLength++;
			MLength++;
			if (WLength >= 50) {
				RecString = "NOT";
				DicMean = "NOT";
				inFile.close();
				return;
			}
		}
		DicMean = "";
	}
	inFile.close();

	if (WLength == 0) {
		RecString = "NOT";
		DicMean = "NOT";
		inFile.close();
		return;
	}
	return;
	//후보들 중 label과 가장 유사한것 찾기
	//.....이용
	//SimilarWord[WLength] = DicWord;
	//SimilarMean[MLength] = DicMean;
	//int* StringLabel
	//사전의 글자하나의정보들을 집합시켜 단어끼리비교

}

RecognitionChar::~RecognitionChar()
{
	//동적할당 해제
	delete[] MaxX;
	delete[] MaxY;
	delete[] MinX;
	delete[] MinY;
	delete[] ReconitionChar;
	for (int i = 0; i< ShortXYNum; i++)
		delete[] ShortXY[i];
	delete[] ShortXY;
	delete[] WordLabel;
}
