#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>
#include <vector>
#include "transaction_record.h" 

using namespace std;

class BankCustomer {
private:
    int id;
    string name;
    double balance;
    vector<TransactionRecord> transactionHistory; 

public:
    BankCustomer(int id, const string& name, double balance) : id(id), name(name), balance(balance) {}

    int getId() const;
    string getName() const;
    double getBalance() const;
    const vector<TransactionRecord>& getHistory() const { return transactionHistory; } // New Getter

    void printInfo() const;
    void setName(const string& name);
    void setBalance(double balance);
    
    void addBalance(double amount, const string& description); 
    bool withdrawBalance(double amount, const string& description);

    void listCashFlow(int days, const string& period);
};

#endif