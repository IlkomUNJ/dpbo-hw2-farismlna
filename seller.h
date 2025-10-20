#pragma once
#include "buyer.h"
#include "item.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class seller : public Buyer {
private:
    int sellerId;
    string sellerName;
    string sellerAddress;
    string sellerPhone;
    string sellerEmail;

    vector<Item> items; 

public:
    seller() = default;

    seller(const seller&) = default;
    seller(seller&&) = default;
    seller& operator=(const seller&) = default;
    seller& operator=(seller&&) = default;

    seller(const Buyer& buyer, int sellerId, const string& sellerName, const string& sellerAddress, 
            const string& sellerPhone, const string& sellerEmail)
        : Buyer(buyer.getId(), buyer.getName(), buyer.getAddress(), buyer.getPhone(), buyer.getEmail(), *buyer.getAccount()), 
            sellerId(sellerId), sellerName(sellerName), sellerAddress(sellerAddress), 
            sellerPhone(sellerPhone), sellerEmail(sellerEmail) {
            Buyer::setId(buyer.getId());
        }

    virtual ~seller() = default;

    int getSellerId() const { return sellerId; }
    string getSellerName() const { return sellerName; }
    string getSellerAddress() const { return sellerAddress; }
    string getSellerPhone() const { return sellerPhone; }
    string getSellerEmail() const { return sellerEmail; }

    void setSellerName(const string& name) { sellerName = name; }
    void setSellerAddress(const string& address) { sellerAddress = address; }
    void setSellerPhone(const string& phone) { sellerPhone = phone; }
    void setSellerEmail(const string& email) { sellerEmail = email; }

    const std::vector<Item>& getItems() const { 
        return items; 
    }

    void addNewItem(int newId, const std::string& newName, int newQuantity, double newPrice) {
        Item newItem(newId, newName, newQuantity, newPrice);
        items.push_back(newItem);
    }

    void updateItem(int itemId, const std::string& newName, int newQuantity, double newPrice) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.alterItemById(itemId, newName, newQuantity, newPrice); 
            }
        }
    }

    void makeItemVisibleToCustomer(int itemId) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.setDisplay(true); 
                break;
            }
        }
    }

    void registerNewItem(int newId, const std::string& newName, int newQuantity, double newPrice) {
        items.emplace_back(newId, newName, newQuantity, newPrice);
        cout << "Item '" << newName << "' registered with price $" << newPrice << endl;
    }

    void replenishItem(int itemId, int quantity) { 
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.setQuantity(item.getQuantity() + quantity);
                cout << "Replenished Item ID " << itemId << ". New stock: " << item.getQuantity() << endl;
                return;
            }
        }
        cout << "Item ID " << itemId << " not found for replenishment." << endl;
    }
    
    void discardItem(int itemId) {
        // Penggunaan remove_if dan erase sudah benar
        items.erase(remove_if(items.begin(), items.end(), 
                     [itemId](const Item& item){ return item.getId() == itemId; }), items.end());
        cout << "Item ID " << itemId << " discarded." << endl;
    }
    
    Item* findItem(int itemId) {
        for (auto& item : items) {
            if (item.getId() == itemId) return &item;
        }
        return nullptr;
    }
    
    void reduceItemStock(int itemId, int quantity) {
        Item* item = findItem(itemId);
        if (item && item->getQuantity() >= quantity) {
            item->setQuantity(item->getQuantity() - quantity);
        }
    }

    void printInfo() const {
        cout << "Seller ID: " << sellerId << endl;
        cout << "Seller Name: " << sellerName << endl;
        cout << "Seller Address: " << sellerAddress << endl;
        cout << "Seller Phone: " << sellerPhone << endl;
        cout << "Seller Email: " << sellerEmail << endl;
        Buyer::printInfo(); 
    }
};