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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SrvPortNum))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
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
			this->pictureBox1->Location = System::Drawing::Point(38, 236);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(16, 16);
			this->pictureBox1->TabIndex = 5;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Visible = false;
			// 
			// button1
			// 
			this->button1->AutoSize = true;
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->button1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button1.Image")));
			this->button1->Location = System::Drawing::Point(38, 315);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(22, 22);
			this->button1->TabIndex = 8;
			this->button1->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageAboveText;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Visible = false;
			this->button1->Click += gcnew System::EventHandler(this, &mainform::ChangeImg);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(22, 275);
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
			// mainform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(952, 441);
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SrvPortNum))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
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
	};
}
