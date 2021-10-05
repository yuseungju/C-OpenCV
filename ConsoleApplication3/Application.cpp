#include "Application.h"


Application::Application()
{
	src =NULL;//(보고있는 화면중) 드래그한 영역만의 이미지를 가져옴
	HandledSrc = NULL;//(보고있는 화면중) 드래그한 영역만의 이미지를 가져옴
}
void Application::run()
{
	while (1){
		ShowAMean();
	}
} 
void Application::ShowAMean()
{	
	src = cvCloneImage(captureImg.GetCapImg());//(보고있는 화면중) 드래그한 영역만의 이미지를 가져옴
	//128임계치로 이진화
	ChangeImgToBinary convert;
	HandledSrc = cvCloneImage(src);
	HandledSrc = convert.GetPreProcess(HandledSrc, 3, 0);
	cvNamedWindow("My Window11", CV_WINDOW_NORMAL);
	cvMoveWindow("My Window11", 0, 0);
	cvShowImage("My Window11", HandledSrc);
	cvWaitKey(0);

	/*
	//칼라임계화방식 1
	int CaseGetArea = 1;
	for (int CaseDetail = 38; CaseDetail < 58; CaseDetail += 4){
	//전처리(이진화): 밝기값을 영역으로 분할하여 차지한 비율에따라 이진화
	HandledSrc = cvCloneImage(src);
	HandledSrc = convert.GetPreProcess(HandledSrc, CaseDetail, CaseGetArea);//가장많은 영역만 흰색, 나머지검정
	}
	//칼라임계화방식 2
	ChangeImgToBinary convert;
	int CaseGetArea = 0;
	for (int CaseDetail = 28; CaseDetail < 40; CaseDetail += 4){
	//전처리(이진화): 밝기값을 영역으로 분할하여 차지한 비율에따라 이진화
	HandledSrc = cvCloneImage(src);
	HandledSrc = convert.GetPreProcess(HandledSrc, CaseDetail, CaseGetArea);//두번째로 많은 영역만 블랙처리, 나머지 흰색
	}*/

	//문자인식으로 인식단어의 값을 받아옴
	Labeling Labeling(HandledSrc);
	int* label = new int[Labeling.GetSize()];
	Labeling.GetXLabelingXY(label);//label로 라벨링된 좌표를 가져옴
	RecChar Rec;
	string RecStr = Rec.GetAString(label, Labeling.GetWidth(), Labeling.GetHeigth(), Labeling.GetMaxLabelXY());

	//얻은단어로 유사단어 검색하여 출력
	string SimilarWord[13] = { "" };//인식단어와 유사한 사전단어후보
	string SimilarMean[13] = { "" };//인식단어와 유사한 사전단어후보의 뜻,의미
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
	int WLength = 0;//사전단어후보길이		
	int MLength = 0;//사전단어후보의 뜻 길이
	inFile.open("dictionary.txt");//사전파일열기
	
	//파일의 모든단어중 인식단어와 유사하면 값을보관
	while (!inFile.eof()){//전체단어만큼
		inFile >> DicWord;
		//단어중 뜻은버리고 영어단어만 가져오기
		string temp = "";
		for (int a = 0; a < DicWord.length(); a++){
			if (DicWord.at(a) != '/')
				temp += DicWord.at(a);
			else
				break;
		}
		DicWord = temp;

		//뜻가져오기
		while (inFile.get() != '\n' && !inFile.eof()){
			inFile >> TempMean;
			DicMean += TempMean;
		}   
		
		int numsame = NumSame(RecString, DicWord);//두단어의 유사한 문자수를 얻어옴
		//같은문자수와 문자길이에 따라 비슷한지 확인
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

		if (PossibleRec){//두단어가 비슷하면

			//뜻중에서 ///를 제거
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

	//유사갯수가 가장많은값 size개수 이하로 선택
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

	//가장 유사도가 비슷한 단어를 선출하여 원하는 수만큼 얻음
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

	//인식글자 가능성조사
	if (MyWord.length() < 0)//인식글자의 내용이 없으면
		return -1;
	if (MyWord.length() != DicWord.length())//두 단어의 길이가 다르면
		return -1;

	//두단어비교시작
	int MyPos = 0;//인식한단어의 위치
	int DicPos = 0;//사전단어의 위치
	char MyChar = ' ';//인식단어의 문자하나
	char DicChar = ' ';//사전단어의 문자하나
	int sameNum = 0;
	int differentNum = 0;

	MyPos = 0;//인식단어위치 처음으로
	while (true) {
		if (DicPos == DicWord.length()) {//두단어를 모두비교했으면
			break;
		}
		MyChar = MyWord.at(MyPos);//인식단어의 위치에 해당하는 문자를 가져옴
		DicChar = DicWord.at(DicPos);//사전단어의 위치에 해당하는 문자를 가져옴

		if (MyChar == DicChar) {//두단어의 위치에 있는 문자값이 같으면
			sameNum++;
			MyPos++;//인식단어의 다음위치로
			DicPos++;//사전단어의 다음위치로
		}
		else{
			differentNum++;
			MyPos++;//인식단어의 다음위치로
			DicPos++;//사전단어의 다음위치로
		}
	}
		return sameNum;
}


Application::~Application()
{
	cvReleaseImage(&src);
	cvReleaseImage(&HandledSrc);
}
