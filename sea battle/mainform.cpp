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
	srv = gcnew TcpListener(/*localAddr*/IPAddress::Any, port);
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
					case 50:
					{
						int I = bytes[1], J = bytes[2];

						array<Byte>^ msg = gcnew array<Byte>(7);
						msg[0] = 51;

						int ship = 0;
						bool turned = false;
						int sI = 0, sJ = 0;

						if (AreaStat[I][J] > 12) {
							msg[1] = 1;	//hit
							msg[2] = TryKill(I, J, &ship, &turned, &sI, &sJ);
						}
						else {
							msg[1] = 0;	//miss
							msg[2] = 0;
							SetMiss(I, J);

							prgstat = ProgStat::MyTurn;
							this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
						}
						msg[3] = ship;
						msg[4] = turned;
						msg[5] = sI;
						msg[6] = sJ;
						Stream->Write(msg, 0, msg->Length);

						if (AreaStat[0][1] == 4 && AreaStat[0][2] == 3 && AreaStat[0][3] == 2 && AreaStat[0][4] == 1) {
							prgstat = ProgStat::Lose;
							this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
						}
					}
					break;
					case 51:
					{
						if (bytes[1]) {	//hit
							if (bytes[2]) {	//destroyed
								Kill(bytes[5], bytes[6], bytes[3], bytes[4]);
							}
							else {	//wounded
								EnemyStat[LastHitI][LastHitJ] = 1;
								SyncStat(LastHitI, LastHitJ, true);
							}
						}
						else {	//miss
							EnemyStat[LastHitI][LastHitJ] = 2;
							SyncStat(LastHitI, LastHitJ, true);

							prgstat = ProgStat::EnemyTurn;
							this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
						}
					}
					break;
					case 99:
					{
						prgstat = ProgStat::Win;
						this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
					}
					break;
					}
			} while (1);
		}
		catch (SocketException^ e) {

		}
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

void seabattle::mainform::PrepareArea(array<array<Button^>^>^ %area, array<array<int>^>^ %stat, bool isEnemyArea)
{
	area = gcnew array<array<Button^>^>(_strings);
	stat = gcnew array<array<int>^>(_strings);
	for (int i = 0; i < _strings; i++) {
		area[i] = gcnew array<Button^>(_collomns);
		stat[i] = gcnew array<int>(_collomns);
		for (int j = 0; j < _collomns; j++) {
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
				area[i][j]->Location = System::Drawing::Point(200 + 22 * _collomns + 50 + i * 22, 40 + j * 22);
			else {
				area[i][j]->Location = System::Drawing::Point(200 + i * 22, 40 + j * 22);
				area[i][j]->AllowDrop = true;
				area[i][j]->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &mainform::button1_DragOver);
				area[i][j]->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &mainform::button1_DragDrop);
			}
			area[i][j]->Size = System::Drawing::Size(22, 22);
			area[i][j]->TabStop = false;

			if (!isEnemyArea)
				area[i][j]->Click += gcnew System::EventHandler(this, &mainform::ChangeImg);
			else
				area[i][j]->Click += gcnew System::EventHandler(this, &mainform::button1_Click);
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
		turnedShip2->Visible = true;
		pictureBox6->Visible = true;
		pictureBox5->Visible = true;
		SinglesLeft->Visible = true;
		Ships2Left->Visible = true;
		Ships3Left->Visible = true;
		Ships4Left->Visible = true;

		AreaStat[0][1] = 4;
		AreaStat[0][2] = 3;
		AreaStat[0][3] = 2;
		AreaStat[0][4] = 1;
		//ready flags
		EnemyReadyFlag->Visible = true;
		MyReadyFlag->Visible = true;
	}
	break;
	case ProgStat::ServerGameStart:
	{
		//prepare enemy area
		PrepareArea(EnemyArea, EnemyStat, true);
		//hide interface
		Ship1Img->Visible = false;
		Ship2Img->Visible = false;
		Ship3Img->Visible = false;
		Ship4Img->Visible = false;
		turnedShip2->Visible = false;
		pictureBox6->Visible = false;
		pictureBox5->Visible = false;
		SinglesLeft->Visible = false;
		Ships2Left->Visible = false;
		Ships3Left->Visible = false;
		Ships4Left->Visible = false;
		for (int i = 1; i < _strings; i++)
			for (int j = 1; j < _collomns; j++)
				GameArea[i][j]->Click -= gcnew System::EventHandler(this, &mainform::ChangeImg);
		//start game

		EnemyReadyFlag->Visible = false;
		MyReadyFlag->Visible = false;
		StartGameBtn->Visible = false;
		StartGameBtn->Enabled = false;

		array<Byte>^ msg = gcnew array<Byte>(5);
		msg[0] = 15;
		msg[1] = 1;
		msg[2] = 1;
		msg[3] = 1;
		msg[4] = 1;
		Stream->Write(msg, 0, msg->Length);

		prgstat = ProgStat::MyTurn;
		UpdateWnd();
	}
	break;
	case ProgStat::MyTurn:
	{
		//Enable enemy area
		for (int i = 1; i < _strings; i++)
			for (int j = 1; j < _collomns; j++)
				EnemyArea[i][j]->Enabled = true;
		TurnLbl->Visible = true;
		TurnLbl->Text = "You turn";
	}
	break;
	case ProgStat::EnemyTurn:
	{
		//Disable enemy area
		for (int i = 1; i < _strings; i++)
			for (int j = 1; j < _collomns; j++)
				EnemyArea[i][j]->Enabled = false;
		TurnLbl->Visible = true;
		TurnLbl->Text = "Enemy turn";
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

		AreaStat[0][1] = 4;
		AreaStat[0][2] = 3;
		AreaStat[0][3] = 2;
		AreaStat[0][4] = 1;

		Ship1Img->Visible = true;
		Ship2Img->Visible = true;
		Ship3Img->Visible = true;
		Ship4Img->Visible = true;
		turnedShip2->Visible = true;
		pictureBox6->Visible = true;
		pictureBox5->Visible = true;
		SinglesLeft->Visible = true;
		Ships2Left->Visible = true;
		Ships3Left->Visible = true;
		Ships4Left->Visible = true;

		SetStatusLbl("Connected");
		EnemyReadyFlag->Visible = true;
		MyReadyFlag->Visible = true;
	}
	break;
	case ProgStat::ClientGameStart:
	{
		//prepare enemy area
		PrepareArea(EnemyArea, EnemyStat, true);
		//hide interface
		Ship1Img->Visible = false;
		Ship2Img->Visible = false;
		Ship3Img->Visible = false;
		Ship4Img->Visible = false;
		turnedShip2->Visible = false;
		pictureBox6->Visible = false;
		pictureBox5->Visible = false;
		SinglesLeft->Visible = false;
		Ships2Left->Visible = false;
		Ships3Left->Visible = false;
		Ships4Left->Visible = false;
		for (int i = 1; i < _strings; i++)
			for (int j = 1; j < _collomns; j++)
				GameArea[i][j]->Click -= gcnew System::EventHandler(this, &mainform::ChangeImg);

		EnemyReadyFlag->Visible = false;
		MyReadyFlag->Visible = false;

		prgstat = ProgStat::EnemyTurn;
		UpdateWnd();
	}
	break;
	case ProgStat::Lose:
	{
		for (int i = 1; i < _strings; i++)
			for (int j = 1; j < _collomns; j++)
				EnemyArea[i][j]->Enabled = false;
		
		TurnLbl->Text = "You lose";

		array<Byte>^ msg = gcnew array<Byte>(5);
		msg[0] = 99;
		msg[1] = 1;
		msg[2] = 1;
		msg[3] = 1;
		msg[4] = 1;
		Stream->Write(msg, 0, msg->Length);
	}
	break;
	case ProgStat::Win:
	{
		//TODO: возможно, обмен€тьс€ расположени€ми кораблей?

		for (int i = 1; i < _strings; i++)
			for (int j = 1; j < _collomns; j++)
				EnemyArea[i][j]->Enabled = false;

		TurnLbl->Text = "You win!";
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
	//TCPclient = gcnew TcpClient(HostNameEdit->Text, port);
	int count = 0;
	while (count < 10) {
		count++;
		try {
			TCPclient = gcnew TcpClient(HostNameEdit->Text, port);
			break;
		}
		catch (...)
		{

		}
	}
	if (TCPclient == nullptr) {
		this->Invoke(gcnew Action<String^>(this, &mainform::SetStatusLbl), "Connection error");
	}

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
				case 15:	//game started
				{
					//start game
					prgstat = ProgStat::ClientGameStart;
					this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
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
				case 50:
				{
					int I = bytes[1], J = bytes[2];

					array<Byte>^ msg = gcnew array<Byte>(7);
					msg[0] = 51;

					int ship = 0;
					bool turned = false;
					int sI = 0, sJ = 0;

					if (AreaStat[I][J] > 12) {
						msg[1] = 1;	//hit
						msg[2] = TryKill(I, J, &ship, &turned, &sI, &sJ);
					}
					else {
						msg[1] = 0;	//miss
						msg[2] = 0;
						SetMiss(I, J);

						prgstat = ProgStat::MyTurn;
						this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
					}
					msg[3] = ship;
					msg[4] = turned;
					msg[5] = sI;
					msg[6] = sJ;

					Stream->Write(msg, 0, msg->Length);

					if (AreaStat[0][1] == 4 && AreaStat[0][2] == 3 && AreaStat[0][3] == 2 && AreaStat[0][4] == 1) {
						prgstat = ProgStat::Lose;
						this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
					}
				}
				break;
				case 51:
				{
					if (bytes[1]) {	//hit
						if (bytes[2]) {	//destroyed
							Kill(bytes[5], bytes[6], bytes[3], bytes[4]);
						}
						else {	//wounded
							EnemyStat[LastHitI][LastHitJ] = 1;
							SyncStat(LastHitI, LastHitJ, true);
						}
					}
					else {	//miss
						EnemyStat[LastHitI][LastHitJ] = 2;
						SyncStat(LastHitI, LastHitJ, true);

						prgstat = ProgStat::EnemyTurn;
						this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
					}
				}
				break;
				case 99:
				{
					prgstat = ProgStat::Win;
					this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
				}
				break;
				}
		} while (1);
	}
	catch (SocketException^ e) {

	}
	try {
		TCPclient->Close();
	}
	catch (...) {}
}

bool seabattle::mainform::CheckIfPlaceble(int I, int J, int ship, bool turned)
{
	int I_right;
	int J_down;
	if (!turned) {
		I_right = I + ship + 1;
		J_down = J + 2;
	}
	else
	{
		I_right = I + 2;
		J_down = J + ship + 1;
	}
	for (int i = I - 1; i < I_right; i++) {
		if (!i || i == _collomns) continue;
		if (i > _collomns) return false;
		for (int j = J - 1; j < J_down; j++) {
			if (!j || j == _strings) continue;
			if (j > _strings) return false;
			if (AreaStat[i][j])
				return false;
		}
	}

	return true;
}

void seabattle::mainform::PlaceShip(int I, int J, int ship, bool turned)
{
	switch (ship) {
	case 1:
		AreaStat[I][J] = (int)CellIndex::Single;
		GameArea[I][J]->Image = ImageProvider::Single;
		break;
	case 2:
		if (!turned) {
			AreaStat[I][J] = (int)CellIndex::LeftPart;
			GameArea[I][J]->Image = ImageProvider::LeftPart;
			AreaStat[I + 1][J] = (int)CellIndex::RightPart;
			GameArea[I + 1][J]->Image = ImageProvider::RightPart;
		}
		else {
			AreaStat[I][J] = (int)CellIndex::UpPart;
			GameArea[I][J]->Image = ImageProvider::UpPart;
			AreaStat[I][J + 1] = (int)CellIndex::DownPart;
			GameArea[I][J + 1]->Image = ImageProvider::DownPart;
		}
		break;
	case 3:
		if (!turned) {
			AreaStat[I][J] = (int)CellIndex::LeftPart;
			GameArea[I][J]->Image = ImageProvider::LeftPart;
			AreaStat[I + 1][J] = (int)CellIndex::LeftRightPart;
			GameArea[I + 1][J]->Image = ImageProvider::LeftRightPart;
			AreaStat[I + 2][J] = (int)CellIndex::RightPart;
			GameArea[I + 2][J]->Image = ImageProvider::RightPart;
		}
		else {
			AreaStat[I][J] = (int)CellIndex::UpPart;
			GameArea[I][J]->Image = ImageProvider::UpPart;
			AreaStat[I][J + 1] = (int)CellIndex::UpDownPart;
			GameArea[I][J + 1]->Image = ImageProvider::UpDownPart;
			AreaStat[I][J + 2] = (int)CellIndex::DownPart;
			GameArea[I][J + 2]->Image = ImageProvider::DownPart;
		}
		break;
	case 4:
		if (!turned) {
			AreaStat[I][J] = (int)CellIndex::LeftPart;
			GameArea[I][J]->Image = ImageProvider::LeftPart;
			AreaStat[I + 1][J] = (int)CellIndex::LeftRightPart;
			GameArea[I + 1][J]->Image = ImageProvider::LeftRightPart;
			AreaStat[I + 2][J] = (int)CellIndex::LeftRightPart;
			GameArea[I + 2][J]->Image = ImageProvider::LeftRightPart;
			AreaStat[I + 3][J] = (int)CellIndex::RightPart;
			GameArea[I + 3][J]->Image = ImageProvider::RightPart;
		}
		else {
			AreaStat[I][J] = (int)CellIndex::UpPart;
			GameArea[I][J]->Image = ImageProvider::UpPart;
			AreaStat[I][J + 1] = (int)CellIndex::UpDownPart;
			GameArea[I][J + 1]->Image = ImageProvider::UpDownPart;
			AreaStat[I][J + 2] = (int)CellIndex::UpDownPart;
			GameArea[I][J + 2]->Image = ImageProvider::UpDownPart;
			AreaStat[I][J + 3] = (int)CellIndex::DownPart;
			GameArea[I][J + 3]->Image = ImageProvider::DownPart;
		}
		break;
	}
}

void seabattle::mainform::DeleteShip(int I, int J)
{

	if (!AreaStat[I][J]) return;

	int temp_i = I;
	int temp_j = J;
	int ship = 0;
	//go left
	do {
		AreaStat[temp_i][temp_j] = 0;
		GameArea[temp_i][temp_j]->Image = ImageProvider::Empty;
		temp_i--;
		ship++;
	} while (temp_i&&AreaStat[temp_i][temp_j]);

	//go right
	temp_i = I;
	do {
		AreaStat[temp_i][temp_j] = 0;
		GameArea[temp_i][temp_j]->Image = ImageProvider::Empty;
		temp_i++;
		ship++;
	} while (temp_i < _collomns&&AreaStat[temp_i][temp_j]);

	//go up
	temp_i = I;
	do {
		AreaStat[temp_i][temp_j] = 0;
		GameArea[temp_i][temp_j]->Image = ImageProvider::Empty;
		temp_j--;
		ship++;
	} while (temp_j&&AreaStat[temp_i][temp_j]);

	//go down
	temp_j = J;
	do {
		AreaStat[temp_i][temp_j] = 0;
		GameArea[temp_i][temp_j]->Image = ImageProvider::Empty;
		temp_j++;
		ship++;
	} while (temp_j < _strings&&AreaStat[temp_i][temp_j]);
	ship -= 3;
	AreaStat[0][ship]++;
}

void seabattle::mainform::Shoot(int I, int J)
{
	LastHitI = I;
	LastHitJ = J;
	array<Byte>^ msg = gcnew array<Byte>(4);
	msg[0] = 50;
	msg[1] = I;
	msg[2] = J;
	msg[3] = 1;
	Stream->Write(msg, 0, msg->Length);
}

int seabattle::mainform::TryKill(int I, int J, int* ship, bool* turned, int* sI, int* sJ)
{
	AreaStat[I][J] -= 10;	//set to destroyed
	SyncStat(I, J, false);

	int temp_i = I;
	int temp_j = J;
	int alive_parts = 0;
	int destroyed_parts = 0;
	int stepsL = 0, stepsR = 0, stepsUp = 0, stepsDown = 0;

	//go left
	do {
		temp_i--;
		if (!temp_i) break;
		if (AreaStat[temp_i][temp_j] > 12) {
			alive_parts++;
			stepsL++;
		}
		else if (AreaStat[temp_i][temp_j] > 2 && AreaStat[temp_i][temp_j] < 10) {
			destroyed_parts++;
			stepsL++;
		}
	} while (AreaStat[temp_i][temp_j] != 0 && AreaStat[temp_i][temp_j] != 2);

	//go right
	temp_i = I;
	do {
		temp_i++;
		if (!(temp_i < _collomns)) break;
		if (AreaStat[temp_i][temp_j] > 12) {
			alive_parts++;
			stepsR++;
		}
		else if (AreaStat[temp_i][temp_j] > 2 && AreaStat[temp_i][temp_j] < 10) {
			destroyed_parts++;
			stepsR++;
		}
	} while (AreaStat[temp_i][temp_j] != 0 && AreaStat[temp_i][temp_j] != 2);

	//go up
	temp_i = I;
	do {
		temp_j--;
		if (!temp_j) break;
		if (AreaStat[temp_i][temp_j] > 12) {
			alive_parts++;
			stepsUp++;
		}
		else if (AreaStat[temp_i][temp_j] > 2 && AreaStat[temp_i][temp_j] < 10) {
			destroyed_parts++;
			stepsUp++;
		}
	} while (AreaStat[temp_i][temp_j] != 0 && AreaStat[temp_i][temp_j] != 2);

	//go down
	temp_j = J;
	do {
		temp_j++;
		if (!(temp_j < _strings)) break;
		if (AreaStat[temp_i][temp_j] > 12) {
			alive_parts++;
			stepsDown++;
		}
		else if (AreaStat[temp_i][temp_j] > 2 && AreaStat[temp_i][temp_j] < 10) {
			destroyed_parts++;
			stepsDown++;
		}
	} while (AreaStat[temp_i][temp_j] != 0 && AreaStat[temp_i][temp_j] != 2);

	if (alive_parts)
		return 0;
	else {
		*sI = I - stepsL;
		*sJ = J - stepsUp;
		if (stepsDown + stepsUp)
			*turned = true;
		else
			*turned = false;
		*ship = destroyed_parts + 1;
		SetMiss(*sI, *sJ, *ship, *turned);
		AreaStat[0][*ship]++;
		return 1;
	}
}

void seabattle::mainform::SyncStat(int I, int J, bool isEnemy)
{
	array<array<Button^>^>^ imgArea;
	array<array<int>^>^ statArea;

	if (isEnemy) {
		imgArea = EnemyArea;
		statArea = EnemyStat;
	}
	else {
		imgArea = GameArea;
		statArea = AreaStat;
	}

	switch ((CellIndex)statArea[I][J])
	{
	case CellIndex::Down_Destroyed:
		imgArea[I][J]->Image = ImageProvider::Down_Destroyed;
		break;
	case CellIndex::LeftRight_Destroyed:
		imgArea[I][J]->Image = ImageProvider::LeftRight_Destroyed;
		break;
	case CellIndex::Left_Destroyed:
		imgArea[I][J]->Image = ImageProvider::Left_Destroyed;
		break;
	case CellIndex::Right_Destroyed:
		imgArea[I][J]->Image = ImageProvider::Right_Destroyed;
		break;
	case CellIndex::Single_Destroyed:
		imgArea[I][J]->Image = ImageProvider::Single_Destroyed;
		break;
	case CellIndex::UpDown_Destroyed:
		imgArea[I][J]->Image = ImageProvider::UpDown_Destroyed;
		break;
	case CellIndex::Up_Destroyed:
		imgArea[I][J]->Image = ImageProvider::Up_Destroyed;
		break;
	case CellIndex::Empty:
		imgArea[I][J]->Image = ImageProvider::Empty;
		break;
	case CellIndex::Miss:
		imgArea[I][J]->Image = ImageProvider::Miss;
		break;
	case CellIndex::Damaged_Unknown:
		imgArea[I][J]->Image = ImageProvider::Damaged_Unknown;
		break;
	default:
		break;
	}
}

void seabattle::mainform::Kill(int I, int J, int ship, bool turned)
{
	int I_right;
	int J_down;
	int replaced_parts = 0;
	if (!turned) {
		I_right = I + ship + 1;
		J_down = J + 2;
	}
	else
	{
		I_right = I + 2;
		J_down = J + ship + 1;
	}
	EnemyStat[LastHitI][LastHitJ] = (int)CellIndex::Damaged_Unknown;
	for (int i = I - 1; i < I_right; i++) {
		if (!i || i >= _collomns) continue;
		for (int j = J - 1; j < J_down; j++) {
			if (!j || j >= _strings) continue;
			if (EnemyStat[i][j] == (int)CellIndex::Empty) {
				EnemyStat[i][j] = (int)CellIndex::Miss;
			}
			else if (EnemyStat[i][j] == (int)CellIndex::Damaged_Unknown) {
				switch (ship)
				{
				case 1:
				{
					EnemyStat[i][j] = (int)CellIndex::Single_Destroyed;
					replaced_parts++;
				}
				break;
				case 2:
				{
					if (!turned) {
						if (!replaced_parts)
							EnemyStat[i][j] = (int)CellIndex::Left_Destroyed;
						else
							EnemyStat[i][j] = (int)CellIndex::Right_Destroyed;
						replaced_parts++;
					}
					else {
						if (!replaced_parts)
							EnemyStat[i][j] = (int)CellIndex::Up_Destroyed;
						else
							EnemyStat[i][j] = (int)CellIndex::Down_Destroyed;
						replaced_parts++;
					}
				}
				break;
				case 3:
				{
					if (!turned) {
						if (!replaced_parts)
							EnemyStat[i][j] = (int)CellIndex::Left_Destroyed;
						else if (replaced_parts == 1)
							EnemyStat[i][j] = (int)CellIndex::LeftRight_Destroyed;
						else
							EnemyStat[i][j] = (int)CellIndex::Right_Destroyed;
						replaced_parts++;
					}
					else {
						if (!replaced_parts)
							EnemyStat[i][j] = (int)CellIndex::Up_Destroyed;
						else if (replaced_parts == 1)
							EnemyStat[i][j] = (int)CellIndex::UpDown_Destroyed;
						else
							EnemyStat[i][j] = (int)CellIndex::Down_Destroyed;
						replaced_parts++;
					}
				}
				break;
				case 4:
				{
					if (!turned) {
						if (!replaced_parts)
							EnemyStat[i][j] = (int)CellIndex::Left_Destroyed;
						else if (replaced_parts == 1 || replaced_parts == 2)
							EnemyStat[i][j] = (int)CellIndex::LeftRight_Destroyed;
						else
							EnemyStat[i][j] = (int)CellIndex::Right_Destroyed;
						replaced_parts++;
					}
					else {
						if (!replaced_parts)
							EnemyStat[i][j] = (int)CellIndex::Up_Destroyed;
						else if (replaced_parts == 1 || replaced_parts == 2)
							EnemyStat[i][j] = (int)CellIndex::UpDown_Destroyed;
						else
							EnemyStat[i][j] = (int)CellIndex::Down_Destroyed;
						replaced_parts++;
					}
				}
				break;
				default:
					break;
				}
			}
			SyncStat(i, j, true);
		}
	}
}

void seabattle::mainform::SetMiss(int I, int J)
{
	AreaStat[I][J] = (int)CellIndex::Miss;
	SyncStat(I, J, false);
}

void seabattle::mainform::SetMiss(int I, int J, int ship, bool turned)
{
	int I_right;
	int J_down;
	int replaced_parts = 0;
	if (!turned) {
		I_right = I + ship + 1;
		J_down = J + 2;
	}
	else
	{
		I_right = I + 2;
		J_down = J + ship + 1;
	}
	for (int i = I - 1; i < I_right; i++) {
		if (!i || i >= _collomns) continue;
		for (int j = J - 1; j < J_down; j++) {
			if (!j || j >= _strings) continue;
			if (AreaStat[i][j] == 0) {
				AreaStat[i][j] = (int)CellIndex::Miss;
				SyncStat(i, j, false);
			}
		}
	}
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
	if (AreaStat[0][1] || AreaStat[0][2] || AreaStat[0][3] || AreaStat[0][4]) {
		MyReadyFlag->Checked = false;
		return;
	}
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
	int I = 0, J = 0;
	Button^ btn = dynamic_cast<Button^>(sender);
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (GameArea[i][j] == btn) {
				I = i;
				J = j;
			}
		}
	}
	DeleteShip(I, J);
	SinglesLeft->Text = AreaStat[0][1].ToString();
	Ships2Left->Text = AreaStat[0][2].ToString();
	Ships3Left->Text = AreaStat[0][3].ToString();
	Ships4Left->Text = AreaStat[0][4].ToString();
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
			DragDropEffects dropEffect = dynamic_cast<PictureBox^>(sender)->DoDragDrop(this->tempImg, static_cast<DragDropEffects>(DragDropEffects::Copy));
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
				int I = 0, J = 0;
				Button^ btn = dynamic_cast<Button^>(sender);
				for (int i = 0; i < 11; i++) {
					for (int j = 0; j < 11; j++) {
						if (GameArea[i][j] == btn) {
							I = i;
							J = j;
						}
					}
				}
				bool turned = false;
				if (item->Image->Height != 16)
					turned = true;
				int index = (item->Image->Width / 16 + item->Image->Height / 16 - 1);

				if (AreaStat[0][index] > 0) {
					if (CheckIfPlaceble(I, J, index, turned)) {
						AreaStat[0][index]--;
						PlaceShip(I, J, index, turned);
						SinglesLeft->Text = AreaStat[0][1].ToString();
						Ships2Left->Text = AreaStat[0][2].ToString();
						Ships3Left->Text = AreaStat[0][3].ToString();
						Ships4Left->Text = AreaStat[0][4].ToString();
					}
				}
				/*Test--------------------------------------------------
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
				//------------------------------------------------------*/
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

System::Void seabattle::mainform::button1_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	int I = 0, J = 0;
	Button^ btn = dynamic_cast<Button^>(sender);
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (EnemyArea[i][j] == btn) {
				I = i;
				J = j;
			}
		}
	}
	if (!EnemyStat[I][J])
		Shoot(I, J);
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

	UpDownPart = (safe_cast<System::Drawing::Image^>(Resource->GetObject("udpImg.Image")));
	LeftRightPart = (safe_cast<System::Drawing::Image^>(Resource->GetObject("rlpImg.Image")));
	LeftPart = (safe_cast<System::Drawing::Image^>(Resource->GetObject("lpImg.Image")));
	RightPart = (safe_cast<System::Drawing::Image^>(Resource->GetObject("rpImg.Image")));
	UpPart = (safe_cast<System::Drawing::Image^>(Resource->GetObject("upImg.Image")));
	DownPart = (safe_cast<System::Drawing::Image^>(Resource->GetObject("dpImg.Image")));

	UpDown_Destroyed = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox8.Image")));
	LeftRight_Destroyed = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox7.Image")));
	Left_Destroyed = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox9.Image")));
	Right_Destroyed = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox10.Image")));
	Up_Destroyed = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox12.Image")));
	Down_Destroyed = (safe_cast<System::Drawing::Image^>(Resource->GetObject("pictureBox11.Image")));
}
