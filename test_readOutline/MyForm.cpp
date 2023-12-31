#include "MyForm.h"

using namespace testreadOutline;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Collections::Generic;



[STAThreadAttribute]

int main() {
	//テキストデータの読み込み
	String^ path = "C:\\Users\\chach\\Desktop\\Outline_testData.txt";
	MyForm::readFile(path);
	//比較処理をして一致リストと不一致リストの作成
	MyForm::makeTopCompareList();
	//ファイルの出力
	MyForm::ExportText();

	Application::Run(gcnew MyForm());
	return 0;
}
//フラグを全てリセット
void flgReset() {
	MyForm::custmer_top = false;
	MyForm::custmer_bottom = false;
	MyForm::local_top = false;
	MyForm::local_bottom = false;
}
//ヘッダーに応じてフラグを立てる
void flgSet(String^ tmp) {
	if(tmp=="##Customer_Top") MyForm::custmer_top = true;
	if(tmp == "##Customer_Bottom") MyForm::custmer_bottom = true;
	if(tmp == "##Local_Top") MyForm::local_top = true;
	if(tmp == "##Local_Bottom") MyForm::local_bottom = true;
}
//フラグに応じて各リストを作成
void selectData(List<List<MyForm::OutlineData^>^>^ list) {
	
	if (MyForm::custmer_top) MyForm::c_topData = list;
	if (MyForm::custmer_bottom) MyForm::c_bottomData = list;
	if (MyForm::local_top) MyForm::l_topData = list;
}
//ファイルの読み込み
void testreadOutline::MyForm::readFile(String^ filepath)
{
	StreamReader^ sr = gcnew StreamReader(filepath);
	flgReset();
	String^ tmp;
	//構成点情報リスト
	cli::array<String^>^ tmpDataList;
	//図形情報
	List<MyForm::OutlineData^>^ PrimInfoList = gcnew List<MyForm::OutlineData^>;
	//図面層情報
	List<List<MyForm::OutlineData^>^>^ BigCategoryInfoList=gcnew List<List<MyForm::OutlineData^>^>;
	//全て読み込むまで反復
	try {
		while ((tmp = sr->ReadLine()) != nullptr) {

			//空白の場合は読み飛ばし
			if (tmp == "") continue;
			//図面層情報の取り込み
			if (tmp->Substring(0, 2) == "##") {
				//2回目以降であれば、図形データを取得
				if (PrimInfoList->Count != 0) {
					BigCategoryInfoList->Add(PrimInfoList);
					selectData(BigCategoryInfoList);
					BigCategoryInfoList = gcnew List<List<MyForm::OutlineData^>^>;
					PrimInfoList = gcnew List<MyForm::OutlineData^>;
				}
				flgReset();
				flgSet(tmp);
				continue;
			}
			//小ヘッダーの定義
			//図形情報の取得
			else if (tmp->Substring(0, 1) == "#") {
				if (PrimInfoList->Count != 0) {
					BigCategoryInfoList->Add(PrimInfoList);
				}
				PrimInfoList = gcnew List<MyForm::OutlineData^>;
			}
			//構成点情報の取得
			else if (tmp->Split(',')->Length == 4) {
				tmpDataList = tmp->Split(',');
				OutlineData^ tmpdata = MyForm::retOutlineData(tmpDataList);
				PrimInfoList->Add(tmpdata);
			}
			else {
				continue;

			}
		}
		//最後の追加
		BigCategoryInfoList->Add(PrimInfoList);
		MyForm::l_bottomData = BigCategoryInfoList;
	}
	catch (Exception^ ex) {
		MessageBox::Show(ex->ToString());
	}
	finally{
		sr->Close();
	}
}
//構成点のリストデータをOutputDataの形式に変換
MyForm::OutlineData^ testreadOutline::MyForm::retOutlineData(cli::array<String^>^ list)
{
	MyForm::OutlineData^ data=gcnew MyForm::OutlineData();
	int resX;
	int resY;
	int reswidth;
	int resradius;
	bool sucX = int::TryParse(list[0],resX);
	bool sucY = int::TryParse(list[1], resY);
	bool sucWidth = int::TryParse(list[2], reswidth);
	bool sucRadius = int::TryParse(list[3], resradius);


	if (sucX&&sucY&&sucWidth&&sucRadius) {
		data->X = resX;
		data->Y = resY;
		data->linewidth = reswidth;
		data->radius = resradius;

	}
	return data;
}

void testreadOutline::MyForm::makeTopCompareList()
{
	bool match = false;

	List<List<OutlineData^>^>^ diffmatchList = gcnew List<List<OutlineData^>^>;
	List<List<OutlineData^>^>^ diffUnmatchList = gcnew List<List<OutlineData^>^>;
	//localの中で一致するものがあるインデックスのリスト
	List<int>^ MatchIndexList = gcnew List<int>;


	for (int i = 0; i < c_topData->Count; i++) {
		match = false;
		for (int j = 0; j < l_topData->Count; j++) {
			if (compareOutlineData(c_topData[i], l_topData[j])) {
				//図形の完全一致
				match = true;
				MatchIndexList->Add(j);
				diffmatchList->Add(c_topData[i]);
				break;
			}
		}
		if (match == false) {
			//c_topData[i]の図形が不一致リストに格納
			diffUnmatchList->Add(c_topData[i]);
		}
	}

	bool matching = false;
	for (int i = 0; i < l_topData->Count; i++) {
		matching = false;
		for (int j = 0; j < MatchIndexList->Count; j++) {
			if (i == MatchIndexList[j]) {
				matching = true;
			}
		}
		if (matching == false) {
			//ローカルの不一致図形を追加
			diffUnmatchList->Add(l_topData[i]);
		}
	}

	UnmatchedList_Top = diffUnmatchList;
	MatchedList_Top = diffmatchList;

}

bool testreadOutline::MyForm::compareOutlineData(List<OutlineData ^>^ aPrim, List<OutlineData ^>^ bPrim)
{

	bool res = false;
	bool result = true;

	for (int i = 0; i < aPrim->Count; i++) {
		res = false;
		for (int j = 0; j < bPrim->Count; j++) {
			if (aPrim[i]->X == bPrim[j]->X&&aPrim[i]->Y == bPrim[j]->Y&&aPrim[i]->linewidth == bPrim[j]->linewidth&&aPrim[i]->radius == bPrim[j]->radius) {
				res = true;
				continue;
			}
		}
		if (res == false) {
			result = false;
			break;
		}
	}
	return result;
}

void testreadOutline::MyForm::paintTest()
{
	for (int i = 0; i < UnmatchedList_Top->Count; i++) {
		List<OutlineData^>^ tmpDataList = UnmatchedList_Top[i];
		for (int j = 0; j < tmpDataList->Count-1; j++) {
			MyForm::gr->DrawLine(pen, tmpDataList[j]->X,tmpDataList[j]->Y, tmpDataList[j+1]->X, tmpDataList[j+1]->Y);
		}
	}
}

void testreadOutline::MyForm::ExportText()
{
	String^ path1 = "C:\\Users\\chach\\Desktop\\Export_unMatched.txt";
	String^ path2 = "C:\\Users\\chach\\Desktop\\Export_Matched.txt";
	StreamWriter^ sw1 = gcnew StreamWriter(path1);

	String^ lineInfo = "";
	for (int i = 0; i < UnmatchedList_Top->Count; i++) {
		List<OutlineData^>^ tmpDataList = UnmatchedList_Top[i];
		lineInfo = "###"+"\n";
		for (int j = 0; j < tmpDataList->Count - 1; j++) {
			lineInfo += tmpDataList[j]->X + "," + tmpDataList[j]->Y + "," + tmpDataList[j]->linewidth + "," + tmpDataList[j]->radius;
			sw1->WriteLine(lineInfo);
			lineInfo = "";
		}
	}
	sw1->Close();
	StreamWriter^ sw2 = gcnew StreamWriter(path2);
	for (int i = 0; i < MatchedList_Top->Count; i++) {
		List<OutlineData^>^ tmpDataList = MatchedList_Top[i];
		lineInfo = "###"+"\n";
		for (int j = 0; j < tmpDataList->Count - 1; j++) {
			lineInfo += tmpDataList[j]->X + "," + tmpDataList[j]->Y + "," + tmpDataList[j]->linewidth + "," + tmpDataList[j]->radius;
			sw2->WriteLine(lineInfo);
			lineInfo = "";
		}
	}
	sw2->Close();
}

System::Void testreadOutline::MyForm::MyForm_Load(System::Object ^ sender, System::EventArgs ^ e)
{
	for (int i = 0; i < UnmatchedList_Top->Count; i++) {
		List<OutlineData^>^ tmpDataList = UnmatchedList_Top[i];
		for (int j = 0; j < tmpDataList->Count - 1; j++) {
			MyForm::gr->DrawLine(pen, tmpDataList[j]->X, tmpDataList[j]->Y, tmpDataList[j + 1]->X, tmpDataList[j + 1]->Y);
			pictureBox1->Refresh();
		}
	}
	for (int i = 0; i < MatchedList_Top->Count; i++) {
		List<OutlineData^>^ tmpDataList = MatchedList_Top[i];
		for (int j = 0; j < tmpDataList->Count - 1; j++) {
			MyForm::gr->DrawLine(pen2, tmpDataList[j]->X, tmpDataList[j]->Y, tmpDataList[j + 1]->X, tmpDataList[j + 1]->Y);
			pictureBox1->Refresh();
		}
	}
	pictureBox1->Refresh();
	return System::Void();
}


