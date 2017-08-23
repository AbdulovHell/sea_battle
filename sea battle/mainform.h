#pragma once

namespace seabattle {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Net;
	using namespace System::Net::Sockets;

	/// <summary>
	/// —водка дл€ mainform
	/// </summary>
	public ref class mainform : public System::Windows::Forms::Form
	{
	public:
		mainform(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~mainform()
		{
			if (Stream != nullptr) {
				Stream->Close();
			}
			if (TCPclient != nullptr) {
				TCPclient->Close();
			}
			if (srv != nullptr) {
				srv->Stop();
			}
			if (tcp_tr != nullptr) {
				tcp_tr->Abort();
			}
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  ClientBtn;
	protected:

	private: System::Windows::Forms::TextBox^  PlayerNameEd;
	private: System::Windows::Forms::NumericUpDown^  SrvPortNum;
	private: System::Windows::Forms::Label^  StatusLbl;
	private: System::Windows::Forms::PictureBox^  pictureBox1;


	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Button^  StartSrvBtn;
	private: System::Windows::Forms::Label^  EnemyNameLbl;
	private: System::Windows::Forms::CheckBox^  MyReadyFlag;
	private: System::Windows::Forms::CheckBox^  EnemyReadyFlag;
	private: System::Windows::Forms::TextBox^  HostNameEdit;
	private: System::Windows::Forms::Button^  ConnectBtn;
	private: System::Windows::Forms::Button^  StartGameBtn;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::PictureBox^  Ship4Img;
	private: System::Windows::Forms::PictureBox^  Ship3Img;
	private: System::Windows::Forms::PictureBox^  Ship2Img;
	private: System::Windows::Forms::PictureBox^  Ship1Img;
	private: System::Windows::Forms::Label^  PointEstLbl;
	private: System::Windows::Forms::Button^  SrvBtn;




	private:
		enum class ProgStat : int {
			Main = 1,
			ServerSetup = 2,
			ServerWaitConn = 3,
			ServerWaitClientReady = 4,
			ServerGameStart = 5,
			ServerPlay = 6,
			ClientSetup = 7,
			ClientAttemptToConnect = 8,
			ClientConnected = 9
		};
		ref class ImageProvider {
		private:
			static System::ComponentModel::ComponentResourceManager^ Resource;
		public:
			ImageProvider();

			static Image^ Empty;
			static Image^ Damaged_Unknown;
			static Image^ Single_Destroyed;
			static Image^ Miss;
			static Image^ Ship4;
			static Image^ Ship3;
			static Image^ Ship2;
			static Image^ Single;
		};
		void Server();
		void SetStatusLbl(System::String^ msg);
		void InitGameSession();
		void PrepareArea(array<array<Button^>^>^ area, array<array<int>^>^ stat, bool isEnemyArea);
		void UpdateWnd();
		void SetEnemyName(System::String^ name);
		void UpdateEnemyFlag(bool stat);
		void Client();
		NetworkStream^ Stream;
		array<array<Button^>^>^ GameArea;
		array<array<int>^>^ AreaStat;
		array<array<Button^>^>^ EnemyArea;
		array<array<int>^>^ EnemyStat;
		ProgStat prgstat = ProgStat::Main;
		Thread^ tcp_tr;
		TcpClient^ TCPclient;
		TcpListener^ srv;
		Rectangle dragBoxFromMouseDown;
		Object^ tempImg;
		Point screenOffset;
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainform::typeid));
			this->ClientBtn = (gcnew System::Windows::Forms::Button());
			this->SrvBtn = (gcnew System::Windows::Forms::Button());
			this->PlayerNameEd = (gcnew System::Windows::Forms::TextBox());
			this->SrvPortNum = (gcnew System::Windows::Forms::NumericUpDown());
			this->StatusLbl = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->StartSrvBtn = (gcnew System::Windows::Forms::Button());
			this->EnemyNameLbl = (gcnew System::Windows::Forms::Label());
			this->MyReadyFlag = (gcnew System::Windows::Forms::CheckBox());
			this->EnemyReadyFlag = (gcnew System::Windows::Forms::CheckBox());
			this->HostNameEdit = (gcnew System::Windows::Forms::TextBox());
			this->ConnectBtn = (gcnew System::Windows::Forms::Button());
			this->StartGameBtn = (gcnew System::Windows::Forms::Button());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->Ship4Img = (gcnew System::Windows::Forms::PictureBox());
			this->Ship3Img = (gcnew System::Windows::Forms::PictureBox());
			this->Ship2Img = (gcnew System::Windows::Forms::PictureBox());
			this->Ship1Img = (gcnew System::Windows::Forms::PictureBox());
			this->PointEstLbl = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SrvPortNum))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship4Img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship3Img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship2Img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship1Img))->BeginInit();
			this->SuspendLayout();
			// 
			// ClientBtn
			// 
			this->ClientBtn->Location = System::Drawing::Point(53, 24);
			this->ClientBtn->Name = L"ClientBtn";
			this->ClientBtn->Size = System::Drawing::Size(75, 23);
			this->ClientBtn->TabIndex = 0;
			this->ClientBtn->Text = L"Client";
			this->ClientBtn->UseVisualStyleBackColor = true;
			this->ClientBtn->Click += gcnew System::EventHandler(this, &mainform::ClientBtn_Click);
			// 
			// SrvBtn
			// 
			this->SrvBtn->Location = System::Drawing::Point(53, 53);
			this->SrvBtn->Name = L"SrvBtn";
			this->SrvBtn->Size = System::Drawing::Size(75, 23);
			this->SrvBtn->TabIndex = 1;
			this->SrvBtn->Text = L"Server";
			this->SrvBtn->UseVisualStyleBackColor = true;
			this->SrvBtn->Click += gcnew System::EventHandler(this, &mainform::SrvBtn_Click);
			// 
			// PlayerNameEd
			// 
			this->PlayerNameEd->Location = System::Drawing::Point(28, 120);
			this->PlayerNameEd->Name = L"PlayerNameEd";
			this->PlayerNameEd->Size = System::Drawing::Size(120, 20);
			this->PlayerNameEd->TabIndex = 2;
			this->PlayerNameEd->Text = L"Player";
			this->PlayerNameEd->Visible = false;
			// 
			// SrvPortNum
			// 
			this->SrvPortNum->Location = System::Drawing::Point(28, 146);
			this->SrvPortNum->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 32000, 0, 0, 0 });
			this->SrvPortNum->Name = L"SrvPortNum";
			this->SrvPortNum->Size = System::Drawing::Size(120, 20);
			this->SrvPortNum->TabIndex = 3;
			this->SrvPortNum->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 4550, 0, 0, 0 });
			this->SrvPortNum->Visible = false;
			// 
			// StatusLbl
			// 
			this->StatusLbl->AutoSize = true;
			this->StatusLbl->Location = System::Drawing::Point(50, 189);
			this->StatusLbl->Name = L"StatusLbl";
			this->StatusLbl->Size = System::Drawing::Size(35, 13);
			this->StatusLbl->TabIndex = 4;
			this->StatusLbl->Text = L"label1";
			this->StatusLbl->Visible = false;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(34, 236);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(16, 16);
			this->pictureBox1->TabIndex = 5;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Visible = false;
			// 
			// button1
			// 
			this->button1->AllowDrop = true;
			this->button1->AutoSize = true;
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->button1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button1.Image")));
			this->button1->Location = System::Drawing::Point(12, 280);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(53, 43);
			this->button1->TabIndex = 8;
			this->button1->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageAboveText;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &mainform::ChangeImg);
			this->button1->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &mainform::button1_DragDrop);
			this->button1->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &mainform::button1_DragOver);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(12, 236);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(16, 16);
			this->pictureBox2->TabIndex = 9;
			this->pictureBox2->TabStop = false;
			this->pictureBox2->Visible = false;
			// 
			// StartSrvBtn
			// 
			this->StartSrvBtn->Location = System::Drawing::Point(53, 82);
			this->StartSrvBtn->Name = L"StartSrvBtn";
			this->StartSrvBtn->Size = System::Drawing::Size(75, 23);
			this->StartSrvBtn->TabIndex = 10;
			this->StartSrvBtn->Text = L"Start";
			this->StartSrvBtn->UseVisualStyleBackColor = true;
			this->StartSrvBtn->Visible = false;
			this->StartSrvBtn->Click += gcnew System::EventHandler(this, &mainform::StartSrvBtn_Click);
			// 
			// EnemyNameLbl
			// 
			this->EnemyNameLbl->AutoSize = true;
			this->EnemyNameLbl->Location = System::Drawing::Point(237, 9);
			this->EnemyNameLbl->Name = L"EnemyNameLbl";
			this->EnemyNameLbl->Size = System::Drawing::Size(35, 13);
			this->EnemyNameLbl->TabIndex = 11;
			this->EnemyNameLbl->Text = L"label1";
			this->EnemyNameLbl->Visible = false;
			// 
			// MyReadyFlag
			// 
			this->MyReadyFlag->AutoSize = true;
			this->MyReadyFlag->Location = System::Drawing::Point(240, 349);
			this->MyReadyFlag->Name = L"MyReadyFlag";
			this->MyReadyFlag->Size = System::Drawing::Size(63, 17);
			this->MyReadyFlag->TabIndex = 12;
			this->MyReadyFlag->Text = L"Ready\?";
			this->MyReadyFlag->UseVisualStyleBackColor = true;
			this->MyReadyFlag->Visible = false;
			this->MyReadyFlag->CheckedChanged += gcnew System::EventHandler(this, &mainform::MyReadyFlag_CheckedChanged);
			// 
			// EnemyReadyFlag
			// 
			this->EnemyReadyFlag->AutoSize = true;
			this->EnemyReadyFlag->Enabled = false;
			this->EnemyReadyFlag->Location = System::Drawing::Point(240, 372);
			this->EnemyReadyFlag->Name = L"EnemyReadyFlag";
			this->EnemyReadyFlag->Size = System::Drawing::Size(80, 17);
			this->EnemyReadyFlag->TabIndex = 13;
			this->EnemyReadyFlag->Text = L"checkBox2";
			this->EnemyReadyFlag->UseVisualStyleBackColor = true;
			this->EnemyReadyFlag->Visible = false;
			// 
			// HostNameEdit
			// 
			this->HostNameEdit->Location = System::Drawing::Point(106, 232);
			this->HostNameEdit->Name = L"HostNameEdit";
			this->HostNameEdit->Size = System::Drawing::Size(100, 20);
			this->HostNameEdit->TabIndex = 14;
			this->HostNameEdit->Text = L"127.0.0.1";
			this->HostNameEdit->Visible = false;
			// 
			// ConnectBtn
			// 
			this->ConnectBtn->Location = System::Drawing::Point(131, 82);
			this->ConnectBtn->Name = L"ConnectBtn";
			this->ConnectBtn->Size = System::Drawing::Size(75, 23);
			this->ConnectBtn->TabIndex = 15;
			this->ConnectBtn->Text = L"Connect";
			this->ConnectBtn->UseVisualStyleBackColor = true;
			this->ConnectBtn->Visible = false;
			this->ConnectBtn->Click += gcnew System::EventHandler(this, &mainform::ConnectBtn_Click);
			// 
			// StartGameBtn
			// 
			this->StartGameBtn->Location = System::Drawing::Point(228, 314);
			this->StartGameBtn->Name = L"StartGameBtn";
			this->StartGameBtn->Size = System::Drawing::Size(75, 23);
			this->StartGameBtn->TabIndex = 16;
			this->StartGameBtn->Text = L"Start game";
			this->StartGameBtn->UseVisualStyleBackColor = true;
			this->StartGameBtn->Visible = false;
			this->StartGameBtn->Click += gcnew System::EventHandler(this, &mainform::StartGameBtn_Click);
			// 
			// pictureBox3
			// 
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(34, 258);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(16, 16);
			this->pictureBox3->TabIndex = 17;
			this->pictureBox3->TabStop = false;
			this->pictureBox3->Visible = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(12, 258);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(16, 16);
			this->pictureBox4->TabIndex = 18;
			this->pictureBox4->TabStop = false;
			this->pictureBox4->Visible = false;
			// 
			// Ship4Img
			// 
			this->Ship4Img->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Ship4Img.Image")));
			this->Ship4Img->Location = System::Drawing::Point(476, 146);
			this->Ship4Img->Name = L"Ship4Img";
			this->Ship4Img->Size = System::Drawing::Size(64, 16);
			this->Ship4Img->TabIndex = 19;
			this->Ship4Img->TabStop = false;
			this->Ship4Img->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseDown);
			this->Ship4Img->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseMove);
			this->Ship4Img->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseUp);
			// 
			// Ship3Img
			// 
			this->Ship3Img->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Ship3Img.Image")));
			this->Ship3Img->Location = System::Drawing::Point(476, 124);
			this->Ship3Img->Name = L"Ship3Img";
			this->Ship3Img->Size = System::Drawing::Size(48, 16);
			this->Ship3Img->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->Ship3Img->TabIndex = 20;
			this->Ship3Img->TabStop = false;
			this->Ship3Img->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseDown);
			this->Ship3Img->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseMove);
			this->Ship3Img->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseUp);
			// 
			// Ship2Img
			// 
			this->Ship2Img->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Ship2Img.Image")));
			this->Ship2Img->Location = System::Drawing::Point(476, 102);
			this->Ship2Img->Name = L"Ship2Img";
			this->Ship2Img->Size = System::Drawing::Size(32, 16);
			this->Ship2Img->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->Ship2Img->TabIndex = 21;
			this->Ship2Img->TabStop = false;
			this->Ship2Img->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseDown);
			this->Ship2Img->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseMove);
			this->Ship2Img->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseUp);
			// 
			// Ship1Img
			// 
			this->Ship1Img->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Ship1Img.Image")));
			this->Ship1Img->Location = System::Drawing::Point(476, 80);
			this->Ship1Img->Name = L"Ship1Img";
			this->Ship1Img->Size = System::Drawing::Size(16, 16);
			this->Ship1Img->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->Ship1Img->TabIndex = 22;
			this->Ship1Img->TabStop = false;
			this->Ship1Img->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseDown);
			this->Ship1Img->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseMove);
			this->Ship1Img->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseUp);
			// 
			// PointEstLbl
			// 
			this->PointEstLbl->AutoSize = true;
			this->PointEstLbl->Location = System::Drawing::Point(473, 58);
			this->PointEstLbl->Name = L"PointEstLbl";
			this->PointEstLbl->Size = System::Drawing::Size(35, 13);
			this->PointEstLbl->TabIndex = 23;
			this->PointEstLbl->Text = L"label1";
			// 
			// mainform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(952, 441);
			this->Controls->Add(this->PointEstLbl);
			this->Controls->Add(this->Ship1Img);
			this->Controls->Add(this->Ship2Img);
			this->Controls->Add(this->Ship3Img);
			this->Controls->Add(this->Ship4Img);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->StartGameBtn);
			this->Controls->Add(this->ConnectBtn);
			this->Controls->Add(this->HostNameEdit);
			this->Controls->Add(this->EnemyReadyFlag);
			this->Controls->Add(this->MyReadyFlag);
			this->Controls->Add(this->EnemyNameLbl);
			this->Controls->Add(this->StartSrvBtn);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->StatusLbl);
			this->Controls->Add(this->SrvPortNum);
			this->Controls->Add(this->PlayerNameEd);
			this->Controls->Add(this->SrvBtn);
			this->Controls->Add(this->ClientBtn);
			this->Name = L"mainform";
			this->Text = L"Sea Battle";
			this->Load += gcnew System::EventHandler(this, &mainform::mainform_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SrvPortNum))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship4Img))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship3Img))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship2Img))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship1Img))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void StartSrvBtn_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ClientBtn_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void SrvBtn_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MyReadyFlag_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ConnectBtn_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void StartGameBtn_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ChangeImg(System::Object^  sender, System::EventArgs^  e);
	private: System::Void mainform_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Ship4Img_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void Ship4Img_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void Ship4Img_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void button1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
	private: System::Void button1_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
};
}
