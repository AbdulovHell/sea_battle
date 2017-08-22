#include "mainform.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(cli::array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	seabattle::mainform^ mainwnd = gcnew seabattle::mainform();
	Application::Run(mainwnd);
}