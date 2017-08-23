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
					case 50:
					{
						int I= bytes[1], J = bytes[2];
						if (AreaStat[I][J] > 3) {
							array<Byte>^ msg = gcnew array<Byte>(2);
							msg[0] = 51;
							msg[1] = 1;
							Stream->Write(msg, 0, msg->Length);

							AreaStat[I][J]

							prgstat=ProgStat::ServerTurn;
							this->Invoke(gcnew Action(this, &mainform::UpdateWnd));
						}
					}
						break;
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
				area[i][j]->Location = System::Drawing::Point(500 + i * 22, 40 + j * 22);
			else {
				area[i][j]->Location = System::Drawing::Point(200 + i * 22, 40 + j * 22);
				area[i][j]->AllowDrop = true;
				area[i][j]->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &mainform::button1_DragOver);
				area[i][j]->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &mainform::button1_DragDrop);
			}
			area[i][j]->Size = System::Drawing::Size(22, 22);
			area[i][j]->TabStop = false;

			if (!isEnemyArea)	area[i][j]->Click += gcnew System::EventHandler(this, &mainform::ChangeImg);
			else area[i][j]->Click += gcnew System::EventHandler(this, &mainform::button1_Click);
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
		//PointEstLbl->Visible = true;
		//PointEstLbl->Text = "20";

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

		AreaStat[0][1] = 4;
		AreaStat[0][2] = 3;
		AreaStat[0][3] = 2;
		AreaStat[0][4] = 1;

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
		//TODO: обработать ошибк
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
	array<Byte>^ msg = gcnew array<Byte>(3);
	msg[0] = 50;
	msg[1] = I;
	msg[2] = J;
	Stream->Write(msg, 0, msg->Length);
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
			if (GameArea[i][j] == btn) {
				I = i;
				J = j;
			}
		}
	}
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
}
