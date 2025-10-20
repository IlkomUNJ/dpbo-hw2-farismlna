#include "menu_handler.h"
#include "date_util.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// Helper 
inline void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- BANK MENU HANDLER ---
void handleBankingMenu(BankCustomer& account) {
    bool running = true;
    while (running) {
        cout << "\n--- Banking Menu ---\n";
        cout << "1. Check Balance\n";
        cout << "2. Topup / Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. List Cash Flow (Today/Month)\n";
        cout << "5. Back\n";
        int choice;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input." << endl;
            clear_input();
            continue;
        }

        double amount;
        switch (choice) {
            case 1:
                account.printInfo();
                break;
            case 2:
                cout << "Enter topup amount: "; cin >> amount;
                if (cin.fail() || amount <= 0) { cout << "Invalid amount.\n"; clear_input(); break; }
                account.addBalance(amount, "Manual Topup/Deposit");
                cout << "Topup successful. New balance: $" << account.getBalance() << endl;
                break;
            case 3:
                cout << "Enter withdraw amount: "; cin >> amount;
                if (cin.fail() || amount <= 0) { cout << "Invalid amount.\n"; clear_input(); break; }
                account.withdrawBalance(amount, "Manual Withdrawal");
                break;
            case 4: { // List Cash Flow
                int cfChoice;
                cout << "1. Today (1 Day)\n2. Last Month (30 Days)\n3. Back\n";
                cout << "Enter choice: "; cin >> cfChoice;
                if (cin.fail()) { cout << "Invalid choice.\n"; clear_input(); break; }
                
                if (cfChoice == 1) {
                    account.listCashFlow(1, "Today");
                } else if (cfChoice == 2) {
                    account.listCashFlow(30, "Last Month");
                }
                break;
            }
            case 5:
                running = false;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }
}

// --- BUYER MENU HANDLER ---
void handleBuyerMenu(Buyer& loggedInBuyer, seller* sellerPtr, Store& store) {
    bool running = true;
    while (running) {
        cout << "\n--- Buyer Menu (" << loggedInBuyer.getName() << ") ---\n";
        cout << "1. Check Account Status\n";
        cout << "2. Banking Functions\n";
        cout << "3. Upgrade Account to Seller (If not already)\n";
        cout << "4. Browse Store (Demo Purchase)\n";
        cout << "5. List/Manage Orders\n";
        cout << "6. Check Spending (last k days)\n";
        cout << "7. Logout\n";
        cout << "8. Delete Account (Not implemented)\n";
        
        int choice;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input." << endl;
            clear_input();
            continue;
        }

        switch (choice) {
            case 1: // Check Account Status
                loggedInBuyer.printInfo();
                if (sellerPtr) {
                    cout << "\n--- Linked Seller Account ---\n";
                    sellerPtr->printInfo();
                }
                break;
            case 2: // Banking Functions
                handleBankingMenu(*loggedInBuyer.getAccount());
                break;
            case 3: // Upgrade Account to Seller
                if (sellerPtr) {
                    cout << "You are already a Seller (" << sellerPtr->getSellerName() << ").\n";
                } else {
                    cout << "[PLACEHOLDER] Feature to be implemented: Seller Upgrade\n";
                }
                break;
            case 4: { // Browse Store
                int sId, iId, qty;
                cout << "\n--- Browse Store ---\n";
                
                // Listing sellers
                cout << "Available Sellers (ID: Name):\n";
                for (const auto& s : store.getSellers()) {
                    cout << s.getSellerId() << ": " << s.getSellerName() << endl;
                }
                
                cout << "Enter Seller ID to purchase from: "; cin >> sId;
                cout << "Enter Item ID to purchase: "; cin >> iId;
                cout << "Enter Quantity: "; cin >> qty;

                if (cin.fail() || qty <= 0) { cout << "Invalid input.\n"; clear_input(); break; }

                map<int, int> order;
                order[iId] = qty;
                
                store.processTransaction(loggedInBuyer.getId(), sId, order);
                break;
            }
            case 5: { // List/Manage Orders
                cout << "\n--- List/Manage Orders ---\n";
                store.listOrdersByBuyer(loggedInBuyer.getId()); 
                
                int tId;
                cout << "Enter Transaction ID to manage (0 to skip payment): ";
                if (!(cin >> tId)) { cout << "Invalid ID format." << endl; clear_input(); break; }

                if (tId > 0) {
                    cout << "1. Pay Order\n2. Cancel Order\n3. Back\nEnter action: ";
                    int action; cin >> action;
                    if (cin.fail()) { cout << "Invalid choice.\n"; clear_input(); break; }

                    if (action == 1) {
                        store.payOrder(tId);
                    } else if (action == 2) {
                        store.cancelOrder(tId);
                    }
                }
                break;
            }
            case 6: { // Check Spending (last k days)
                int k;
                cout << "Check spending for the last k days. Enter k: "; cin >> k;
                if (cin.fail() || k <= 0) { cout << "Invalid number of days.\n"; clear_input(); break; }
                loggedInBuyer.getAccount()->listCashFlow(k, to_string(k) + " Days");
                break;
            }
            case 7: // Logout
                running = false;
                break;
            case 8: // Delete Account
                cout << "[PLACEHOLDER] Feature to be implemented: Account Deletion\n";
                running = false;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }
}

// --- SELLER MENU HANDLER ---
void handleSellerMenu(seller& loggedInSeller, Store& store) {
    bool running = true;
    while (running) {
        cout << "\n--- Seller Menu (" << loggedInSeller.getSellerName() << ") ---\n";
        cout << "1. Check Inventory\n";
        cout << "2. Add/Replenish Item to Inventory\n";
        cout << "3. Remove/Discard Item from Inventory\n";
        cout << "4. View Orders Placed (Paid/Uncompleted)\n";
        cout << "5. Discovery Reports (Top Items / Loyal Customers)\n";
        cout << "6. Logout\n";
        
        int choice;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input." << endl;
            clear_input();
            continue;
        }

        switch (choice) {
            case 1: // Check Inventory
                cout << "\n--- Inventory for " << loggedInSeller.getSellerName() << " ---\n";
                // loggedInSeller.printInventory();
                cout << "[PLACEHOLDER] Inventory listing logic needs to be fully implemented.\n";
                break;

            case 2: { // Add/Replenish Item
                int id, qty; double price; string name;
                cout << "Enter Item ID to add/replenish: "; cin >> id;
                cout << "Enter Quantity: "; cin >> qty;
                if (cin.fail() || qty <= 0) { cout << "Invalid input.\n"; clear_input(); break; }
                
                Item* existingItem = loggedInSeller.findItem(id);
                if (existingItem) {
                    loggedInSeller.replenishItem(id, qty);
                } else {
                    cout << "Enter New Item Name (no spaces): "; cin >> name;
                    cout << "Enter Price: "; cin >> price;
                    if (cin.fail() || price <= 0) { cout << "Invalid price.\n"; clear_input(); break; }
                    loggedInSeller.registerNewItem(id, name, qty, price);
                }
                break;
            }
            case 3: { // Remove/Discard Item
                int id;
                cout << "Enter Item ID to discard: "; cin >> id;
                if (cin.fail()) { cout << "Invalid ID.\n"; clear_input(); break; }
                loggedInSeller.discardItem(id);
                break;
            }
            case 4: { // View Orders Placed (Paid/Uncompleted)
                cout << "\n--- Seller Orders ---\n";
                store.listPaidUncompletedTransactions(loggedInSeller.getSellerId());
                
                int tId;
                cout << "Enter Transaction ID to Complete (0 to skip): "; cin >> tId;
                if (tId > 0) {
                    store.completeOrder(tId);
                }
                break;
            }
            case 5: { // Discovery Reports
                int reportChoice;
                cout << "\n--- Seller Discovery Reports ---\n";
                cout << "1. Discover top k most popular items (This store)\n";
                cout << "2. Discover loyal customer (This store)\n";
                cout << "Enter report choice: "; cin >> reportChoice;
                if (cin.fail()) { cout << "Invalid choice.\n"; clear_input(); break; }
                
                if (reportChoice == 1) {
                    int k; cout << "Enter k (top items): "; cin >> k;
                    if (cin.fail() || k <= 0) { cout << "Invalid input.\n"; clear_input(); break; }
                    
                    store.listSellerTopItems(loggedInSeller.getSellerId(), k); 
                    
                } else if (reportChoice == 2) {
                
                    store.listSellerLoyalCustomers(loggedInSeller.getSellerId()); 
                    
                } else {
                    cout << "Invalid report option.\n";
                }
                break;
            }
            case 6:
                running = false;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }
}