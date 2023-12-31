#include "MyForm.h"

using namespace testreadOutline;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Collections::Generic;



[STAThreadAttribute]

int main() {
	//�e�L�X�g�f�[�^�̓ǂݍ���
	String^ path = "C:\\Users\\chach\\Desktop\\Outline_testData.txt";
	MyForm::readFile(path);
	//��r���������Ĉ�v���X�g�ƕs��v���X�g�̍쐬
	MyForm::makeTopCompareList();
	//�t�@�C���̏o��
	MyForm::ExportText();

	Application::Run(gcnew MyForm());
	return 0;
}
//�t���O��S�ă��Z�b�g
void flgReset() {
	MyForm::custmer_top = false;
	MyForm::custmer_bottom = false;
	MyForm::local_top = false;
	MyForm::local_bottom = false;
}
//�w�b�_�[�ɉ����ăt���O�𗧂Ă�
void flgSet(String^ tmp) {
	if(tmp=="##Customer_Top") MyForm::custmer_top = true;
	if(tmp == "##Customer_Bottom") MyForm::custmer_bottom = true;
	if(tmp == "##Local_Top") MyForm::local_top = true;
	if(tmp == "##Local_Bottom") MyForm::local_bottom = true;
}
//�t���O�ɉ����Ċe���X�g���쐬
void selectData(List<List<MyForm::OutlineData^>^>^ list) {
	
	if (MyForm::custmer_top) MyForm::c_topData = list;
	if (MyForm::custmer_bottom) MyForm::c_bottomData = list;
	if (MyForm::local_top) MyForm::l_topData = list;
}
//�t�@�C���̓ǂݍ���
void testreadOutline::MyForm::readFile(String^ filepath)
{
	StreamReader^ sr = gcnew StreamReader(filepath);
	flgReset();
	String^ tmp;
	//�\���_��񃊃X�g
	cli::array<String^>^ tmpDataList;
	//�}�`���
	List<MyForm::OutlineData^>^ PrimInfoList = gcnew List<MyForm::OutlineData^>;
	//�}�ʑw���
	List<List<MyForm::OutlineData^>^>^ BigCategoryInfoList=gcnew List<List<MyForm::OutlineData^>^>;
	//�S�ēǂݍ��ނ܂Ŕ���
	try {
		while ((tmp = sr->ReadLine()) != nullptr) {

			//�󔒂̏ꍇ�͓ǂݔ�΂�
			if (tmp == "") continue;
			//�}�ʑw���̎�荞��
			if (tmp->Substring(0, 2) == "##") {
				//2��ڈȍ~�ł���΁A�}�`�f�[�^���擾
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
			//���w�b�_�[�̒�`
			//�}�`���̎擾
			else if (tmp->Substring(0, 1) == "#") {
				if (PrimInfoList->Count != 0) {
					BigCategoryInfoList->Add(PrimInfoList);
				}
				PrimInfoList = gcnew List<MyForm::OutlineData^>;
			}
			//�\���_���̎擾
			else if (tmp->Split(',')->Length == 4) {
				tmpDataList = tmp->Split(',');
				OutlineData^ tmpdata = MyForm::retOutlineData(tmpDataList);
				PrimInfoList->Add(tmpdata);
			}
			else {
				continue;

			}
		}
		//�Ō�̒ǉ�
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
//�\���_�̃��X�g�f�[�^��OutputData�̌`���ɕϊ�
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
	//local�̒��ň�v������̂�����C���f�b�N�X�̃��X�g
	List<int>^ MatchIndexList = gcnew List<int>;


	for (int i = 0; i < c_topData->Count; i++) {
		match = false;
		for (int j = 0; j < l_topData->Count; j++) {
			if (compareOutlineData(c_topData[i], l_topData[j])) {
				//�}�`�̊��S��v
				match = true;
				MatchIndexList->Add(j);
				diffmatchList->Add(c_topData[i]);
				break;
			}
		}
		if (match == false) {
			//c_topData[i]�̐}�`���s��v���X�g�Ɋi�[
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
			//���[�J���̕s��v�}�`��ǉ�
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


