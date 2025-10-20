#include "menu_handler.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

inline void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void handleAdminMenu(Store& store) {
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;
    
    bool viewRunning = true;
    
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;

    if (username != ADMIN_USERNAME || password != ADMIN_PASSWORD) {
        cout << "Invalid admin credentials. Access denied.\n";
        return;
    }

    bool adminRunning = true;
    bool searchRunning = true;
    bool createRunning = true;

    while (adminRunning) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. View All Buyers (Summary)\n";
        cout << "2. View All Sellers (Summary)\n";
        cout << "3. View All Details (Buyers or Sellers)\n";
        cout << "4. Search Buyer/Seller by Name/ID/Address/Phone\n";
        cout << "5. Create new Buyer/Seller/Bank account\n";
        cout << "6. Remove Buyer/Seller by ID\n";
        cout << "7. System Reports (Bank & Store)\n";
        cout << "8. Logout\n";
        
        int adminChoice;
        cout << "Enter choice: ";
        if (!(cin >> adminChoice)) {
            cout << "Invalid input." << endl;
            clear_input();
            continue;
        }

        const auto& storeBuyers = store.getBuyers();
        const auto& storeSellers = store.getSellers();

        switch (adminChoice) {
            case 1:
                cout << "\n--- List of Buyers ---\n";
                if (storeBuyers.empty()) { cout << "No buyers available.\n"; } 
                for (const auto &b : storeBuyers) {
                    cout << "ID: " << b.getId() << " | Name: " << b.getName()
                         << " | Balance: $" << b.getAccount()->getBalance() << endl;
                }
                break;
            case 2:
                cout << "\n--- List of Sellers ---\n";
                if (storeSellers.empty()) { cout << "No sellers available.\n"; }
                for (const auto& s : storeSellers) {
                    cout << "ID: " << s.getSellerId() << " | Store: " << s.getSellerName()
                         << " | Balance: $" << s.getAccount()->getBalance() << endl;
                }
                break;
            case 3: {
                while(viewRunning) {
                    cout << "\n--- View Details Menu ---\n";
                    cout << "1. View All Buyer Details\n";
                    cout << "2. View All Seller Details\n";
                    cout << "3. Back\n";
                    int detailChoice; cin >> detailChoice;
                    if (cin.fail()) { cout << "Invalid input." << endl; clear_input(); continue; }

                    if (detailChoice == 1) store.viewAllBuyerDetails();
                    else if (detailChoice == 2) store.viewAllSellerDetails();
                    else if (detailChoice == 3) viewRunning = false;
                    else cout << "Invalid option.\n";
                }
                break;
            }
            case 4: {
                while(searchRunning) {
                    cout << "\n--- Search Menu ---\n";
                    cout << "1. Search Buyer\n";
                    cout << "2. Search Seller\n";
                    cout << "3. Back\n";
                    int searchChoice; cin >> searchChoice;
                    if (cin.fail()) { cout << "Invalid input." << endl; clear_input(); continue; }
                    
                    if (searchChoice == 1 || searchChoice == 2) {
                        cout << "Search by:\n1. ID\n2. Name\n3. Phone\n4. Address\n";
                        int opt; string query; cin >> opt;
                        if (cin.fail() || opt < 1 || opt > 4) { cout << "Invalid option." << endl; clear_input(); continue; }
                        
                        cout << "Enter Query: "; cin >> query;
                        if (searchChoice == 1) store.searchBuyer(opt, query);
                        else store.searchSeller(opt, query);
                    } else if (searchChoice == 3) {
                        searchRunning = false;
                    } else {
                        cout << "Invalid option.\n";
                    }
                }
                break;
            }
            case 5: {
                while (createRunning) {
                    cout << "\n--- Create Menu ---\n";
                    cout << "1. Create Buyer Account\n";
                    cout << "2. Create Seller Account\n";
                    cout << "3. Back\n";
                    int createChoice; cin >> createChoice;
                    if (cin.fail()) { cout << "Invalid input." << endl; clear_input(); continue; }

                    if (createChoice == 1) { // Create Buyer
                        int id; string name, addr, phone, email; double balance;
                        cout << "Enter Buyer ID: "; cin >> id;
                        cout << "Enter Name (no space): "; cin >> name;
                        cout << "Enter Address (no space): "; cin >> addr;
                        cout << "Enter Phone: "; cin >> phone;
                        cout << "Enter Email (no space): "; cin >> email;
                        cout << "Enter Initial Balance: "; cin >> balance;
                        if (cin.fail()) { cout << "Invalid input.\n"; clear_input(); continue; }
                        
                        extern vector<BankCustomer> bankAccounts; 
                        store.createBuyerAccount(id, name, addr, phone, email, balance, bankAccounts);
                        
                    } else if (createChoice == 2) { // Create Seller
                        int sid, bid;
                        string storeName, storeAddr, storePhone, storeEmail;
                        cout << "Enter Seller ID: "; cin >> sid;
                        cout << "Link to existing Buyer ID: "; cin >> bid;
                        cout << "Enter Store Name (no space): "; cin >> storeName;
                        cout << "Enter Store Address (no space): "; cin >> storeAddr;
                        cout << "Enter Store Phone: "; cin >> storePhone;
                        cout << "Enter Store Email (no space): "; cin >> storeEmail;
                        if (cin.fail()) { cout << "Invalid input.\n"; clear_input(); continue; }

                        store.createSellerAccount(bid, sid, storeName, storeAddr, storePhone, storeEmail);

                    } else if (createChoice == 3) {
                        createRunning = false;
                    } else {
                        cout << "Invalid option.\n";
                    }
                }
                break;
            }
            case 6: { // Remove Buyer/Seller by ID
                int removeId;
                cout << "\n--- Remove Account ---\n";
                cout << "Enter Buyer/Seller ID to remove: ";
                if (!(cin >> removeId)) {
                    cout << "Invalid ID format.\n";
                    clear_input();
                    break;
                }

                if (store.removeBuyer(removeId)) {
                    cout << "Success: Buyer ID " << removeId << " and associated data removed. (Note: Linked BankCustomer object persists).\n";
                } else if (store.removeSeller(removeId)) {
                    cout << "Success: Seller ID " << removeId << " removed.\n";
                } else {
                    cout << "Error: ID " << removeId << " not found as Buyer or Seller.\n";
                }
                break;
            }
            case 7: { // System Reports (New)
                cout << "\n--- System Reports Menu ---\n";
                cout << "1. Store Reports\n";
                cout << "2. Bank Reports\n";
                cout << "3. Back\n";
                int reportChoice; cin >> reportChoice;
                if (cin.fail()) { cout << "Invalid choice.\n"; clear_input(); break; }
                
                if (reportChoice == 1) { // Store Reports
                    int k_days, m_items;
                    cout << "\n--- Store Reports ---\n";
                    cout << "1. List transactions of the latest k days (Enter k): "; cin >> k_days;
                    if (cin.fail()) { cout << "Invalid input.\n"; clear_input(); break; }
                    store.listRecentTransactions(k_days);
                    
                    cout << "2. List paid transactions but yet to be completed\n";
                    store.listPaidUncompletedTransactions(0); // 0 untuk semua seller
                    
                    cout << "3. List most m frequent item transactions (Enter m): "; cin >> m_items;
                    if (cin.fail()) { cout << "Invalid input.\n"; clear_input(); break; }
                    store.listMostFrequentItems(m_items);
                    
                    int n_users;
                    cout << "4. List n most active buyers (Enter n): "; cin >> n_users;
                    store.listMostActiveBuyers(n_users);
                    
                } else if (reportChoice == 2) { // Bank Reports
                    cout << "\n--- Bank Reports ---\n";
                    
                    cout << "1. List all transaction within a week (7 days)\n";
                    store.listBankTransactions(7);
                    
                    cout << "2. List all bank customers\n";
                    store.listAllBankCustomers();
                    
                    cout << "3. List dormant accounts (no transaction within a month - 30 days)\n";
                    store.listDormantAccounts(30);
                    
                    cout << "4. List n top users that conduct most transaction for today (Enter n): ";
                    int n_top; cin >> n_top;
                    if (cin.fail() || n_top <= 0) { cout << "Invalid input.\n"; clear_input(); break; }
                    store.listTopTransactionUsers(n_top);
                }
                break;
            }
            case 8:
                cout << "Logging out from Admin.\n";
                adminRunning = false;
                break;
            default:
                cout << "Invalid option.\n";
                break;
        }
    }
}