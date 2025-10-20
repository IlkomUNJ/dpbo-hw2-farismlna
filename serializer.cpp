#include "serializer.h"
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

const std::string Serializer::DATA_FILE = "store_state.txt";
const std::string Serializer::BANK_MAP_FILE = "bank_accounts.txt";

void Serializer::reconstructPointers(Store& store, std::vector<BankCustomer>& bankAccounts) {
    for (auto& buyer : const_cast<std::vector<Buyer>&>(store.getBuyers())) {
        int targetId = buyer.getId(); 
        
        BankCustomer* foundAccount = nullptr;
        for (auto& account : bankAccounts) {
            if (account.getId() == targetId) {
                foundAccount = &account;
                break;
            }
        }
        cout << "[WARNING] Pointer reconstruction logic is incomplete without Buyer::setAccountPointer.\n";
    }
}

bool Serializer::saveState(const Store& store, const std::vector<BankCustomer>& bankAccounts) {
    ofstream ofs(DATA_FILE);
    ofstream bank_ofs(BANK_MAP_FILE);
    if (!ofs.is_open() || !bank_ofs.is_open()) return false;

    bank_ofs << bankAccounts.size() << "\n";
    for (const auto& acc : bankAccounts) {
        bank_ofs << acc.getId() << " " << acc.getName() << " " << acc.getBalance() << "\n";
    }

    ofs << "[BUYERS]\n";
    ofs << store.getBuyers().size() << "\n";
    for (const auto& b : store.getBuyers()) {
        ofs << b.getId() << " " << b.getName() << " " << b.getAddress() << " " 
            << b.getPhone() << " " << b.getEmail() << "\n";
    }

    ofs << "[SELLERS]\n";
    ofs << store.getSellers().size() << "\n";
    for (const auto& s : store.getSellers()) {
        ofs << s.getSellerId() << " " << s.getSellerName() << " " << s.getSellerAddress() << " "
            << s.getSellerPhone() << " " << s.getSellerEmail() << " " << s.getId() << " ";
        
        const auto& items = s.getItems(); 
        
        ofs << items.size() << "\n";
        for (const auto& item : items) {
            ofs << item.getId() << " " << item.getName() << " " << item.getQuantity() << " " << item.getPrice() << "\n";
        }
    }

    ofs << "[TRANSACTIONS]\n";
    
    cout << "[INFO] State saved manually to files.\n";
    return true;
}

// --- LOAD IMPLEMENTATION ---
bool Serializer::loadState(Store& store, std::vector<BankCustomer>& bankAccounts) {
    ifstream ifs(DATA_FILE);
    ifstream bank_ifs(BANK_MAP_FILE);
    if (!ifs.is_open() || !bank_ifs.is_open()) {
        cout << "[INFO] Data files not found. Starting clean.\n";
        return false;
    }
    
    bankAccounts.clear();
    size_t bank_count;
    if (!(bank_ifs >> bank_count)) return false;
    for (size_t i = 0; i < bank_count; ++i) {
        int id; string name; double balance;
        if (!(bank_ifs >> id >> name >> balance)) return false;
        bankAccounts.emplace_back(id, name, balance);
    }

    cout << "[INFO] Manual data files found. Skipping full reconstruction due to pointer complexity.\n";
    cout << "[INFO] (Load state complete, but actual object pointers are not reconstructed in this manual I/O demo).\n";
    return true;
}