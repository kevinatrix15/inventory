#pragma once

#include "ItemRecord.h"

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Inventory
{
  public:
  Inventory() : m_inventory()
  {
  }

  void receive(const string& sku, const size_t quantity, const double price)
  {
    if (m_inventory.count(sku) == 0) {
      m_inventory.emplace(sku, ItemRecord());
    }
    m_inventory[sku].buy(quantity, price);
  }

  void sell(const string& sku, const size_t requestedQty, const double price)
  {
    if (m_inventory.count(sku) == 0 || m_inventory[sku].numAvailable() <= 0) {
      cout << "No inventory available of SKU " << sku << " to sell." << endl;
      return;
    }

    size_t soldQty;
    if (m_inventory[sku].numAvailable() < requestedQty) {
      // TODO: check for negative numAvailable
      soldQty = m_inventory[sku].numAvailable();
      // TODO: use stderr instead
      cout << "Insufficient inventory available of SKU " << sku
           << " for requested amount " << requestedQty << ". Selling "
           << soldQty << " instead." << endl;
    } else {
      soldQty = requestedQty;
    }
    m_inventory[sku].sell(soldQty, price);
  }

  void report()
  {
    for (const auto& [key, item] : m_inventory) {
      cout << item.numSold() << " boxes of " << key << " have been sold"
           << endl;
      cout << item.numAvailable() << " boxes of " << key
           << " are currently in stock" << endl;
      cout << "Profit on " << key << " is $" << item.profit() << endl;
      cout << "Unsold stock of " << key << " has a cost of $"
           << item.unsoldCost() << endl;
    }
  }

  private:
  unordered_map<string, ItemRecord> m_inventory;
};
