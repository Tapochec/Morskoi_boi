#include "MyForm.h"
#include "Bot.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThread]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Morskoiboi::MyForm form; //����� ����� ������ - MyForm
	Application::Run(% form);
}

