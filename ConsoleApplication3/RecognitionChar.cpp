#include "RecognitionChar.h"
#include "ChangeImgToBinary.h"
#include <fstream>

RecognitionChar::RecognitionChar(int* WordLabel, int MaxWordLabel, int height, int width)
{
	this->WordLabel = new int[height * width];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			this->WordLabel[width * y + x] = WordLabel[width * y + x];//�󺧸��� �̹�����ǥ�� ���̺��ȣ�� ����
		}
	}





	//////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* image;
	image = cvLoadImage("ImageFolder/8-1.jpg");
	//src = ReceiveImg();//�����ν��� �̹����� �����Ͽ� ��ȯ
	//�̹������´� ��ǥ �Ҵ�
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//����ȭ���� ��ü ����
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//�̹����� ���ȭ�� ����ȭ���� ������

	for (int a = 0; a < height; a++) {
		for (int as = 0; as < width; as++) {
			WhiteBlack->imageData[a*WhiteBlack->width + as] = this->WordLabel[a*width + as];
		}
	}
	cvNamedWindow("result",2);      // result ��� �̸��� �����츦 �����
	cvShowImage("result", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	cvWaitKey(0);
	cvDestroyWindow("result");
	////////////////////////////////////////////////////////////////////////////////////////////




	this->MaxWordLabel = MaxWordLabel;//�󺧰��� ����
	this->height = height;//2���迭�� ���� ����
	this->width = width;//2���迭�� �ʺ� ����

	//�ܾ� ����ŭ �޸�Ȯ��
	ShortXY = new int*[MaxWordLabel];
	MaxX = new int[MaxWordLabel];
	MaxY = new int[MaxWordLabel];
	MinX = new int[MaxWordLabel];
	MinY = new int[MaxWordLabel];
	ReconitionChar = new string[MaxWordLabel];
	DicMean = new string[MaxWordLabel];

	//�ʱ�ȭ
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
	//���ڸ� �ν��Ͽ� ����ȹ��
	CalShortXY();//���� �ܾ�κи��Ͽ� ��ǥ������
	ShortXYNum = this->ShortXYNum;//�ĺ��� �ܾ��� ����
	FromLabelToInformation();//�ܾ�� �и��� �ܾ�� ��θ� �νĵ� ���������� ����
	for (int row = 0; row < ShortXYNum; row++){//�ܾ�κи��� ������ŭ
		ConverToDicString(this->ReconitionChar[row], this->DicMean[row], this->ShortXY[row]);//�������ִ´ܾ�� ���ǹ̸� ������
	}
	//�νĵ� ��������  ��ȯ
	MaxX = this->MaxX;
	MaxY = this->MaxY;
	MinX = this->MinX;
	MinY = this->MinY;
	ReconitionChar = this->ReconitionChar;
	DicMean = this->DicMean;
}

void RecognitionChar::CalShortXY()
{
	//�ܾ���� �ִ��ּڰ� ����
	int LabelNumber;//���� ������ ��ǥ
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			if (WordLabel[width * y + x] != -1){
				LabelNumber = WordLabel[width * y + x] - 1; //�˾ƺ� ��ǥ�� �󺧹�ȣ - 1
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

	//�ܾ���� �ִ� �ּڰ��߿��� �ν��ϱ� ����ũ���� �ܾ �����ϰ� �����ܾ��� ���̺��� ����
	int InLabelRow = 0;//�ܾ����� ���� �ִ� �ڸ�
	int SizeX = 0;//�ܾ�ʺ�
	int SizeY = 0;//�ܾ����
	for (int OutLabelRow = 0; OutLabelRow < MaxWordLabel; OutLabelRow++){//�ִ�󺧰�����ŭ
		SizeX = MaxX[OutLabelRow] - MinX[OutLabelRow] + 1;//�� �� �ܾ��� �ʺ�
		SizeY = MaxY[OutLabelRow] - MinY[OutLabelRow] + 1;//�� �� �ܾ��� ����
		if (SizeX  > 30 || SizeY > 30) {//���� ����� ������ �ܾ�ĺ� ũ���̸�
			MaxX[InLabelRow] = MaxX[OutLabelRow];//�ܾ��� �ִ�XȮ��
			MinX[InLabelRow] = MinX[OutLabelRow];//�ܾ��� �ּڰ�XȮ��
			MaxY[InLabelRow] = MaxY[OutLabelRow];//�ܾ��� �ִ�YȮ��
			MinY[InLabelRow] = MinY[OutLabelRow];//�ܾ��� �ּڰ�YȮ��
			ShortXY[InLabelRow] = new int[SizeX * SizeY];//�ܾ��ϳ��� ������ŭ �Ҵ�

			//�ش�ܾ� ���� ��� �ܾ���� ����
			for (int y = MinY[InLabelRow]; y <= MaxY[InLabelRow]; y++) {//�ܾ� ���̸�ŭ
				for (int x = MinX[InLabelRow]; x <= MaxX[InLabelRow]; x++) {//�ܾ�ʺ�ŭ
					if (OutLabelRow + 1 == WordLabel[y * width + x])//���� �ܾ�󺧹�ȣ�̸�
						ShortXY[InLabelRow][(y - MinY[InLabelRow]) * SizeX + (x - MinX[InLabelRow])]
						= WordLabel[y * width + x];//��ǥ ����
					else//�󺧹�ȣ�� �ٸ���
						ShortXY[InLabelRow][(y - MinY[InLabelRow]) * SizeX + (x - MinX[InLabelRow])] = -1;
				}
			}

			InLabelRow++;//���ڸ� ��������
		} 
	}
	ShortXYNum = InLabelRow;//�ĺ��� �ܾ��� ��������
}

void RecognitionChar::FromLabelToInformation()
{
	int* ShortXY;//�ܾ��ϳ��� ��ǥ�� ����
	int SizeX = 0;//�ܾ�ʺ�
	int SizeY = 0;//�ܾ����
	for (int LabelRow = 48; LabelRow < ShortXYNum; LabelRow++) {//�ν��Ҽ� �ִ� �ܾ�󺧼���ŭ
		SizeX = MaxX[LabelRow] - MinX[LabelRow] + 1;//���õ� �ܾ��ϳ��� �ʺ�
		SizeY = MaxY[LabelRow] - MinY[LabelRow] + 1;//���õ� �ܾ��ϳ��� ����
		ShortXY = new int[SizeX * SizeY];//���õȴܾ��� ��ǥ����ŭ�Ҵ�
		for (int i = 0; i < SizeX * SizeY; i++)
			ShortXY[i] = this->ShortXY[LabelRow][i];//����
		

		//////�ܾ���� �Ѵܾ� ���////////////////////////////////////////////////////////////////////////////////////////
		IplImage* image;
		image = cvLoadImage("ImageFolder/8-1.jpg");
		//src = ReceiveImg();//�����ν��� �̹����� �����Ͽ� ��ȯ
		ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//����ȭ���� ��ü ����
		IplImage* WhiteBlack;
		WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//�̹����� ���ȭ�� ����ȭ���� ������

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
		cvNamedWindow("ShortXY", 2);      // result ��� �̸��� �����츦 �����
		cvShowImage("ShortXY", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
		cvWaitKey(0);
		cvDestroyWindow("ShortXY");
		////////////////////////////////////////////////////////////////////////////////////////////




		ReconitionChar[LabelRow] = GetAString(ShortXY, SizeX, SizeY);//�ܾ� �ϳ��� �ν��ؼ� ���ڷ� ����
		delete[] ShortXY;
	}
}

string RecognitionChar::GetAString(int* StringXY, int SizeX, int SizeY)
{
	//�ܾ ���ڴ����� �󺧸�
	//"NumOfChar:�ܾ �����ǰ���,   LabeledXY:�ܾ �󺧸��Ͽ� ��������ǥ" ����
	Labeling LabellingWord = Labeling(SizeY, SizeX);//�Ѵܾ �󺧸��ϴ� �󺧰��� ��ü����
	int* LabeledXY;//�ܾ �󺧸��Ͽ� ����� ��ǥ
	LabeledXY = new int[SizeX * SizeY];//�ܾ�ũ�⸸ŭ�Ҵ�
	int * temp;//�ӽ÷� �ܾ��� ��ǥ����
	temp = LabellingWord.GetLabelingStringXY(StringXY);//�ܾ ���ڴ����� �󺧸�
	int NumOfChar = LabellingWord.GetMaxLabelXY();//�ܾ ���� ���ڶ��� �ִ� ����
	for (int y = 0; y < SizeY; y++) {//�ܾ���̸�ŭ
		for (int x = 0; x < SizeX; x++) {//�ܾ�ʺ�ŭ
			LabeledXY[y * SizeX + x] = temp[y * SizeX + x];//����
		}
	}

	//�ѱ��ڸ����� �ִ� �ּڰ���ǥ ����� ������ �����ͼ��� �� �ʱ�ȭ
	int* SizeCharX = new int[NumOfChar];//�����ϳ��� ũ��
	int* SizeCharY = new int[NumOfChar];//�����ϳ��� ũ��
	int* MaxCharX = new int[NumOfChar];//�����ϳ��� x�ִ�
	int* MaxCharY = new int[NumOfChar];//�����ϳ��� y�ִ�
	int* MinCharX = new int[NumOfChar];//�����ϳ��� x�ּڰ�
	int* MinCharY = new int[NumOfChar];//�����ϳ��� y�ּڰ�
	int* NumAchar = new int[NumOfChar];//�����ϳ��� �󺧰�	
	for (int i = 0; i < NumOfChar; i++){
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
			if (LabeledXY[SizeX * y + x] != -1){//�󺧰��� �����ϸ�
				LabelNumber = LabeledXY[SizeX * y + x] - 1;//�󺧰�-1�� �ڸ��� LabelNumber����
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
	}//�ִ� �ּڰ� ����

	//���ʿ� ��ġ�� ���ں��� �����ι�ġ
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

	//�������
	for (int LabelNumber = 0; LabelNumber < NumOfChar; LabelNumber++) {
		SizeCharX[LabelNumber] = MaxCharX[LabelNumber] - MinCharX[LabelNumber] + 1;//���õ� �ܾ��ϳ��� �ʺ�
		SizeCharY[LabelNumber] = MaxCharY[LabelNumber] - MinCharY[LabelNumber] + 1;//���õ� �ܾ��ϳ��� ����
	}

	//�����ϳ����� ������ ����ǥ�� ������ ���� �� �ʱ�ȭ
	//CharXY�� �����ϳ������� ���̺���� ����
	int** CharXY = new int*[NumOfChar];//�ܾ ���ڼ���ŭ �Ҵ�
	for (int CharRow = 0; CharRow < NumOfChar; CharRow++){//�ܾ ���ڼ���ŭ
		CharXY[CharRow] = new int[SizeCharX[CharRow] * SizeCharY[CharRow]];//���ڳ� ���̺����ŭ �Ҵ�
		for (int y = MinCharY[CharRow]; y <= MaxCharY[CharRow]; y++) {//�����ϳ��� ���̸�ŭ
			for (int x = MinCharX[CharRow]; x <= MaxCharX[CharRow]; x++) {//�����ϳ��� �ʺ�ŭ
				if (NumAchar[CharRow] + 1 == LabeledXY[y * SizeX + x])//�ܾ� ���ڸ��� �����ϳ��� �󺧹�ȣ�� ������
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = 1;//1�� �󺧸�
				else//�ܾ��ϳ��� ���̺� �ڸ��� �󺧹�ȣ�� ������
					CharXY[CharRow][(y - MinCharY[CharRow]) * SizeCharX[CharRow] + (x - MinCharX[CharRow])] = -1;//-1�� �󺧸�
			}
		}
	}

	//�ܾ��� ���y���� ���ϱ�
	int AverageY = 0;
	for (int CharRow = 0; CharRow < NumOfChar; CharRow++)//�ܾ ���ڼ���ŭ
		AverageY += SizeCharY[CharRow];
	AverageY = AverageY / NumOfChar;
    
	//���ڷ��̺����� �νĵ� ���ڰ��� RecString �ΰ����´�
	string RecChar;
	string RecString = "";//�ܾ �ν��� ���ڸ� ���� ���ڿ�
	for (int CharRow = 0; CharRow < NumOfChar; CharRow++){
		//���ڰ� �νĵǸ� ����, �ƴϸ� '/' �� ������
		if (SizeCharY[CharRow] > AverageY / 5 && SizeCharY[CharRow] > 6){//�ϳ��� ���������� ���̿� �ȼ����̸�
			RecChar = GetAChar(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow]);//�����ϳ� ��������

			if (RecChar == "l") {//l���νĵ� �����̸�
				int RecY;//l�� ��������� y������ �ȼ����� �����ϰ� �� ��ġ
				int RecX;//l�� ��������� x������ �ȼ����� �����ϰ� �� ��ġ
				int Gradiant;// �������� ���� ����������, ��������� �������밪
				Gradiant = SizeCharY[CharRow] / SizeCharX[CharRow];
				if (SizeCharX[CharRow] == 1) {//�ʺ�������
					//ó�� ������ġ ����
					RecX = MinCharX[CharRow];
					RecY = MinCharY[CharRow];

					RecChar = "i";//i���ν�
				}
				else if (SizeCharY[CharRow] > SizeCharX[CharRow] && SizeCharX[CharRow] > 1) { //���� ���밪�� 1 �̻��� ����̸� i�� �´��� ����
					if (RightOfUp(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow], 1 / 2) < LeftOfUp(CharXY[CharRow], SizeCharX[CharRow], SizeCharY[CharRow], 1 / 2)){//���Ⱑ �����̸�
						//ó�� ������ġ ����
						RecX = MinCharX[CharRow];
						RecY = MinCharY[CharRow];
						
						while ((RecX > 1) || (RecY > 1)){//������ġ�� 0.0 �� �Ǳ��������ݺ� 
							//������ġ�� 0.0 ������ �������� �ű�
							RecX = RecX - 1;// x�� ��������
							RecY = RecY - Gradiant;//y�� ���� ������ ��ŭ ����
							Gradiant = RecY / RecX;//X�ϳ��� Y�Ǻ�ȭ��
							if (CharXY[CharRow][RecY * SizeCharX[CharRow] + RecX] == 1 || CharXY[CharRow][(RecY - 1) * SizeCharX[CharRow] + RecX - 1] == 1 
								|| CharXY[CharRow][(RecY +1) * SizeCharX[CharRow] + RecX + 1] == 1)//���ذ� �������κ��� �A�� �ȼ� �� ����������
								RecChar = "i";//i���ν�
						}
					}
					else {//���Ⱑ ����̸�
						//ó�� ������ġ ����
						RecX = MinCharX[CharRow];
						RecY = MinCharY[CharRow];

						while ((RecX < MaxCharX[CharRow] - 1) || (RecY >= 0)){//������ġ�� 0.X�ִ� �� �Ǳ��������ݺ� 
							//������ġ�� 0.X�ִ� ������ �������� �ű�
							RecX = RecX + 1;// x�� ������������
							RecY = RecY - Gradiant;//y�� ���� ������ ��ŭ ����
							Gradiant = RecY / RecX;//X�ϳ��� Y�Ǻ�ȭ��
							if (CharXY[CharRow][RecY * SizeCharX[CharRow] + RecX] == 1 || CharXY[CharRow][(RecY - 1) * SizeCharX[CharRow] + RecX - 1] == 1
								|| CharXY[CharRow][(RecY + 1) * SizeCharX[CharRow] + RecX + 1] == 1)//���ذ� �������κ��� �A�� �ȼ� �� ����������
								RecChar = "i";//i���ν�
						}
					}
				}
			}
			RecString = RecString + RecChar;//���ڿ��� �����ϳ� �߰�
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
	int POS = 8;//���� �����κ��� ������ �ȼ��Ÿ�
	for (int y = 0; y < SizeY; y++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 8) {//���� �ȼ������κ��� 8�̻����� ���� �����ϸ�
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
int RecognitionChar::NumCrossDotY8(int* CharXY, int SizeX, int SizeY, int y)
{
	int CROSS = 0;
	int POS = 8;//���� �����κ��� ������ �ȼ��Ÿ�
	for (int x = 0; x < SizeX; x++) {
		if (CharXY[y*SizeX + x] == 1 && POS >= 8) {//���� �ȼ������κ��� 8�̻����� ���� �����ϸ�
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
int RecognitionChar::NumCrossDotX2(int* CharXY, int SizeX, int SizeY, int x)
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
int RecognitionChar::NumCrossDotY2(int* CharXY, int SizeX, int SizeY, int y)
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
int RecognitionChar::NumOfRight(int* CharXY, int SizeX, int SizeY, int StandardX)
{
	int NumRight = 1;
	
	//�������κ��� ������ �ʼ��� 1�� ����
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
	//�������κ��� ���� �ʼ��� 1�� ����
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
	//�������κ��� �Ʒ��� �ʼ��� 1�� ����
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

int RecognitionChar::RightOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
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
int RecognitionChar::LeftOfUp(int* CharXY, int SizeX, int SizeY, int StandardY)
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
int RecognitionChar::RightOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Right = 0;
	//�������κ��� �Ʒ��� �ʼ����� ����
	for (int y = StandardY + 1; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Right < x)
					Right = x;
			}
		}
	}
	return Right;//������� ��ȯ
}
int RecognitionChar::LeftOfButtom(int* CharXY, int SizeX, int SizeY, int StandardY)
{
	int Left = SizeX;
	//�������κ��� �Ʒ��� �ʼ����� ����
	for (int y = StandardY + 1; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			if (CharXY[y*SizeX + x] == 1){
				if (Left > x)
					Left = x;
			}
		}
	}
	return Left;//������� ��ȯ
}
bool RecognitionChar::IsAlpha_o(int* CharXY, int SizeX, int SizeY)
{
	//������ 2����1���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 2) != 2)
		return false;
	//������ 4����1���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY / 4) != 2)
		return false;
	//������ 4����3���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotY8(CharXY, SizeX, SizeY, SizeY*3 / 4) != 2)
		return false;
	//�ʺ��� 2����1���� ���� �������մ� ���� ������ 2���ƴϸ� ����
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX/2) != 2)
		return false;
	//�ʺ��� 2����1���� + �ʺ��� 9����1�� ����x�࿡�� ���� �������մ� ���� ������ 2���ƴϸ� ���� 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 + SizeX / 9) != 2)
		return false;
	//�ʺ��� 2����1���� - �ʺ��� 9����1�� ����x�࿡�� ���� �������մ� ���� ������ 2���ƴϸ� ���� 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 - SizeX / 9) != 2)
		return false;
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* image;
	image = cvLoadImage("ImageFolder/8-1.jpg");
	//src = ReceiveImg();//�����ν��� �̹����� �����Ͽ� ��ȯ
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//����ȭ���� ��ü ����
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//�̹����� ���ȭ�� ����ȭ���� ������

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
	cvNamedWindow("ooooo", 2);      // result ��� �̸��� �����츦 �����
	cvShowImage("ooooo", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	cvWaitKey(0);
	cvDestroyWindow("ooooo");
	////////////////////////////////////////////////////////////////////////////////////////////


	return true;
}
bool RecognitionChar::IsAlpha_r(int* CharXY, int SizeX, int SizeY)
{
	//������ �Ʒ��ʺ��� �������ʿ� ���������� r�� �ƴ�
	if (RightOfUp(CharXY, SizeX, SizeY, SizeY / 2) < RightOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//���� �ȼ� ���� �Ʒ��� ������ ����������  r�� �ƴ�
	if (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) < NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2))
		return false;
	//������ �ȼ� ���� �������� ������ 3�� �̻� ����������  r�� �ƴ�
	if (NumOfLeft(CharXY, SizeX, SizeY, SizeX / 2) < NumOfRight(CharXY, SizeX, SizeY, SizeX / 2) * 3)
		return false;
	//������ 2����1���� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ�
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY / 2) != 1)
		return false;
	//������ 4����1���� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ�
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY / 4) != 1)
		return false;
	//������ 4����3���� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ�
	if (NumCrossDotY2(CharXY, SizeX, SizeY, SizeY * 3 / 4) != 1)
		return false;
	//�ʺ��� 2����1���� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ�
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2) != 1)
		return false;
	//�ʺ��� 2����1���� + �ʺ��� 9����1�� ����x�࿡�� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ� 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 + SizeX / 9) != 1)
		return false;
	//�ʺ��� 2����1���� - �ʺ��� 9����1�� ����x�࿡�� ���� �������մ� ���� ������ 1���ƴϸ�  r�� �ƴ� 
	if (NumCrossDotX8(CharXY, SizeX, SizeY, SizeX / 2 - SizeX / 9) != 1)
		return false;
	//������ 1/2���� ��������y���߼��� �������մ� ���� ���� 2�� �ϳ��� ������ r�� �ƴ� 
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
	//src = ReceiveImg();//�����ν��� �̹����� �����Ͽ� ��ȯ
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//����ȭ���� ��ü ����
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//�̹����� ���ȭ�� ����ȭ���� ������

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
	cvNamedWindow("rrr", 2);      // result ��� �̸��� �����츦 �����
	cvShowImage("rrr", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	cvWaitKey(0);
	cvDestroyWindow("rrr");
	////////////////////////////////////////////////////////////////////////////////////////////	


	return true;
}
bool RecognitionChar::IsAlpha_l(int* CharXY, int SizeX, int SizeY) {

	//////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* image;
	image = cvLoadImage("ImageFolder/8-1.jpg");
	//src = ReceiveImg();//�����ν��� �̹����� �����Ͽ� ��ȯ
	ChangeImgToBinary ImageToBinary = ChangeImgToBinary();//����ȭ���� ��ü ����
	IplImage* WhiteBlack;
	WhiteBlack = cvCloneImage(ImageToBinary.ConvertImgToBinary(image));//�̹����� ���ȭ�� ����ȭ���� ������

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
	cvNamedWindow("l������", 2);      // result ��� �̸��� �����츦 �����
	cvShowImage("l������", WhiteBlack);  // �� �����쿡 ���� ���� ipl_Img �̹����� ���̰�
	cvWaitKey(0);
	cvDestroyWindow("l������");
	////////////////////////////////////////////////////////////////////////////////////////////	

	//////////////////������
	//������ ���� �������� ���� �Ʒ��� �ȼ� ���� ������������� l�� �ƴ�
	if (!(-(SizeY * SizeX/10) < (NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) - NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2)) &&
		(NumOfUp(CharXY, SizeX, SizeY, SizeY / 2) - NumOfButtom(CharXY, SizeX, SizeY, SizeY / 2)) < +(SizeY * SizeX / 10)))
		return false;
	//���̰� �ʺ񺸴� 3���̻� ũ�������� l�� �ƴ�
	if (SizeX * 3 > SizeY)
		return false;
	
 	cout << "asd" << endl;



}
bool RecognitionChar::FitWord(string MyWord, string DicWord) {

	//�νı��ڳ��뿩��
	if (MyWord.length() < 0)//�νı����� ������ ������
		return false;

	int MyPos = 0;//�ν��Ѵܾ��� ��ġ
	int DicPos = 0;//�����ܾ��� ��ġ
	char MyChar = ' ';//�νĴܾ��� �����ϳ�
	char DicChar = ' ';//�����ܾ��� �����ϳ�

	//�ܾ��� ó���� ���� ������
	if ((MyWord.at(0) != DicWord.at(0) && MyWord.at(0) != '/') ||
		//�νĹ��ڿ� ���������� �ձ��ڰ������ʰ� �νĵ������� '/'���̸�
		(MyWord.at(MyWord.length() - 1) != DicWord.at(DicWord.length() - 1)
		&& MyWord.at(MyWord.length() - 1) != '/')){
		//�νĹ��ڿ� ���������� �ޱ��ڰ������ʰ�  �νĵ������� '/'���̸�
		return false;
	}
	
	//���� ���ɼ��� �ִ� �ܾ ���� ��
	MyPos = 0;//�νĴܾ���ġ ó������
	while (true) {
		MyChar = MyWord.at(MyPos);//�νĴܾ��� ��ġ�� �ش��ϴ� ���ڸ� ������
		DicChar = DicWord.at(DicPos);//�����ܾ��� ��ġ�� �ش��ϴ� ���ڸ� ������

		if (MyChar == '/') {//�νĴܾ��� �����ϳ��� ��Ȯ���� ���� �����̸�
			if (MyPos + 1 == MyWord.length()) {//�ν��� �ܾ��� ���� ���ڰ� ������
				return true;
			}
			else if (DicPos + 1 == DicWord.length()) {//�����ܾ��� ���� ���ڰ� ������
				return false;
			}
			MyPos++;//�ν��� �ܾ ������ġ��
			DicPos++;//���� �ܾ ������ġ��
		}
		else if (MyChar == DicChar) {//�δܾ��� ��ġ�� �ִ� ���ڰ��� ������
			if (MyPos + 1 == MyWord.length()) {//�ν��� �ܾ��� ���� ���ڰ� ������
				return true;    
			}
			else if (DicPos + 1 == DicWord.length()) {//�����ܾ��� ���� ���ڰ� ������
				return false;
			}
			MyPos++;//�νĴܾ��� ������ġ��
			DicPos++;//�����ܾ��� ������ġ��
		}
		else{// �δܾ��� ��ġ���ִ� ���ڰ��� �ٸ���
			if (0 < MyPos){//�νĴܾ��� �񱳱�����ġ�� ó���̾ƴϸ�
				if (MyWord.at(MyPos - 1) != '/'){//�ٷξ� ���ڰ� ��Ȯ���Ѱ��� �ƴϿ�����
					MyPos--;//�� ������ġ��
					return false;
				}
			}
			if (DicPos + 1 == DicWord.length()) {//�����ܾ��� ���� ���ڰ� ������
				return false;
			}
			DicPos++;//�����ܾ��� ������ġ��
		}
	}
}

void RecognitionChar::ConverToDicString(string& RecString, string& DicMean, int* StringLabel)
{
	//�ν��� ���ڿ��� ��Ȯ���� ���ڿ� ó��
	int MyPos;
	for (MyPos = 0; MyPos < RecString.length(); MyPos++) {//�νĹ��ڱ��̸�ŭ
		if (MyPos > 0){
			if (RecString.at(MyPos - 1) == RecString.at(MyPos) && RecString.at(MyPos) == '/'){// �Ǿ� �����̰ų� '/'�� �տ��� �ִ� ���
				RecString.erase(MyPos, 1);//  '/'�� ���ڿ����� ����
				MyPos--;
			}
		}
	}

	//�νĵ� ���ڿ��� ��Ȯ���Ѱ��� ������� 
	for (MyPos = 0; MyPos < RecString.length(); MyPos++) {//�νĹ��ڱ��̸�ŭ
		if (RecString.at(MyPos) != '/')//Ȯ���ϰ� �νĵ� �����̸�
			break;
	}
	if (MyPos == RecString.length()){//�νĹ����� Ȯ���ѹ��ڰ� �ϳ���������
		RecString = "NOT";
		DicMean = "NOT";
		return;
	}

	//�νĵ� ���ڿ��� ��Ȯ���Ѱ��� �������
	int num = 0;
	for (MyPos = 0; MyPos < RecString.length(); MyPos++) {//�νĹ��ڱ��̸�ŭ
		if (RecString.at(MyPos) == '/')//Ȯ���ϰ� �νĵ� �����̸�
			num++;
	}
	if (num > RecString.length() / 2.5) {//  �𸣴� ���ڰ� �ܾ��� 1/ 2.5 �� ������
		RecString = "NOT";
		DicMean = "NOT";
		return;
	}

	string FirstChar;
	FirstChar = RecString.at(0);
	FirstChar = "��������ڷ�����\dic_" + FirstChar;
	FirstChar = FirstChar + ".txt";

	fstream inFile;
	DicMean = "";
	string TempMean;
	string DicWord;
	string SimilarWord[50] = { "" };//�νĴܾ�� ������ �����ܾ��ĺ�
	string SimilarMean[50] = { "" };//�νĴܾ�� ������ �����ܾ��ĺ��� ��,�ǹ�
	int WLength = 0;//�����ܾ��ĺ�����		
	int MLength = 0;//�����ܾ��ĺ��� �� ����
	inFile.open(FirstChar);//�������Ͽ���

	while (!inFile.eof()){//������ ���ܾ��� �νĴܾ�� �����ϸ� ��������
		inFile >> DicWord;

		while (inFile.get() != '\n' && !inFile.eof()){
			inFile >> TempMean;
			DicMean += TempMean;
		}

		if (FitWord(RecString, DicWord)){//����ܾ�� �νĴܾ �����ϸ�
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
	//�ĺ��� �� label�� ���� �����Ѱ� ã��
	//.....�̿�
	//SimilarWord[WLength] = DicWord;
	//SimilarMean[MLength] = DicMean;
	//int* StringLabel
	//������ �����ϳ����������� ���ս��� �ܾ����

}

RecognitionChar::~RecognitionChar()
{
	//�����Ҵ� ����
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
