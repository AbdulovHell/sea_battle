#pragma once
#define _collomns 11
#define _strings 11
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
	/// Сводка для mainform
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
		/// Освободить все используемые ресурсы.
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
	private: System::Windows::Forms::PictureBox^  lpImg;
	private: System::Windows::Forms::PictureBox^  rpImg;
	private: System::Windows::Forms::PictureBox^  dpImg;
	private: System::Windows::Forms::PictureBox^  upImg;
	private: System::Windows::Forms::PictureBox^  singleImg;
	private: System::Windows::Forms::PictureBox^  rlpImg;
	private: System::Windows::Forms::PictureBox^  udpImg;
	private: System::Windows::Forms::PictureBox^  pictureBox5;
	private: System::Windows::Forms::PictureBox^  pictureBox6;
	private: System::Windows::Forms::PictureBox^  turnedShip2;
	private: System::Windows::Forms::Label^  SinglesLeft;
	private: System::Windows::Forms::Label^  Ships2Left;
	private: System::Windows::Forms::Label^  Ships3Left;
	private: System::Windows::Forms::Label^  Ships4Left;
	private: System::Windows::Forms::PictureBox^  pictureBox7;
	private: System::Windows::Forms::PictureBox^  pictureBox8;
	private: System::Windows::Forms::PictureBox^  pictureBox9;
	private: System::Windows::Forms::PictureBox^  pictureBox10;
	private: System::Windows::Forms::PictureBox^  pictureBox11;
	private: System::Windows::Forms::PictureBox^  pictureBox12;
	private: System::Windows::Forms::Label^  TurnLbl;
	private: System::Windows::Forms::Button^  SrvBtn;

	private:
		//Перечисление возможных статусов программы
		enum class ProgStat : int {
			Main = 1,
			ServerSetup = 2,
			ServerWaitConn = 3,
			ServerWaitClientReady = 4,
			ServerGameStart = 5,
			ClientSetup = 7,
			ClientAttemptToConnect = 8,
			ClientConnected = 9,
			MyTurn = 10,
			EnemyTurn = 11,
			ClientGameStart=12
		};
		//Перечисление возможных статусов клетки
		enum class CellIndex : int {
			Empty = 0,
			Damaged_Unknown = 1,
			Miss = 2,

			Single_Destroyed = 3,
			UpDown_Destroyed = 4,
			LeftRight_Destroyed = 5,
			Left_Destroyed = 6,
			Right_Destroyed = 7,
			Up_Destroyed = 8,
			Down_Destroyed = 9,

			Single = 13,
			UpDownPart = 14,
			LeftRightPart = 15,
			LeftPart = 16,
			RightPart = 17,
			UpPart = 18,
			DownPart = 19
		};
		//Класс, предоставляющий указатели на все наши изображения
		ref class ImageProvider {
		private:
			static System::ComponentModel::ComponentResourceManager^ Resource;
		public:
			//Конструктор по умолчания заполняет указатели обьектами Image
			ImageProvider();

			static Image^ Empty;
			static Image^ Damaged_Unknown;
			static Image^ Single_Destroyed;
			static Image^ Miss;
			static Image^ Ship4;
			static Image^ Ship3;
			static Image^ Ship2;
			static Image^ Single;

			static Image^ UpDownPart;
			static Image^ LeftRightPart;
			static Image^ LeftPart;
			static Image^ RightPart;
			static Image^ UpPart;
			static Image^ DownPart;

			static Image^ UpDown_Destroyed;
			static Image^ LeftRight_Destroyed;
			static Image^ Left_Destroyed;
			static Image^ Right_Destroyed;
			static Image^ Up_Destroyed;
			static Image^ Down_Destroyed;
		};
		//Функция, работающая во втором потоке, в случае когда программа работает в качестве сервера
		void Server();
		//Устанавливает текст о состоянии соединения
		void SetStatusLbl(System::String^ msg);
		//Не используется
		void InitGameSession();
		//Инициализация массивов своей или вражеской арены и расположение их в окне
		void PrepareArea(array<array<Button^>^>^ %area, array<array<int>^>^ %stat, bool isEnemyArea);
		//Обновление состояния окна по внутреннему статусу программы
		void UpdateWnd();
		//Устанавливает имя противника
		void SetEnemyName(System::String^ name);
		//Обновление флага готовности противника
		void UpdateEnemyFlag(bool stat);
		//Функция, работающая во втором потоке, в случае когда прогамма работает в качестве клиента
		void Client();
		//Проверка, возможно ли установить кораблся в поле
		bool CheckIfPlaceble(int I, int J, int ship, bool turned);
		//Установка кораблся в поле
		void PlaceShip(int I, int J, int ship, bool turned);
		//Удаление кораблся из поля
		void DeleteShip(int I, int J);
		//Функция обработка нажатия по полю противника
		void Shoot(int I, int J);
		//Проверка, убил ли наш корабль противник
		int TryKill(int I, int J, int* ship, bool* turned,int* sI,int* sJ);
		//Обновление картинки клетки в соответствии со статусом поля
		void SyncStat(int I, int J,bool isEnemyArea);
		//Обновление поля противника, в случае смерти его корабля целиком
		void Kill(int I, int J,int ship,bool turned);
		//Установка еденичного промаха
		void SetMiss(int I,int J);
		//Установка на нашем поле промахов вокруг нашего убитого кораблся
		void SetMiss(int I, int J, int ship, bool turned);
		int LastHitI;
		int LastHitJ;
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
		/// Требуемый метод для поддержки конструктора — не изменяйте 
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
			this->lpImg = (gcnew System::Windows::Forms::PictureBox());
			this->rpImg = (gcnew System::Windows::Forms::PictureBox());
			this->dpImg = (gcnew System::Windows::Forms::PictureBox());
			this->upImg = (gcnew System::Windows::Forms::PictureBox());
			this->singleImg = (gcnew System::Windows::Forms::PictureBox());
			this->rlpImg = (gcnew System::Windows::Forms::PictureBox());
			this->udpImg = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox6 = (gcnew System::Windows::Forms::PictureBox());
			this->turnedShip2 = (gcnew System::Windows::Forms::PictureBox());
			this->SinglesLeft = (gcnew System::Windows::Forms::Label());
			this->Ships2Left = (gcnew System::Windows::Forms::Label());
			this->Ships3Left = (gcnew System::Windows::Forms::Label());
			this->Ships4Left = (gcnew System::Windows::Forms::Label());
			this->pictureBox7 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox8 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox9 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox10 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox11 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox12 = (gcnew System::Windows::Forms::PictureBox());
			this->TurnLbl = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SrvPortNum))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship4Img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship3Img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship2Img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ship1Img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->lpImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rpImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dpImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->upImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->singleImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rlpImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->udpImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->turnedShip2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox10))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox11))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox12))->BeginInit();
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
			this->button1->Location = System::Drawing::Point(12, 258);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(53, 43);
			this->button1->TabIndex = 8;
			this->button1->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageAboveText;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Visible = false;
			this->button1->Click += gcnew System::EventHandler(this, &mainform::button1_Click);
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
			this->pictureBox3->Location = System::Drawing::Point(69, 372);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(16, 16);
			this->pictureBox3->TabIndex = 17;
			this->pictureBox3->TabStop = false;
			this->pictureBox3->Visible = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(53, 236);
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
			this->Ship4Img->Visible = false;
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
			this->Ship3Img->Visible = false;
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
			this->Ship2Img->Visible = false;
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
			this->Ship1Img->Visible = false;
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
			this->PointEstLbl->Visible = false;
			// 
			// lpImg
			// 
			this->lpImg->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"lpImg.Image")));
			this->lpImg->Location = System::Drawing::Point(12, 349);
			this->lpImg->Name = L"lpImg";
			this->lpImg->Size = System::Drawing::Size(16, 16);
			this->lpImg->TabIndex = 27;
			this->lpImg->TabStop = false;
			this->lpImg->Visible = false;
			// 
			// rpImg
			// 
			this->rpImg->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"rpImg.Image")));
			this->rpImg->Location = System::Drawing::Point(34, 349);
			this->rpImg->Name = L"rpImg";
			this->rpImg->Size = System::Drawing::Size(16, 16);
			this->rpImg->TabIndex = 26;
			this->rpImg->TabStop = false;
			this->rpImg->Visible = false;
			// 
			// dpImg
			// 
			this->dpImg->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"dpImg.Image")));
			this->dpImg->Location = System::Drawing::Point(12, 327);
			this->dpImg->Name = L"dpImg";
			this->dpImg->Size = System::Drawing::Size(16, 16);
			this->dpImg->TabIndex = 25;
			this->dpImg->TabStop = false;
			this->dpImg->Visible = false;
			// 
			// upImg
			// 
			this->upImg->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"upImg.Image")));
			this->upImg->Location = System::Drawing::Point(34, 327);
			this->upImg->Name = L"upImg";
			this->upImg->Size = System::Drawing::Size(16, 16);
			this->upImg->TabIndex = 24;
			this->upImg->TabStop = false;
			this->upImg->Visible = false;
			// 
			// singleImg
			// 
			this->singleImg->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"singleImg.Image")));
			this->singleImg->Location = System::Drawing::Point(12, 371);
			this->singleImg->Name = L"singleImg";
			this->singleImg->Size = System::Drawing::Size(16, 16);
			this->singleImg->TabIndex = 28;
			this->singleImg->TabStop = false;
			this->singleImg->Visible = false;
			// 
			// rlpImg
			// 
			this->rlpImg->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"rlpImg.Image")));
			this->rlpImg->Location = System::Drawing::Point(34, 393);
			this->rlpImg->Name = L"rlpImg";
			this->rlpImg->Size = System::Drawing::Size(16, 16);
			this->rlpImg->TabIndex = 30;
			this->rlpImg->TabStop = false;
			this->rlpImg->Visible = false;
			// 
			// udpImg
			// 
			this->udpImg->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"udpImg.Image")));
			this->udpImg->Location = System::Drawing::Point(34, 371);
			this->udpImg->Name = L"udpImg";
			this->udpImg->Size = System::Drawing::Size(16, 16);
			this->udpImg->TabIndex = 29;
			this->udpImg->TabStop = false;
			this->udpImg->Visible = false;
			// 
			// pictureBox5
			// 
			this->pictureBox5->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox5.Image")));
			this->pictureBox5->Location = System::Drawing::Point(555, 98);
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->Size = System::Drawing::Size(16, 64);
			this->pictureBox5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox5->TabIndex = 33;
			this->pictureBox5->TabStop = false;
			this->pictureBox5->Visible = false;
			this->pictureBox5->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseDown);
			this->pictureBox5->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseMove);
			this->pictureBox5->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseUp);
			// 
			// pictureBox6
			// 
			this->pictureBox6->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox6.Image")));
			this->pictureBox6->Location = System::Drawing::Point(533, 92);
			this->pictureBox6->Name = L"pictureBox6";
			this->pictureBox6->Size = System::Drawing::Size(16, 48);
			this->pictureBox6->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox6->TabIndex = 32;
			this->pictureBox6->TabStop = false;
			this->pictureBox6->Visible = false;
			this->pictureBox6->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseDown);
			this->pictureBox6->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseMove);
			this->pictureBox6->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseUp);
			// 
			// turnedShip2
			// 
			this->turnedShip2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"turnedShip2.Image")));
			this->turnedShip2->Location = System::Drawing::Point(514, 86);
			this->turnedShip2->Name = L"turnedShip2";
			this->turnedShip2->Size = System::Drawing::Size(16, 32);
			this->turnedShip2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->turnedShip2->TabIndex = 31;
			this->turnedShip2->TabStop = false;
			this->turnedShip2->Visible = false;
			this->turnedShip2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseDown);
			this->turnedShip2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseMove);
			this->turnedShip2->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::Ship4Img_MouseUp);
			// 
			// SinglesLeft
			// 
			this->SinglesLeft->AutoSize = true;
			this->SinglesLeft->Location = System::Drawing::Point(457, 82);
			this->SinglesLeft->Name = L"SinglesLeft";
			this->SinglesLeft->Size = System::Drawing::Size(13, 13);
			this->SinglesLeft->TabIndex = 34;
			this->SinglesLeft->Text = L"4";
			this->SinglesLeft->Visible = false;
			// 
			// Ships2Left
			// 
			this->Ships2Left->AutoSize = true;
			this->Ships2Left->Location = System::Drawing::Point(457, 105);
			this->Ships2Left->Name = L"Ships2Left";
			this->Ships2Left->Size = System::Drawing::Size(13, 13);
			this->Ships2Left->TabIndex = 35;
			this->Ships2Left->Text = L"3";
			this->Ships2Left->Visible = false;
			// 
			// Ships3Left
			// 
			this->Ships3Left->AutoSize = true;
			this->Ships3Left->Location = System::Drawing::Point(457, 127);
			this->Ships3Left->Name = L"Ships3Left";
			this->Ships3Left->Size = System::Drawing::Size(13, 13);
			this->Ships3Left->TabIndex = 36;
			this->Ships3Left->Text = L"2";
			this->Ships3Left->Visible = false;
			// 
			// Ships4Left
			// 
			this->Ships4Left->AutoSize = true;
			this->Ships4Left->Location = System::Drawing::Point(457, 148);
			this->Ships4Left->Name = L"Ships4Left";
			this->Ships4Left->Size = System::Drawing::Size(13, 13);
			this->Ships4Left->TabIndex = 37;
			this->Ships4Left->Text = L"1";
			this->Ships4Left->Visible = false;
			// 
			// pictureBox7
			// 
			this->pictureBox7->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox7.Image")));
			this->pictureBox7->Location = System::Drawing::Point(91, 394);
			this->pictureBox7->Name = L"pictureBox7";
			this->pictureBox7->Size = System::Drawing::Size(16, 16);
			this->pictureBox7->TabIndex = 43;
			this->pictureBox7->TabStop = false;
			this->pictureBox7->Visible = false;
			// 
			// pictureBox8
			// 
			this->pictureBox8->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox8.Image")));
			this->pictureBox8->Location = System::Drawing::Point(91, 372);
			this->pictureBox8->Name = L"pictureBox8";
			this->pictureBox8->Size = System::Drawing::Size(16, 16);
			this->pictureBox8->TabIndex = 42;
			this->pictureBox8->TabStop = false;
			this->pictureBox8->Visible = false;
			// 
			// pictureBox9
			// 
			this->pictureBox9->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox9.Image")));
			this->pictureBox9->Location = System::Drawing::Point(69, 350);
			this->pictureBox9->Name = L"pictureBox9";
			this->pictureBox9->Size = System::Drawing::Size(16, 16);
			this->pictureBox9->TabIndex = 41;
			this->pictureBox9->TabStop = false;
			this->pictureBox9->Visible = false;
			// 
			// pictureBox10
			// 
			this->pictureBox10->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox10.Image")));
			this->pictureBox10->Location = System::Drawing::Point(91, 350);
			this->pictureBox10->Name = L"pictureBox10";
			this->pictureBox10->Size = System::Drawing::Size(16, 16);
			this->pictureBox10->TabIndex = 40;
			this->pictureBox10->TabStop = false;
			this->pictureBox10->Visible = false;
			// 
			// pictureBox11
			// 
			this->pictureBox11->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox11.Image")));
			this->pictureBox11->Location = System::Drawing::Point(69, 328);
			this->pictureBox11->Name = L"pictureBox11";
			this->pictureBox11->Size = System::Drawing::Size(16, 16);
			this->pictureBox11->TabIndex = 39;
			this->pictureBox11->TabStop = false;
			this->pictureBox11->Visible = false;
			// 
			// pictureBox12
			// 
			this->pictureBox12->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox12.Image")));
			this->pictureBox12->Location = System::Drawing::Point(91, 328);
			this->pictureBox12->Name = L"pictureBox12";
			this->pictureBox12->Size = System::Drawing::Size(16, 16);
			this->pictureBox12->TabIndex = 38;
			this->pictureBox12->TabStop = false;
			this->pictureBox12->Visible = false;
			// 
			// TurnLbl
			// 
			this->TurnLbl->AutoSize = true;
			this->TurnLbl->Location = System::Drawing::Point(445, 34);
			this->TurnLbl->Name = L"TurnLbl";
			this->TurnLbl->Size = System::Drawing::Size(35, 13);
			this->TurnLbl->TabIndex = 44;
			this->TurnLbl->Text = L"label1";
			this->TurnLbl->Visible = false;
			// 
			// mainform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(952, 441);
			this->Controls->Add(this->TurnLbl);
			this->Controls->Add(this->pictureBox7);
			this->Controls->Add(this->pictureBox8);
			this->Controls->Add(this->pictureBox9);
			this->Controls->Add(this->pictureBox10);
			this->Controls->Add(this->pictureBox11);
			this->Controls->Add(this->pictureBox12);
			this->Controls->Add(this->Ships4Left);
			this->Controls->Add(this->Ships3Left);
			this->Controls->Add(this->Ships2Left);
			this->Controls->Add(this->SinglesLeft);
			this->Controls->Add(this->pictureBox5);
			this->Controls->Add(this->pictureBox6);
			this->Controls->Add(this->turnedShip2);
			this->Controls->Add(this->rlpImg);
			this->Controls->Add(this->udpImg);
			this->Controls->Add(this->singleImg);
			this->Controls->Add(this->lpImg);
			this->Controls->Add(this->rpImg);
			this->Controls->Add(this->dpImg);
			this->Controls->Add(this->upImg);
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->lpImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rpImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dpImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->upImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->singleImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rlpImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->udpImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->turnedShip2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox10))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox11))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox12))->EndInit();
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
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
	};
}
