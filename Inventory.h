#pragma once

#include "ItemRecord.h"

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

/**
 * @brief Class used for maintaining the inventory of all products received and
 * sold in a store.
 *
 * REQUIREMENTS:
 * 1. This class shall provide means for teacking items purchased (received).
 * 2. This class shall provide means for teacking items sold.
 * 3. This class shall generate a report of all relevant information for each
 * product, to date.
 */
class Inventory
{
  public:
  /**
   * @brief Create a new inventory.
   */
  Inventory() : m_inventory()
  {
    //  nothing to be done here
  }

  /**
   * @brief Receive an amount of an item identified by its stock-keeping unit
   * (SKU), at a given price.
   *
   * @param sku The item's SKU.
   * @param quantity The quantity to be received.
   * @param price The purchase price (in dollars).
   */
  void receive(const string& sku, const size_t quantity, const double price)
  {
    if (m_inventory.count(sku) == 0) {
      m_inventory.emplace(sku, ItemRecord());
    }
    m_inventory[sku].buy(quantity, price);
  }

  /**
   * @brief Receive an amount of an item identified by its stock-keeping unit
   * (SKU), at a given price.
   *
   * NOTE: if the amount requested exceeds the available quantity, the amount
   * sold is reduced to match the amount in stock.
   *
   * @param sku The item's SKU.
   * @param requestedQty The quantity requested to be sold.
   * @param price The sell price (in dollars).
   */
  void sell(const string& sku, const size_t requestedQty, const double price)
  {
    if (m_inventory.count(sku) == 0 || m_inventory[sku].numAvailable() == 0) {
      cerr << "No inventory available of SKU " << sku << " to sell." << endl;
      return;
    }

    size_t soldQty;
    if (m_inventory[sku].numAvailable() < requestedQty) {
      soldQty = m_inventory[sku].numAvailable();
      cerr << "Insufficient inventory available of SKU " << sku
           << " for requested amount " << requestedQty << ". Selling "
           << soldQty << " instead." << endl;
    } else {
      soldQty = requestedQty;
    }
    m_inventory[sku].sell(soldQty, price);
  }

  /**
   * @brief Generate a report of the following for each product in the
   * inventory:
   * // clang-format off
   *  a. number of items sold
   *  b. number of items in stock
   *  c. profit earned on the items sold
   *  d. cost of unsold items
   * // clang-format on
   */
  void report()
  {
    for (const auto& [sku, item] : m_inventory) {
      cout << item.numSold() << " boxes of " << sku << " have been sold"
           << endl;
      cout << item.numAvailable() << " boxes of " << sku
           << " are currently in stock" << endl;
      cout << "Profit on " << sku << " is $" << item.profit() << endl;
      cout << "Unsold stock of " << sku << " has a cost of $"
           << item.unsoldCost() << endl;
    }
  }

  private:
  unordered_map<string, ItemRecord> m_inventory;
};
