#include<iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <cctype>
#include <fstream>
using namespace std;

//Global Variables 
const string ClientsFileName = "ClientsData.txt";
const string Separator = "#//#";
struct stClients
{
	string Name;
	string Phone;
	string AccountNumber;
	string PinCode;
	double AccountBalance = 0;
};
stClients CurrentClient;
// End Global Variables 
void Clear()
{
	system("CLS");
}
void Pause()
{
	system("pause");
}
void Lines(int NumberOfLines)
{
	for (size_t i = 0; i < NumberOfLines; i++)
	{
		cout << "\n";
	}
}
void LoginScreen();
void MainScreen();
int ReadNumberInRange(string Message, int From, int To)
{
	int Number = 0;
	do
	{
		cout << Message << endl;
		cin >> Number;
	} while (Number < From || Number > To);
	return Number;
}
vector <string> SaveClientsDateInVector(string FileName)
{
	vector <string> vDate;
	fstream myfile;
	myfile.open(FileName, ios::in);
	if (myfile.is_open())
	{
		string OneLine;
		while (getline(myfile, OneLine))
		{
			vDate.push_back(OneLine);
		}
		myfile.close();
	}
	return vDate;
}
vector <string> SplitString(string S1, string delim)
{
	vector <string> vTokens;
	int pos = 0;
	string sWord; // define a string variable
	// use find() function to get the position of the delimiters
	while ((pos = S1.find(delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vTokens.push_back(sWord);
		}
		S1.erase(0, pos + delim.length());
	}
	if (S1 != "")
	{
		vTokens.push_back(S1);
	}
	return vTokens;
}
string JoinString(vector <string>& vClientDataLine, string Delim)
{
	string FullName = "";
	for (string& Line : vClientDataLine)
	{
		if (Line != "")
		{
			FullName = FullName + Line + Delim;
		}
	}
	return FullName.substr(0, FullName.length() - Delim.length());
}
bool IsClientHasAccount(string AccountNumber, string PinCode)
{
	vector <string> Clients = SaveClientsDateInVector(ClientsFileName);
	for (string& Line : Clients)
	{
		vector <string> Client = SplitString(Line, Separator);
		if (Client[0] == AccountNumber && Client[1] == PinCode)
			return true;
	}
	return false;
}
enum class enMainScreen
{
	QuickWithdraw = 1, NormalWithdraw = 2, Dposit = 3, CheckBalance = 4, Logout = 5
};
// <=========================== Check Balance ==============================>
double CheckBalance()
{
	vector <string> Clients = SaveClientsDateInVector(ClientsFileName);
	for (string& Line : Clients)
	{
		vector <string> Client = SplitString(Line, Separator);
		if (Client[0] == CurrentClient.AccountNumber)
			return stod(Client[4]);
	}
}
void BalanceScreen()
{
	Clear();
	cout << "\n=================================================================\n";
	cout << right << setw(40) << "-( Check Balance Screen )-" << endl;
	cout << "=================================================================\n";
	cout << "\nYour Balance == " << CheckBalance() << "\n\n";
	Pause();
	Clear();
	MainScreen();
}
// <=========================== Quick Withdraw ==============================>
enum class enQuickWithdraw
{
	Twenty = 1,
	Fifty = 2,
	OneHundred = 3,
	TwoHundred = 4,
	FourHundred = 5,
	SixHundred = 6,
	EightHundred = 7,
	OneThousand = 8,
	Exit = 9
};
void SaveNewDateToFileAfterUpdate(vector <string> Clients, string FileName)
{
	fstream myfile;
	myfile.open(FileName, ios::out);
	if (myfile.is_open())
	{
		for (string& Line : Clients)
		{
			myfile << Line << endl;
		}
	}
	myfile.close();
}
void ActivateWithdrawalProcess(int Number)
{
	double TotalBalanceAfterWithDraw = CheckBalance() - Number;
	vector <string> Clients = SaveClientsDateInVector(ClientsFileName);
	for (string& Line : Clients)
	{
		vector <string> Client = SplitString(Line, Separator);
		if (Client[0] == CurrentClient.AccountNumber)
		{
			Client[4] = to_string(TotalBalanceAfterWithDraw);
			Line = JoinString(Client, Separator);
			SaveNewDateToFileAfterUpdate(Clients, ClientsFileName);
			break;
		}
	}
}
void ConfirmWithdrawalProcess(int Number)
{
	char Perform = 'y';
	if (Number <= CheckBalance())
	{
		cout << "\nAre you sure you want perform this transactions ? y/n ? ";
		cin >> Perform;
		if (toupper(Perform) == 'Y')
		{
			ActivateWithdrawalProcess(Number);
			cout << "\nDone Successfully , New balance is : " << CheckBalance() << endl;
			Pause();
			Clear();
			MainScreen();
		}
		else
		{
			Pause();
			Clear();
			MainScreen();
		}
	}
	else
	{
		cout << "\nThis operation is rejected because the amount exceeds your balance, make anthor choice \n";
		Pause();
		Clear();
		MainScreen();
	}
}
void WithDrawOperations(enQuickWithdraw Choice)
{
	switch (Choice)
	{
	case enQuickWithdraw::Twenty:
		ConfirmWithdrawalProcess(20);
		break;
	case enQuickWithdraw::Fifty:
		ConfirmWithdrawalProcess(50);
		break;
	case enQuickWithdraw::OneHundred:
		ConfirmWithdrawalProcess(100);
		break;
	case enQuickWithdraw::TwoHundred:
		ConfirmWithdrawalProcess(200);
		break;
	case enQuickWithdraw::FourHundred:
		ConfirmWithdrawalProcess(400);
		break;
	case enQuickWithdraw::SixHundred:
		ConfirmWithdrawalProcess(600);
		break;
	case enQuickWithdraw::EightHundred:
		ConfirmWithdrawalProcess(800);
		break;
	case enQuickWithdraw::OneThousand:
		ConfirmWithdrawalProcess(1000);
		break;
	case enQuickWithdraw::Exit:
		Clear();
		MainScreen();
		break;
	default:
		break;
	}
}
void WhatClientChooseInQuickWithdrawScreen()
{
	cout << "Your Balance is " << CheckBalance() << endl;
	int Number = 0;
	Number = ReadNumberInRange("Choose what to Withdraw from [ 1 to 9 ] ? ", 1, 9);
	enQuickWithdraw Choice;
	Choice = (enQuickWithdraw)Number;
	WithDrawOperations(Choice);
}
void QuickWithdrawScreen()
{
	Clear();
	cout << "\n=================================================================\n";
	cout << right << setw(40) << "-( Quick Withdraw )-" << endl;
	cout << "=================================================================\n";
	cout << setw(20) << "[1]" << " 20" << setw(16) << "[2]" << " 50" << endl;
	cout << setw(20) << "[3]" << " 100" << setw(15) << "[4]" << " 200" << endl;
	cout << setw(20) << "[5]" << " 400" << setw(15) << "[6]" << " 600" << endl;
	cout << setw(20) << "[7]" << " 800" << setw(15) << "[8]" << " 1000" << endl;
	cout << setw(20) << "[9]" << " Exit" << endl;
	cout << "=================================================================\n";
	WhatClientChooseInQuickWithdrawScreen();
}
// <=========================== Quick Withdraw ==============================>
void WhatClientChooseInNormalWithdrawScreen()
{
	cout << "Your Balance is " << CheckBalance() << endl;
	int Number = 0;
	do
	{
		cout << "\nEnter amount multiple of 5's ? ";
		cin >> Number;

	} while (Number % 5 != 0);
	// we use the same ConfirmWithdrawalProcess function to do what the client will choose
	ConfirmWithdrawalProcess(Number);
}
void NormalWithdrawScreen()
{
	Clear();
	cout << "\n=================================================================\n";
	cout << right << setw(40) << "-( Normal Withdraw )-" << endl;
	cout << "=================================================================\n";
	WhatClientChooseInNormalWithdrawScreen();
}
// <=========================== Dposit ==============================>
void ConfirmDepositeProcess(int Number)
{
	char Perform = 'y';
	cout << "\nAre you sure you want perform this transactions ? y/n ? ";
	cin >> Perform;
	if (toupper(Perform) == 'Y')
	{
		ActivateWithdrawalProcess(-Number);
		cout << "\nDone Successfully , New balance is : " << CheckBalance() << endl;
		Pause();
		Clear();
		MainScreen();
	}
	else
	{
		Pause();
		Clear();
		MainScreen();
	}
}
void WhatClientChooseInNormalDepositeScreen()
{
	cout << "Your Balance is " << CheckBalance() << endl;
	double Number = 0;
	cout << "\nEnter a positive Deposite amount ? ";
	cin >> Number;
	ConfirmDepositeProcess(Number);
}
void DepositScreen()
{
	Clear();
	cout << "\n=================================================================\n";
	cout << right << setw(40) << "-( Dposit Screen )-" << endl;
	cout << "=================================================================\n";
	WhatClientChooseInNormalDepositeScreen();
}
void ResultOfClientChoice(enMainScreen Choice)
{
	switch (Choice)
	{
	case enMainScreen::QuickWithdraw:
		QuickWithdrawScreen();
		break;
	case enMainScreen::NormalWithdraw:
		NormalWithdrawScreen();
		break;
	case enMainScreen::Dposit:
		DepositScreen();
		break;
	case enMainScreen::CheckBalance:
		BalanceScreen();
		break;
	case enMainScreen::Logout:
		Clear();
		LoginScreen();
		break;
	default:
		Clear();
		LoginScreen();
		break;
	}
}
void ClientChoice()
{
	int Number = 0;
	Number = ReadNumberInRange("Choose what do you want to do [ 1 to 5 ] ? ", 1, 5);
	enMainScreen Choice;
	Choice = (enMainScreen)Number;
	ResultOfClientChoice(Choice);
}
void MainScreen()
{
	cout << "\n=================================================================\n";
	cout << right << setw(40) << "-( ATM Main Menue Screen )-" << endl;
	cout << "=================================================================\n";
	cout << setw(20) << right << "[1]" << " Quick Withdraw." << endl;
	cout << setw(20) << "[2]" << " Normal Withdraw." << endl;
	cout << setw(20) << "[3]" << " Dposit." << endl;
	cout << setw(20) << "[4]" << " Check Balance." << endl;
	cout << setw(20) << "[5]" << " Logout." << endl;
	cout << "=================================================================\n";
	ClientChoice();
}
void WelcomeScreen()
{
	vector <string> Clients = SaveClientsDateInVector(ClientsFileName);
	for (string& Line : Clients)
	{
		vector <string> Client = SplitString(Line, Separator);
		if (Client[0] == CurrentClient.AccountNumber)
		{
			CurrentClient.Name = Client[2];
			break;
		}
	}
	Lines(10);
	cout << "\n=================================================================\n\n";
	cout << right << setw(30) << "Welcome you , " << CurrentClient.Name << "\n\n";
	cout << "=================================================================\n";
	Pause();
	Clear();
	MainScreen();
}
void LoginScreen()
{
	bool LoginStatus = true;
	do
	{
		Clear();
		cout << "\n=================================================================\n";
		cout << right << setw(35) << "-( Login Screen )-" << endl;
		cout << "=================================================================\n";
		vector <string> vClientsDate = SaveClientsDateInVector(ClientsFileName);
		if (LoginStatus == false)
			cout << "Invalid Account Number / Pin Code , try again \n";
		cout << "Enter your account number ? ";
		cin >> CurrentClient.AccountNumber;
		cout << "Enter PIN code ? ";
		cin >> CurrentClient.PinCode;
		if (IsClientHasAccount(CurrentClient.AccountNumber, CurrentClient.PinCode))
		{
			Clear();
			WelcomeScreen();
			LoginStatus = true;
		}
		else
			LoginStatus = false;

	} while (LoginStatus == false);
}
int main()
{
	LoginScreen();
}