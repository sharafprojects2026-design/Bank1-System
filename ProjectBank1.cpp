#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;
const string ClientsFileName = "Client.txt";

void ShowMainMenu();
struct sClient
{
    string AccountNumber = "";
    string PinCode;
    string Name = "";
    string Phone = "";
    double AccountBalance;
    bool MarkforDelete = false;
};
enum enMenue { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Exit = 6 };
string ReadAccountNumber()
{
    string AccountNumber = "";
    cout << "Enter Account Number? ";
    cin >> AccountNumber;
    return AccountNumber;
}
vector <string> SplitString(string S1, string Delime)
{
    vector <string> vString;
    string Word;
    int Pos = 0;
    while ((Pos = S1.find(Delime)) != std::string::npos)
    {
        Word = S1.substr(0, Pos);
        if (Word != "")
        {
            vString.push_back(Word);

        }
        S1.erase(0, Pos + Delime.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}
string ConvertFileRecordToLine(sClient Client, string Seperator = "#//#")
{
    string StringText = "";
    StringText += Client.AccountNumber + Seperator;
    StringText += Client.PinCode + Seperator;
    StringText += Client.Name + Seperator;
    StringText += Client.Phone + Seperator;
    StringText += to_string(Client.AccountBalance);

    return StringText;
}
sClient ConvertFileLineToRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector <string> vClient;
    vClient = SplitString(Line, Seperator);

    Client.AccountNumber = vClient[0];
    Client.PinCode = vClient[1];
    Client.Name = vClient[2];
    Client.Phone = vClient[3];
    Client.AccountBalance = stod(vClient[4]);

    return Client;

}

vector <sClient>LoadClientDataFromFile(string FileName)
{
    vector <sClient>vClient;
    fstream MyFile;
    string line;
    sClient Client;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        while (getline(MyFile, line))
        {
            Client = ConvertFileLineToRecord(line);
            vClient.push_back(Client);

        }
        MyFile.close();

    }
    return vClient;
}
vector <sClient> SaveClientToFile(string FileName, vector<sClient>vClient)
{
    sClient Client;
    fstream MyFile;
    string Dataline;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (sClient& C : vClient)
        {
            if (C.MarkforDelete == false)
            {
                Dataline = ConvertFileRecordToLine(C);
                MyFile << Dataline << endl;
            }
        }
        MyFile.close();

    }
    return vClient;
}
bool IsClientExists(string AccountNumber, vector<sClient>& vClient)
{

    for (sClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            return true;
        }
    }

    return false;
}


void ShowClient(sClient Client)
{
    cout << "|" << setw(15) << left << Client.AccountNumber;
    cout << "|" << setw(15) << left << Client.PinCode;
    cout << "|" << setw(30) << left << Client.Name;
    cout << "|" << setw(20) << left << Client.Phone;
    cout << "|" << setw(10) << left << Client.AccountBalance;

    cout << endl;
}

void ShowAllClientsScreen()
{
    system("cls");
    vector <sClient> vClient = LoadClientDataFromFile(ClientsFileName);

    cout << "\n\t                           Client List(" << vClient.size() << ") Client (s).\n\n";
    cout << "_____________________________________________________________________________________________________________________\n\n";
    cout << "|" << setw(15) << left << "Account Number ";
    cout << "|" << setw(13) << left << "Pin Code ";
    cout << "|" << setw(30) << left << "Client Name ";
    cout << "|" << setw(20) << left << "Phone ";
    cout << "|" << setw(9) << left << "Account Balance ";

    cout << "\n_____________________________________________________________________________________________________________________\n\n";

    if (vClient.size() == 0)

        cout << "\t\t\t\tNo Client  Available in The System .";
    else
        for (sClient C : vClient)
        {
            ShowClient(C);
        }
    cout << "_______________________________________________________________________________________________________________________\n\n\n";

 

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient>& vClient, sClient& Client)
{

    for (sClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;

        }
    }
    return false;
}
bool MarkDeletByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
    for (sClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkforDelete = true;
            return true;
        }
    }
    return false;
}

sClient ChangClinet(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Pin Code? ";
    getline(cin >> ws, Client.PinCode);
    cout << "enter Name ? ";
    getline(cin, Client.Name);
    cout << "Please enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Please enter Account Balance? ";
    cin >> Client.AccountBalance;
    return Client;

}

void PrintClient(sClient Client)
{
    cout << "\n\nThe Following are The Client datailse:\n";
    cout << "----------------------------------------------------\n ";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PinCose        : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "\n---------------------------------------------------------- \n\n";

}


bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
   
    sClient Client;
    char Answer = 'n';
  
    
    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {

        PrintClient(Client);
        cout << "Are You Sure You Want Delete This Client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkDeletByAccountNumber(AccountNumber, vClient);

            SaveClientToFile(ClientsFileName, vClient);

            vClient = LoadClientDataFromFile(ClientsFileName);

            cout << "\n\n Client Delete Successfully.\n\n";
            
            return true;
          


        }


    }
    else
    {
        cout << "\n\nClient With Account Number (" << AccountNumber << ") is Not Found!\n\n";
       
        return false;
       
    }
}
void ShowDeleteClientsScreen()
{
    cout << "\n------------------------------------------------------\n";
    cout << "\t       Delete Client Screen \n";
    cout << "---------------------------------------------------------\n";
    vector<sClient> vClient = LoadClientDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClient);

}

bool  UpdatClientByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
    
   
    sClient Client;
    char Answer = 'n';
   
    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {

        PrintClient(Client);
        cout << "Are You Sure You Want Update This Client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClient)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangClinet(AccountNumber);

                    break;
                }
            }
            SaveClientToFile(ClientsFileName, vClient);

            cout << "\n\n Client Update Successfully.\n\n";
            return true;


        }


    }
    else
    {
        cout << "\n\nClient With Account Number (" << AccountNumber << ") is Not Found!\n\n";
         return false;

    }
}
void ShowUpdateClientScreen()
{
    cout << "\n------------------------------------------------ \n";
    cout << "             Update Client Info Screen              ";
    cout << "\n--------------------------------------------------\n\n";
    vector<sClient> vClient = LoadClientDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    UpdatClientByAccountNumber(AccountNumber, vClient);
}

void ShowFindClientsScreen ()
{
    sClient Client;
    vector <sClient> vClient = LoadClientDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClient(Client);
      
    }
    else
    {
       cout << "\n\nClient With  Account Number (" << AccountNumber << ") is Not Found!\n";
       

    }

}

void ShowEndProgramming()
{
   
    cout << "\n----------------------------------------------------------\n";
    cout << "\t             Programm End:-)                        \n";
    cout << "------------------------------------------------------------\n";

}
void GoBackToMainMenue()
{
    cout << "\n\nPress Any Key To Go Back To Main Menue...";
    system("Pause>0");
    ShowMainMenu();
}


short ReadMainMenue()
{
    int choice = 0;
    cout << "Choose what do you want to do? [1 to 6]? ";
    cin >> choice;
    return choice;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <sClient> vClient;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertFileLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}
sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);
    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient With [" << Client.AccountNumber << "] already exists,Enter anothe Account Number?";
        getline(cin >> ws, Client.AccountNumber);

    }
    cout << "Enter Pin Code? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin >> ws, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;
    return Client;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}
void AddNewClient()
{
   sClient Client;

   Client = ReadNewClient();
   AddDataLineToFile(ClientsFileName, ConvertFileRecordToLine(Client));
    

        
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Add New Client:\n\n";
        AddNewClient();
        cout << "\nClient Add Successfully,do You Want To Add More Clients? Y/N? ";
        cin >> AddMore;
        
    } while (toupper(AddMore) == 'Y');
}
void ShowAddNewClientsScreen()
{
    cout << "\n------------------------------------------------\n";
    cout << "\t Add New Clients Screen";
    cout << "\n------------------------------------------------\n";
    AddNewClients();

}
void PerfromMainMenueOption(enMenue MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMenue::Show:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    case enMenue::Add:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMenue::Delete:
        system("cls");
        ShowDeleteClientsScreen();
        GoBackToMainMenue();
        break;
    case enMenue::Update:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
        
    case enMenue::Find:
        system("cls");
        ShowFindClientsScreen();
        GoBackToMainMenue();
        break;

    case enMenue::Exit:
        system("cls");
        ShowEndProgramming();
        GoBackToMainMenue();
        break;
    }
    

}
void ShowMainMenu()
{

    system("cls");
    cout << "===================================================\n ";
    cout << "                Min Menue Screen                  \n";
    cout << "===================================================\n";
    cout << "    [1] show Client List.\n";
    cout << "    [2] Add New Client  .\n";
    cout << "    [3] Delete Client.\n";
    cout << "    [4] Update Client Info.\n";
    cout << "    [5] Find Client .\n";
    cout << "    [6] Exit.\n";
    cout << "===================================================\n";

    PerfromMainMenueOption((enMenue)ReadMainMenue());
}
int main()
{

    ShowMainMenu();
    system("Pause>0");
    
    return 0;


}

