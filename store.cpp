#include "store.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>

const std::string DATA_FILE = "store_data_boost.txt";

struct ItemFrequency {
    int itemId;
    std::string name;
    int totalQuantitySold;

    bool operator<(const ItemFrequency& other) const {
        return totalQuantitySold > other.totalQuantitySold; 
    }
};

template <typename T>
struct RankItem {
    int id;
    int count;
    T* dataPtr;

    bool operator<(const RankItem& other) const {
        return count > other.count;
    }
};

Buyer* Store::findBuyer(int id) {
    for (auto& b : buyers) {
        if (b.getId() == id) return &b;
    }
    return nullptr;
}

seller* Store::findSeller(int id) {
    for (auto& s : sellers) {
        if (s.getSellerId() == id) return &s;
    }
    return nullptr;
}

Transaction* Store::findTransaction(int id) {
    for (auto& t : transactions) {
        if (t.getId() == id) return &t;
    }
    return nullptr;
}

bool Store::removeBuyer(int id) {
    auto it = remove_if(buyers.begin(), buyers.end(), 
                        [id](const Buyer& b) { return b.getId() == id; });
    if (it != buyers.end()) {
        buyers.erase(it, buyers.end());
        return true;
    }
    return false;
}

bool Store::removeSeller(int id) {
    auto it = remove_if(sellers.begin(), sellers.end(), 
                        [id](const seller& s) { return s.getSellerId() == id; });
    if (it != sellers.end()) {
        sellers.erase(it, sellers.end());
        return true;
    }
    return false;
}

bool Store::processTransaction(int buyerId, int sellerId, std::map<int, int> itemQuantities) {
    Buyer* buyer = findBuyer(buyerId);
    seller* seller = findSeller(sellerId);
    
    if (!buyer || !seller) {
        cout << "Transaction Failed: Buyer or Seller not found." << endl;
        return false;
    }

    vector<OrderItem> orderItems;
    double totalAmount = 0.0;
    
    for (const auto& pair : itemQuantities) {
        int itemId = pair.first;
        int requestedQty = pair.second;
        
        Item* inventoryItem = seller->findItem(itemId); 
        
        if (!inventoryItem || inventoryItem->getQuantity() < requestedQty) {
            cout << "Transaction Failed: Item ID " << itemId << " out of stock or not found at " << seller->getSellerName() << "." << endl;
            return false;
        }
        
        orderItems.emplace_back(itemId, inventoryItem->getName(), inventoryItem->getPrice(), requestedQty);
        totalAmount += inventoryItem->getPrice() * requestedQty;
    }
    
    if (buyer->getAccount()->getBalance() < totalAmount) {
        cout << "Transaction Failed: Insufficient balance in Buyer's account. Required: $" << totalAmount << endl;
        return false;
    }

    Transaction newTransaction(nextTransactionId++, buyerId, sellerId, orderItems);
    transactions.push_back(newTransaction);
    
    for (const auto& orderItem : orderItems) {
        seller->reduceItemStock(orderItem.getItemId(), orderItem.getQuantity()); 
    }
    
    cout << "Order placed successfully! Transaction ID: " << newTransaction.getId() << ". Status: PENDING" << endl;
    return true;
}

void Store::payOrder(int transactionId) {
    Transaction* t = findTransaction(transactionId);
    if (!t || t->getStatus() != OrderStatus::PENDING) {
        cout << "Payment Failed: Transaction not found or not PENDING." << endl;
        return;
    }

    Buyer* buyer = findBuyer(t->getBuyerId());
    seller* seller = findSeller(t->getSellerId());
    double amount = t->getTotalAmount();
    string desc = "Purchase for Order ID " + to_string(transactionId);
    
    if (!buyer || !seller) {
        cout << "Payment Failed: Linked Buyer or Seller account lost." << endl;
        return;
    }

    if (buyer->getAccount()->withdrawBalance(amount, desc)) { 
        seller->getAccount()->addBalance(amount, desc); 
        t->setStatus(OrderStatus::PAID);
        cout << "Payment Successful! Transaction ID " << transactionId << " is now PAID." << endl;
    } else {
        cout << "Payment Failed." << endl;
    }
}

void Store::completeOrder(int transactionId) {
    Transaction* t = findTransaction(transactionId);
    if (!t || t->getStatus() != OrderStatus::PAID) {
        cout << "Failed: Transaction not found or not PAID. Current status: " << (t ? (int)t->getStatus() : -1) << ".\n";
        return;
    }
    t->setStatus(OrderStatus::COMPLETED);
    cout << "Success: Order ID " << transactionId << " marked as COMPLETED.\n";
}

void Store::cancelOrder(int transactionId) {
    Transaction* t = findTransaction(transactionId);
    if (!t || t->getStatus() != OrderStatus::PENDING) {
        cout << "Failed: Only PENDING orders can be cancelled.\n";
        return;
    }
    t->setStatus(OrderStatus::CANCELLED);
    cout << "Success: Order ID " << transactionId << " cancelled.\n";
}

void Store::listOrdersByBuyer(int buyerId) const {
    cout << "--- Orders for Buyer ID " << buyerId << " ---\n";
    bool found = false;
    for (const auto& t : transactions) {
        if (t.getBuyerId() == buyerId) {
            t.printInfo();
            found = true;
            cout << "-----------------------\n";
        }
    }
    if (!found) {
        cout << "No orders found for this buyer.\n";
    }
}

void Store::listPaidUncompletedTransactions(int sellerId) const {
    cout << "--- Paid but Uncompleted Transactions ---\n";
    bool found = false;
    for (const auto& t : transactions) {
        if (t.getStatus() == OrderStatus::PAID) {
            if (sellerId == 0 || t.getSellerId() == sellerId) {
                t.printInfo();
                found = true;
                cout << "-----------------------\n";
            }
        }
    }
    if (!found) {
        cout << "No pending completion orders found.\n";
    }
}

void Store::viewAllBuyerDetails() const {
    cout << "\n--- ALL BUYER DETAILS ---\n";
    if (buyers.empty()) {
        cout << "No buyers available.\n";
    } else {
        for (const auto &b : buyers) {
            b.printInfo();
            cout << "----------------------\n";
        }
    }
}

void Store::viewAllSellerDetails() const {
    cout << "\n--- ALL SELLER DETAILS ---\n";
    if (sellers.empty()) {
        cout << "No sellers available.\n";
    } else {
        for (const auto &s : sellers) {
            s.printInfo();
            cout << "----------------------\n";
        }
    }
}

void Store::searchBuyer(int opt, const string& query) const {
    bool found = false;
    cout << "\n--- Search Buyer Results ---\n";
    for (const auto& b : buyers) {
        bool match = false;
        if (opt == 1 && to_string(b.getId()) == query) match = true;
        else if (opt == 2 && b.getName() == query) match = true;
        else if (opt == 3 && b.getPhone() == query) match = true;
        else if (opt == 4 && b.getAddress() == query) match = true;

        if (match) {
            b.printInfo();
            found = true;
            cout << "----------------------\n";
        }
    }
    if (!found) cout << "Buyer not found.\n";
}

void Store::searchSeller(int opt, const string& query) const {
    bool found = false;
    cout << "\n--- Search Seller Results ---\n";
    for (const auto& s : sellers) {
        bool match = false;
        if (opt == 1 && to_string(s.getSellerId()) == query) match = true;
        else if (opt == 2 && s.getSellerName() == query) match = true;
        else if (opt == 3 && s.getSellerPhone() == query) match = true;
        else if (opt == 4 && s.getSellerAddress() == query) match = true;

        if (match) {
            s.printInfo();
            found = true;
            cout << "----------------------\n";
        }
    }
    if (!found) cout << "Seller not found.\n";
}

void Store::createBuyerAccount(int id, const string& name, const string& addr, const string& phone, const string& email, double initialBalance, vector<BankCustomer>& bankAccounts) {
    if (findBuyer(id)) {
        cout << "Error: Buyer ID " << id << " already exists.\n";
        return;
    }
    
    bankAccounts.emplace_back(id, name, initialBalance);
    BankCustomer& newAcc = bankAccounts.back();
    
    Buyer newBuyer(id, name, addr, phone, email, newAcc);
    addBuyer(newBuyer);
    
    cout << "Success! Buyer '" << name << "' created with Account ID " << id << ".\n";
}

void Store::createSellerAccount(int buyerId, int sellerId, const string& storeName, const string& storeAddr, const string& storePhone, const string& storeEmail) {
    if (findSeller(sellerId)) {
        cout << "Error: Seller ID " << sellerId << " already exists.\n";
        return;
    }

    Buyer* linkedBuyer = findBuyer(buyerId);
    if (!linkedBuyer) {
        cout << "Error: Cannot create Seller. Linked Buyer ID " << buyerId << " not found.\n";
        return;
    }
    
    seller newSeller(*linkedBuyer, sellerId, storeName, storeAddr, storePhone, storeEmail);
    addSeller(newSeller);
    
    cout << "Success! Seller Store '" << storeName << "' created and linked to Buyer ID " << buyerId << ".\n";
}

void Store::listRecentTransactions(int k) const {
    cout << "--- Transactions in the Last " << k << " Days ---\n";
    bool found = false;
    for (const auto& t : transactions) {
        if (DateUtil::isWithinLastDays(t.getDate(), k)) {
            t.printInfo();
            found = true;
            cout << "-----------------------\n";
        }
    }
    if (!found) {
        cout << "No transactions found in the specified period.\n";
    }
}

void Store::listMostFrequentItems(int m) const {
    cout << "--- Top " << m << " Most Frequent Item Transactions (by total quantity sold) ---\n";
    
    map<int, pair<int, string>> itemSales;

    for (const auto& t : transactions) {
        for (const auto& item : t.getItems()) {
            int itemId = item.getItemId();
            int qty = item.getQuantity();
            string name = item.getName();
            
            itemSales[itemId].first += qty;
            itemSales[itemId].second = name;
        }
    }

    vector<ItemFrequency> rankings;
    for (const auto& pair : itemSales) {
        rankings.push_back({
            pair.first, 
            pair.second.second, // Item Name
            pair.second.first   // Total Quantity Sold
        });
    }

    // Sort descending
    sort(rankings.begin(), rankings.end());

    int limit = min((int)rankings.size(), m);
    if (limit == 0) {
        cout << "No items have been sold yet.\n";
        return;
    }

    for (int i = 0; i < limit; ++i) {
        cout << (i + 1) << ". Item ID " << rankings[i].itemId
             << " (" << rankings[i].name << "): " 
             << rankings[i].totalQuantitySold << " units sold\n";
    }
}

void Store::listSellerTopItems(int sellerId, int k) const {
    cout << "--- Top " << k << " Most Frequent Items for Seller ID " << sellerId << " ---\n";
    
    map<int, pair<int, string>> itemSales; // Item ID -> {Total Quantity, Item Name}

    for (const auto& t : transactions) {
        if (t.getSellerId() != sellerId) continue;
        
        for (const auto& item : t.getItems()) {
            itemSales[item.getItemId()].first += item.getQuantity();
            itemSales[item.getItemId()].second = item.getName();
        }
    }

    vector<ItemFrequency> rankings;
    for (const auto& pair : itemSales) {
        rankings.push_back({
            pair.first, 
            pair.second.second, 
            pair.second.first
        });
    }

    sort(rankings.begin(), rankings.end());

    int limit = min((int)rankings.size(), k);
    if (limit == 0) {
        cout << "Seller " << sellerId << " has not sold any items yet.\n";
        return;
    }

    for (int i = 0; i < limit; ++i) {
        cout << (i + 1) << ". Item ID " << rankings[i].itemId
             << " (" << rankings[i].name << "): " 
             << rankings[i].totalQuantitySold << " units sold\n";
    }
}

void Store::listMostActiveBuyers(int n) const {
    std::cout << "--- Top " << n << " Most Active Buyers (by Transaction Count) ---\n";
    std::map<int, int> buyerTxnCount; // {Buyer ID: Count}

    for (const auto& t : transactions) {
        if (t.getStatus() == OrderStatus::PAID || t.getStatus() == OrderStatus::COMPLETED) {
            buyerTxnCount[t.getBuyerId()]++;
        }
    }

    std::vector<RankItem<Buyer>> rankings;
    for (const auto& pair : buyerTxnCount) {
        Buyer* b = const_cast<Store*>(this)->findBuyer(pair.first); 
        if (b) {
            rankings.push_back({pair.first, pair.second, b});
        }
    }

    std::sort(rankings.begin(), rankings.end());

    int limit = std::min((int)rankings.size(), n);
    for (int i = 0; i < limit; ++i) {
        std::cout << (i + 1) << ". Buyer ID " << rankings[i].id
             << " (" << rankings[i].dataPtr->getName() << "): " 
             << rankings[i].count << " Transactions\n";
    }
    if (rankings.empty()) std::cout << "No transactions recorded yet.\n";
}

// --- Implementasi listSellerLoyalCustomers ---
void Store::listSellerLoyalCustomers(int sellerId) const {
    cout << "--- Loyal Customers for Seller ID " << sellerId << " (by Total Orders) ---\n";
    map<int, int> buyerOrderCount;

    for (const auto& t : transactions) {
        if (t.getSellerId() == sellerId && t.getStatus() != OrderStatus::CANCELLED) {
            buyerOrderCount[t.getBuyerId()]++;
        }
    }

    vector<RankItem<Buyer>> rankings;
    for (const auto& pair : buyerOrderCount) {
        Buyer* b = const_cast<Store*>(this)->findBuyer(pair.first);
        if (b) {
            rankings.push_back({pair.first, pair.second, b});
        }
    }

    sort(rankings.begin(), rankings.end());

    if (rankings.empty()) {
        cout << "No completed or paid orders found for this seller.\n";
        return;
    }
    
    int rank = 1;
    for (const auto& item : rankings) {
        cout << rank++ << ". Buyer ID " << item.id
             << " (" << item.dataPtr->getName() << "): " 
             << item.count << " total orders\n";
    }
}

// --- Reporting Functions (Bank) ---

void Store::listBankTransactions(int days) const {
    cout << "--- All Bank Transactions in the Last " << days << " Days ---\n";
    bool foundAny = false;
    for (const auto& b : buyers) {
        const auto& history = b.getAccount()->getHistory();
        bool foundBuyerTxn = false;
        
        cout << ">> Buyer ID " << b.getId() << " (" << b.getName() << ")\n";
        
        for (const auto& record : history) {
            if (DateUtil::isWithinLastDays(record.getTimestamp(), days)) {
                record.printInfo();
                foundBuyerTxn = true;
                foundAny = true;
            }
        }
        if (!foundBuyerTxn) {
            cout << "   (No bank activity in the last " << days << " days)\n";
        }
    }
    if (!foundAny) {
        cout << "No bank activity found across all customers in the specified period.\n";
    }
}

void Store::listAllBankCustomers() const {
    cout << "--- List All Bank Customers ---\n";
    if (buyers.empty()) {
        cout << "No buyers registered yet.\n";
        return;
    }
    for (const auto& b : buyers) {
        b.getAccount()->printInfo();
        cout << "-------------------------------------------\n";
    }
}

void Store::listDormantAccounts(int days) const {
    cout << "--- Dormant Accounts (No transaction in " << days << " days) ---\n";
    time_t now = time(nullptr);
    time_t cutoff = now - (days * 24 * 60 * 60);
    bool foundDormant = false;

    for (const auto& b : buyers) {
        time_t lastTxnTime = 0;
        const auto& history = b.getAccount()->getHistory();
        
        for (const auto& record : history) {
            if (record.getTimestamp() > lastTxnTime) {
                lastTxnTime = record.getTimestamp();
            }
        }
        
        if (lastTxnTime == 0 || lastTxnTime < cutoff) {
            b.getAccount()->printInfo();
            cout << "   (Last activity: " << (lastTxnTime == 0 ? "None" : DateUtil::formatTime(lastTxnTime)) << ")\n";
            cout << "-------------------------------------------\n";
            foundDormant = true;
        }
    }
    if (!foundDormant) {
        cout << "No dormant accounts found.\n";
    }
}

void Store::listTopTransactionUsers(int n) const {
    cout << "--- Top " << n << " Transaction Users Today ---\n";
    map<int, int> userTxnCount; 
    vector<Buyer*> activeUsers; 

    for (const auto& b : buyers) {
        int todayCount = 0;
        for (const auto& record : b.getAccount()->getHistory()) {
            if (DateUtil::isToday(record.getTimestamp())) {
                todayCount++;
            }
        }
        if (todayCount > 0) {
            userTxnCount[b.getId()] = todayCount;
            bool alreadyAdded = false;
            for (auto* user : activeUsers) {
                if (user->getId() == b.getId()) {
                    alreadyAdded = true;
                    break;
                }
            }
            if (!alreadyAdded) {
                activeUsers.push_back(const_cast<Buyer*>(&b));
            }
        }
    }

    vector<RankItem<Buyer>> rankings;
    for (auto& user : activeUsers) {
        if (userTxnCount.count(user->getId())) {
             rankings.push_back({user->getId(), userTxnCount[user->getId()], user});
        }
    }

    sort(rankings.begin(), rankings.end());

    int limit = min((int)rankings.size(), n);
    for (int i = 0; i < limit; ++i) {
        cout << (i + 1) << ". User ID " << rankings[i].id
             << " (" << rankings[i].dataPtr->getName() << "): " 
             << rankings[i].count << " Transactions Today\n";
    }
    if (rankings.empty()) cout << "No transaction activity today.\n";
}

void Store::saveState() {
    std::ofstream ofs(DATA_FILE); // Teks Archive
    if (!ofs.is_open()) {
        std::cerr << "[ERROR] Failed to open file for saving.\n";
        return;
    }
    
    std::cout << "\n[INFO] Saving state using Boost Serialization...\n";
}