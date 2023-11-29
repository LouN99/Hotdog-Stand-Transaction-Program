#ifndef TRANSACTIONS_CLASS_CLASS_H
#define TRANSACTIONS_CLASS_CLASS_H

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class TransactionsClass {

private:
  string   standIdentifcationStr;
  string   transactionIdStr;
  unsigned transactionArgumentUns;

public:
  const string& getIdentifcationStr() const
  { return standIdentifcationStr; };
  void          setIdentifcationStr(string standIdentifcationStr)
  { this-> standIdentifcationStr = standIdentifcationStr; }

  const string& getTransactionIdStr() const
  { return transactionIdStr;};
  void          setTransactionIdStr(string transactionIdStr)
  { this-> transactionIdStr = transactionIdStr; }

  unsigned getTransactionArgumentUns() const
  {return transactionArgumentUns; };
  void     setTransactionArgumentUns(unsigned transactionArgumentUns)
  { this-> transactionArgumentUns = transactionArgumentUns; }

};

#endif