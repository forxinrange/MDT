Author - Michael Bradley
Version - 1.0

Description - This tool will collect details on Windows machines for display and write them out to a CSV file.

*/

#include "stdafx.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

class csv
{
	public:

		void generateBlank()
		{
			ofstream csvFile("Details.csv");
			csvFile << "Asset Tag,Computer Name,UUID,Serial Number,Mac Address" << endl;
			csvFile.close();
		}
		
		bool filePresent()
		{
			ifstream csvFile("Details.csv");
			if (csvFile)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void createCSV(string assetNo, string cName, string UUID, string SerialNo, vector<string> macAddr)
		{
			ofstream csvFile;
			csvFile.open("Details.csv", ios_base::app);
			csvFile << assetNo << "," << cName << "," << UUID << "," << SerialNo << ",";

			for (int i = 0; i < macAddr.size(); i++)
			{
				csvFile << macAddr[i];
				if (i < macAddr.size() - 1)
				{
					csvFile << ",";
				}
			}

			csvFile << endl;

			csvFile.close();
		}

		void csvProcess(string assetNo, string cName, string UUID, string SerialNo, vector<string> macAddr)
		{
			bool csvExists = filePresent();
			if (csvExists == false)
			{
				generateBlank();
				createCSV(assetNo,cName,UUID,SerialNo,macAddr);
			}
			else
			{
				createCSV(assetNo, cName, UUID, SerialNo, macAddr);
			}
		}
};

class machine // Machine class
{
	public:
		string assetTag;
		string UUID;
		string serialNo;
		string computerName;
		vector<string> macAddr;

		void captureAssetTag(string assetString) // Method to store user input from main to object attribute
		{
			assetTag = assetString;
		}

		void captureUUID() // Method to collect UUID and store it in object attribute.
		{
			char empty = char(0); // Store empty char value in memory.
			char space = char(32); // Store space char value in memory.
			string linePlaceholder;
			system("wmic csproduct get UUID >> u.dat");
			ifstream uuidFile;
			uuidFile.open("u.dat");
			for (int lineNo = 0; getline(uuidFile, linePlaceholder) && lineNo < 2; lineNo++)
			{
				if (lineNo == 1)
				{
					UUID = linePlaceholder; // Store line to machine object attribute
					UUID.erase(remove(UUID.begin(), UUID.end(), empty), UUID.end()); // Clean blank spaces from string
					UUID.erase(remove(UUID.begin(), UUID.end(), '\r'), UUID.end()); // Clean blank spaces from string
					UUID.erase(remove(UUID.begin(), UUID.end(), space), UUID.end()); // Clean blank spaces from string
				}
			}
			uuidFile.close();
			system("del /f u.dat");
		}

		void captureSerialNo() // Method to collect Serial Number and store as object Attribute
		{
			char empty = char(0); // Store empty char value in memory.
			char space = char(32); // Store space char value in memory.
			string linePlaceholder;
			system("wmic bios get serialnumber >> s.dat");
			ifstream serialFile;
			serialFile.open("s.dat");
			for (int lineNo = 0; getline(serialFile, linePlaceholder) && lineNo < 2; lineNo++)
			{
				if (lineNo == 1)
				{
					serialNo = linePlaceholder; // Store line to machine object attribute
					serialNo.erase(remove(serialNo.begin(), serialNo.end(), empty), serialNo.end()); // Clean blank spaces from string
					serialNo.erase(remove(serialNo.begin(), serialNo.end(), '\r'), serialNo.end()); // Clean blank spaces from string
					serialNo.erase(remove(serialNo.begin(), serialNo.end(), space), serialNo.end()); // Clean blank spaces from string
				}
			}
			serialFile.close();
			system("del /f s.dat");
		}

		void captureComputerName() // Capture the Computer Name and server to object
		{
			system("hostname >> cn.dat");
			ifstream hostNFile;
			hostNFile.open("cn.dat");
			hostNFile >> computerName;
			hostNFile.close();
			system("del /f cn.dat");
		}

		void captureMacAddr()
		{
			// declare scrub vector
			vector<string> scrubVect;

			// read loop
			char empty = char(0);
			char space = char(32); // Store space char value in memory.
			string linePlaceholder;
			system("wmic nic get MACaddress >> m.dat");
			ifstream macFile;
			macFile.open("m.dat");
			for (int lineNo = 0; getline(macFile, linePlaceholder) && lineNo < 50; lineNo++)
			{
				string stringCleaner = linePlaceholder;
				stringCleaner.erase(remove(stringCleaner.begin(), stringCleaner.end(), empty), stringCleaner.end());
				scrubVect.push_back(stringCleaner);
			}
			macFile.close();
			system("del /f m.dat");

			for (int i = 0; i < scrubVect.size(); i++) // Stores correct values in to new vector
			{
				string scrubTest = scrubVect[i];
				if (scrubTest[0] != char(32) && scrubTest[0] != char(-1))
				{
					scrubTest.erase(remove(scrubTest.begin(), scrubTest.end(), '\r'), scrubTest.end()); // Clean blank spaces from string
					scrubTest.erase(remove(scrubTest.begin(), scrubTest.end(), space), scrubTest.end()); // Clean blank spaces from string
					macAddr.push_back(scrubTest);
				}
			}


		}
};

class program // Main program class
{
	public:

		void displayHeader()
		{
			cout << "----------Machine Details Tool - UCLan v1.0----------";
			cout << endl;
			cout << endl;
		}

		void PrintDetails(machine &machineObj)
		{
			cout << " Computer Name: " << machineObj.computerName;
			cout << endl;
			cout << "          UUID: " << machineObj.UUID;
			cout << endl;
			cout << " Serial Number: " << machineObj.serialNo;
			cout << endl;
			cout << "Mac Address(s): ";

			for (int i = 0; i < machineObj.macAddr.size(); i++)
			{
				if (i != 0)
				{
					cout << "                " << machineObj.macAddr[i];
				}
				else
				{
					cout << machineObj.macAddr[i];
				}
				cout << endl;
			}
		}

		string getUserInput(string message)
		{
			string inputString;
			cout << message;
			cin >> inputString;
			return inputString;
		}
};

int main()
{
	// Initialise
	system("CLS"); // Clear the screen

	// Declare Objects
	machine activeMachine; // Machine Object
	program root; // Main method collection object
	csv outputCsv; // Csv object with writing methods

	// Display Header
	root.displayHeader();

	// Call methods to retrieve machine data from OS
	activeMachine.captureComputerName();
	activeMachine.captureUUID();
	activeMachine.captureSerialNo();
	activeMachine.captureMacAddr();

	// Print details for validation
	root.PrintDetails(activeMachine);

	// Get Asset number from user and store in object attribute
	string Atag = root.getUserInput("Please enter the asset tag of this machine: ");
	activeMachine.captureAssetTag(Atag);


	// Pass machine attributes to csv generator and write/append csv file
	cout << "Write details to CSV? y/n: ";
	char yesNo;
	cin >> yesNo;
	if (yesNo == 'y' || yesNo == 'Y')
	{
		outputCsv.csvProcess(activeMachine.assetTag, activeMachine.computerName, activeMachine.UUID, activeMachine.serialNo, activeMachine.macAddr);
	}
	else
	{
		exit(0);
	}

	// Clear the screen
	system("CLS");

	// Display summary screen and exit
	root.displayHeader();
	cout << "     Asset Tag: " << Atag << endl;
	root.PrintDetails(activeMachine);
	cout << endl << endl << "The above detail have been written to Details.csv." << endl << endl << "Will exit in 5 seconds...";
	system("timeout /t 5 >nul");

	return 0;
}