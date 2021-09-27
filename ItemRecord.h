#pragma once

#include <exception>

/**
 * @brief Class used for maintaining a record of transactions involving a
 * particular item.
 *
 * REQUIREMENTS:
 * 1. This class shall provide means for tracking and accessing purchases of an
 * item.
 * 2. This class shall provide means for tracking and acessing sales of an item.
 * 3. This class shall provide means for tracking and accessing available
 * inventory of an item.
 * 4. This class shall provide the profit (or loss) to date, using the weighted
 * average cost of the item.
 * 5. This class shall provide the cost of unsold stock to date, using the
 * weighted average cost of the item.
 */
class ItemRecord
{
  public:
  /**
   * @brief Create a new ItemRecord.
   */
  ItemRecord()
      : m_numBought(0),
        m_numSold(0),
        m_avgPurchasePrice(0.0),
        m_avgSalesPrice(0.0)
  {
    //  nothing to be done here
  }

  /**
   * @brief Buy a quantity of the item at a given price.
   *
   * @param quantity The quantity to be bought.
   * @param price The purchase price (in dollars).
   */
  void buy(const size_t quantity, const double price)
  {
    updateAverage(quantity, price, m_numBought, m_avgPurchasePrice);
    m_numBought += quantity;
  }

  /**
   * @brief Sell a quantity of the item at a given price.
   *
   * @param quantity The quantity to be sold.
   * @param price The sales price (in dollars).
   *
   * @throw An exception if the quantity requested exceeds the number
   * available.
   */
  void sell(const size_t quantity, const double price)
  {
    if (quantity > this->numAvailable()) {
      throw std::exception("Can not sell more items than available.");
    }

    updateAverage(quantity, price, m_numSold, m_avgSalesPrice);
    m_numSold += quantity;
  }

  /**
   * @brief The available inventory of the item.
   *
   * @return The available inventory.
   */
  int numAvailable() const
  {
    return static_cast<int>(m_numBought) - static_cast<int>(m_numSold);
  }

  /**
   * @brief Get the profit (or loss) to date, using the current weighted average
   * of the sales and purchases.
   *
   * @return The profit (or loss) to date (in dollars).
   */
  double profit() const
  {
    return m_numSold * (m_avgSalesPrice - m_avgPurchasePrice);
  }

  /**
   * @brief Get the cost of unsold inventory of this item.
   *
   * @return The unsold cost (in dollars).
   */
  double unsoldCost() const
  {
    return (this->numAvailable() * m_avgPurchasePrice);
  }

  /**
   * @brief Get the number of items bought to date.
   *
   * @return The number of items bought to date.
   */
  size_t numBought() const
  {
    return m_numBought;
  }

  /**
   * @brief Get the number of items sold to date.
   *
   * @return The number of items sold to date.
   */
  size_t numSold() const
  {
    return m_numSold;
  }

  private:
  /**
   * @brief Update the weighted average, using a method found described here,
   * for example: https://stackoverflow.com/questions/28820904/how-to-efficiently-compute-average-on-the-fly-moving-average.
   *
   * @param quantity The quantity to add to the average at a given value.
   * @param val The value.
   * @param startCount The number of items considered in the average, to date.
   * @param currAvgRef The current average value to date, to be updated.
   */
  void updateAverage(const size_t quantity,
                     const double val,
                     const size_t startCount,
                     double& currAvgRef) const
  {
    const size_t totalCount = startCount + quantity;
    for (size_t count = startCount; count < totalCount; ++count) {
      currAvgRef += (val - currAvgRef) / (count + 1);
    }
  }

  size_t m_numBought;
  size_t m_numSold;
  double m_avgPurchasePrice;
  double m_avgSalesPrice;
};
