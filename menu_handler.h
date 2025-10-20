#ifndef MENU_HANDLERS_H
#define MENU_HANDLERS_H

#include "buyer.h"
#include "seller.h"
#include "bank_customer.h"
#include "store.h"
#include <iostream>

void handleBuyerMenu(Buyer& buyer, seller* sellerPtr, Store& store);
void handleSellerMenu(seller& seller, Store& store);
void handleBankingMenu(BankCustomer& account);
void handleAdminMenu(Store& store);

#endif