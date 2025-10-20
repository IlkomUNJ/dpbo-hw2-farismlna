// serializer.h (Sederhana)
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include "store.h" 

class Serializer {
private:
    const static std::string DATA_FILE;
    const static std::string BANK_MAP_FILE;
    
    static void reconstructPointers(Store& store, std::vector<BankCustomer>& bankAccounts);

public:
    static bool saveState(const Store& store, const std::vector<BankCustomer>& bankAccounts);
    static bool loadState(Store& store, std::vector<BankCustomer>& bankAccounts);
};

#endif // SERIALIZER_H