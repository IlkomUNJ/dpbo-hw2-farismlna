#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "store.h"
#include "serializer.h"
#include "menu_handler.h"

enum PrimaryPrompt{LOGIN, REGISTER, EXIT, ADMIN_LOGIN};
enum RegisterPrompt{CREATE_BUYER, CREATE_SELLER, BACK};
using namespace std;

Store mainStore; 

vector<BankCustomer> bankAccounts; 

// Helper
inline void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    // Variable setup
    PrimaryPrompt prompt = LOGIN;
    RegisterPrompt regPrompt = CREATE_BUYER;
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;

    // Load state 
    Serializer::loadState(mainStore, bankAccounts);

    // DUMMY DATA SETUP
    if (mainStore.getBuyers().empty()) {
        std::cout << "[INFO] Initializing default data...\n";
        
        bankAccounts.emplace_back(1, "Alice", 5000.0);
        bankAccounts.emplace_back(2, "Bob", 3000.0);
        
        Buyer buyer1(1, "Alice", "Jl. Mawar", "0812345678", "alice@mail.com", bankAccounts[0]);
        mainStore.addBuyer(buyer1);

        Buyer buyer2(2, "Bob", "Jl. Melati", "0898765432", "bob@mail.com", bankAccounts[1]);
        mainStore.addBuyer(buyer2);

        seller seller1(buyer2, 101, "Bob Store", "Jl. Melati No.2", "0811223344", "storebob@mail.com");
        seller1.registerNewItem(1, "Gaming Mouse", 10, 50.0);
        seller1.registerNewItem(2, "Keyboard", 5, 120.0);
        mainStore.addSeller(seller1);
    }

    while (prompt != EXIT) {
        cout << "\n--- Main Menu ---\n";
        cout << "Select an option: " << endl;
        cout << "1. Login (Buyer/Seller)" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "4. Admin Login" << endl;
        
        int choice;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            clear_input();
            continue;
        }
        
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        
        switch (prompt) {
            case LOGIN: {
                int loginId;
                cout << "Enter Buyer/Seller ID to Login: ";
                if (!(cin >> loginId)) {
                    cout << "Invalid ID format." << endl;
                    clear_input();
                    break;
                }

                Buyer* loggedBuyer = mainStore.findBuyer(loginId);
                seller* loggedSeller = mainStore.findSeller(loginId);

                if (loggedBuyer) {
                    cout << "Login successful as Buyer: " << loggedBuyer->getName() << endl;
                    handleBuyerMenu(*loggedBuyer, loggedSeller, mainStore);
                } else if (loggedSeller) {
                    cout << "Login successful as Seller: " << loggedSeller->getSellerName() << endl;
                    handleSellerMenu(*loggedSeller, mainStore);
                } else {
                    cout << "Login failed. Invalid ID." << endl;
                }
                break;
            }
            case REGISTER:
                regPrompt = CREATE_BUYER;
                while (regPrompt != BACK){
                    cout << "\n--- Register Menu ---\n";
                    cout << "Select an option: " << endl;
                    cout << "1. Create Buyer Account (Must create Bank Account)\n";
                    cout << "2. Create Seller Account (Must link to existing Buyer)\n";
                    cout << "3. Back" << endl;
                    int regChoice;
                    cout << "Enter choice: ";
                    if (!(cin >> regChoice)) {
                        cout << "Invalid input." << endl;
                        clear_input();
                        continue;
                    }
                    regPrompt = static_cast<RegisterPrompt>(regChoice - 1);
                    
                    if (regPrompt == CREATE_BUYER) {
                        int id; string name, addr, phone, email; double balance;
                        cout << "--- New Buyer Registration ---\n";
                        cout << "Enter Buyer ID: "; cin >> id;
                        cout << "Enter Name (no space): "; cin >> name;
                        cout << "Enter Address (no space): "; cin >> addr;
                        cout << "Enter Phone: "; cin >> phone;
                        cout << "Enter Email (no space): "; cin >> email;
                        cout << "Enter Initial Balance: "; cin >> balance;
                        if (cin.fail()) { cout << "Invalid input.\n"; clear_input(); continue; }
                        
                        // Panggil fungsi create yang sudah diimplementasikan di Store
                        mainStore.createBuyerAccount(id, name, addr, phone, email, balance, bankAccounts);
                        
                    } else if (regPrompt == CREATE_SELLER) {
                        int sid, bid;
                        string storeName, storeAddr, storePhone, storeEmail;
                        cout << "--- New Seller Registration ---\n";
                        cout << "Enter Seller ID: "; cin >> sid;
                        cout << "Link to existing Buyer ID: "; cin >> bid;
                        cout << "Enter Store Name (no space): "; cin >> storeName;
                        cout << "Enter Store Address (no space): "; cin >> storeAddr;
                        cout << "Enter Store Phone: "; cin >> storePhone;
                        cout << "Enter Store Email (no space): "; cin >> storeEmail;
                        if (cin.fail()) { cout << "Invalid input.\n"; clear_input(); continue; }

                        // Panggil fungsi create Seller
                        mainStore.createSellerAccount(bid, sid, storeName, storeAddr, storePhone, storeEmail);
                    }
                }
                break;
            case EXIT:
                cout << "Exiting. Saving data state..." << endl;
                Serializer::saveState(mainStore, bankAccounts); 
                return 0;
            case ADMIN_LOGIN:
                handleAdminMenu(mainStore); // handler Admin
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }
    
    // Panggil save state di akhir
    // if (prompt == EXIT) {
    //     cout << "Exiting. Saving data state..." << endl;
    //     Serializer::saveState(mainStore, bankAccounts);
    // }
    
    return 0;
}