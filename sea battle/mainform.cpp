#include <stdint.h>
#include "mainform.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Net;
using namespace System::Net::Sockets;

[STAThreadAttribute]
void Main(cli::array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	seabattle::mainform^ mainwnd = gcnew seabattle::mainform();
	Application::Run(mainwnd);
}

void seabattle::mainform::Server()
{
	Int32 port = Decimal::ToInt32(SrvPortNum->Value);
	IPAddress^ localAddr = IPAddress::Parse("127.0.0.1");
	srv = gcnew TcpListener(localAddr, port);
	srv->Start();

	array<Byte>^ bytes = gcnew array<Byte>(256);
	String^ EnemyName = nullptr;

	while (true) {
		this->Invoke(gcnew Action<String^>(this, &mainform::SetStatusLbl), "Waiting for connection...");

		TCPclient = srv->AcceptTcpClient();
		this->Invoke(gcnew Action<String^>(this, &mainform::SetStatusLbl), "Connected");

		Stream = TCPclient->GetStream();
		int i;
		try {
			do {
				i = Stream->Read(bytes, 0, bytes->Length);
				if (i > 0)
					switch (bytes[0]) {
					case 11:	//ready flags
					{
						this->Invoke(gcnew Action<bool>(this, &mainform::UpdateEnemyFlag), (bool)bytes[1]);
					}
					break;
					case 48:	//hello
					{
						prgstat = ProgStat::ServerWaitClientReady;
						EnemyName = System::Text::Encoding::ASCII->GetString(bytes, 2, 2 + bytes[1]);
						this->Invoke(gcnew Action<String^>(this, &mainform::SetEnemyName), EnemyName);
						this->Invoke(gcnew Action(this, &mainform::UpdateWnd));

						uint8_t len = PlayerNameEd->Text->Length;
						array<Byte>^ msg = gcnew array<Byte>(PlayerNameEd->Text->Length + 2);
						msg[0] = 49;
						msg[1] = len;
						System::Text::Encoding::ASCII->GetBytes(PlayerNameEd->Text->ToCharArray(), 0, len, msg, 2);
						Stream->Write(msg, 0, msg->Length);
					}
					break;
					}
			} while (1);
		}
		catch (...) {}
		try {
			TCPclient->Close();
		}
		catch (...) {}
	}
}

void seabattle::mainform::SetStatusLbl(System::String ^ msg)
{
	StatusLbl->Text = msg;
}

void seabattle::mainform::InitGameSession()
{
	PrepareArea(EnemyArea, EnemyStat, true);
}

void seabattle::mainform::PrepareArea(array<array<Button^>^>^ area, array<array<int>^>^ stat, bool isEnemyArea)
{
	area = gcnew array<array<Button^>^>(11);
	stat = gcnew array<array<int>^>(11);
	for (int i = 0; i < 11; i++) {
		area[i] = gcnew array<Button^>(11);
		stat[i] = gcnew array<int>(11);
		for (int j = 0; j < 11; j++) {
			area[i][j] = gcnew Button();
			stat[i][j] = 0;

			if (j == 0 && i == 0) {
				area[i][j]->Image = ImageProvider::Empty;
				area[i][j]->Visible = false;
			}
			else if (j == 0 && i != 0) {
				char txt[2] = { 'A' + i - 1 ,0 };
				area[i][j]->Text = gcnew String(txt);
				area[i][j]->Enabled = false;
			}
			else if (j != 0 && i == 0) {
				char txt[3] = { 0 ,0 ,0 };
				if (j < 10)
					txt[0] = '0' + j;
				else {
					txt[0] = '1';
					txt[1] = '0';
					area[i][j]->Font = gcnew Drawing::Font(area[i][j]->Font->FontFamily, 6);
				}
				area[i][j]->Text = gcnew String(txt);
				area[i][j]->Enabled = false;
			}
			else
				area[i][j]->Image = ImageProvider::Empty;

			//char name[3] = { 'A' + j - 1 ,'0' + i,0 };
			//area[i][j]->Name = gcnew String(name);
			if (isEnemyArea)
				area[i][j]->Location = System::Drawing::Point(500 + i * 22, 40 + j * 22);
			else
				area[i][j]->Location = System::Drawing::Point(200 + i * 22, 40 + j * 22);
			area[i][j]->Size = System::Drawing::Size(22, 22);
			area[i][j]->TabStop = false;

			area[i][j]->Click += gcnew System::EventHandler(this, &mainform::ChangeImg);
			this->Controls->Add(area[i][j]);
		}
	}
}

void seabattle::mainform::UpdateWnd()
{
	switch (prgstat) {
	case ProgStat::ServerSetup:
	{
		//hide srv & clnt btn
		SrvBtn->Visible = false;
		ClientBtn->Visible = false;
		//show player name edit, port num, start srv btn
		PlayerNameEd->Visible = true;
		SrvPortNum->Visible = true;
		StartSrvBtn->Visible = true;
		this->Text = this->Text + "(Server)";
	}
	break;
	case ProgStat::ServerWaitConn:
	{
		//hide prev, show player name, show conn stat label, start srv
		PlayerNameEd->Visible = false;
		SrvPortNum->Visible = false;
		StartSrvBtn->Visible = false;

		StatusLbl->Visible = true;

		tcp_tr = gcnew Thread(gcnew ThreadStart(this, &mainform::Server));
		tcp_tr->Start();
	}
	break;
	case ProgStat::ServerWaitClientReady:
	{
		//show conn stat, show enemy name
		//prepare game area
		PrepareArea(GameArea, AreaStat, false);
		//show placing ships interface
		Ship1Img->Visible = true;
		Ship2Img->Visible = true;
		Ship3Img->Visible = true;
		Ship4Img->Visible = true;
		PointEstLbl->Visible = true;
		PointEstLbl->Text = "20";
		//ready flags
		EnemyReadyFlag->Visible = true;
		MyReadyFlag->Visible = true;
	}
	break;
	case ProgStat::ServerGameStart:
	{
		//prepare enemy area
		//start game
	}
	break;
	case ProgStat::ServerPlay:
	{
		//
	}
	break;
	case ProgStat::ClientSetup:
	{
		//hide srv & clnt btn
		SrvBtn->Visible = false;
		ClientBtn->Visible = false;
		//show player name, port, host name
		PlayerNameEd->Visible = true;
		SrvPortNum->Visible = true;
		HostNameEdit->Visible = true;
		//conn btn
		ConnectBtn->Visible = true;
		this->Text = this->Text + "(Client)";
	}
	break;
	case ProgStat::ClientAttemptToConnect:
	{
		PlayerNameEd->Visible = false;
		SrvPortNum->Visible = false;
		HostNameEdit->Visible = false;
		//conn btn
		ConnectBtn->Visible = false;

		StatusLbl->Visible = true;
		tcp_tr = gcnew Thread(gcnew ThreadStart(this, &mainform::Client));
		tcp_tr->Start();
	}
	break;
	case ProgStat::ClientConnected:
	{
		PrepareArea(GameArea, AreaStat, false);

		SetStatusLbl("Connected");
		EnemyReadyFlag->Visible = true;
		MyReadyFlag->Visible = true;
	}
	break;
	}
}

void seabattle::mainform::SetEnemyName(System::String ^ name)
{
	EnemyNameLbl->Text = name;
	EnemyReadyFlag->Text = name;
}

void seabattle::mainform::UpdateEnemyFlag(bool stat)
{
	EnemyReadyFlag->Checked = stat;

	if (prgstat == ProgStat::ServerWaitClientReady && MyReadyFlag->Checked && EnemyReadyFlag->Checked)
		StartGameBtn->Visible = true;
	else
		StartGameBtn->Visible = false;
}

void seabattle::mainform::Client()
{
	Int32 port = Decimal::ToInt32(SrvPortNum->Value);
	this->Invoke(gcnew Action<String^>(this, &mainform::SetStatusLbl), "Connecting...");
	TCPclient = gcnew TcpClient(HostNameEdit->Text, port);

	Stream = TCPclient->GetStream();

	uint8_t len = PlayerNameEd->Text->Length;
	array<Byte>^ msg = gcnew array<Byte>(PlayerNameEd->Text->Length + 2);
	msg[0] = 48;
	msg[1] = len;
	System::Text::Encoding::ASCII->GetBytes(PlayerNameEd->Text->ToCharArray(), 0, len, msg, 2);
	Stream->Write(msg, 0, msg->Length);
	array<Byte>^ bytes = gcnew array<Byte>(256);
	int i;
	try {
		do {
			i = Stream->Read(bytes, 0, bytes->Length);

			if (i > 0)
				switch (bytes[0]) {
				case 11:	//ready flags
				{
					this->Invoke(gcnew Action<bool>(this, &mainform::UpdateEnemyFlag), (bool)bytes[1]);
				}
				break;
				case 49:	//oh, hi!
				{
					prgstat = ProgStat::ClientConnected;
					String^ EnemyName = System::Text::Encoding::ASCII->GetString(bytes, 2, 2 + bytes[1]);
					this->Invoke(gcnew Action<String^>(this, &mainform::SetEnemyName), EnemyName);
					this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
				}
				break;
				}
		} while (1);
	}
	catch (...) {}
	try {
		TCPclient->Close();
	}
	catch (...) {}
}

System::Void seabattle::mainform::StartSrvBtn_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	prgstat = ProgStat::ServerWaitConn;
	UpdateWnd();
}

System::Void seabattle::mainform::ClientBtn_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	prgstat = ProgStat::ClientSetup;
	UpdateWnd();
}

System::Void seabattle::mainform::SrvBtn_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	prgstat = ProgStat::ServerSetup;
	UpdateWnd();
}

System::Void seabattle::mainform::MyReadyFlag_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e)
{
	array<Byte>^ msg = gcnew array<Byte>(2);
	msg[0] = 11;
	msg[1] = MyReadyFlag->Checked;
	Stream->Write(msg, 0, msg->Length);

	if (prgstat == ProgStat::ServerWaitClientReady && MyReadyFlag->Checked && EnemyReadyFlag->Checked)
		StartGameBtn->Visible = true;
	else
		StartGameBtn->Visible = false;
}

System::Void seabattle::mainform::ConnectBtn_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	prgstat = ProgStat::ClientAttemptToConnect;
	UpdateWnd();
}

System::Void seabattle::mainform::StartGameBtn_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	prgstat = ProgStat::ServerGameStart;
	UpdateWnd();
}

System::Void seabattle::mainform::ChangeImg(System::Object ^ sender, System::EventArgs ^ e)
{
	Button^ btn = dynamic_cast<Button^>(sender);
	Random rnd;
	int a = rnd.Next(1, 5);

	switch (a)
	{
	case 1:
		btn->Image = ImageProvider::Empty;
		break;
	case 2:
		btn->Image = ImageProvider::Damaged_Unknown;
		break;
	case 3:
		btn->Image = ImageProvider::Single_Destroyed;
		break;
	case 4:
		btn->Image = ImageProvider::Miss;
		break;
	default:
		break;
	}
}

System::Void seabattle::mainform::mainform_Load(System::Object ^ sender, System::EventArgs ^ e)
{
	ImageProvider::ImageProvider();
}

System::Void seabattle::mainform::Ship4Img_MouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	tempImg = sender;
	if (tempImg != nullptr) {
		System::Drawing::Size dragSize = SystemInformation::DragSize;

		dragBoxFromMouseDown = Rectangle(Point(e->X - (dragSize.Width / 2), e->Y - (dragSize.Height / 2)), dragSize);
	}
	else
		dragBoxFromMouseDown = Rectangle::Empty;
}

System::Void seabattle::mainform::Ship4Img_MouseMove(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	if ((e->Button & System::Windows::Forms::MouseButtons::Left) == System::Windows::Forms::MouseButtons::Left) {
		if (dragBoxFromMouseDown != Rectangle::Empty && !dragBoxFromMouseDown.Contains(e->X, e->Y)) {
			screenOffset = SystemInformation::WorkingArea.Location;
			DragDropEffects dropEffect = this->Ship4Img->DoDragDrop(this->tempImg, static_cast<DragDropEffects>(DragDropEffects::Copy));
		}
	}
}

System::Void seabattle::mainform::Ship4Img_MouseUp(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	dragBoxFromMouseDown = Rectangle::Empty;
}

System::Void seabattle::mainform::button1_DragDrop(System::Object ^ sender, System::Windows::Forms::DragEventArgs ^ e)
{
	if (e->Data->GetDataPresent(PictureBox::typeid)) {
		PictureBox^ item = dynamic_cast<PictureBox^>(e->Data->GetData(PictureBox::typeid));
		if (e->Effect == DragDropEffects::Copy) {
			if (item != nullptr) {
				//TODO: вставляем кораблик!

				//Test--------------------------------------------------
				Button^ btn = dynamic_cast<Button^>(sender);
				//btn->Image = item->Image;
				//if (item->Image->Width == 64)
				//	btn->Image = ImageProvider::Ship4;
				switch (item->Image->Width) {
				case 16:
					btn->Image = ImageProvider::Single;
					break;
				case 32:
					btn->Image = ImageProvider::Ship2;
					break;
				case 48:
					btn->Image = ImageProvider::Ship3;
					break;
				case 64:
					btn->Image = ImageProvider::Ship4;
					break;
				default:
					btn->Image = ImageProvider::Miss;
					break;
				}
				//------------------------------------------------------
			}
		}
	}
}

System::Void seabattle::mainform::button1_DragOver(System::Object ^ sender, System::Windows::Forms::DragEventArgs ^ e)
{
	if (!e->Data->GetDataPresent(PictureBox::typeid)) {
		e->Effect = DragDropEffects::None;
		return;
	}
	if ((e->AllowedEffect&DragDropEffects::Copy) == DragDropEffects::Copy)
		e->Effect = DragDropEffects::Copy;
	else
		e->Effect = DragDropEffects::None;
}

seabattle::mainform::ImageProvider::ImageProvider()
{
	Resource = (gcnew System::ComponentModel::ComponentResourceManager(mainform::typeid));
	Empty = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox2.Image")));
	Damaged_Unknown = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox1.Image")));
	Single_Destroyed = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox3.Image")));
	Miss = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox4.Image")));
	Ship4 = (safe_cast<System::Drawing::Image^>(Resource->GetObject("Ship4Img.Image")));
	Ship3 = (safe_cast<System::Drawing::Image^>(Resource->GetObject("Ship3Img.Image")));
	Ship2 = (safe_cast<System::Drawing::Image^>(Resource->GetObject("Ship2Img.Image")));
	Single = (safe_cast<System::Drawing::Image^>(Resource->GetObject("Ship1Img.Image")));
}
