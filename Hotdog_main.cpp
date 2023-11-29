/*
Program Name: Hot Dog Stands
Date: 2023-09-28
Author: Lewis Njau
Module Purpose: Program handles business transactions and tracks the activities of all the hot dog stands.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "HotDogStandsClass.h"
#include "TransactionsClass.h"


using namespace std;

const string HOTDOG_STANDS_FILE_NAME_STR = "InfoHotDogStands.txt";
const string TRANSACTIONS_FILE_NAME_STR = "InfoHotDogStandsTransactions.txt";
const string GLOBAL_SOLD_FILE_NAME_STR = "InfoGlobalSold.txt";

ostream& operator << (ostream& leftSideOutputStream, const HotDogStandsClass& hotDogStandObj);
ostream& operator << (ostream& leftSideOutputStream, const TransactionsClass& transactionsObj);
void CheckFileStreamOpen (string globalSoldFileNameStr, ifstream& inFile);
void ReadInFromGlobalSoldFile (string globalSoldFileNameStr);
void ReadInFromHotDogStandsFileTo (string hotDogStandsFileNameStr, HotDogStandsClass*& hotDogStandsAry, unsigned& hotDogStandsCountUns);
void ReadInFromTransactionsFileTo (string transactionsFileNameStr, TransactionsClass*& transactionsAry, unsigned& transactionsLineCountUns);
void displayStands (HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns);
unsigned getMatchingHotDogStandIndexUns (HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns, TransactionsClass* transactionsAry, unsigned transactionsCountUns);
void processTransactions (HotDogStandsClass*& hotDogStandsAry, unsigned totalNoHotDogStandsUns, TransactionsClass* transactionsAry, unsigned transactionsCountUns);
void UpdateHotDogStandsFile (string hotDogStandsFileNameStr, HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns);
void UpdateGlobalSoldFile (string globalSoldFileNameStr);

int main ()
{
    cout << setprecision(2) << fixed << showpoint;

    HotDogStandsClass* hotDogStandsAry = nullptr;
    unsigned           hotDogStandsCountUns;
    TransactionsClass* transactionsAry = nullptr;
    unsigned           transactionCountUns;

    ReadInFromGlobalSoldFile (GLOBAL_SOLD_FILE_NAME_STR);
    ReadInFromHotDogStandsFileTo (HOTDOG_STANDS_FILE_NAME_STR, hotDogStandsAry, hotDogStandsCountUns);
    ReadInFromTransactionsFileTo (TRANSACTIONS_FILE_NAME_STR, transactionsAry, transactionCountUns);

    cout << "Initial states of stands:" << endl;
    cout << "==========================" << endl << endl;
    displayStands(hotDogStandsAry, hotDogStandsCountUns);

    cout << "Process Transactions:" << endl;
    cout <<"======================" << endl << endl;
    processTransactions (hotDogStandsAry, hotDogStandsCountUns, transactionsAry, transactionCountUns);

    cout << "Final states of stands:" << endl;
    cout << "=======================" << endl << endl;
    displayStands(hotDogStandsAry, hotDogStandsCountUns);

    UpdateHotDogStandsFile(HOTDOG_STANDS_FILE_NAME_STR, hotDogStandsAry, hotDogStandsCountUns);
    UpdateGlobalSoldFile(GLOBAL_SOLD_FILE_NAME_STR);

    cout << "Please press enter key once or twice to end...";
    cin.ignore();
    cin.get();


    exit(EXIT_SUCCESS);
}

ostream& operator << (ostream& leftSideOutputStream, const HotDogStandsClass& hotDogStandObj)
{
    leftSideOutputStream <<
    "Stand Id       : " + hotDogStandObj.getIdStr() << endl <<
    "Address        : " + hotDogStandObj.getAddressStr() << endl <<
    "Price          : " << hotDogStandObj.getPriceFl() << endl <<
    "Inventory      : " << hotDogStandObj.getInventoryAmountUns() << endl <<
    "Store Sold     : " << hotDogStandObj.getSoldAmountUns() <<
    " at $" << hotDogStandObj.getPriceFl() << " ea." << endl;

    return leftSideOutputStream;
}

ostream& operator << (ostream& leftSideOutputStream, const TransactionsClass& transactionsObj)
{
    leftSideOutputStream <<
    "Transaction Stand Id   : " + transactionsObj.getIdentifcationStr() << endl <<
    "Transaction Id         : " + transactionsObj.getTransactionIdStr() << endl <<
    "Transaction Argument   : " << transactionsObj.getTransactionArgumentUns() << endl;

    return leftSideOutputStream;
}

void CheckFileStreamOpen (string globalSoldFileNameStr, ifstream& inFile)
{
    if(!inFile)
    {
        cout << "File " << globalSoldFileNameStr << " could not be opened!" << endl;
        cout << endl << "Press the enter key once or twice to continue..." << endl;
        cin.ignore();
        cin.get();
        exit(EXIT_FAILURE);
    }
}

void ReadInFromGlobalSoldFile (string globalSoldFileNameStr)
{
    ifstream inFile(globalSoldFileNameStr);
    CheckFileStreamOpen(globalSoldFileNameStr, inFile);

    inFile >> HotDogStandsClass::globalSoldCountUns;
    inFile.close();
}

void ReadInFromHotDogStandsFileTo (string hotDogStandsFileNameStr, HotDogStandsClass*& hotDogStandsAry, unsigned& hotDogStandsCountUns)
{
    ifstream inFile (hotDogStandsFileNameStr);
    CheckFileStreamOpen(hotDogStandsFileNameStr, inFile);

    string lineInFileBufferStr;

    // Count the number of lines in the file.
    hotDogStandsCountUns = 0;
    while (getline(inFile, lineInFileBufferStr))
        ++hotDogStandsCountUns;

    hotDogStandsAry = new HotDogStandsClass[hotDogStandsCountUns];

    inFile.clear(); inFile.seekg(0, ios::beg);

    string inputStr;
    constexpr char COMMA_DELIMITER_CHAR = ',';

    // For each line read from the file
    for (unsigned lineCount = 0; lineCount < hotDogStandsCountUns; ++lineCount)
    {
        getline(inFile, lineInFileBufferStr);
        istringstream isStringStream (lineInFileBufferStr);

        // Read everything up to the comma delimeter.
        getline(isStringStream, inputStr, COMMA_DELIMITER_CHAR);
        hotDogStandsAry[lineCount].setIdStr(inputStr);

        getline(isStringStream, inputStr, COMMA_DELIMITER_CHAR);
        hotDogStandsAry[lineCount].setAddressStr(inputStr);

        getline(isStringStream, inputStr, COMMA_DELIMITER_CHAR);
        hotDogStandsAry[lineCount].setPriceFl(stof(inputStr));

        getline(isStringStream, inputStr, COMMA_DELIMITER_CHAR);
        isStringStream >> inputStr;
        hotDogStandsAry[lineCount].setInventoryAmountUns(static_cast<unsigned>(stoul(inputStr)));

        getline(isStringStream, inputStr, COMMA_DELIMITER_CHAR);
        isStringStream >> inputStr;
        hotDogStandsAry[lineCount].setSoldAmountUns(static_cast<unsigned>(stoul(inputStr)));
    }
    inFile.close();
}

void ReadInFromTransactionsFileTo (string transactionsFileNameStr, TransactionsClass*& transactionsAry, unsigned& transactionsLineCountUns)
{
    ifstream inFile (transactionsFileNameStr);
    CheckFileStreamOpen(transactionsFileNameStr, inFile);

    string lineInFileBufferStr;

    // Count the number of lines in the file.
    transactionsLineCountUns = 0;
    while (getline(inFile, lineInFileBufferStr))
        ++transactionsLineCountUns;

    transactionsAry = new TransactionsClass[transactionsLineCountUns];

    inFile.clear(); inFile.seekg(0, ios::beg);

    string inputStr;
    constexpr char COMMA_DELIMTER_CHAR = ',';

    // For each line read from the file
    for (unsigned lineCount = 0; lineCount < transactionsLineCountUns; ++lineCount)
    {
        getline(inFile, lineInFileBufferStr);
        istringstream isStringStream (lineInFileBufferStr);

        // Read everything up to the comma delimeter.
        getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
        transactionsAry[lineCount].setIdentifcationStr(inputStr);

        getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
        transactionsAry[lineCount].setTransactionIdStr(inputStr);

        getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
        transactionsAry[lineCount].setTransactionArgumentUns(static_cast<unsigned>(stoul(inputStr)));
    }
    inFile.close();
}

void displayStands (HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns)
{
    for (unsigned displayIndex = 0; displayIndex < totalNoHotDogStandsUns; ++displayIndex)
        cout << hotDogStandsAry[displayIndex] << endl;

    cout << endl << "Global Sold : " << HotDogStandsClass::globalSoldCountUns << endl << endl;
    cout << "Please press enter key once or twice to continue...";
    cin.ignore();
    cin.get();

    cout << endl << endl;
}

unsigned getMatchingHotDogStandIndexUns (HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns, TransactionsClass* transactionsAry, unsigned transactionsCountUns)
{
    for (unsigned seekIndex = 0; seekIndex < totalNoHotDogStandsUns; ++seekIndex)
    {
        if (hotDogStandsAry[seekIndex].getIdStr() == transactionsAry[transactionsCountUns].getIdentifcationStr())
            return seekIndex;
    }
    return 0;
}

void processTransaction (TransactionsClass transactionObj, HotDogStandsClass& hotDogStandObj)
{
    if (transactionObj.getTransactionIdStr() == "stock inventory")
        hotDogStandObj.stockInventoryAmountUns(transactionObj.getTransactionArgumentUns());
    else if (transactionObj.getTransactionIdStr() == "buy")
        hotDogStandObj.hotDogsBuyUns(transactionObj.getTransactionArgumentUns());
}

void processTransactions (HotDogStandsClass*& hotDogStandsAry, unsigned totalNoHotDogStandsUns, TransactionsClass* transactionsAry, unsigned transactionsCountUns)
{
    for (unsigned transactionNoUns = 0; transactionNoUns < transactionsCountUns; ++transactionNoUns)
    {
        // Find correct stand.
        unsigned hotDogStandIndexUns = getMatchingHotDogStandIndexUns(hotDogStandsAry, totalNoHotDogStandsUns, transactionsAry, transactionNoUns);

        cout << "_______________________________________" << endl << endl;
        cout << "HotDog Stand Current Status :" << endl << endl;
        cout << hotDogStandsAry[hotDogStandIndexUns] << endl << endl;
        cout << "Transaction: " << endl << endl;
        cout << transactionsAry[transactionNoUns] << endl;
        processTransaction(transactionsAry[transactionNoUns], hotDogStandsAry[hotDogStandIndexUns]);
        cout << "HotDog Stand Status After Transaction :" << endl << endl;
        cout << hotDogStandsAry[hotDogStandIndexUns] << endl << endl;
        cout << "Global Sold : " << HotDogStandsClass::globalSoldCountUns << endl << endl;
        cout << "Please press enter key once or twice to continue...";
        cin.ignore();
        cin.get();
    }
}

void UpdateHotDogStandsFile (string hotDogStandsFileNameStr, HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns)
{
    ofstream outFile (hotDogStandsFileNameStr);

    for (unsigned writeIndex = 0; writeIndex < totalNoHotDogStandsUns; ++writeIndex)
    {
        outFile <<
        hotDogStandsAry[writeIndex].getIdStr()                  << "," <<
        hotDogStandsAry[writeIndex].getAddressStr()             << "," <<
        hotDogStandsAry[writeIndex].getPriceFl()                << "," <<
        hotDogStandsAry[writeIndex].getInventoryAmountUns()     << "," <<
        hotDogStandsAry[writeIndex].getSoldAmountUns()          << endl;
    }
    outFile.close();
}

void UpdateGlobalSoldFile (string globalSoldFileNameStr)
{
    ofstream outFile (globalSoldFileNameStr);
    outFile << HotDogStandsClass::globalSoldCountUns << endl;
}