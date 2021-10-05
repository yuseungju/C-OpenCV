#include "Application.h"


Application::Application()
{
	src =NULL;//(�����ִ� ȭ����) �巡���� �������� �̹����� ������
	HandledSrc = NULL;//(�����ִ� ȭ����) �巡���� �������� �̹����� ������
}
void Application::run()
{
	while (1){
		ShowAMean();
	}
} 
void Application::ShowAMean()
{	
	src = cvCloneImage(captureImg.GetCapImg());//(�����ִ� ȭ����) �巡���� �������� �̹����� ������
	//128�Ӱ�ġ�� ����ȭ
	ChangeImgToBinary convert;
	HandledSrc = cvCloneImage(src);
	HandledSrc = convert.GetPreProcess(HandledSrc, 3, 0);
	cvNamedWindow("My Window11", CV_WINDOW_NORMAL);
	cvMoveWindow("My Window11", 0, 0);
	cvShowImage("My Window11", HandledSrc);
	cvWaitKey(0);

	/*
	//Į���Ӱ�ȭ��� 1
	int CaseGetArea = 1;
	for (int CaseDetail = 38; CaseDetail < 58; CaseDetail += 4){
	//��ó��(����ȭ): ��Ⱚ�� �������� �����Ͽ� ������ ���������� ����ȭ
	HandledSrc = cvCloneImage(src);
	HandledSrc = convert.GetPreProcess(HandledSrc, CaseDetail, CaseGetArea);//���帹�� ������ ���, ����������
	}
	//Į���Ӱ�ȭ��� 2
	ChangeImgToBinary convert;
	int CaseGetArea = 0;
	for (int CaseDetail = 28; CaseDetail < 40; CaseDetail += 4){
	//��ó��(����ȭ): ��Ⱚ�� �������� �����Ͽ� ������ ���������� ����ȭ
	HandledSrc = cvCloneImage(src);
	HandledSrc = convert.GetPreProcess(HandledSrc, CaseDetail, CaseGetArea);//�ι�°�� ���� ������ ��ó��, ������ ���
	}*/

	//�����ν����� �νĴܾ��� ���� �޾ƿ�
	Labeling Labeling(HandledSrc);
	int* label = new int[Labeling.GetSize()];
	Labeling.GetXLabelingXY(label);//label�� �󺧸��� ��ǥ�� ������
	RecChar Rec;
	string RecStr = Rec.GetAString(label, Labeling.GetWidth(), Labeling.GetHeigth(), Labeling.GetMaxLabelXY());

	//�����ܾ�� ����ܾ� �˻��Ͽ� ���
	string SimilarWord[13] = { "" };//�νĴܾ�� ������ �����ܾ��ĺ�
	string SimilarMean[13] = { "" };//�νĴܾ�� ������ �����ܾ��ĺ��� ��,�ǹ�
	cout << RecStr << endl;
	ConverToDicString(RecStr, SimilarWord, SimilarMean, 13);
	captureImg.ToDialog(SimilarWord, SimilarMean, 13);
	return;
}
void Application::ConverToDicString(string RecString, string* SimilarWord, string* Similarmean, int LimitWordNum)
{
	int MyPos;
	string GetSimilarWord[250] = {""};
	string GetSimilarmean[250] = {""};

	fstream inFile;
	string DicMean = "";
	string TempMean;
	string DicWord;
	int WLength = 0;//�����ܾ��ĺ�����		
	int MLength = 0;//�����ܾ��ĺ��� �� ����
	inFile.open("dictionary.txt");//�������Ͽ���
	
	//������ ���ܾ��� �νĴܾ�� �����ϸ� ��������
	while (!inFile.eof()){//��ü�ܾŭ
		inFile >> DicWord;
		//�ܾ��� ���������� ����ܾ ��������
		string temp = "";
		for (int a = 0; a < DicWord.length(); a++){
			if (DicWord.at(a) != '/')
				temp += DicWord.at(a);
			else
				break;
		}
		DicWord = temp;

		//�氡������
		while (inFile.get() != '\n' && !inFile.eof()){
			inFile >> TempMean;
			DicMean += TempMean;
		}   
		
		int numsame = NumSame(RecString, DicWord);//�δܾ��� ������ ���ڼ��� ����
		//�������ڼ��� ���ڱ��̿� ���� ������� Ȯ��
		bool PossibleRec = false;
		if (RecString.length() == 1 && numsame > -1)
			PossibleRec = true;
		else if (RecString.length() == 2 && numsame >0)
			PossibleRec = true;
		else if (RecString.length() == 3 && numsame >1)
			PossibleRec = true;
		else if (RecString.length() == 4 && numsame >1)
			PossibleRec = true;
		else if (RecString.length() == 5 && numsame >1)
			PossibleRec = true;
		else if (RecString.length() == 6 && numsame >2)
			PossibleRec = true;
		else if (RecString.length() == 7 && numsame >3)
			PossibleRec = true;
		else if (RecString.length() == 8 && numsame >3)
			PossibleRec = true;
		else if (RecString.length() > 9 && numsame > -1)
			PossibleRec = true;

		if (PossibleRec){//�δܾ ����ϸ�

			//���߿��� ///�� ����
			TempMean = "";
			for (int a = 3; a < DicMean.length() - 3; a++)
				TempMean += DicMean.at(a);
			DicMean = TempMean;

			GetSimilarWord[WLength] = DicWord;
			GetSimilarmean[MLength] = DicMean;
			WLength++;
			MLength++;
			if (WLength >= 250) {
				break;
			}
		}
		DicMean = "";
	}

	inFile.close();

	//���簹���� ���帹���� size���� ���Ϸ� ����
	int NumeSame[250] = { 0 };
	for (int a = 0; a < WLength; a++)
		NumeSame[a] = NumSame(RecString, GetSimilarWord[a]);
	int temp;
	for (int a = 0; a < WLength - 1; a++){
		for (int b = a + 1; b < WLength; b++){
			if (NumeSame[a] < NumeSame[b]){
				temp = NumeSame[b];
				NumeSame[b] = NumeSame[a];
				NumeSame[a] = temp;
			}
		}
	}
	int LimitSame = NumeSame[LimitWordNum];
	int OutRowWord = 0;
	int InRowWord = 0;

	//���� ���絵�� ����� �ܾ �����Ͽ� ���ϴ� ����ŭ ����
	while (OutRowWord < WLength){
		if (NumSame(RecString, GetSimilarWord[OutRowWord]) > NumeSame[LimitWordNum]){
			SimilarWord[InRowWord] = GetSimilarWord[OutRowWord];
			Similarmean[InRowWord] = GetSimilarmean[OutRowWord];
			InRowWord++;
		}
		OutRowWord++;
	}
	OutRowWord = 0;
	while (OutRowWord< 250){
		if (NumSame(RecString, GetSimilarWord[OutRowWord]) == NumeSame[LimitWordNum]){
			SimilarWord[InRowWord] = GetSimilarWord[OutRowWord];
			Similarmean[InRowWord] = GetSimilarmean[OutRowWord];
			InRowWord++;
		}
		if (InRowWord >= LimitWordNum)
			break;
		OutRowWord++;
	}

	return;

}

int Application::NumSame(string MyWord, string DicWord) {

	//�νı��� ���ɼ�����
	if (MyWord.length() < 0)//�νı����� ������ ������
		return -1;
	if (MyWord.length() != DicWord.length())//�� �ܾ��� ���̰� �ٸ���
		return -1;

	//�δܾ�񱳽���
	int MyPos = 0;//�ν��Ѵܾ��� ��ġ
	int DicPos = 0;//�����ܾ��� ��ġ
	char MyChar = ' ';//�νĴܾ��� �����ϳ�
	char DicChar = ' ';//�����ܾ��� �����ϳ�
	int sameNum = 0;
	int differentNum = 0;

	MyPos = 0;//�νĴܾ���ġ ó������
	while (true) {
		if (DicPos == DicWord.length()) {//�δܾ ��κ�������
			break;
		}
		MyChar = MyWord.at(MyPos);//�νĴܾ��� ��ġ�� �ش��ϴ� ���ڸ� ������
		DicChar = DicWord.at(DicPos);//�����ܾ��� ��ġ�� �ش��ϴ� ���ڸ� ������

		if (MyChar == DicChar) {//�δܾ��� ��ġ�� �ִ� ���ڰ��� ������
			sameNum++;
			MyPos++;//�νĴܾ��� ������ġ��
			DicPos++;//�����ܾ��� ������ġ��
		}
		else{
			differentNum++;
			MyPos++;//�νĴܾ��� ������ġ��
			DicPos++;//�����ܾ��� ������ġ��
		}
	}
		return sameNum;
}


Application::~Application()
{
	cvReleaseImage(&src);
	cvReleaseImage(&HandledSrc);
}
