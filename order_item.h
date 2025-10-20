#ifndef ORDER_ITEM_H
#define ORDER_ITEM_H

#include <string>

class OrderItem {
private:
    int itemId;
    std::string name;
    double price;
    int quantity;

public:
    OrderItem(int itemId, const std::string& name, double price, int quantity)
        : itemId(itemId), name(name), price(price), quantity(quantity) {}

    int getItemId() const { return itemId; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    double getSubtotal() const { return price * quantity; }
};

#endif