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
	System::ComponentModel::ComponentResourceManager^ resource = (gcnew System::ComponentModel::ComponentResourceManager(mainform::typeid));
	Image^ empty = (safe_cast<System::Drawing::Image^>(resource->GetObject("pictureBox2.Image")));

	area = gcnew array<array<Button^>^>(11);
	stat = gcnew array<array<int>^>(11);
	for (int i = 0; i < 11; i++) {
		area[i] = gcnew array<Button^>(11);
		stat[i] = gcnew array<int>(11);
		for (int j = 0; j < 11; j++) {
			area[i][j] = gcnew Button();
			stat[i][j] = 0;

			if (j == 0 && i == 0) {
				area[i][j]->Image = empty;
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
				area[i][j]->Image = empty;

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
	//
}
