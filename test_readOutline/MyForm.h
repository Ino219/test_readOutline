#pragma once

namespace testreadOutline {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm の概要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
			bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
			pictureBox1->Image = bmp;
			gr = Graphics::FromImage(pictureBox1->Image);
			pen = gcnew Pen(Color::Gray, 1);
			pen2 = gcnew Pen(Color::Red, 2);

		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	protected:

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::DarkSalmon;
			this->pictureBox1->Location = System::Drawing::Point(13, 13);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(420, 355);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(445, 380);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	public: bool static custmer_top;
	public: bool static custmer_bottom;
	public: bool static local_top;
	public: bool static local_bottom;
	public: static Bitmap^ bmp;
	public: static Graphics^ gr;
	public: static Pen^ pen;
	public: static Pen^ pen2;


	public: ref struct OutlineData{
		int X = this->X;
		int Y = this->Y;
		int linewidth = this->linewidth;
		int radius = this->radius;
	};
	public: void static readFile(String^ filepath);
	public: static MyForm::OutlineData^ retOutlineData(cli::array<String^>^ data);
	public: static List<List<MyForm::OutlineData^>^>^ c_topData = gcnew List<List<MyForm::OutlineData^>^>;
	public: static List<List<MyForm::OutlineData^>^>^ c_bottomData = gcnew List<List<MyForm::OutlineData^>^>;
	public: static List<List<MyForm::OutlineData^>^>^ l_topData = gcnew List<List<MyForm::OutlineData^>^>;
	public: static List<List<MyForm::OutlineData^>^>^ l_bottomData = gcnew List<List<MyForm::OutlineData^>^>;
	public:static void makeTopCompareList();
	public:static bool compareOutlineData(List<OutlineData^>^ a, List<OutlineData^>^ b);
	public:static List<List<OutlineData^>^>^ UnmatchedList_Top;
	public:static List<List<OutlineData^>^>^ MatchedList_Top;
	public:static void paintTest();
	public:static void ExportText();

	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e);
	};
}
