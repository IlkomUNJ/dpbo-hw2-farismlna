#ifndef STORE_H
#define STORE_H

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "buyer.h"
#include "seller.h"
#include "transaction.h"
#include "date_util.h" 

using namespace std;

class Store {
private:
    vector<Buyer> buyers;
    vector<seller> sellers; 
    vector<Transaction> transactions;
    
    int nextTransactionId = 1;

    Transaction* findTransaction(int id); 
    
public:
    Store() = default;
    
    Buyer* findBuyer(int id);
    seller* findSeller(int id);
    
    void addBuyer(const Buyer& b) { buyers.push_back(b); }
    void addSeller(const seller& s) { sellers.push_back(s); }
    
    bool removeBuyer(int id);
    bool removeSeller(int id); 

    bool processTransaction(int buyerId, int sellerId, map<int, int> itemQuantities);
    void payOrder(int transactionId);
    void completeOrder(int transactionId);
    void cancelOrder(int transactionId); 
    
    void listOrdersByBuyer(int buyerId) const;
    void listPaidUncompletedTransactions(int sellerId) const;

    void createBuyerAccount(int id, const string& name, const string& addr, const string& phone, const string& email, double initialBalance, vector<BankCustomer>& bankAccounts);
    void createSellerAccount(int buyerId, int sellerId, const string& storeName, const string& storeAddr, const string& storePhone, const string& storeEmail);

    void viewAllBuyerDetails() const;
    void viewAllSellerDetails() const;

    void searchBuyer(int opt, const string& query) const;
    void searchSeller(int opt, const string& query) const;

    void listRecentTransactions(int k) const;
    void listMostFrequentItems(int m) const; 

    void listMostActiveBuyers(int n) const;
    void listMostActiveSellers(int n) const;

    void listSellerTopItems(int sellerId, int k) const;
    void listSellerLoyalCustomers(int sellerId) const;
    
    void listBankTransactions(int days) const;
    void listAllBankCustomers() const;
    void listDormantAccounts(int days) const;
    void listTopTransactionUsers(int n) const;

    const vector<Buyer>& getBuyers() const { return buyers; }
    const vector<seller>& getSellers() const { return sellers; }
    const vector<Transaction>& getTransactions() const { return transactions; }

    // Serialization
    void saveState();
    void loadState();
};

#endif 