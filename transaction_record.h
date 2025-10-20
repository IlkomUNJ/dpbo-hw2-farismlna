#ifndef TRANSACTION_RECORD_H
#define TRANSACTION_RECORD_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

enum class TransactionType { DEBIT, CREDIT };

class TransactionRecord {
private:

    time_t timestamp;
    double amount;
    TransactionType type;
    string description;

public:
    TransactionRecord(double amount, TransactionType type, const string& description)
        : amount(amount), type(type), description(description) {
        timestamp = time(nullptr); 
    }

    time_t getTimestamp() const { return timestamp; }
    double getAmount() const { return amount; }
    TransactionType getType() const { return type; }
    string getDescription() const { return description; }

    void printInfo() const {
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
        cout << "[" << buffer << "] ";
        cout << (type == TransactionType::CREDIT ? "CREDIT" : "DEBIT")
             << " $" << amount << " - " << description << endl;
    }
};

#endif