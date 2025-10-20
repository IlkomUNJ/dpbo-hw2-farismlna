#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <vector>
#include <iostream>
#include <string>
#include <ctime>
#include "order_item.h"

using namespace std;

enum class OrderStatus { PENDING, PAID, COMPLETED, CANCELLED };

class Transaction {
private:
    int transactionId;
    int buyerId;
    int sellerId;
    time_t transactionDate;
    double totalAmount;
    OrderStatus status;
    vector<OrderItem> items;

public:
    Transaction(int id, int bId, int sId, const vector<OrderItem>& orderItems) 
        : transactionId(id), buyerId(bId), sellerId(sId), items(orderItems), status(OrderStatus::PENDING) {
        transactionDate = time(nullptr);
        totalAmount = 0.0;
        for (const auto& item : items) {
            totalAmount += item.getSubtotal();
        }
    }

    // Getters
    int getId() const { return transactionId; }
    int getBuyerId() const { return buyerId; }
    int getSellerId() const { return sellerId; }
    time_t getDate() const { return transactionDate; }
    double getTotalAmount() const { return totalAmount; }
    OrderStatus getStatus() const { return status; }
    const vector<OrderItem>& getItems() const { return items; }

    // Setters
    void setStatus(OrderStatus newStatus) { status = newStatus; }

    void printInfo() const {
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&transactionDate));
        
        cout << "--- Transaction ID: " << transactionId << " ---\n";
        cout << "Date: " << buffer << endl;
        cout << "Buyer ID: " << buyerId << ", Seller ID: " << sellerId << endl;
        cout << "Status: ";
        if (status == OrderStatus::PENDING) cout << "PENDING";
        else if (status == OrderStatus::PAID) cout << "PAID";
        else if (status == OrderStatus::COMPLETED) cout << "COMPLETED";
        else if (status == OrderStatus::CANCELLED) cout << "CANCELLED";
        cout << endl;
        
        cout << "Items:\n";
        for (const auto& item : items) {
            cout << "- " << item.getName() << " (x" << item.getQuantity() 
                 << ") @ $" << item.getPrice() << " = $" << item.getSubtotal() << endl;
        }
        cout << "Total Amount: $" << totalAmount << endl;
    }
};

#endif