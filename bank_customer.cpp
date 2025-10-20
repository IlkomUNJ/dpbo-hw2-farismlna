#include "bank_customer.h"
#include <iostream>
#include <ctime>

int BankCustomer::getId() const {
    return this->id;
}

string BankCustomer::getName() const {
    return this->name;
}

double BankCustomer::getBalance() const {
    return this->balance;
}

void BankCustomer::setName(const string& newName) {
    this->name = newName;
}

void BankCustomer::setBalance(double amount) {
    this->balance = amount;
}

void BankCustomer::printInfo() const {
    std::cout << "--- Banking Account Info ---\n";
    std::cout << "Customer Name: " << this->name << std::endl;
    std::cout << "Customer ID: " << this->id << std::endl;
    std::cout << "Balance: $" << this->balance << std::endl;
}

void BankCustomer::addBalance(double amount, const string& description) {
    this->balance += amount;
    transactionHistory.emplace_back(amount, TransactionType::CREDIT, description);
}

bool BankCustomer::withdrawBalance(double amount, const string& description){
    if (amount > this->balance) {
        std::cout << "Rejected: Insufficient funds!" << std::endl;
        return false;
    }
    this->balance -= amount;
    transactionHistory.emplace_back(amount, TransactionType::DEBIT, description);
    return true;
}

void BankCustomer::listCashFlow(int days, const string& period) {
    time_t now = time(nullptr);
    time_t cutoff = now - (days * 24 * 60 * 60);

    cout << "\n--- Cash Flow for the Last " << period << " ---\n";
    double totalCredit = 0.0;
    double totalDebit = 0.0;
    bool found = false;

    for (const auto& record : transactionHistory) {
        if (record.getTimestamp() >= cutoff) {
            record.printInfo();
            found = true;
            if (record.getType() == TransactionType::CREDIT) {
                totalCredit += record.getAmount();
            } else {
                totalDebit += record.getAmount();
            }
        }
    }

    if (!found) {
        cout << "No transactions recorded in the last " << period << ".\n";
    }

    cout << "-------------------------------------------\n";
    cout << "Total Credit: $" << totalCredit << endl;
    cout << "Total Debit: $" << totalDebit << endl;
    cout << "Net Flow: $" << (totalCredit - totalDebit) << endl;
}