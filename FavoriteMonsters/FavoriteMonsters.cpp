// FavoriteMonsters.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;


class FavoriteMonster
{
	string MonsterName;
	string MonsterElement;
	int	Speed;
	int Attack;
	int Hp;
public:
	FavoriteMonster();
	FavoriteMonster(string MonsterElement, string MonsterName, int Speed, int Attack, int Hp);
	void CaptureMonster();
	void ShowMonster();
	int SaveMonster(ofstream &outfile);
};


class MonsterList
{
	FavoriteMonster **list;
	int numrecords;
	int listsize;
	int ReallocateArray();
public:
	MonsterList();
	~MonsterList();
	void GetUserInput();
	void ShowMonsterList();
	int SaveMonsterList(string filename);
	int ReadMonsterList(string filename);
};

// default constructor
FavoriteMonster::FavoriteMonster()
{
	MonsterElement = ""; MonsterName = ""; Speed = 0; Attack = 0; Hp = 0;
}
// overload constructor
FavoriteMonster::FavoriteMonster(string ele, string name, int spd, int atk, int hhp)
{
	MonsterElement = ele;
	MonsterName = name;
	Speed = spd;
	Attack = atk;
	Hp = hhp;
}
// Get monster info from user
void FavoriteMonster::CaptureMonster()
{
	cout << "What element is this monster? -->";
	getline(cin, MonsterElement);
	cout << "What is this monster's awakened name?  -->";
	getline(cin, MonsterName);
	cout << "What is the monster's attack speed? -->";
	cin >> Speed;
	cin.ignore();
	cin.clear();
	cout << "What is the monster's max attack? -->";
	cin >> Attack;
	cin.ignore();
	cin.clear();
	cout << "What is the monster's max hit points? -->";
	cin >> Hp;
	cin.ignore();
	cin.clear();
}
// display monster info
void FavoriteMonster::ShowMonster()
{
	cout << "Monster Element: " << MonsterElement << " Awakened Name: " << MonsterName << " Attack Speed: " << Speed << " Attack Power: " << Attack << " Max HP: " << Hp << endl;
}
// save monster info to file path
int FavoriteMonster::SaveMonster(ofstream& outfile)
{
	outfile << MonsterElement << "|" << MonsterName << "|" << Speed << "|" << Attack << "|" << Hp << endl;
	return 0;
}

// monster list constructor   allocate space
MonsterList::MonsterList()
{
	list = new FavoriteMonster*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}
// free allocated memory
MonsterList::~MonsterList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}
// Reallocate memory 
int MonsterList::ReallocateArray()
{
	FavoriteMonster **temp;
	temp = new FavoriteMonster*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;
	return 0;
}

// get user input 
void MonsterList::GetUserInput()
{
	string answer;
	cout << "Enter item? Y or N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		list[numrecords] = new FavoriteMonster();
		list[numrecords]->CaptureMonster();
		numrecords++;
		cout << "Enter item? Y or N?" << endl;
		getline(cin, answer);
	}
}
// Show list
void MonsterList::ShowMonsterList()
{
	string answer;
	cout << "Would you like to display the list? Y/N" << endl;
	cin >> answer;

	if (answer == "Y" || answer == "y")
	{
		for (int i = 0; i < numrecords; i++)
			list[i]->ShowMonster();
	}
}
// save list to filepath
int MonsterList::SaveMonsterList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->SaveMonster(output);
		}
		output.close();
		return 0;
	}
	else
		return WRITEERROR;
}
// read monster list from file given filepath
// error if file not found
int MonsterList::ReadMonsterList(string filename)
{
	string iele, iname, ispd, iatk, ihhp;
	string answer;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "Could not find or could not open file" << endl;
		return READERROR;
	}
	if (numrecords == listsize)
	{
		ReallocateArray();
	}
	while (!infile.eof())
	{
		if (numrecords == listsize)
		{
			ReallocateArray();
		}
		getline(infile, iele, '|');
		if (!iele.empty())
		{
			getline(infile, iname, '|');
			getline(infile, ispd, '|');
			int spd = stoi(ispd);
			getline(infile, iatk, '|');
			int atk = stoi(iatk);
			getline(infile, ihhp);
			int hhp = stoi(ihhp);
			list[numrecords] = new FavoriteMonster(iele, iname, spd, atk, hhp);
			numrecords++;
		}
	}
	infile.close();
	return 0;
}

int main()
{
	string filename = "C:\\Users\\reedj\\Desktop\\Info450\\Applications\\Assignment4\\MonsterList.txt";
	MonsterList myMonsters;
	string answer;
	int error;
	cout << "Default location: " << filename << endl;
	cout << "Change location? Y/N" << endl;
	cin >> answer;
	cin.ignore();
	cin.clear();
	if (answer != "n" && answer != "N")
	{
		cout << "Enter new file location: " << endl;
		getline(cin, filename);
	}
	error = myMonsters.ReadMonsterList(filename);
	if (error)
	{
		cout << "Cannot read list - continue creating new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return 1;
		}
	}
	myMonsters.GetUserInput();
	myMonsters.SaveMonsterList(filename);
	myMonsters.ShowMonsterList();

	return 0;
}
